#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ExampleBevelBordersScreen : ScreenSystem<UIContext<InputAction>> {
  afterhours::Color bg{176, 176, 176, 255};
  afterhours::Color panel{210, 210, 210, 255};
  afterhours::Color text{30, 30, 30, 255};
  afterhours::Color light_edge{248, 248, 248, 255};
  afterhours::Color dark_edge{64, 64, 64, 255};
  afterhours::Color sunken_text{230, 230, 230, 255};

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    Theme theme;
    theme.font = text;
    theme.darkfont = afterhours::Color{0, 0, 0, 255};
    theme.font_muted = afterhours::Color{80, 80, 80, 255};
    theme.background = bg;
    theme.surface = panel;
    theme.primary = afterhours::Color{70, 120, 200, 255};
    theme.secondary = afterhours::Color{110, 160, 220, 255};
    theme.accent = afterhours::Color{235, 175, 80, 255};
    theme.error = afterhours::Color{200, 80, 80, 255};
    theme.roundness = 0.0f;
    context.theme = theme;

    int screen_width = Settings::get().get_screen_width();
    int screen_height = Settings::get().get_screen_height();

    // Background
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(
                ComponentSize{pixels(screen_width), pixels(screen_height)})
            .with_custom_background(bg)
            .with_debug_name("bg"));

    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_label("BEVEL BORDERS")
            .with_size(ComponentSize{pixels(screen_width - 60.0f), pixels(40)})
            .with_absolute_position()
            .with_translate(30.0f, 20.0f)
            .with_custom_background(panel)
            .with_border(afterhours::Color{120, 120, 120, 255}, 2.0f)
            .with_font(UIComponent::DEFAULT_FONT, 22.0f)
            .with_custom_text_color(text)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("title"));

    div(context, mk(entity, 2),
        ComponentConfig{}
            .with_label(
                "Raised: light top/left, dark bottom/right. Sunken: inverted.")
            .with_size(
                ComponentSize{pixels(screen_width - 60.0f), pixels(24.0f)})
            .with_absolute_position()
            .with_translate(30.0f, 64.0f)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_custom_text_color(text)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("legend"));

    struct StyleRow {
      const char *label;
      BevelStyle style;
    };

    StyleRow rows[] = {{"Raised", BevelStyle::Raised},
                       {"Sunken", BevelStyle::Sunken}};
    float thicknesses[] = {1.0f, 2.0f, 3.0f, 4.0f};

    float cell = 110.0f;
    float gap = 18.0f;
    float start_x = 190.0f;
    float start_y = 120.0f;

    for (int c = 0; c < 4; ++c) {
      div(context, mk(entity, 10 + c),
          ComponentConfig{}
              .with_label(fmt::format("{}px", static_cast<int>(thicknesses[c])))
              .with_size(ComponentSize{pixels(cell), pixels(28.0f)})
              .with_absolute_position()
              .with_translate(start_x + c * (cell + gap), start_y - 36.0f)
              .with_font(UIComponent::DEFAULT_FONT, 14.0f)
              .with_custom_text_color(text)
              .with_alignment(TextAlignment::Center)
              .with_debug_name("col_label_" + std::to_string(c)));
    }

    for (int r = 0; r < 2; ++r) {
      div(context, mk(entity, 20 + r),
          ComponentConfig{}
              .with_label(rows[r].label)
              .with_size(ComponentSize{pixels(120.0f), pixels(cell)})
              .with_absolute_position()
              .with_translate(40.0f, start_y + r * (cell + gap))
              .with_font(UIComponent::DEFAULT_FONT, 16.0f)
              .with_custom_text_color(text)
              .with_alignment(TextAlignment::Center)
              .with_debug_name("row_label_" + std::to_string(r)));

      for (int c = 0; c < 4; ++c) {
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
                .with_absolute_position()
                .with_translate(x, y)
                .with_custom_background(fill)
                .with_bevel(rows[r].style, light_edge, dark_edge,
                            thicknesses[c])
                .with_font(UIComponent::DEFAULT_FONT, 14.0f)
                .with_custom_text_color(label_color)
                .with_alignment(TextAlignment::Center)
                .with_debug_name("bevel_" + std::to_string(r) + "_" +
                                 std::to_string(c)));
      }
    }

    float big_x = start_x + 4 * (cell + gap) + 30.0f;
    float big_y = start_y;
    float big_size = 170.0f;

    div(context, mk(entity, 300),
        ComponentConfig{}
            .with_label("Raised 6px")
            .with_size(ComponentSize{pixels(big_size), pixels(big_size)})
            .with_absolute_position()
            .with_translate(big_x, big_y)
            .with_custom_background(panel)
            .with_bevel(BevelStyle::Raised, light_edge, dark_edge, 6.0f)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_custom_text_color(text)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("big_raised"));

    div(context, mk(entity, 301),
        ComponentConfig{}
            .with_label("Sunken 6px")
            .with_size(ComponentSize{pixels(big_size), pixels(big_size)})
            .with_absolute_position()
            .with_translate(big_x, big_y + big_size + gap)
            .with_custom_background(panel)
            .with_bevel(BevelStyle::Sunken, light_edge, dark_edge, 6.0f)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_custom_text_color(sunken_text)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("big_sunken"));
  }
};

REGISTER_EXAMPLE_SCREEN(bevel_borders, "Component Galleries",
                        "Bevel styles and thickness levels",
                        ExampleBevelBordersScreen)
