#include "headless_screenshots.h"

#include "font_config.h"
#include "game.h"
#include "input_mapping.h"
#include "log.h"
#include "settings.h"
#include "systems/ExampleScreenRegistry.h"
#include "systems/RenderSystemHelpers.h"
#include "systems/RenderRenderTexture.h"
#include "systems/RenderScreenHUD.h"
#include "systems/UpdateRenderTexture.h"
#include "systems/SetupSimpleButtonTest.h"
#include "systems/SetupTabbingTest.h"
#include "systems/TestSystem.h"
#include "testing/test_app.h"
#include "testing/test_input.h"
#include "testing/test_macros.h"
#include "testing/layout_summary.h"
#include <afterhours/src/graphics.h>

#include <afterhours/src/plugins/e2e_testing/test_input.h>
#include <afterhours/src/plugins/e2e_testing/ui_commands.h>
#include <afterhours/src/plugins/files.h>
#include <afterhours/src/plugins/modal.h>
#include <afterhours/src/plugins/toast.h>
#include <afterhours/src/plugins/ui/entity_management.h>
#include <afterhours/src/plugins/ui/ui_collection.h>
#include <afterhours/src/plugins/ui/validation_systems.h>
#include <filesystem>
#include <iostream>

// Globals defined in game.cpp
extern raylib::RenderTexture2D mainRT;
extern raylib::RenderTexture2D screenRT;
extern raylib::Font uiFont;
extern afterhours::SystemBase *g_current_screen;

// Globals defined in main.cpp (declared in headless_screenshots.h)
// bool g_headless_mode and std::string g_headless_output_dir
std::vector<HeadlessResolution> g_headless_resolutions;

