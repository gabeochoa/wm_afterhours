#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ExampleTabbing : ScreenSystem<UIContext<InputAction>> {
  int focused_button = 0;
  std::vector<int> button_clicks = {0, 0, 0, 0};
  bool theme_initialized = false;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    // Set up theme with proper contrasting colors for auto_text_color
    if (!theme_initialized) {
      theme_initialized = true;
      Theme theme;
      theme.font = afterhours::Color{255, 255, 255, 255};
      theme.darkfont = afterhours::Color{30, 30, 30, 255};
      theme.background = afterhours::Color{40, 40, 50, 255};
      context.theme = theme;
    }

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
                        static_cast<unsigned char>(50 + i * 15), 80,
                        static_cast<unsigned char>(120 + i * 15), 255})  // Darker colors for better contrast
                     .with_auto_text_color(true)
                     .with_font(UIComponent::DEFAULT_FONT, 22.0f)
                     .with_debug_name("tab_button_" + std::to_string(i)));

      if (button_result) {
        button_clicks[i]++;
        log_info("Button {} clicked! Count: {}", i + 1, button_clicks[i]);
      }
    }

    float instruction_y = start_y - 40.0f;
    div(context, mk(entity, 10),
        ComponentConfig{}
            .with_label("Use TAB to navigate, ENTER to click")
            .with_size(
                ComponentSize{pixels(screen_width * 0.8f), pixels(30.0f)})
            .with_absolute_position()
            .with_translate((screen_width - screen_width * 0.8f) / 2.0f,
                            instruction_y)
            .with_custom_background(afterhours::Color{30, 30, 30, 200})
            .with_padding(Spacing::xs)
            .with_font(UIComponent::DEFAULT_FONT, 20.0f)
            .with_debug_name("instructions"));
  }
};

REGISTER_EXAMPLE_SCREEN(tabbing, "System Demos",
                        "Keyboard tabbing navigation example", ExampleTabbing)
