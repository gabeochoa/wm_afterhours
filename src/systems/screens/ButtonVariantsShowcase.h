#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// --- Design system presets for this showcase ---

namespace bv_presets {

inline ComponentConfig RootConfig() {
  return ComponentConfig{}
      .with_size(ComponentSize{screen_pct(0.95f), screen_pct(0.95f)})
      .with_self_align(SelfAlign::Center)
      .with_background(Theme::Usage::Background)
      .with_padding(Spacing::md)
      .with_no_wrap();
}

inline ComponentConfig TitleConfig(const std::string &label) {
  return ComponentConfig{}
      .with_label(label)
      .with_size(ComponentSize{percent(1.0f), percent(0.07f)})
      .with_background(Theme::Usage::Surface)
      .with_auto_text_color(true)
      .with_padding(Spacing::xs)
      .with_font(UIComponent::DEFAULT_FONT, pixels(30.0f));
}

inline ComponentConfig RowConfig() {
  return ComponentConfig{}
      .with_size(ComponentSize{percent(1.0f), percent(0.16f)})
      .with_background(Theme::Usage::Surface)
      .with_padding(Spacing::xs)
      .with_align_items(AlignItems::Center);
}

inline ComponentConfig SectionLabelConfig(const std::string &label) {
  return ComponentConfig{}
      .with_label(label)
      .with_size(ComponentSize{pixels(120), percent(0.9f)})
      .with_background(Theme::Usage::Surface)
      .with_skip_tabbing(true)
      .with_font(UIComponent::DEFAULT_FONT, pixels(20.0f));
}

inline ComponentConfig ButtonConfig(const std::string &label,
                                    ButtonVariant variant = ButtonVariant::Filled) {
  return ComponentConfig{}
      .with_label(label)
      .with_size(ComponentSize{pixels(180), percent(0.85f)})
      .with_background(Theme::Usage::Primary)
      .with_auto_text_color(true)
      .with_button_variant(variant)
      .with_font(UIComponent::DEFAULT_FONT, pixels(20.0f))
      .with_margin(Spacing::xs);
}

inline ComponentConfig SmallButtonConfig(const std::string &label,
                                         ButtonVariant variant = ButtonVariant::Filled) {
  return ButtonConfig(label, variant)
      .with_size(ComponentSize{pixels(100), percent(0.6f)})
      .with_font(UIComponent::DEFAULT_FONT, pixels(14.0f))
      .with_padding(Spacing::xs);
}

inline ComponentConfig LargeButtonConfig(const std::string &label,
                                          ButtonVariant variant = ButtonVariant::Filled) {
  return ButtonConfig(label, variant)
      .with_size(ComponentSize{pixels(200), percent(0.9f)})
      .with_font(UIComponent::DEFAULT_FONT, pixels(24.0f));
}

inline ComponentConfig ThemeLabelConfig(const std::string &label) {
  return ComponentConfig{}
      .with_label(label)
      .with_size(ComponentSize{percent(0.95f), percent(0.2f)})
      .with_background(Theme::Usage::Background)
      .with_auto_text_color(true)
      .with_font(UIComponent::DEFAULT_FONT, pixels(18.0f))
      .with_skip_tabbing(true);
}

inline ComponentConfig ThemeColumnConfig() {
  return ComponentConfig{}
      .with_size(ComponentSize{pixels(200), percent(0.95f)})
      .with_background(Theme::Usage::Background)
      .with_padding(Spacing::xs)
      .with_align_items(AlignItems::Center)
      .with_margin(Spacing::xs);
}

inline ComponentConfig ThemeButtonConfig(const std::string &label,
                                         ButtonVariant variant = ButtonVariant::Filled) {
  return ButtonConfig(label, variant)
      .with_size(ComponentSize{percent(0.95f), percent(0.22f)})
      .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
      .with_padding(Spacing::xs)
      .with_margin(Margin{});
}

inline ComponentConfig StatusBarConfig(const std::string &label) {
  return ComponentConfig{}
      .with_label(label)
      .with_size(ComponentSize{percent(1.0f), percent(0.05f)})
      .with_auto_text_color(true)
      .with_padding(Spacing::sm)
      .with_rounded_corners(RoundedCorners())
      .with_roundness(0.1f)
      .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f));
}

} // namespace bv_presets

struct ButtonVariantsShowcase : ScreenSystem<UIContext<InputAction>> {
  int click_counts[16] = {0};

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = afterhours::ui::theme_presets::ocean_navy();
    context.theme = theme;
    context.scaling_mode = ScalingMode::Adaptive;

    using namespace bv_presets;

    auto root = vstack(context, mk(entity, 0),
                       RootConfig().with_debug_name("bv_root"));

    div(context, mk(root.ent(), 0), TitleConfig("Button Variants"));

    // --- Row 1: Filled variant ---
    auto row1 = hstack(context, mk(root.ent(), 1),
                       RowConfig().with_debug_name("bv_row_filled"));
    div(context, mk(row1.ent(), 0), SectionLabelConfig("Filled"));