namespace {

// Helper function to load fonts in headless mode
// raylib's LoadFont/LoadFontEx fail to create textures in headless mode,
// but manually creating the atlas and texture works
raylib::Font load_font_headless(const char *filename, int fontSize = 32) {
  raylib::Font font = {0};

  int dataSize = 0;
  unsigned char *fontData = raylib::LoadFileData(filename, &dataSize);
  if (!fontData || dataSize <= 0) {
    log_error("Failed to load font file: {}", filename);
    return font;
  }

  font.baseSize = fontSize;
  font.glyphCount = 95;
  font.glyphPadding = 1;
  font.glyphs = raylib::LoadFontData(fontData, dataSize, fontSize, nullptr, 95,
                                     raylib::FONT_DEFAULT);

  if (!font.glyphs) {
    log_error("Failed to load font glyphs: {}", filename);
    raylib::UnloadFileData(fontData);
    return font;
  }

  raylib::Image atlas = raylib::GenImageFontAtlas(
      font.glyphs, &font.recs, font.glyphCount, fontSize, 1, 0);
  font.texture = raylib::LoadTextureFromImage(atlas);
  raylib::SetTextureFilter(font.texture, raylib::TEXTURE_FILTER_BILINEAR);

  raylib::UnloadImage(atlas);
  raylib::UnloadFileData(fontData);

  return font;
}

// Load font with specific codepoints for CJK support
raylib::Font load_font_headless_with_codepoints(const char *filename,
                                                const int *codepoints,
                                                int codepointCount,
                                                int fontSize = 32) {
  raylib::Font font = {0};

  int dataSize = 0;
  unsigned char *fontData = raylib::LoadFileData(filename, &dataSize);
  if (!fontData || dataSize <= 0) {
    log_error("Failed to load font file: {}", filename);
    return font;
  }

  font.baseSize = fontSize;
  font.glyphCount = codepointCount;
  font.glyphPadding = 1;
  font.glyphs = raylib::LoadFontData(fontData, dataSize, fontSize,
                                     const_cast<int *>(codepoints),
                                     codepointCount, raylib::FONT_DEFAULT);

  if (!font.glyphs) {
    log_error("Failed to load font glyphs with codepoints: {}", filename);
    raylib::UnloadFileData(fontData);
    return font;
  }

  raylib::Image atlas = raylib::GenImageFontAtlas(
      font.glyphs, &font.recs, font.glyphCount, fontSize, 1, 0);
  font.texture = raylib::LoadTextureFromImage(atlas);
  raylib::SetTextureFilter(font.texture, raylib::TEXTURE_FILTER_BILINEAR);

  raylib::UnloadImage(atlas);
  raylib::UnloadFileData(fontData);

  return font;
}

// Load all fonts using the shared font configuration
void load_fonts_into_manager(afterhours::ui::FontManager &font_mgr) {
  int loaded_count = 0;
  for (const auto &font_def : font_config::get_all_fonts()) {
    std::string path =
        afterhours::files::get_resource_path("fonts", font_def.filename)
            .string();

    if (font_def.needs_codepoints && font_def.get_codepoints) {
      auto codepoints = font_def.get_codepoints();
      raylib::Font font = load_font_headless_with_codepoints(
          path.c_str(), codepoints.data(), static_cast<int>(codepoints.size()));
      if (font.glyphCount > 0) {
        font_mgr.load_font(font_def.name, font);
        loaded_count++;
      } else {
        log_warn("[Headless] Failed to load CJK font: {}", font_def.name);
      }
    } else {
      raylib::Font font = load_font_headless(path.c_str());
      if (font.glyphCount > 0) {
        font_mgr.load_font(font_def.name, font);
        loaded_count++;
      } else {
        log_warn("[Headless] Failed to load font: {}", font_def.name);
      }
    }
  }
  log_info("[Headless] Loaded {} fonts into FontManager", loaded_count);
}

// Configure UI validation for design rule enforcement
void configure_validation() {
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
  config.highlight_violations = true;
}

// Setup ECS singletons for headless rendering
void setup_ecs_singletons(int screenshot_width, int screenshot_height) {
  // Create window_manager resolution singleton
  afterhours::Entity &resolution_entity =
      afterhours::EntityHelper::createPermanentEntity();
  resolution_entity
      .addComponent<afterhours::window_manager::ProvidesCurrentResolution>(
          afterhours::window_manager::Resolution{.width = screenshot_width,
                                                 .height = screenshot_height});
  resolution_entity.addComponent<afterhours::window_manager::ProvidesTargetFPS>(
      60);
  resolution_entity.addComponent<
      afterhours::window_manager::ProvidesAvailableWindowResolutions>();

  afterhours::EntityHelper::registerSingleton<
      afterhours::window_manager::ProvidesCurrentResolution>(resolution_entity);
  afterhours::EntityHelper::registerSingleton<
      afterhours::window_manager::ProvidesTargetFPS>(resolution_entity);
  afterhours::EntityHelper::registerSingleton<
      afterhours::window_manager::ProvidesAvailableWindowResolutions>(
      resolution_entity);

  // Initialize UI plugin (creates root + singletons in UI collection)
  afterhours::ui::init_ui_plugin<InputAction>();

  // Load custom fonts for headless rendering
  auto *font_mgr = afterhours::EntityHelper::get_singleton_cmp<
      afterhours::ui::FontManager>();
  if (font_mgr) {
    load_fonts_into_manager(*font_mgr);
  }

  // Create input singleton components (with game's input mapping for test
  // input)
  afterhours::Entity &input_entity =
      afterhours::EntityHelper::createPermanentEntity();
  afterhours::input::add_singleton_components(input_entity, get_mapping());
}

// Reset state between screens
void reset_screen_state(int ui_entity_id) {
  // Clear cached source-location → entity-ID mappings so stale IDs from the
  // previous screen don't cause lookup failures in mk().
  afterhours::ui::imm::clear_existing_ui_elements();

  // Reset UI context
  auto *ui_context = afterhours::EntityHelper::get_singleton_cmp<
      afterhours::ui::UIContext<InputAction>>();
  if (ui_context) {
    ui_context->reset();
  }

  // Clean up toast singleton
  if (afterhours::EntityHelper::has_singleton<afterhours::toast::ToastRoot>()) {
    auto &toast_singleton =
        afterhours::EntityHelper::get_singleton<afterhours::toast::ToastRoot>()
            .get();
    if (toast_singleton.has<afterhours::toast::ToastRoot>()) {
      auto &toast_root = toast_singleton.get<afterhours::toast::ToastRoot>();
      if (toast_root.entity_id >= 0) {
        auto opt_root =
            afterhours::EntityHelper::getEntityForID(toast_root.entity_id);
        if (opt_root.valid()) {
          opt_root.asE().cleanup = true;
        }
      }
    }
    toast_singleton.cleanup = true;
  }

  // Clean up modal singleton
  if (afterhours::EntityHelper::has_singleton<afterhours::modal::ModalRoot>()) {
    auto &modal_singleton =
        afterhours::EntityHelper::get_singleton<afterhours::modal::ModalRoot>()
            .get();
    if (modal_singleton.has<afterhours::modal::ModalRoot>()) {
      auto &modal_root = modal_singleton.get<afterhours::modal::ModalRoot>();
      for (auto modal_id : modal_root.modal_stack) {
        if (modal_id >= 0) {
          auto opt_modal = afterhours::EntityHelper::getEntityForID(modal_id);
          if (opt_modal.valid()) {
            opt_modal.asE().cleanup = true;
          }
        }
      }
      modal_root.modal_stack.clear();
    }
    modal_singleton.cleanup = true;
  }

  // Clean up UI entities (except permanent root) - default collection
  for (const auto &e : afterhours::EntityHelper::get_entities()) {
    if (!e)
      continue;
    if (e->id == ui_entity_id)
      continue;
    if (e->has<afterhours::ui::UIComponent>()) {
      e->cleanup = true;
    }
  }
  afterhours::EntityHelper::cleanup();

  // Clean up UI collection entities (except permanent root)
  auto &ui_coll = afterhours::ui::UICollectionHolder::get().collection;
  for (const auto &e : ui_coll.get_entities()) {
    if (!e)
      continue;
    if (e->id == ui_entity_id)
      continue;
    if (e->has<afterhours::ui::UIComponent>()) {
      e->cleanup = true;
    }
  }
  ui_coll.cleanup();

  // Reset root entity's children
  auto &root_entity = afterhours::EntityHelper::get_singleton<
                          afterhours::ui::UIContext<InputAction>>()
                          .get();
  if (root_entity.has<afterhours::ui::UIComponent>()) {
    root_entity.get<afterhours::ui::UIComponent>().children.clear();
  }
}

// Create and configure systems for a screen
afterhours::SystemManager
create_screen_systems(const std::string &screen_name) {
  afterhours::SystemManager systems;

  afterhours::ui::enforce_singletons<InputAction>(systems);
  afterhours::input::enforce_singletons(systems);
  afterhours::toast::enforce_singletons(systems);
  afterhours::modal::enforce_singletons(systems);

  afterhours::input::register_update_systems(systems);
  afterhours::toast::register_update_systems(systems);
  afterhours::toast::register_layout_systems<InputAction>(systems);
  afterhours::modal::register_update_systems<InputAction>(systems);

  afterhours::ui::register_before_ui_updates<InputAction>(systems);

  auto screen_system = ExampleScreenRegistry::get().create_screen(screen_name);
  if (!screen_system) {
    log_error("[Headless] Failed to create screen: {}", screen_name);
    return systems;
  }
  g_current_screen = screen_system.get();
  systems.register_update_system(std::move(screen_system));

  afterhours::ui::register_after_ui_updates<InputAction>(systems);

  // Register render systems
  systems.register_render_system(std::make_unique<BeginWorldRender>());
  afterhours::modal::register_render_systems<InputAction>(systems);
  afterhours::ui::register_batched_render_systems<InputAction>(
      systems, InputAction::ToggleUILayoutDebug);
  systems.register_render_system(std::make_unique<EndWorldRender>());

  return systems;
}

// Full present pipeline so was_rendered_to_screen is set (needed for layout summary).
void register_present_systems(afterhours::SystemManager &systems) {
  systems.register_update_system(std::make_unique<UpdateRenderTexture>());
  systems.register_render_system(
      std::make_unique<BeginPostProcessingRender>());
  systems.register_render_system(std::make_unique<RenderRenderTexture>());
  systems.register_render_system(std::make_unique<RenderScreenHUD>());
  systems.register_render_system(std::make_unique<EndDrawing>());
}

void tick_and_render(afterhours::SystemManager &systems, float dt) {
  systems.run(dt);
}

// Walk the settled UI tree directly and return human-readable layout problems.
// We do this here (rather than via the ECS validation systems) because those
// validators filter the main entity collection, while the UI widgets live in
// the UI collection — so they don't fire in the batch screenshot path. This
// reuses the same UI-collection walk that layout_summary uses (proven headless).
//
// Catches the two real bug classes from the layout audit, while suppressing the
// two big false-positive sources (scroll/clip containers and freely-positioned
// absolute elements):
//   1. off-screen  — a non-clipped element's rect leaves the viewport.
//   2. child escapes parent — a non-absolute child spills past a non-clipping
//      parent on the bottom/right (the "content taller/wider than a fixed-size
//      container" bug); left/top escapes are usually deliberate negative offsets.
std::vector<std::string> collect_layout_problems(int vw, int vh) {
  using afterhours::testing::ui_commands::get_screen_rect;
  using afterhours::testing::ui_commands::ui_query;
  constexpr float VIEWPORT_TOL = 2.0f;
  // Ignore hairline escapes (edge-flush controls, borders, rounding); only flag
  // escapes big enough to read as a real overflow.
  constexpr float ESCAPE_TOL = 8.0f;
  std::vector<std::string> problems;

  auto name_of = [](afterhours::Entity &e) -> std::string {
    if (e.has<afterhours::ui::UIComponentDebug>()) {
      auto n = e.get<afterhours::ui::UIComponentDebug>().name();
      if (!n.empty())
        return n;
    }
    if (e.has<afterhours::ui::HasLabel>()) {
      const auto &l = e.get<afterhours::ui::HasLabel>().label;
      if (!l.empty())
        return "\"" + l + "\"";
    }
    return "entity_" + std::to_string(e.id);
  };

  // True if this element sits inside a Scroll/Hidden container, which clips its
  // children on purpose — an off-bounds rect there is expected, not a bug.
  auto has_clip_ancestor = [](afterhours::Entity &e) -> bool {
    afterhours::EntityID pid = e.get<afterhours::ui::UIComponent>().parent;
    while (pid != (afterhours::EntityID)-1) {
      afterhours::OptEntity opt =
          afterhours::ui::UICollectionHolder::getEntityForID(pid);
      if (!opt.valid())
        break;
      afterhours::Entity &p = opt.asE();
      if (p.has<afterhours::ui::HasClipChildren>())
        return true;
      if (!p.has<afterhours::ui::UIComponent>())
        break;
      pid = p.get<afterhours::ui::UIComponent>().parent;
    }
    return false;
  };

  for (afterhours::Entity &parent :
       ui_query().whereHasComponent<afterhours::ui::UIComponent>().gen()) {
    auto &pc = parent.get<afterhours::ui::UIComponent>();
    if (!pc.was_rendered_to_screen || pc.should_hide)
      continue;
    auto pr = get_screen_rect(parent);
    if (pr.width < 1.f || pr.height < 1.f)
      continue;

    // 1. Off-screen: the parent itself leaves the viewport (and isn't clipped).
    if (!parent.has<afterhours::ui::HasClipChildren>() &&
        !has_clip_ancestor(parent)) {
      bool off = pr.x < -VIEWPORT_TOL || pr.y < -VIEWPORT_TOL ||
                 pr.x + pr.width > vw + VIEWPORT_TOL ||
                 pr.y + pr.height > vh + VIEWPORT_TOL;
      if (off)
        problems.push_back(name_of(parent) + " (off-screen)");
    }

    // 2. Child escapes parent — skip parents that clip on purpose.
    if (parent.has<afterhours::ui::HasClipChildren>())
      continue;

    for (afterhours::EntityID cid : pc.children) {
      afterhours::OptEntity opt =
          afterhours::ui::UICollectionHolder::getEntityForID(cid);
      if (!opt.valid())
        continue;
      afterhours::Entity &child = opt.asE();
      if (!child.has<afterhours::ui::UIComponent>())
        continue;
      auto &cc = child.get<afterhours::ui::UIComponent>();
      if (!cc.was_rendered_to_screen || cc.should_hide || cc.absolute)
        continue; // absolute children are positioned freely by design
      auto cr = get_screen_rect(child);
      if (cr.width < 1.f || cr.height < 1.f)
        continue;
      bool escapes = cr.x + cr.width > pr.x + pr.width + ESCAPE_TOL ||
                     cr.y + cr.height > pr.y + pr.height + ESCAPE_TOL;
      if (escapes)
        problems.push_back(name_of(child) + " escapes " + name_of(parent));
    }
  }
  return problems;
}

} // namespace

