#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

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

    int screen_width = Settings::get().get_screen_width();
    int screen_height = Settings::get().get_screen_height();

    // Scale factor based on screen height (base: 720p)
    float scale = static_cast<float>(screen_height) / 720.0f;

    // Responsive sizing using theme typography scale for visual hierarchy
    // Title: font_size_xl (42px) - Screen titles
    // Section headers: font_size_lg (32px) - Section headers
    // Labels: font_size_md (20px) - Labels
    // Body text: font_size_sm (16px) - Body text, values
    const float TITLE_HEIGHT = 60.0f * scale;
    const float TITLE_FONT = theme.font_size_xl() * scale;  // 42px - largest
    const float HEADER_HEIGHT = 40.0f * scale;
    const float HEADER_FONT = theme.font_size_lg() * 0.75f * scale;  // 24px - section headers
    const float CHECKBOX_HEIGHT = 40.0f * scale;
    const float CHECKBOX_FONT = theme.font_size_md() * scale;  // 20px - labels
    const float STATUS_HEIGHT = 44.0f * scale;
    const float STATUS_FONT = theme.font_size_sm() * scale;  // 16px - body text

    // Left: 3 headers + 6 rows (3 labeled + 1 no-label row + 2 disabled)
    // Right: 2 headers + 7 checkboxes
    float left_content = 3 * HEADER_HEIGHT + 6 * CHECKBOX_HEIGHT + 24.0f * scale;
    float right_content = 2 * HEADER_HEIGHT + 7 * CHECKBOX_HEIGHT + 24.0f * scale;
    float column_content_height = std::max(left_content, right_content);

    // Card sized to fit content tightly
    float card_content = TITLE_HEIGHT + column_content_height + STATUS_HEIGHT;
    float card_height = card_content + 8.0f * scale;
    float card_width = screen_width * 0.85f;

    // Root - full screen background
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_width), pixels(screen_height)})
            .with_background(Theme::Usage::Background)
            .with_debug_name("bg"));

    // Main card - centered
    float card_x = (screen_width - card_width) / 2.0f;
    float card_y = (screen_height - card_height) / 2.0f;

    auto card = div(context, mk(entity, 1),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(card_width), pixels(card_height)})
            .with_absolute_position()
            .with_translate(card_x, card_y)
            .with_background(Theme::Usage::Surface)
            .with_roundness(0.02f)
            .with_padding(Spacing::sm)
            .with_flex_direction(FlexDirection::Column)
            .with_debug_name("card"));

    // Title
    div(context, mk(card.ent(), 0),
        ComponentConfig{}
            .with_label("Checkbox Components")
            .with_size(ComponentSize{percent(1.0f), pixels(TITLE_HEIGHT)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, h720(TITLE_FONT))
            .with_alignment(TextAlignment::Center)
            .with_skip_tabbing(true)
            .with_debug_name("title"));

    // Content row - two columns
    auto content = div(context, mk(card.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(column_content_height)})
            .with_flex_direction(FlexDirection::Row)
            .with_justify_content(JustifyContent::SpaceAround)
            .with_debug_name("content"));

    // ========== LEFT COLUMN ==========
    auto left_col = div(context, mk(content.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.49f), percent(1.0f)})
            .with_custom_background(afterhours::colors::darken(theme.surface, 0.95f))
            .with_padding(Spacing::xs)
            .with_flex_direction(FlexDirection::Column)
            .with_justify_content(JustifyContent::FlexStart)
            .with_debug_name("left_col"));

    // With Label section - use Primary for consistent section headers
    div(context, mk(left_col.ent(), 0),
        ComponentConfig{}
            .with_label("With Label")
            .with_size(ComponentSize{percent(1.0f), pixels(HEADER_HEIGHT)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_padding(Spacing::xs)
            .with_font(UIComponent::DEFAULT_FONT, h720(HEADER_FONT))
            .with_skip_tabbing(true)
            .with_debug_name("with_label_hdr"));

    checkbox(context, mk(left_col.ent(), 1), labeled_primary,
             ComponentConfig{}
                 .with_label("Primary")
                 .with_size(ComponentSize{percent(1.0f), pixels(44)})
                 .with_background(Theme::Usage::Primary)
                 .with_checkbox_indicators("V", " ")
                 .with_font(UIComponent::DEFAULT_FONT, h720(CHECKBOX_FONT))
                 .with_debug_name("cb_primary"));

    checkbox(context, mk(left_col.ent(), 2), labeled_secondary,
             ComponentConfig{}
                 .with_label("Secondary")
                 .with_size(ComponentSize{percent(1.0f), pixels(44)})
                 .with_background(Theme::Usage::Secondary)
                 .with_checkbox_indicators("V", " ")
                 .with_font(UIComponent::DEFAULT_FONT, h720(CHECKBOX_FONT))
                 .with_debug_name("cb_secondary"));

    checkbox(context, mk(left_col.ent(), 3), labeled_accent,
             ComponentConfig{}
                 .with_label("Accent")
                 .with_size(ComponentSize{percent(1.0f), pixels(44)})
                 .with_background(Theme::Usage::Accent)
                 .with_checkbox_indicators("V", " ")
                 .with_font(UIComponent::DEFAULT_FONT, h720(CHECKBOX_FONT))
                 .with_debug_name("cb_accent"));

    // Box Only section - use Primary for consistent section headers
    div(context, mk(left_col.ent(), 4),
        ComponentConfig{}
            .with_label("Box Only")
            .with_size(ComponentSize{percent(1.0f), pixels(HEADER_HEIGHT)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_padding(Spacing::xs)
            .with_font(UIComponent::DEFAULT_FONT, h720(HEADER_FONT))
            .with_skip_tabbing(true)
            .with_debug_name("box_only_hdr"));

    auto no_label_row = div(context, mk(left_col.ent(), 5),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(44 + 8.0f * scale)})
            .with_flex_direction(FlexDirection::Row)
            .with_justify_content(JustifyContent::SpaceAround)
            .with_align_items(AlignItems::Center)
            .with_debug_name("no_label_row"));

    float box_size = 32.0f * scale;
    // Add aria-labels via debug_name for accessibility identification
    checkbox_no_label(context, mk(no_label_row.ent(), 0), no_label_1,
        ComponentConfig{}
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_background(Theme::Usage::Primary)
            .with_checkbox_indicators("V", " ")
            .with_font(UIComponent::SYMBOL_FONT, h720(20.0f * scale))
            .with_debug_name("nl_option_1"));

    checkbox_no_label(context, mk(no_label_row.ent(), 1), no_label_2,
        ComponentConfig{}
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_background(Theme::Usage::Secondary)
            .with_checkbox_indicators("V", " ")
            .with_font(UIComponent::SYMBOL_FONT, h720(20.0f * scale))
            .with_debug_name("nl_option_2"));

    checkbox_no_label(context, mk(no_label_row.ent(), 2), no_label_3,
        ComponentConfig{}
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_background(Theme::Usage::Accent)
            .with_checkbox_indicators("V", " ")
            .with_font(UIComponent::SYMBOL_FONT, h720(20.0f * scale))
            .with_debug_name("nl_option_3"));

    checkbox_no_label(context, mk(no_label_row.ent(), 3), no_label_4,
        ComponentConfig{}
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_background(Theme::Usage::Primary)
            .with_checkbox_indicators("V", " ")
            .with_font(UIComponent::SYMBOL_FONT, h720(20.0f * scale))
            .with_debug_name("nl_option_4"));

    // Disabled section - use Primary for consistent section headers
    div(context, mk(left_col.ent(), 6),
        ComponentConfig{}
            .with_label("Disabled")
            .with_size(ComponentSize{percent(1.0f), pixels(HEADER_HEIGHT)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_padding(Spacing::xs)
            .with_font(UIComponent::DEFAULT_FONT, h720(HEADER_FONT))
            .with_skip_tabbing(true)
            .with_debug_name("disabled_hdr"));

    // Use a darker background for disabled checkboxes to ensure the label text
    // maintains sufficient contrast (WCAG AA 4.5:1) after the framework applies
    // its 50% darken for disabled state. The auto_text_color picks cool white
    // (~{230,250,255}) which darkens to ~{115,125,127}. Against a darkened
    // surface-level background, this achieves ~4.9:1 contrast ratio.
    afterhours::Color disabled_bg = afterhours::colors::darken(theme.primary, 0.7f);

    checkbox(context, mk(left_col.ent(), 7), disabled_checked,
             ComponentConfig{}
                 .with_label("Disabled ON")
                 .with_size(ComponentSize{percent(1.0f), pixels(44)})
                 .with_custom_background(disabled_bg)
                 .with_auto_text_color(true)
                 .with_disabled(true)
                 .with_checkbox_indicators("V", " ")
                 .with_font(UIComponent::DEFAULT_FONT, h720(CHECKBOX_FONT))
                 .with_debug_name("cb_dis_on"));

    checkbox(context, mk(left_col.ent(), 8), disabled_unchecked,
             ComponentConfig{}
                 .with_label("Disabled OFF")
                 .with_size(ComponentSize{percent(1.0f), pixels(44)})
                 .with_custom_background(disabled_bg)
                 .with_auto_text_color(true)
                 .with_disabled(true)
                 .with_checkbox_indicators("V", " ")
                 .with_font(UIComponent::DEFAULT_FONT, h720(CHECKBOX_FONT))
                 .with_debug_name("cb_dis_off"));

    // ========== RIGHT COLUMN ==========
    auto right_col = div(context, mk(content.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.49f), percent(1.0f)})
            .with_custom_background(afterhours::colors::darken(theme.surface, 0.95f))
            .with_padding(Spacing::xs)
            .with_flex_direction(FlexDirection::Column)
            .with_justify_content(JustifyContent::FlexStart)
            .with_debug_name("right_col"));

    // Multi-Select section - use Primary for consistent section headers
    // Dynamic feedback showing current selection count
    int ms_count = static_cast<int>(options_group.count());
    std::string multi_label = "Multi-Select (Selected: " + std::to_string(ms_count) + " of 4)";
    div(context, mk(right_col.ent(), 0),
        ComponentConfig{}
            .with_label(multi_label)
            .with_size(ComponentSize{percent(1.0f), pixels(HEADER_HEIGHT)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_padding(Spacing::xs)
            .with_font(UIComponent::DEFAULT_FONT, h720(HEADER_FONT))
            .with_skip_tabbing(true)
            .with_debug_name("multi_hdr"));

    // Individual checkboxes for multi-select - 44px touch targets
    bool opt_a = options_group.test(0);
    bool opt_b = options_group.test(1);
    bool opt_c = options_group.test(2);
    bool opt_d = options_group.test(3);

    if (checkbox(context, mk(right_col.ent(), 1), opt_a,
             ComponentConfig{}
                 .with_label("Option A")
                 .with_size(ComponentSize{percent(1.0f), pixels(44)})
                 .with_background(Theme::Usage::Primary)
                 .with_checkbox_indicators("V", " ")
                 .with_font(UIComponent::DEFAULT_FONT, h720(CHECKBOX_FONT))
                 .with_debug_name("opt_a"))) {
        if (opt_a) options_group.set(0); else options_group.reset(0);
    }

    if (checkbox(context, mk(right_col.ent(), 2), opt_b,
             ComponentConfig{}
                 .with_label("Option B")
                 .with_size(ComponentSize{percent(1.0f), pixels(44)})
                 .with_background(Theme::Usage::Primary)
                 .with_checkbox_indicators("V", " ")
                 .with_font(UIComponent::DEFAULT_FONT, h720(CHECKBOX_FONT))
                 .with_debug_name("opt_b"))) {
        if (opt_b) options_group.set(1); else options_group.reset(1);
    }

    if (checkbox(context, mk(right_col.ent(), 3), opt_c,
             ComponentConfig{}
                 .with_label("Option C")
                 .with_size(ComponentSize{percent(1.0f), pixels(44)})
                 .with_background(Theme::Usage::Primary)
                 .with_checkbox_indicators("V", " ")
                 .with_font(UIComponent::DEFAULT_FONT, h720(CHECKBOX_FONT))
                 .with_debug_name("opt_c"))) {
        if (opt_c) options_group.set(2); else options_group.reset(2);
    }

    if (checkbox(context, mk(right_col.ent(), 4), opt_d,
             ComponentConfig{}
                 .with_label("Option D")
                 .with_size(ComponentSize{percent(1.0f), pixels(44)})
                 .with_background(Theme::Usage::Primary)
                 .with_checkbox_indicators("V", " ")
                 .with_font(UIComponent::DEFAULT_FONT, h720(CHECKBOX_FONT))
                 .with_debug_name("opt_d"))) {
        if (opt_d) options_group.set(3); else options_group.reset(3);
    }

    // Min/Max section - use Primary for consistent section headers
    // Dynamic feedback showing current selection count
    int mm_count = static_cast<int>(min_max_group.count());
    std::string minmax_label = "Select 1-2 options (" + std::to_string(mm_count) + " selected)";
    div(context, mk(right_col.ent(), 5),
        ComponentConfig{}
            .with_label(minmax_label)
            .with_size(ComponentSize{percent(1.0f), pixels(HEADER_HEIGHT)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_padding(Spacing::xs)
            .with_font(UIComponent::DEFAULT_FONT, h720(HEADER_FONT))
            .with_skip_tabbing(true)
            .with_debug_name("minmax_hdr"));

    // Individual checkboxes for min/max with constraints - 44px touch targets
    bool ch_1 = min_max_group.test(0);
    bool ch_2 = min_max_group.test(1);
    bool ch_3 = min_max_group.test(2);

    // Disable unchecking if at min (1), disable checking if at max (2)
    bool dis_1 = (ch_1 && mm_count <= 1) || (!ch_1 && mm_count >= 2);
    bool dis_2 = (ch_2 && mm_count <= 1) || (!ch_2 && mm_count >= 2);
    bool dis_3 = (ch_3 && mm_count <= 1) || (!ch_3 && mm_count >= 2);

    // Use darker background for disabled items to ensure sufficient label contrast
    afterhours::Color disabled_secondary_bg = afterhours::colors::darken(theme.secondary, 0.7f);

    if (checkbox(context, mk(right_col.ent(), 6), ch_1,
             ComponentConfig{}
                 .with_label("Choice 1")
                 .with_size(ComponentSize{percent(1.0f), pixels(44)})
                 .with_custom_background(dis_1 ? disabled_secondary_bg : theme.secondary)
                 .with_auto_text_color(true)
                 .with_checkbox_indicators("V", " ")
                 .with_font(UIComponent::DEFAULT_FONT, h720(CHECKBOX_FONT))
                 .with_disabled(dis_1)
                 .with_debug_name("ch_1"))) {
        if (ch_1) min_max_group.set(0); else min_max_group.reset(0);
    }

    if (checkbox(context, mk(right_col.ent(), 7), ch_2,
             ComponentConfig{}
                 .with_label("Choice 2")
                 .with_size(ComponentSize{percent(1.0f), pixels(44)})
                 .with_custom_background(dis_2 ? disabled_secondary_bg : theme.secondary)
                 .with_auto_text_color(true)
                 .with_checkbox_indicators("V", " ")
                 .with_font(UIComponent::DEFAULT_FONT, h720(CHECKBOX_FONT))
                 .with_disabled(dis_2)
                 .with_debug_name("ch_2"))) {
        if (ch_2) min_max_group.set(1); else min_max_group.reset(1);
    }

    if (checkbox(context, mk(right_col.ent(), 8), ch_3,
             ComponentConfig{}
                 .with_label("Choice 3")
                 .with_size(ComponentSize{percent(1.0f), pixels(44)})
                 .with_custom_background(dis_3 ? disabled_secondary_bg : theme.secondary)
                 .with_auto_text_color(true)
                 .with_checkbox_indicators("V", " ")
                 .with_font(UIComponent::DEFAULT_FONT, h720(CHECKBOX_FONT))
                 .with_disabled(dis_3)
                 .with_debug_name("ch_3"))) {
        if (ch_3) min_max_group.set(2); else min_max_group.reset(2);
    }

    // Status bar
    std::string status = "Selected: Group " + std::to_string(options_group.count()) + "/4";
    status += "  |  MinMax " + std::to_string(min_max_group.count()) + "/3";

    div(context, mk(card.ent(), 2),
        ComponentConfig{}
            .with_label(status)
            .with_size(ComponentSize{percent(1.0f), pixels(STATUS_HEIGHT)})
            .with_background(Theme::Usage::Surface)
            .with_auto_text_color(true)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, h720(STATUS_FONT))
            .with_alignment(TextAlignment::Center)
            .with_skip_tabbing(true)
            .with_debug_name("status"));
  }
};

REGISTER_EXAMPLE_SCREEN(checkboxes, "Component Galleries",
                        "All checkbox component types and variations",
                        CheckboxShowcase)
