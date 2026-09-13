#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <cctype>
#include <string>
#include <vector>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct PowerWashSettingsScreen : ScreenSystem<UIContext<InputAction>> {
  size_t active_tab = 2;
  size_t selected_row = 4;
  bool settings_open = true;
  std::string status = "Use Left / Right to change the selected value.";
  float design_left = 0;
  float design_top = 0;

  struct SettingRow {
    std::string label;
    std::vector<std::string> options;
    size_t option_idx;
  };

  std::array<std::string, 5> tabs = {"GENERAL", "GAMEPLAY", "VIDEO", "AUDIO", "CONTROLS"};

  std::vector<SettingRow> video_settings = {
      {"Screen Resolution", {"1920 × 1080", "2560 × 1440", "3840 × 2160"}, 1},
      {"Window Mode", {"Fullscreen", "Borderless Windowed", "Windowed"}, 1},
      {"Target Framerate", {"30", "60", "120", "Unlimited"}, 3},
      {"VSync", {"Off", "On"}, 1},
      {"Anti-Aliasing", {"Off", "2x", "4x", "8x"}, 1},
      {"Ambient occlusion", {"Off", "2x", "4x"}, 1},
      {"Render Scale", {"100% (native)", "75%", "125%"}, 0},
      {"Model Quality", {"Low", "Medium", "High", "Ultra"}, 2},
      {"Texture Quality", {"Low", "Medium", "High", "Ultra"}, 3},
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
  const afterhours::Color text_dark{14, 54, 113, 255};

  ComponentConfig box(float scale, float x, float y, float w, float h) const {
    return ComponentConfig{}
        .with_size({pixels(w * scale), pixels(h * scale)})
        .with_absolute_position((design_left + x) * scale, (design_top + y) * scale)
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
    const auto point = [r](float x, float y) { return raylib::Vector2{r.x + x * r.width / 1280, r.y + y * r.height / 720}; };
    const auto triangle = [&](float x1, float y1, float x2, float y2, float x3, float y3, afterhours::Color color) {
      afterhours::draw_triangle(point(x1, y1), point(x2, y2), point(x3, y3), color);
    };
    const float scale = std::min(r.width / 1280, r.height / 720);
    afterhours::draw_rectangle_gradient_v(r, {245, 194, 151, 255}, {248, 220, 149, 255});
    triangle(0, 15, 480, 111, 480, 620, {171, 123, 101, 255});
    triangle(0, 15, 480, 620, 0, 695, {171, 123, 101, 255});
    triangle(480, 111, 1280, 10, 1280, 553, {188, 145, 117, 255});
    triangle(480, 111, 1280, 553, 480, 620, {188, 145, 117, 255});
    for (int i = 0; i < 12; ++i) {
      const float t = static_cast<float>(i) / 11;
      afterhours::draw_line_ex(point(0, 15 + t * 680), point(480, 111 + t * 509), 2 * scale, {216, 173, 139, 190});
      afterhours::draw_line_ex(point(480, 111 + t * 509), point(1280, 10 + t * 543), 2 * scale, {216, 173, 139, 190});
    }
    for (int i = 1; i < 7; ++i) {
      const float t = static_cast<float>(i) / 7;
      afterhours::draw_line_ex(point(t * 480, 15 + t * 96), point(t * 480, 695 - t * 75), 2 * scale, {216, 173, 139, 190});
      afterhours::draw_line_ex(point(480 + t * 800, 111 - t * 101), point(480 + t * 800, 620 - t * 67), 2 * scale, {216, 173, 139, 190});
    }
    triangle(0, 648, 1280, 553, 1280, 720, {198, 149, 96, 255});
    triangle(0, 648, 1280, 720, 0, 720, {198, 149, 96, 255});
    triangle(1155, 720, 1040, 546, 1100, 489, {36, 153, 193, 255});
    triangle(1155, 720, 1100, 489, 1280, 643, {36, 153, 193, 255});
    afterhours::draw_line_ex(point(1155, 720), point(1040, 546), 10 * scale, {23, 63, 86, 255});
    afterhours::draw_line_ex(point(1040, 546), point(1100, 489), 10 * scale, {23, 63, 86, 255});
  }

  static void draw_tablet(RectangleType r) {
    afterhours::draw_rectangle(r, afterhours::Color{16, 58, 118, 255});
    raylib::DrawRectangleLinesEx(r, 7.f * r.width / 1168.f, afterhours::Color{7, 44, 99, 255});
    afterhours::draw_rectangle({r.x, r.y + r.height - 11.f * r.width / 1168.f, r.width, 11.f * r.width / 1168.f},
                               afterhours::Color{8, 45, 99, 255});
  }


  static void draw_header_icon(RectangleType r, int index, bool active) {
    const afterhours::Color color = active ? afterhours::Color{238, 248, 255, 255} : afterhours::Color{151, 181, 214, 255};
    const float scale = r.width / 38;
    const float cx = r.x + r.width / 2;
    const float cy = r.y + r.height / 2;
    const auto point = [cx, cy, scale](float x, float y) { return raylib::Vector2{cx + x * scale, cy + y * scale}; };
    if (index == 0) {
      afterhours::draw_rectangle_outline({cx - 8 * scale, cy - 8 * scale, 16 * scale, 16 * scale}, color, 2 * scale);
      return;
    }
    if (index == 1) {
      for (int i = 0; i < 5; ++i) {
        const float y = -8 + static_cast<float>(i) * 4;
        afterhours::draw_line_ex(point(-10, y), point(10, y), 1.5f * scale, color);
      }
      return;
    }
    if (index == 2) {
      afterhours::draw_triangle(point(-8, -9), point(-8, 9), point(9, 0), color);
      return;
    }
    if (index == 3) {
      raylib::DrawCircleLinesV(point(0, 0), 11 * scale, color);
      raylib::DrawCircleLinesV(point(0, 0), 8 * scale, color);
      return;
    }
    if (index == 4) {
      raylib::DrawCircleV(point(0, -7), 5 * scale, color);
      raylib::DrawCircleV(point(-7, 4), 5 * scale, color);
      raylib::DrawCircleV(point(7, 4), 5 * scale, color);
      afterhours::draw_line_ex(point(0, -1), point(0, 12), 2 * scale, color);
      return;
    }
    raylib::DrawCircleLinesV(point(0, 0), 10 * scale, color);
    for (int i = 0; i < 8; ++i) {
      const float angle = static_cast<float>(i) * .7853982f;
      afterhours::draw_line_ex(point(std::cos(angle) * 7, std::sin(angle) * 7),
          point(std::cos(angle) * 14, std::sin(angle) * 14), 2 * scale, color);
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
    design_left = (screen_w / scale - 1280) / 2;
    design_top = (screen_h / scale - 720) / 2;
    if (settings_open && afterhours::input::is_key_pressed(raylib::KEY_A)) {
      active_tab = (active_tab + tabs.size() - 1) % tabs.size();
      selected_row = 0;
      status = tabs[active_tab];
    }
    if (settings_open && afterhours::input::is_key_pressed(raylib::KEY_D)) {
      active_tab = (active_tab + 1) % tabs.size();
      selected_row = 0;
      status = tabs[active_tab];
    }
    if (settings_open && afterhours::input::is_key_pressed(raylib::KEY_R)) reset_current_tab();

    if (context.pressed(InputAction::MenuBack)) {
      if (settings_open)
        close_settings();
      else
        open_settings();
    }

    auto &settings = get_current_settings();
    if (selected_row >= settings.size())
      selected_row = 0;
    bool row_navigation = false;
    if (settings_open) {
      if (context.pressed(InputAction::WidgetDown)) {
        selected_row = (selected_row + 1) % settings.size();
        row_navigation = true;
      }
      if (context.pressed(InputAction::WidgetUp)) {
        selected_row = (selected_row + settings.size() - 1) % settings.size();
        row_navigation = true;
      }
      if (context.pressed(InputAction::WidgetRight)) {
        auto &row = settings[selected_row];
        row.option_idx = (row.option_idx + 1) % row.options.size();
        status = row.label + ": " + row.options[row.option_idx];
      }
      if (context.pressed(InputAction::WidgetLeft)) {
        auto &row = settings[selected_row];
        row.option_idx = (row.option_idx + row.options.size() - 1) % row.options.size();
        status = row.label + ": " + row.options[row.option_idx];
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

    auto root = div(context, mk(entity, 0),
                    ComponentConfig{}.with_size({pixels(screen_w), pixels(screen_h)}).with_corner_radius(0)
                        .with_on_draw_bg([](RectangleType r) { draw_scene(r); })
                        .with_debug_name("pw_root"));

    div(context, mk(root.ent(), 9), box(scale, 44, 56, 1192, 604).with_ignore_pointer_events()
        .with_on_draw_bg([](RectangleType r) {
          for (int i = 0; i < 7; ++i) {
            const float inset = static_cast<float>(i) * r.width / 596;
            afterhours::draw_rectangle({r.x + inset, r.y + inset, r.width - 2 * inset, r.height - 2 * inset}, {10, 30, 48, 12});
          }
        }));
    div(context, mk(root.ent(), 10),
        box(scale, 56, 62, 1168, 576)
            .with_on_draw_bg([](RectangleType r) { draw_tablet(r); })
            .with_debug_name("pw_tablet"));
    div(context, mk(root.ent(), 11),
        box(scale, 63, 69, 1154, 54)
            .with_custom_background(deep)
            .with_border(afterhours::Color{39, 83, 135, 255}, 1.f)
            .with_debug_name("pw_header"));
    if (button(context, mk(root.ent(), 12),
               box(scale, 63, 70, 36, 38)
                   .with_on_draw_fg([this, scale](RectangleType r) {
                     const float x = r.x + r.width / 2;
                     const float y = r.y + r.height / 2;
                     afterhours::draw_line_ex({x - 7 * scale, y - 7 * scale}, {x + 7 * scale, y + 7 * scale}, 2.5f * scale, white);
                     afterhours::draw_line_ex({x + 7 * scale, y - 7 * scale}, {x - 7 * scale, y + 7 * scale}, 2.5f * scale, white);
                   })
                   .with_custom_background(afterhours::Color{44, 97, 161, 255})
                   .with_font("ArchivoMockBold", pixels(26.f * scale))
                   .with_custom_text_color(white)
                   .with_alignment(TextAlignment::Center)
                   .with_debug_name("pw_close"))) {
      close_settings();
    }
    div(context, mk(root.ent(), 13),
        box(scale, 107, 73, 160, 31)
            .with_label("SETTINGS")
            .with_font("ArchivoMockBold", pixels(23.f * scale))
            .with_custom_text_color(white)
            .with_alignment(TextAlignment::Left));

    const std::array<std::string, 5> category_names{"General", "Gameplay", "Video", "Audio", "Controls"};
    const auto label = [&](int id, float x, float y, float w, float h, const std::string &value,
                           float size, afterhours::Color color, bool bold = false, const std::string &name = "") {
      return div(context, mk(root.ent(), id), box(scale, x, y, w, h).with_label(value)
          .with_font(bold ? "ArchivoMockBold" : "ArchivoMock", pixels(size * scale))
          .with_custom_text_color(color).with_alignment(TextAlignment::Left)
          .with_text_overflow(TextOverflow::Wrap).with_ignore_pointer_events().with_debug_name(name));
    };
    label(14, 107, 102, 270, 20, category_names[active_tab] + " settings", 17, muted);

    if (!settings_open) {
      div(context, mk(root.ent(), 700),
          box(scale, 444, 274, 392, 132)
              .with_custom_background(afterhours::Color{9, 47, 103, 245})
              .with_border(afterhours::Color{108, 155, 239, 255}, 2.f)
              .with_debug_name("pw_closed_panel"));
      div(context, mk(root.ent(), 701),
          box(scale, 486, 298, 308, 36)
              .with_label("Settings closed")
              .with_font("ArchivoMockBold", pixels(28.f * scale))
              .with_custom_text_color(white)
              .with_alignment(TextAlignment::Center));
      if (button(context, mk(root.ent(), 702),
                 box(scale, 535, 348, 210, 38)
                     .with_label("Reopen settings")
                     .with_custom_background(afterhours::Color{53, 107, 193, 255})
                     .with_border(afterhours::Color{151, 181, 214, 255}, 1.f)
                     .with_font("ArchivoMockBold", pixels(22.f * scale))
                     .with_custom_text_color(white)
                     .with_alignment(TextAlignment::Center)
                     .with_debug_name("pw_reopen"))) {
        open_settings();
      }
      return;
    }

    for (int i = 0; i < 6; ++i) {
      const bool active = i == 5;
      div(context, mk(root.ent(), 20 + i), box(scale, 500.f + static_cast<float>(i) * 49.f, 75, 38, 26)
          .with_custom_background(active ? afterhours::Color{71, 124, 210, 255} : afterhours::Color{0, 0, 0, 0})
          .with_on_draw_fg([i, active](RectangleType r) { draw_header_icon(r, i, active); })
          .with_ignore_pointer_events());
    }
    label(26, 500, 102, 240, 20, "Tablet navigation", 16, muted);
    label(27, 746, 102, 77, 20, "Settings", 16, white, true);
    const std::array<std::string, 3> counters{"10.00", "200", "1"};
    for (int i = 0; i < 3; ++i) {
      const float x = 912 + static_cast<float>(i) * 100;
      div(context, mk(root.ent(), 30 + i), box(scale, x, 85, 20, 20).with_ignore_pointer_events()
          .with_on_draw_fg([i, this, scale](RectangleType r) {
            const raylib::Vector2 c{r.x + r.width / 2, r.y + r.height / 2};
            if (i == 2) {
              afterhours::draw_rectangle_outline(r, white, 2 * scale);
              afterhours::draw_line_ex({c.x, r.y}, {c.x, r.y + r.height}, scale, white);
              return;
            }
            raylib::DrawCircleLinesV(c, r.width * .45f, white);
            if (i == 0) { raylib::DrawCircleLinesV(c, r.width * .28f, white); return; }
            afterhours::draw_line_ex(c, {c.x, c.y - 6 * scale}, 2 * scale, white);
            afterhours::draw_line_ex(c, {c.x + 5 * scale, c.y}, 2 * scale, white);
          }));
      label(33 + i, x + 27, 81, 66, 29, counters[i], 20, white, true);
    }

    for (size_t i = 0; i < settings.size(); ++i) {
      const bool active = i == selected_row;
      const float y = 141.f + static_cast<float>(i) * 46.f;
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
          box(scale, 87, y + 5, 327, 29)
              .with_label(settings[i].label)
              .with_font("ArchivoMock", pixels(23.f * scale))
              .with_custom_text_color(active ? navy : afterhours::Color{220, 234, 251, 255})
              .with_alignment(TextAlignment::Left)
              .with_ignore_pointer_events());
      auto value = button(context, mk(root.ent(), 300 + static_cast<int>(i)),
                 box(scale, 433, y + 1, 251, 35)
                     .with_label(settings[i].options[settings[i].option_idx])
                     .with_custom_background(value_blue)
                     .with_border(afterhours::Color{98, 149, 212, 255}, 1.f)
                     .with_font("ArchivoMockBold", pixels(22.f * scale))
                     .with_custom_text_color(white)
                     .with_alignment(TextAlignment::Left)
                     .with_text_inset(0)
                     .disable_rounded_corners()
                     .with_debug_name("pw_value_" + std::to_string(i)));
      value.ent().get<HasLabel>().text_x_offset = 10 * scale;
      if (row_navigation && selected_row == i) context.set_focus(value.ent().id);
      if (value) {
        selected_row = i;
        settings[i].option_idx = (settings[i].option_idx + 1) % settings[i].options.size();
        status = settings[i].label + ": " + settings[i].options[settings[i].option_idx];
      }
      div(context, mk(root.ent(), 360 + static_cast<int>(i)), box(scale, 658, y + 12, 12, 12)
          .with_on_draw_fg([this, scale](RectangleType r) {
            afterhours::draw_line_ex({r.x + 3 * scale, r.y}, {r.x + 9 * scale, r.y + 6 * scale}, 2 * scale, white);
            afterhours::draw_line_ex({r.x + 9 * scale, r.y + 6 * scale}, {r.x + 3 * scale, r.y + 12 * scale}, 2 * scale, white);
          }).with_ignore_pointer_events());
      if (active) div(context, mk(root.ent(), 380 + static_cast<int>(i)), box(scale, 75, y, 610, 37)
          .with_ignore_pointer_events().with_on_draw_fg([this, scale](RectangleType r) {
            afterhours::draw_rectangle_outline(r, white, 2 * scale);
          }));
    }
    div(context, mk(root.ent(), 390),
        box(scale, 700, 132, 2, 431)
            .with_custom_background(afterhours::Color{94, 134, 184, 255})
            .with_debug_name("pw_scroll"));

    const auto &selected_setting = settings[selected_row];
    std::string help = "Change " + selected_setting.label + " for this settings preview.";
    std::string detail = "Click the value to cycle forward. Left and Right move through its options. This mock keeps changes while you switch categories.";
    if (selected_setting.label == "Anti-Aliasing") {
      help = "Multisample anti-aliasing smooths jagged lines and edges.";
      detail = "Higher sample counts can improve edge quality. Turning MSAA off can reduce graphics processing load.";
    }
    if (selected_setting.label == "VSync") {
      help = "Vertical synchronization matches frame presentation to the display refresh cycle.";
      detail = "It can reduce screen tearing. The best choice depends on your display and rendering setup.";
    }
    if (selected_setting.label == "Ambient occlusion") {
      help = "SSAO means screen-space ambient occlusion. It adds contact shading where surfaces meet.";
      detail = "Higher settings increase the occlusion sampling level in the mock's options.";
    }
    if (selected_setting.label == "Render Scale") {
      help = "100% uses the selected resolution without scaling. 75% reduces it; 125% increases it.";
      detail = "These choices describe render scale. They do not resize this application's window.";
    }
    label(400, 725, 143, 460, 39, selected_setting.label, 29, white, true, "pw_help_title");
    label(401, 725, 192, 446, 97, help, 25, white);
    label(402, 725, 300, 446, 150, detail, 24, afterhours::Color{215, 231, 253, 255});
    label(404, 725, 456, 461, 50, "Changes apply immediately in this preview.", 21, white);
    label(405, 725, 510, 461, 45, status, 21, muted, false, "pw_status");
    label(403, 861, 555, 337, 21, "FUTURLAB / POWERWASH SIMULATOR", 18, afterhours::Color{143, 172, 205, 255});

    div(context, mk(root.ent(), 500),
        box(scale, 64, 579, 1152, 47)
            .with_custom_background(afterhours::Color{9, 47, 103, 255})
            .with_debug_name("pw_tabs"));
    div(context, mk(root.ent(), 501),
        box(scale, 74, 589, 17, 23)
            .with_label("A")
            .with_custom_background(white)
            .with_font("ArchivoMockBold", pixels(22.f * scale))
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center));
    div(context, mk(root.ent(), 502),
        box(scale, 1192, 589, 17, 23)
            .with_label("D")
            .with_custom_background(white)
            .with_font("ArchivoMockBold", pixels(22.f * scale))
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center));
    for (size_t i = 0; i < tabs.size(); ++i) {
      const bool active = i == active_tab;
      if (button(context, mk(root.ent(), 510 + static_cast<int>(i)),
                 box(scale, 109.f + static_cast<float>(i) * 211.f, 583, 198, 36)
                     .with_label(tabs[i])
                     .with_custom_background(active ? afterhours::Color{53, 107, 193, 255}
                                                    : afterhours::Color{0, 0, 0, 0})
                     .with_on_draw_fg([active, scale](RectangleType r) {
                       if (!active) return;
                       afterhours::draw_rectangle({r.x, r.y + r.height - 3 * scale, r.width, 3 * scale}, {145, 186, 255, 255});
                     })
                     .with_font("ArchivoMockBold", pixels(22.f * scale))
                     .with_custom_text_color(active ? white : muted)
                     .with_alignment(TextAlignment::Center)
                     .with_debug_name("pw_tab_" + std::to_string(i)))) {
        active_tab = i;
        selected_row = 0;
        status = tabs[i];
      }
    }

    div(context, mk(root.ent(), 599), box(scale, 56, 655, 1168, 57).with_custom_background(deep));
    const auto action = [&](int id, float x, float w, const std::string &key, const std::string &caption, const std::string &name) {
      const float key_width = key == "Enter" ? 61.f : key == "Esc" ? 44.f : 30.f;
      auto result = button(context, mk(root.ent(), id), box(scale, x, 662, w, 43).with_debug_name(name));
      div(context, mk(root.ent(), id + 1), box(scale, x, 671, key_width, 25)
          .with_custom_background(white).with_label(key).with_custom_text_color(navy)
          .with_font("ArchivoMockBold", pixels(18 * scale)).with_alignment(TextAlignment::Center).with_ignore_pointer_events());
      label(id + 2, x + key_width + 8, 666, w - key_width - 8, 33, caption, 22, white);
      return static_cast<bool>(result);
    };
    if (action(600, 72, 113, "Esc", "Close", "pw_footer_close")) close_settings();
    std::string lower_category = category_names[active_tab];
    lower_category[0] = static_cast<char>(std::tolower(static_cast<unsigned char>(lower_category[0])));
    if (action(610, 210, 276, "R", "Reset " + lower_category + " settings", "pw_footer_reset")) reset_current_tab();
    if (action(620, 511, 186, "Enter", "Change value", "pw_footer_select")) {
      auto &row = get_current_settings()[selected_row];
      row.option_idx = (row.option_idx + 1) % row.options.size();
      status = row.label + ": " + row.options[row.option_idx];
    }
    label(630, 725, 666, 469, 33, "A / D  Change category     Up / Down  Select row", 22, muted);

  }
};

REGISTER_EXAMPLE_SCREEN(powerwash_settings, "Game Mockups",
                        "Simulation game settings (PowerWash style)",
                        PowerWashSettingsScreen)