    if (button(context, mk(row1.ent(), 1), ButtonConfig("Normal"))) {
      click_counts[0]++;
    }
    if (button(context, mk(row1.ent(), 2),
               ButtonConfig("Accent").with_background(Theme::Usage::Accent))) {
      click_counts[1]++;
    }
    if (button(context, mk(row1.ent(), 3),
               ButtonConfig("Secondary").with_background(Theme::Usage::Secondary))) {
      click_counts[2]++;
    }
    button(context, mk(row1.ent(), 4),
           ButtonConfig("Disabled").with_disabled(true));

    // --- Row 2: Outline variant ---
    auto row2 = hstack(context, mk(root.ent(), 2),
                       RowConfig().with_debug_name("bv_row_outline"));
    div(context, mk(row2.ent(), 0), SectionLabelConfig("Outline"));

    if (button(context, mk(row2.ent(), 1),
               ButtonConfig("Normal", ButtonVariant::Outline))) {
      click_counts[3]++;
    }
    if (button(context, mk(row2.ent(), 2),
               ButtonConfig("Accent", ButtonVariant::Outline)
                   .with_background(Theme::Usage::Accent))) {
      click_counts[4]++;
    }
    if (button(context, mk(row2.ent(), 3),
               ButtonConfig("Secondary", ButtonVariant::Outline)
                   .with_background(Theme::Usage::Secondary))) {
      click_counts[5]++;
    }
    button(context, mk(row2.ent(), 4),
           ButtonConfig("Disabled", ButtonVariant::Outline)
               .with_disabled(true));

    // --- Row 3: Ghost variant ---
    auto row3 = hstack(context, mk(root.ent(), 3),
                       RowConfig().with_debug_name("bv_row_ghost"));
    div(context, mk(row3.ent(), 0), SectionLabelConfig("Ghost"));

    if (button(context, mk(row3.ent(), 1),
               ButtonConfig("Normal", ButtonVariant::Ghost))) {
      click_counts[6]++;
    }
    if (button(context, mk(row3.ent(), 2),
               ButtonConfig("Accent", ButtonVariant::Ghost)
                   .with_background(Theme::Usage::Accent))) {
      click_counts[7]++;
    }
    if (button(context, mk(row3.ent(), 3),
               ButtonConfig("Secondary", ButtonVariant::Ghost)
                   .with_background(Theme::Usage::Secondary))) {
      click_counts[8]++;
    }
    button(context, mk(row3.ent(), 4),
           ButtonConfig("Disabled", ButtonVariant::Ghost)
               .with_disabled(true));

    // --- Row 4: Size variants ---
    auto row4 = hstack(context, mk(root.ent(), 4),
                       RowConfig().with_debug_name("bv_row_sizes"));
    div(context, mk(row4.ent(), 0), SectionLabelConfig("Sizes"));

    if (button(context, mk(row4.ent(), 1), SmallButtonConfig("Small"))) {
      click_counts[9]++;
    }
    if (button(context, mk(row4.ent(), 2), ButtonConfig("Medium"))) {
      click_counts[10]++;
    }
    if (button(context, mk(row4.ent(), 3), LargeButtonConfig("Large"))) {
      click_counts[11]++;
    }
    button(context, mk(row4.ent(), 4),
           LargeButtonConfig("Lg Outline", ButtonVariant::Outline)
               .with_background(Theme::Usage::Accent));

    // --- Row 5: Theme comparison ---
    auto row5 = hstack(
        context, mk(root.ent(), 5),
        RowConfig().with_size(ComponentSize{percent(1.0f), percent(0.24f)})
            .with_align_items(AlignItems::FlexStart)
            .with_debug_name("bv_row_themes"));
    div(context, mk(row5.ent(), 0), SectionLabelConfig("Themes"));

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
      auto demo = themes[t].theme;
      auto saved_theme = context.theme;
      context.theme = demo;

      auto col = vstack(context, mk(row5.ent(), 1 + t),
                        ThemeColumnConfig().with_debug_name(
                            fmt::format("bv_theme_col_{}", themes[t].name)));

      div(context, mk(col.ent(), 0), ThemeLabelConfig(themes[t].name));

      button(context, mk(col.ent(), 1), ThemeButtonConfig("Filled"));
      button(context, mk(col.ent(), 2),
             ThemeButtonConfig("Outline", ButtonVariant::Outline));
      button(context, mk(col.ent(), 3),
             ThemeButtonConfig("Ghost", ButtonVariant::Ghost));

      context.theme = saved_theme;
    }

    // --- Click counter ---
    int total = 0;
    for (int i = 0; i < 16; i++)
      total += click_counts[i];

    div(context, mk(root.ent(), 6),
        StatusBarConfig("Total clicks across all variants: " +
                        std::to_string(total))
            .with_custom_background(
                afterhours::colors::lighten(context.theme.background, 0.08f)));
  }
};

REGISTER_EXAMPLE_SCREEN(button_variants, "Component Galleries",
                        "Button variants: Filled, Outline, Ghost with sizes "
                        "and themes",
                        ButtonVariantsShowcase)
