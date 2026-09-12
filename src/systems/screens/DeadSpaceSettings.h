#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <algorithm>
#include <array>
#include <string>
#include <vector>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct DeadSpaceSettingsScreen : ScreenSystem<UIContext<InputAction>> {
  size_t selected_initial = 7;
  size_t active_tab = 1;
  bool detail_open = false;
  std::array<std::array<size_t, 4>, 8> selected_values{};

  const afterhours::Color black{0, 0, 0, 255};
  const afterhours::Color panel{22, 38, 45, 224};
  const afterhours::Color panel_dark{13, 28, 34, 236};
  const afterhours::Color panel_line{105, 163, 177, 155};
  const afterhours::Color glow{110, 225, 235, 165};
  const afterhours::Color text{217, 245, 246, 255};
  const afterhours::Color muted{135, 183, 188, 255};
  const afterhours::Color active_bg{92, 143, 151, 245};

  std::array<std::string, 8> initial_settings = {"Continue",
                                                 "Menu Narration",
                                                 "Voice Language",
                                                 "Subtitles",
                                                 "Select Difficulty",
                                                 "Inverted Camera (Y-Axis)",
                                                 "Show Content Warning",
                                                 "More Settings"};

  std::array<std::string, 8> categories = {"Controls",
                                           "Gameplay",
                                           "Display and Graphics",
                                           "Audio",
                                           "Language and Subtitles",
                                           "Accessibility",
                                           "Policies and Licenses",
                                           "Credits"};

  struct DetailRow {
    const char *label;
    std::vector<const char *> values;
  };

  std::vector<std::vector<DetailRow>> detail_rows = {
      {{"Aim sensitivity", {"4", "5", "6", "7"}},
       {"Aim assist", {"On", "Off"}},
       {"Vibration", {"On", "Off"}},
       {"Hold to sprint", {"Off", "On"}}},
      {{"Difficulty", {"Hard", "Medium", "Easy", "Impossible"}},
       {"Auto save", {"Every 5 min", "Off", "Manual"}},
       {"Objective marker", {"On", "Off"}},
       {"Hints", {"Contextual", "Off"}}},
      {{"Brightness", {"62", "70", "80"}},
       {"Field of view", {"85", "90", "100"}},
       {"Motion blur", {"Off", "On"}},
       {"Film grain", {"Low", "Off", "High"}}},
      {{"Master volume", {"80", "90", "100"}},
       {"Effects", {"75", "85", "100"}},
       {"Music", {"40", "50", "60"}},
       {"Dynamic range", {"Night", "Full"}}},
      {{"Voice language", {"English", "French", "German"}},
       {"Text language", {"English", "Spanish", "French"}},
       {"Subtitles", {"On", "Off"}},
       {"Subtitle size", {"Large", "Medium", "Small"}}},
      {{"High contrast", {"Off", "On"}},
       {"Colourblind mode", {"Off", "Deuteranopia", "Protanopia"}},
       {"Screen shake", {"Reduced", "Full", "Off"}},
       {"Text to speech", {"Off", "On"}}},
      {{"Privacy policy", {"View"}},
       {"Terms of service", {"View"}},
       {"Open source notices", {"View"}},
       {"Data collection", {"Minimal", "Off"}}},
      {{"Studio", {"Harbour"}},
       {"Engine", {"afterhours"}},
       {"Build", {"2.4.1"}},
       {"Special thanks", {"View"}}},
  };

  ComponentConfig box(float scale, float x, float y, float w, float h) const {
    return ComponentConfig{}
        .with_size({pixels(w * scale), pixels(h * scale)})
        .with_absolute_position(x * scale, y * scale)
        .with_background(Theme::Usage::None)
        .with_corner_radius(0);
  }

  static void draw_holo_plate(RectangleType r, afterhours::Color fill,
                              afterhours::Color line, bool bright) {
    const float scale = r.width / 455.f;
    const float cut = std::min(10.f * scale, r.height * .22f);
    const std::array<Vector2Type, 6> vertices{{{r.x + cut, r.y},
                                               {r.x + r.width - cut, r.y},
                                               {r.x + r.width, r.y + cut},
                                               {r.x + r.width, r.y + r.height},
                                               {r.x, r.y + r.height},
                                               {r.x, r.y + cut}}};
    for (size_t i = 1; i + 1 < vertices.size(); ++i)
      afterhours::draw_triangle(vertices[0], vertices[i + 1], vertices[i],
                                fill);
    for (size_t i = 0; i < vertices.size(); ++i)
      afterhours::draw_line_ex(vertices[i], vertices[(i + 1) % vertices.size()],
                               (bright ? 2.f : 1.f) * scale, line);
    for (float y = 3.f * scale; y < r.height; y += 4.f * scale) {
      afterhours::draw_line_ex(
          {r.x + cut, r.y + y}, {r.x + r.width - cut, r.y + y}, scale,
          afterhours::Color{155, 230, 238,
                            static_cast<unsigned char>(bright ? 38 : 22)});
    }
  }

  static void draw_signal_field(RectangleType r) {
    afterhours::draw_rectangle(r, afterhours::Color{0, 0, 0, 255});
    for (int x = 270; x < 990; x += 13) {
      const float xf = r.x + static_cast<float>(x) * r.width / 1280.f;
      afterhours::draw_line_ex({xf, r.y + 151.f * r.height / 720.f},
                               {xf, r.y + 591.f * r.height / 720.f}, 1.f,
                               afterhours::Color{91, 149, 162, 7});
    }
    for (int y = 120; y < 640; y += 9) {
      const float yf = r.y + static_cast<float>(y) * r.height / 720.f;
      afterhours::draw_line_ex({r.x + 210.f * r.width / 1280.f, yf},
                               {r.x + 1020.f * r.width / 1280.f, yf}, 1.f,
                               afterhours::Color{80, 180, 190, 4});
    }
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

    if (context.pressed(InputAction::WidgetDown))
      active_tab = (active_tab + 1) % categories.size();
    if (context.pressed(InputAction::WidgetUp))
      active_tab = (active_tab + categories.size() - 1) % categories.size();
    if (context.pressed(InputAction::WidgetPress))
      detail_open = true;
    if (context.pressed(InputAction::MenuBack))
      detail_open = false;

    Theme theme;
    theme.font = text;
    theme.darkfont = black;
    theme.font_muted = muted;
    theme.background = black;
    theme.surface = panel_dark;
    theme.primary = active_bg;
    theme.secondary = panel_line;
    theme.accent = glow;
    theme.roundness = 0.f;
    theme.segments = 4;
    context.set_theme(theme);
    context.scaling_mode = ScalingMode::Proportional;
    UIStylingDefaults::get().set_default_font("Archivo", h720(22.f));

    auto root =
        div(context, mk(entity, 0),
            box(scale, 0, 0, 1280, 720)
                .with_on_draw_bg([](RectangleType r) { draw_signal_field(r); })
                .with_debug_name("ds_root"));

    div(context, mk(root.ent(), 10),
        box(scale, 24, 237, 395, 390)
            .with_on_draw_bg([fill = afterhours::Color{11, 25, 31, 112},
                              line = panel_line](RectangleType r) {
              draw_holo_plate(r, fill, line, false);
            })
            .with_debug_name("ds_back_layer"));

    div(context, mk(root.ent(), 11),
        box(scale, 38, 244, 330, 28)
            .with_label("INITIAL SETTINGS")
            .with_font("Archivo@bold", h720(23.f))
            .with_custom_text_color(muted)
            .with_text_shadow(glow, 0.f, 0.f)
            .with_debug_name("ds_initial_title"));

    for (size_t i = 0; i < initial_settings.size(); ++i) {
      const bool active = i == selected_initial;
      const float y = i == 0 ? 274.f : 315.f + static_cast<float>(i - 1) * 40.f;
      if (button(context, mk(root.ent(), 20 + static_cast<int>(i)),
                 box(scale, 23, y, 395, 35)
                     .with_label("")
                     .with_custom_text_color(
                         active ? text : afterhours::Color{124, 151, 156, 175})
                     .with_alignment(TextAlignment::Left)
                     .with_on_draw_bg([active](RectangleType r) {
                       draw_holo_plate(
                           r,
                           active ? afterhours::Color{73, 120, 131, 130}
                                  : afterhours::Color{8, 18, 23, 100},
                           afterhours::Color{
                               94, 145, 157,
                               static_cast<unsigned char>(active ? 150 : 70)},
                           active);
                     })
                     .with_debug_name("ds_initial_" + std::to_string(i)))) {
        selected_initial = i;
        if (i == 7)
          detail_open = false;
      }
      div(context, mk(root.ent(), 60 + static_cast<int>(i)),
          box(scale, 38, y + 3.f, 335, 29)
              .with_label(initial_settings[i])
              .with_font("Archivo", h720(23.f))
              .with_custom_text_color(
                  active ? text : afterhours::Color{124, 151, 156, 175})
              .with_alignment(TextAlignment::Left)
              .with_ignore_pointer_events());
    }

    div(context, mk(root.ent(), 40),
        box(scale, 294, 91, 455, 34)
            .with_label("MORE SETTINGS")
            .with_font("Archivo@bold", h720(30.f))
            .with_custom_text_color(afterhours::Color{158, 211, 220, 255})
            .with_alignment(TextAlignment::Center)
            .with_text_shadow(glow, 0.f, 0.f)
            .with_debug_name("ds_more_settings"));

    div(context, mk(root.ent(), 41),
        box(scale, 286, 136, 455, 44)
            .with_label("")
            .with_on_draw_bg([fill = afterhours::Color{69, 103, 117, 140},
                              line = panel_line](RectangleType r) {
              draw_holo_plate(r, fill, line, true);
            })
            .with_debug_name("ds_front_header"));
    div(context, mk(root.ent(), 43),
        box(scale, 307, 140, 405, 36)
            .with_label("// SETTINGS")
            .with_font("Archivo", h720(32.f))
            .with_custom_text_color(text)
            .with_alignment(TextAlignment::Left)
            .with_ignore_pointer_events());

    div(context, mk(root.ent(), 42),
        box(scale, 286, 135, 455, 496)
            .with_ignore_pointer_events()
            .with_debug_name("ds_front_layer"));

    for (size_t i = 0; i < categories.size(); ++i) {
      const bool active = i == active_tab;
      const float y = 188.f + static_cast<float>(i) * 47.f;
      if (button(context, mk(root.ent(), 100 + static_cast<int>(i)),
                 box(scale, 286, y, 455, 44)
                     .with_label("")
                     .with_custom_text_color(
                         active ? text : afterhours::Color{183, 223, 225, 230})
                     .with_alignment(TextAlignment::Left)
                     .with_on_draw_bg([active](RectangleType r) {
                       draw_holo_plate(
                           r,
                           active ? afterhours::Color{100, 150, 159, 238}
                                  : afterhours::Color{31, 50, 59, 224},
                           active ? afterhours::Color{237, 255, 254, 230}
                                  : afterhours::Color{130, 180, 190, 140},
                           active);
                     })
                     .with_debug_name("ds_category_" + std::to_string(i)))) {
        active_tab = i;
        detail_open = true;
      }
      div(context, mk(root.ent(), 300 + static_cast<int>(i)),
          box(scale, 307, y + 5.f, 400, 35)
              .with_label(categories[i])
              .with_font("Archivo", h720(32.f))
              .with_custom_text_color(
                  active ? text : afterhours::Color{183, 223, 225, 230})
              .with_alignment(TextAlignment::Left)
              .with_ignore_pointer_events());
    }

    div(context, mk(root.ent(), 190),
        box(scale, 291, 566, 455, 46)
            .with_on_draw_bg([fill = afterhours::Color{37, 66, 75, 180},
                              line = panel_line](RectangleType r) {
              draw_holo_plate(r, fill, line, false);
            })
            .with_debug_name("ds_front_footer"));
    div(context, mk(root.ent(), 191),
        box(scale, 520, 581, 116, 27)
            .with_label("Enter SELECT")
            .with_font("Archivo@bold", h720(22.f))
            .with_custom_text_color(text)
            .with_debug_name("ds_prompt_select"));
    div(context, mk(root.ent(), 192),
        box(scale, 640, 581, 92, 27)
            .with_label("Esc BACK")
            .with_font("Archivo@bold", h720(22.f))
            .with_custom_text_color(text)
            .with_debug_name("ds_prompt_back"));

    if (!detail_open)
      return;

    const auto &rows = detail_rows[active_tab];
    div(context, mk(root.ent(), 250),
        box(scale, 804, 189, 365, 352)
            .with_custom_background(afterhours::Color{16, 43, 52, 118})
            .with_border(afterhours::Color{94, 140, 148, 185}, 1.f)
            .with_debug_name("ds_detail_panel"));

    if (button(context, mk(root.ent(), 251),
               box(scale, 824, 205, 120, 26)
                   .with_label("< SETTINGS")
                   .with_font("Archivo", h720(15.f))
                   .with_custom_text_color(muted)
                   .with_alignment(TextAlignment::Left)
                   .with_debug_name("ds_detail_back"))) {
      detail_open = false;
    }

    div(context, mk(root.ent(), 252),
        box(scale, 824, 242, 320, 34)
            .with_label(categories[active_tab])
            .with_font("Archivo", h720(25.f))
            .with_custom_text_color(text)
            .with_debug_name("ds_detail_title"));

    for (size_t i = 0; i < rows.size(); ++i) {
      const auto &row = rows[i];
      const size_t row_index = i < 4 ? i : 3;
      auto &value_index = selected_values[active_tab][row_index];
      if (value_index >= row.values.size())
        value_index = 0;
      const float y = 295.f + static_cast<float>(i) * 54.f;
      div(context, mk(root.ent(), 260 + static_cast<int>(i) * 10),
          box(scale, 824, y, 325, 46)
              .with_custom_background(afterhours::Color{13, 36, 44, 155})
              .with_border(afterhours::Color{55, 85, 94, 190}, 1.f)
              .with_debug_name("ds_detail_row_" + std::to_string(i)));
      div(context, mk(root.ent(), 261 + static_cast<int>(i) * 10),
          box(scale, 837, y + 11.f, 145, 25)
              .with_label(row.label)
              .with_font("Archivo", h720(18.f))
              .with_custom_text_color(text)
              .with_alignment(TextAlignment::Left));
      if (button(context, mk(root.ent(), 262 + static_cast<int>(i) * 10),
                 box(scale, 994, y + 7.f, 28, 31)
                     .with_label("<")
                     .with_font("Archivo@bold", h720(25.f))
                     .with_custom_text_color(text)
                     .with_alignment(TextAlignment::Center)
                     .with_debug_name("ds_cycle_prev_" + std::to_string(i)))) {
        value_index = (value_index + row.values.size() - 1) % row.values.size();
      }
      div(context, mk(root.ent(), 263 + static_cast<int>(i) * 10),
          box(scale, 1028, y + 10.f, 74, 26)
              .with_label(row.values[value_index])
              .with_font("Archivo", h720(18.f))
              .with_custom_text_color(afterhours::Color{190, 244, 247, 255})
              .with_alignment(TextAlignment::Center)
              .with_debug_name("ds_detail_value_" + std::to_string(i)));
      if (button(context, mk(root.ent(), 264 + static_cast<int>(i) * 10),
                 box(scale, 1108, y + 7.f, 28, 31)
                     .with_label(">")
                     .with_font("Archivo@bold", h720(25.f))
                     .with_custom_text_color(text)
                     .with_alignment(TextAlignment::Center)
                     .with_debug_name("ds_cycle_next_" + std::to_string(i)))) {
        value_index = (value_index + 1) % row.values.size();
      }
    }
  }
};

REGISTER_EXAMPLE_SCREEN(deadspace_settings, "Game Mockups",
                        "Sci-fi horror settings menu (Dead Space style)",
                        DeadSpaceSettingsScreen)
