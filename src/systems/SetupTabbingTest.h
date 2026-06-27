#pragma once

#include "../external.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct SetupTabbingTest : afterhours::System<UIContext<InputAction>> {
  std::vector<int> button_clicks = {0, 0, 0, 0};

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    int screen_width = Settings::get().get_screen_width();
    int screen_height = Settings::get().get_screen_height();

    float button_width = 180.0f;
    float button_height = 50.0f;
    float button_spacing = 20.0f;
    float start_y =
        (screen_height - (4 * button_height + 3 * button_spacing)) / 2.0f;
    float start_x = (screen_width - button_width) / 2.0f;

    for (int i = 0; i < 4; i++) {
      float button_y = start_y + i * (button_height + button_spacing);
      std::string label = "Button " + std::to_string(i + 1);
      if (button_clicks[i] > 0) {
        label += " (" + std::to_string(button_clicks[i]) + ")";
      }

      auto button_result =
          button(context, mk(entity, i),
                 ComponentConfig{}
                     .with_label(label)
                     .with_size(ComponentSize{pixels(button_width),
                                              pixels(button_height)})
                     .with_absolute_position()
                     .with_translate(start_x, button_y)
                     .with_custom_background(afterhours::Color{
                         static_cast<unsigned char>(80 + i * 20), 100,
                         static_cast<unsigned char>(150 + i * 20), 255})
                     .with_font(UIComponent::DEFAULT_FONT, 20.0f)
                     .with_debug_name("tab_button_" + std::to_string(i)));

      if (button_result) {
        button_clicks[i]++;
        log_info("Button {} clicked! Count: {}", i + 1, button_clicks[i]);
      }
    }
  }
};
