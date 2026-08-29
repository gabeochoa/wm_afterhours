#include "preload.h"

#include <afterhours/src/gestures_macos.h>

#include <cstdarg>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <vector>

#include "font_config.h"
#include "log.h"
#include "rl.h"

#include "input_mapping.h"
#include "settings.h"
#include <afterhours/src/graphics.h>
#include <afterhours/src/plugins/color.h>
#include <afterhours/src/plugins/files.h>
#include <afterhours/src/plugins/toast.h>
#include <afterhours/src/plugins/ui/theme.h>

using namespace afterhours;

#ifdef AFTER_HOURS_ENABLE_MCP
extern bool g_mcp_mode;

// Custom log callback that writes to stderr instead of stdout
static void mcp_trace_log_callback(int logLevel, const char *text,
                                   va_list args) {
  if (logLevel < raylib::LOG_ERROR) {
    return; // In MCP mode, only log errors
  }
  char buffer[1024];
  vsnprintf(buffer, sizeof(buffer), text, args);
  fprintf(stderr, "%s\n", buffer);
}
#endif

static void load_gamepad_mappings() {
  std::ifstream ifs(
      files::get_resource_path("", "gamecontrollerdb.txt").string().c_str());
  if (!ifs.is_open()) {
    log_warn("failed to load game controller db");
    return;
  }
  std::stringstream buffer;
  buffer << ifs.rdbuf();
  input::set_gamepad_mappings(buffer.str().c_str());
}

Preload::Preload() {}

Preload &Preload::init(const char *title) {
  files::init("Prime Pressure", "resources");

  int width = Settings::get().get_screen_width();
  int height = Settings::get().get_screen_height();

  // In MCP mode, redirect raylib logs to stderr to keep stdout clean for JSON
#ifdef AFTER_HOURS_ENABLE_MCP
  if (g_mcp_mode) {
    raylib::SetTraceLogCallback(mcp_trace_log_callback);
  }
#endif

  // Set log level BEFORE InitWindow to suppress init messages
  raylib::SetTraceLogLevel(raylib::LOG_ERROR);

  raylib::InitWindow(width, height, title);

  // The pinch monitor is app-wide, but installing it before there is an app to
  // monitor is pointless -- do it once the window exists.
  afterhours::gestures::install_pinch_monitor();
  raylib::SetWindowSize(width, height);
  raylib::SetWindowState(raylib::FLAG_WINDOW_RESIZABLE);

  raylib::SetTargetFPS(200);

  raylib::SetAudioStreamBufferSizeDefault(4096);
  raylib::InitAudioDevice();
  if (!raylib::IsAudioDeviceReady()) {
    log_warn("audio device not ready; continuing without audio");
  }
  raylib::SetMasterVolume(1.f);

  raylib::SetExitKey(0);

  load_gamepad_mappings();

  return *this;
}

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

  // The same ranges the windowed loader uses, or headless renders ASCII while
  // the real window renders arrows and chevrons, and a screenshot test can
  // never see the difference.
  std::vector<int> cps = afterhours::default_codepoints();
  font.baseSize = fontSize;
  font.glyphCount = static_cast<int>(cps.size());
  font.glyphPadding = 1;
  font.glyphs =
      raylib::LoadFontData(fontData, dataSize, fontSize, cps.data(),
                           static_cast<int>(cps.size()), raylib::FONT_DEFAULT);

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

// Load font with specific codepoints for CJK support in headless mode
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

} // namespace

void apply_ui_styling_defaults() {
  ui::imm::ThemeDefaults::get()
      .set_theme_color(ui::Theme::Usage::Primary, colors::UI_GREEN)
      .set_theme_color(ui::Theme::Usage::Error, colors::UI_RED)
      .set_theme_color(ui::Theme::Usage::Font, colors::UI_WHITE)
      .set_theme_color(ui::Theme::Usage::DarkFont,
                       afterhours::Color{30, 30, 30, 255})
      .set_theme_color(ui::Theme::Usage::Background, colors::UI_BLACK)
      .set_theme_color(ui::Theme::Usage::Surface,
                       afterhours::Color{40, 40, 50, 255})
      .set_theme_color(ui::Theme::Usage::Secondary, raylib::YELLOW)
      .set_theme_color(ui::Theme::Usage::Accent, raylib::GREEN);

  // These screens were laid out against the 5px the renderer used to reserve
  // inside every label, so they ask for it back rather than each growing its
  // own padding. A new screen should prefer real padding.
  ui::imm::ThemeDefaults::get().theme.text_inset = {5.f, 5.f};

  // Without this every screen that does not name a font warns once per text
  // element. base() is what ComponentConfig already defaults to, so this sets
  // the name without changing any size.
  ui::imm::UIStylingDefaults::get().set_default_font(
      ui::UIComponent::DEFAULT_FONT, ui::imm::TypographyScale::base());

  ui::imm::UIStylingDefaults::get().set_grid_snapping(true);
}

Preload &Preload::make_singleton() {
  auto &sophie = EntityHelper::createEntity();
  {
    input::add_singleton_components(sophie, get_mapping());
    window_manager::add_singleton_components(
        sophie,
        window_manager::Resolution{.width = Settings::get().get_screen_width(),
                                   .height = Settings::get().get_screen_height()},
        200);
    ui::init_ui_plugin<InputAction>();

    // Load all fonts using the shared font configuration
    // In headless mode, use special font loading that works without a window
    auto &font_mgr = *EntityHelper::get_singleton_cmp<ui::FontManager>();
    bool is_headless = afterhours::graphics::is_headless();

    for (const auto &font_def : font_config::get_all_fonts()) {
      std::string path =
          files::get_resource_path("fonts", font_def.filename).string();

      if (is_headless) {
        // Headless mode: use manual font loading that creates textures properly
        if (font_def.needs_codepoints && font_def.get_codepoints) {
          auto codepoints = font_def.get_codepoints();
          raylib::Font font = load_font_headless_with_codepoints(
              path.c_str(), codepoints.data(),
              static_cast<int>(codepoints.size()));
          if (font.glyphCount > 0) {
            font_mgr.load_font(font_def.name, font);
          } else {
            log_warn("[Preload] Failed to load CJK font in headless: {}",
                     font_def.name);
          }
        } else {
          raylib::Font font = load_font_headless(path.c_str());
          if (font.glyphCount > 0) {
            font_mgr.load_font(font_def.name, font);
          } else {
            log_warn("[Preload] Failed to load font in headless: {}",
                     font_def.name);
          }
        }
      } else {
        // Normal windowed mode: use standard raylib font loading
        if (font_def.needs_codepoints && font_def.get_codepoints) {
          auto codepoints = font_def.get_codepoints();
          font_mgr.load_font_with_codepoints(
              font_def.name, path.c_str(), codepoints.data(),
              static_cast<int>(codepoints.size()));
        } else {
          font_mgr.load_font(font_def.name, path.c_str());
        }
      }
    }

    apply_ui_styling_defaults();
  }
  return *this;
}

Preload::~Preload() {
  if (raylib::IsAudioDeviceReady()) {
    raylib::CloseAudioDevice();
  }
  if (raylib::IsWindowReady()) {
    raylib::CloseWindow();
  }
}