// Capture all screens at a single resolution
void run_headless_screenshots_at(int width, int height,
                                 const std::string &label) {
  // 1. Configure and initialize graphics backend
  afterhours::graphics::Config cfg;
  cfg.display = afterhours::graphics::DisplayMode::Headless;
  cfg.width = width;
  cfg.height = height;
  cfg.title = "Headless Screenshots";
  cfg.target_fps = 60;

  if (!afterhours::graphics::init(cfg)) {
    log_error("[Headless] Failed to initialize graphics backend for {}", label);
    return;
  }
  log_info("[Headless] Graphics backend initialized ({}x{} = {})", width,
           height, label);

  // 1b. Update Settings singleton so screens using
  // Settings::get().get_screen_width/height()
  //     see the correct resolution
  Settings::get().update_resolution(
      afterhours::window_manager::Resolution{.width = width, .height = height});

  // 2. Initialize files plugin (required for resource loading)
  afterhours::files::init("Prime Pressure", "resources");

  // 3. Set up global render textures
  mainRT = afterhours::graphics::get_render_texture();
  screenRT = raylib::LoadRenderTexture(width, height);

  // 4. Load main font for legacy usage
  uiFont = load_font_headless(
      afterhours::files::get_resource_path("fonts", "Gaegu-Bold.ttf")
          .string()
          .c_str());

  // 5. Configure UI validation
  configure_validation();

  // 6. Get all screen names
  std::vector<std::string> screen_names =
      ExampleScreenRegistry::get().get_screen_names();

  // Optional single-screen filter (--screen <name>)
  if (!g_headless_screen_filter.empty()) {
    if (ExampleScreenRegistry::get().has_screen(g_headless_screen_filter)) {
      screen_names = {g_headless_screen_filter};
    } else {
      log_error("[Headless] Screen not found: {}", g_headless_screen_filter);
      screen_names.clear();
    }
  }

  if (screen_names.empty()) {
    log_error("[Headless] No screens available");
    raylib::UnloadRenderTexture(screenRT);
    afterhours::graphics::shutdown();
    return;
  }

  log_info("[Headless][{}] Rendering {} screens", label, screen_names.size());

  // 7. Ensure output directory exists
  std::filesystem::create_directories(g_headless_output_dir);

  // 8. Setup ECS singletons
  setup_ecs_singletons(width, height);

  int ui_entity_id = afterhours::EntityHelper::get_singleton<
                         afterhours::ui::UIContext<InputAction>>()
                         .get()
                         .id;

  // 9. Iterate all screens and capture screenshots
  int total_violations = 0;
  int screens_with_violations = 0;
  for (const std::string &screen_name : screen_names) {
    // Reset state between screens
    reset_screen_state(ui_entity_id);

    // Create systems for this screen
    afterhours::SystemManager systems = create_screen_systems(screen_name);

    // Verify font manager
    auto *font_mgr = afterhours::EntityHelper::get_singleton_cmp<
        afterhours::ui::FontManager>();
    if (font_mgr) {
      raylib::Font test_font =
          font_mgr->get_font(afterhours::ui::UIComponent::DEFAULT_FONT);
      if (test_font.glyphCount == 0 || test_font.glyphs == nullptr ||
          test_font.texture.id == 0) {
        log_error("[Headless] Font invalid! Skipping: {}", screen_name);
        continue;
      }
    }

    // Run two tick+render passes so that decorators which depend on
    // computed sizes from the previous frame (e.g. corner brackets,
    // grid backgrounds) have valid data on the second pass.
    for (int pass = 0; pass < 2; pass++) {
      {
        auto &entities = afterhours::EntityHelper::get_entities_for_mod();
        systems.tick_all(entities, 0.016f);
      }
      {
        auto &entities = afterhours::EntityHelper::get_entities_for_mod();
        systems.render(entities, 0.016f);
      }
      afterhours::EntityHelper::cleanup();
    }

    // Ensure GPU operations complete and flush render batch
    raylib::rlDrawRenderBatchActive();

    // Surface layout problems for this screen so `make screenshots` warns the
    // dev instead of requiring a manual screenshot audit. Concise per-screen
    // roll-up; caps the list so output stays scannable across all screens.
    {
      auto problems = collect_layout_problems(width, height);
      if (!problems.empty()) {
        std::string joined;
        const size_t cap = 6;
        for (size_t i = 0; i < problems.size() && i < cap; i++)
          joined += (joined.empty() ? "" : "; ") + problems[i];
        if (problems.size() > cap)
          joined += "; +" + std::to_string(problems.size() - cap) + " more";
        log_warn("[Headless][validate] {}: {} issue(s): {}", screen_name,
                 problems.size(), joined);
        total_violations += static_cast<int>(problems.size());
        screens_with_violations++;
      }
    }

    // Capture screenshot: {screen}_{label}.png
    std::string filename = screen_name + "_" + label + ".png";
    std::filesystem::path output_path =
        std::filesystem::path(g_headless_output_dir) / filename;
    afterhours::graphics::capture_frame(output_path);

    log_info("[Headless][{}] Saved: {}", label, output_path.string());
  }

  // 10. Cleanup
  g_current_screen = nullptr;

  for (const auto &e : afterhours::EntityHelper::get_entities()) {
    if (!e)
      continue;
    e->cleanup = true;
  }
  afterhours::EntityHelper::cleanup();

  // Also clean UI collection entities
  {
    auto &ui_coll = afterhours::ui::UICollectionHolder::get().collection;
    for (const auto &e : ui_coll.get_entities()) {
      if (!e)
        continue;
      e->cleanup = true;
    }
    ui_coll.cleanup();
  }

  raylib::UnloadFont(uiFont);
  // Note: mainRT is owned by the graphics backend, do not unload it manually
  raylib::UnloadRenderTexture(screenRT);
  afterhours::graphics::shutdown();

  if (total_violations > 0) {
    log_warn("[Headless][{}] {} layout violation(s) across {} screen(s) — see "
             "[validate] lines above (child-overflow / off-screen / contrast / "
             "font-size). Fix by letting containers size to content "
             "(children()) or trimming content to fit.",
             label, total_violations, screens_with_violations);
  } else {
    log_info("[Headless][{}] No layout violations detected", label);
  }

  log_info("[Headless][{}] Completed - rendered {} screens", label,
           screen_names.size());
}

