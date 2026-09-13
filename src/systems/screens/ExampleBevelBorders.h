#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <array>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ExampleBevelBordersScreen : ScreenSystem<UIContext<InputAction>> {
  // Use richer colors instead of pure gray
  afterhours::Color bg{45, 55, 75, 255};      // Muted blue-gray background
  afterhours::Color panel{65, 78, 102, 255};  // Panel surface
  afterhours::Color text{235, 235, 240, 255}; // High contrast light text
  afterhours::Color light_edge{180, 190, 205, 255}; // Bevel light edge
  afterhours::Color dark_edge{30, 38, 52, 255};     // Bevel dark edge
  afterhours::Color sunken_text{200, 205, 215,
                                255}; // Slightly dimmer for sunken

  int selected_thickness = 6;
  BevelStyle selected_style = BevelStyle::Raised;

  // Configurable label sizes
  float column_header_font_size =
      22.0f; // Font size for column headers ("1px", "2px", etc.)
  float cell_label_font_size = 22.0f; // Font size for labels inside bevel cells

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    Theme theme;
    theme.font = text;
    theme.darkfont = afterhours::Color{20, 25, 35, 255};
    theme.font_muted = afterhours::Color{160, 170, 190, 255};
    theme.background = bg;
    theme.surface = panel;
    theme.primary = afterhours::Color{90, 140, 210, 255};
    theme.secondary = afterhours::Color{130, 175, 230, 255};
    theme.accent = afterhours::Color{235, 175, 80, 255};
    theme.error = afterhours::Color{200, 80, 80, 255};
    theme.roundness = 0.0f;
    context.theme = theme;

    context.scaling_mode = ScalingMode::Proportional;
    const float s = std::min(context.screen_height / 720.f, context.screen_width / 1280.f);
    const float top = (context.screen_height / s - 720.f) / 2.f;
    const float left = (context.screen_width / s - 1144.f) / 2.f;
    const afterhours::Color muted{186, 197, 215, 255};
    const afterhours::Color line{101, 119, 146, 255};
    const auto box = [s, top](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, (y + top) * s).with_corner_radius(0)
          .with_background(Theme::Usage::None);
    };
    // Background
    auto root = div(context, mk(entity), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background(bg).with_corner_radius(0).with_debug_name("bevel_canvas"));
    int id = 0;
    const auto label = [&](const std::string &value, float x, float y, float w, float h, float size,
                           afterhours::Color color, const std::string &name = "", bool emphasis = false) {
      return div(context, mk(root.ent(), id++), box(left + x, y, w, h).with_label(value)
          .with_font(emphasis ? "AtkinsonMockBold" : "AtkinsonMock", pixels(size * s))
          .with_custom_text_color(color).with_alignment(TextAlignment::Left)
          .with_ignore_pointer_events().with_debug_name(name));
    };
    div(context, mk(root.ent(), id++), box(left, 24, 1144, 84)
        .with_custom_background({36, 45, 62, 255}).with_corner_radius(12 * s));
    label("Bevel borders", 20, 31, 1104, 42, 34, text);
    label(fmt::format("Outer squares: {:.0f} x {:.0f}px. Select one to inspect its edge layers below.", 100 * s, 100 * s),
          20, 77, 1104, 25, 21, muted);
    for (int row = 0; row < 2; ++row) {
      const float x = row * 420.f;
      div(context, mk(root.ent(), id++), box(left + x, 124, 404, 68)
          .with_custom_background({36, 45, 62, 255}).with_corner_radius(9 * s));
      div(context, mk(root.ent(), id++), box(left + x + 28, 134, 48, 48)
          .with_custom_background(panel).with_bevel(row == 0 ? BevelStyle::Raised : BevelStyle::Sunken,
                                                    light_edge, dark_edge, 3.f)
          .with_ignore_pointer_events());
      div(context, mk(root.ent(), id++), box(left + x + 8, 129, 20, 22)
          .with_ignore_pointer_events().with_on_draw_fg([s](RectangleType r) {
            const raylib::Color color{231, 200, 135, 255};
            raylib::DrawLineEx({r.x, r.y}, {r.x + 16 * s, r.y + 16 * s}, 2 * s, color);
            raylib::DrawLineEx({r.x + 8 * s, r.y + 16 * s}, {r.x + 16 * s, r.y + 16 * s}, 2 * s, color);
            raylib::DrawLineEx({r.x + 16 * s, r.y + 8 * s}, {r.x + 16 * s, r.y + 16 * s}, 2 * s, color);
          }));
      label(row == 0 ? "Raised / highlight top + left" : "Sunken / shadow top + left", x + 88, 132, 306, 26, 20, text);
      label(row == 0 ? "Shadow bottom + right" : "Highlight bottom + right", x + 88, 162, 306, 24, 18, muted);
    }
    const std::array<afterhours::Color, 3> swatches{panel, light_edge, dark_edge};
    const std::array<const char *, 3> swatch_labels{"Face #414E66", "Highlight #B4BECD", "Shadow #1E2634"};
    for (int i = 0; i < 3; ++i) {
      div(context, mk(root.ent(), id++), box(left + 860, 125 + i * 23.f, 18, 18)
          .with_custom_background(swatches[i]).with_border(line, 1.f).with_ignore_pointer_events());
      label(swatch_labels[i], 888, 121 + i * 23.f, 250, 26, 19, text);
    }

    div(context, mk(root.ent(), id++), box(left, 206, 1144, 302)
        .with_custom_background({36, 45, 62, 255}).with_corner_radius(12 * s));
    label(fmt::format("Outer {:.0f}px", 100.f * s), 12, 213, 122, 26, 17, muted, "bevel_outer_size");
    label("Inner size", 12, 242, 122, 25, 17, muted);
    for (int column = 0; column < 7; ++column) {
      const float x = 144.f + column * 136.f;
      label(fmt::format("{}px", column), x + 16, 209, 88, 30,
            column_header_font_size * 1.3f, text, "col_label_" + std::to_string(column), true);
      const float inner = 100.f * s - 2.f * column;
      label(fmt::format("{:.0f} x {:.0f}", inner, inner), x - 2, 241, 112, 25,
            22, muted, "interior_" + std::to_string(column), true);
      for (int row = 0; row < 2; ++row) {
        const auto style = row == 0 ? BevelStyle::Raised : BevelStyle::Sunken;
        const std::string name = column == 0 ? "bevel_zero_" + std::to_string(row)
                                             : "bevel_" + std::to_string(row) + "_" + std::to_string(column - 1);
        if (button(context, mk(root.ent(), id++), box(left + x, 276 + row * 120.f, 100, 100)
            .with_label("+").with_font("AtkinsonMock", pixels(cell_label_font_size * s))
            .with_custom_text_color(row == 0 ? text : sunken_text).with_alignment(TextAlignment::Center)
            .with_custom_background(panel).with_custom_hover_bg(panel)
            .with_bevel(style, light_edge, dark_edge, static_cast<float>(column))
            .with_debug_name(name))) {
          selected_style = style;
          selected_thickness = column;
        }
      }
    }
    label("Raised", 24, 307, 110, 32, 23, text, "row_label_0");
    label("Sunken", 24, 427, 110, 32, 23, text, "row_label_1");
    label("0px = flat", 143, 377, 116, 20, 15, muted);
    div(context, mk(root.ent(), id++), box(left + 1072, 276, 66, 220)
        .with_ignore_pointer_events().with_on_draw_fg([s, line](RectangleType r) {
          const float x = r.x;
          raylib::DrawLineEx({x, r.y}, {x, r.y + 6.f}, s, line);
          raylib::DrawLineEx({x - 4 * s, r.y}, {x + 4 * s, r.y}, s, line);
          raylib::DrawLineEx({x - 4 * s, r.y + 6.f}, {x + 4 * s, r.y + 6.f}, s, line);
          raylib::DrawLineEx({x + 20 * s, r.y + 74 * s}, {x + 20 * s, r.y + 166 * s}, s, line);
          raylib::DrawLineEx({x + 14 * s, r.y + 160 * s}, {x + 20 * s, r.y + 166 * s}, s, line);
          raylib::DrawLineEx({x + 26 * s, r.y + 160 * s}, {x + 20 * s, r.y + 166 * s}, s, line);
        }));
    label("6px", 1083, 271, 54, 25, 17, text);
    label("Invert", 1064, 379, 74, 24, 16, muted);

    div(context, mk(root.ent(), id++), box(left, 524, 274, 170)
        .with_custom_background({36, 45, 62, 255}).with_corner_radius(12 * s));
    label("1px corner, enlarged", 16, 533, 244, 29, 22, text);
    div(context, mk(root.ent(), id++), box(left + 20, 578, 84, 84)
        .with_custom_background(panel).with_ignore_pointer_events()
        .with_on_draw_fg([color = light_edge](RectangleType r) {
          const float pixel = r.width / 12.f;
          afterhours::draw_rectangle({r.x, r.y, r.width, pixel}, color);
          afterhours::draw_rectangle({r.x, r.y, pixel, r.height}, color);
        }));
    label("Top + left", 120, 578, 144, 24, 19, text);
    label("1 outer layer", 120, 609, 144, 24, 18, muted);
    label("Raised corner", 120, 642, 144, 24, 18, muted);
    div(context, mk(root.ent(), id++), box(left + 290, 524, 854, 170)
        .with_custom_background({36, 45, 62, 255}).with_corner_radius(12 * s));
    const bool raised = selected_style == BevelStyle::Raised;
    label(fmt::format("Selected: {} / {}px / {:.0f}px interior", raised ? "raised" : "sunken",
                      selected_thickness, 100 * s - 2 * selected_thickness),
          306, 532, 822, 30, 23, text, "bevel_selection");
    label("Top / left", 310, 572, 140, 24, 18, muted);
    label("Face", 693, 572, 96, 24, 18, muted);
    label("Bottom / right", 948, 572, 180, 24, 18, muted);
    div(context, mk(root.ent(), id++), box(left + 466, 602, 500, 34)
        .with_custom_background(panel).with_ignore_pointer_events()
        .with_on_draw_fg([s, raised, count = selected_thickness, face = panel,
                          highlight = light_edge, shadow = dark_edge](RectangleType r) {
          const float pixel = r.width / (100.f * s);
          for (int layer = 0; layer < count; ++layer) {
            const auto bright = layer == 0 ? highlight : afterhours::colors::lighten(face, .35f);
            const auto dark = layer == 0 ? shadow : afterhours::colors::darken(face, .35f);
            afterhours::draw_rectangle({r.x + layer * pixel, r.y, pixel, r.height}, raised ? bright : dark);
            afterhours::draw_rectangle({r.x + r.width - (layer + 1) * pixel, r.y, pixel, r.height}, raised ? dark : bright);
          }
        }));
    label("5x horizontal slice", 306, 648, 230, 26, 18, text);
    label("Outer: edge color. Inner: 35% toward white / 35% face RGB.", 542, 648, 586, 26, 18, muted);
  }
};

REGISTER_EXAMPLE_SCREEN(bevel_borders, "Component Galleries",
                        "Bevel styles and thickness levels",
                        ExampleBevelBordersScreen)
