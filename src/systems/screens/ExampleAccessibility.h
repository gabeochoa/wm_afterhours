#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Demonstrates the accessibility features:
// - Auto-contrast text color selection
// - Theme validation
// - WCAG compliance indicators
struct ExampleAccessibility : afterhours::System<UIContext<InputAction>> {
  bool validated_theme = false;
  bool theme_is_accessible = false;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    int screen_width = Settings::get().get_screen_width();
    (void)screen_width; // Used for layout calculations if needed

    // Validate theme once on first frame
    if (!validated_theme) {
      theme_is_accessible = ThemeDefaults::get().validate_theme_accessibility();
      validated_theme = true;
    }

    float start_y = 50.0f;
    float button_width = 280.0f;
    float button_height = 50.0f;
    float spacing = 60.0f;
    float center_x = (screen_width - button_width) / 2.0f;

    // Title
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_label("Accessibility Demo (Garnish)")
            .with_size(ComponentSize{pixels(400.0f), pixels(40.0f)})
            .with_absolute_position()
            .with_translate((screen_width - 400.0f) / 2.0f, start_y)
            .with_font(UIComponent::DEFAULT_FONT, 24.0f)
            .with_debug_name("title"));

    start_y += 60.0f;

    // Theme validation status
    std::string status_text = theme_is_accessible ? "Theme: WCAG AA Compliant"
                                                  : "Theme: Needs Improvement";
    afterhours::Color status_color = theme_is_accessible
                                         ? afterhours::Color{50, 150, 50, 255}
                                         : afterhours::Color{200, 100, 50, 255};

    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_label(status_text)
            .with_size(ComponentSize{pixels(300.0f), pixels(35.0f)})
            .with_absolute_position()
            .with_translate((screen_width - 300.0f) / 2.0f, start_y)
            .with_custom_color(status_color)
            .with_auto_text_color()
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_debug_name("theme_status"));

    start_y += 60.0f;

    // Section: Without auto-text-color (default behavior)
    div(context, mk(entity, 2),
        ComponentConfig{}
            .with_label("Without auto_text_color:")
            .with_size(ComponentSize{pixels(300.0f), pixels(25.0f)})
            .with_absolute_position()
            .with_translate((screen_width - 300.0f) / 2.0f, start_y)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_debug_name("section1_label"));

    start_y += 35.0f;

    // Light background - text may be hard to read
    button(context, mk(entity, 10),
           ComponentConfig{}
               .with_label("Light BG (hard to read)")
               .with_size(
                   ComponentSize{pixels(button_width), pixels(button_height)})
               .with_absolute_position()
               .with_translate(center_x, start_y)
               .with_custom_color(afterhours::Color{230, 230, 230, 255})
               .with_font(UIComponent::DEFAULT_FONT, 16.0f)
               .with_debug_name("light_no_auto"));

    start_y += spacing;

    // Dark background - works well with default light font
    button(context, mk(entity, 11),
           ComponentConfig{}
               .with_label("Dark BG (readable)")
               .with_size(
                   ComponentSize{pixels(button_width), pixels(button_height)})
               .with_absolute_position()
               .with_translate(center_x, start_y)
               .with_custom_color(afterhours::Color{40, 40, 60, 255})
               .with_font(UIComponent::DEFAULT_FONT, 16.0f)
               .with_debug_name("dark_no_auto"));

    start_y += spacing + 20.0f;

    // Section: With auto-text-color (Garnish feature)
    div(context, mk(entity, 3),
        ComponentConfig{}
            .with_label("With auto_text_color:")
            .with_size(ComponentSize{pixels(300.0f), pixels(25.0f)})
            .with_absolute_position()
            .with_translate((screen_width - 300.0f) / 2.0f, start_y)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_debug_name("section2_label"));

    start_y += 35.0f;

    // Light background - auto picks dark text
    button(context, mk(entity, 20),
           ComponentConfig{}
               .with_label("Light BG (auto dark text)")
               .with_size(
                   ComponentSize{pixels(button_width), pixels(button_height)})
               .with_absolute_position()
               .with_translate(center_x, start_y)
               .with_custom_color(afterhours::Color{230, 230, 230, 255})
               .with_auto_text_color()
               .with_font(UIComponent::DEFAULT_FONT, 16.0f)
               .with_debug_name("light_auto"));

    start_y += spacing;

    // Dark background - auto picks light text
    button(context, mk(entity, 21),
           ComponentConfig{}
               .with_label("Dark BG (auto light text)")
               .with_size(
                   ComponentSize{pixels(button_width), pixels(button_height)})
               .with_absolute_position()
               .with_translate(center_x, start_y)
               .with_custom_color(afterhours::Color{40, 40, 60, 255})
               .with_auto_text_color()
               .with_font(UIComponent::DEFAULT_FONT, 16.0f)
               .with_debug_name("dark_auto"));

    start_y += spacing;

    // Various colors to showcase
    afterhours::Color colors[] = {
        {255, 200, 100, 255}, // Yellow-ish
        {100, 180, 255, 255}, // Light blue
        {255, 100, 150, 255}, // Pink
        {150, 255, 150, 255}, // Light green
    };

    for (int i = 0; i < 4; i++) {
      button(
          context, mk(entity, 30 + i),
          ComponentConfig{}
              .with_label("Auto Contrast")
              .with_size(ComponentSize{pixels(130.0f), pixels(button_height)})
              .with_absolute_position()
              .with_translate(center_x - 150.0f + i * 145.0f, start_y)
              .with_custom_color(colors[i])
              .with_auto_text_color()
              .with_font(UIComponent::DEFAULT_FONT, 14.0f)
              .with_debug_name("color_demo_" + std::to_string(i)));
    }
  }
};

REGISTER_EXAMPLE_SCREEN(accessibility, "System Demos",
                        "Demonstrates Garnish accessibility features",
                        ExampleAccessibility)
