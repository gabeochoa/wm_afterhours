#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ExampleBordersScreen : ScreenSystem<UIContext<InputAction>> {
  // Modern blueprint/technical drawing aesthetic
  afterhours::Color bg_deep{24, 32, 48, 255};          // Deep blue-gray
  afterhours::Color panel_dark{32, 42, 62, 255};       // Panel blue
  afterhours::Color grid_line{45, 58, 85, 255};        // Grid lines
  afterhours::Color accent_blue{80, 145, 220, 255};    // Blueprint blue
  afterhours::Color accent_cyan{70, 190, 200, 255};    // Cyan highlight
  afterhours::Color accent_orange{235, 145, 70, 255};  // Orange accent
  afterhours::Color text_light{220, 225, 235, 255};    // Light text
  afterhours::Color text_muted{120, 135, 160, 255};    // Muted text
  afterhours::Color sample_white{250, 250, 252, 255};  // Sample card white

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    Theme theme;
    theme.font = text_light;
    theme.darkfont = bg_deep;
    theme.font_muted = text_muted;
    theme.background = bg_deep;
    theme.surface = panel_dark;
    theme.primary = accent_blue;
    theme.secondary = accent_cyan;
    theme.accent = accent_orange;
    theme.error = afterhours::Color{200, 80, 80, 255};
    theme.roundness = 0.1f;
    context.theme = theme;

    int screen_width = Settings::get().get_screen_width();
    int screen_height = Settings::get().get_screen_height();

    // Background
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_width), pixels(screen_height)})
            .with_custom_background(bg_deep)
            .with_debug_name("bg"));

    // Subtle grid pattern
    for (int i = 0; i < 20; i++) {
      // Vertical lines
      div(context, mk(entity, 200 + i),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(1), pixels(screen_height)})
              .with_absolute_position()
              .with_translate((float)(i * 65 + 30), 0)
              .with_custom_background(grid_line)
              .with_debug_name("vgrid_" + std::to_string(i)));
    }
    for (int i = 0; i < 12; i++) {
      // Horizontal lines
      div(context, mk(entity, 230 + i),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(screen_width), pixels(1)})
              .with_absolute_position()
              .with_translate(0, (float)(i * 55 + 40))
              .with_custom_background(grid_line)
              .with_debug_name("hgrid_" + std::to_string(i)));
    }

    float start_x = 45.0f;
    float start_y = 55.0f;
    float box_size = 95.0f;
    float gap = 22.0f;

    // Title
    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_label("BORDER STYLES")
            .with_size(ComponentSize{pixels(screen_width - 90.0f), pixels(38)})
            .with_absolute_position()
            .with_translate(start_x, 8.0f)
            .with_custom_background(accent_blue)
            .with_font("EqProRounded", 24.0f)
            .with_custom_text_color(bg_deep)
            .with_padding(Spacing::xs)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("title"));

    // Row 1: Basic borders with labels
    struct BorderDemo {
      std::string label;
      afterhours::Color border_color;
      float border_width;
      afterhours::Color bg_color;
      bool rounded;
    };

    BorderDemo row1[] = {
        {"Thin", afterhours::Color{100, 100, 100, 255}, 1.0f, sample_white, false},
        {"Medium", afterhours::Color{60, 60, 60, 255}, 3.0f, sample_white, false},
        {"Thick", afterhours::Color{30, 30, 30, 255}, 6.0f, sample_white, false},
        {"Blue", accent_blue, 3.0f, sample_white, false},
        {"Cyan", accent_cyan, 3.0f, sample_white, false},
        {"Orange", accent_orange, 3.0f, sample_white, false},
    };

    for (int i = 0; i < 6; i++) {
      float x = start_x + i * (box_size + gap);
      div(context, mk(entity, 10 + i),
          ComponentConfig{}
              .with_label(row1[i].label)
              .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
              .with_absolute_position()
              .with_translate(x, start_y)
              .with_custom_background(row1[i].bg_color)
              .with_border(row1[i].border_color, row1[i].border_width)
              .with_font(UIComponent::DEFAULT_FONT, 15.0f)
              .with_custom_text_color(bg_deep)
              .with_alignment(TextAlignment::Center)
              .with_debug_name("border_" + std::to_string(i)));
    }

    // Row label
    div(context, mk(entity, 19),
        ComponentConfig{}
            .with_label("Width & Color")
            .with_size(ComponentSize{pixels(120), pixels(20)})
            .with_absolute_position()
            .with_translate(start_x + 6 * (box_size + gap) + 10, start_y + 38)
            .with_font(UIComponent::DEFAULT_FONT, 13.0f)
            .with_custom_text_color(text_muted)
            .with_debug_name("row1_label"));

    // Row 2: Rounded corners and shadows
    float row2_y = start_y + box_size + gap + 5;

    div(context, mk(entity, 20),
        ComponentConfig{}
            .with_label("Rounded")
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_absolute_position()
            .with_translate(start_x, row2_y)
            .with_custom_background(sample_white)
            .with_border(accent_blue, 2.0f)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.2f)
            .with_font(UIComponent::DEFAULT_FONT, 15.0f)
            .with_custom_text_color(bg_deep)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("rounded"));

    div(context, mk(entity, 21),
        ComponentConfig{}
            .with_label("Pill")
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_absolute_position()
            .with_translate(start_x + box_size + gap, row2_y)
            .with_custom_background(sample_white)
            .with_border(accent_cyan, 2.0f)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.5f)
            .with_font(UIComponent::DEFAULT_FONT, 15.0f)
            .with_custom_text_color(bg_deep)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("pill"));

    div(context, mk(entity, 22),
        ComponentConfig{}
            .with_label("Top Only")
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_absolute_position()
            .with_translate(start_x + 2 * (box_size + gap), row2_y)
            .with_custom_background(sample_white)
            .with_border(accent_blue, 2.0f)
            .with_rounded_corners(std::bitset<4>(0b1100))
            .with_roundness(0.25f)
            .with_font(UIComponent::DEFAULT_FONT, 15.0f)
            .with_custom_text_color(bg_deep)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("top_only"));

    div(context, mk(entity, 23),
        ComponentConfig{}
            .with_label("Diagonal")
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_absolute_position()
            .with_translate(start_x + 3 * (box_size + gap), row2_y)
            .with_custom_background(sample_white)
            .with_border(accent_orange, 2.0f)
            .with_rounded_corners(std::bitset<4>(0b1001))
            .with_roundness(0.3f)
            .with_font(UIComponent::DEFAULT_FONT, 15.0f)
            .with_custom_text_color(bg_deep)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("diagonal"));

    div(context, mk(entity, 24),
        ComponentConfig{}
            .with_label("Hard Shadow")
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_absolute_position()
            .with_translate(start_x + 4 * (box_size + gap), row2_y)
            .with_custom_background(sample_white)
            .with_border(afterhours::Color{80, 80, 80, 255}, 2.0f)
            .with_hard_shadow(5.0f, 5.0f, afterhours::Color{0, 0, 0, 120})
            .with_font(UIComponent::DEFAULT_FONT, 15.0f)
            .with_custom_text_color(bg_deep)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("hard_shadow"));

    div(context, mk(entity, 25),
        ComponentConfig{}
            .with_label("Soft Shadow")
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_absolute_position()
            .with_translate(start_x + 5 * (box_size + gap), row2_y)
            .with_custom_background(sample_white)
            .with_border(afterhours::Color{100, 100, 100, 255}, 1.0f)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_soft_shadow(4.0f, 6.0f, 14.0f, afterhours::Color{0, 0, 0, 80})
            .with_font(UIComponent::DEFAULT_FONT, 15.0f)
            .with_custom_text_color(bg_deep)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("soft_shadow"));

    // Row label
    div(context, mk(entity, 29),
        ComponentConfig{}
            .with_label("Corners & Shadows")
            .with_size(ComponentSize{pixels(140), pixels(20)})
            .with_absolute_position()
            .with_translate(start_x + 6 * (box_size + gap) + 10, row2_y + 38)
            .with_font(UIComponent::DEFAULT_FONT, 13.0f)
            .with_custom_text_color(text_muted)
            .with_debug_name("row2_label"));

    // Row 3: Themed examples
    float row3_y = row2_y + box_size + gap + 5;

    div(context, mk(entity, 30),
        ComponentConfig{}
            .with_label("Dark Mode")
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_absolute_position()
            .with_translate(start_x, row3_y)
            .with_custom_background(bg_deep)
            .with_border(accent_cyan, 2.0f)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_font(UIComponent::DEFAULT_FONT, 15.0f)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("dark_mode"));

    div(context, mk(entity, 31),
        ComponentConfig{}
            .with_label("Neon Glow")
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_absolute_position()
            .with_translate(start_x + box_size + gap, row3_y)
            .with_custom_background(afterhours::Color{15, 20, 30, 255})
            .with_border(afterhours::Color{0, 255, 200, 255}, 3.0f)
            .with_soft_shadow(0, 0, 12.0f, afterhours::Color{0, 255, 200, 60})
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_font(UIComponent::DEFAULT_FONT, 15.0f)
            .with_custom_text_color(afterhours::Color{0, 255, 200, 255})
            .with_alignment(TextAlignment::Center)
            .with_debug_name("neon_glow"));

    div(context, mk(entity, 32),
        ComponentConfig{}
            .with_label("Cozy")
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_absolute_position()
            .with_translate(start_x + 2 * (box_size + gap), row3_y)
            .with_custom_background(afterhours::Color{255, 250, 242, 255})
            .with_border(afterhours::Color{160, 130, 100, 255}, 3.0f)
            .with_soft_shadow(3.0f, 4.0f, 10.0f, afterhours::Color{80, 60, 40, 50})
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.15f)
            .with_font("Gaegu-Bold", 18.0f)
            .with_custom_text_color(afterhours::Color{80, 60, 45, 255})
            .with_alignment(TextAlignment::Center)
            .with_debug_name("cozy"));

    div(context, mk(entity, 33),
        ComponentConfig{}
            .with_label("Danger")
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_absolute_position()
            .with_translate(start_x + 3 * (box_size + gap), row3_y)
            .with_custom_background(afterhours::Color{60, 20, 20, 255})
            .with_border(afterhours::Color{200, 60, 60, 255}, 3.0f)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_font(UIComponent::DEFAULT_FONT, 15.0f)
            .with_custom_text_color(afterhours::Color{255, 180, 180, 255})
            .with_alignment(TextAlignment::Center)
            .with_debug_name("danger"));

    div(context, mk(entity, 34),
        ComponentConfig{}
            .with_label("Success")
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_absolute_position()
            .with_translate(start_x + 4 * (box_size + gap), row3_y)
            .with_custom_background(afterhours::Color{20, 55, 35, 255})
            .with_border(afterhours::Color{60, 180, 100, 255}, 3.0f)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_font(UIComponent::DEFAULT_FONT, 15.0f)
            .with_custom_text_color(afterhours::Color{180, 255, 200, 255})
            .with_alignment(TextAlignment::Center)
            .with_debug_name("success"));

    div(context, mk(entity, 35),
        ComponentConfig{}
            .with_label("Outline Only")
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_absolute_position()
            .with_translate(start_x + 5 * (box_size + gap), row3_y)
            .with_custom_background(afterhours::Color{0, 0, 0, 0})
            .with_border(text_light, 2.0f)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("outline_only"));

    // Row label
    div(context, mk(entity, 39),
        ComponentConfig{}
            .with_label("Themed Styles")
            .with_size(ComponentSize{pixels(120), pixels(20)})
            .with_absolute_position()
            .with_translate(start_x + 6 * (box_size + gap) + 10, row3_y + 38)
            .with_font(UIComponent::DEFAULT_FONT, 13.0f)
            .with_custom_text_color(text_muted)
            .with_debug_name("row3_label"));

    // Interactive button with border
    float row4_y = row3_y + box_size + gap + 15;

    auto btn_result = button(
        context, mk(entity, 40),
        ComponentConfig{}
            .with_label("Interactive Button")
            .with_size(ComponentSize{pixels(200.0f), pixels(52.0f)})
            .with_absolute_position()
            .with_translate(start_x, row4_y)
            .with_custom_background(accent_blue)
            .with_border(afterhours::colors::lighten(accent_blue, 1.3f), 3.0f)
            .with_soft_shadow(3.0f, 5.0f, 12.0f, afterhours::Color{80, 145, 220, 60})
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(0.4f)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_custom_text_color(bg_deep)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("bordered_button"));

    if (btn_result) {
      log_info("Bordered button clicked!");
    }

    // Nested panel demo
    div(context, mk(entity, 50),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(280.0f), pixels(110.0f)})
            .with_absolute_position()
            .with_translate(start_x + 230.0f, row4_y - 5)
            .with_custom_background(panel_dark)
            .with_border(accent_cyan, 2.0f)
            .with_padding(Spacing::sm)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_debug_name("nested_outer"));

    div(context, mk(entity, 51),
        ComponentConfig{}
            .with_label("Nested Panel")
            .with_size(ComponentSize{pixels(260), pixels(22)})
            .with_absolute_position()
            .with_translate(start_x + 240.0f, row4_y + 5)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_custom_text_color(accent_cyan)
            .with_debug_name("nested_title"));

    div(context, mk(entity, 52),
        ComponentConfig{}
            .with_label("Inner Card")
            .with_size(ComponentSize{pixels(120.0f), pixels(55.0f)})
            .with_absolute_position()
            .with_translate(start_x + 250.0f, row4_y + 35)
            .with_custom_background(sample_white)
            .with_border(accent_orange, 2.0f)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_custom_text_color(bg_deep)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("nested_child"));

    div(context, mk(entity, 53),
        ComponentConfig{}
            .with_label("Second")
            .with_size(ComponentSize{pixels(100.0f), pixels(55.0f)})
            .with_absolute_position()
            .with_translate(start_x + 385.0f, row4_y + 35)
            .with_custom_background(afterhours::Color{0, 0, 0, 0})
            .with_border(text_light, 2.0f)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("nested_child2"));
  }
};

REGISTER_EXAMPLE_SCREEN(example_borders, "Components",
                        "Border styling options", ExampleBordersScreen)