void run_headless_screenshots() {
  auto resolutions = g_headless_resolutions;
  if (resolutions.empty()) {
    // Default: 720p only (backward compat)
    resolutions = {{1280, 720, "720p"}};
  }

  log_info("[Headless] Capturing at {} resolution(s)", resolutions.size());
  for (const auto &res : resolutions) {
    log_info("[Headless] === {} ({}x{}) ===", res.label, res.width, res.height);
    run_headless_screenshots_at(res.width, res.height, res.label);
  }
  log_info("[Headless] All resolutions complete");
}

int run_layout_summary(const std::string &screen_name, int width, int height,
                       int frame_count, const std::string &output_path) {
  if (!ExampleScreenRegistry::get().has_screen(screen_name)) {
    std::cerr << "Unknown screen: " << screen_name << "\n";
    return 1;
  }

  afterhours::graphics::Config cfg;
  cfg.display = afterhours::graphics::DisplayMode::Headless;
  cfg.width = width;
  cfg.height = height;
  cfg.title = "Layout Summary";
  cfg.target_fps = 60;

  if (!afterhours::graphics::init(cfg)) {
    std::cerr << "Failed to initialize headless graphics backend\n";
    return 1;
  }

  Settings::get().update_resolution(
      afterhours::window_manager::Resolution{.width = width, .height = height});

  afterhours::files::init("Prime Pressure", "resources");

  mainRT = afterhours::graphics::get_render_texture();
  screenRT = raylib::LoadRenderTexture(width, height);
  uiFont = load_font_headless(
      afterhours::files::get_resource_path("fonts", "Gaegu-Bold.ttf")
          .string()
          .c_str());

  configure_validation();
  setup_ecs_singletons(width, height);

  int ui_entity_id = afterhours::EntityHelper::get_singleton<
                         afterhours::ui::UIContext<InputAction>>()
                         .get()
                         .id;

  reset_screen_state(ui_entity_id);
  afterhours::SystemManager systems = create_screen_systems(screen_name);
  register_present_systems(systems);
  if (!g_current_screen) {
    raylib::UnloadFont(uiFont);
    raylib::UnloadRenderTexture(screenRT);
    afterhours::graphics::shutdown();
    return 1;
  }

  const int passes = std::max(2, frame_count);
  for (int pass = 0; pass < passes; ++pass) {
    tick_and_render(systems, 0.016f);
  }

  bool ok = false;
  if (output_path.empty() || output_path == "-") {
    ok = layout_summary::write(std::cout);
  } else {
    ok = layout_summary::write_to_file(output_path);
  }

  g_current_screen = nullptr;

  for (const auto &e : afterhours::EntityHelper::get_entities()) {
    if (!e)
      continue;
    e->cleanup = true;
  }
  afterhours::EntityHelper::cleanup();

  {
    auto &ui_coll = afterhours::ui::UICollectionHolder::get().collection;
    for (const auto &e : ui_coll.get_entities()) {
      if (!e)
        continue;
      e->cleanup = true;
    }
    ui_coll.cleanup();
  }

  raylib::UnloadFont(uiFont);
  raylib::UnloadRenderTexture(screenRT);
  afterhours::graphics::shutdown();

  return ok ? 0 : 1;
}

