#pragma once

#include "../external.h"
#include "../input_mapping.h"
#include "ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <bitset>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ExampleCheckboxes : afterhours::System<UIContext<InputAction>> {
  bool checkbox_1 = false;
  bool checkbox_2 = true;
  bool checkbox_3 = false;
  bool checkbox_no_label_1 = false;
  bool checkbox_no_label_2 = true;
  std::bitset<4> checkbox_group_values;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    int screen_width = Settings::get().get_screen_width();
    int screen_height = Settings::get().get_screen_height();

    float start_y = screen_height * 0.15f;
    float spacing = 60.0f;
    float component_width = 300.0f;
    float component_height = 50.0f;
    float start_x = (screen_width - component_width) / 2.0f;

    div(context, mk(entity, 100),
        ComponentConfig{}
            .with_label("Screen: checkboxes - Checkbox Test Screen")
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
            .with_label("Individual Checkboxes")
            .with_size(ComponentSize{pixels(component_width), pixels(30.0f)})
            .with_absolute_position()
            .with_translate(start_x, current_y)
            .with_custom_color(afterhours::Color{30, 30, 30, 200})
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_debug_name("section_label_1"));

    current_y += 40.0f;

    checkbox(context, mk(entity, 10), checkbox_1,
             ComponentConfig{}
                 .with_label("Unchecked by default")
                 .with_size(ComponentSize{pixels(component_width),
                                          pixels(component_height)})
                 .with_absolute_position()
                 .with_translate(start_x, current_y)
                 .with_debug_name("checkbox_1"));

    current_y += spacing;

    checkbox(context, mk(entity, 11), checkbox_2,
             ComponentConfig{}
                 .with_label("Checked by default")
                 .with_size(ComponentSize{pixels(component_width),
                                          pixels(component_height)})
                 .with_absolute_position()
                 .with_translate(start_x, current_y)
                 .with_debug_name("checkbox_2"));

    current_y += spacing;

    checkbox(context, mk(entity, 12), checkbox_3,
             ComponentConfig{}
                 .with_label("Another checkbox")
                 .with_size(ComponentSize{pixels(component_width),
                                          pixels(component_height)})
                 .with_absolute_position()
                 .with_translate(start_x, current_y)
                 .with_debug_name("checkbox_3"));

    current_y += spacing + 20.0f;

    div(context, mk(entity, 2),
        ComponentConfig{}
            .with_label("Checkboxes Without Labels")
            .with_size(ComponentSize{pixels(component_width), pixels(30.0f)})
            .with_absolute_position()
            .with_translate(start_x, current_y)
            .with_custom_color(afterhours::Color{30, 30, 30, 200})
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_debug_name("section_label_2"));

    current_y += 40.0f;

    div(context, mk(entity, 20),
        ComponentConfig{}
            .with_label("Checkbox (no label) - Unchecked")
            .with_size(
                ComponentSize{pixels(component_width * 0.7f), pixels(30.0f)})
            .with_absolute_position()
            .with_translate(start_x, current_y)
            .with_custom_color(afterhours::Color{30, 30, 30, 150})
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_debug_name("label_for_no_label_1"));

    checkbox_no_label(
        context, mk(entity, 30), checkbox_no_label_1,
        ComponentConfig{}
            .with_size(ComponentSize{pixels(component_width * 0.25f),
                                     pixels(component_height)})
            .with_absolute_position()
            .with_translate(start_x + component_width * 0.75f, current_y)
            .with_debug_name("checkbox_no_label_1"));

    current_y += spacing;

    div(context, mk(entity, 21),
        ComponentConfig{}
            .with_label("Checkbox (no label) - Checked")
            .with_size(
                ComponentSize{pixels(component_width * 0.7f), pixels(30.0f)})
            .with_absolute_position()
            .with_translate(start_x, current_y)
            .with_custom_color(afterhours::Color{30, 30, 30, 150})
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_debug_name("label_for_no_label_2"));

    checkbox_no_label(
        context, mk(entity, 31), checkbox_no_label_2,
        ComponentConfig{}
            .with_size(ComponentSize{pixels(component_width * 0.25f),
                                     pixels(component_height)})
            .with_absolute_position()
            .with_translate(start_x + component_width * 0.75f, current_y)
            .with_debug_name("checkbox_no_label_2"));

    current_y += spacing + 20.0f;

    div(context, mk(entity, 3),
        ComponentConfig{}
            .with_label("Checkbox Group")
            .with_size(ComponentSize{pixels(component_width), pixels(30.0f)})
            .with_absolute_position()
            .with_translate(start_x, current_y)
            .with_custom_color(afterhours::Color{30, 30, 30, 200})
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_debug_name("section_label_3"));

    current_y += 40.0f;

    std::array<std::string_view, 4> group_labels = {"Option A", "Option B",
                                                    "Option C", "Option D"};

    checkbox_group(
        context, mk(entity, 40), checkbox_group_values, group_labels,
        std::make_pair(1, 3),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(component_width), pixels(200.0f)})
            .with_absolute_position()
            .with_translate(start_x, current_y)
            .with_flex_direction(FlexDirection::Column)
            .with_debug_name("checkbox_group"));

    current_y += 220.0f;

    std::string group_status =
        "Selected: " + std::to_string(checkbox_group_values.count()) +
        "/4 (min: 1, max: 3)";
    div(context, mk(entity, 200),
        ComponentConfig{}
            .with_label(group_status)
            .with_size(ComponentSize{pixels(component_width), pixels(30.0f)})
            .with_absolute_position()
            .with_translate(start_x, current_y)
            .with_custom_color(afterhours::Color{50, 50, 50, 200})
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_debug_name("group_status"));
  }
};

REGISTER_EXAMPLE_SCREEN(checkboxes,
                        "Checkbox test screen with all checkbox variants",
                        ExampleCheckboxes)
