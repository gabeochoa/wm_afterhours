#include "headless_screenshots.h"

#include "engine/headless_gl.h"
#include "font_config.h"
#include "game.h"
#include "input_mapping.h"
#include "log.h"
#include "systems/ExampleScreenRegistry.h"
#include "systems/RenderSystemHelpers.h"

#include <afterhours/src/plugins/files.h>
#include <afterhours/src/plugins/modal.h>
#include <afterhours/src/plugins/toast.h>
#include <filesystem>

// Globals defined in game.cpp
extern raylib::RenderTexture2D mainRT;
extern raylib::RenderTexture2D screenRT;
extern raylib::Font uiFont;
extern afterhours::SystemBase *g_current_screen;

// Globals defined in main.cpp (declared in headless_screenshots.h)
// bool g_headless_mode and std::string g_headless_output_dir

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

  // Load font data (glyphs info)
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

  // Generate font atlas
  raylib::Image atlas = raylib::GenImageFontAtlas(font.glyphs, &font.recs,
                                                  font.glyphCount, fontSize, 1, 0);

  // Load texture from atlas image
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
        afterhours::files::get_resource_path("fonts", font_def.filename).string();

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

} // namespace

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

  // 4. Initialize files plugin (required for resource loading)
  afterhours::files::init("Prime Pressure", "resources");
  log_info("[Headless] Initialized files plugin");

  // 5. Create render textures
  mainRT = raylib::LoadRenderTexture(1920, 1080);
  screenRT = raylib::LoadRenderTexture(1920, 1080);
  log_info("[Headless] Created render textures");

  // 6. Load main font for legacy usage
  uiFont = load_font_headless(
      afterhours::files::get_resource_path("fonts", "Gaegu-Bold.ttf")
          .string()
          .c_str());
  log_info("[Headless] Loaded font (textureId: {})", uiFont.texture.id);

  // Configure UI validation
  configure_validation();

  // 7. Get all screen names
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

  // Create window_manager resolution singleton manually
  {
    afterhours::Entity &resolution_entity =
        afterhours::EntityHelper::createPermanentEntity();
    resolution_entity
        .addComponent<afterhours::window_manager::ProvidesCurrentResolution>(
            afterhours::window_manager::Resolution{.width = 1920, .height = 1080});
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
  }

  // Create UI singleton components manually
  {
    afterhours::Entity &ui_entity =
        afterhours::EntityHelper::createPermanentEntity();

    ui_entity.addComponent<afterhours::ui::UIContext<InputAction>>();
    afterhours::EntityHelper::registerSingleton<
        afterhours::ui::UIContext<InputAction>>(ui_entity);

    // Load all fonts using the shared font configuration
    auto &font_mgr = ui_entity.addComponent<afterhours::ui::FontManager>();
    load_fonts_into_manager(font_mgr);
    afterhours::EntityHelper::registerSingleton<afterhours::ui::FontManager>(
        ui_entity);

    // Add TextMeasureCache with measurement function
    auto &text_cache =
        ui_entity.addComponent<afterhours::ui::TextMeasureCache>();
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

    ui_entity.addComponent<afterhours::ui::UIComponent>(ui_entity.id)
        .set_desired_width(afterhours::ui::screen_pct(1.f))
        .set_desired_height(afterhours::ui::screen_pct(1.f))
        .enable_font(afterhours::ui::UIComponent::DEFAULT_FONT,
                     afterhours::ui::pixels(75.f));
    ui_entity.addComponent<afterhours::ui::AutoLayoutRoot>();
    ui_entity.addComponent<afterhours::ui::UIComponentDebug>("headless_root");
  }

  int ui_entity_id = afterhours::EntityHelper::get_singleton<
                         afterhours::ui::UIContext<InputAction>>()
                         .get()
                         .id;

  // Create input singleton components
  {
    afterhours::Entity &input_entity =
        afterhours::EntityHelper::createPermanentEntity();
    afterhours::input::add_singleton_components(input_entity);
  }

  // 8. Iterate all screens and capture screenshots
  for (const std::string &screen_name : screen_names) {
    // Reset UI state between screens
    auto *ui_context = afterhours::EntityHelper::get_singleton_cmp<
        afterhours::ui::UIContext<InputAction>>();
    if (ui_context) {
      ui_context->reset();
    }

    // Clean up UI entities (except permanent root)
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

    // Reset root entity's children
    auto &root_entity = afterhours::EntityHelper::get_singleton<
                            afterhours::ui::UIContext<InputAction>>()
                            .get();
    if (root_entity.has<afterhours::ui::UIComponent>()) {
      root_entity.get<afterhours::ui::UIComponent>().children.clear();
    }

    // Create fresh system manager for this screen
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
      continue;
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

    // Verify font manager
    auto *font_mgr =
        afterhours::EntityHelper::get_singleton_cmp<afterhours::ui::FontManager>();
    if (font_mgr) {
      raylib::Font test_font =
          font_mgr->get_font(afterhours::ui::UIComponent::DEFAULT_FONT);
      if (test_font.glyphCount == 0 || test_font.glyphs == nullptr ||
          test_font.texture.id == 0) {
        log_error(
            "[Headless] Font invalid! textureId={}, glyphCount={}, Skipping: {}",
            test_font.texture.id, test_font.glyphCount, screen_name);
        continue;
      }
    }

    // Run update systems
    {
      auto &entities = afterhours::EntityHelper::get_entities_for_mod();
      systems.tick_all(entities, 0.016f);
      afterhours::EntityHelper::cleanup();
    }

    // Run render systems
    {
      const auto &entities = afterhours::EntityHelper::get_entities();
      systems.render(entities, 0.016f);
    }

    // Ensure GPU operations complete
    raylib::rlDrawRenderBatchActive();
    glFlush();
    glFinish();

    // Save screenshot
    std::string path = g_headless_output_dir + "/" + screen_name + ".png";
    {
      glBindFramebuffer(GL_FRAMEBUFFER, mainRT.id);

      unsigned char *pixels = (unsigned char *)RL_MALLOC(1920 * 1080 * 4);
      glReadPixels(0, 0, 1920, 1080, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

      raylib::Image image;
      image.data = pixels;
      image.width = 1920;
      image.height = 1080;
      image.format = raylib::PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
      image.mipmaps = 1;

      raylib::ImageFlipVertical(&image);
      raylib::ExportImage(image, path.c_str());
      raylib::UnloadImage(image);
    }
    log_info("[Headless] Saved: {}", path);
  }

  // 9. Cleanup
  g_current_screen = nullptr;

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
