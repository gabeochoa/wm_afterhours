#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <string>
#include <vector>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct SportsSettingsScreen : ScreenSystem<UIContext<InputAction>> {
  struct SettingRow {
    std::string label;
    bool is_slider;
    int slider_value;
    std::vector<std::string> options;
    size_t option_idx;
  };

  const std::array<std::vector<SettingRow>, 4> defaults() const {
    return {gameplay_settings, video_settings, audio_settings,
            controls_settings};
  }
  raylib::Texture2D scene{};
  bool scene_loaded = false;
  bool settings_open = true;
  size_t active_tab = 1;
  size_t selected_row = 4;
  std::string status = "Applied locally";

  std::array<std::string, 4> tabs = {"GAMEPLAY", "VIDEO", "AUDIO", "CONTROLS"};
  std::vector<SettingRow> video_settings = {
      {"Window mode",
       false,
       0,
       {"Fullscreen", "Borderless window", "Windowed"},
       1},
      {"Resolution",
       false,
       0,
       {"1920 × 1080", "2560 × 1440", "3840 × 2160"},
       1},
      {"Screen percentage", true, 100, {}, 0},
      {"V Sync", false, 0, {"Off", "On"}, 1},
      {"Max FPS", true, 50, {}, 0},
      {"Gamma", true, 32, {}, 0},
      {"Anti-Aliasing method",
       false,
       0,
       {"Off", "FXAA", "TAA", "TSR", "DLSS"},
       3},
      {"Dynamic resolution", false, 0, {"Off", "On"}, 1},
      {"Framerate target", true, 25, {}, 0},
      {"Motion blur", true, 50, {}, 0},
      {"Graphics quality", false, 0, {"Low", "Medium", "High", "Ultra"}, 3},
      {"Texture quality", false, 0, {"Low", "Medium", "High", "Ultra"}, 3},
  };
  std::vector<SettingRow> gameplay_settings = {
      {"Difficulty", false, 0, {"Amateur", "Professional", "World Class"}, 1},
      {"Game speed", false, 0, {"Slow", "Normal", "Fast"}, 1},
      {"Match length", false, 0, {"4 min", "8 min", "15 min"}, 1},
      {"Camera height", true, 50, {}, 0},
      {"Camera zoom", true, 50, {}, 0},
  };
  std::vector<SettingRow> audio_settings = {
      {"Master volume", true, 80, {}, 0},     {"Music volume", true, 70, {}, 0},
      {"Commentary volume", true, 90, {}, 0}, {"Crowd volume", true, 85, {}, 0},
      {"SFX volume", true, 75, {}, 0},
  };
  std::vector<SettingRow> controls_settings = {
      {"Vibration", false, 0, {"Off", "On"}, 1},
      {"Vibration strength", true, 70, {}, 0},
      {"Pass assistance", false, 0, {"Manual", "Semi", "Assisted"}, 2},
      {"Shot assistance", false, 0, {"Manual", "Semi", "Assisted"}, 1},
  };

  const std::array<std::vector<SettingRow>, 4> initial_settings = defaults();

  const afterhours::Color white{223, 237, 245, 255};
  const afterhours::Color dark{25, 44, 56, 230};
  const afterhours::Color darker{24, 38, 47, 240};
  const afterhours::Color green{92, 255, 162, 255};
  const afterhours::Color green_dark{19, 58, 45, 255};
  const afterhours::Color muted{156, 177, 189, 255};

  ComponentConfig box(float scale, float x, float y, float w, float h) const {
    return ComponentConfig{}
        .with_size({pixels(w * scale), pixels(h * scale)})
        .with_absolute_position(x * scale, y * scale)
        .with_background(Theme::Usage::None)
        .with_corner_radius(0)
        .with_letter_spacing(-.3f * scale);
  }

  std::vector<SettingRow> &rows() {
    if (active_tab == 0)
      return gameplay_settings;
    if (active_tab == 2)
      return audio_settings;
    if (active_tab == 3)
      return controls_settings;
    return video_settings;
  }

  std::string heading() const {
    if (active_tab == 0)
      return "GAMEPLAY";
    if (active_tab == 2)
      return "AUDIO";
    if (active_tab == 3)
      return "CONTROLS";
    return "VIDEO SETTINGS";
  }

  std::string value_for(const SettingRow &row) const {
    if (row.is_slider) {
      if (row.label == "Max FPS" || row.label == "Framerate target")
        return std::to_string(
            static_cast<int>(std::round(row.slider_value * 2.4f))) + " FPS";
      if (row.label == "Gamma")
        return fmt::format("{:.2f}", 1.0f + row.slider_value * 0.0375f);
      if (row.label == "Motion blur")
        return std::to_string(row.slider_value * 2) + "%";
      return std::to_string(row.slider_value) + "%";
    }
    if (!row.options.empty())
      return row.options[row.option_idx];
    return "---";
  }

  std::string description_for(const SettingRow &row) const {
    if (row.label == "Max FPS")
      return "The maximum frames displayed per second. This local preview does not change the running application's frame limit.";
    if (row.label == "Framerate target")
      return "Dynamic resolution adjusts image quality to aim for this frame rate. It takes effect only while Dynamic resolution is On.";
    if (row.label == "Anti-Aliasing method") {
      const std::array<const char *, 5> help{
          "Off: no anti-aliasing.", "FXAA: Fast Approximate Anti-Aliasing.",
          "TAA: Temporal Anti-Aliasing combines information from previous frames.",
          "TSR: Temporal Super Resolution reconstructs a sharper image using previous frames.",
          "DLSS: Deep Learning Super Sampling. This is a local settings preview, not a GPU capability check."};
      return help[row.option_idx];
    }
    if (row.label == "Motion blur")
      return "Motion blur intensity: 0% disables the effect, 100% is the default, and 200% is the strongest preview setting.";
    if (row.label == "Texture quality" || row.label == "Graphics quality")
      return "Graphics quality is a preset preview. Texture quality is an independent override; changing one does not change the other.";
    if (row.label == "Resolution") return "Set the resolution of the rendered image in this local settings preview.";
    if (row.label == "Gamma") return "Adjust the brightness response. The local preview range is 1.0 to 4.75; the default is 2.2.";
    return "Adjust " + row.label + ". Changes apply to this local preview immediately.";
  }

  std::string range_for(const SettingRow &row) const {
    if (!row.is_slider) return "Click to cycle / Left and Right to adjust.";
    if (row.label == "Max FPS" || row.label == "Framerate target") return "Range: 0–240 FPS / no unlimited option";
    if (row.label == "Gamma") return "Range: 1.0–4.75";
    if (row.label == "Motion blur") return "Range: 0–200% intensity";
    return "Range: 0–100%";
  }

  void reset_current() {
    rows() = initial_settings[active_tab];
    selected_row = 0;
    status = tabs[active_tab] + " defaults restored";
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    const float screen_w =
        context.screen_width > 0.f
            ? context.screen_width
            : static_cast<float>(Settings::get().get_screen_width());
    const float screen_h =
        context.screen_height > 0.f
            ? context.screen_height
            : static_cast<float>(Settings::get().get_screen_height());
    const float scale = std::min(screen_w / 1280.f, screen_h / 720.f);
    auto &current = rows();
    if (selected_row >= current.size())
      selected_row = 0;

    if (settings_open && afterhours::input::is_key_pressed(raylib::KEY_Y)) reset_current();
    if (settings_open && afterhours::input::is_key_pressed(raylib::KEY_B)) settings_open = false;
    if (settings_open && context.pressed(InputAction::WidgetDown))
      selected_row = (selected_row + 1) % current.size();
    if (settings_open && context.pressed(InputAction::WidgetUp))
      selected_row = (selected_row + current.size() - 1) % current.size();
    auto focused = UICollectionHolder::getEntityForID(context.focus_id);
    const bool native_slider_focused = focused.valid() && focused.asE().has<HasSliderState>();
    if (settings_open && !native_slider_focused && context.pressed(InputAction::WidgetRight)) {
      auto &row = current[selected_row];
      status = "Applied locally";
      if (row.is_slider)
        row.slider_value = std::min(100, row.slider_value + 5);
      else
        row.option_idx = (row.option_idx + 1) % row.options.size();
    }
    if (settings_open && !native_slider_focused && context.pressed(InputAction::WidgetLeft)) {
      auto &row = current[selected_row];
      status = "Applied locally";
      if (row.is_slider)
        row.slider_value = std::max(0, row.slider_value - 5);
      else
        row.option_idx =
            (row.option_idx + row.options.size() - 1) % row.options.size();
    }

    Theme theme;
    theme.font = white;
    theme.darkfont = green_dark;
    theme.font_muted = muted;
    theme.background = dark;
    theme.surface = darker;
    theme.primary = green;
    theme.secondary = muted;
    theme.accent = green;
    theme.roundness = 0.f;
    theme.segments = 4;
    context.set_theme(theme);
    context.scaling_mode = ScalingMode::Proportional;
    UIStylingDefaults::get().set_grid_snapping(false);
    UIStylingDefaults::get().set_default_font("AtkinsonMock", pixels(21.f * scale));

    if (!scene_loaded) {
      scene_loaded = true;
      scene = raylib::LoadTexture(afterhours::files::get_resource_path(
                                      "images", "sports_settings/scene.png")
                                      .string()
                                      .c_str());
      raylib::SetTextureFilter(scene, raylib::TEXTURE_FILTER_BILINEAR);
    }
    auto root =
        div(context, mk(entity, 0),
            box(scale, 0, 0, 1280, 720)
                .with_absolute_position((screen_w - 1280 * scale) / 2, (screen_h - 720 * scale) / 2)
                .with_on_draw_bg([this](RectangleType r) {
                  if (scene.id)
                    raylib::DrawTexturePro(
                        scene, {0, 0, (float)scene.width, (float)scene.height},
                        r, {0, 0}, 0, raylib::WHITE);
                  afterhours::draw_rectangle_gradient_h(
                      r, afterhours::Color{24, 44, 56, 246},
                      afterhours::Color{27, 53, 71, 125});
                })
                .with_debug_name("sports_root"));
    if (afterhours::input::is_key_pressed(raylib::KEY_ESCAPE))
      settings_open = !settings_open;
    if (!settings_open) {
      div(context, mk(root.ent(), 1200),
          box(scale, 390, 250, 500, 70)
              .with_label("Settings closed")
              .with_font("AtkinsonMock", pixels(34.f * scale))
              .with_custom_text_color(white)
              .with_alignment(TextAlignment::Center));
      if (button(context, mk(root.ent(), 1201),
                 box(scale, 490, 355, 300, 60)
                     .with_label("Return to settings")
                     .with_custom_background(green)
                     .with_custom_text_color(green_dark)
                     .with_font("AtkinsonMock", pixels(25.f * scale))
                     .with_alignment(TextAlignment::Center)
                     .with_debug_name("sports_reopen")))
        settings_open = true;
      return;
    }

    div(context, mk(root.ent(), 10),
        box(scale, 0, 0, 1280, 50)
            .with_custom_background(darker)
            .with_debug_name("sports_tabs"));
    div(context, mk(root.ent(), 11),
        box(scale, 30, 11, 48, 30)
            .with_label("LB")
            .with_custom_background(white)
            .with_font("AtkinsonMockBold", pixels(17.f * scale))
            .with_custom_text_color(green_dark)
            .with_alignment(TextAlignment::Center));
    div(context, mk(root.ent(), 12),
        box(scale, 672, 11, 48, 30)
            .with_label("RB")
            .with_custom_background(white)
            .with_font("AtkinsonMockBold", pixels(17.f * scale))
            .with_custom_text_color(green_dark)
            .with_alignment(TextAlignment::Center));
    for (size_t i = 0; i < tabs.size(); ++i) {
      const bool active = i == active_tab;
      if (button(context, mk(root.ent(), 20 + static_cast<int>(i)),
                 box(scale, std::array<float, 4>{96, 252, 390, 524}[i], 0,
                     std::array<float, 4>{142, 122, 120, 136}[i], 50)
                     .with_label(tabs[i])
                     .with_custom_background(afterhours::Color{0, 0, 0, 0})
                     .with_font("AtkinsonMockBold", pixels(22.f * scale))
                     .with_custom_text_color(active ? green : muted)
                     .with_alignment(TextAlignment::Center)
                     .with_debug_name("sports_tab_" + std::to_string(i)))) {
        active_tab = i;
        selected_row = 0;
        status = tabs[i];
      }
      if (active) {
        div(context, mk(root.ent(), 30 + static_cast<int>(i)),
            box(scale, std::array<float, 4>{96, 252, 390, 524}[i], 46,
                std::array<float, 4>{142, 122, 120, 136}[i], 4)
                .with_custom_background(green)
                .with_on_draw_fg([scale](RectangleType r) {
                  for (float x = r.x; x + 4 * scale < r.x + r.width; x += 8 * scale)
                    afterhours::draw_line_ex({x, r.y + r.height}, {x + 4 * scale, r.y}, scale, {28, 106, 70, 255});
                }).with_ignore_pointer_events());
      }
    }

    div(context, mk(root.ent(), 100),
        box(scale, 34, 128, 572, 510).with_debug_name("sports_settings"));
    div(context, mk(root.ent(), 101),
        box(scale, 34, 115, 300, 30)
            .with_label(heading())
            .with_font("AtkinsonMockBold", pixels(22.f * scale))
            .with_custom_text_color(white)
            .with_debug_name("sports_heading"));

    for (size_t i = 0; i < current.size(); ++i) {
      auto &row = current[i];
      const bool active = i == selected_row;
      const float y = 157.f + static_cast<float>(i) * 40.f;
      if (button(context, mk(root.ent(), 200 + static_cast<int>(i)),
                 box(scale, 34, y, 559, 39)
                     .with_label("")
                     .with_custom_background(
                         active ? green : afterhours::Color{0, 0, 0, 0})
                     .with_debug_name("sports_row_" + std::to_string(i)))) {
        selected_row = i;
        status = row.label;
      }
      if (active)
        div(context, mk(root.ent(), 250 + static_cast<int>(i)), box(scale, 34, y, 244, 39)
            .with_custom_background(afterhours::Color{66, 197, 127, 255}).with_ignore_pointer_events());
      div(context, mk(root.ent(), 300 + static_cast<int>(i)),
          box(scale, 46, y + 7, 230, 27)
              .with_label(row.label)
              .with_font("AtkinsonMock", pixels(21.f * scale))
              .with_custom_text_color(active ? green_dark : white)
              .with_alignment(TextAlignment::Left)
              .with_ignore_pointer_events());
      if (row.is_slider) {
        div(context, mk(root.ent(), 400 + static_cast<int>(i)),
            box(scale, 282, y + 6, 96, 29)
                .with_label(value_for(row))
                .with_font("AtkinsonMock", pixels(21.f * scale))
                .with_custom_text_color(active ? green_dark : green)
                .with_alignment(TextAlignment::Left)
                .with_debug_name("sports_numeric_" + std::to_string(i)));
        float position = row.slider_value / 100.f;
        auto control =
            slider(context,
                   mk(root.ent(), 500 + static_cast<int>(active_tab * 20 + i)),
                   position,
                   box(scale, 386, y + 8, 188, 22)
                       .with_custom_background(afterhours::Color{0, 0, 0, 0})
                       .with_debug_name("sports_slider_" + std::to_string(i)),
                   SliderHandleValueLabelPosition::None);
        if (control) {
          row.slider_value = static_cast<int>(std::round(position * 100));
          selected_row = i;
        } else {
          for (auto id : control.ent().get<UIComponent>().children) {
            auto &child = UICollectionHolder::getEntityForIDEnforce(id);
            if (child.has<HasSliderState>())
              child.get<HasSliderState>().value = row.slider_value / 100.f;
          }
        }
        position = row.slider_value / 100.f;
        div(context, mk(root.ent(), 600 + static_cast<int>(i)),
            box(scale, 386, y + 8, 188, 22)
                .with_ignore_pointer_events()
                .with_on_draw_fg(
                    [position, active, scale, this](RectangleType r) {
                      const auto color = active ? green_dark : green;
                      const float width = std::max(0.f, r.width - 6.f);
                      const float x = r.x + position * width;
                      const float y = r.y + r.height * .5f;
                      afterhours::draw_line_ex({r.x, y}, {r.x + width, y}, 4 * scale,
                          active ? afterhours::Color{47, 162, 103, 255} : afterhours::Color{86, 107, 115, 255});
                      afterhours::draw_line_ex({r.x, y}, {x, y}, 4 * scale, color);
                      afterhours::draw_poly({x, y}, 4, 10 * scale, 15, white);
                      afterhours::draw_poly({x, y}, 4, 7 * scale, 15, color);
                    }));
      } else {
        const bool binary = row.options.size() == 2 && (row.options[0] == "Off" || row.options[0] == "Disabled");
        auto value = button(context, mk(root.ent(), 700 + static_cast<int>(i)),
            box(scale, 292, y + 5, 284, 29).with_label(row.options[row.option_idx])
                .with_font("AtkinsonMock", pixels(20 * scale)).with_custom_text_color(active ? green_dark : green)
                .with_alignment(TextAlignment::Left).with_text_inset(0, 0)
                .with_on_draw_fg([binary, enabled = row.option_idx == 1, active, this, scale](RectangleType r) {
                  const auto color = active ? green_dark : green;
                  if (binary) {
                    const RectangleType track{r.x + r.width - 52 * scale, r.y + 3 * scale, 48 * scale, 23 * scale};
                    afterhours::draw_rectangle_rounded(track, 1, 16, enabled ? color : muted, RoundedCorners().all_round());
                    afterhours::draw_circle(static_cast<int>(track.x + (enabled ? 36 : 12) * scale),
                        static_cast<int>(track.y + 11.5f * scale), 8 * scale, enabled ? white : green_dark);
                    return;
                  }
                  const float x = r.x + r.width - 15 * scale;
                  const float y = r.y + r.height / 2;
                  afterhours::draw_line_ex({x - 4 * scale, y - 5 * scale}, {x + 2 * scale, y}, 2 * scale, color);
                  afterhours::draw_line_ex({x + 2 * scale, y}, {x - 4 * scale, y + 5 * scale}, 2 * scale, color);
                }).with_debug_name("sports_value_" + std::to_string(i)));
        value.ent().get<HasLabel>().text_x_offset = 12 * scale;
        if (value) {
          selected_row = i;
          row.option_idx = (row.option_idx + 1) % row.options.size();
          status = "Applied locally";
        }
      }
    }
    div(context, mk(root.ent(), 950),
        box(scale, 620, 124, 2, 498)
            .with_custom_background(afterhours::Color{159, 174, 189, 110})
            .with_debug_name("sports_divider"));
    const auto &selected = current[selected_row];
    div(context, mk(root.ent(), 1000),
        box(scale, 680, 110, 530, 400).with_debug_name("sports_help"));
    div(context, mk(root.ent(), 1001),
        box(scale, 680, 110, 530, 44)
            .with_label(selected.label)
            .with_font("AtkinsonMockBold", pixels(32.f * scale))
            .with_custom_text_color(white)
            .with_alignment(TextAlignment::Left));
    div(context, mk(root.ent(), 1002),
        box(scale, 680, 168, 510, 106)
            .with_label(description_for(selected))
            .with_font("AtkinsonMock", pixels(22.f * scale))
            .with_custom_text_color(white)
            .with_text_overflow(TextOverflow::Wrap));
    div(context, mk(root.ent(), 1003),
        box(scale, 680, 288, 510, 34)
            .with_label("Default: " +
                        value_for(initial_settings[active_tab][selected_row]))
            .with_font("AtkinsonMock", pixels(22.f * scale))
            .with_custom_text_color(white));

    div(context, mk(root.ent(), 1004), box(scale, 680, 336, 510, 30)
        .with_label(range_for(selected)).with_font("AtkinsonMock", pixels(18 * scale))
        .with_custom_text_color(muted).with_debug_name("sports_range"));
    if (selected.is_slider) {
      div(context, mk(root.ent(), 1006), box(scale, 680, 386, 510, 22)
          .with_ignore_pointer_events().with_on_draw_fg([this, value = selected.slider_value, scale](RectangleType r) {
            const float x = r.x + r.width * static_cast<float>(value) / 100;
            const float y = r.y + r.height / 2;
            afterhours::draw_line_ex({r.x, y}, {r.x + r.width, y}, 4 * scale, muted);
            afterhours::draw_line_ex({r.x, y}, {x, y}, 4 * scale, green);
            afterhours::draw_circle(static_cast<int>(x), static_cast<int>(y), 7 * scale, white);
          }).with_debug_name("sports_range_track"));
      auto minimum = selected;
      auto maximum = selected;
      minimum.slider_value = 0;
      maximum.slider_value = 100;
      div(context, mk(root.ent(), 1007), box(scale, 680, 419, 220, 26)
          .with_label(value_for(minimum)).with_font("AtkinsonMock", pixels(18 * scale))
          .with_custom_text_color(white));
      div(context, mk(root.ent(), 1008), box(scale, 970, 419, 220, 26)
          .with_label(value_for(maximum)).with_font("AtkinsonMock", pixels(18 * scale))
          .with_alignment(TextAlignment::Right).with_custom_text_color(white));
    }
    div(context, mk(root.ent(), 1005), box(scale, 680, 467, 510, 32)
        .with_label("Settings preview / changes stay local")
        .with_font("AtkinsonMock", pixels(18 * scale)).with_custom_text_color(muted));

    div(context, mk(root.ent(), 1100),
        box(scale, 0, 673, 1280, 47)
            .with_custom_background(afterhours::Color{32, 45, 54, 255})
            .with_debug_name("sports_footer"));
    if (button(context, mk(root.ent(), 1101),
               box(scale, 880, 681, 270, 32)
                   .with_label("[Y] Reset " + tabs[active_tab])
                   .with_font("AtkinsonMockBold", pixels(23.f * scale))
                   .with_custom_text_color(white)
                   .with_alignment(TextAlignment::Left)
                   .with_debug_name("sports_reset"))) {
      reset_current();
    }
    if (button(context, mk(root.ent(), 1102),
               box(scale, 1154, 681, 108, 32)
                   .with_label("[B] Back")
                   .with_font("AtkinsonMockBold", pixels(23.f * scale))
                   .with_custom_text_color(white)
                   .with_alignment(TextAlignment::Left)
                   .with_debug_name("sports_back"))) {
      status = "Back";
      settings_open = false;
    }
    div(context, mk(root.ent(), 1103),
        box(scale, 34, 684, 800, 28)
            .with_label(status)
            .with_font("AtkinsonMockBold", pixels(21.f * scale))
            .with_custom_text_color(muted)
            .with_alignment(TextAlignment::Left)
            .with_debug_name("sports_status"));
  }
};

REGISTER_EXAMPLE_SCREEN(sports_settings, "Game Mockups",
                        "Sports game settings (Rematch style)",
                        SportsSettingsScreen)
