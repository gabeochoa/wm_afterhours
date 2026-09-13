#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/clipboard.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct FormsGallery : ScreenSystem<UIContext<InputAction>> {
  // Slider values
  float volume_slider = 0.75f;
  float brightness_slider = 0.5f;
  float difficulty_slider = 0.3f;

  // Checkbox values
  bool enable_music = true;
  bool enable_sfx = true;
  bool fullscreen = false;
  bool vsync = true;
  bool show_fps = false;
  bool show_hud = true;
  bool auto_save = true;
  bool subtitles = false;
  bool motion_blur = true;
  bool anti_aliasing = true;
  bool ambient_occlusion = false;
  bool bloom_effect = true;

  // Dropdown values
  size_t resolution_index = 2;
  size_t quality_index = 1;
  size_t language_index = 0;

  std::vector<std::string> resolutions = {"640x480", "1280x720", "1920x1080",
                                          "2560x1440", "3840x2160"};
  std::vector<std::string> quality_options = {"Low", "Medium", "High", "Ultra"};
  std::vector<std::string> languages = {"English", "Spanish",  "French",
                                        "German",  "Japanese", "Korean"};

  // Clipboard demo state
  std::string clipboard_display = "(clipboard empty)";

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();
    context.theme.primary = {88, 186, 183, 255};
    context.theme.secondary = {49, 64, 82, 255};
    UIStylingDefaults::get().set_grid_snapping(false);
    const float s = std::min(context.screen_width / 1280.f,
                             context.screen_height / 720.f);
    const float ox = (context.screen_width - 1280 * s) / 2;
    const float oy = (context.screen_height - 720 * s) / 2;
    const afterhours::Color ink{233, 241, 249, 255};
    const afterhours::Color muted{161, 182, 202, 255};
    const afterhours::Color panel{24, 35, 49, 255};
    const auto at = [=](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(ox + x * s, oy + y * s).with_corner_radius(0);
    };
    const auto text = [&](int id, const std::string &value, float x, float y,
                          float w, float h, float size, afterhours::Color color,
                          const std::string &name) {
      div(context, mk(entity, id), at(x, y, w, h).with_label(value)
          .with_font("AtkinsonMock", pixels(size * s)).with_custom_text_color(color)
          .with_background(Theme::Usage::None).with_ignore_pointer_events()
          .with_debug_name(name));
    };
    div(context, mk(entity, 0), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background({14, 22, 33, 255}).with_corner_radius(0)
        .with_debug_name("forms_bg"));
    text(1, "Form Components", 48, 20, 1184, 44, 34, ink, "forms_title");
    text(2, "Adjust the demo controls. Copy a settings summary or read your clipboard.",
         48, 70, 1184, 28, 20, muted, "forms_subtitle");
    div(context, mk(entity, 3), at(48, 110, 564, 390)
        .with_custom_background(panel).with_corner_radius(12 * s)
        .with_debug_name("left_column"));
    div(context, mk(entity, 4), at(636, 110, 596, 390)
        .with_custom_background(panel).with_corner_radius(12 * s)
        .with_debug_name("right_column"));
    text(5, "Sliders", 68, 120, 524, 31, 24, ink, "forms_sliders_heading");
    const auto slider_row = [&](int id, const std::string &name, const char *debug,
                                float y, float &value) {
      text(id, name, 68, y, 420, 27, 20, ink, std::string(debug) + "_label");
      const float drawn_value = value;
      auto result = slider(context, mk(entity, id + 1), value,
          at(84, y + 26, 492, 32).with_custom_background({0, 0, 0, 0})
              .with_debug_name(debug).with_on_draw_fg([=](RectangleType r) {
                const float width = std::max(0.f, r.width - 6.f);
                const float center_y = r.y + r.height / 2;
                const float center_x = r.x + width * drawn_value;
                raylib::DrawRectangleRounded({r.x, center_y - 3 * s, width, 6 * s},
                                              1, 8, {53, 75, 96, 255});
                if (drawn_value > 0) {
                  raylib::DrawRectangleRounded({r.x, center_y - 3 * s,
                                                width * drawn_value, 6 * s},
                                                1, 8, {88, 186, 183, 255});
                }
                raylib::DrawCircleV({center_x, center_y}, 11 * s, {13, 31, 44, 255});
                raylib::DrawCircleV({center_x, center_y}, 9 * s, {239, 250, 247, 255});
                raylib::DrawCircleV({center_x, center_y}, 3 * s, {53, 111, 115, 255});
              }));
      for (const auto child_id : result.cmp().children) {
        auto child = UICollectionHolder::getEntityForID(child_id);
        if (!child.valid() || !child.asE().has<HasSliderState>()) continue;
        child.asE().addComponentIfMissing<UIComponentDebug>(std::string(debug) + "_input")
            .set(std::string(debug) + "_input");
      }
      text(id + 2, fmt::format("{}%", static_cast<int>(value * 100)),
           512, y, 80, 27, 20, ink, std::string(debug) + "_value");
    };
    slider_row(20, "Volume", "volume_slider", 150, volume_slider);
    slider_row(30, "Brightness", "brightness_slider", 216, brightness_slider);
    slider_row(40, "Difficulty", "difficulty_slider", 282, difficulty_slider);
    text(50, "Easy", 68, 337, 180, 23, 17, muted, "forms_easy");
    text(51, "Hard", 538, 337, 56, 23, 17, muted, "forms_hard");
    for (int i = 0; i < 2; ++i) {
      div(context, mk(entity, 52 + i), at(i == 0 ? 60 : 580, 244, 20, 24)
          .with_background(Theme::Usage::None).with_ignore_pointer_events()
          .with_on_draw_fg([=](RectangleType r) {
            const raylib::Vector2 center{r.x + r.width / 2, r.y + r.height / 2};
            const float radius = (i == 0 ? 3 : 5) * s;
            raylib::DrawCircleV(center, radius, {199, 223, 223, 255});
            if (i == 0) return;
            for (int ray = 0; ray < 8; ++ray) {
              const float angle = static_cast<float>(ray) * 3.14159265f / 4;
              raylib::DrawLineEx({center.x + std::cos(angle) * 7 * s,
                                  center.y + std::sin(angle) * 7 * s},
                                 {center.x + std::cos(angle) * 10 * s,
                                  center.y + std::sin(angle) * 10 * s}, s,
                                 {199, 223, 223, 255});
            }
          }).with_debug_name(i == 0 ? "forms_dim_icon" : "forms_bright_icon"));
    }
    text(60, "Progress Bars", 68, 367, 524, 29, 24, ink, "forms_progress_heading");
    text(61, "Audio Level / percentage", 68, 401, 420, 25, 19, muted, "forms_audio_label");
    text(62, fmt::format("{}%", static_cast<int>(volume_slider * 100)),
         512, 401, 80, 25, 19, ink, "forms_audio_value");
    // Progress bar showing volume value (dynamic)
    progress_bar(context, mk(entity, 63), volume_slider,
        at(68, 428, 524, 8).with_corner_radius(4 * s)
            .with_debug_name("volume_progress"), ProgressBarLabelStyle::None);
    text(64, "Level Progress / custom range", 68, 450, 420, 25, 19, muted, "forms_level_label");
    text(65, "75/100", 506, 450, 86, 25, 19, ink, "forms_level_value");
    // Progress bar with custom range
    progress_bar(context, mk(entity, 66), 75.f,
        at(68, 478, 524, 8).with_corner_radius(4 * s)
            .with_debug_name("level_progress"), ProgressBarLabelStyle::None, 0.f, 100.f);
    text(70, "Checkboxes", 660, 120, 268, 31, 24, ink, "forms_checkboxes_heading");
    text(74, fmt::format("Music {} / effects {}", enable_music ? "on" : "off", enable_sfx ? "on" : "off"),
         940, 125, 268, 26, 17, muted, "forms_audio_state");
    auto options = vstack(context, mk(entity, 71), at(660, 164, 548, 290)
        .with_background(Theme::Usage::None).with_no_wrap()
        .with_overflow(Overflow::Scroll, Axis::Y)
        .with_debug_name("forms_options"));
    auto &scroll = options.ent().get<HasScrollView>();
    scroll.scroll_speed = 32 * s;
    scroll.scrollbar_thickness = pixels(8 * s);
    scroll.scrollbar_track_color = afterhours::Color{55, 76, 95, 255};
    scroll.scrollbar_thumb_color = afterhours::Color{180, 205, 222, 255};
    const auto group = [&](int id, const std::string &value) {
      div(context, mk(options.ent(), id), ComponentConfig{}
          .with_size({pixels(524 * s), pixels(28 * s)})
          .with_label(value).with_font("AtkinsonMock", pixels(17 * s))
          .with_custom_text_color(muted).with_background(Theme::Usage::None)
          .with_ignore_pointer_events());
    };
    const auto checkbox_row = [&](int id, const std::string &name,
                                  const std::string &debug, bool &value,
                                  bool disabled = false) {
      const bool checked = value;
      auto result = checkbox(context, mk(options.ent(), id), value,
          ComponentConfig{}.with_size({pixels(524 * s), pixels(44 * s)})
              .with_label(name).with_font("AtkinsonMock", pixels(20 * s))
              .with_custom_text_color(ink).with_alignment(TextAlignment::Left)
              .with_checkbox_indicators("", "")
              .with_custom_background({0, 0, 0, 0}).with_corner_radius(0)
              .with_disabled(disabled).with_debug_name(debug)
              .with_on_draw_fg([=](RectangleType r) {
                const auto edge = disabled ? raylib::Color{98, 117, 134, 255}
                                           : raylib::Color{186, 210, 224, 255};
                const float x = r.x + 10 * s;
                const float y = r.y + 10 * s;
                raylib::DrawRectangleRec({x, y, 24 * s, 24 * s}, edge);
                raylib::DrawRectangleRec({x + 2 * s, y + 2 * s, 20 * s, 20 * s},
                                         checked ? raylib::Color{63, 135, 137, 255}
                                                 : raylib::Color{24, 35, 49, 255});
                if (!checked) return;
                raylib::DrawLineEx({x + 5 * s, y + 12 * s},
                                   {x + 10 * s, y + 17 * s}, 3 * s, edge);
                raylib::DrawLineEx({x + 10 * s, y + 17 * s},
                                   {x + 20 * s, y + 6 * s}, 3 * s, edge);
              }));
      const auto &children = result.cmp().children;
      if (children.size() != 2) return;
      auto &label = UICollectionHolder::getEntityForIDEnforce(children[0]).get<UIComponent>();
      label.set_desired_width(pixels(480 * s));
      label.set_desired_margin(pixels(44 * s), Axis::left);
      auto &toggle = UICollectionHolder::getEntityForIDEnforce(children[1]);
      auto &toggle_cmp = toggle.get<UIComponent>();
      toggle_cmp.set_desired_width(pixels(44 * s));
      toggle_cmp.absolute = true;
      toggle.addComponentIfMissing<UIComponentDebug>(debug + "_toggle").set(debug + "_toggle");
    };
    group(0, "Audio");
    checkbox_row(1, "Music", "forms_music", enable_music);
    checkbox_row(2, "Sound effects", "forms_sfx", enable_sfx);
    group(3, "Display");
    checkbox_row(4, "Fullscreen", "forms_fullscreen", fullscreen);
    checkbox_row(5, "V-Sync", "forms_vsync", vsync);
    div(context, mk(options.ent(), 6), ComponentConfig{}
        .with_size({pixels(524 * s), pixels(28 * s)})
        .with_label("Synchronize frames with the display.")
        .with_font("AtkinsonMock", pixels(17 * s)).with_custom_text_color(muted)
        .with_background(Theme::Usage::None).with_ignore_pointer_events());
    checkbox_row(7, "Show FPS", "forms_fps", show_fps);
    checkbox_row(8, "Show HUD", "forms_hud", show_hud);
    group(9, "Gameplay and graphics");
    checkbox_row(10, "Auto Save", "forms_auto_save", auto_save);
    checkbox_row(11, "Subtitles", "forms_subtitles", subtitles);
    checkbox_row(12, "Motion Blur", "forms_motion_blur", motion_blur);
    checkbox_row(13, "Anti-Aliasing", "forms_antialiasing", anti_aliasing);
    checkbox_row(14, "Ambient Occlusion", "forms_ambient", ambient_occlusion);
    checkbox_row(15, "Bloom Effect", "forms_bloom", bloom_effect);
    bool disabled_value = true;
    checkbox_row(16, "Disabled", "forms_disabled", disabled_value, true);
    group(17, "Language");
    dropdown(context, mk(options.ent(), 18), languages, language_index,
        ComponentConfig{}.with_size({pixels(524 * s), pixels(40 * s)})
            .with_font("AtkinsonMock", pixels(20 * s))
            .with_custom_background({42, 67, 88, 255}).with_custom_text_color(ink)
            .with_corner_radius(6 * s).with_debug_name("language_dropdown"));
    const float max_scroll = std::max(0.f, scroll.content_size.y - options.cmp().rect().height);
    text(72, scroll.scroll_offset.y + s < max_scroll ? "More options below / scroll this list"
                                                   : "End of options / scroll up to Audio",
         660, 466, 548, 26, 17, muted, "forms_scroll_hint");
    const std::string status = "Volume: " + std::to_string(static_cast<int>(volume_slider * 100)) +
        "% | Resolution: " + resolutions[resolution_index] +
        " | Quality: " + quality_options[quality_index];
    div(context, mk(entity, 80), at(48, 516, 1184, 63)
        .with_custom_background(panel).with_corner_radius(10 * s)
        .with_debug_name("forms_status_panel"));
    text(81, "Current settings", 64, 533, 228, 30, 22, ink, "forms_status_heading");
    text(82, "Volume", 316, 522, 210, 23, 16, muted, "forms_status_volume_label");
    text(83, fmt::format("{}%", static_cast<int>(volume_slider * 100)),
         316, 546, 210, 27, 22, ink, "forms_status_volume");
    text(84, "Resolution / demo preset", 596, 522, 290, 23, 16, muted, "forms_status_resolution_label");
    text(85, resolutions[resolution_index], 596, 546, 290, 27, 22, ink, "forms_status_resolution");
    text(86, "Quality / demo preset", 952, 522, 250, 23, 16, muted, "forms_status_quality_label");
    text(87, quality_options[quality_index], 952, 546, 250, 27, 22, ink, "forms_status_quality");
    div(context, mk(entity, 90), at(48, 596, 1184, 106)
        .with_custom_background(panel).with_corner_radius(10 * s)
        .with_debug_name("clipboard_row"));
    text(91, "Copy saves the summary as text.", 64, 604, 372, 23, 18, muted, "forms_clipboard_copy_help");
    text(92, "Paste reads text; it does not apply settings.", 64, 628, 388, 23, 17, muted, "forms_clipboard_paste_help");
    // Copy button - copies current status to clipboard
    if (button(context, mk(entity, 93), at(64, 658, 162, 36)
        .with_label("Copy Status").with_font("AtkinsonMock", pixels(19 * s))
        .with_custom_background({85, 181, 176, 255}).with_custom_text_color({12, 30, 38, 255})
        .with_corner_radius(6 * s).with_debug_name("copy_btn"))) {
      afterhours::clipboard::set_text(status);
      clipboard_display = "Copied!";
    }
    // Paste button - reads from clipboard
    if (button(context, mk(entity, 94), at(238, 658, 128, 36)
        .with_label("Paste").with_font("AtkinsonMock", pixels(19 * s))
        .with_custom_background({48, 71, 93, 255}).with_custom_text_color(ink)
        .with_corner_radius(6 * s).with_debug_name("paste_btn"))) {
      clipboard_display = afterhours::clipboard::has_text()
          ? afterhours::clipboard::get_text() : "(clipboard empty)";
    }
    text(95, "Clipboard result / read only", 466, 604, 746, 25, 18, muted, "forms_clipboard_label");
    const std::string display_text = clipboard_display.length() > 140
        ? clipboard_display.substr(0, 137) + "..." : clipboard_display;
    div(context, mk(entity, 96), at(466, 636, 746, 58)
        .with_label(display_text).with_font("AtkinsonMock", pixels(19 * s))
        .with_custom_background({14, 25, 38, 255}).with_custom_text_color(ink)
        .with_text_overflow(TextOverflow::Wrap).with_text_inset(10 * s)
        .with_corner_radius(6 * s).with_debug_name("forms_clipboard_output"));
  }
};

REGISTER_EXAMPLE_SCREEN(forms, "Component Galleries",
                        "Form components: sliders, checkboxes, dropdowns",
                        FormsGallery)
