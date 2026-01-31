#include "game.h"

#include "engine/headless_gl.h"

#include "components.h"
#include "input_mapping.h"
#include "log.h"
#include "preload.h"
#include "render_backend.h"
#include "settings.h"
#include "systems/ExampleScreenRegistry.h"
#include "systems/RenderRenderTexture.h"
#include "systems/RenderScreenHUD.h"
#include "systems/RenderSystemHelpers.h"
#include "systems/RenderTestFeedback.h"
#include "systems/SetupSimpleButtonTest.h"
#include "systems/SetupTabbingTest.h"
#include "systems/TestSystem.h"
#include "systems/UpdateRenderTexture.h"
#include "testing/e2e_integration.h"
#include "testing/screenshot_validation.h"
#include "testing/test_app.h"
#include "testing/test_input.h"
#include "testing/test_macros.h"
#include "testing/tests/SimpleButtonClickTest.h"
#include <afterhours/src/plugins/files.h>

#include <afterhours/src/plugins/animation.h>
#include <afterhours/src/plugins/modal.h>
#include <afterhours/src/plugins/toast.h>
#include <afterhours/src/plugins/e2e_testing/e2e_testing.h>
#include <afterhours/src/plugins/ui/validation_systems.h>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <thread>

// Configure UI validation for design rule enforcement
// Call this before running any screens
static void configure_validation() {
  auto &config =
      afterhours::ui::imm::UIStylingDefaults::get().get_validation_config_mut();
  config.mode = afterhours::ui::ValidationMode::Warn;
  config.enforce_screen_bounds = true;
  config.enforce_child_containment = true;
  config.enforce_contrast_ratio = true;
  config.enforce_min_font_size = true;
  config.safe_area_margin = 16.0f;
  config.min_font_size = 12.0f;
  config.min_contrast_ratio = 4.5f;
  config.highlight_violations = true; // Draw red borders on violations
}

#ifdef AFTER_HOURS_ENABLE_MCP
#include "engine/input_injector.h"
#include <afterhours/src/plugins/mcp_server.h>
#include <nlohmann/json.hpp>
#include <sstream>

#ifdef AFTER_HOURS_ENABLE_MCP
extern bool g_mcp_mode;
extern int g_saved_stdout_fd;
#endif

extern bool g_headless_mode;
extern std::string g_headless_output_dir;

namespace {

// Build JSON representation of UI component tree
nlohmann::json build_ui_tree_json(afterhours::Entity &entity,
                                  afterhours::ui::UIComponent &cmp) {
  nlohmann::json node;
  node["id"] = cmp.id;

  if (entity.has<afterhours::ui::UIComponentDebug>()) {
    node["name"] = entity.get<afterhours::ui::UIComponentDebug>().name();
  }

  node["rect"] = {{"x", cmp.rect().x},
                  {"y", cmp.rect().y},
                  {"width", cmp.rect().width},
                  {"height", cmp.rect().height}};

  node["computed"] = {{"width", cmp.computed[afterhours::ui::Axis::X]},
                      {"height", cmp.computed[afterhours::ui::Axis::Y]}};

  node["relative_pos"] = {{"x", cmp.computed_rel[afterhours::ui::Axis::X]},
                          {"y", cmp.computed_rel[afterhours::ui::Axis::Y]}};

  node["padding"] = {
      {"left", cmp.computed_padd[afterhours::ui::Axis::left]},
      {"top", cmp.computed_padd[afterhours::ui::Axis::top]},
      {"right", cmp.computed_padd[afterhours::ui::Axis::right]},
      {"bottom", cmp.computed_padd[afterhours::ui::Axis::bottom]}};

  node["margin"] = {
      {"left", cmp.computed_margin[afterhours::ui::Axis::left]},
      {"top", cmp.computed_margin[afterhours::ui::Axis::top]},
      {"right", cmp.computed_margin[afterhours::ui::Axis::right]},
      {"bottom", cmp.computed_margin[afterhours::ui::Axis::bottom]}};

  node["absolute"] = cmp.absolute;
  node["visible"] = cmp.was_rendered_to_screen;

  // Add children recursively
  nlohmann::json children_arr = nlohmann::json::array();
  for (afterhours::EntityID child_id : cmp.children) {
    try {
      auto &child_ent = afterhours::ui::AutoLayout::to_ent_static(child_id);
      auto &child_cmp = afterhours::ui::AutoLayout::to_cmp_static(child_id);
      children_arr.push_back(build_ui_tree_json(child_ent, child_cmp));
    } catch (...) {
      // Skip invalid children
    }
  }
  node["children"] = children_arr;

  return node;
}

std::string dump_ui_tree() {
  nlohmann::json result;
  result["tree"] = nlohmann::json::array();

  // Find all root UI components (those with AutoLayoutRoot)
  auto roots = afterhours::EntityQuery()
                   .whereHasComponent<afterhours::ui::AutoLayoutRoot>()
                   .whereHasComponent<afterhours::ui::UIComponent>()
                   .gen();

  for (auto &entity_ref : roots) {
    afterhours::Entity &entity = entity_ref.get();
    auto &cmp = entity.get<afterhours::ui::UIComponent>();
    result["tree"].push_back(build_ui_tree_json(entity, cmp));
  }

  return result.dump(2); // Pretty print with 2-space indent
}

std::vector<uint8_t> capture_screenshot_png() {
  raylib::Image image = raylib::LoadImageFromTexture(mainRT.texture);
  if (image.data == nullptr) {
    return {};
  }
  raylib::ImageFlipVertical(&image);

  int file_size = 0;
  unsigned char *png_data =
      raylib::ExportImageToMemory(image, ".png", &file_size);
  raylib::UnloadImage(image);

  if (png_data == nullptr || file_size <= 0) {
    return {};
  }

  std::vector<uint8_t> result(png_data, png_data + file_size);
  raylib::MemFree(png_data);
  return result;
}

void init_mcp() {
  if (!g_mcp_mode) {
    return;
  }

  afterhours::mcp::MCPConfig config;
  config.get_screen_size = []() {
    return std::make_pair(Settings::get().get_screen_width(),
                          Settings::get().get_screen_height());
  };
  config.capture_screenshot = capture_screenshot_png;
  config.mouse_move = [](int x, int y) {
    input_injector::set_mouse_position(x, y);
  };
  config.mouse_click = [](int x, int y, int /* button */) {
    raylib::Rectangle rect{static_cast<float>(x), static_cast<float>(y), 1.0f,
                           1.0f};
    input_injector::schedule_mouse_click_at(rect);
    input_injector::inject_scheduled_click();
  };
  config.key_down = [](int keycode) { input_injector::set_key_down(keycode); };
  config.key_up = [](int keycode) { input_injector::set_key_up(keycode); };
  config.dump_ui_tree = dump_ui_tree;

  afterhours::mcp::init(config, g_saved_stdout_fd);
}
} // namespace
#endif