// ---------------------------------------------------------------------------
// run_all_tests_headless -- drive every registered coroutine-based TestApp
// in headless mode (no window).  Returns the number of failures.
// ---------------------------------------------------------------------------
int run_all_tests_headless() {
  constexpr int WIDTH = 1280;
  constexpr int HEIGHT = 720;
  constexpr int MAX_FRAMES = 10000; // safety timeout per test

  // 1. Init headless graphics
  afterhours::graphics::Config cfg;
  cfg.display = afterhours::graphics::DisplayMode::Headless;
  cfg.width = WIDTH;
  cfg.height = HEIGHT;
  cfg.title = "Headless Tests";
  cfg.target_fps = 60;

  if (!afterhours::graphics::init(cfg)) {
    log_error("[HeadlessTests] Failed to init graphics backend");
    return -1;
  }

  Settings::get().update_resolution(
      afterhours::window_manager::Resolution{.width = WIDTH, .height = HEIGHT});

  afterhours::files::init("Prime Pressure", "resources");

  mainRT = afterhours::graphics::get_render_texture();
  screenRT = raylib::LoadRenderTexture(WIDTH, HEIGHT);

  uiFont = load_font_headless(
      afterhours::files::get_resource_path("fonts", "Gaegu-Bold.ttf")
          .string()
          .c_str());

  configure_validation();
  setup_ecs_singletons(WIDTH, HEIGHT);

  int ui_entity_id = afterhours::EntityHelper::get_singleton<
                         afterhours::ui::UIContext<InputAction>>()
                         .get()
                         .id;

  // 2. Gather tests
  TestRegistry &registry = TestRegistry::get();
  if (registry.tests.empty()) {
    log_error("[HeadlessTests] No tests registered");
    raylib::UnloadRenderTexture(screenRT);
    afterhours::graphics::shutdown();
    return -1;
  }

  int total = static_cast<int>(registry.tests.size());
  int passed = 0;
  int failed = 0;
  std::vector<std::string> failures;

  log_info("[HeadlessTests] Running {} tests (UI coroutine tests migrated to E2E; see tests/e2e_scripts/102_*.e2e)",
           total);

  // 3. Run each test
  for (const auto &[test_name, test_func] : registry.tests) {
    // Reset state between tests
    reset_screen_state(ui_entity_id);
    test_input::test_mode = true;
    test_input::slow_test_mode = false;
    test_input::clear_queue();
    test_app::frame_counter = 0;

    // Build systems for this test
    afterhours::SystemManager systems;

    afterhours::ui::enforce_singletons<InputAction>(systems);
    afterhours::input::enforce_singletons(systems);
    afterhours::toast::enforce_singletons(systems);
    afterhours::modal::enforce_singletons(systems);

    afterhours::input::register_update_systems(systems);
    afterhours::toast::register_update_systems(systems);
    afterhours::toast::register_layout_systems<InputAction>(systems);
    afterhours::modal::register_update_systems<InputAction>(systems);

    // TestSystem drives the coroutine
    auto test_system = std::make_unique<TestSystem>();
    TestSystem *test_system_ptr = test_system.get();
    systems.register_update_system(std::move(test_system));

    // UI pre-update
    afterhours::ui::register_before_ui_updates<InputAction>(systems);

    // Find matching screen for the test name (longest match to avoid
    // e.g. "text" matching before "text_input")
    bool screen_found = false;
    std::string best_match;
    for (const auto &screen_name :
         ExampleScreenRegistry::get().get_screen_names()) {
      if (test_name.find(screen_name + "_") == 0 || test_name == screen_name) {
        if (screen_name.size() > best_match.size()) {
          best_match = screen_name;
        }
      }
    }
    if (!best_match.empty()) {
      auto screen = ExampleScreenRegistry::get().create_screen(best_match);
      if (screen) {
        g_current_screen = screen.get();
        systems.register_update_system(std::move(screen));
        screen_found = true;
      }
    }

    if (!screen_found) {
      if (test_name == "tabbing") {
        systems.register_update_system(std::make_unique<SetupTabbingTest>());
      } else {
        systems.register_update_system(
            std::make_unique<SetupSimpleButtonTest>());
      }
    }

    // UI post-update
    afterhours::ui::register_after_ui_updates<InputAction>(systems);

    // Render systems (needed for layout even in headless)
    systems.register_render_system(std::make_unique<BeginWorldRender>());
    afterhours::modal::register_render_systems<InputAction>(systems);
    afterhours::ui::register_batched_render_systems<InputAction>(
        systems, InputAction::ToggleUILayoutDebug);
    systems.register_render_system(std::make_unique<EndWorldRender>());

    afterhours::ui::validation::register_systems<InputAction>(systems);

    // Create and set the test coroutine
    TestApp test = test_func();
    test_system_ptr->set_test(test_name, std::move(test));

    // Tick loop
    bool timed_out = true;
    for (int frame = 0; frame < MAX_FRAMES; frame++) {
      {
        auto &entities = afterhours::EntityHelper::get_entities_for_mod();
        systems.tick_all(entities, 0.016f);
      }
      {
        auto &entities = afterhours::EntityHelper::get_entities_for_mod();
        systems.render(entities, 0.016f);
      }
      afterhours::EntityHelper::cleanup();

      if (test_system_ptr->is_complete()) {
        timed_out = false;
        break;
      }
    }

    // Check result
    if (timed_out) {
      std::cout << "FAIL  " << test_name << "  (timed out after " << MAX_FRAMES
                << " frames)" << std::endl;
      failed++;
      failures.push_back(test_name + " (timeout)");
    } else {
      std::string error = test_system_ptr->get_error();
      if (!error.empty()) {
        std::cout << "FAIL  " << test_name << "  " << error << std::endl;
        failed++;
        failures.push_back(test_name + ": " + error);
      } else {
        std::cout << "PASS  " << test_name << std::endl;
        passed++;
      }
    }

    test_input::test_mode = false;
    g_current_screen = nullptr;
  }

  // 4. Summary
  std::cout << "\n========================================" << std::endl;
  std::cout << "Results: " << passed << " passed, " << failed << " failed, "
            << total << " total" << std::endl;
  if (!failures.empty()) {
    std::cout << "\nFailed tests:" << std::endl;
    for (const auto &f : failures) {
      std::cout << "  - " << f << std::endl;
    }
  }
  std::cout << "========================================" << std::endl;

  // 5. Cleanup
  for (const auto &e : afterhours::EntityHelper::get_entities()) {
    if (!e)
      continue;
    e->cleanup = true;
  }
  afterhours::EntityHelper::cleanup();

  {
    auto &ui_coll = afterhours::ui::UICollectionHolder::get().collection;
    for (const auto &e : ui_coll.get_entities()) {
      if (!e)
        continue;
      e->cleanup = true;
    }
    ui_coll.cleanup();
  }

  raylib::UnloadFont(uiFont);
  raylib::UnloadRenderTexture(screenRT);
  afterhours::graphics::shutdown();

  return failed;
}

