#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ExampleBordersScreen : ScreenSystem<UIContext<InputAction>> {
  // Modern blueprint/technical drawing aesthetic
  afterhours::Color bg_deep{24, 32, 48, 255};         // Deep blue-gray
  afterhours::Color panel_dark{32, 42, 62, 255};      // Panel blue
  afterhours::Color grid_line{45, 58, 85, 255};       // Grid lines
  afterhours::Color accent_blue{80, 145, 220, 255};   // Blueprint blue
  afterhours::Color accent_cyan{70, 190, 200, 255};   // Cyan highlight
  afterhours::Color accent_orange{235, 145, 70, 255}; // Orange accent
  afterhours::Color text_light{220, 225, 235, 255};   // Light text
  afterhours::Color text_muted{120, 135, 160, 255};   // Muted text
  afterhours::Color sample_white{250, 250, 252, 255}; // Sample card white

  // Configurable typography sizes
  float section_header_font_size =
      18.0f; // Category labels (e.g., "Width & Color")

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
    theme.roundness = 0.08f;
    context.theme = theme;

    int screen_width = Settings::get().get_screen_width();
    int screen_height = Settings::get().get_screen_height();

    // Background - covers entire screen
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels((float)screen_width),
                                     pixels((float)screen_height)})
            .with_custom_background(bg_deep)
            .with_debug_name("bg"));

    float sw = static_cast<float>(screen_width);
    float label_reserve = 160.0f;
    float avail = sw - label_reserve - 30.0f;
    float gap = std::min(20.0f, avail * 0.02f);
    float box_size = std::min(100.0f, (avail - 5.0f * gap) / 6.0f);
    float content_width = 6.0f * box_size + 5.0f * gap;
    float start_x = label_reserve + (sw - label_reserve - content_width) / 2.0f;
    float start_y = 85.0f;

    // Title - centered above content
    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_label("BORDER STYLES")
            .with_size(ComponentSize{pixels(content_width), pixels(42)})
            .with_absolute_position(start_x, 12.0f)
            .with_custom_background(accent_blue)
            .with_font("EqProRounded", h720(26.0f))
            .with_custom_text_color(bg_deep)
            .with_padding(Spacing::xs)
            .with_alignment(TextAlignment::Center));

    // Legend - explanation of border types
    div(context, mk(entity, 2),
        ComponentConfig{}
            .with_label("Width & Colors | Corners & Shadows | Themed | "
                        "Interactive | Per-Side | Dotted")
            .with_size(ComponentSize{pixels(content_width), pixels(22)})
            .with_absolute_position(start_x, 56.0f)
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Center));

    // Row 1: Basic borders with labels
    struct BorderDemo {
      std::string label;
      afterhours::Color border_color;
      float border_width;
      afterhours::Color bg_color;
      bool rounded;
    };

    BorderDemo row1[] = {
        {"Thin", afterhours::Color{100, 100, 100, 255}, 1.0f, sample_white,
         false},
        {"Medium", afterhours::Color{60, 60, 60, 255}, 3.0f, sample_white,
         false},
        {"Thick", afterhours::Color{30, 30, 30, 255}, 6.0f, sample_white,
         false},
        {"Blue", accent_blue, 3.0f, sample_white, false},
        {"Cyan", accent_cyan, 3.0f, sample_white, false},
        {"Orange", accent_orange, 3.0f, sample_white, false},
    };

    for (int i = 0; i < 6; i++) {
      float x = start_x + i * (box_size + gap);
      button(context, mk(entity, 10 + i),
             ComponentConfig{}
                 .with_label(row1[i].label)
                 .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
                 .with_absolute_position(x, start_y)
                 .with_custom_background(row1[i].bg_color)
                 .with_border(row1[i].border_color, row1[i].border_width)
                 .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
                 .with_custom_text_color(bg_deep)
                 .with_alignment(TextAlignment::Center)
                 .with_debug_name("border_" + std::to_string(i)));
    }

    // Row 1 label - positioned as a header on the left
    // Ensure labels stay within screen bounds
    float row_label_width = 140.0f;
    float min_left_margin = 15.0f;
    float row_label_x =
        std::max(min_left_margin, start_x - row_label_width - 15.0f);

    div(context, mk(entity, 19),
        ComponentConfig{}
            .with_label("Width & Color")
            .with_size(ComponentSize{pixels(row_label_width), pixels(box_size)})
            .with_absolute_position(row_label_x, start_y)
            .with_font(UIComponent::DEFAULT_FONT,
                       h720(section_header_font_size))
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Right));

    // Row 2: Rounded corners and shadows
    float row2_y = start_y + box_size + gap;

    div(context, mk(entity, 20),
        ComponentConfig{}
            .with_label("Rounded")
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_absolute_position(start_x, row2_y)
            .with_custom_background(sample_white)
            .with_border(accent_blue, 2.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.2f)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(bg_deep)
            .with_alignment(TextAlignment::Center));

    div(context, mk(entity, 21),
        ComponentConfig{}
            .with_label("Pill")
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_absolute_position(start_x + box_size + gap, row2_y)
            .with_custom_background(sample_white)
            .with_border(accent_cyan, 2.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.5f)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(bg_deep)
            .with_alignment(TextAlignment::Center));

    div(context, mk(entity, 22),
        ComponentConfig{}
            .with_label("Top Only")
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_absolute_position(start_x + 2 * (box_size + gap), row2_y)
            .with_custom_background(sample_white)
            .with_border(accent_blue, 2.0f)
            .with_rounded_corners(std::bitset<4>(0b1100))
            .with_roundness(0.25f)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(bg_deep)
            .with_alignment(TextAlignment::Center));

    div(context, mk(entity, 23),
        ComponentConfig{}
            .with_label("Diagonal")
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_absolute_position(start_x + 3 * (box_size + gap), row2_y)
            .with_custom_background(sample_white)
            .with_border(accent_orange, 2.0f)
            .with_rounded_corners(std::bitset<4>(0b1001))
            .with_roundness(0.3f)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(bg_deep)
            .with_alignment(TextAlignment::Center));

    div(context, mk(entity, 24),
        ComponentConfig{}
            .with_label("Hard Shadow")
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_absolute_position(start_x + 4 * (box_size + gap), row2_y)
            .with_custom_background(sample_white)
            .with_border(afterhours::Color{80, 80, 80, 255}, 2.0f)
            .with_hard_shadow(5.0f, 5.0f, afterhours::Color{0, 0, 0, 120})
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_custom_text_color(bg_deep)
            .with_alignment(TextAlignment::Center));

    div(context, mk(entity, 25),
        ComponentConfig{}
            .with_label("Soft Shadow")
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_absolute_position(start_x + 5 * (box_size + gap), row2_y)
            .with_custom_background(sample_white)
            .with_border(afterhours::Color{100, 100, 100, 255}, 1.0f)
            .with_rounded_corners(RoundedCorners())
            .with_soft_shadow(4.0f, 6.0f, 14.0f, afterhours::Color{0, 0, 0, 80})
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_custom_text_color(bg_deep)
            .with_alignment(TextAlignment::Center));

    // Row 2 label
    div(context, mk(entity, 29),
        ComponentConfig{}
            .with_label("Corners &\nShadows")
            .with_size(ComponentSize{pixels(row_label_width), pixels(box_size)})
            .with_absolute_position(row_label_x, row2_y)
            .with_font(UIComponent::DEFAULT_FONT,
                       h720(section_header_font_size))
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Right));

    // Row 3: Themed examples
    float row3_y = row2_y + box_size + gap;

    div(context, mk(entity, 30),
        ComponentConfig{}
            .with_label("Dark Mode")
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_absolute_position(start_x, row3_y)
            .with_custom_background(bg_deep)
            .with_border(accent_cyan, 2.0f)
            .with_rounded_corners(RoundedCorners())
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center));

    div(context, mk(entity, 31),
        ComponentConfig{}
            .with_label("Neon Glow")
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_absolute_position(start_x + box_size + gap, row3_y)
            .with_custom_background(afterhours::Color{15, 20, 30, 255})
            .with_border(afterhours::Color{0, 255, 200, 255}, 3.0f)
            .with_soft_shadow(0, 0, 12.0f, afterhours::Color{0, 255, 200, 60})
            .with_rounded_corners(RoundedCorners())
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(afterhours::Color{0, 255, 200, 255})
            .with_alignment(TextAlignment::Center));

    div(context, mk(entity, 32),
        ComponentConfig{}
            .with_label("Cozy")
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_absolute_position(start_x + 2 * (box_size + gap), row3_y)
            .with_custom_background(afterhours::Color{255, 250, 242, 255})
            .with_border(afterhours::Color{160, 130, 100, 255}, 3.0f)
            .with_soft_shadow(3.0f, 4.0f, 10.0f,
                              afterhours::Color{80, 60, 40, 50})
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.15f)
            .with_font("Gaegu-Bold", h720(18.0f))
            .with_custom_text_color(afterhours::Color{80, 60, 45, 255})
            .with_alignment(TextAlignment::Center));

    div(context, mk(entity, 33),
        ComponentConfig{}
            .with_label("! Danger")
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_absolute_position(start_x + 3 * (box_size + gap), row3_y)
            .with_custom_background(afterhours::Color{80, 25, 25, 255})
            .with_border(afterhours::Color{220, 70, 70, 255}, 3.0f)
            .with_rounded_corners(RoundedCorners())
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(afterhours::Color{255, 220, 220, 255})
            .with_alignment(TextAlignment::Center));

    div(context, mk(entity, 34),
        ComponentConfig{}
            .with_label("* Success")
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_absolute_position(start_x + 4 * (box_size + gap), row3_y)
            .with_custom_background(afterhours::Color{25, 70, 40, 255})
            .with_border(afterhours::Color{80, 200, 120, 255}, 3.0f)
            .with_rounded_corners(RoundedCorners())
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(afterhours::Color{220, 255, 230, 255})
            .with_alignment(TextAlignment::Center));

    div(context, mk(entity, 35),
        ComponentConfig{}
            .with_label("Outline Only")
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_absolute_position(start_x + 5 * (box_size + gap), row3_y)
            .with_custom_background(afterhours::Color{0, 0, 0, 0})
            .with_border(text_light, 2.0f)
            .with_rounded_corners(RoundedCorners())
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center));

    // Row 3 label
    div(context, mk(entity, 39),
        ComponentConfig{}
            .with_label("Themed Styles")
            .with_size(ComponentSize{pixels(row_label_width), pixels(box_size)})
            .with_absolute_position(row_label_x, row3_y)
            .with_font(UIComponent::DEFAULT_FONT,
                       h720(section_header_font_size))
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Right));

    // Interactive button with border
    float row4_y = row3_y + box_size + gap;
    float row4_height = 115.0f;

    // Row 4 label
    div(context, mk(entity, 49),
        ComponentConfig{}
            .with_label("Interactive")
            .with_size(
                ComponentSize{pixels(row_label_width), pixels(row4_height)})
            .with_absolute_position(row_label_x, row4_y)
            .with_font(UIComponent::DEFAULT_FONT,
                       h720(section_header_font_size))
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Right));

    auto btn_result = button(
        context, mk(entity, 40),
        ComponentConfig{}
            .with_label("Interactive Button")
            .with_size(ComponentSize{pixels(220.0f), pixels(56.0f)})
            .with_absolute_position(start_x, row4_y)
            .with_custom_background(accent_blue)
            .with_custom_hover_bg(
                afterhours::colors::lighten(accent_blue, 1.2f))
            .with_border(afterhours::colors::lighten(accent_blue, 1.3f), 3.0f)
            .with_soft_shadow(3.0f, 5.0f, 12.0f,
                              afterhours::Color{80, 145, 220, 60})
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.4f)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(bg_deep)
            .with_alignment(TextAlignment::Center)
            .with_cursor(CursorType::Pointer));

    if (btn_result) {
      log_info("Bordered button clicked!");
    }

    // Nested panel demo
    div(context, mk(entity, 50),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(280.0f), pixels(110.0f)})
            .with_absolute_position(start_x + 230.0f, row4_y - 5)
            .with_custom_background(panel_dark)
            .with_border(accent_cyan, 2.0f)
            .with_padding(Spacing::sm)
            .with_rounded_corners(RoundedCorners())
            .with_debug_name("nested_outer"));

    div(context, mk(entity, 51),
        ComponentConfig{}
            .with_label("Nested Panel")
            .with_size(ComponentSize{pixels(260), pixels(22)})
            .with_absolute_position(start_x + 240.0f, row4_y + 5)
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_custom_text_color(accent_cyan));

    div(context, mk(entity, 52),
        ComponentConfig{}
            .with_label("Inner Card")
            .with_size(ComponentSize{pixels(120.0f), pixels(55.0f)})
            .with_absolute_position(start_x + 250.0f, row4_y + 35)
            .with_custom_background(sample_white)
            .with_border(accent_orange, 2.0f)
            .with_rounded_corners(RoundedCorners())
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_custom_text_color(bg_deep)
            .with_alignment(TextAlignment::Center));

    div(context, mk(entity, 53),
        ComponentConfig{}
            .with_label("Second")
            .with_size(ComponentSize{pixels(100.0f), pixels(55.0f)})
            .with_absolute_position(start_x + 385.0f, row4_y + 35)
            .with_custom_background(afterhours::Color{0, 0, 0, 0})
            .with_border(text_light, 2.0f)
            .with_rounded_corners(RoundedCorners())
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center));

    // Row 5: Per-side borders (new feature demo)
    // ps_box sized so rows 5+6 both fit within 720p (row 6's bottom edge —
    // the dotted underline — must stay on-screen).
    float row5_y = row4_y + row4_height;
    float ps_box = 64.0f;
    // rows 5 and 6 hold 5 boxes, not 6, so they get the extra width
    float ps_w = (content_width - 4.0f * gap) / 5.0f;

    div(context, mk(entity, 59),
        ComponentConfig{}
            .with_label("Per-Side")
            .with_size(ComponentSize{pixels(row_label_width), pixels(ps_box)})
            .with_absolute_position(row_label_x, row5_y)
            .with_font(UIComponent::DEFAULT_FONT,
                       h720(section_header_font_size))
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Right));

    div(context, mk(entity, 60),
        ComponentConfig{}
            .with_label("Top Only")
            .with_size(ComponentSize{pixels(ps_w), pixels(ps_box)})
            .with_absolute_position(start_x, row5_y)
            .with_custom_background(sample_white)
            .with_border_top(accent_blue, pixels(3.0f))
            .with_font(UIComponent::DEFAULT_FONT, h720(13.0f))
            .with_custom_text_color(bg_deep)
            .with_alignment(TextAlignment::Center));

    div(context, mk(entity, 61),
        ComponentConfig{}
            .with_label("Bottom Only")
            .with_size(ComponentSize{pixels(ps_w), pixels(ps_box)})
            .with_absolute_position(start_x + ps_w + gap, row5_y)
            .with_custom_background(sample_white)
            .with_border_bottom(accent_cyan, pixels(3.0f))
            .with_font(UIComponent::DEFAULT_FONT, h720(13.0f))
            .with_custom_text_color(bg_deep)
            .with_alignment(TextAlignment::Center));

    div(context, mk(entity, 62),
        ComponentConfig{}
            .with_label("Left+Right")
            .with_size(ComponentSize{pixels(ps_w), pixels(ps_box)})
            .with_absolute_position(start_x + 2 * (ps_w + gap), row5_y)
            .with_custom_background(sample_white)
            .with_border_left(accent_orange, pixels(3.0f))
            .with_border_right(accent_orange, pixels(3.0f))
            .with_font(UIComponent::DEFAULT_FONT, h720(13.0f))
            .with_custom_text_color(bg_deep)
            .with_alignment(TextAlignment::Center));

    div(context, mk(entity, 63),
        ComponentConfig{}
            .with_label("Mixed")
            .with_size(ComponentSize{pixels(ps_w), pixels(ps_box)})
            .with_absolute_position(start_x + 3 * (ps_w + gap), row5_y)
            .with_custom_background(sample_white)
            .with_border_top(accent_blue, pixels(3.0f))
            .with_border_right(accent_cyan, pixels(2.0f))
            .with_border_bottom(accent_orange, pixels(3.0f))
            .with_border_left(text_light, pixels(2.0f))
            .with_font(UIComponent::DEFAULT_FONT, h720(13.0f))
            .with_custom_text_color(bg_deep)
            .with_alignment(TextAlignment::Center));

    div(context, mk(entity, 64),
        ComponentConfig{}
            .with_label("Underline")
            .with_size(ComponentSize{pixels(ps_w), pixels(ps_box)})
            .with_absolute_position(start_x + 4 * (ps_w + gap), row5_y)
            .with_custom_background(sample_white)
            .with_border_bottom(afterhours::Color{200, 60, 60, 255},
                                pixels(4.0f))
            .with_font(UIComponent::DEFAULT_FONT, h720(13.0f))
            .with_custom_text_color(bg_deep)
            .with_alignment(TextAlignment::Center));

    // Row 6: Dotted borders (new BorderStyle::Dotted feature demo)
    float row6_y = row5_y + ps_box + gap;

    div(context, mk(entity, 69),
        ComponentConfig{}
            .with_label("Dotted")
            .with_size(ComponentSize{pixels(row_label_width), pixels(ps_box)})
            .with_absolute_position(row_label_x, row6_y)
            .with_font(UIComponent::DEFAULT_FONT,
                       h720(section_header_font_size))
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Right));

    div(context, mk(entity, 70),
        ComponentConfig{}
            .with_label("Thin")
            .with_size(ComponentSize{pixels(ps_w), pixels(ps_box)})
            .with_absolute_position(start_x, row6_y)
            .with_custom_background(sample_white)
            .with_border(afterhours::Color{80, 80, 80, 255}, 1.0f,
                         BorderStyle::Dotted)
            .with_font(UIComponent::DEFAULT_FONT, h720(13.0f))
            .with_custom_text_color(bg_deep)
            .with_alignment(TextAlignment::Center));

    div(context, mk(entity, 71),
        ComponentConfig{}
            .with_label("Medium")
            .with_size(ComponentSize{pixels(ps_w), pixels(ps_box)})
            .with_absolute_position(start_x + ps_w + gap, row6_y)
            .with_custom_background(sample_white)
            .with_border(afterhours::Color{50, 50, 50, 255}, 3.0f,
                         BorderStyle::Dotted)
            .with_font(UIComponent::DEFAULT_FONT, h720(13.0f))
            .with_custom_text_color(bg_deep)
            .with_alignment(TextAlignment::Center));

    div(context, mk(entity, 72),
        ComponentConfig{}
            .with_label("Thick")
            .with_size(ComponentSize{pixels(ps_w), pixels(ps_box)})
            .with_absolute_position(start_x + 2 * (ps_w + gap), row6_y)
            .with_custom_background(sample_white)
            .with_border(afterhours::Color{30, 30, 30, 255}, 6.0f,
                         BorderStyle::Dotted)
            .with_font(UIComponent::DEFAULT_FONT, h720(13.0f))
            .with_custom_text_color(bg_deep)
            .with_alignment(TextAlignment::Center));

    div(context, mk(entity, 73),
        ComponentConfig{}
            .with_label("Orange")
            .with_size(ComponentSize{pixels(ps_w), pixels(ps_box)})
            .with_absolute_position(start_x + 3 * (ps_w + gap), row6_y)
            .with_custom_background(sample_white)
            .with_border(accent_orange, 3.0f, BorderStyle::Dotted)
            .with_font(UIComponent::DEFAULT_FONT, h720(13.0f))
            .with_custom_text_color(bg_deep)
            .with_alignment(TextAlignment::Center));

    div(context, mk(entity, 74),
        ComponentConfig{}
            .with_label("Underline")
            .with_size(ComponentSize{pixels(ps_w), pixels(ps_box)})
            .with_absolute_position(start_x + 4 * (ps_w + gap), row6_y)
            .with_custom_background(sample_white)
            .with_border_bottom(accent_blue, pixels(4.0f), BorderStyle::Dotted)
            .with_font(UIComponent::DEFAULT_FONT, h720(13.0f))
            .with_custom_text_color(bg_deep)
            .with_alignment(TextAlignment::Center));
  }
};

REGISTER_EXAMPLE_SCREEN(example_borders, "Component Galleries", "Border styling options",
                        ExampleBordersScreen)