bool running = true;
raylib::RenderTexture2D mainRT;
raylib::RenderTexture2D screenRT;
raylib::Font uiFont;

void game() {
  // Configure UI validation for design rule enforcement
  configure_validation();

  mainRT = raylib::LoadRenderTexture(Settings::get().get_screen_width(),
                                     Settings::get().get_screen_height());
  screenRT = raylib::LoadRenderTexture(Settings::get().get_screen_width(),
                                       Settings::get().get_screen_height());
  uiFont = afterhours::load_font_from_file(
      afterhours::files::get_resource_path("fonts", "Gaegu-Bold.ttf")
          .string()
          .c_str());

  afterhours::SystemManager systems;

  {
    afterhours::window_manager::enforce_singletons(systems);
    afterhours::ui::enforce_singletons<InputAction>(systems);
    afterhours::input::enforce_singletons(systems);
    afterhours::toast::enforce_singletons(systems);
    afterhours::modal::enforce_singletons(systems);
  }

  TestSystem *test_system_ptr = nullptr;

  {
    afterhours::input::register_update_systems(systems);
    afterhours::window_manager::register_update_systems(systems);
    afterhours::toast::register_update_systems(systems);
    afterhours::toast::register_layout_systems<InputAction>(systems);
    afterhours::modal::register_update_systems<InputAction>(systems);

    auto test_system = std::make_unique<TestSystem>();
    test_system_ptr = test_system.get();
    systems.register_update_system(std::move(test_system));
  }

  {
    systems.register_render_system(std::make_unique<BeginWorldRender>());
    afterhours::modal::register_render_systems<InputAction>(systems);
    afterhours::ui::register_batched_render_systems<InputAction>(
        systems, InputAction::ToggleUILayoutDebug);
    systems.register_render_system(std::make_unique<EndWorldRender>());
    systems.register_render_system(
        std::make_unique<BeginPostProcessingRender>());
    systems.register_render_system(std::make_unique<RenderRenderTexture>());
    systems.register_render_system(std::make_unique<EndDrawing>());
  }

  afterhours::ui::validation::register_systems<InputAction>(systems);

  while (running && !raylib::WindowShouldClose()) {
    if (raylib::IsKeyPressed(raylib::KEY_ESCAPE)) {
      running = false;
    }
    float dt = raylib::GetFrameTime();
    systems.run(dt);

    if (test_system_ptr && test_system_ptr->is_complete()) {
      std::string error = test_system_ptr->get_error();
      if (!error.empty()) {
        std::cout << "Test '" << test_system_ptr->get_test_name()
                  << "' failed: " << error << std::endl;
        running = false;
      } else {
        std::cout << "Test '" << test_system_ptr->get_test_name() << "' passed!"
                  << std::endl;
        running = false;
      }
    }
  }
}