// ---------------------------------------------------------------------------
// run_focus_ring_test -- for each screen, capture initial screenshot then
// inject Tab keypresses one at a time, capturing after each.
// ---------------------------------------------------------------------------
void run_focus_ring_test(const std::string &screen_filter, int max_tabs) {
  constexpr int WIDTH = 1280;
  constexpr int HEIGHT = 720;

  // 1. Init headless graphics
  afterhours::graphics::Config cfg;
  cfg.display = afterhours::graphics::DisplayMode::Headless;
  cfg.width = WIDTH;
  cfg.height = HEIGHT;
  cfg.title = "Focus Ring Test";
  cfg.target_fps = 60;

  if (!afterhours::graphics::init(cfg)) {
    log_error("[FocusTest] Failed to init graphics backend");
    return;
  }

  Settings::get().update_resolution(
      afterhours::window_manager::Resolution{.width = WIDTH, .height = HEIGHT});

  afterhours::files::init("Prime Pressure", "resources");

  mainRT = afterhours::graphics::get_render_texture();
  screenRT = raylib::LoadRenderTexture(WIDTH, HEIGHT);

  uiFont = load_font_headless(
      afterhours::files::get_resource_path("fonts", "Gaegu-Bold.ttf")
          .string()
          .c_str());

  configure_validation();
  setup_ecs_singletons(WIDTH, HEIGHT);

  int ui_entity_id = afterhours::EntityHelper::get_singleton<
                         afterhours::ui::UIContext<InputAction>>()
                         .get()
                         .id;

  // 2. Determine which screens to test
  std::vector<std::string> screen_names;
  if (screen_filter.empty()) {
    screen_names = ExampleScreenRegistry::get().get_screen_names();
  } else {
    if (ExampleScreenRegistry::get().has_screen(screen_filter)) {
      screen_names.push_back(screen_filter);
    } else {
      log_error("[FocusTest] Screen not found: {}", screen_filter);
      raylib::UnloadRenderTexture(screenRT);
      afterhours::graphics::shutdown();
      return;
    }
  }

  // 3. Create output directory
  std::filesystem::path base_dir =
      std::filesystem::path(g_headless_output_dir) / "focus_test";
  std::filesystem::create_directories(base_dir);

  log_info("[FocusTest] Testing {} screens, max {} tabs each",
           screen_names.size(), max_tabs);

  // Enable test input mode so synthetic key presses are consumed
  test_input::test_mode = true;
  afterhours::testing::test_input::detail::test_mode = true;

  for (const std::string &screen_name : screen_names) {
    // Reset state
    reset_screen_state(ui_entity_id);
    test_input::clear_queue();
    afterhours::testing::test_input::clear_queue();

    // Create per-screen output directory
    std::filesystem::path screen_dir = base_dir / screen_name;
    std::filesystem::create_directories(screen_dir);

    // Create systems for this screen
    afterhours::SystemManager systems = create_screen_systems(screen_name);

    // Verify font manager
    auto *font_mgr = afterhours::EntityHelper::get_singleton_cmp<
        afterhours::ui::FontManager>();
    if (font_mgr) {
      raylib::Font test_font =
          font_mgr->get_font(afterhours::ui::UIComponent::DEFAULT_FONT);
      if (test_font.glyphCount == 0 || test_font.glyphs == nullptr ||
          test_font.texture.id == 0) {
        log_error("[FocusTest] Font invalid! Skipping: {}", screen_name);
        continue;
      }
    }

    // Initial 2-pass stabilization (no input)
    for (int pass = 0; pass < 2; pass++) {
      {
        auto &entities = afterhours::EntityHelper::get_entities_for_mod();
        systems.tick_all(entities, 0.016f);
      }
      {
        auto &entities = afterhours::EntityHelper::get_entities_for_mod();
        systems.render(entities, 0.016f);
      }
      afterhours::EntityHelper::cleanup();
    }

    // Capture initial screenshot (no focus)
    raylib::rlDrawRenderBatchActive();
    {
      std::string filename = "tab_0.png";
      std::filesystem::path output_path = screen_dir / filename;
      afterhours::graphics::capture_frame(output_path);
      log_info("[FocusTest] {}: saved tab_0 (initial)", screen_name);
    }

    // Tab through elements
    for (int tab_n = 1; tab_n <= max_tabs; tab_n++) {
      // Inject tab keypress
      test_input::simulate_tab();
      afterhours::testing::test_input::simulate_tab();

      // Run 2 tick+render passes to allow focus to settle and layout to update
      for (int pass = 0; pass < 2; pass++) {
        test_input::reset_frame();
        afterhours::testing::test_input::reset_frame();
        {
          auto &entities = afterhours::EntityHelper::get_entities_for_mod();
          systems.tick_all(entities, 0.016f);
        }
        {
          auto &entities = afterhours::EntityHelper::get_entities_for_mod();
          systems.render(entities, 0.016f);
        }
        afterhours::EntityHelper::cleanup();
      }

      // Capture screenshot
      raylib::rlDrawRenderBatchActive();
      {
        std::string filename = "tab_" + std::to_string(tab_n) + ".png";
        std::filesystem::path output_path = screen_dir / filename;
        afterhours::graphics::capture_frame(output_path);
      }
    }

    log_info("[FocusTest] {}: captured {} screenshots", screen_name,
             max_tabs + 1);
  }

  // Cleanup
  test_input::test_mode = false;
  afterhours::testing::test_input::detail::test_mode = false;
  g_current_screen = nullptr;

  for (const auto &e : afterhours::EntityHelper::get_entities()) {
    if (!e)
      continue;
    e->cleanup = true;
  }
  afterhours::EntityHelper::cleanup();

  {
    auto &ui_coll = afterhours::ui::UICollectionHolder::get().collection;
    for (const auto &e : ui_coll.get_entities()) {
      if (!e)
        continue;
      e->cleanup = true;
    }
    ui_coll.cleanup();
  }

  raylib::UnloadFont(uiFont);
  raylib::UnloadRenderTexture(screenRT);
  afterhours::graphics::shutdown();

  log_info("[FocusTest] Complete - tested {} screens", screen_names.size());
}
