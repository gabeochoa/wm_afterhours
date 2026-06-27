#pragma once

#include "../external.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct SetupSimpleButtonTest : afterhours::System<UIContext<InputAction>> {
  int button_click_count = 0;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    int screen_width = Settings::get().get_screen_width();
    int screen_height = Settings::get().get_screen_height();

    float button_width = 200.0f;
    float button_height = 60.0f;
    float button_x = (screen_width - button_width) / 2.0f;
    float button_y = (screen_height - button_height) / 2.0f;

    auto button_result = button(
        context, mk(entity, 0),
        ComponentConfig{}
            .with_label("Click Me")
            .with_size(
                ComponentSize{pixels(button_width), pixels(button_height)})
            .with_absolute_position()
            .with_translate(button_x, button_y)
            .with_custom_background(afterhours::Color{100, 100, 200, 255})
            .with_font(UIComponent::DEFAULT_FONT, 24.0f)
            .with_debug_name("test_button"));

    if (button_result) {
      button_click_count++;
      log_info("Button clicked! Count: {}", button_click_count);
    }

    float counter_y = button_y + button_height + 20.0f;
    float counter_height = 30.0f;
    std::string counter_text = "Clicks: " + std::to_string(button_click_count);

    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_label(counter_text)
            .with_size(
                ComponentSize{pixels(button_width), pixels(counter_height)})
            .with_absolute_position()
            .with_translate(button_x, counter_y)
            .with_custom_background(afterhours::Color{50, 50, 50, 200})
            .with_font(UIComponent::DEFAULT_FONT, 20.0f)
            .with_debug_name("click_counter"));
  }
};
