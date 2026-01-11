#pragma once

// WORKAROUND: Decorative Frame/Border
// See LIBRARY_GAPS.md #31
//
// This helper creates a decorative frame around the screen by drawing
// multiple overlapping border rectangles to create a kraft paper / scrapbook edge effect.
//
// Migration: When library adds with_border_image() for 9-slice borders,
// replace with actual border texture.

#include <afterhours/ah.h>

namespace ui_workarounds {

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Creates a decorative kraft paper style frame around the screen
template <typename Context, typename Entity>
inline void kraft_paper_frame(
    Context &context, 
    Entity &entity,
    int base_id,
    float screen_w, float screen_h,
    afterhours::Color frame_color,      // Main frame color (brown)
    afterhours::Color bg_color,         // Background color (kraft tan)
    float frame_thickness = 12.0f
) {
    // Outer dark border
    div(context, mk(entity, base_id),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(screen_w)), 
                                    pixels(static_cast<int>(screen_h))})
            .with_custom_background(frame_color)
            .with_debug_name("frame_outer"));
    
    // Inner lighter border (creates depth)
    afterhours::Color lighter_frame{
        static_cast<unsigned char>(std::min(255, frame_color.r + 20)),
        static_cast<unsigned char>(std::min(255, frame_color.g + 15)),
        static_cast<unsigned char>(std::min(255, frame_color.b + 10)),
        frame_color.a
    };
    
    float inset1 = frame_thickness * 0.3f;
    div(context, mk(entity, base_id + 1),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(screen_w - inset1 * 2)), 
                                    pixels(static_cast<int>(screen_h - inset1 * 2))})
            .with_absolute_position()
            .with_translate(inset1, inset1)
            .with_custom_background(lighter_frame)
            .with_debug_name("frame_inner1"));
    
    // Main background
    float inset2 = frame_thickness;
    div(context, mk(entity, base_id + 2),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(screen_w - inset2 * 2)), 
                                    pixels(static_cast<int>(screen_h - inset2 * 2))})
            .with_absolute_position()
            .with_translate(inset2, inset2)
            .with_custom_background(bg_color)
            .with_debug_name("frame_bg"));
    
    // Add corner accents for "hand-made" feel
    // These are small darker squares in the corners
    float corner_size = 8.0f;
    afterhours::Color corner_color{
        static_cast<unsigned char>(std::max(0, frame_color.r - 30)),
        static_cast<unsigned char>(std::max(0, frame_color.g - 25)),
        static_cast<unsigned char>(std::max(0, frame_color.b - 20)),
        frame_color.a
    };
    
    // Top-left corner
    div(context, mk(entity, base_id + 3),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(corner_size)), 
                                    pixels(static_cast<int>(corner_size))})
            .with_absolute_position()
            .with_translate(2.0f, 2.0f)
            .with_custom_background(corner_color)
            .with_debug_name("corner_tl"));
    
    // Top-right corner
    div(context, mk(entity, base_id + 4),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(corner_size)), 
                                    pixels(static_cast<int>(corner_size))})
            .with_absolute_position()
            .with_translate(screen_w - corner_size - 2.0f, 2.0f)
            .with_custom_background(corner_color)
            .with_debug_name("corner_tr"));
    
    // Bottom-left corner
    div(context, mk(entity, base_id + 5),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(corner_size)), 
                                    pixels(static_cast<int>(corner_size))})
            .with_absolute_position()
            .with_translate(2.0f, screen_h - corner_size - 2.0f)
            .with_custom_background(corner_color)
            .with_debug_name("corner_bl"));
    
    // Bottom-right corner
    div(context, mk(entity, base_id + 6),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(corner_size)), 
                                    pixels(static_cast<int>(corner_size))})
            .with_absolute_position()
            .with_translate(screen_w - corner_size - 2.0f, screen_h - corner_size - 2.0f)
            .with_custom_background(corner_color)
            .with_debug_name("corner_br"));
}

} // namespace ui_workarounds

