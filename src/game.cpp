#include "game.h"

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
#include "testing/test_app.h"
#include "testing/test_input.h"
#include "testing/test_macros.h"
#include "testing/tests/SimpleButtonClickTest.h"
#include <afterhours/src/plugins/files.h>

#include <afterhours/src/plugins/animation.h>
#include <afterhours/src/plugins/ui/validation_systems.h>
#include <chrono>
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
  uiFont = raylib::LoadFont(
      afterhours::files::get_resource_path("fonts", "Gaegu-Bold.ttf")
          .string()
          .c_str());

  afterhours::SystemManager systems;

  {
    afterhours::window_manager::enforce_singletons(systems);
    afterhours::ui::enforce_singletons<InputAction>(systems);
    afterhours::input::enforce_singletons(systems);
  }

  TestSystem *test_system_ptr = nullptr;

  {
    afterhours::input::register_update_systems(systems);
    afterhours::window_manager::register_update_systems(systems);

    auto test_system = std::make_unique<TestSystem>();
    test_system_ptr = test_system.get();
    systems.register_update_system(std::move(test_system));
  }

  {
    systems.register_render_system(std::make_unique<BeginWorldRender>());
    afterhours::ui::register_render_systems<InputAction>(
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
  uiFont = raylib::LoadFont(
      afterhours::files::get_resource_path("fonts", "Gaegu-Bold.ttf")
          .string()
          .c_str());

  afterhours::SystemManager systems;

  {
    afterhours::window_manager::enforce_singletons(systems);
    afterhours::ui::enforce_singletons<InputAction>(systems);
    afterhours::input::enforce_singletons(systems);
  }

  TestSystem *test_system_ptr = nullptr;

  {
    afterhours::input::register_update_systems(systems);
    afterhours::window_manager::register_update_systems(systems);

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
      if (test_name.find(screen_name + "_") == 0 ||
          test_name == screen_name) {
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
        systems.register_update_system(std::make_unique<SetupSimpleButtonTest>());
      }
    }

    // Register UI post-update systems (HandleClicks, HandleTabbing, layout,
    // etc.)
    afterhours::ui::register_after_ui_updates<InputAction>(systems);
  }

  {
    systems.register_render_system(std::make_unique<BeginWorldRender>());
    afterhours::ui::register_render_systems<InputAction>(
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

  uiFont = raylib::LoadFont(
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
  }

  {
    afterhours::input::register_update_systems(systems);
    afterhours::window_manager::register_update_systems(systems);

    systems.register_update_system(std::make_unique<UpdateRenderTexture>());
  }

  {
    systems.register_render_system(std::make_unique<BeginWorldRender>());
    afterhours::ui::register_render_systems<InputAction>(
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
    auto *ui_context =
        afterhours::EntityHelper::get_singleton_cmp<
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
