#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct DecorativeFrameShowcase : ScreenSystem<UIContext<InputAction>> {

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    // Warm, craft-inspired theme
    Theme theme;
    theme.background = afterhours::Color{245, 240, 230, 255}; // Cream
    theme.surface = afterhours::Color{255, 250, 240, 255};    // Off-white
    theme.primary = afterhours::Color{139, 90, 43, 255};      // Brown
    theme.secondary = afterhours::Color{180, 140, 90, 255};   // Tan
    theme.accent = afterhours::Color{200, 80, 60, 255};       // Rust red
    theme.font = afterhours::Color{60, 40, 20, 255};          // Dark brown
    theme.font_muted = afterhours::Color{120, 100, 80, 255};
    context.theme = theme;

    // Main container with background
    auto main = div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), percent(1.0f)})
            .with_background(Theme::Usage::Background)
            .with_flex_direction(FlexDirection::Column)
            .with_align_items(AlignItems::Center)
            .with_debug_name("main"));

    // Title
    div(context, mk(main.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), percent(0.06f)})
            .with_label("Decorative Frames")
            .with_font(UIComponent::DEFAULT_FONT, h720(26.0f))
            .with_custom_text_color(theme.font)
            .with_alignment(TextAlignment::Center)
            .with_margin(Margin{.top = h720(8.0f)})
            .with_debug_name("title"));

    // Subtitle
    div(context, mk(main.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), percent(0.04f)})
            .with_label("Three style variants for decorative borders")
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(theme.font_muted)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("subtitle"));

    // Layout configuration parameters
    constexpr float frame_row_height_pct = 0.38f;    // Compact height to reduce gap before info section
    constexpr float frame_row_width_pct = 0.95f;
    constexpr float frame_row_top_margin = 16.0f;

    // Container for the three frames - compact layout
    auto row = div(context, mk(main.ent(), 2),
        ComponentConfig{}
            .with_size(ComponentSize{percent(frame_row_width_pct), percent(frame_row_height_pct)})
            .with_flex_direction(FlexDirection::Row)
            .with_justify_content(JustifyContent::SpaceAround)
            .with_align_items(AlignItems::FlexStart)
            .with_margin(Margin{.top = h720(frame_row_top_margin)})
            .with_debug_name("frame_row"));

    // Column and frame configuration parameters
    constexpr float col_width_pct = 0.30f;           // Column width percentage
    constexpr float frame_size_px = 140.0f;          // Frame dimensions in pixels
    constexpr float label_height_px = 30.0f;         // Label height
    constexpr float desc_height_px = 24.0f;          // Description height
    constexpr float label_top_margin_px = 6.0f;      // Space between frame and label
    constexpr float border_thick_px = 8.0f;          // Thick border (KraftPaper)
    constexpr float border_thin_px = 3.0f;           // Thin border (Simple)
    constexpr float border_medium_px = 5.0f;         // Medium border (Inset)

    // Each column width - height determined by content
    Size col_width = percent(col_width_pct);
    // Frame size - use size that matches visual rendering
    Size frame_width = h720(frame_size_px);
    Size frame_height = h720(frame_size_px);

    // ========== KRAFT PAPER STYLE ==========
    afterhours::Color kraft_frame_color{101, 67, 33, 255};
    afterhours::Color kraft_bg{222, 184, 135, 255};

    auto kraft_col = div(context, mk(row.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{col_width, percent(1.0f)})
            .with_flex_direction(FlexDirection::Column)
            .with_align_items(AlignItems::Center)
            .with_justify_content(JustifyContent::FlexStart)
            .with_debug_name("kraft_col"));

    decorative_frame(context, mk(kraft_col.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{frame_width, frame_height})
            .with_custom_background(kraft_bg)
            .with_border(kraft_frame_color, h720(border_thick_px))
            .with_debug_name("kraft_frame"),
        DecorativeFrameStyle::KraftPaper);

    div(context, mk(kraft_col.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{frame_width, h720(label_height_px)})
            .with_label("KraftPaper")
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(theme.font)
            .with_alignment(TextAlignment::Center)
            .with_margin(Margin{.top = h720(label_top_margin_px)})
            .with_debug_name("kraft_label"));

    div(context, mk(kraft_col.ent(), 2),
        ComponentConfig{}
            .with_size(ComponentSize{frame_width, h720(desc_height_px)})
            .with_label("Layered + corners")
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_custom_text_color(theme.font_muted)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("kraft_desc"));

    // ========== SIMPLE STYLE ==========
    afterhours::Color simple_frame_color{70, 130, 180, 255};
    afterhours::Color simple_bg{240, 248, 255, 255};

    auto simple_col = div(context, mk(row.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{col_width, percent(1.0f)})
            .with_flex_direction(FlexDirection::Column)
            .with_align_items(AlignItems::Center)
            .with_justify_content(JustifyContent::FlexStart)
            .with_debug_name("simple_col"));

    decorative_frame(context, mk(simple_col.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{frame_width, frame_height})
            .with_custom_background(simple_bg)
            .with_border(simple_frame_color, h720(border_thin_px))
            .with_debug_name("simple_frame"),
        DecorativeFrameStyle::Simple);

    div(context, mk(simple_col.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{frame_width, h720(label_height_px)})
            .with_label("Simple")
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(theme.font)
            .with_alignment(TextAlignment::Center)
            .with_margin(Margin{.top = h720(label_top_margin_px)})
            .with_debug_name("simple_label"));

    div(context, mk(simple_col.ent(), 2),
        ComponentConfig{}
            .with_size(ComponentSize{frame_width, h720(desc_height_px)})
            .with_label("Border + background")
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_custom_text_color(theme.font_muted)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("simple_desc"));

    // ========== INSET STYLE ==========
    afterhours::Color inset_frame_color{128, 128, 128, 255};
    afterhours::Color inset_bg{220, 220, 220, 255};

    auto inset_col = div(context, mk(row.ent(), 2),
        ComponentConfig{}
            .with_size(ComponentSize{col_width, percent(1.0f)})
            .with_flex_direction(FlexDirection::Column)
            .with_align_items(AlignItems::Center)
            .with_justify_content(JustifyContent::FlexStart)
            .with_debug_name("inset_col"));

    decorative_frame(context, mk(inset_col.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{frame_width, frame_height})
            .with_custom_background(inset_bg)
            .with_border(inset_frame_color, h720(border_medium_px))
            .with_debug_name("inset_frame"),
        DecorativeFrameStyle::Inset);

    div(context, mk(inset_col.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{frame_width, h720(label_height_px)})
            .with_label("Inset")
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(theme.font)
            .with_alignment(TextAlignment::Center)
            .with_margin(Margin{.top = h720(label_top_margin_px)})
            .with_debug_name("inset_label"));

    div(context, mk(inset_col.ent(), 2),
        ComponentConfig{}
            .with_size(ComponentSize{frame_width, h720(desc_height_px)})
            .with_label("Sunken shadow")
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_custom_text_color(theme.font_muted)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("inset_desc"));

    // ========== INFO SECTION ==========
    constexpr float info_section_height_pct = 0.20f;  // Height of info section
    constexpr float info_section_width_pct = 0.90f;   // Width of info section
    constexpr float info_section_margin_top_px = 8.0f; // Reduced gap before info section

    auto info_section = div(context, mk(main.ent(), 3),
        ComponentConfig{}
            .with_size(ComponentSize{percent(info_section_width_pct), percent(info_section_height_pct)})
            .with_background(Theme::Usage::Surface)
            .with_border(theme.secondary, h720(2.0f))
            .with_flex_direction(FlexDirection::Column)
            .with_align_items(AlignItems::Center)
            .with_justify_content(JustifyContent::Center)
            .with_margin(Margin{.top = h720(info_section_margin_top_px)})
            .with_debug_name("info_section"));

    div(context, mk(info_section.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.95f), percent(0.35f)})
            .with_label("Usage Tips")
            .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
            .with_custom_text_color(theme.font)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("info_title"));

    div(context, mk(info_section.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.95f), percent(0.55f)})
            .with_label("KraftPaper: Rustic warmth | Simple: Clean minimal | Inset: Depth effect")
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_custom_text_color(theme.font_muted)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("info_content"));
  }
};

REGISTER_EXAMPLE_SCREEN(decorative_frame, "Component Galleries",
                        "Decorative frame border styles", DecorativeFrameShowcase)
