#pragma once

// WORKAROUND: Text Drop Shadow
// See LIBRARY_GAPS.md #34
//
// This helper renders text with a drop shadow by drawing the text twice:
// once offset in the shadow color, then once in the main position.
//
// Migration: When library adds with_text_shadow(), replace these calls with
// the native implementation.

#include <afterhours/ah.h>

namespace ui_workarounds {

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Renders text with a drop shadow effect
template <typename Context, typename Entity>
inline void text_with_shadow(
    Context &context, 
    Entity &entity,
    int base_id,
    const std::string &text,
    float x, float y,
    float width, float height,
    const std::string &font_name,
    float font_size,
    afterhours::Color text_color,
    afterhours::Color shadow_color,
    float shadow_offset_x = 2.0f,
    float shadow_offset_y = 2.0f,
    TextAlignment alignment = TextAlignment::Left
) {
    // Draw shadow first (behind)
    div(context, mk(entity, base_id),
        ComponentConfig{}
            .with_label(text)
            .with_size(ComponentSize{pixels(static_cast<int>(width)), 
                                    pixels(static_cast<int>(height))})
            .with_absolute_position()
            .with_translate(x + shadow_offset_x, y + shadow_offset_y)
            .with_font(font_name, font_size)
            .with_custom_text_color(shadow_color)
            .with_alignment(alignment)
            .with_debug_name("text_shadow"));
    
    // Draw main text on top
    div(context, mk(entity, base_id + 1),
        ComponentConfig{}
            .with_label(text)
            .with_size(ComponentSize{pixels(static_cast<int>(width)), 
                                    pixels(static_cast<int>(height))})
            .with_absolute_position()
            .with_translate(x, y)
            .with_font(font_name, font_size)
            .with_custom_text_color(text_color)
            .with_alignment(alignment)
            .with_debug_name("text_main"));
}

} // namespace ui_workarounds

