#pragma once

// WORKAROUND: Notification Badge
// See LIBRARY_GAPS.md (new gap to add)
//
// This helper creates a small circular badge with a number or symbol,
// typically used for notifications on icons.
//
// Migration: When library adds proper badge support, replace with native.

#include <afterhours/ah.h>

namespace ui_workarounds {

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Creates a notification badge (red circle with number/symbol)
template <typename Context, typename Entity>
inline void notification_badge(
    Context &context, 
    Entity &entity,
    int id,
    const std::string &text,  // "2", "!", etc.
    float x, float y,
    float size = 20.0f,
    afterhours::Color bg_color = afterhours::Color{180, 60, 60, 255},  // Red
    afterhours::Color text_color = afterhours::Color{255, 255, 255, 255}  // White
) {
    // Badge background (circular)
    div(context, mk(entity, id),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(size)), 
                                    pixels(static_cast<int>(size))})
            .with_absolute_position()
            .with_translate(x, y)
            .with_custom_background(bg_color)
            .with_rounded_corners(std::bitset<4>(0b1111))
            .with_roundness(1.0f)  // Fully circular
            .with_debug_name("badge_bg"));
    
    // Badge text
    div(context, mk(entity, id + 1),
        ComponentConfig{}
            .with_label(text)
            .with_size(ComponentSize{pixels(static_cast<int>(size)), 
                                    pixels(static_cast<int>(size))})
            .with_absolute_position()
            .with_translate(x, y)
            .with_font("Gaegu-Bold", size * 0.6f)
            .with_custom_text_color(text_color)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("badge_text"));
}

// Creates an icon button with an optional notification badge
template <typename Context, typename Entity>
inline void icon_button_with_badge(
    Context &context, 
    Entity &entity,
    int base_id,
    const std::string &icon_text,  // Symbol to show in the icon
    const std::string &label,      // Label below the icon
    float x, float y,
    float icon_size,
    afterhours::Color icon_bg,
    afterhours::Color icon_border,
    afterhours::Color icon_text_color,
    afterhours::Color label_color,
    const std::string &font_name,
    float icon_font_size,
    float label_font_size,
    const std::string &badge_text = "",  // Empty = no badge
    afterhours::Color badge_bg = afterhours::Color{180, 60, 60, 255}
) {
    // Icon background
    button(context, mk(entity, base_id),
           ComponentConfig{}
               .with_label(icon_text)
               .with_size(ComponentSize{pixels(static_cast<int>(icon_size)), 
                                       pixels(static_cast<int>(icon_size))})
               .with_absolute_position()
               .with_translate(x, y)
               .with_custom_background(icon_bg)
               .with_border(icon_border, 2.0f)
               .with_font(font_name, icon_font_size)
               .with_custom_text_color(icon_text_color)
               .with_alignment(TextAlignment::Center)
               .with_rounded_corners(std::bitset<4>(0b1111))
               .with_roundness(0.2f)
               .with_debug_name("icon_btn"));
    
    // Label below
    div(context, mk(entity, base_id + 1),
        ComponentConfig{}
            .with_label(label)
            .with_size(ComponentSize{pixels(static_cast<int>(icon_size + 20)), 
                                    pixels(20)})
            .with_absolute_position()
            .with_translate(x - 10.0f, y + icon_size + 5.0f)
            .with_font(font_name, label_font_size)
            .with_custom_text_color(label_color)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("icon_label"));
    
    // Badge (if text provided)
    if (!badge_text.empty()) {
        notification_badge(context, entity, base_id + 2,
            badge_text,
            x + icon_size - 12.0f,  // Position at top-right of icon
            y - 5.0f,
            18.0f,
            badge_bg);
    }
}

} // namespace ui_workarounds

