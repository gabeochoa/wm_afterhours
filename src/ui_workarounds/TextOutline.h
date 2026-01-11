#pragma once

// WORKAROUND: Text Stroke/Outline
// See LIBRARY_GAPS.md #29
//
// This helper renders text with an outline effect by drawing the text
// multiple times at offsets in the outline color, then once in the center
// with the main text color.
//
// Migration: When library adds with_text_stroke(), replace these calls with
// the native implementation.

#include <afterhours/ah.h>

namespace ui_workarounds {

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Renders text with an outline/stroke effect
// Creates 8 offset copies of the text (outline) plus the centered text
template <typename Context, typename Entity>
inline void text_with_outline(
    Context &context, 
    Entity &entity,
    int base_id,
    const std::string &text,
    float x, float y,
    float width, float height,
    const std::string &font_name,
    float font_size,
    afterhours::Color text_color,
    afterhours::Color outline_color,
    float outline_thickness = 2.0f,
    TextAlignment alignment = TextAlignment::Left
) {
    // Draw outline by rendering text at 8 offset positions
    std::vector<std::pair<float, float>> offsets = {
        {-outline_thickness, -outline_thickness},
        {0, -outline_thickness},
        {outline_thickness, -outline_thickness},
        {-outline_thickness, 0},
        {outline_thickness, 0},
        {-outline_thickness, outline_thickness},
        {0, outline_thickness},
        {outline_thickness, outline_thickness},
    };
    
    int id = base_id;
    for (const auto &[ox, oy] : offsets) {
        div(context, mk(entity, id++),
            ComponentConfig{}
                .with_label(text)
                .with_size(ComponentSize{pixels(static_cast<int>(width)), 
                                        pixels(static_cast<int>(height))})
                .with_absolute_position()
                .with_translate(x + ox, y + oy)
                .with_font(font_name, font_size)
                .with_custom_text_color(outline_color)
                .with_alignment(alignment)
                .with_debug_name("outline_" + std::to_string(id)));
    }
    
    // Draw main text on top
    div(context, mk(entity, id),
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

