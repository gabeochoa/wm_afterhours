#pragma once

#include "../external.h"
#include "../input_mapping.h"
#include "ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ExampleColors : afterhours::System<UIContext<InputAction>> {
  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    int screen_width = Settings::get().get_screen_width();
    int screen_height = Settings::get().get_screen_height();

    float box_size = 100.0f;
    float spacing = 20.0f;
    float start_x = (screen_width - (4 * box_size + 3 * spacing)) / 2.0f;
    float start_y = (screen_height - box_size) / 2.0f;

    afterhours::Color colors[] = {
        afterhours::Color{255, 100, 100, 255},
        afterhours::Color{100, 255, 100, 255},
        afterhours::Color{100, 100, 255, 255},
        afterhours::Color{255, 255, 100, 255},
    };

    std::string labels[] = {"Red", "Green", "Blue", "Yellow"};

    for (int i = 0; i < 4; i++) {
      float x = start_x + i * (box_size + spacing);
      div(context, mk(entity, i),
          ComponentConfig{}
              .with_label(labels[i])
              .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
              .with_absolute_position()
              .with_translate(x, start_y)
              .with_custom_color(colors[i])
              .with_padding(Spacing::xs)
              .with_font(UIComponent::DEFAULT_FONT, 18.0f)
              .with_debug_name("color_box_" + std::to_string(i)));
    }

    float title_y = start_y - 60.0f;
    div(context, mk(entity, 10),
        ComponentConfig{}
            .with_label("Color Examples")
            .with_size(
                ComponentSize{pixels(screen_width * 0.6f), pixels(40.0f)})
            .with_absolute_position()
            .with_translate((screen_width - screen_width * 0.6f) / 2.0f,
                            title_y)
            .with_custom_color(afterhours::Color{30, 30, 30, 200})
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, 24.0f)
            .with_debug_name("title"));
  }
};

REGISTER_EXAMPLE_SCREEN(colors, "Color system examples", ExampleColors)
