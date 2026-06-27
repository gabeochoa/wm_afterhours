#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

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

    int screen_width = Settings::get().get_screen_width();
    int screen_height = Settings::get().get_screen_height();

    // Center content vertically: content spans ~336px (title to grid bottom)
    float y_offset = std::max(0.0f, (screen_height - 336.0f) / 2.0f - 20.0f);

    // Background
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(
                ComponentSize{screen_pct(1.0f), screen_pct(1.0f)})
            .with_custom_background(bg)
            .with_debug_name("bg"));

    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_label("BEVEL BORDERS")
            .with_size(ComponentSize{pixels(screen_width - 60.0f), pixels(44)})
            .with_absolute_position(30.0f, 20.0f + y_offset)
            .with_custom_background(panel)
            .with_border(afterhours::Color{120, 130, 150, 255}, 2.0f)
            .with_font(UIComponent::DEFAULT_FONT, h720(24.0f))
            .with_custom_text_color(text)
            .with_alignment(TextAlignment::Center));

    div(context, mk(entity, 2),
        ComponentConfig{}
            .with_label(
                "Raised: light top/left, dark bottom/right. Sunken: inverted.")
            .with_size(
                ComponentSize{pixels(screen_width - 60.0f), pixels(24.0f)})
            .with_absolute_position(30.0f, 68.0f + y_offset)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(text)
            .with_alignment(TextAlignment::Center));

    struct StyleRow {
      const char *label;
      BevelStyle style;
    };

    StyleRow rows[] = {{"Raised", BevelStyle::Raised},
                       {"Sunken", BevelStyle::Sunken}};
    float thicknesses[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};

    float cell = 100.0f;
    float gap = 16.0f;
    float row_label_width = 100.0f;
    float col_header_height = 28.0f;

    // Calculate total content width and center horizontally
    float grid_width = 6 * cell + 5 * gap; // 6 cells with 5 gaps between
    float content_width = row_label_width + 20.0f + grid_width;

    // Header area: title bar ends at ~60, legend ends at ~88
    float header_bottom = 96.0f + y_offset;

    float start_x =
        (screen_width - content_width) / 2.0f + row_label_width + 20.0f;
    float start_y = header_bottom + col_header_height + 16.0f;

    for (int c = 0; c < 6; ++c) {
      div(context, mk(entity, 10 + c),
          ComponentConfig{}
              .with_label(fmt::format("{}px", static_cast<int>(thicknesses[c])))
              .with_size(ComponentSize{pixels(cell), pixels(col_header_height)})
              .with_absolute_position(start_x + c * (cell + gap),
                                      start_y - col_header_height - 8.0f)
              .with_font(UIComponent::DEFAULT_FONT,
                         h720(column_header_font_size))
              .with_custom_text_color(text)
              .with_alignment(TextAlignment::Center)
              .with_debug_name("col_label_" + std::to_string(c)));
    }

    float row_label_x = (screen_width - content_width) / 2.0f;

    for (int r = 0; r < 2; ++r) {
      div(context, mk(entity, 20 + r),
          ComponentConfig{}
              .with_label(rows[r].label)
              .with_size(ComponentSize{pixels(row_label_width), pixels(cell)})
              .with_absolute_position(row_label_x, start_y + r * (cell + gap))
              .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
              .with_custom_text_color(text)
              .with_alignment(TextAlignment::Center)
              .with_debug_name("row_label_" + std::to_string(r)));

      for (int c = 0; c < 6; ++c) {
        float x = start_x + c * (cell + gap);
        float y = start_y + r * (cell + gap);
        afterhours::Color fill = panel;
        afterhours::Color label_color =
            (rows[r].style == BevelStyle::Raised) ? text : sunken_text;
        div(context, mk(entity, 100 + r * 10 + c),
            ComponentConfig{}
                .with_label(
                    fmt::format("{}px", static_cast<int>(thicknesses[c])))
                .with_size(ComponentSize{pixels(cell), pixels(cell)})
                .with_absolute_position(x, y)
                .with_custom_background(fill)
                .with_bevel(rows[r].style, light_edge, dark_edge,
                            thicknesses[c])
                .with_font(UIComponent::DEFAULT_FONT,
                           h720(cell_label_font_size))
                .with_custom_text_color(label_color)
                .with_alignment(TextAlignment::Center)
                .with_debug_name("bevel_" + std::to_string(r) + "_" +
                                 std::to_string(c)));
      }
    }
  }
};

REGISTER_EXAMPLE_SCREEN(bevel_borders, "Component Galleries",
                        "Bevel styles and thickness levels",
                        ExampleBevelBordersScreen)
