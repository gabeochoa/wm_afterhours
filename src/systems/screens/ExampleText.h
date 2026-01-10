#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ExampleText : afterhours::System<UIContext<InputAction>> {
  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    int screen_width = Settings::get().get_screen_width();
    int screen_height = Settings::get().get_screen_height();

    float y_pos = screen_height * 0.2f;
    float spacing = 50.0f;
    float text_width = screen_width * 0.7f;
    float text_height = 40.0f;
    float start_x = (screen_width - text_width) / 2.0f;

    struct TextExample {
      std::string label;
      float font_size;
      afterhours::Color color;
    };

    TextExample examples[] = {
        {"Large Text (32px)", 32.0f, afterhours::Color{255, 255, 255, 255}},
        {"Medium Text (24px)", 24.0f, afterhours::Color{200, 200, 200, 255}},
        {"Small Text (18px)", 18.0f, afterhours::Color{150, 150, 150, 255}},
        {"Colored Text", 20.0f, afterhours::Color{100, 200, 255, 255}},
    };

    for (size_t i = 0; i < sizeof(examples) / sizeof(examples[0]); i++) {
      div(context, mk(entity, static_cast<int>(i)),
          ComponentConfig{}
              .with_label(examples[i].label)
              .with_size(ComponentSize{pixels(text_width), pixels(text_height)})
              .with_absolute_position()
              .with_translate(start_x, y_pos + i * spacing)
              .with_custom_background(afterhours::Color{30, 30, 30, 200})
              .with_padding(Spacing::xs)
              .with_font(UIComponent::DEFAULT_FONT, examples[i].font_size)
              .with_debug_name("text_example_" + std::to_string(i)));
    }

    float title_y = y_pos - 60.0f;
    div(context, mk(entity, 10),
        ComponentConfig{}
            .with_label("Text Rendering Examples")
            .with_size(
                ComponentSize{pixels(screen_width * 0.6f), pixels(40.0f)})
            .with_absolute_position()
            .with_translate((screen_width - screen_width * 0.6f) / 2.0f,
                            title_y)
            .with_custom_background(afterhours::Color{40, 40, 40, 255})
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, 28.0f)
            .with_debug_name("title"));
  }
};

REGISTER_EXAMPLE_SCREEN(text, "System Demos",
                        "Text rendering examples with different sizes",
                        ExampleText)
