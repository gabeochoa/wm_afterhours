#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <string>
#include <vector>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct PowerWashSettingsScreen : ScreenSystem<UIContext<InputAction>> {
  size_t active_tab = 2;
  size_t selected_row = 4;
  bool settings_open = true;
  std::string status = "Select";

  struct SettingRow {
    std::string label;
    std::vector<std::string> options;
    size_t option_idx;
  };

  std::array<std::string, 5> tabs = {"GENERAL", "GAMEPLAY", "VIDEO", "AUDIO", "CONTROLS"};

  std::vector<SettingRow> video_settings = {
      {"Screen Resolution", {"1920 x 1080", "2560 x 1440", "3840 x 2160"}, 1},
      {"Window Mode", {"Fullscreen", "Borderless Windowed", "Windowed"}, 1},
      {"Target Framerate", {"30", "60", "120", "Unlimited"}, 3},
      {"Vsync", {"Off", "On"}, 1},
      {"Anti-Aliasing", {"Off", "2x", "4x", "8x"}, 1},
      {"SSAO", {"Off", "2x", "4x"}, 1},
      {"Render Scale", {"Off", "75%", "100%", "125%"}, 0},
      {"Model Quality", {"Low", "Medium", "High", "Ultra"}, 2},
      {"Texture Quality", {"Low", "Medium", "High", "Highest"}, 3},
  };
  std::vector<SettingRow> general_settings = {
      {"Language", {"English", "Spanish", "French", "German"}, 0},
      {"Subtitles", {"Off", "On"}, 1},
      {"Colorblind Mode", {"Off", "Deuteranopia", "Protanopia", "Tritanopia"}, 0},
  };
  std::vector<SettingRow> gameplay_settings = {
      {"Difficulty", {"Easy", "Normal", "Hard"}, 1},
      {"Camera Sensitivity", {"Low", "Medium", "High"}, 1},
      {"Invert Y Axis", {"Off", "On"}, 0},
  };
  std::vector<SettingRow> audio_settings = {
      {"Master Volume", {"0%", "25%", "50%", "75%", "100%"}, 4},
      {"Music Volume", {"0%", "25%", "50%", "75%", "100%"}, 3},
      {"SFX Volume", {"0%", "25%", "50%", "75%", "100%"}, 4},
      {"Voice Volume", {"0%", "25%", "50%", "75%", "100%"}, 4},
  };
  std::vector<SettingRow> controls_settings = {
      {"Controller Vibration", {"Off", "On"}, 1},
      {"Button Layout", {"Default", "Alternate A", "Alternate B"}, 0},
  };

  const std::vector<SettingRow> default_video_settings = video_settings;
  const std::vector<SettingRow> default_general_settings = general_settings;
  const std::vector<SettingRow> default_gameplay_settings = gameplay_settings;
  const std::vector<SettingRow> default_audio_settings = audio_settings;
  const std::vector<SettingRow> default_controls_settings = controls_settings;

  const afterhours::Color navy{14, 54, 113, 255};
  const afterhours::Color deep{7, 38, 84, 255};
  const afterhours::Color row_blue{51, 119, 211, 255};
  const afterhours::Color value_blue{24, 82, 150, 255};
  const afterhours::Color selected{226, 233, 241, 255};
  const afterhours::Color white{238, 248, 255, 255};
  const afterhours::Color muted{151, 181, 214, 255};
  const afterhours::Color text_dark{82, 113, 150, 255};

  ComponentConfig box(float scale, float x, float y, float w, float h) const {
    return ComponentConfig{}
        .with_size({pixels(w * scale), pixels(h * scale)})
        .with_absolute_position(x * scale, y * scale)
        .with_background(Theme::Usage::None)
        .with_corner_radius(0.f)
        .disable_rounded_corners();
  }

  std::vector<SettingRow> &get_current_settings() {
    switch (active_tab) {
    case 0:
      return general_settings;
    case 1:
      return gameplay_settings;
    case 2:
      return video_settings;
    case 3:
      return audio_settings;
    case 4:
      return controls_settings;
    default:
      return video_settings;
    }
  }

  static void draw_scene(RectangleType r) {
    afterhours::draw_rectangle(r, afterhours::Color{245, 194, 151, 255});
    afterhours::draw_triangle({r.x, r.y + 30.f}, {r.x + r.width, r.y + 36.f},
                              {r.x + r.width, r.y + 555.f},
                              afterhours::Color{181, 134, 110, 255});
    afterhours::draw_rectangle({r.x, r.y + 645.f, r.width, 75.f},
                               afterhours::Color{199, 151, 96, 255});
    for (int x = 0; x < 1280; x += 90) {
      const float sx = r.x + static_cast<float>(x) * r.width / 1280.f;
      afterhours::draw_line_ex({sx, r.y + 30.f}, {sx, r.y + 645.f}, 2.f,
                               afterhours::Color{216, 173, 139, 160});
    }
    for (int y = 75; y < 650; y += 38) {
      const float sy = r.y + static_cast<float>(y) * r.height / 720.f;
      afterhours::draw_line_ex({r.x, sy}, {r.x + r.width, sy - 18.f}, 2.f,
                               afterhours::Color{216, 173, 139, 160});
    }
    afterhours::draw_triangle({r.x + 1088.f, r.y + 594.f},
                              {r.x + 1280.f, r.y + 553.f},
                              {r.x + 1280.f, r.y + 720.f},
                              afterhours::Color{31, 152, 193, 255});
    afterhours::draw_line_ex({r.x + 1090.f, r.y + 594.f},
                             {r.x + 1157.f, r.y + 718.f}, 9.f,
                             afterhours::Color{22, 63, 86, 255});
  }

  static void draw_tablet(RectangleType r) {
    afterhours::draw_rectangle(r, afterhours::Color{16, 58, 118, 255});
    raylib::DrawRectangleLinesEx(r, 7.f, afterhours::Color{7, 44, 99, 255});
    afterhours::draw_rectangle({r.x, r.y + r.height - 11.f, r.width, 11.f},
                               afterhours::Color{8, 45, 99, 255});
  }


  static void draw_header_icon(RectangleType r, int index, bool active) {
    const afterhours::Color color = active ? afterhours::Color{238, 248, 255, 255}
                                        : afterhours::Color{151, 181, 214, 255};
    const float cx = r.x + r.width * .5f;
    const float cy = r.y + r.height * .5f;
    if (index == 0) {
      afterhours::draw_rectangle_outline({cx - 8.f, cy - 8.f, 16.f, 16.f}, color, 2.f);
    } else if (index == 1) {
      for (int i = 0; i < 5; ++i)
        afterhours::draw_line_ex({cx - 10.f, cy - 8.f + static_cast<float>(i) * 4.f},
                                 {cx + 10.f, cy - 8.f + static_cast<float>(i) * 4.f}, 1.5f, color);
    } else if (index == 2) {
      afterhours::draw_triangle({cx - 8.f, cy - 9.f}, {cx - 8.f, cy + 9.f},
                                {cx + 9.f, cy}, color);
    } else if (index == 3) {
      afterhours::draw_circle_lines(static_cast<int>(cx), static_cast<int>(cy), 11.f, color);
      afterhours::draw_circle_lines(static_cast<int>(cx), static_cast<int>(cy), 8.f, color);
    } else if (index == 4) {
      afterhours::draw_circle(static_cast<int>(cx), static_cast<int>(cy - 7.f), 5.f, color);
      afterhours::draw_circle(static_cast<int>(cx - 7.f), static_cast<int>(cy + 4.f), 5.f, color);
      afterhours::draw_circle(static_cast<int>(cx + 7.f), static_cast<int>(cy + 4.f), 5.f, color);
      afterhours::draw_line_ex({cx, cy - 1.f}, {cx, cy + 12.f}, 2.f, color);
    } else {
      afterhours::draw_circle_lines(static_cast<int>(cx), static_cast<int>(cy), 10.f, color);
      for (int i = 0; i < 8; ++i) {
        const float a = static_cast<float>(i) * 0.7853982f;
        afterhours::draw_line_ex({cx + std::cos(a) * 7.f, cy + std::sin(a) * 7.f},
                                 {cx + std::cos(a) * 14.f, cy + std::sin(a) * 14.f}, 2.f, color);
      }
    }
  }

  void reset_current_tab() {
    switch (active_tab) {
    case 0:
      general_settings = default_general_settings;
      break;
    case 1:
      gameplay_settings = default_gameplay_settings;
      break;
    case 2:
      video_settings = default_video_settings;
      break;
    case 3:
      audio_settings = default_audio_settings;
      break;
    case 4:
      controls_settings = default_controls_settings;
      break;
    default:
      video_settings = default_video_settings;
      active_tab = 2;
      break;
    }
    selected_row = 0;
    status = "Defaults restored";
  }

  void close_settings() {
    settings_open = false;
    status = "Settings closed";
  }

  void open_settings() {
    settings_open = true;
    status = "Settings open";
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    const float screen_w = context.screen_width > 0.f
                               ? context.screen_width
                               : static_cast<float>(Settings::get().get_screen_width());
    const float screen_h = context.screen_height > 0.f
                               ? context.screen_height
                               : static_cast<float>(Settings::get().get_screen_height());
    const float scale = std::min(screen_w / 1280.f, screen_h / 720.f);

    if (context.pressed(InputAction::MenuBack)) {
      if (settings_open)
        close_settings();
      else
        open_settings();
    }

    auto &settings = get_current_settings();
    if (selected_row >= settings.size())
      selected_row = 0;
    if (settings_open) {
      if (context.pressed(InputAction::WidgetDown))
        selected_row = (selected_row + 1) % settings.size();
      if (context.pressed(InputAction::WidgetUp))
        selected_row = (selected_row + settings.size() - 1) % settings.size();
      if (context.pressed(InputAction::WidgetRight)) {
        auto &row = settings[selected_row];
        row.option_idx = (row.option_idx + 1) % row.options.size();
      }
      if (context.pressed(InputAction::WidgetLeft)) {
        auto &row = settings[selected_row];
        row.option_idx = (row.option_idx + row.options.size() - 1) % row.options.size();
      }
    }

    Theme theme;
    theme.font = white;
    theme.darkfont = text_dark;
    theme.font_muted = muted;
    theme.background = navy;
    theme.surface = row_blue;
    theme.primary = value_blue;
    theme.secondary = muted;
    theme.accent = selected;
    theme.roundness = 0.f;
    theme.corner_radius = 0.f;
    theme.segments = 4;
    context.theme = theme;
    context.scaling_mode = ScalingMode::Proportional;
    UIStylingDefaults::get().set_default_font("ArchivoMock", h720(22.f));

    auto root = div(context, mk(entity, 0),
                    box(scale, 0, 0, 1280, 720)
                        .with_on_draw_bg([](RectangleType r) { draw_scene(r); })
                        .with_debug_name("pw_root"));

    div(context, mk(root.ent(), 10),
        box(scale, 56, 62, 1168, 576)
            .with_on_draw_bg([](RectangleType r) { draw_tablet(r); })
            .with_debug_name("pw_tablet"));
    div(context, mk(root.ent(), 11),
        box(scale, 63, 69, 1154, 43)
            .with_custom_background(deep)
            .with_border(afterhours::Color{39, 83, 135, 255}, 1.f)
            .with_debug_name("pw_header"));
    if (button(context, mk(root.ent(), 12),
               box(scale, 63, 70, 36, 38)
                   .with_label("X")
                   .with_custom_background(afterhours::Color{44, 97, 161, 255})
                   .with_font("ArchivoMockBold", h720(26.f))
                   .with_custom_text_color(white)
                   .with_alignment(TextAlignment::Center)
                   .with_debug_name("pw_close"))) {
      close_settings();
    }
    div(context, mk(root.ent(), 13),
        box(scale, 107, 80, 140, 31)
            .with_label("SETTINGS")
            .with_font("ArchivoMockBold", h720(23.f))
            .with_custom_text_color(white)
            .with_alignment(TextAlignment::Left));

    if (!settings_open) {
      div(context, mk(root.ent(), 700),
          box(scale, 444, 274, 392, 132)
              .with_custom_background(afterhours::Color{9, 47, 103, 245})
              .with_border(afterhours::Color{108, 155, 239, 255}, 2.f)
              .with_debug_name("pw_closed_panel"));
      div(context, mk(root.ent(), 701),
          box(scale, 486, 298, 308, 36)
              .with_label("Settings closed")
              .with_font("ArchivoMockBold", h720(28.f))
              .with_custom_text_color(white)
              .with_alignment(TextAlignment::Center));
      if (button(context, mk(root.ent(), 702),
                 box(scale, 535, 348, 210, 38)
                     .with_label("Reopen settings")
                     .with_custom_background(afterhours::Color{53, 107, 193, 255})
                     .with_border(afterhours::Color{151, 181, 214, 255}, 1.f)
                     .with_font("ArchivoMockBold", h720(22.f))
                     .with_custom_text_color(white)
                     .with_alignment(TextAlignment::Center)
                     .with_debug_name("pw_reopen"))) {
        open_settings();
      }
      return;
    }

    for (int i = 0; i < 6; ++i) {
      const bool active = i == 5;
      div(context, mk(root.ent(), 20 + i),
          box(scale, 500.f + static_cast<float>(i) * 49.f, 81, 38, 27)
              .with_label("")
              .with_custom_background(active ? afterhours::Color{71, 124, 210, 255}
                                             : afterhours::Color{0, 0, 0, 0})
              .with_on_draw_fg([i, active](RectangleType r) {
                draw_header_icon(r, i, active);
              })
              .with_ignore_pointer_events());
    }
    div(context, mk(root.ent(), 30),
        box(scale, 1084, 83, 112, 18)
            .with_label("O 10.00   O 200   [] 1")
            .with_font("ArchivoMock", h720(14.f))
            .with_custom_text_color(white)
            .with_alignment(TextAlignment::Right));

    for (size_t i = 0; i < settings.size(); ++i) {
      const bool active = i == selected_row;
      const float y = 131.f + static_cast<float>(i) * 46.f;
      if (button(context, mk(root.ent(), 100 + static_cast<int>(i)),
                 box(scale, 75, y, 610, 37)
                     .with_label("")
                     .with_custom_background(active ? selected : row_blue)
                     .with_border(active ? afterhours::Color{255, 255, 255, 160}
                                         : afterhours::Color{50, 109, 179, 255},
                                  active ? 1.5f : .5f)
                     .with_debug_name("pw_row_" + std::to_string(i)))) {
        selected_row = i;
      }
      div(context, mk(root.ent(), 200 + static_cast<int>(i)),
          box(scale, 87, y + 8, 285, 24)
              .with_label(settings[i].label)
              .with_font("ArchivoMock", h720(23.f))
              .with_custom_text_color(active ? text_dark : afterhours::Color{197, 216, 244, 255})
              .with_alignment(TextAlignment::Left)
              .with_ignore_pointer_events());
      if (button(context, mk(root.ent(), 300 + static_cast<int>(i)),
                 box(scale, 433, y, 252, 34)
                     .with_label(settings[i].options[settings[i].option_idx])
                     .with_custom_background(value_blue)
                     .with_border(afterhours::Color{98, 149, 212, 255}, 1.f)
                     .with_font("ArchivoMockBold", h720(22.f))
                     .with_custom_text_color(white)
                     .with_alignment(TextAlignment::Left)
                     .with_text_inset(17.f * scale, 6.f * scale)
                     .disable_rounded_corners()
                     .with_debug_name("pw_value_" + std::to_string(i)))) {
        selected_row = i;
        settings[i].option_idx = (settings[i].option_idx + 1) % settings[i].options.size();
      }
      div(context, mk(root.ent(), 360 + static_cast<int>(i)),
          box(scale, 658, y + 9, 16, 18)
              .with_label("v")
              .with_font("ArchivoMockBold", h720(15.f))
              .with_custom_text_color(white)
              .with_alignment(TextAlignment::Center)
              .with_ignore_pointer_events());
    }
    div(context, mk(root.ent(), 390),
        box(scale, 700, 117, 4, 449)
            .with_custom_background(afterhours::Color{94, 134, 184, 255})
            .with_debug_name("pw_scroll"));

    div(context, mk(root.ent(), 400),
        box(scale, 725, 132, 420, 26)
            .with_label(settings[selected_row].label)
            .with_font("ArchivoMockBold", h720(22.f))
            .with_custom_text_color(white)
            .with_debug_name("pw_help_title"));
    div(context, mk(root.ent(), 401),
        box(scale, 725, 154, 470, 48)
            .with_label(settings[selected_row].label == "Anti-Aliasing" ? "Turn MSAA on or off and choose from different anti-aliasing amounts." : "Choose the " + settings[selected_row].label + " setting for your job.")
            .with_font("ArchivoMock", h720(22.f))
            .with_custom_text_color(afterhours::Color{215, 231, 253, 255})
            .with_letter_spacing(0.f)
            .with_text_overflow(TextOverflow::Wrap));
    div(context, mk(root.ent(), 402),
        box(scale, 725, 200, 470, 88)
            .with_label(settings[selected_row].label == "Anti-Aliasing" ? "Multisample Anti-Aliasing smooths the image, reducing jagged lines and edges. Higher amounts use more samples for a better image. Turning this setting off will reduce FPS load." : "Changes are applied from the live settings panel. Higher visual settings can use more performance during large cleanup jobs.")
            .with_font("ArchivoMock", h720(22.f))
            .with_custom_text_color(afterhours::Color{215, 231, 253, 255})
            .with_letter_spacing(0.f)
            .with_text_overflow(TextOverflow::Wrap));
    div(context, mk(root.ent(), 403),
        box(scale, 1002, 559, 203, 24)
            .with_label("FUTURLAB / POWERWASH SIMULATOR")
            .with_font("ArchivoMock", h720(14.f))
            .with_custom_text_color(afterhours::Color{89, 129, 173, 150})
            .with_alignment(TextAlignment::Right));

    div(context, mk(root.ent(), 500),
        box(scale, 64, 579, 1152, 47)
            .with_custom_background(afterhours::Color{9, 47, 103, 255})
            .with_debug_name("pw_tabs"));
    div(context, mk(root.ent(), 501),
        box(scale, 74, 589, 17, 23)
            .with_label("A")
            .with_custom_background(white)
            .with_font("ArchivoMockBold", h720(22.f))
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center));
    div(context, mk(root.ent(), 502),
        box(scale, 1192, 589, 17, 23)
            .with_label("D")
            .with_custom_background(white)
            .with_font("ArchivoMockBold", h720(22.f))
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center));
    for (size_t i = 0; i < tabs.size(); ++i) {
      const bool active = i == active_tab;
      if (button(context, mk(root.ent(), 510 + static_cast<int>(i)),
                 box(scale, 109.f + static_cast<float>(i) * 211.f, 583, 198, 36)
                     .with_label(tabs[i])
                     .with_custom_background(active ? afterhours::Color{53, 107, 193, 255}
                                                    : afterhours::Color{0, 0, 0, 0})
                     .with_border(active ? afterhours::Color{108, 155, 239, 255}
                                         : afterhours::Color{0, 0, 0, 0},
                                  active ? 1.f : 0.f)
                     .with_font("ArchivoMockBold", h720(22.f))
                     .with_custom_text_color(active ? white : muted)
                     .with_alignment(TextAlignment::Center)
                     .with_debug_name("pw_tab_" + std::to_string(i)))) {
        active_tab = i;
        selected_row = 0;
        status = tabs[i];
      }
    }

    auto prompt = [&](int id, float x, const std::string &key, const std::string &label) {
      div(context, mk(root.ent(), id),
          box(scale, x, 675, 32, 21)
              .with_label(key)
              .with_custom_background(white)
              .with_font("ArchivoMockBold", h720(14.f))
              .with_custom_text_color(text_dark)
              .with_alignment(TextAlignment::Center));
      div(context, mk(root.ent(), id + 1),
          box(scale, x + 34, 672, 230, 28)
              .with_label(label)
              .with_font("ArchivoMockBold", h720(22.f))
              .with_custom_text_color(white)
              .with_alignment(TextAlignment::Left));
    };
    if (button(context, mk(root.ent(), 600), box(scale, 72, 675, 25, 21)
                                              .with_label("Esc")
                                              .with_custom_background(white)
                                              .with_font("ArchivoMockBold", h720(14.f))
                                              .with_custom_text_color(text_dark)
                                              .with_alignment(TextAlignment::Center)
                                              .with_debug_name("pw_footer_close"))) {
      close_settings();
    }
    div(context, mk(root.ent(), 601), box(scale, 106, 674, 52, 22)
                                    .with_label("Close")
                                    .with_font("ArchivoMockBold", h720(22.f))
                                    .with_custom_text_color(white));
    if (button(context, mk(root.ent(), 602), box(scale, 162, 675, 25, 21)
                                              .with_label("R")
                                              .with_custom_background(white)
                                              .with_font("ArchivoMockBold", h720(14.f))
                                              .with_custom_text_color(text_dark)
                                              .with_alignment(TextAlignment::Center)
                                              .with_debug_name("pw_footer_reset"))) {
      reset_current_tab();
    }
    div(context, mk(root.ent(), 603), box(scale, 196, 674, 52, 22)
                                    .with_label("Reset")
                                    .with_font("ArchivoMockBold", h720(22.f))
                                    .with_custom_text_color(white));
    prompt(604, 243, "Enter", status);
  }
};

REGISTER_EXAMPLE_SCREEN(powerwash_settings, "Game Mockups",
                        "Simulation game settings (PowerWash style)",
                        PowerWashSettingsScreen)
