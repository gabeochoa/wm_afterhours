#pragma once

#include "../external.h"
#include "../input_mapping.h"
#include "ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ExampleSliders : afterhours::System<UIContext<InputAction>> {
  float slider_1 = 0.0f;
  float slider_2 = 0.5f;
  float slider_3 = 1.0f;
  float slider_with_label = 0.25f;
  float slider_with_label_newline = 0.75f;
  float slider_on_handle = 0.4f;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    int screen_width = Settings::get().get_screen_width();
    int screen_height = Settings::get().get_screen_height();

    float start_y = screen_height * 0.1f;
    float spacing = 80.0f;
    float component_width = 400.0f;
    float component_height = 50.0f;
    float start_x = (screen_width - component_width) / 2.0f;

    div(context, mk(entity, 100),
        ComponentConfig{}
            .with_label("Screen: sliders - Slider Test Screen")
            .with_size(
                ComponentSize{pixels(screen_width * 0.6f), pixels(40.0f)})
            .with_absolute_position()
            .with_translate((screen_width - screen_width * 0.6f) / 2.0f,
                            start_y - 60.0f)
            .with_custom_color(afterhours::Color{40, 40, 40, 255})
            .with_font(UIComponent::DEFAULT_FONT, 28.0f)
            .with_debug_name("title"));

    float current_y = start_y;

    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_label("Slider at 0%")
            .with_size(ComponentSize{pixels(component_width), pixels(30.0f)})
            .with_absolute_position()
            .with_translate(start_x, current_y)
            .with_custom_color(afterhours::Color{30, 30, 30, 200})
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_debug_name("label_1"));

    current_y += 35.0f;
    slider(context, mk(entity, 10), slider_1,
           ComponentConfig{}
               .with_size(ComponentSize{pixels(component_width),
                                        pixels(component_height)})
               .with_absolute_position()
               .with_translate(start_x, current_y)
               .with_debug_name("slider_1"),
           SliderHandleValueLabelPosition::None);

    current_y += spacing;

    div(context, mk(entity, 2),
        ComponentConfig{}
            .with_label("Slider at 50%")
            .with_size(ComponentSize{pixels(component_width), pixels(30.0f)})
            .with_absolute_position()
            .with_translate(start_x, current_y)
            .with_custom_color(afterhours::Color{30, 30, 30, 200})
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_debug_name("label_2"));

    current_y += 35.0f;
    slider(context, mk(entity, 11), slider_2,
           ComponentConfig{}
               .with_size(ComponentSize{pixels(component_width),
                                        pixels(component_height)})
               .with_absolute_position()
               .with_translate(start_x, current_y)
               .with_debug_name("slider_2"),
           SliderHandleValueLabelPosition::None);

    current_y += spacing;

    div(context, mk(entity, 3),
        ComponentConfig{}
            .with_label("Slider at 100%")
            .with_size(ComponentSize{pixels(component_width), pixels(30.0f)})
            .with_absolute_position()
            .with_translate(start_x, current_y)
            .with_custom_color(afterhours::Color{30, 30, 30, 200})
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_debug_name("label_3"));

    current_y += 35.0f;
    slider(context, mk(entity, 12), slider_3,
           ComponentConfig{}
               .with_size(ComponentSize{pixels(component_width),
                                        pixels(component_height)})
               .with_absolute_position()
               .with_translate(start_x, current_y)
               .with_debug_name("slider_3"),
           SliderHandleValueLabelPosition::None);

    current_y += spacing;

    slider(context, mk(entity, 13), slider_with_label,
           ComponentConfig{}
               .with_label("Volume")
               .with_size(ComponentSize{pixels(component_width),
                                        pixels(component_height)})
               .with_absolute_position()
               .with_translate(start_x, current_y)
               .with_debug_name("slider_with_label"),
           SliderHandleValueLabelPosition::WithLabel);

    current_y += spacing;

    slider(context, mk(entity, 14), slider_with_label_newline,
           ComponentConfig{}
               .with_label("Brightness")
               .with_size(ComponentSize{pixels(component_width),
                                        pixels(component_height)})
               .with_absolute_position()
               .with_translate(start_x, current_y)
               .with_debug_name("slider_with_label_newline"),
           SliderHandleValueLabelPosition::WithLabelNewLine);

    current_y += spacing;

    slider(context, mk(entity, 15), slider_on_handle,
           ComponentConfig{}
               .with_label("Value on Handle")
               .with_size(ComponentSize{pixels(component_width),
                                        pixels(component_height)})
               .with_absolute_position()
               .with_translate(start_x, current_y)
               .with_debug_name("slider_on_handle"),
           SliderHandleValueLabelPosition::OnHandle);
  }
};

REGISTER_EXAMPLE_SCREEN(sliders,
                        "Slider test screen with different configurations",
                        ExampleSliders)
