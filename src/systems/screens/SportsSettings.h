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
  std::string status = "Ready";

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
       {"1,920x1,080", "2,560x1,440", "3,840x2,160"},
       1},
      {"Screen percentage", true, 100, {}, 0},
      {"V Sync", false, 0, {"Disabled", "Enabled"}, 1},
      {"Max FPS", true, 50, {}, 0},
      {"Gamma", true, 32, {}, 0},
      {"Anti-Aliasing method",
       false,
       0,
       {"Off", "FXAA", "TAA", "TSR", "DLSS"},
       3},
      {"Dynamic resolution", false, 0, {"Disabled", "Enabled"}, 1},
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
    return "GRAPHICS";
  }

  std::string value_for(const SettingRow &row) const {
    if (row.is_slider) {
      if (row.label == "Max FPS" || row.label == "Framerate target")
        return std::to_string(
            static_cast<int>(std::round(row.slider_value * 2.4f)));
      if (row.label == "Gamma")
        return fmt::format("{:.1f}", 1.0f + row.slider_value * 0.0375f);
      if (row.label == "Motion blur")
        return fmt::format("{:g}", row.slider_value / 50.f);
      return std::to_string(row.slider_value);
    }
    if (!row.options.empty())
      return row.options[row.option_idx];
    return "---";
  }

  std::string description_for(const SettingRow &row) const {
    if (row.label == "Max FPS")
      return "The maximum number of frames the game will try to display per "
             "second";
    if (row.label == "Anti-Aliasing method")
      return "TSR smooths jagged edges using information from previous frames.";
    if (row.label == "Resolution")
      return "Set the resolution of the rendered image.";
    return "Adjust " + row.label + " to your preference.";
  }

  void reset_current() {
    rows() = initial_settings[active_tab];
    selected_row = 0;
    status = "Defaults restored";
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

    if (settings_open && context.pressed(InputAction::WidgetDown))
      selected_row = (selected_row + 1) % current.size();
    if (settings_open && context.pressed(InputAction::WidgetUp))
      selected_row = (selected_row + current.size() - 1) % current.size();
    if (settings_open && context.pressed(InputAction::WidgetRight)) {
      auto &row = current[selected_row];
      if (row.is_slider)
        row.slider_value = std::min(100, row.slider_value + 5);
      else
        row.option_idx = (row.option_idx + 1) % row.options.size();
    }
    if (settings_open && context.pressed(InputAction::WidgetLeft)) {
      auto &row = current[selected_row];
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
    UIStylingDefaults::get().set_default_font("AtkinsonMock", h720(21.f));

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
                .with_on_draw_bg([this](RectangleType r) {
                  if (scene.id)
                    raylib::DrawTexturePro(
                        scene, {0, 0, (float)scene.width, (float)scene.height},
                        r, {0, 0}, 0, raylib::WHITE);
                  afterhours::draw_rectangle_gradient_h(
                      r, afterhours::Color{24, 44, 56, 220},
                      afterhours::Color{27, 53, 71, 125});
                })
                .with_debug_name("sports_root"));
    if (afterhours::input::is_key_pressed(raylib::KEY_ESCAPE))
      settings_open = !settings_open;
    if (!settings_open) {
      div(context, mk(root.ent(), 1200),
          box(scale, 390, 250, 500, 70)
              .with_label("Settings closed")
              .with_font("AtkinsonMock", h720(34.f))
              .with_custom_text_color(white)
              .with_alignment(TextAlignment::Center));
      if (button(context, mk(root.ent(), 1201),
                 box(scale, 490, 355, 300, 60)
                     .with_label("Return to settings")
                     .with_custom_background(green)
                     .with_custom_text_color(green_dark)
                     .with_font("AtkinsonMock", h720(25.f))
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
        box(scale, 69, 17, 25, 18)
            .with_label("LB")
            .with_custom_background(white)
            .with_font("ArchivoMockBold", h720(9.f))
            .with_custom_text_color(green_dark)
            .with_alignment(TextAlignment::Center));
    div(context, mk(root.ent(), 12),
        box(scale, 464, 17, 19, 16)
            .with_label("RB")
            .with_custom_background(white)
            .with_font("ArchivoMockBold", h720(9.f))
            .with_custom_text_color(green_dark)
            .with_alignment(TextAlignment::Center));
    for (size_t i = 0; i < tabs.size(); ++i) {
      const bool active = i == active_tab;
      if (button(context, mk(root.ent(), 20 + static_cast<int>(i)),
                 box(scale, std::array<float, 4>{104, 208, 280, 353}[i], 0,
                     std::array<float, 4>{92, 60, 62, 91}[i], 50)
                     .with_label(tabs[i])
                     .with_custom_background(afterhours::Color{0, 0, 0, 0})
                     .with_font("ArchivoMockBold", h720(19.f))
                     .with_custom_text_color(active ? green : muted)
                     .with_alignment(TextAlignment::Center)
                     .with_debug_name("sports_tab_" + std::to_string(i)))) {
        active_tab = i;
        selected_row = 0;
        status = tabs[i];
      }
      if (active) {
        div(context, mk(root.ent(), 30 + static_cast<int>(i)),
            box(scale, std::array<float, 4>{104, 208, 280, 353}[i], 46,
                std::array<float, 4>{92, 60, 62, 91}[i], 4)
                .with_custom_background(green)
                .with_ignore_pointer_events());
      }
    }

    div(context, mk(root.ent(), 100),
        box(scale, 34, 128, 572, 510).with_debug_name("sports_settings"));
    div(context, mk(root.ent(), 101),
        box(scale, 34, 128, 180, 22)
            .with_label(heading())
            .with_font("ArchivoMockBold", h720(19.f))
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
      div(context, mk(root.ent(), 300 + static_cast<int>(i)),
          box(scale, 46, y + 9, 240, 23)
              .with_label(row.label)
              .with_font("AtkinsonMock", h720(21.f))
              .with_custom_text_color(active ? green_dark : white)
              .with_alignment(TextAlignment::Left)
              .with_ignore_pointer_events());
      if (row.is_slider) {
        div(context, mk(root.ent(), 400 + static_cast<int>(i)),
            box(scale, 280, y + 7, 44, 29)
                .with_label(value_for(row))
                .with_font("AtkinsonMock", h720(21.f))
                .with_custom_text_color(active ? green_dark : green)
                .with_alignment(TextAlignment::Left)
                .with_debug_name("sports_numeric_" + std::to_string(i)));
        float position = row.slider_value / 100.f;
        auto control =
            slider(context,
                   mk(root.ent(), 500 + static_cast<int>(active_tab * 20 + i)),
                   position,
                   box(scale, 338, y + 8, 224, 22)
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
            box(scale, 338, y + 8, 224, 22)
                .with_ignore_pointer_events()
                .with_on_draw_fg(
                    [position, active, scale, this](RectangleType r) {
                      const auto color = active ? green_dark : green;
                      afterhours::draw_line_ex(
                          {r.x, r.y + r.height * .5f},
                          {r.x + r.width, r.y + r.height * .5f}, 3.f * scale,
                          color);
                      afterhours::draw_poly(
                          {r.x + position * r.width, r.y + r.height * .5f}, 4,
                          7.f * scale, 15.f, color);
                    }));
      } else {
        if (button(context, mk(root.ent(), 700 + static_cast<int>(i)),
                   box(scale, 315, y + 6, 170, 24)
                       .with_label(row.options[row.option_idx])
                       .with_font("AtkinsonMock", h720(21.f))
                       .with_custom_text_color(active ? green_dark : green)
                       .with_alignment(TextAlignment::Center)
                       .with_debug_name("sports_value_" + std::to_string(i)))) {
          selected_row = i;
          row.option_idx = (row.option_idx + 1) % row.options.size();
        }
        for (size_t j = 0; j < row.options.size(); ++j) {
          div(context, mk(root.ent(), 800 + static_cast<int>(i * 10 + j)),
              box(scale, 398.f + static_cast<float>(j) * 7.f, y + 31, 2, 2)
                  .with_custom_background(
                      j == row.option_idx
                          ? (active ? green_dark : green)
                          : afterhours::Color{189, 212, 234, 255})
                  .with_debug_name("sports_choice_" + std::to_string(i) + "_" +
                                   std::to_string(j)));
        }
      }
    }
    div(context, mk(root.ent(), 950),
        box(scale, 600, 124, 6, 498)
            .with_custom_background(afterhours::Color{159, 174, 189, 110})
            .with_debug_name("sports_scroll"));
    div(context, mk(root.ent(), 951),
        box(scale, 600, 124, 6, 265)
            .with_custom_background(green)
            .with_ignore_pointer_events());

    const auto &selected = current[selected_row];
    div(context, mk(root.ent(), 1000),
        box(scale, 684, 92, 530, 230).with_debug_name("sports_help"));
    div(context, mk(root.ent(), 1001),
        box(scale, 684, 92, 360, 26)
            .with_label(selected.label)
            .with_font("ArchivoMockBold", h720(23.f))
            .with_custom_text_color(white)
            .with_alignment(TextAlignment::Left));
    div(context, mk(root.ent(), 1002),
        box(scale, 684, 123, 540, 65)
            .with_label(description_for(selected))
            .with_font("AtkinsonMock", h720(24.f))
            .with_custom_text_color(white)
            .with_text_overflow(TextOverflow::Wrap));
    div(context, mk(root.ent(), 1003),
        box(scale, 684, 191, 430, 30)
            .with_label("Default value: " +
                        value_for(initial_settings[active_tab][selected_row]))
            .with_font("AtkinsonMock", h720(24.f))
            .with_custom_text_color(white));

    div(context, mk(root.ent(), 1100),
        box(scale, 0, 673, 1280, 47)
            .with_custom_background(afterhours::Color{32, 45, 54, 255})
            .with_debug_name("sports_footer"));
    if (button(context, mk(root.ent(), 1101),
               box(scale, 1004, 683, 158, 32)
                   .with_label("Y  Reset to default")
                   .with_font("ArchivoMockBold", h720(23.f))
                   .with_custom_text_color(white)
                   .with_alignment(TextAlignment::Left)
                   .with_debug_name("sports_reset"))) {
      reset_current();
    }
    if (button(context, mk(root.ent(), 1102),
               box(scale, 1166, 683, 95, 32)
                   .with_label("B  Back")
                   .with_font("ArchivoMockBold", h720(23.f))
                   .with_custom_text_color(white)
                   .with_alignment(TextAlignment::Left)
                   .with_debug_name("sports_back"))) {
      status = "Back";
      settings_open = false;
    }
    div(context, mk(root.ent(), 1103),
        box(scale, 850, 688, 140, 22)
            .with_label(status)
            .with_font("ArchivoMockBold", h720(21.f))
            .with_custom_text_color(muted)
            .with_alignment(TextAlignment::Right)
            .with_debug_name("sports_status"));
  }
};

REGISTER_EXAMPLE_SCREEN(sports_settings, "Game Mockups",
                        "Sports game settings (Rematch style)",
                        SportsSettingsScreen)