void run_test(const std::string &test_name, bool slow_mode, bool hold_on_end) {
  configure_validation();

  TestRegistry &registry = TestRegistry::get();
  auto it = registry.tests.find(test_name);
  if (it == registry.tests.end()) {
    std::cout << "Test '" << test_name << "' not found" << std::endl;
    return;
  }

  test_input::slow_test_mode = slow_mode;
  test_input::test_mode = true;

  mainRT = raylib::LoadRenderTexture(Settings::get().get_screen_width(),
                                     Settings::get().get_screen_height());
  screenRT = raylib::LoadRenderTexture(Settings::get().get_screen_width(),
                                       Settings::get().get_screen_height());
  uiFont = afterhours::load_font_from_file(
      afterhours::files::get_resource_path("fonts", "Gaegu-Bold.ttf")
          .string()
          .c_str());

  afterhours::SystemManager systems;

  {
    afterhours::window_manager::enforce_singletons(systems);
    afterhours::ui::enforce_singletons<InputAction>(systems);
    afterhours::input::enforce_singletons(systems);
    afterhours::toast::enforce_singletons(systems);
    afterhours::modal::enforce_singletons(systems);
  }

  TestSystem *test_system_ptr = nullptr;

  {
    afterhours::input::register_update_systems(systems);
    afterhours::window_manager::register_update_systems(systems);
    afterhours::toast::register_update_systems(systems);
    afterhours::toast::register_layout_systems<InputAction>(systems);
    afterhours::modal::register_update_systems<InputAction>(systems);

    systems.register_update_system(std::make_unique<UpdateRenderTexture>());

    auto test_system = std::make_unique<TestSystem>();
    test_system_ptr = test_system.get();
    systems.register_update_system(std::move(test_system));

    // Register UI pre-update systems (clears, resets)
    afterhours::ui::register_before_ui_updates<InputAction>(systems);

    // Register the test UI so buttons exist when processed
    // Check if test name starts with a registered screen name
    bool screen_found = false;
    for (const auto &screen_name :
         ExampleScreenRegistry::get().get_screen_names()) {
      if (test_name.find(screen_name + "_") == 0 || test_name == screen_name) {
        auto screen = ExampleScreenRegistry::get().create_screen(screen_name);
        if (screen) {
          // Set as current screen so ScreenSystem runs
          g_current_screen = screen.get();
          systems.register_update_system(std::move(screen));
          screen_found = true;
          break;
        }
      }
    }

    if (!screen_found) {
      // Fall back to built-in test setups
      if (test_name == "tabbing") {
        systems.register_update_system(std::make_unique<SetupTabbingTest>());
      } else {
        systems.register_update_system(
            std::make_unique<SetupSimpleButtonTest>());
      }
    }

    // Register UI post-update systems (HandleClicks, HandleTabbing, layout,
    // etc.)
    afterhours::ui::register_after_ui_updates<InputAction>(systems);
  }

  {
    systems.register_render_system(std::make_unique<BeginWorldRender>());
    afterhours::modal::register_render_systems<InputAction>(systems);
    afterhours::ui::register_batched_render_systems<InputAction>(
        systems, InputAction::ToggleUILayoutDebug);
    systems.register_render_system(std::make_unique<EndWorldRender>());
    systems.register_render_system(
        std::make_unique<BeginPostProcessingRender>());
    systems.register_render_system(std::make_unique<RenderRenderTexture>());

    systems.register_render_system(std::make_unique<RenderTestFeedback>());
    systems.register_render_system(std::make_unique<EndDrawing>());
  }

  afterhours::ui::validation::register_systems<InputAction>(systems);

  TestApp test = it->second();
  test_system_ptr->set_test(test_name, std::move(test));

  while (running && !raylib::WindowShouldClose()) {
    if (raylib::IsKeyPressed(raylib::KEY_ESCAPE)) {
      running = false;
    }
    float dt = raylib::GetFrameTime();
    systems.run(dt);

    if (test_input::slow_test_mode) {
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    if (test_system_ptr && test_system_ptr->is_complete()) {
      std::string error = test_system_ptr->get_error();
      if (!error.empty()) {
        std::cout << "Test '" << test_system_ptr->get_test_name()
                  << "' failed: " << error << std::endl;
        if (!hold_on_end) {
          running = false;
        }
      } else {
        std::cout << "Test '" << test_system_ptr->get_test_name() << "' passed!"
                  << std::endl;
        if (!hold_on_end) {
          running = false;
        }
      }
      if (hold_on_end) {
        std::cout << "Test complete. Window will stay open. Press ESC to exit."
                  << std::endl;
      }
    }
  }
}

struct ScreenCyclerSystem : afterhours::System<> {
  afterhours::SystemBase *current_screen = nullptr;
  afterhours::SystemManager *systems_ptr = nullptr;

  virtual void once(const float dt) override {
    if (!current_screen || !systems_ptr) {
      return;
    }
    current_screen->once(dt);
  }

  virtual void for_each(afterhours::Entity &entity, const float dt) override {
    if (!current_screen) {
      return;
    }
    current_screen->for_each(entity, dt);
  }

  virtual void for_each(const afterhours::Entity &entity,
                        const float dt) const override {
    if (!current_screen) {
      return;
    }
    current_screen->for_each(entity, dt);
  }

  virtual void for_each_derived(afterhours::Entity &entity,
                                const float dt) override {
    if (!current_screen) {
      return;
    }
    current_screen->for_each_derived(entity, dt);
  }

  virtual void for_each_derived(const afterhours::Entity &entity,
                                const float dt) const override {
    if (!current_screen) {
      return;
    }
    current_screen->for_each_derived(entity, dt);
  }

  virtual void after(const float dt) override {
    if (!current_screen) {
      return;
    }
    current_screen->after(dt);
  }
};

void run_screen_demo(const std::string &screen_name, bool /* hold_on_end */) {
  configure_validation();

  mainRT = raylib::LoadRenderTexture(Settings::get().get_screen_width(),
                                     Settings::get().get_screen_height());
  screenRT = raylib::LoadRenderTexture(Settings::get().get_screen_width(),
                                       Settings::get().get_screen_height());

  uiFont = afterhours::load_font_from_file(
      afterhours::files::get_resource_path("fonts", "Gaegu-Bold.ttf")
          .string()
          .c_str());

#ifdef AFTER_HOURS_ENABLE_MCP
  init_mcp();
#endif

  std::vector<std::string> screen_names =
      ExampleScreenRegistry::get().get_screen_names();
  if (screen_names.empty()) {
    std::cerr << "ERROR: No screens available" << std::endl;
    return;
  }

  int current_screen_index = 0;
  for (size_t i = 0; i < screen_names.size(); i++) {
    if (screen_names[i] == screen_name) {
      current_screen_index = static_cast<int>(i);
      break;
    }
  }

  afterhours::SystemManager systems;

  {
    afterhours::window_manager::enforce_singletons(systems);
    afterhours::ui::enforce_singletons<InputAction>(systems);
    afterhours::input::enforce_singletons(systems);
    afterhours::toast::enforce_singletons(systems);
    afterhours::modal::enforce_singletons(systems);
  }

  {
    afterhours::input::register_update_systems(systems);
    afterhours::window_manager::register_update_systems(systems);
    afterhours::toast::register_update_systems(systems);
    afterhours::toast::register_layout_systems<InputAction>(systems);
    afterhours::modal::register_update_systems<InputAction>(systems);

    systems.register_update_system(std::make_unique<UpdateRenderTexture>());
  }

  {
    systems.register_render_system(std::make_unique<BeginWorldRender>());
    afterhours::modal::register_render_systems<InputAction>(systems);
    afterhours::ui::register_batched_render_systems<InputAction>(
        systems, InputAction::ToggleUILayoutDebug);
    systems.register_render_system(std::make_unique<EndWorldRender>());
    systems.register_render_system(
        std::make_unique<BeginPostProcessingRender>());
    systems.register_render_system(std::make_unique<RenderRenderTexture>());
    systems.register_render_system(std::make_unique<RenderScreenHUD>());
    systems.register_render_system(std::make_unique<EndDrawing>());
  }

  // Initialize HUD state
  ScreenHUDState::total_screens = static_cast<int>(screen_names.size());

  std::unique_ptr<afterhours::SystemBase> current_screen_system;
  std::unique_ptr<ScreenCyclerSystem> cycler_system =
      std::make_unique<ScreenCyclerSystem>();
  cycler_system->systems_ptr = &systems;
  ScreenCyclerSystem *cycler_ptr = cycler_system.get();

  auto load_screen = [&](int index) {
    if (index < 0 || index >= static_cast<int>(screen_names.size())) {
      return;
    }

    // Reset the UIContext focus state and mark old UI as not rendered
    // This prevents tabbing/clicking on elements from old screens
    auto *ui_context = afterhours::EntityHelper::get_singleton_cmp<
        afterhours::ui::UIContext<InputAction>>();
    if (ui_context) {
      ui_context->reset();
    }

    // Mark all UI entities as not rendered to prevent input handling
    for (const auto &e : afterhours::EntityHelper::get_entities()) {
      if (!e)
        continue;
      if (e->has<afterhours::ui::UIComponent>()) {
        e->get<afterhours::ui::UIComponent>().was_rendered_to_screen = false;
      }
    }

    std::string new_screen_name = screen_names[index];
    current_screen_system =
        ExampleScreenRegistry::get().create_screen(new_screen_name);
    if (!current_screen_system) {
      std::cerr << "ERROR: Failed to create screen: " << new_screen_name
                << std::endl;
      return;
    }
    cycler_ptr->current_screen = current_screen_system.get();
    g_current_screen = current_screen_system.get();

    // Update HUD state
    ScreenHUDState::current_screen_name = new_screen_name;
    ScreenHUDState::current_screen_description =
        ExampleScreenRegistry::get().get_screen_description(new_screen_name);
    ScreenHUDState::current_index = index;

#ifdef AFTER_HOURS_ENABLE_MCP
    if (g_mcp_mode) {
      std::cerr << "Switched to screen: " << new_screen_name << std::endl;
    } else {
      std::cout << "Switched to screen: " << new_screen_name << std::endl;
    }
#else
    std::cout << "Switched to screen: " << new_screen_name << std::endl;
#endif
  };

  {
    afterhours::ui::register_before_ui_updates<InputAction>(systems);

    load_screen(current_screen_index);
    if (!current_screen_system) {
      std::cerr << "ERROR: Failed to create initial screen: " << screen_name
                << std::endl;
      return;
    }
    systems.register_update_system(std::move(cycler_system));

    afterhours::ui::register_after_ui_updates<InputAction>(systems);
  }

  afterhours::ui::validation::register_systems<InputAction>(systems);

  while (running && !raylib::WindowShouldClose()) {
#ifdef AFTER_HOURS_ENABLE_MCP
    if (g_mcp_mode) {
      // Process any pending input injections BEFORE systems run
      input_injector::update_key_hold(raylib::GetFrameTime());

      // Check if exit was requested via MCP
      if (afterhours::mcp::exit_requested()) {
        running = false;
        break;
      }
    }
#endif

    if (raylib::IsKeyPressed(raylib::KEY_ESCAPE)) {
      running = false;
    }

    // Screen navigation: . or PageDown = next, , or PageUp = previous
    if (raylib::IsKeyPressed(raylib::KEY_PAGE_DOWN) ||
        raylib::IsKeyPressed(raylib::KEY_PERIOD)) {
      current_screen_index = (current_screen_index + 1) % screen_names.size();
      load_screen(current_screen_index);
    }
    if (raylib::IsKeyPressed(raylib::KEY_PAGE_UP) ||
        raylib::IsKeyPressed(raylib::KEY_COMMA)) {
      current_screen_index = static_cast<int>(
          (current_screen_index - 1 + static_cast<int>(screen_names.size())) %
          static_cast<int>(screen_names.size()));
      load_screen(current_screen_index);
    }

    float dt = raylib::GetFrameTime();
    systems.run(dt);

#ifdef AFTER_HOURS_ENABLE_MCP
    if (g_mcp_mode) {
      // Release click state AFTER systems processed it
      input_injector::release_scheduled_click();
      input_injector::reset_frame();

      // Process MCP commands AFTER rendering so screenshots capture the current
      // frame New clicks scheduled here will be processed in the NEXT frame
      afterhours::mcp::update();
    }
#endif
  }

#ifdef AFTER_HOURS_ENABLE_MCP
  if (g_mcp_mode) {
    afterhours::mcp::shutdown();
  }
#endif
}

int run_e2e_tests(const e2e::E2EArgs &args,
                  afterhours::testing::E2ERunner &runner) {
  configure_validation();

  // Set global update-baselines flag
  screenshot_validation::set_update_baselines(args.update_baselines);
  if (screenshot_validation::is_update_baselines()) {
    log_info("[E2E] Running in update-baselines mode - screenshots will be "
             "saved as new baselines");
  }

  // Enable test mode for both test_input systems
  test_input::test_mode = true;
  afterhours::testing::test_input::detail::test_mode = true;

  mainRT = raylib::LoadRenderTexture(Settings::get().get_screen_width(),
                                     Settings::get().get_screen_height());
  screenRT = raylib::LoadRenderTexture(Settings::get().get_screen_width(),
                                       Settings::get().get_screen_height());

  uiFont = afterhours::load_font_from_file(
      afterhours::files::get_resource_path("fonts", "Gaegu-Bold.ttf")
          .string()
          .c_str());

  std::vector<std::string> screen_names =
      ExampleScreenRegistry::get().get_screen_names();
  if (screen_names.empty()) {
    std::cerr << "ERROR: No screens available for E2E testing" << std::endl;
    return 1;
  }

  int current_screen_index = 0;

  afterhours::SystemManager systems;

  {
    afterhours::window_manager::enforce_singletons(systems);
    afterhours::ui::enforce_singletons<InputAction>(systems);
    afterhours::input::enforce_singletons(systems);
    afterhours::toast::enforce_singletons(systems);
    afterhours::modal::enforce_singletons(systems);
  }

  {
    afterhours::input::register_update_systems(systems);
    afterhours::window_manager::register_update_systems(systems);
    afterhours::toast::register_update_systems(systems);
    afterhours::toast::register_layout_systems<InputAction>(systems);
    afterhours::modal::register_update_systems<InputAction>(systems);
    systems.register_update_system(std::make_unique<UpdateRenderTexture>());
  }

  {
    systems.register_render_system(std::make_unique<BeginWorldRender>());
    afterhours::modal::register_render_systems<InputAction>(systems);
    afterhours::ui::register_batched_render_systems<InputAction>(
        systems, InputAction::ToggleUILayoutDebug);
    systems.register_render_system(std::make_unique<EndWorldRender>());
    systems.register_render_system(
        std::make_unique<BeginPostProcessingRender>());
    systems.register_render_system(std::make_unique<RenderRenderTexture>());
    systems.register_render_system(std::make_unique<RenderScreenHUD>());
    systems.register_render_system(std::make_unique<EndDrawing>());
  }

  // Initialize HUD state
  ScreenHUDState::total_screens = static_cast<int>(screen_names.size());

  std::unique_ptr<afterhours::SystemBase> current_screen_system;
  std::unique_ptr<ScreenCyclerSystem> cycler_system =
      std::make_unique<ScreenCyclerSystem>();
  cycler_system->systems_ptr = &systems;
  ScreenCyclerSystem *cycler_ptr = cycler_system.get();

  auto load_screen = [&](int index) {
    if (index < 0 || index >= static_cast<int>(screen_names.size())) {
      return;
    }

    auto *ui_context = afterhours::EntityHelper::get_singleton_cmp<
        afterhours::ui::UIContext<InputAction>>();
    if (ui_context) {
      ui_context->reset();
    }

    for (const auto &e : afterhours::EntityHelper::get_entities()) {
      if (!e)
        continue;
      if (e->has<afterhours::ui::UIComponent>()) {
        e->get<afterhours::ui::UIComponent>().was_rendered_to_screen = false;
      }
    }

    std::string new_screen_name = screen_names[index];
    current_screen_system =
        ExampleScreenRegistry::get().create_screen(new_screen_name);
    if (!current_screen_system) {
      std::cerr << "ERROR: Failed to create screen: " << new_screen_name
                << std::endl;
      return;
    }
    cycler_ptr->current_screen = current_screen_system.get();
    g_current_screen = current_screen_system.get();

    ScreenHUDState::current_screen_name = new_screen_name;
    ScreenHUDState::current_screen_description =
        ExampleScreenRegistry::get().get_screen_description(new_screen_name);
    ScreenHUDState::current_index = index;

    std::cout << "[E2E] Loaded screen: " << new_screen_name << std::endl;
  };

  {
    afterhours::ui::register_before_ui_updates<InputAction>(systems);

    load_screen(current_screen_index);
    if (!current_screen_system) {
      std::cerr << "ERROR: Failed to create initial screen" << std::endl;
      return 1;
    }
    systems.register_update_system(std::move(cycler_system));

    afterhours::ui::register_after_ui_updates<InputAction>(systems);
  }

  // Reset callback for per-script cleanup
  auto reset_fn = []() {
    // Clear input + visible text
    afterhours::testing::test_input::reset_all();
    afterhours::testing::VisibleTextRegistry::instance().clear();

    // Reset UI focus state
    auto *ui_context = afterhours::EntityHelper::get_singleton_cmp<
        afterhours::ui::UIContext<InputAction>>();
    if (ui_context) {
      ui_context->reset();
    }

    // Note: avoid deleting UI entities mid-frame; load_screen already
    // marks old UI as not rendered when switching screens.
  };

  // Register E2E command handlers
  afterhours::testing::register_builtin_handlers(systems);
  systems.register_update_system(
      std::make_unique<afterhours::testing::HandleResetTestStateCommand>(
          reset_fn));
  runner.set_reset_callback(reset_fn);

  // Register 'action' command for semantic UI actions (WidgetNext, etc.)
  struct HandleActionCommand
      : afterhours::System<afterhours::testing::PendingE2ECommand> {
    void for_each_with(afterhours::Entity &,
                       afterhours::testing::PendingE2ECommand &cmd,
                       float) override {
      if (cmd.is_consumed() || !cmd.is("action"))
        return;
      if (!cmd.has_args(1)) {
        cmd.fail("action requires action name");
        return;
      }

      auto *ctx =
          afterhours::EntityHelper::get_singleton_cmp<afterhours::ui::UIContext<
              InputAction>>();
      if (!ctx) {
        cmd.fail("UIContext not found");
        return;
      }

      auto action = magic_enum::enum_cast<InputAction>(cmd.arg(0));
      if (!action) {
        cmd.fail("Unknown action: " + cmd.arg(0));
        return;
      }

      ctx->last_action = *action;
      cmd.consume();
    }
  };
  systems.register_update_system(std::make_unique<HandleActionCommand>());

  // Register 'focus_element' command to focus UI element by debug_name
  struct HandleFocusElementCommand
      : afterhours::System<afterhours::testing::PendingE2ECommand> {
    void for_each_with(afterhours::Entity &,
                       afterhours::testing::PendingE2ECommand &cmd,
                       float) override {
      if (cmd.is_consumed() || !cmd.is("focus_element"))
        return;
      if (!cmd.has_args(1)) {
        cmd.fail("focus_element requires element debug_name");
        return;
      }

      auto *ctx =
          afterhours::EntityHelper::get_singleton_cmp<afterhours::ui::UIContext<
              InputAction>>();
      if (!ctx) {
        cmd.fail("UIContext not found");
        return;
      }

      const std::string &target_name = cmd.arg(0);

      // Search for entity with matching debug_name
      auto opt = afterhours::EntityQuery()
                     .whereHasComponent<afterhours::ui::UIComponentDebug>()
                     .whereLambda([&](const afterhours::Entity &e) {
                       return e.get<afterhours::ui::UIComponentDebug>().name() ==
                              target_name;
                     })
                     .gen_first();

      if (!opt) {
        cmd.fail("Element not found: " + target_name);
        return;
      }

      afterhours::Entity &target = opt.asE();
      int focus_target_id = target.id;

      // For text_input components, the HasTextInputState is on the parent
      // but focus should go to a child field entity. Check if this entity
      // has UIComponent with children and find the focusable field.
      if (target.has<afterhours::ui::UIComponent>()) {
        auto &ui_cmp = target.get<afterhours::ui::UIComponent>();
        // Look for a child entity that's in the focus cluster (the field)
        // Use the LAST matching child since text_input puts field after label
        for (int child_id : ui_cmp.children) {
          auto child_opt =
              afterhours::EntityQuery().whereID(child_id).gen_first();
          if (child_opt &&
              child_opt.asE().has<afterhours::ui::InFocusCluster>()) {
            focus_target_id = child_id; // Keep updating to get the LAST one
          }
        }
      }

      // Clear any pending input before switching focus
      afterhours::testing::test_input::clear_queue();
      ctx->set_focus(focus_target_id);

      cmd.consume();
    }
  };
  systems.register_update_system(std::make_unique<HandleFocusElementCommand>());

  // Register goto_screen command with callback
  auto goto_screen_cmd =
      std::make_unique<e2e_commands::HandleGotoScreenCommand>();
  goto_screen_cmd->goto_screen_fn = [&](const std::string &name) {
    for (int i = 0; i < static_cast<int>(screen_names.size()); i++) {
      if (screen_names[i] == name) {
        current_screen_index = i;
        load_screen(i);
        return;
      }
    }
    log_error("[E2E] Screen not found: {}", name);
  };
  systems.register_update_system(std::move(goto_screen_cmd));

  // Register next_screen and prev_screen commands
  systems.register_update_system(
      std::make_unique<e2e_commands::HandleNextScreenCommand>());
  systems.register_update_system(
      std::make_unique<e2e_commands::HandlePrevScreenCommand>());

  // Register screenshot command
  systems.register_update_system(
      std::make_unique<afterhours::testing::HandleScreenshotCommand>(
          [](const std::string &name) {
            std::string path = "/tmp/e2e_screenshot_" + name + ".png";
            screenshot_validation::save_screenshot_to(path);
            log_info("[E2E] Screenshot saved: {}", path);
          }));

  // Register validate_screen command for visual regression testing
  systems.register_update_system(
      std::make_unique<e2e_commands::HandleValidateScreenCommand>(
          screenshot_validation::validate_screen_against_baseline));

  // Register reset_test_state command to clear UI between scripts
  systems.register_update_system(
      std::make_unique<afterhours::testing::HandleResetTestStateCommand>(
          []() { reset_e2e_state(); }));

  afterhours::testing::register_unknown_handler(systems);
  afterhours::testing::register_cleanup(systems);

  afterhours::ui::validation::register_systems<InputAction>(systems);

  // Main E2E loop with visual rendering
  while (running && !raylib::WindowShouldClose() && !runner.is_finished()) {
    if (raylib::IsKeyPressed(raylib::KEY_ESCAPE)) {
      running = false;
      break;
    }

    float dt = raylib::GetFrameTime();

    // Advance E2E runner (dispatches commands)
    runner.tick(dt);

    // Run game systems (input, UI, rendering, E2E command handlers)
    // Note: E2E handlers (update) run first, then rendering populates registry
    // The visible text registry accumulates text from render; expect_text
    // checks in the next frame after rendering has populated it
    systems.run(dt);

    // Fail fast on first error
    if (afterhours::testing::get_command_error_count() > 0) {
      log_warn("Stopping test early due to error");
      break;
    }

    // Screen navigation via test input (AFTER E2E commands are processed)
    // Check the afterhours input_injector since E2E handlers use that
    namespace ah_input = afterhours::testing::input_injector;
    if (ah_input::consume_press(afterhours::keys::PAGE_DOWN) ||
        ah_input::consume_press(afterhours::keys::PERIOD)) {
      current_screen_index =
          (current_screen_index + 1) % static_cast<int>(screen_names.size());
      load_screen(current_screen_index);
    }
    if (ah_input::consume_press(afterhours::keys::PAGE_UP) ||
        ah_input::consume_press(afterhours::keys::COMMA)) {
      current_screen_index =
          (current_screen_index - 1 + static_cast<int>(screen_names.size())) %
          static_cast<int>(screen_names.size());
      load_screen(current_screen_index);
    }

    // Reset test input state for next frame
    afterhours::testing::test_input::reset_frame();
  }

  runner.print_results();
  Settings::get().write_save_file();

  return runner.has_failed() ? 1 : 0;
}

void reset_e2e_state() {
  // Reset UI focus/input state and clear UI entities between scripts.
  if (auto *ui_context = afterhours::EntityHelper::get_singleton_cmp<
          afterhours::ui::UIContext<InputAction>>()) {
    ui_context->reset();
  }

  // Clear text registry and input queues
  afterhours::testing::VisibleTextRegistry::instance().clear();
  afterhours::testing::test_input::clear_queue();
  afterhours::testing::test_input::reset_frame();
  afterhours::testing::input_injector::reset_frame();

  // Mark all UI entities as not rendered to avoid stale focus/click handling.
  for (const auto &e : afterhours::EntityHelper::get_entities()) {
    if (!e)
      continue;
    if (e->has<afterhours::ui::UIComponent>()) {
      e->get<afterhours::ui::UIComponent>().was_rendered_to_screen = false;
    }
  }

  // Wipe UI component entities between scripts to avoid state leakage.
  for (const auto &e : afterhours::EntityHelper::get_entities()) {
    if (!e)
      continue;
    if (e->has<afterhours::ui::UIComponent>()) {
      e->cleanup = true;
    }
  }
  afterhours::EntityHelper::cleanup();
}

// Helper function to load fonts in headless mode
// raylib's LoadFont/LoadFontEx fail to create textures in headless mode,
// but manually creating the atlas and texture works
static raylib::Font load_font_headless(const char *filename, int fontSize = 32) {
  raylib::Font font = {0};

  int dataSize = 0;
  unsigned char *fontData = raylib::LoadFileData(filename, &dataSize);
  if (!fontData || dataSize <= 0) {
    log_error("Failed to load font file: {}", filename);
    return font;
  }

  // Load font data (glyphs info)
  font.baseSize = fontSize;
  font.glyphCount = 95;
  font.glyphPadding = 1;
  font.glyphs =
      raylib::LoadFontData(fontData, dataSize, fontSize, nullptr, 95, raylib::FONT_DEFAULT);

  if (!font.glyphs) {
    log_error("Failed to load font glyphs: {}", filename);
    raylib::UnloadFileData(fontData);
    return font;
  }

  // Generate font atlas
  raylib::Image atlas =
      raylib::GenImageFontAtlas(font.glyphs, &font.recs, font.glyphCount, fontSize, 1, 0);

  // Load texture from atlas image
  font.texture = raylib::LoadTextureFromImage(atlas);
  raylib::SetTextureFilter(font.texture, raylib::TEXTURE_FILTER_BILINEAR);

  raylib::UnloadImage(atlas);
  raylib::UnloadFileData(fontData);

  return font;
}

void run_headless_screenshots() {
  // 1. Initialize headless GL context (no window needed)
  HeadlessGL gl;
  if (!gl.init(1920, 1080)) {
    log_error("Failed to create headless GL context");
    return;
  }
  log_info("[Headless] Initialized headless GL context");

  // 2. Load GL extensions (required before rlglInit)
  raylib::rlLoadExtensions(gl.get_proc_address());
  log_info("[Headless] Loaded GL extensions");

  // 3. Initialize rlgl (raylib's GL layer)
  raylib::rlglInit(1920, 1080);
  raylib::rlSetBlendMode(raylib::RL_BLEND_ALPHA);
  log_info("[Headless] Initialized rlgl");

  // Test font loading right after rlglInit
  // Note: We removed the debug tests that were here

  // 4. Initialize files plugin (required for resource loading)
  afterhours::files::init("Prime Pressure", "resources");
  log_info("[Headless] Initialized files plugin");

  // 5. Create render textures
  mainRT = raylib::LoadRenderTexture(1920, 1080);
  screenRT = raylib::LoadRenderTexture(1920, 1080);
  log_info("[Headless] Created render textures");

  // 6. Load resources (font) using headless-compatible loader
  uiFont = load_font_headless(
      afterhours::files::get_resource_path("fonts", "Gaegu-Bold.ttf")
          .string()
          .c_str());
  log_info("[Headless] Loaded font (textureId: {})", uiFont.texture.id);

  // Configure UI validation
  configure_validation();

  // 5. Get all screen names
  std::vector<std::string> screen_names =
      ExampleScreenRegistry::get().get_screen_names();

  if (screen_names.empty()) {
    log_error("[Headless] No screens available");
    raylib::UnloadRenderTexture(screenRT);
    raylib::UnloadRenderTexture(mainRT);
    raylib::rlglClose();
    gl.shutdown();
    return;
  }

  log_info("[Headless] Found {} screens to render", screen_names.size());

  // Ensure output directory exists
  std::filesystem::create_directories(g_headless_output_dir);

  // Create window_manager resolution singleton manually with our fixed resolution
  // (We can't use GetRenderWidth/GetRenderHeight without a window)
  {
    afterhours::Entity &resolution_entity =
        afterhours::EntityHelper::createPermanentEntity();
    resolution_entity.addComponent<
        afterhours::window_manager::ProvidesCurrentResolution>(
        afterhours::window_manager::Resolution{.width = 1920, .height = 1080});
    resolution_entity
        .addComponent<afterhours::window_manager::ProvidesTargetFPS>(60);
    resolution_entity.addComponent<
        afterhours::window_manager::ProvidesAvailableWindowResolutions>();

    // Register these as singletons in the singleton map
    afterhours::EntityHelper::registerSingleton<
        afterhours::window_manager::ProvidesCurrentResolution>(
        resolution_entity);
    afterhours::EntityHelper::registerSingleton<
        afterhours::window_manager::ProvidesTargetFPS>(resolution_entity);
    afterhours::EntityHelper::registerSingleton<
        afterhours::window_manager::ProvidesAvailableWindowResolutions>(
        resolution_entity);
  }

  // Create UI singleton components manually (skip add_singleton_components to
  // avoid loading raylib's internal default font which may be invalid in
  // headless mode)
  {
    afterhours::Entity &ui_entity =
        afterhours::EntityHelper::createPermanentEntity();

    // Add UIContext
    ui_entity.addComponent<afterhours::ui::UIContext<InputAction>>();
    afterhours::EntityHelper::registerSingleton<
        afterhours::ui::UIContext<InputAction>>(ui_entity);

    // Load fonts for all screens - same as preload.cpp
    std::string english_font =
        afterhours::files::get_resource_path("fonts", "Gaegu-Bold.ttf")
            .string();
    std::string rounded_font =
        afterhours::files::get_resource_path("fonts", "eqprorounded-regular.ttf")
            .string();
    std::string garamond_font =
        afterhours::files::get_resource_path("fonts", "EBGaramond-Regular.ttf")
            .string();
    std::string symbols_font =
        afterhours::files::get_resource_path("fonts",
                                             "SymbolsNerdFont-Regular.ttf")
            .string();
    std::string fredoka_font =
        afterhours::files::get_resource_path("fonts",
                                             "Fredoka-VariableFont_wdth,wght.ttf")
            .string();
    std::string blackops_font =
        afterhours::files::get_resource_path("fonts", "BlackOpsOne-Regular.ttf")
            .string();
    std::string atkinson_font =
        afterhours::files::get_resource_path("fonts",
                                             "AtkinsonHyperlegible-Regular.ttf")
            .string();

    // Add FontManager with fonts loaded using headless-compatible loader
    auto &font_mgr = ui_entity.addComponent<afterhours::ui::FontManager>();
    font_mgr
        .load_font(afterhours::ui::UIComponent::DEFAULT_FONT,
                   load_font_headless(english_font.c_str()))
        .load_font(afterhours::ui::UIComponent::SYMBOL_FONT,
                   load_font_headless(english_font.c_str()))
        .load_font(afterhours::ui::UIComponent::UNSET_FONT,
                   load_font_headless(english_font.c_str()))
        .load_font("Gaegu-Bold", load_font_headless(english_font.c_str()))
        .load_font("EqProRounded", load_font_headless(rounded_font.c_str()))
        .load_font("Garamond", load_font_headless(garamond_font.c_str()))
        .load_font("NerdSymbols", load_font_headless(symbols_font.c_str()))
        .load_font("Fredoka", load_font_headless(fredoka_font.c_str()))
        .load_font("BlackOpsOne", load_font_headless(blackops_font.c_str()))
        .load_font("Atkinson", load_font_headless(atkinson_font.c_str()));
    log_info("[Headless] Loaded {} fonts into FontManager", 10);
    afterhours::EntityHelper::registerSingleton<afterhours::ui::FontManager>(
        ui_entity);

    // Add TextMeasureCache with measurement function
    auto &text_cache = ui_entity.addComponent<afterhours::ui::TextMeasureCache>();
    text_cache.set_measure_function(
        [](std::string_view text, std::string_view font_name, float font_size,
           float spacing) {
          auto font_manager = afterhours::EntityHelper::get_singleton_cmp<
              afterhours::ui::FontManager>();
          if (!font_manager) {
            return raylib::Vector2{0.0f, 0.0f};
          }
          const std::string font_name_str(font_name);
          const std::string text_str(text);
          raylib::Font font = font_manager->get_font(font_name_str);
          return afterhours::measure_text(font, text_str.c_str(), font_size,
                                          spacing);
        });
    afterhours::EntityHelper::registerSingleton<
        afterhours::ui::TextMeasureCache>(ui_entity);

    // Add UIComponent and AutoLayoutRoot so screens can use this entity
    // as a parent for their UI hierarchies
    ui_entity.addComponent<afterhours::ui::UIComponent>(ui_entity.id)
        .set_desired_width(afterhours::ui::screen_pct(1.f))
        .set_desired_height(afterhours::ui::screen_pct(1.f))
        .enable_font(afterhours::ui::UIComponent::DEFAULT_FONT,
                     afterhours::ui::pixels(75.f));
    ui_entity.addComponent<afterhours::ui::AutoLayoutRoot>();
    ui_entity.addComponent<afterhours::ui::UIComponentDebug>("headless_root");
  }

  // Store the ui_entity ID so we can skip it during cleanup
  int ui_entity_id = afterhours::EntityHelper::get_singleton<
                         afterhours::ui::UIContext<InputAction>>()
                         .get()
                         .id;

  // Create input singleton components (InputCollector, etc.)
  {
    afterhours::Entity &input_entity =
        afterhours::EntityHelper::createPermanentEntity();
    afterhours::input::add_singleton_components(input_entity);
  }

  // 7. Iterate all screens and capture screenshots
  for (const std::string &screen_name : screen_names) {
    // Reset UI state between screens
    auto *ui_context = afterhours::EntityHelper::get_singleton_cmp<
        afterhours::ui::UIContext<InputAction>>();
    if (ui_context) {
      ui_context->reset();
    }

    // Mark all existing UI entities as not rendered and clean them up
    // (except the root ui_entity which is permanent)
    for (const auto &e : afterhours::EntityHelper::get_entities()) {
      if (!e)
        continue;
      if (e->id == ui_entity_id)
        continue; // Skip the permanent root entity
      if (e->has<afterhours::ui::UIComponent>()) {
        e->cleanup = true;
      }
    }
    afterhours::EntityHelper::cleanup();

    // Reset the root entity's children list since we cleaned up all UI children
    auto &root_entity =
        afterhours::EntityHelper::get_singleton<
            afterhours::ui::UIContext<InputAction>>()
            .get();
    if (root_entity.has<afterhours::ui::UIComponent>()) {
      root_entity.get<afterhours::ui::UIComponent>().children.clear();
    }

    // Create fresh system manager for this screen
    afterhours::SystemManager systems;

    // Enforce singletons (but skip window_manager since we created it manually)
    // Note: We don't call window_manager::enforce_singletons since we created
    // the singletons manually above
    afterhours::ui::enforce_singletons<InputAction>(systems);
    afterhours::input::enforce_singletons(systems);
    afterhours::toast::enforce_singletons(systems);
    afterhours::modal::enforce_singletons(systems);

    // Register update systems (skip window_manager - its update uses
    // GetRenderWidth which crashes without a window)
    afterhours::input::register_update_systems(systems);
    // NOTE: Don't call window_manager::register_update_systems in headless mode
    afterhours::toast::register_update_systems(systems);
    afterhours::toast::register_layout_systems<InputAction>(systems);
    afterhours::modal::register_update_systems<InputAction>(systems);

    // Register UI pre-update systems
    afterhours::ui::register_before_ui_updates<InputAction>(systems);

    // Create and register the screen
    auto screen_system = ExampleScreenRegistry::get().create_screen(screen_name);
    if (!screen_system) {
      log_error("[Headless] Failed to create screen: {}", screen_name);
      continue;
    }
    g_current_screen = screen_system.get();
    systems.register_update_system(std::move(screen_system));

    // Register UI post-update systems
    afterhours::ui::register_after_ui_updates<InputAction>(systems);

    // Register render systems - these have const methods and work with systems.render()
    systems.register_render_system(std::make_unique<BeginWorldRender>());
    afterhours::modal::register_render_systems<InputAction>(systems);
    afterhours::ui::register_batched_render_systems<InputAction>(
        systems, InputAction::ToggleUILayoutDebug);
    systems.register_render_system(std::make_unique<EndWorldRender>());

    // Run one frame - this runs update systems which build the UI entities
    // First verify the font manager has valid fonts
    auto *font_mgr =
        afterhours::EntityHelper::get_singleton_cmp<afterhours::ui::FontManager>();
    if (font_mgr) {
      raylib::Font test_font =
          font_mgr->get_font(afterhours::ui::UIComponent::DEFAULT_FONT);
      if (test_font.glyphCount == 0 || test_font.glyphs == nullptr ||
          test_font.texture.id == 0) {
        log_error("[Headless] Font appears invalid! textureId={}, glyphCount={}, "
                  "Skipping screen: {}",
                  test_font.texture.id, test_font.glyphCount, screen_name);
        continue;
      }
    } else {
      log_error("[Headless] FontManager not found!");
    }

    // Run update systems to build UI
    {
      auto &entities = afterhours::EntityHelper::get_entities_for_mod();
      systems.tick_all(entities, 0.016f);
      afterhours::EntityHelper::cleanup();
    }

    // Run render systems - BeginWorldRender/EndWorldRender handle texture mode
    {
      const auto &entities = afterhours::EntityHelper::get_entities();
      systems.render(entities, 0.016f);
    }

    // Ensure GPU operations complete before reading pixels
    raylib::rlDrawRenderBatchActive();
    glFlush();
    glFinish();

    // Save screenshot - use glReadPixels directly from the FBO
    // since LoadImageFromTexture might not work correctly in headless mode
    std::string path = g_headless_output_dir + "/" + screen_name + ".png";
    {
      // Check which FBO is currently bound before we bind ours
      GLint boundFbo;
      glGetIntegerv(GL_FRAMEBUFFER_BINDING, &boundFbo);
      log_info("[Headless] Before screenshot - currently bound FBO: {}", boundFbo);

      // Debug: Print FBO and texture IDs
      log_info("[Headless] mainRT - id(FBO): {}, texture.id: {}", mainRT.id, mainRT.texture.id);

      // Bind the framebuffer and read pixels
      glBindFramebuffer(GL_FRAMEBUFFER, mainRT.id);

      // Verify binding worked
      glGetIntegerv(GL_FRAMEBUFFER_BINDING, &boundFbo);
      log_info("[Headless] After binding mainRT.id - bound FBO: {}", boundFbo);

      unsigned char *pixels = (unsigned char *)RL_MALLOC(1920 * 1080 * 4);
      glReadPixels(0, 0, 1920, 1080, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

      // Debug: Check first pixel
      log_info("[Headless] First pixel RGBA: {}, {}, {}, {}",
               pixels[0], pixels[1], pixels[2], pixels[3]);

      // Create raylib Image from the pixels
      raylib::Image image;
      image.data = pixels;
      image.width = 1920;
      image.height = 1080;
      image.format = raylib::PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
      image.mipmaps = 1;

      raylib::ImageFlipVertical(&image);
      raylib::ExportImage(image, path.c_str());
      raylib::UnloadImage(image); // Use raylib's unload instead of free
    }
    log_info("[Headless] Saved: {}", path);
  }

  // 8. Cleanup
  g_current_screen = nullptr;

  // Clean up any remaining entities
  for (const auto &e : afterhours::EntityHelper::get_entities()) {
    if (!e)
      continue;
    e->cleanup = true;
  }
  afterhours::EntityHelper::cleanup();

  raylib::UnloadFont(uiFont);
  raylib::UnloadRenderTexture(screenRT);
  raylib::UnloadRenderTexture(mainRT);
  raylib::rlglClose();
  gl.shutdown();

  log_info("[Headless] Completed - rendered {} screens", screen_names.size());
}
