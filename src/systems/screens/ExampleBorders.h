#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ExampleBordersScreen : ScreenSystem<UIContext<InputAction>> {
  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    int screen_width = Settings::get().get_screen_width();
    int screen_height = Settings::get().get_screen_height();

    // Screen background
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels((float)screen_width),
                                     pixels((float)screen_height)})
            .with_custom_background(afterhours::Color{240, 240, 240, 255})
            .with_debug_name("border_test_bg"));

    float start_x = 40.0f;
    float start_y = 80.0f;
    float box_size = 100.0f;
    float gap = 30.0f;

    // Title
    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_label("Border Test Screen")
            .with_size(ComponentSize{pixels((float)screen_width - 80.0f),
                                     pixels(50.0f)})
            .with_absolute_position()
            .with_translate(start_x, 20.0f)
            .with_custom_background(afterhours::Color{60, 60, 80, 255})
            .with_font(UIComponent::DEFAULT_FONT, 28.0f)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("title"));

    // Row 1: Basic borders
    // Test 1: Basic 2px black border
    div(context, mk(entity, 10),
        ComponentConfig{}
            .with_label("Basic")
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_absolute_position()
            .with_translate(start_x, start_y)
            .with_custom_background(afterhours::Color{255, 255, 255, 255})
            .with_border(afterhours::Color{0, 0, 0, 255}, 2.0f)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("basic_border"));

    // Test 2: Thick 6px border
    div(context, mk(entity, 11),
        ComponentConfig{}
            .with_label("Thick")
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_absolute_position()
            .with_translate(start_x + box_size + gap, start_y)
            .with_custom_background(afterhours::Color{255, 255, 255, 255})
            .with_border(afterhours::Color{0, 0, 0, 255}, 6.0f)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("thick_border"));

    // Test 3: Red border
    div(context, mk(entity, 12),
        ComponentConfig{}
            .with_label("Red")
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_absolute_position()
            .with_translate(start_x + 2 * (box_size + gap), start_y)
            .with_custom_background(afterhours::Color{255, 240, 240, 255})
            .with_border(afterhours::Color{200, 60, 60, 255}, 3.0f)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("red_border"));

    // Test 4: Green border
    div(context, mk(entity, 13),
        ComponentConfig{}
            .with_label("Green")
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_absolute_position()
            .with_translate(start_x + 3 * (box_size + gap), start_y)
            .with_custom_background(afterhours::Color{240, 255, 240, 255})
            .with_border(afterhours::Color{60, 160, 60, 255}, 3.0f)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("green_border"));

    // Test 5: Blue border
    div(context, mk(entity, 14),
        ComponentConfig{}
            .with_label("Blue")
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_absolute_position()
            .with_translate(start_x + 4 * (box_size + gap), start_y)
            .with_custom_background(afterhours::Color{240, 240, 255, 255})
            .with_border(afterhours::Color{60, 60, 200, 255}, 3.0f)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("blue_border"));

    // Row 2: Border + rounded corners
    float row2_y = start_y + box_size + gap;

    // Test 6: Border + all rounded corners
    div(context, mk(entity, 20),
        ComponentConfig{}
            .with_label("Rounded")
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_absolute_position()
            .with_translate(start_x, row2_y)
            .with_custom_background(afterhours::Color{255, 255, 255, 255})
            .with_border(afterhours::Color{100, 100, 100, 255}, 2.0f)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("rounded_border"));

    // Test 7: Border + shadow
    div(context, mk(entity, 21),
        ComponentConfig{}
            .with_label("Shadow")
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_absolute_position()
            .with_translate(start_x + box_size + gap, row2_y)
            .with_custom_background(afterhours::Color{255, 255, 255, 255})
            .with_border(afterhours::Color{80, 80, 80, 255}, 2.0f)
            .with_hard_shadow(4.0f, 4.0f, afterhours::Color{0, 0, 0, 100})
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("shadow_border"));

    // Test 8: Transparent background + border only
    div(context, mk(entity, 22),
        ComponentConfig{}
            .with_label("Border Only")
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_absolute_position()
            .with_translate(start_x + 2 * (box_size + gap), row2_y)
            .with_custom_background(afterhours::Color{0, 0, 0, 0})
            .with_border(afterhours::Color{100, 60, 180, 255}, 3.0f)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("border_only"));

    // Test 9: Brown border (cozy cafe style)
    div(context, mk(entity, 23),
        ComponentConfig{}
            .with_label("Cozy")
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_absolute_position()
            .with_translate(start_x + 3 * (box_size + gap), row2_y)
            .with_custom_background(afterhours::Color{255, 250, 240, 255})
            .with_border(afterhours::Color{139, 119, 101, 255}, 3.0f)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_soft_shadow(3.0f, 4.0f, 8.0f, afterhours::Color{0, 0, 0, 60})
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("cozy_border"));

    // Row 3: Button with border
    float row3_y = row2_y + box_size + gap;

    auto btn_result = button(
        context, mk(entity, 30),
        ComponentConfig{}
            .with_label("Button with Border")
            .with_size(ComponentSize{pixels(180.0f), pixels(50.0f)})
            .with_absolute_position()
            .with_translate(start_x, row3_y)
            .with_custom_background(afterhours::Color{100, 150, 200, 255})
            .with_border(afterhours::Color{50, 80, 120, 255}, 3.0f)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("bordered_button"));

    if (btn_result) {
      log_info("Bordered button clicked!");
    }

    // Row 4: Nested borders
    float row4_y = row3_y + 70.0f;

    // Outer panel
    div(context, mk(entity, 40),
        ComponentConfig{}
            .with_label("Nested Panel")
            .with_size(ComponentSize{pixels(300.0f), pixels(150.0f)})
            .with_absolute_position()
            .with_translate(start_x, row4_y)
            .with_custom_background(afterhours::Color{230, 230, 240, 255})
            .with_border(afterhours::Color{100, 100, 120, 255}, 2.0f)
            .with_padding(Spacing::md)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_alignment(TextAlignment::Left)
            .with_debug_name("nested_outer"));

    // Inner child with border
    div(context, mk(entity, 41),
        ComponentConfig{}
            .with_label("Child Element")
            .with_size(ComponentSize{pixels(140.0f), pixels(60.0f)})
            .with_absolute_position()
            .with_translate(start_x + 20.0f, row4_y + 50.0f)
            .with_custom_background(afterhours::Color{255, 255, 255, 255})
            .with_border(afterhours::Color{80, 100, 160, 255}, 2.0f)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("nested_child"));
  }
};

REGISTER_EXAMPLE_SCREEN(example_borders, "Components",
                        "Border styling options", ExampleBordersScreen)

