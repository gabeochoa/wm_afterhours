#pragma once

#include "../external.h"
#include "../input_mapping.h"
#include "ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <string>
#include <vector>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ExampleDropdowns : afterhours::System<UIContext<InputAction>> {
  size_t static_1_index = 0;
  size_t static_2_index = 0;
  size_t static_5_index = 0;
  size_t dynamic_index = 0;
  int dynamic_counter = 0;
  std::vector<std::string> dynamic_options;
  int button_under_1_clicks = 0;
  int button_under_2_clicks = 0;
  int button_under_5_clicks = 0;
  int button_under_dynamic_clicks = 0;

  ExampleDropdowns() { update_dynamic_options(); }

  void update_dynamic_options() {
    dynamic_options.clear();
    for (int i = 0; i < 3 + (dynamic_counter % 5); i++) {
      dynamic_options.push_back("Dynamic Option " + std::to_string(i + 1));
    }
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    int screen_width = Settings::get().get_screen_width();
    int screen_height = Settings::get().get_screen_height();

    float start_y = screen_height * 0.15f;
    float dropdown_width = 250.0f;
    float dropdown_height = 50.0f;
    float start_x = (screen_width - dropdown_width) / 2.0f;

    div(context, mk(entity, 100),
        ComponentConfig{}
            .with_label("Screen: dropdowns - Dropdown Test Screen")
            .with_size(
                ComponentSize{pixels(screen_width * 0.6f), pixels(40.0f)})
            .with_absolute_position()
            .with_translate((screen_width - screen_width * 0.6f) / 2.0f,
                            start_y - 60.0f)
            .with_custom_color(afterhours::Color{40, 40, 40, 255})
            .with_font(UIComponent::DEFAULT_FONT, 28.0f)
            .with_debug_name("title"));

    std::vector<std::string> static_1_options = {"Only Option"};
    std::vector<std::string> static_2_options = {"Option A", "Option B"};
    std::vector<std::string> static_5_options = {"First", "Second", "Third",
                                                 "Fourth", "Fifth"};

    float current_y = start_y;

    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_label("Static Dropdown (1 option)")
            .with_size(ComponentSize{pixels(dropdown_width), pixels(30.0f)})
            .with_absolute_position()
            .with_translate(start_x, current_y)
            .with_custom_color(afterhours::Color{30, 30, 30, 200})
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_debug_name("label_1"));

    current_y += 35.0f;
    dropdown(context, mk(entity, 10), static_1_options, static_1_index,
             ComponentConfig{}
                 .with_label("1 Option:")
                 .with_size(ComponentSize{pixels(dropdown_width),
                                          pixels(dropdown_height)})
                 .with_absolute_position()
                 .with_translate(start_x, current_y)
                 .with_debug_name("dropdown_1"));

    current_y += dropdown_height + 10.0f;
    auto button_1_result = button(
        context, mk(entity, 20),
        ComponentConfig{}
            .with_label("Button Under 1-Option Dropdown (Clicks: " +
                        std::to_string(button_under_1_clicks) + ")")
            .with_size(ComponentSize{pixels(dropdown_width), pixels(50.0f)})
            .with_absolute_position()
            .with_translate(start_x, current_y)
            .with_custom_color(afterhours::Color{150, 100, 100, 255})
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_debug_name("button_under_1"));

    if (button_1_result) {
      button_under_1_clicks++;
      log_info("Button under 1-option dropdown clicked: {}",
               button_under_1_clicks);
    }

    current_y += 80.0f;

    div(context, mk(entity, 2),
        ComponentConfig{}
            .with_label("Static Dropdown (2 options)")
            .with_size(ComponentSize{pixels(dropdown_width), pixels(30.0f)})
            .with_absolute_position()
            .with_translate(start_x, current_y)
            .with_custom_color(afterhours::Color{30, 30, 30, 200})
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_debug_name("label_2"));

    current_y += 35.0f;
    dropdown(context, mk(entity, 11), static_2_options, static_2_index,
             ComponentConfig{}
                 .with_label("2 Options:")
                 .with_size(ComponentSize{pixels(dropdown_width),
                                          pixels(dropdown_height)})
                 .with_absolute_position()
                 .with_translate(start_x, current_y)
                 .with_debug_name("dropdown_2"));

    current_y += dropdown_height + 10.0f;
    auto button_2_result = button(
        context, mk(entity, 21),
        ComponentConfig{}
            .with_label("Button Under 2-Option Dropdown (Clicks: " +
                        std::to_string(button_under_2_clicks) + ")")
            .with_size(ComponentSize{pixels(dropdown_width), pixels(50.0f)})
            .with_absolute_position()
            .with_translate(start_x, current_y)
            .with_custom_color(afterhours::Color{100, 150, 100, 255})
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_debug_name("button_under_2"));

    if (button_2_result) {
      button_under_2_clicks++;
      log_info("Button under 2-option dropdown clicked: {}",
               button_under_2_clicks);
    }

    current_y += 80.0f;

    div(context, mk(entity, 3),
        ComponentConfig{}
            .with_label("Static Dropdown (5 options)")
            .with_size(ComponentSize{pixels(dropdown_width), pixels(30.0f)})
            .with_absolute_position()
            .with_translate(start_x, current_y)
            .with_custom_color(afterhours::Color{30, 30, 30, 200})
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_debug_name("label_5"));

    current_y += 35.0f;
    dropdown(context, mk(entity, 12), static_5_options, static_5_index,
             ComponentConfig{}
                 .with_label("5 Options:")
                 .with_size(ComponentSize{pixels(dropdown_width),
                                          pixels(dropdown_height)})
                 .with_absolute_position()
                 .with_translate(start_x, current_y)
                 .with_debug_name("dropdown_5"));

    current_y += dropdown_height + 10.0f;
    auto button_5_result = button(
        context, mk(entity, 22),
        ComponentConfig{}
            .with_label("Button Under 5-Option Dropdown (Clicks: " +
                        std::to_string(button_under_5_clicks) + ")")
            .with_size(ComponentSize{pixels(dropdown_width), pixels(50.0f)})
            .with_absolute_position()
            .with_translate(start_x, current_y)
            .with_custom_color(afterhours::Color{100, 100, 150, 255})
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_debug_name("button_under_5"));

    if (button_5_result) {
      button_under_5_clicks++;
      log_info("Button under 5-option dropdown clicked: {}",
               button_under_5_clicks);
    }

    current_y += 80.0f;

    div(context, mk(entity, 4),
        ComponentConfig{}
            .with_label("Dynamic Dropdown (changes every click)")
            .with_size(ComponentSize{pixels(dropdown_width), pixels(30.0f)})
            .with_absolute_position()
            .with_translate(start_x, current_y)
            .with_custom_color(afterhours::Color{30, 30, 30, 200})
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_debug_name("label_dynamic"));

    current_y += 35.0f;
    auto dropdown_dynamic_result =
        dropdown(context, mk(entity, 13), dynamic_options, dynamic_index,
                 ComponentConfig{}
                     .with_label("Dynamic:")
                     .with_size(ComponentSize{pixels(dropdown_width),
                                              pixels(dropdown_height)})
                     .with_absolute_position()
                     .with_translate(start_x, current_y)
                     .with_debug_name("dropdown_dynamic"));

    if (dropdown_dynamic_result) {
      dynamic_counter++;
      update_dynamic_options();
      if (dynamic_index >= dynamic_options.size()) {
        dynamic_index = 0;
      }
    }

    current_y += dropdown_height + 10.0f;
    auto button_dynamic_result = button(
        context, mk(entity, 23),
        ComponentConfig{}
            .with_label("Button Under Dynamic Dropdown (Clicks: " +
                        std::to_string(button_under_dynamic_clicks) + ")")
            .with_size(ComponentSize{pixels(dropdown_width), pixels(50.0f)})
            .with_absolute_position()
            .with_translate(start_x, current_y)
            .with_custom_color(afterhours::Color{150, 150, 100, 255})
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_debug_name("button_under_dynamic"));

    if (button_dynamic_result) {
      button_under_dynamic_clicks++;
      log_info("Button under dynamic dropdown clicked: {}",
               button_under_dynamic_clicks);
    }

    current_y += 80.0f;
    div(context, mk(entity, 200),
        ComponentConfig{}
            .with_label("Instructions: Open dropdowns and try clicking buttons "
                        "underneath to test click-through prevention")
            .with_size(
                ComponentSize{pixels(screen_width * 0.8f), pixels(40.0f)})
            .with_absolute_position()
            .with_translate((screen_width - screen_width * 0.8f) / 2.0f,
                            current_y)
            .with_custom_color(afterhours::Color{50, 50, 50, 200})
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_debug_name("instructions"));
  }
};

REGISTER_EXAMPLE_SCREEN(dropdowns,
                        "Dropdown test screen with static and dynamic options",
                        ExampleDropdowns)
