#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ButtonVariantsShowcase : ScreenSystem<UIContext<InputAction>> {
  int click_counts[16] = {0};

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = afterhours::ui::theme_presets::ocean_navy();
    context.theme = theme;

    const auto FONT = UIComponent::DEFAULT_FONT;

    // Main container
    auto root =
        div(context, mk(entity, 0),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(0.95f), screen_pct(0.95f)})
                .with_self_align(SelfAlign::Center)
                .with_background(Theme::Usage::Background)
                .with_padding(Spacing::md)
                .with_flex_direction(FlexDirection::Column)
                .with_no_wrap()
                .with_debug_name("bv_root"));

    // Title
    div(context, mk(root.ent(), 0),
        ComponentConfig{}
            .with_label("Button Variants")
            .with_size(ComponentSize{percent(1.0f), percent(0.07f)})
            .with_background(Theme::Usage::Surface)
            .with_auto_text_color(true)
            .with_padding(Spacing::xs)
            .with_font(FONT, h720(30.0f)));

    // Helper lambda for row labels
    auto row_label = [&](afterhours::Entity &parent, int id, const char *text) {
      div(context, mk(parent, id),
          ComponentConfig{}
              .with_label(text)
              .with_size(ComponentSize{w1280(120), percent(0.9f)})
              .with_background(Theme::Usage::Surface)
              .with_skip_tabbing(true)
              .with_font(FONT, h720(20.0f))
              .with_debug_name(fmt::format("bv_label_{}", text)));
    };

    // Common button size
    auto btn_size = ComponentSize{w1280(180), percent(0.85f)};
    auto btn_font = h720(20.0f);

    // --- Row 1: Filled variant ---
    auto row1 = div(context, mk(root.ent(), 1),
                    ComponentConfig{}
                        .with_size(ComponentSize{percent(1.0f), percent(0.16f)})
                        .with_background(Theme::Usage::Surface)
                        .with_padding(Spacing::xs)
                        .with_flex_direction(FlexDirection::Row)
                        .with_align_items(AlignItems::Center)
                        .with_debug_name("bv_row_filled"));

    row_label(row1.ent(), 0, "Filled");

    if (button(context, mk(row1.ent(), 1),
               ComponentConfig{}
                   .with_label("Normal")
                   .with_size(btn_size)
                   .with_background(Theme::Usage::Primary)
                   .with_auto_text_color(true)
                   .with_button_variant(ButtonVariant::Filled)
                   .with_font(FONT, btn_font)
                   .with_margin(Spacing::xs)
                   .with_debug_name("bv_filled_normal"))) {
      click_counts[0]++;
    }

    if (button(context, mk(row1.ent(), 2),
               ComponentConfig{}
                   .with_label("Accent")
                   .with_size(btn_size)
                   .with_background(Theme::Usage::Accent)
                   .with_auto_text_color(true)
                   .with_button_variant(ButtonVariant::Filled)
                   .with_font(FONT, btn_font)
                   .with_margin(Spacing::xs)
                   .with_debug_name("bv_filled_accent"))) {
      click_counts[1]++;
    }

    if (button(context, mk(row1.ent(), 3),
               ComponentConfig{}
                   .with_label("Secondary")
                   .with_size(btn_size)
                   .with_background(Theme::Usage::Secondary)
                   .with_auto_text_color(true)
                   .with_button_variant(ButtonVariant::Filled)
                   .with_font(FONT, btn_font)
                   .with_margin(Spacing::xs)
                   .with_debug_name("bv_filled_secondary"))) {
      click_counts[2]++;
    }

    button(context, mk(row1.ent(), 4),
           ComponentConfig{}
               .with_label("Disabled")
               .with_size(btn_size)
               .with_background(Theme::Usage::Primary)
               .with_auto_text_color(true)
               .with_button_variant(ButtonVariant::Filled)
               .with_disabled(true)
               .with_font(FONT, btn_font)
               .with_margin(Spacing::xs));

    // --- Row 2: Outline variant ---
    auto row2 = div(context, mk(root.ent(), 2),
                    ComponentConfig{}
                        .with_size(ComponentSize{percent(1.0f), percent(0.16f)})
                        .with_background(Theme::Usage::Surface)
                        .with_padding(Spacing::xs)
                        .with_flex_direction(FlexDirection::Row)
                        .with_align_items(AlignItems::Center)
                        .with_debug_name("bv_row_outline"));

    row_label(row2.ent(), 0, "Outline");

    if (button(context, mk(row2.ent(), 1),
               ComponentConfig{}
                   .with_label("Normal")
                   .with_size(btn_size)
                   .with_background(Theme::Usage::Primary)
                   .with_button_variant(ButtonVariant::Outline)
                   .with_font(FONT, btn_font)
                   .with_margin(Spacing::xs)
                   .with_debug_name("bv_outline_normal"))) {
      click_counts[3]++;
    }

    if (button(context, mk(row2.ent(), 2),
               ComponentConfig{}
                   .with_label("Accent")
                   .with_size(btn_size)
                   .with_background(Theme::Usage::Accent)
                   .with_button_variant(ButtonVariant::Outline)
                   .with_font(FONT, btn_font)
                   .with_margin(Spacing::xs)
                   .with_debug_name("bv_outline_accent"))) {
      click_counts[4]++;
    }

    if (button(context, mk(row2.ent(), 3),
               ComponentConfig{}
                   .with_label("Secondary")
                   .with_size(btn_size)
                   .with_background(Theme::Usage::Secondary)
                   .with_button_variant(ButtonVariant::Outline)
                   .with_font(FONT, btn_font)
                   .with_margin(Spacing::xs)
                   .with_debug_name("bv_outline_secondary"))) {
      click_counts[5]++;
    }

    button(context, mk(row2.ent(), 4),
           ComponentConfig{}
               .with_label("Disabled")
               .with_size(btn_size)
               .with_background(Theme::Usage::Primary)
               .with_button_variant(ButtonVariant::Outline)
               .with_disabled(true)
               .with_font(FONT, btn_font)
               .with_margin(Spacing::xs));

    // --- Row 3: Ghost variant ---
    auto row3 = div(context, mk(root.ent(), 3),
                    ComponentConfig{}
                        .with_size(ComponentSize{percent(1.0f), percent(0.16f)})
                        .with_background(Theme::Usage::Surface)
                        .with_padding(Spacing::xs)
                        .with_flex_direction(FlexDirection::Row)
                        .with_align_items(AlignItems::Center)
                        .with_debug_name("bv_row_ghost"));

    row_label(row3.ent(), 0, "Ghost");

    if (button(context, mk(row3.ent(), 1),
               ComponentConfig{}
                   .with_label("Normal")
                   .with_size(btn_size)
                   .with_background(Theme::Usage::Primary)
                   .with_button_variant(ButtonVariant::Ghost)
                   .with_font(FONT, btn_font)
                   .with_margin(Spacing::xs)
                   .with_debug_name("bv_ghost_normal"))) {
      click_counts[6]++;
    }

    if (button(context, mk(row3.ent(), 2),
               ComponentConfig{}
                   .with_label("Accent")
                   .with_size(btn_size)
                   .with_background(Theme::Usage::Accent)
                   .with_button_variant(ButtonVariant::Ghost)
                   .with_font(FONT, btn_font)
                   .with_margin(Spacing::xs)
                   .with_debug_name("bv_ghost_accent"))) {
      click_counts[7]++;
    }

    if (button(context, mk(row3.ent(), 3),
               ComponentConfig{}
                   .with_label("Secondary")
                   .with_size(btn_size)
                   .with_background(Theme::Usage::Secondary)
                   .with_button_variant(ButtonVariant::Ghost)
                   .with_font(FONT, btn_font)
                   .with_margin(Spacing::xs)
                   .with_debug_name("bv_ghost_secondary"))) {
      click_counts[8]++;
    }

    button(context, mk(row3.ent(), 4),
           ComponentConfig{}
               .with_label("Disabled")
               .with_size(btn_size)
               .with_background(Theme::Usage::Primary)
               .with_button_variant(ButtonVariant::Ghost)
               .with_disabled(true)
               .with_font(FONT, btn_font)
               .with_margin(Spacing::xs));

    // --- Row 4: Size variants ---
    auto row4 = div(context, mk(root.ent(), 4),
                    ComponentConfig{}
                        .with_size(ComponentSize{percent(1.0f), percent(0.16f)})
                        .with_background(Theme::Usage::Surface)
                        .with_padding(Spacing::xs)
                        .with_flex_direction(FlexDirection::Row)
                        .with_align_items(AlignItems::Center)
                        .with_debug_name("bv_row_sizes"));

    row_label(row4.ent(), 0, "Sizes");

    // Small
    if (button(context, mk(row4.ent(), 1),
               ComponentConfig{}
                   .with_label("Small")
                   .with_size(ComponentSize{w1280(100), percent(0.6f)})
                   .with_background(Theme::Usage::Primary)
                   .with_auto_text_color(true)
                   .with_font(FONT, h720(14.0f))
                   .with_padding(Spacing::xs)
                   .with_margin(Spacing::xs)
                   .with_debug_name("bv_size_small"))) {
      click_counts[9]++;
    }

    // Medium (default)
    if (button(context, mk(row4.ent(), 2),
               ComponentConfig{}
                   .with_label("Medium")
                   .with_size(btn_size)
                   .with_background(Theme::Usage::Primary)
                   .with_auto_text_color(true)
                   .with_font(FONT, btn_font)
                   .with_margin(Spacing::xs)
                   .with_debug_name("bv_size_medium"))) {
      click_counts[10]++;
    }

    // Large
    if (button(context, mk(row4.ent(), 3),
               ComponentConfig{}
                   .with_label("Large")
                   .with_size(ComponentSize{w1280(200), percent(0.9f)})
                   .with_background(Theme::Usage::Primary)
                   .with_auto_text_color(true)
                   .with_font(FONT, h720(24.0f))
                   .with_margin(Spacing::xs)
                   .with_debug_name("bv_size_large"))) {
      click_counts[11]++;
    }

    // Large Outline
    button(context, mk(row4.ent(), 4),
           ComponentConfig{}
               .with_label("Lg Outline")
               .with_size(ComponentSize{w1280(180), percent(0.9f)})
               .with_background(Theme::Usage::Accent)
               .with_button_variant(ButtonVariant::Outline)
               .with_font(FONT, h720(24.0f))
               .with_margin(Spacing::xs));

    // --- Row 5: Theme comparison ---
    auto row5 = div(context, mk(root.ent(), 5),
                    ComponentConfig{}
                        .with_size(ComponentSize{percent(1.0f), percent(0.24f)})
                        .with_background(Theme::Usage::Surface)
                        .with_padding(Spacing::xs)
                        .with_flex_direction(FlexDirection::Row)
                        .with_align_items(AlignItems::FlexStart)
                        .with_debug_name("bv_row_themes"));

    row_label(row5.ent(), 0, "Themes");

    // Theme comparison columns
    struct ThemeDemo {
      const char *name;
      afterhours::ui::Theme theme;
    };
    ThemeDemo themes[] = {
        {"Neon Dark", afterhours::ui::theme_presets::neon_dark()},
        {"Cozy Kraft", afterhours::ui::theme_presets::cozy_kraft()},
        {"Sage", afterhours::ui::theme_presets::sage_natural()},
    };

    for (int t = 0; t < 3; t++) {
      // Capture actual color values so they survive theme restoration
      auto demo = themes[t].theme;
      auto saved_theme = context.theme;
      context.theme = demo;

      auto col =
          div(context, mk(row5.ent(), 1 + t),
              ComponentConfig{}
                  .with_size(ComponentSize{w1280(200), percent(0.95f)})
                  .with_custom_background(demo.background)
                  .with_padding(Spacing::xs)
                  .with_flex_direction(FlexDirection::Column)
                  .with_align_items(AlignItems::Center)
                  .with_margin(Spacing::xs)
                  .with_debug_name(
                      fmt::format("bv_theme_col_{}", themes[t].name)));

      // Auto-pick the best text color for this theme's primary bg
      afterhours::Color filled_text =
          afterhours::colors::auto_text_color(demo.primary, demo.font,
                                              demo.darkfont);

      // Theme name label
      div(context, mk(col.ent(), 0),
          ComponentConfig{}
              .with_label(themes[t].name)
              .with_size(ComponentSize{percent(0.95f), percent(0.2f)})
              .with_custom_background(demo.background)
              .with_auto_text_color(false)
              .with_custom_text_color(demo.font)
              .with_font(FONT, h720(18.0f))
              .with_skip_tabbing(true)
              .with_debug_name(
                  fmt::format("bv_theme_name_{}", themes[t].name)));

      // Filled
      button(context, mk(col.ent(), 1),
             ComponentConfig{}
                 .with_label("Filled")
                 .with_size(ComponentSize{percent(0.95f), percent(0.22f)})
                 .with_custom_background(demo.primary)
                 .with_auto_text_color(false)
                 .with_custom_text_color(filled_text)
                 .with_font(FONT, h720(16.0f))
                 .with_padding(Spacing::xs)
                 .with_debug_name(
                     fmt::format("bv_theme_filled_{}", themes[t].name)));

      // Outline - bg matches column so validator sees correct contrast
      button(context, mk(col.ent(), 2),
             ComponentConfig{}
                 .with_label("Outline")
                 .with_size(ComponentSize{percent(0.95f), percent(0.22f)})
                 .with_custom_background(demo.background)
                 .with_auto_text_color(false)
                 .with_custom_text_color(demo.font)
                 .with_border(demo.primary, h720(2.0f))
                 .with_font(FONT, h720(16.0f))
                 .with_padding(Spacing::xs)
                 .with_debug_name(
                     fmt::format("bv_theme_outline_{}", themes[t].name)));

      // Ghost - bg matches column so validator sees correct contrast
      button(context, mk(col.ent(), 3),
             ComponentConfig{}
                 .with_label("Ghost")
                 .with_size(ComponentSize{percent(0.95f), percent(0.22f)})
                 .with_custom_background(demo.background)
                 .with_auto_text_color(false)
                 .with_custom_text_color(demo.font)
                 .with_font(FONT, h720(16.0f))
                 .with_padding(Spacing::xs)
                 .with_debug_name(
                     fmt::format("bv_theme_ghost_{}", themes[t].name)));

      context.theme = saved_theme;
    }

    // --- Click counter ---
    int total = 0;
    for (int i = 0; i < 16; i++)
      total += click_counts[i];

    div(context, mk(root.ent(), 6),
        ComponentConfig{}
            .with_label("Total clicks across all variants: " + std::to_string(total))
            .with_size(ComponentSize{percent(1.0f), percent(0.05f)})
            .with_custom_background(afterhours::colors::lighten(context.theme.background, 0.08f))
            .with_auto_text_color(true)
            .with_padding(Spacing::sm)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.1f)
            .with_font(FONT, h720(16.0f)));
  }
};

REGISTER_EXAMPLE_SCREEN(button_variants, "Component Galleries",
                        "Button variants: Filled, Outline, Ghost with sizes "
                        "and themes",
                        ButtonVariantsShowcase)
