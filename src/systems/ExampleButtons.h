#pragma once

#include "../external.h"
#include "../input_mapping.h"
#include "ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <vector>
#include <string>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ExampleButtons : afterhours::System<UIContext<InputAction>> {
  int button_1_clicks = 0;
  int button_2_clicks = 0;
  int button_3_clicks = 0;
  int button_group_clicks = 0;
  int button_group_selected = -1;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    int screen_width = Settings::get().get_screen_width();
    int screen_height = Settings::get().get_screen_height();

    float start_y = screen_height * 0.1f;
    float spacing = 80.0f;
    float component_width = 300.0f;
    float component_height = 60.0f;
    float start_x = (screen_width - component_width) / 2.0f;

    div(context, mk(entity, 100),
        ComponentConfig{}
            .with_label("Screen: buttons - Button Test Screen")
            .with_size(ComponentSize{pixels(screen_width * 0.6f), pixels(40.0f)})
            .with_absolute_position()
            .with_translate((screen_width - screen_width * 0.6f) / 2.0f,
                            start_y - 60.0f)
            .with_custom_color(afterhours::Color{40, 40, 40, 255})
            .with_font(UIComponent::DEFAULT_FONT, 28.0f)
            .with_debug_name("title"));

    float current_y = start_y;

    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_label("Individual Buttons")
            .with_size(ComponentSize{pixels(component_width), pixels(30.0f)})
            .with_absolute_position()
            .with_translate(start_x, current_y)
            .with_custom_color(afterhours::Color{30, 30, 30, 200})
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_debug_name("section_label_1"));

    current_y += 40.0f;

    auto button_1_result = button(
        context, mk(entity, 10),
        ComponentConfig{}
            .with_label("Button 1 (Clicks: " + std::to_string(button_1_clicks) + ")")
            .with_size(ComponentSize{pixels(component_width), pixels(component_height)})
            .with_absolute_position()
            .with_translate(start_x, current_y)
            .with_custom_color(afterhours::Color{100, 150, 200, 255})
            .with_font(UIComponent::DEFAULT_FONT, 20.0f)
            .with_debug_name("button_1"));

    if (button_1_result) {
      button_1_clicks++;
      log_info("Button 1 clicked: {}", button_1_clicks);
    }

    current_y += spacing;

    auto button_2_result = button(
        context, mk(entity, 11),
        ComponentConfig{}
            .with_label("Button 2 (Clicks: " + std::to_string(button_2_clicks) + ")")
            .with_size(ComponentSize{pixels(component_width), pixels(component_height)})
            .with_absolute_position()
            .with_translate(start_x, current_y)
            .with_custom_color(afterhours::Color{200, 100, 150, 255})
            .with_font(UIComponent::DEFAULT_FONT, 20.0f)
            .with_debug_name("button_2"));

    if (button_2_result) {
      button_2_clicks++;
      log_info("Button 2 clicked: {}", button_2_clicks);
    }

    current_y += spacing;

    auto button_3_result = button(
        context, mk(entity, 12),
        ComponentConfig{}
            .with_label("Small Button")
            .with_size(ComponentSize{pixels(component_width * 0.5f), pixels(component_height * 0.7f)})
            .with_absolute_position()
            .with_translate(start_x + component_width * 0.25f, current_y)
            .with_custom_color(afterhours::Color{150, 200, 100, 255})
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_debug_name("button_3"));

    if (button_3_result) {
      button_3_clicks++;
      log_info("Button 3 clicked: {}", button_3_clicks);
    }

    current_y += spacing + 20.0f;

    div(context, mk(entity, 2),
        ComponentConfig{}
            .with_label("Button Group (Row)")
            .with_size(ComponentSize{pixels(component_width), pixels(30.0f)})
            .with_absolute_position()
            .with_translate(start_x, current_y)
            .with_custom_color(afterhours::Color{30, 30, 30, 200})
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_debug_name("section_label_2"));

    current_y += 40.0f;

    std::vector<std::string> button_group_labels = {"Option A", "Option B", "Option C"};

    auto button_group_result = button_group(
        context, mk(entity, 20), button_group_labels,
        ComponentConfig{}
            .with_size(ComponentSize{pixels(component_width), pixels(component_height)})
            .with_absolute_position()
            .with_translate(start_x, current_y)
            .with_flex_direction(FlexDirection::Row)
            .with_debug_name("button_group_row"));

    if (button_group_result) {
      button_group_selected = button_group_result.as<int>();
      button_group_clicks++;
    }

    current_y += spacing + 20.0f;

    std::string status_text = "Button Group Selected: ";
    if (button_group_selected >= 0) {
      status_text += button_group_labels[button_group_selected];
    } else {
      status_text += "None";
    }
    status_text += " (Total clicks: " + std::to_string(button_group_clicks) + ")";

    div(context, mk(entity, 200),
        ComponentConfig{}
            .with_label(status_text)
            .with_size(ComponentSize{pixels(component_width), pixels(30.0f)})
            .with_absolute_position()
            .with_translate(start_x, current_y)
            .with_custom_color(afterhours::Color{50, 50, 50, 200})
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_debug_name("status"));
  }
};

REGISTER_EXAMPLE_SCREEN(buttons, "Button test screen with different button types",
                        ExampleButtons)

