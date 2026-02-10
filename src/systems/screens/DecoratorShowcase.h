#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/ui/ui_decorators.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct DecoratorShowcase : ScreenSystem<UIContext<InputAction>> {

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {

    // Dark sci-fi theme
    Theme theme;
    theme.background = afterhours::Color{10, 12, 16, 255};
    theme.surface = afterhours::Color{20, 24, 30, 255};
    theme.primary = afterhours::Color{70, 160, 160, 255};
    theme.secondary = afterhours::Color{50, 120, 120, 255};
    theme.accent = afterhours::Color{200, 160, 80, 255};
    theme.font = afterhours::Color{210, 220, 230, 255};
    theme.font_muted = afterhours::Color{160, 175, 185, 255};
    context.theme = theme;
    UIStylingDefaults::get().set_default_font(UIComponent::DEFAULT_FONT, h720(16.0f));

    using C = afterhours::Color;
    C teal{65, 150, 150, 255};
    C amber{200, 160, 80, 255};
    C green{100, 200, 120, 255};
    C panel_bg{16, 22, 26, 255};
    C panel_border{35, 50, 55, 255};

    // ========== ROOT ==========
    auto root = div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), percent(1.0f)})
            .with_background(Theme::Usage::Background)
            .with_flex_direction(FlexDirection::Column)
            .with_align_items(AlignItems::Center)
            .with_debug_name("root"));

    // Title
    div(context, mk(root.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), h720(36.0f)})
            .with_label("UI Decorators")
            .with_font_size(h720(24.0f))
            .with_custom_text_color(theme.font)
            .with_alignment(TextAlignment::Center)
            .with_margin(Margin{.top = h720(10.0f)}));

    div(context, mk(root.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), h720(20.0f)})
            .with_label("Composable visual effects via .decorate()")
            .with_font_size(h720(13.0f))
            .with_custom_text_color(theme.font_muted)
            .with_alignment(TextAlignment::Center));

    // Separator between title and content
    div(context, mk(root.ent(), 10),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.85f), pixels(1)})
            .with_custom_background(afterhours::Color{255, 255, 255, 40})
            .with_self_align(SelfAlign::Center)
            .with_margin(Margin{.top = h720(4.0f), .bottom = h720(4.0f)})
            .with_debug_name("section_separator_title"));

    // ========== CONTENT ROW ==========
    auto content = div(context, mk(root.ent(), 2),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.94f), percent(0.82f)})
            .with_flex_direction(FlexDirection::Row)
            .with_justify_content(JustifyContent::SpaceBetween)
            .with_align_items(AlignItems::FlexStart)
            .with_margin(Margin{.top = h720(10.0f)}));

    // ====================================================================
    // COLUMN 1: Corner Brackets
    // ====================================================================
    auto col1 = div(context, mk(content.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.31f), percent(1.0f)})
            .with_flex_direction(FlexDirection::Column)
            .with_align_items(AlignItems::Center)
            .with_debug_name("col_brackets"));

    div(context, mk(col1.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.9f), h720(24.0f)})
            .with_label("with_brackets()")
            .with_font_size(h720(14.0f))
            .with_custom_text_color(teal)
            .with_alignment(TextAlignment::Center)
            .with_margin(Margin{.bottom = h720(8.0f)}));

    // Demo 1: HUD-style panel with brackets
    {
      auto panel = div(context, mk(col1.ent(), 1),
          ComponentConfig{}
              .with_size(ComponentSize{percent(0.88f), h720(100.0f)})
              .with_custom_background(panel_bg)
              .with_border(panel_border, 1.0f)
              .with_rounded_corners(RoundedCorners().all_sharp())
              .with_flex_direction(FlexDirection::Column)
              .with_align_items(AlignItems::Center)
              .with_justify_content(JustifyContent::Center)
              .with_debug_name("bracket_panel_1"))
          .decorate(with_brackets(context, teal, 18.0f, 2.0f));

      div(context, mk(panel.ent(), 0),
          ComponentConfig{}
              .with_size(ComponentSize{children(), children()})
              .with_label("SYSTEM STATUS")
              .with_font_size(h720(15.0f))
              .with_custom_text_color(teal)
              .with_alignment(TextAlignment::Center));

      div(context, mk(panel.ent(), 1),
          ComponentConfig{}
              .with_size(ComponentSize{percent(0.6f), h720(3.0f)})
              .with_custom_background(C{teal.r, teal.g, teal.b, 80})
              .with_rounded_corners(RoundedCorners().all_sharp())
              .with_margin(Margin{.top = h720(6.0f)}));

      div(context, mk(panel.ent(), 2),
          ComponentConfig{}
              .with_size(ComponentSize{children(), children()})
              .with_label("All systems operational")
              .with_font_size(h720(12.0f))
              .with_custom_text_color(theme.font_muted)
              .with_margin(Margin{.top = h720(6.0f)}));
    }

    // Demo 2: Amber warning panel
    {
      auto panel = div(context, mk(col1.ent(), 2),
          ComponentConfig{}
              .with_size(ComponentSize{percent(0.88f), h720(80.0f)})
              .with_custom_background(C{25, 20, 14, 255})
              .with_border(C{55, 45, 25, 255}, 1.0f)
              .with_rounded_corners(RoundedCorners().all_sharp())
              .with_flex_direction(FlexDirection::Column)
              .with_align_items(AlignItems::Center)
              .with_justify_content(JustifyContent::Center)
              .with_margin(Margin{.top = h720(12.0f)})
              .with_debug_name("bracket_panel_2"))
          .decorate(with_brackets(context, amber, 22.0f, 3.0f));

      div(context, mk(panel.ent(), 0),
          ComponentConfig{}
              .with_size(ComponentSize{children(), children()})
              .with_label("! WARNING !")
              .with_font_size(h720(14.0f))
              .with_custom_text_color(amber)
              .with_alignment(TextAlignment::Center));

      div(context, mk(panel.ent(), 1),
          ComponentConfig{}
              .with_size(ComponentSize{children(), children()})
              .with_label("Hull integrity at 47%")
              .with_font_size(h720(12.0f))
              .with_custom_text_color(C{210, 180, 110, 255})
              .with_margin(Margin{.top = h720(4.0f)}));
    }

    // Demo 3: Small green status
    {
      auto panel = div(context, mk(col1.ent(), 3),
          ComponentConfig{}
              .with_size(ComponentSize{percent(0.88f), h720(55.0f)})
              .with_custom_background(C{12, 22, 16, 255})
              .with_border(C{30, 55, 35, 255}, 1.0f)
              .with_rounded_corners(RoundedCorners().all_sharp())
              .with_flex_direction(FlexDirection::Row)
              .with_align_items(AlignItems::Center)
              .with_justify_content(JustifyContent::Center)
              .with_padding(Spacing::sm)
              .with_margin(Margin{.top = h720(12.0f)})
              .with_debug_name("bracket_panel_3"))
          .decorate(with_brackets(context, green, 10.0f, 2.0f));

      // Status dot
      div(context, mk(panel.ent(), 0),
          ComponentConfig{}
              .with_size(ComponentSize{h720(8.0f), h720(8.0f)})
              .with_custom_background(green)
              .with_rounded_corners(RoundedCorners().all_round())
              .with_debug_name("status_dot"));

      div(context, mk(panel.ent(), 1),
          ComponentConfig{}
              .with_size(ComponentSize{children(), children()})
              .with_label("ONLINE")
              .with_font_size(h720(13.0f))
              .with_custom_text_color(green));
    }

    // Desc
    div(context, mk(col1.ent(), 4),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.88f), h720(18.0f)})
            .with_label("Sharp panels + L-shaped corner brackets")
            .with_font_size(h720(12.0f))
            .with_custom_text_color(theme.font_muted)
            .with_alignment(TextAlignment::Center)
            .with_margin(Margin{.top = h720(10.0f)}));

    // ====================================================================
    // COLUMN 2: Grid Background
    // ====================================================================
    auto col2 = div(context, mk(content.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.31f), percent(1.0f)})
            .with_flex_direction(FlexDirection::Column)
            .with_align_items(AlignItems::Center));

    div(context, mk(col2.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.9f), h720(24.0f)})
            .with_label("with_grid_bg()")
            .with_font_size(h720(14.0f))
            .with_custom_text_color(teal)
            .with_alignment(TextAlignment::Center)
            .with_margin(Margin{.bottom = h720(8.0f)}));

    // Demo 1: Radar-style grid
    {
      auto panel = div(context, mk(col2.ent(), 1),
          ComponentConfig{}
              .with_size(ComponentSize{percent(0.88f), h720(130.0f)})
              .with_custom_background(C{10, 18, 20, 255})
              .with_border(C{30, 65, 65, 255}, 1.0f)
              .with_rounded_corners(RoundedCorners().all_sharp()))
          .decorate(with_grid_bg(context, 20.0f,
                                 C{35, 75, 75, 180}, 1.0f));

      // Crosshair center marker
      div(context, mk(panel.ent(), 100),
          ComponentConfig{}
              .with_size(ComponentSize{h720(6.0f), h720(6.0f)})
              .with_absolute_position(
                  panel.cmp().computed[Axis::X] * 0.5f - 3.0f,
                  panel.cmp().computed[Axis::Y] * 0.5f - 3.0f)
              .with_custom_background(teal)
              .with_rounded_corners(RoundedCorners().all_round())
              .with_debug_name("radar_dot"));
    }

    // Demo 2: Dense teal grid
    {
      div(context, mk(col2.ent(), 2),
          ComponentConfig{}
              .with_size(ComponentSize{percent(0.88f), h720(95.0f)})
              .with_custom_background(C{8, 14, 16, 255})
              .with_border(teal, 1.0f)
              .with_rounded_corners(RoundedCorners().all_sharp())
              .with_margin(Margin{.top = h720(12.0f)})
              .with_debug_name("grid_panel_2"))
          .decorate(with_grid_bg(context, 12.0f,
                                 C{50, 130, 130, 120}, 1.0f));
    }

    // Demo 3: Grid + brackets combo
    {
      auto panel = div(context, mk(col2.ent(), 3),
          ComponentConfig{}
              .with_size(ComponentSize{percent(0.88f), h720(80.0f)})
              .with_custom_background(panel_bg)
              .with_border(panel_border, 1.0f)
              .with_rounded_corners(RoundedCorners().all_sharp())
              .with_flex_direction(FlexDirection::Column)
              .with_align_items(AlignItems::Center)
              .with_justify_content(JustifyContent::Center)
              .with_margin(Margin{.top = h720(12.0f)})
              .with_debug_name("combo_panel"))
          .decorate(with_grid_bg(context, 24.0f, C{40, 65, 70, 100}, 1.0f))
          .decorate(with_brackets(context, amber, 16.0f, 2.0f));

      div(context, mk(panel.ent(), 0),
          ComponentConfig{}
              .with_size(ComponentSize{children(), children()})
              .with_label("SECTOR 7-G")
              .with_font_size(h720(13.0f))
              .with_custom_text_color(amber));
    }

    // Desc
    div(context, mk(col2.ent(), 4),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.88f), h720(18.0f)})
            .with_label("Sharp panels + line grid overlays")
            .with_font_size(h720(12.0f))
            .with_custom_text_color(theme.font_muted)
            .with_alignment(TextAlignment::Center)
            .with_margin(Margin{.top = h720(10.0f)}));

    // ====================================================================
    // COLUMN 3: Quotes / Accent Bars
    // ====================================================================
    auto col3 = div(context, mk(content.ent(), 2),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.31f), percent(1.0f)})
            .with_flex_direction(FlexDirection::Column)
            .with_align_items(AlignItems::Center));

    div(context, mk(col3.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.9f), h720(24.0f)})
            .with_label("with_quote()")
            .with_font_size(h720(14.0f))
            .with_custom_text_color(teal)
            .with_alignment(TextAlignment::Center)
            .with_margin(Margin{.bottom = h720(8.0f)}));

    // Demo 1: Simple quote with teal accent
    {
      auto panel = div(context, mk(col3.ent(), 1),
          ComponentConfig{}
              .with_size(ComponentSize{percent(0.88f), h720(90.0f)})
              .with_custom_background(panel_bg)
              .with_border(panel_border, 1.0f)
              .with_rounded_corners(RoundedCorners().all_sharp())
              .with_flex_direction(FlexDirection::Row)
              .with_align_items(AlignItems::Stretch)
              .with_debug_name("quote_panel_1"))
          .decorate(with_quote(context,
              QuoteStyle{.accent_color = teal, .accent_width = 4.0f}));

      // Text content next to the accent bar
      div(context, mk(panel.ent(), 10),
          ComponentConfig{}
              .with_size(ComponentSize{percent(0.9f), percent(1.0f)})
              .with_label("The only way to do great work is to love what you do.")
              .with_font_size(h720(12.0f))
              .with_custom_text_color(theme.font)
              .with_alignment(TextAlignment::Left)
              .with_padding(Spacing::sm));
    }

    // Demo 2: Amber quote with attribution
    {
      auto panel = div(context, mk(col3.ent(), 2),
          ComponentConfig{}
              .with_size(ComponentSize{percent(0.88f), h720(100.0f)})
              .with_custom_background(C{22, 18, 12, 255})
              .with_border(C{50, 40, 25, 255}, 1.0f)
              .with_rounded_corners(RoundedCorners().all_sharp())
              .with_flex_direction(FlexDirection::Row)
              .with_align_items(AlignItems::Stretch)
              .with_margin(Margin{.top = h720(12.0f)})
              .with_debug_name("quote_panel_2"))
          .decorate(with_quote(context,
              QuoteStyle{.accent_color = amber,
                         .accent_width = 5.0f}));

      // Text column inside the quote
      auto text_col = div(context, mk(panel.ent(), 10),
          ComponentConfig{}
              .with_size(ComponentSize{percent(0.85f), percent(1.0f)})
              .with_flex_direction(FlexDirection::Column)
              .with_justify_content(JustifyContent::Center)
              .with_padding(Spacing::sm)
              .with_debug_name("quote_inner_2"));

      div(context, mk(text_col.ent(), 0),
          ComponentConfig{}
              .with_size(ComponentSize{percent(1.0f), children()})
              .with_label("I should go.")
              .with_font_size(h720(14.0f))
              .with_custom_text_color(theme.font)
              .with_alignment(TextAlignment::Left));

      div(context, mk(text_col.ent(), 1),
          ComponentConfig{}
              .with_size(ComponentSize{percent(1.0f), children()})
              .with_label("- Commander Shepard")
              .with_font_size(h720(12.0f))
              .with_custom_text_color(C{amber.r, amber.g, amber.b, 220})
              .with_alignment(TextAlignment::Left)
              .with_margin(Margin{.top = h720(6.0f)}));
    }

    // Demo 3: Green quote + brackets chained
    {
      auto panel = div(context, mk(col3.ent(), 3),
          ComponentConfig{}
              .with_size(ComponentSize{percent(0.88f), h720(100.0f)})
              .with_custom_background(C{12, 20, 14, 255})
              .with_border(C{30, 50, 32, 255}, 1.0f)
              .with_rounded_corners(RoundedCorners().all_sharp())
              .with_flex_direction(FlexDirection::Row)
              .with_align_items(AlignItems::Stretch)
              .with_margin(Margin{.top = h720(12.0f)})
              .with_debug_name("quote_panel_3"))
          .decorate(with_quote(context,
              QuoteStyle{.accent_color = green, .accent_width = 4.0f}))
          .decorate(with_brackets(context, green, 12.0f, 2.0f));

      auto text_col = div(context, mk(panel.ent(), 10),
          ComponentConfig{}
              .with_size(ComponentSize{percent(0.85f), percent(1.0f)})
              .with_flex_direction(FlexDirection::Column)
              .with_justify_content(JustifyContent::Center)
              .with_padding(Spacing::sm)
              .with_debug_name("quote_inner_3"));

      div(context, mk(text_col.ent(), 0),
          ComponentConfig{}
              .with_size(ComponentSize{percent(1.0f), children()})
              .with_label("The cake is a lie.")
              .with_font_size(h720(14.0f))
              .with_custom_text_color(theme.font)
              .with_alignment(TextAlignment::Left));

      div(context, mk(text_col.ent(), 1),
          ComponentConfig{}
              .with_size(ComponentSize{percent(1.0f), children()})
              .with_label("- GLaDOS")
              .with_font_size(h720(12.0f))
              .with_custom_text_color(C{green.r, green.g, green.b, 220})
              .with_alignment(TextAlignment::Left)
              .with_margin(Margin{.top = h720(6.0f)}));
    }

    // Desc
    div(context, mk(col3.ent(), 4),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.88f), h720(18.0f)})
            .with_label("Left accent bar + optional attribution")
            .with_font_size(h720(12.0f))
            .with_custom_text_color(theme.font_muted)
            .with_alignment(TextAlignment::Center)
            .with_margin(Margin{.top = h720(10.0f)}));
  }
};

REGISTER_EXAMPLE_SCREEN(decorators, "Component Galleries",
                        "UI decorator higher-order components",
                        DecoratorShowcase)
