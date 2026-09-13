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
  bool settings_open = true;
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
    const std::array<Vector2Type, 6> vertices{{{r.x + cut, r.y + 3.f * scale},
                                               {r.x + r.width - cut, r.y},
                                               {r.x + r.width, r.y + cut},
                                               {r.x + r.width, r.y + r.height - 3.f * scale},
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
                            static_cast<unsigned char>(y > r.height * .22f && y < r.height * .82f ? 4 : bright ? 20 : 10)});
    }
  }

  static void draw_signal_field(RectangleType r) {
    afterhours::draw_rectangle(r, afterhours::Color{0, 0, 0, 255});
    for (int x = 150; x < 1170; x += 16) {
      for (int y = 100; y < 650; y += 28) {
        const float dx = (x - 660.f) / 520.f;
        const float dy = (y - 375.f) / 290.f;
        const float fade = std::max(0.f, 1.f - dx * dx - dy * dy);
        const auto alpha = static_cast<unsigned char>(9.f * fade * fade);
        if (alpha == 0) continue;
        const float px = r.x + x * r.width / 1280.f;
        const float py = r.y + y * r.height / 720.f;
        afterhours::draw_line_ex({px, py}, {px, py + 24.f * r.height / 720.f}, 1.f,
                                 afterhours::Color{91, 149, 162, alpha});
        afterhours::draw_line_ex({px, py}, {px + 13.f * r.width / 1280.f, py}, 1.f,
                                 afterhours::Color{80, 180, 190, alpha});
      }
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

    const bool detail_was_open = detail_open;
    auto &selection = settings_open ? active_tab : selected_initial;
    if (context.pressed(InputAction::WidgetDown))
      selection = (selection + 1) % categories.size();
    if (context.pressed(InputAction::WidgetUp))
      selection = (selection + categories.size() - 1) % categories.size();
    if (context.pressed(InputAction::WidgetPress)) {
      if (settings_open) detail_open = true;
      else if (selected_initial == 7) settings_open = true;
    }
    if (context.pressed(InputAction::MenuBack)) {
      if (detail_open) detail_open = false;
      else settings_open = false;
    }

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

    auto root =
        div(context, mk(entity, 0),
            ComponentConfig{}.with_size({pixels(screen_w), pixels(screen_h)}).with_corner_radius(0)
                .with_on_draw_bg([](RectangleType r) { draw_signal_field(r); })
                .with_debug_name("ds_root"));
    auto content = div(context, mk(root.ent(), 1), ComponentConfig{}
        .with_size({pixels(1280 * scale), pixels(720 * scale)})
        .with_absolute_position((screen_w - 1280 * scale) / 2, (screen_h - 720 * scale) / 2)
        .with_background(Theme::Usage::None).with_corner_radius(0));

    const float initial_x = settings_open ? 64.f : 448.f;
    div(context, mk(content.ent(), 10),
        box(scale, initial_x, 188, 325, 350)
            .with_on_draw_bg([fill = afterhours::Color{11, 25, 31, 112},
                              line = panel_line](RectangleType r) {
              draw_holo_plate(r, fill, line, false);
            })
            .with_debug_name("ds_back_layer"));

    div(context, mk(content.ent(), 11),
        box(scale, initial_x + 18, 196, 285, 28)
            .with_label("INITIAL SETTINGS")
            .with_font("ArchivoMockBold", pixels(23.f * scale))
             .with_custom_text_color(settings_open ? afterhours::Color{120, 164, 174, 155} : text)
            .with_debug_name("ds_initial_title"));

    for (size_t i = 0; i < initial_settings.size(); ++i) {
      const bool active = i == selected_initial;
      const float y = i == 0 ? 234.f : 271.f + static_cast<float>(i - 1) * 36.f;
      if (button(context, mk(content.ent(), 20 + static_cast<int>(i)),
                 box(scale, initial_x, y, 325, 31)
                     .with_label("")
                     .with_custom_text_color(
                         !settings_open ? text : active ? afterhours::Color{157, 204, 208, 185} : afterhours::Color{103, 139, 148, 140})
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
        if (i == 7) {
          detail_open = false;
          settings_open = true;
        }
      }
      div(context, mk(content.ent(), 60 + static_cast<int>(i)),
          box(scale, initial_x + 18, y + 1.f, 282, 28)
              .with_label(initial_settings[i])
              .with_font("ArchivoMock", pixels(23.f * scale))
              .with_custom_text_color(
                  !settings_open ? text : active ? afterhours::Color{157, 204, 208, 185} : afterhours::Color{103, 139, 148, 140})
              .with_alignment(TextAlignment::Left)
              .with_ignore_pointer_events());
    }

    if (!settings_open) {
      div(context, mk(content.ent(), 39), box(scale, 448, 554, 380, 32)
          .with_label("More Settings / Enter to return")
          .with_font("ArchivoMock", pixels(20 * scale))
          .with_custom_text_color(text).with_debug_name("ds_initial_hint"));
      return;
    }

    div(context, mk(content.ent(), 40),
        box(scale, 338, 62, 465, 42)
            .with_label("MORE SETTINGS")
            .with_font("ArchivoMockBold", pixels(36.f * scale))
            .with_custom_text_color(afterhours::Color{158, 211, 220, 255})
            .with_alignment(TextAlignment::Center)
            .with_debug_name("ds_more_settings"));

    div(context, mk(content.ent(), 41),
        box(scale, 338, 114, 465, 40)
            .with_label("")
            .with_on_draw_bg([fill = afterhours::Color{69, 103, 117, 140},
                              line = panel_line](RectangleType r) {
              draw_holo_plate(r, fill, line, true);
            })
            .with_debug_name("ds_front_header"));
    div(context, mk(content.ent(), 43),
        box(scale, 362, 119, 420, 30)
            .with_label("INITIAL SETTINGS > MORE SETTINGS")
             .with_font("ArchivoMock", pixels(22.f * scale))
            .with_custom_text_color(text)
            .with_alignment(TextAlignment::Left)
            .with_ignore_pointer_events());

    div(context, mk(content.ent(), 42),
        box(scale, 338, 114, 465, 526)
            .with_ignore_pointer_events()
            .with_debug_name("ds_front_layer"));

    for (size_t i = 0; i < categories.size(); ++i) {
      const bool active = i == active_tab;
      const float y = 166.f + static_cast<float>(i) * 47.f + (i >= 6 ? 22.f : 0.f);
      if (button(context, mk(content.ent(), 100 + static_cast<int>(i)),
                 box(scale, 338, y, 465, 44)
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
      div(context, mk(content.ent(), 500 + static_cast<int>(i)),
          box(scale, 356, y + 8, 26, 28).with_label(active ? ">" : "")
              .with_font("ArchivoMockBold", pixels(28 * scale)).with_custom_text_color(text)
              .with_ignore_pointer_events());
      div(context, mk(content.ent(), 300 + static_cast<int>(i)),
          box(scale, 390, y + 5.f, 386, 35)
              .with_label(categories[i])
              .with_font(active ? "ArchivoMockBold" : "ArchivoMock", pixels((i >= 6 ? 27.f : 31.f) * scale))
              .with_custom_text_color(
                  active ? text : afterhours::Color{183, 223, 225, 230})
              .with_alignment(TextAlignment::Left)
              .with_ignore_pointer_events());
    }

    div(context, mk(content.ent(), 190),
        box(scale, 338, 586, 465, 54)
            .with_on_draw_bg([fill = afterhours::Color{37, 66, 75, 180},
                              line = panel_line](RectangleType r) {
              draw_holo_plate(r, fill, line, false);
            })
            .with_debug_name("ds_front_footer"));
    div(context, mk(content.ent(), 191),
        box(scale, 446, 600, 110, 30)
            .with_label("SELECT")
            .with_font("ArchivoMockBold", pixels(22.f * scale))
            .with_custom_text_color(text)
            .with_debug_name("ds_prompt_select"));
    if (button(context, mk(content.ent(), 192),
        box(scale, 647, 600, 116, 30)
            .with_label("BACK")
            .with_font("ArchivoMockBold", pixels(22.f * scale))
            .with_custom_text_color(text)
            .with_debug_name("ds_prompt_back"))) {
      if (detail_was_open) detail_open = false;
      else settings_open = false;
    }

    div(context, mk(content.ent(), 194), box(scale, 362, 600, 70, 30)
        .with_label("Enter").with_font("ArchivoMockBold", pixels(22 * scale))
        .with_custom_background(text).with_custom_text_color(panel_dark).with_corner_radius(3 * scale));
    div(context, mk(content.ent(), 195), box(scale, 583, 600, 54, 30)
        .with_label("Esc").with_font("ArchivoMockBold", pixels(22 * scale))
        .with_custom_background(text).with_custom_text_color(panel_dark).with_corner_radius(3 * scale));
    div(context, mk(content.ent(), 196), box(scale, 362, 446, 420, 23)
        .with_label("INFORMATION").with_font("ArchivoMock", pixels(18 * scale))
        .with_custom_text_color(muted).with_ignore_pointer_events());
    const std::array<std::array<const char *, 2>, 8> previews{{
        {{"Adjust aiming, vibration,", "and movement controls."}},
        {{"Set difficulty, objectives,", "automatic saves, and hints."}},
        {{"Tune brightness, field of view,", "and cinematic effects."}},
        {{"Balance music and effects,", "then choose a dynamic range."}},
        {{"Choose voice and text languages,", "subtitles, and subtitle size."}},
        {{"Adjust contrast, camera motion,", "and reading assistance."}},
        {{"Review policy entries and", "data collection preferences."}},
        {{"View the studio, engine,", "build, and acknowledgments."}},
    }};
    if (!detail_open) {
      div(context, mk(content.ent(), 240), box(scale, 844, 164, 380, 248)
          .with_custom_background(afterhours::Color{16, 43, 52, 100})
          .with_border(panel_line, scale).with_debug_name("ds_preview_panel"));
      div(context, mk(content.ent(), 241), box(scale, 864, 186, 340, 40)
          .with_label(categories[active_tab]).with_font("ArchivoMockBold", pixels(28 * scale))
          .with_custom_text_color(text).with_debug_name("ds_preview_title"));
      for (size_t line = 0; line < 2; ++line)
        div(context, mk(content.ent(), 242 + static_cast<int>(line)),
            box(scale, 864, 248 + static_cast<float>(line) * 31, 340, 29)
                .with_label(previews[active_tab][line]).with_font("ArchivoMock", pixels(23 * scale))
                .with_custom_text_color(muted));
      div(context, mk(content.ent(), 244), box(scale, 864, 342, 340, 32)
          .with_label("4 entries / Enter to open").with_font("ArchivoMock", pixels(21 * scale))
          .with_custom_text_color(text));
      return;
    }

    const auto &rows = detail_rows[active_tab];
    div(context, mk(content.ent(), 250),
        box(scale, 844, 164, 380, 412)
            .with_custom_background(afterhours::Color{16, 43, 52, 118})
            .with_border(afterhours::Color{94, 140, 148, 185}, 1.f)
            .with_debug_name("ds_detail_panel"));

    if (button(context, mk(content.ent(), 251),
               box(scale, 864, 178, 200, 28)
                   .with_label("< SETTINGS")
                   .with_font("ArchivoMock", pixels(20.f * scale))
                   .with_custom_text_color(muted)
                   .with_alignment(TextAlignment::Left)
                   .with_debug_name("ds_detail_back"))) {
      detail_open = false;
    }

    div(context, mk(content.ent(), 252),
        box(scale, 864, 213, 340, 38)
            .with_label(categories[active_tab])
            .with_font("ArchivoMock", pixels(25.f * scale))
            .with_custom_text_color(text)
            .with_debug_name("ds_detail_title"));

    for (size_t i = 0; i < rows.size(); ++i) {
      const auto &row = rows[i];
      const size_t row_index = i < 4 ? i : 3;
      auto &value_index = selected_values[active_tab][row_index];
      if (value_index >= row.values.size())
        value_index = 0;
      const float y = 270.f + static_cast<float>(i) * 72.f;
      div(context, mk(content.ent(), 260 + static_cast<int>(i) * 10),
          box(scale, 864, y, 340, 66)
              .with_custom_background(afterhours::Color{13, 36, 44, 155})
              .with_border(afterhours::Color{55, 85, 94, 190}, 1.f)
              .with_debug_name("ds_detail_row_" + std::to_string(i)));
      div(context, mk(content.ent(), 261 + static_cast<int>(i) * 10),
          box(scale, 878, y + 2.f, 310, 27)
              .with_label(row.label)
              .with_font("ArchivoMock", pixels(23.f * scale))
              .with_custom_text_color(text)
              .with_alignment(TextAlignment::Left));
      if (button(context, mk(content.ent(), 262 + static_cast<int>(i) * 10),
                 box(scale, 876, y + 30.f, 32, 31)
                     .with_label("<")
                     .with_font("ArchivoMockBold", pixels(25.f * scale))
                     .with_custom_text_color(text)
                     .with_alignment(TextAlignment::Center)
                     .with_debug_name("ds_cycle_prev_" + std::to_string(i)))) {
        value_index = (value_index + row.values.size() - 1) % row.values.size();
      }
      div(context, mk(content.ent(), 263 + static_cast<int>(i) * 10),
          box(scale, 918, y + 30.f, 232, 31)
              .with_label(row.values[value_index])
              .with_font("ArchivoMock", pixels(23.f * scale))
              .with_custom_text_color(afterhours::Color{190, 244, 247, 255})
              .with_alignment(TextAlignment::Center)
              .with_debug_name("ds_detail_value_" + std::to_string(i)));
      if (button(context, mk(content.ent(), 264 + static_cast<int>(i) * 10),
                 box(scale, 1158, y + 30.f, 32, 31)
                     .with_label(">")
                     .with_font("ArchivoMockBold", pixels(25.f * scale))
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
