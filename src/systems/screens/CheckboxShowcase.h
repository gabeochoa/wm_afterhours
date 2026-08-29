#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

namespace cs_presets {

inline ComponentConfig SectionHeaderConfig(const std::string &label,
                                           Size height, Size font_size) {
  return ComponentConfig{}
      .with_label(label)
      .with_size(ComponentSize{percent(1.0f), height})
      .with_background(Theme::Usage::Primary)
      .with_auto_text_color(true)
      .with_padding(Spacing::xs)
      .with_font(UIComponent::DEFAULT_FONT, font_size)
      .with_skip_tabbing(true);
}

inline ComponentConfig CheckboxConfig(const std::string &label,
                                      Size font_size,
                                      Theme::Usage bg = Theme::Usage::Primary) {
  return ComponentConfig{}
      .with_label(label)
      .with_size(ComponentSize{percent(1.0f), h720(44)})
      .with_background(bg)
      .with_checkbox_indicators("V", " ")
      .with_font(UIComponent::DEFAULT_FONT, font_size);
}

} // namespace cs_presets

struct CheckboxShowcase : ScreenSystem<UIContext<InputAction>> {
  // Checkbox with label values
  bool labeled_primary = true;
  bool labeled_secondary = false;
  bool labeled_accent = true;

  // No-label checkbox values
  bool no_label_1 = true;
  bool no_label_2 = false;
  bool no_label_3 = true;
  bool no_label_4 = false;

  // Disabled checkbox values
  bool disabled_checked = true;
  bool disabled_unchecked = false;

  // Checkbox group values
  std::bitset<4> options_group{0b0101};
  std::bitset<3> min_max_group{0b001};

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = afterhours::ui::theme_presets::neon_dark();
    context.theme = theme;
    context.scaling_mode = ScalingMode::Adaptive;

    // Responsive sizing using theme typography scale for visual hierarchy
    const Size TITLE_HEIGHT = h720(60.0f);
    const Size TITLE_FONT = h720(theme.font_size_xl());
    const Size HEADER_HEIGHT = h720(40.0f);
    const Size HEADER_FONT = h720(theme.font_size_lg() * 0.75f);
    const Size CHECKBOX_FONT = h720(theme.font_size_md());
    const Size STATUS_HEIGHT = h720(44.0f);
    const Size STATUS_FONT = h720(theme.font_size_sm());

    using namespace cs_presets;

    // Root - full screen centering container
    auto root = vstack(
        context, mk(entity, 0),
        ComponentConfig{}
            .with_size(
                ComponentSize{screen_pct(1.0f), screen_pct(1.0f)})
            .with_background(Theme::Usage::Background)
            .with_justify_content(JustifyContent::Center)
            .with_align_items(AlignItems::Center)
            .with_debug_name("bg"));

