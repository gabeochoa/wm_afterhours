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
            .with_custom_background(theme.background)
            .with_flex_direction(FlexDirection::Column)
            .with_align_items(AlignItems::Center)
            .with_debug_name("main"));

    // Title
    div(context, mk(main.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), percent(0.08f)})
            .with_label("Decorative Frames")
            .with_font(UIComponent::DEFAULT_FONT, h720(24.0f))
            .with_custom_text_color(theme.font)
            .with_alignment(TextAlignment::Center)
            .with_margin(Margin{.top = h720(10.0f)})
            .with_debug_name("title"));

    // Subtitle
    div(context, mk(main.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), percent(0.05f)})
            .with_label("Three style variants for decorative borders")
            .with_font(UIComponent::DEFAULT_FONT, h720(12.0f))
            .with_custom_text_color(theme.font_muted)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("subtitle"));

    // Container for the three frames
    auto row = div(context, mk(main.ent(), 2),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.95f), percent(0.70f)})
            .with_flex_direction(FlexDirection::Row)
            .with_justify_content(JustifyContent::SpaceAround)
            .with_align_items(AlignItems::Center)
            .with_debug_name("frame_row"));

    // Each column is ~30% of row width
    Size col_width = percent(0.30f);
    Size col_height = percent(0.95f);

    // ========== KRAFT PAPER STYLE ==========
    afterhours::Color kraft_frame_color{101, 67, 33, 255};
    afterhours::Color kraft_bg{222, 184, 135, 255};

    auto kraft_col = div(context, mk(row.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{col_width, col_height})
            .with_flex_direction(FlexDirection::Column)
            .with_align_items(AlignItems::Center)
            .with_justify_content(JustifyContent::FlexStart)
            .with_debug_name("kraft_col"));

    decorative_frame(context, mk(kraft_col.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.90f), percent(0.75f)})
            .with_custom_background(kraft_bg)
            .with_border(kraft_frame_color, h720(8.0f))
            .with_debug_name("kraft_frame"),
        DecorativeFrameStyle::KraftPaper);

    div(context, mk(kraft_col.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.90f), percent(0.10f)})
            .with_label("KraftPaper")
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_custom_text_color(theme.font)
            .with_alignment(TextAlignment::Center)
            .with_margin(Margin{.top = h720(6.0f)})
            .with_debug_name("kraft_label"));

    div(context, mk(kraft_col.ent(), 2),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.90f), percent(0.08f)})
            .with_label("Layered + corners")
            .with_font(UIComponent::DEFAULT_FONT, h720(12.0f))
            .with_custom_text_color(theme.font_muted)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("kraft_desc"));

    // ========== SIMPLE STYLE ==========
    afterhours::Color simple_frame_color{70, 130, 180, 255};
    afterhours::Color simple_bg{240, 248, 255, 255};

    auto simple_col = div(context, mk(row.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{col_width, col_height})
            .with_flex_direction(FlexDirection::Column)
            .with_align_items(AlignItems::Center)
            .with_justify_content(JustifyContent::FlexStart)
            .with_debug_name("simple_col"));

    decorative_frame(context, mk(simple_col.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.90f), percent(0.75f)})
            .with_custom_background(simple_bg)
            .with_border(simple_frame_color, h720(3.0f))
            .with_debug_name("simple_frame"),
        DecorativeFrameStyle::Simple);

    div(context, mk(simple_col.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.90f), percent(0.10f)})
            .with_label("Simple")
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_custom_text_color(theme.font)
            .with_alignment(TextAlignment::Center)
            .with_margin(Margin{.top = h720(6.0f)})
            .with_debug_name("simple_label"));

    div(context, mk(simple_col.ent(), 2),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.90f), percent(0.08f)})
            .with_label("Border + background")
            .with_font(UIComponent::DEFAULT_FONT, h720(12.0f))
            .with_custom_text_color(theme.font_muted)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("simple_desc"));

    // ========== INSET STYLE ==========
    afterhours::Color inset_frame_color{128, 128, 128, 255};
    afterhours::Color inset_bg{220, 220, 220, 255};

    auto inset_col = div(context, mk(row.ent(), 2),
        ComponentConfig{}
            .with_size(ComponentSize{col_width, col_height})
            .with_flex_direction(FlexDirection::Column)
            .with_align_items(AlignItems::Center)
            .with_justify_content(JustifyContent::FlexStart)
            .with_debug_name("inset_col"));

    decorative_frame(context, mk(inset_col.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.90f), percent(0.75f)})
            .with_custom_background(inset_bg)
            .with_border(inset_frame_color, h720(5.0f))
            .with_debug_name("inset_frame"),
        DecorativeFrameStyle::Inset);

    div(context, mk(inset_col.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.90f), percent(0.10f)})
            .with_label("Inset")
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_custom_text_color(theme.font)
            .with_alignment(TextAlignment::Center)
            .with_margin(Margin{.top = h720(6.0f)})
            .with_debug_name("inset_label"));

    div(context, mk(inset_col.ent(), 2),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.90f), percent(0.08f)})
            .with_label("Sunken shadow")
            .with_font(UIComponent::DEFAULT_FONT, h720(12.0f))
            .with_custom_text_color(theme.font_muted)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("inset_desc"));

    // Footer note
    div(context, mk(main.ent(), 3),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), percent(0.06f)})
            .with_label("All frames use responsive h720() sizing")
            .with_font(UIComponent::DEFAULT_FONT, h720(12.0f))
            .with_custom_text_color(theme.font_muted)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("footer"));
  }
};

REGISTER_EXAMPLE_SCREEN(decorative_frame, "Component Galleries",
                        "Decorative frame border styles", DecorativeFrameShowcase)
