#pragma once

#include "../external.h"
#include "../input_mapping.h"
#include "ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <bitset>
#include <string>
#include <vector>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ExampleComponentShowcase : afterhours::System<UIContext<InputAction>> {
  size_t dropdown_index = 0;
  size_t pagination_index = 0;
  size_t nav_bar_index = 0;
  bool checkbox_1 = false;
  bool checkbox_2 = true;
  float slider_1 = 0.5f;
  float slider_2 = 0.3f;
  int button_clicks = 0;
  std::bitset<3> checkbox_group_values;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    int screen_width = Settings::get().get_screen_width();
    int screen_height = Settings::get().get_screen_height();

    float start_y = screen_height * 0.05f;
    float spacing = 70.0f;
    float component_width = 350.0f;
    float component_height = 50.0f;
    float left_col_x = screen_width * 0.15f;
    float right_col_x = screen_width * 0.55f;

    div(context, mk(entity, 100),
        ComponentConfig{}
            .with_label("Screen: showcase - Complete Component Showcase")
            .with_size(
                ComponentSize{pixels(screen_width * 0.7f), pixels(40.0f)})
            .with_absolute_position()
            .with_translate((screen_width - screen_width * 0.7f) / 2.0f,
                            start_y - 50.0f)
            .with_custom_color(afterhours::Color{40, 40, 40, 255})
            .with_font(UIComponent::DEFAULT_FONT, 28.0f)
            .with_debug_name("title"));

    float current_y = start_y;

    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_label("Dropdown")
            .with_size(ComponentSize{pixels(component_width), pixels(25.0f)})
            .with_absolute_position()
            .with_translate(left_col_x, current_y)
            .with_custom_color(afterhours::Color{30, 30, 30, 200})
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_debug_name("label_dropdown"));

    current_y += 30.0f;
    std::vector<std::string> dropdown_options = {"Option 1", "Option 2",
                                                 "Option 3"};
    dropdown(context, mk(entity, 10), dropdown_options, dropdown_index,
             ComponentConfig{}
                 .with_size(ComponentSize{pixels(component_width),
                                          pixels(component_height)})
                 .with_absolute_position()
                 .with_translate(left_col_x, current_y)
                 .with_debug_name("showcase_dropdown"));

    current_y += spacing;

    div(context, mk(entity, 2),
        ComponentConfig{}
            .with_label("Pagination")
            .with_size(ComponentSize{pixels(component_width), pixels(25.0f)})
            .with_absolute_position()
            .with_translate(left_col_x, current_y)
            .with_custom_color(afterhours::Color{30, 30, 30, 200})
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_debug_name("label_pagination"));

    current_y += 30.0f;
    std::vector<std::string> pagination_options = {"Page 1", "Page 2", "Page 3",
                                                   "Page 4"};
    pagination(context, mk(entity, 11), pagination_options, pagination_index,
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(component_width),
                                            pixels(component_height)})
                   .with_absolute_position()
                   .with_translate(left_col_x, current_y)
                   .with_debug_name("showcase_pagination"));

    current_y += spacing;

    div(context, mk(entity, 3),
        ComponentConfig{}
            .with_label("Navigation Bar")
            .with_size(ComponentSize{pixels(component_width), pixels(25.0f)})
            .with_absolute_position()
            .with_translate(left_col_x, current_y)
            .with_custom_color(afterhours::Color{30, 30, 30, 200})
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_debug_name("label_navbar"));

    current_y += 30.0f;
    std::vector<std::string> nav_options = {"Home", "About", "Contact"};
    navigation_bar(context, mk(entity, 12), nav_options, nav_bar_index,
                   ComponentConfig{}
                       .with_size(ComponentSize{pixels(component_width),
                                                pixels(component_height)})
                       .with_absolute_position()
                       .with_translate(left_col_x, current_y)
                       .with_debug_name("showcase_navbar"));

    current_y = start_y;

    div(context, mk(entity, 4),
        ComponentConfig{}
            .with_label("Checkboxes")
            .with_size(ComponentSize{pixels(component_width), pixels(25.0f)})
            .with_absolute_position()
            .with_translate(right_col_x, current_y)
            .with_custom_color(afterhours::Color{30, 30, 30, 200})
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_debug_name("label_checkboxes"));

    current_y += 30.0f;
    checkbox(context, mk(entity, 13), checkbox_1,
             ComponentConfig{}
                 .with_label("Checkbox 1")
                 .with_size(ComponentSize{pixels(component_width),
                                          pixels(component_height)})
                 .with_absolute_position()
                 .with_translate(right_col_x, current_y)
                 .with_debug_name("showcase_checkbox_1"));

    current_y += spacing * 0.7f;
    checkbox(context, mk(entity, 14), checkbox_2,
             ComponentConfig{}
                 .with_label("Checkbox 2 (checked)")
                 .with_size(ComponentSize{pixels(component_width),
                                          pixels(component_height)})
                 .with_absolute_position()
                 .with_translate(right_col_x, current_y)
                 .with_debug_name("showcase_checkbox_2"));

    current_y += spacing * 0.7f;
    std::array<std::string_view, 3> group_labels = {"A", "B", "C"};
    checkbox_group(
        context, mk(entity, 15), checkbox_group_values, group_labels,
        std::make_pair(0, 3),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(component_width), pixels(150.0f)})
            .with_absolute_position()
            .with_translate(right_col_x, current_y)
            .with_flex_direction(FlexDirection::Column)
            .with_debug_name("showcase_checkbox_group"));

    current_y += 170.0f;

    div(context, mk(entity, 5),
        ComponentConfig{}
            .with_label("Sliders")
            .with_size(ComponentSize{pixels(component_width), pixels(25.0f)})
            .with_absolute_position()
            .with_translate(right_col_x, current_y)
            .with_custom_color(afterhours::Color{30, 30, 30, 200})
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_debug_name("label_sliders"));

    current_y += 30.0f;
    slider(context, mk(entity, 16), slider_1,
           ComponentConfig{}
               .with_label("Slider 1")
               .with_size(ComponentSize{pixels(component_width),
                                        pixels(component_height)})
               .with_absolute_position()
               .with_translate(right_col_x, current_y)
               .with_debug_name("showcase_slider_1"),
           SliderHandleValueLabelPosition::WithLabel);

    current_y += spacing;
    slider(context, mk(entity, 17), slider_2,
           ComponentConfig{}
               .with_label("Slider 2")
               .with_size(ComponentSize{pixels(component_width),
                                        pixels(component_height)})
               .with_absolute_position()
               .with_translate(right_col_x, current_y)
               .with_debug_name("showcase_slider_2"),
           SliderHandleValueLabelPosition::OnHandle);

    current_y += spacing;

    div(context, mk(entity, 6),
        ComponentConfig{}
            .with_label("Button")
            .with_size(ComponentSize{pixels(component_width), pixels(25.0f)})
            .with_absolute_position()
            .with_translate(right_col_x, current_y)
            .with_custom_color(afterhours::Color{30, 30, 30, 200})
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_debug_name("label_button"));

    current_y += 30.0f;
    auto button_result = button(
        context, mk(entity, 18),
        ComponentConfig{}
            .with_label("Click Me (" + std::to_string(button_clicks) + ")")
            .with_size(ComponentSize{pixels(component_width),
                                     pixels(component_height)})
            .with_absolute_position()
            .with_translate(right_col_x, current_y)
            .with_custom_color(afterhours::Color{100, 150, 200, 255})
            .with_font(UIComponent::DEFAULT_FONT, 20.0f)
            .with_debug_name("showcase_button"));

    if (button_result) {
      button_clicks++;
    }

    current_y += spacing + 20.0f;

    std::string status =
        "Dropdown: " + std::to_string(dropdown_index) +
        " | Pagination: " + std::to_string(pagination_index) +
        " | Nav: " + std::to_string(nav_bar_index) +
        " | Checkboxes: " + std::to_string(checkbox_group_values.count()) +
        " | Button: " + std::to_string(button_clicks);

    div(context, mk(entity, 200),
        ComponentConfig{}
            .with_label(status)
            .with_size(
                ComponentSize{pixels(screen_width * 0.8f), pixels(30.0f)})
            .with_absolute_position()
            .with_translate((screen_width - screen_width * 0.8f) / 2.0f,
                            current_y)
            .with_custom_color(afterhours::Color{50, 50, 50, 200})
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_debug_name("status"));
  }
};

REGISTER_EXAMPLE_SCREEN(showcase,
                        "Complete component showcase with all UI components",
                        ExampleComponentShowcase)