    // Main card - centered via parent flexbox
    auto card = vstack(
        context, mk(root.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(0.85f), screen_pct(0.88f)})
            .with_background(Theme::Usage::Surface)
            .with_roundness(0.02f)
            .with_padding(Spacing::sm)
            .with_debug_name("card"));

    // Title
    div(context, mk(card.ent(), 0),
        ComponentConfig{}
            .with_label("Checkbox Components")
            .with_size(ComponentSize{percent(1.0f), TITLE_HEIGHT})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, TITLE_FONT)
            .with_alignment(TextAlignment::Center)
            .with_skip_tabbing(true));

    // Content row - two columns
    auto content = hstack(context, mk(card.ent(), 1),
                          ComponentConfig{}
                              .with_size(ComponentSize{
                                  percent(1.0f), children()})
                              .with_justify_content(JustifyContent::SpaceAround)
                              .with_debug_name("content"));

    // ========== LEFT COLUMN ==========
    auto left_col =
        vstack(context, mk(content.ent(), 0),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(0.49f), children()})
                   .with_custom_background(
                       afterhours::colors::darken(theme.surface, 0.95f))
                   .with_padding(Spacing::xs)
                   .with_justify_content(JustifyContent::FlexStart)
                   .with_debug_name("left_col"));

    // With Label section - use Primary for consistent section headers
    div(context, mk(left_col.ent(), 0),
        SectionHeaderConfig("With Label", HEADER_HEIGHT, HEADER_FONT));

    checkbox(context, mk(left_col.ent(), 1), labeled_primary,
             CheckboxConfig("Primary", CHECKBOX_FONT));
    checkbox(context, mk(left_col.ent(), 2), labeled_secondary,
             CheckboxConfig("Secondary", CHECKBOX_FONT,
                            Theme::Usage::Secondary));
    checkbox(context, mk(left_col.ent(), 3), labeled_accent,
             CheckboxConfig("Accent", CHECKBOX_FONT, Theme::Usage::Accent));

    // Box Only section - use Primary for consistent section headers
    div(context, mk(left_col.ent(), 4),
        SectionHeaderConfig("Box Only", HEADER_HEIGHT, HEADER_FONT));

    auto no_label_row = hstack(
        context, mk(left_col.ent(), 5),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), h720(52.0f)})
            .with_justify_content(JustifyContent::SpaceAround)
            .with_align_items(AlignItems::Center));

    Size box_size = h720(32.0f);
    Size box_font = h720(20.0f);
    // Add aria-labels via debug_name for accessibility identification
    primitive::toggle_button(
        context, mk(no_label_row.ent(), 0),
        ComponentConfig{}
            .with_label(no_label_1 ? "V" : " ")
            .with_size(ComponentSize{box_size, box_size})
            .with_background(Theme::Usage::Primary)
            .with_font(UIComponent::SYMBOL_FONT, box_font)
            .with_auto_text_color(true)
            .with_debug_name("nl_option_1"),
        no_label_1);

    primitive::toggle_button(
        context, mk(no_label_row.ent(), 1),
        ComponentConfig{}
            .with_label(no_label_2 ? "V" : " ")
            .with_size(ComponentSize{box_size, box_size})
            .with_background(Theme::Usage::Secondary)
            .with_font(UIComponent::SYMBOL_FONT, box_font)
            .with_auto_text_color(true)
            .with_debug_name("nl_option_2"),
        no_label_2);

    primitive::toggle_button(
        context, mk(no_label_row.ent(), 2),
        ComponentConfig{}
            .with_label(no_label_3 ? "V" : " ")
            .with_size(ComponentSize{box_size, box_size})
            .with_background(Theme::Usage::Accent)
            .with_font(UIComponent::SYMBOL_FONT, box_font)
            .with_auto_text_color(true)
            .with_debug_name("nl_option_3"),
        no_label_3);

    primitive::toggle_button(
        context, mk(no_label_row.ent(), 3),
        ComponentConfig{}
            .with_label(no_label_4 ? "V" : " ")
            .with_size(ComponentSize{box_size, box_size})
            .with_background(Theme::Usage::Primary)
            .with_font(UIComponent::SYMBOL_FONT, box_font)
            .with_auto_text_color(true)
            .with_debug_name("nl_option_4"),
        no_label_4);

    // Disabled section - use Primary for consistent section headers
    div(context, mk(left_col.ent(), 6),
        SectionHeaderConfig("Disabled", HEADER_HEIGHT, HEADER_FONT));

    // Use a darker background for disabled checkboxes to ensure the label text
    // maintains sufficient contrast (WCAG AA 4.5:1) after the framework applies
    // its 50% darken for disabled state. The auto_text_color picks cool white
    // (~{230,250,255}) which darkens to ~{115,125,127}. Against a darkened
    // surface-level background, this achieves ~4.9:1 contrast ratio.
    afterhours::Color disabled_bg =
        afterhours::colors::darken(theme.primary, 0.7f);

    checkbox(context, mk(left_col.ent(), 7), disabled_checked,
             ComponentConfig{}
                 .with_label("Disabled ON")
                 .with_size(ComponentSize{percent(1.0f), h720(44)})
                 .with_custom_background(disabled_bg)
                 .with_auto_text_color(true)
                 .with_disabled(true)
                 .with_checkbox_indicators("V", " ")
                 .with_font(UIComponent::DEFAULT_FONT, CHECKBOX_FONT));

    checkbox(context, mk(left_col.ent(), 8), disabled_unchecked,
             ComponentConfig{}
                 .with_label("Disabled OFF")
                 .with_size(ComponentSize{percent(1.0f), h720(44)})
                 .with_custom_background(disabled_bg)
                 .with_auto_text_color(true)
                 .with_disabled(true)
                 .with_checkbox_indicators("V", " ")
                 .with_font(UIComponent::DEFAULT_FONT, CHECKBOX_FONT));

    // ========== RIGHT COLUMN ==========
    auto right_col =
        vstack(context, mk(content.ent(), 1),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(0.49f), children()})
                   .with_custom_background(
                       afterhours::colors::darken(theme.surface, 0.95f))
                   .with_padding(Spacing::xs)
                   .with_justify_content(JustifyContent::FlexStart)
                   .with_debug_name("right_col"));

    // Multi-Select section - use Primary for consistent section headers
    // Dynamic feedback showing current selection count
    int ms_count = static_cast<int>(options_group.count());
    std::string multi_label =
        "Multi-Select (Selected: " + std::to_string(ms_count) + " of 4)";
    div(context, mk(right_col.ent(), 0),
        SectionHeaderConfig(multi_label, HEADER_HEIGHT, HEADER_FONT));

    // Individual checkboxes for multi-select - 44px touch targets
    bool opt_a = options_group.test(0);
    bool opt_b = options_group.test(1);
    bool opt_c = options_group.test(2);
    bool opt_d = options_group.test(3);

    if (checkbox(context, mk(right_col.ent(), 1), opt_a,
                 CheckboxConfig("Option A", CHECKBOX_FONT)
                     .with_debug_name("opt_a"))) {
      if (opt_a)
        options_group.set(0);
      else
        options_group.reset(0);
    }

    if (checkbox(context, mk(right_col.ent(), 2), opt_b,
                 CheckboxConfig("Option B", CHECKBOX_FONT)
                     .with_debug_name("opt_b"))) {
      if (opt_b)
        options_group.set(1);
      else
        options_group.reset(1);
    }

    if (checkbox(context, mk(right_col.ent(), 3), opt_c,
                 CheckboxConfig("Option C", CHECKBOX_FONT)
                     .with_debug_name("opt_c"))) {
      if (opt_c)
        options_group.set(2);
      else
        options_group.reset(2);
    }

    if (checkbox(context, mk(right_col.ent(), 4), opt_d,
                 CheckboxConfig("Option D", CHECKBOX_FONT)
                     .with_debug_name("opt_d"))) {
      if (opt_d)
        options_group.set(3);
      else
        options_group.reset(3);
    }

    // Min/Max section - use Primary for consistent section headers
    // Dynamic feedback showing current selection count
    int mm_count = static_cast<int>(min_max_group.count());
    std::string minmax_label =
        "Select 1-2 options (" + std::to_string(mm_count) + " selected)";
    div(context, mk(right_col.ent(), 5),
        SectionHeaderConfig(minmax_label, HEADER_HEIGHT, HEADER_FONT));

    // Individual checkboxes for min/max with constraints - 44px touch targets
    bool ch_1 = min_max_group.test(0);
    bool ch_2 = min_max_group.test(1);
    bool ch_3 = min_max_group.test(2);

    // Disable unchecking if at min (1), disable checking if at max (2)
    bool dis_1 = (ch_1 && mm_count <= 1) || (!ch_1 && mm_count >= 2);
    bool dis_2 = (ch_2 && mm_count <= 1) || (!ch_2 && mm_count >= 2);
    bool dis_3 = (ch_3 && mm_count <= 1) || (!ch_3 && mm_count >= 2);

    // Use darker background for disabled items to ensure sufficient label
    // contrast
    afterhours::Color disabled_secondary_bg =
        afterhours::colors::darken(theme.secondary, 0.7f);

    if (checkbox(
            context, mk(right_col.ent(), 6), ch_1,
            ComponentConfig{}
                .with_label("Choice 1")
                .with_size(ComponentSize{percent(1.0f), h720(44)})
                .with_custom_background(dis_1 ? disabled_secondary_bg
                                              : theme.secondary)
                .with_auto_text_color(true)
                .with_checkbox_indicators("V", " ")
                .with_font(UIComponent::DEFAULT_FONT, CHECKBOX_FONT)
                .with_disabled(dis_1)
                .with_debug_name("ch_1"))) {
      if (ch_1)
        min_max_group.set(0);
      else
        min_max_group.reset(0);
    }

    if (checkbox(
            context, mk(right_col.ent(), 7), ch_2,
            ComponentConfig{}
                .with_label("Choice 2")
                .with_size(ComponentSize{percent(1.0f), h720(44)})
                .with_custom_background(dis_2 ? disabled_secondary_bg
                                              : theme.secondary)
                .with_auto_text_color(true)
                .with_checkbox_indicators("V", " ")
                .with_font(UIComponent::DEFAULT_FONT, CHECKBOX_FONT)
                .with_disabled(dis_2)
                .with_debug_name("ch_2"))) {
      if (ch_2)
        min_max_group.set(1);
      else
        min_max_group.reset(1);
    }

    if (checkbox(
            context, mk(right_col.ent(), 8), ch_3,
            ComponentConfig{}
                .with_label("Choice 3")
                .with_size(ComponentSize{percent(1.0f), h720(44)})
                .with_custom_background(dis_3 ? disabled_secondary_bg
                                              : theme.secondary)
                .with_auto_text_color(true)
                .with_checkbox_indicators("V", " ")
                .with_font(UIComponent::DEFAULT_FONT, CHECKBOX_FONT)
                .with_disabled(dis_3)
                .with_debug_name("ch_3"))) {
      if (ch_3)
        min_max_group.set(2);
      else
        min_max_group.reset(2);
    }

    // Status bar
    std::string status =
        "Group selected: " + std::to_string(options_group.count()) + " of 4";
    status += "   MinMax selected: " + std::to_string(min_max_group.count()) +
              " of 3";

    div(context, mk(card.ent(), 2),
        ComponentConfig{}
            .with_label(status)
            .with_size(ComponentSize{percent(1.0f), STATUS_HEIGHT})
            .with_background(Theme::Usage::Surface)
            .with_auto_text_color(true)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, STATUS_FONT)
            .with_alignment(TextAlignment::Center)
            .with_skip_tabbing(true));
  }
};

REGISTER_EXAMPLE_SCREEN(checkboxes, "Component Galleries",
                        "All checkbox component types and variations",
                        CheckboxShowcase)
