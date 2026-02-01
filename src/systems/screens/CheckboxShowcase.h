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

    // Responsive sizing
    const float HEADER_HEIGHT = 28.0f * scale;
    const float HEADER_FONT = 14.0f * scale;
    const float CHECKBOX_HEIGHT = 36.0f * scale;
    const float CHECKBOX_FONT = 13.0f * scale;
    const float GROUP_ROW_HEIGHT = 32.0f * scale;
    const float TITLE_HEIGHT = 50.0f * scale;
    const float TITLE_FONT = 26.0f * scale;
    const float STATUS_HEIGHT = 40.0f * scale;
    const float STATUS_FONT = 16.0f * scale;

    // Root - full screen background
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_width), pixels(screen_height)})
            .with_custom_background(theme.background)
            .with_debug_name("bg"));

    // Main card - centered
    float card_width = screen_width * 0.90f;
    float card_height = screen_height * 0.85f;
    float card_x = (screen_width - card_width) / 2.0f;
    float card_y = (screen_height - card_height) / 2.0f;

    auto card = div(context, mk(entity, 1),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(card_width), pixels(card_height)})
            .with_absolute_position()
            .with_translate(card_x, card_y)
            .with_custom_background(theme.surface)
            .with_roundness(0.03f)
            .with_padding(Spacing::md)
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
            .with_font(UIComponent::DEFAULT_FONT, TITLE_FONT)
            .with_alignment(TextAlignment::Center)
            .with_skip_tabbing(true)
            .with_debug_name("title"));

    // Content row - two columns
    float content_height = card_height - TITLE_HEIGHT - STATUS_HEIGHT - 60.0f * scale;
    auto content = div(context, mk(card.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(content_height)})
            .with_flex_direction(FlexDirection::Row)
            .with_debug_name("content"));

    // ========== LEFT COLUMN ==========
    auto left_col = div(context, mk(content.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.48f), percent(1.0f)})
            .with_custom_background(afterhours::colors::darken(theme.surface, 0.95f))
            .with_padding(Spacing::sm)
            .with_flex_direction(FlexDirection::Column)
            .with_no_wrap()
            .with_debug_name("left_col"));

    // With Label section
    div(context, mk(left_col.ent(), 0),
        ComponentConfig{}
            .with_label("With Label")
            .with_size(ComponentSize{percent(1.0f), pixels(HEADER_HEIGHT)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_padding(Spacing::xs)
            .with_font(UIComponent::DEFAULT_FONT, HEADER_FONT)
            .with_skip_tabbing(true)
            .with_debug_name("with_label_hdr"));

    checkbox(context, mk(left_col.ent(), 1), labeled_primary,
             ComponentConfig{}
                 .with_label("Primary")
                 .with_size(ComponentSize{percent(1.0f), pixels(CHECKBOX_HEIGHT)})
                 .with_background(Theme::Usage::Primary)
                 .with_font(UIComponent::DEFAULT_FONT, CHECKBOX_FONT)
                 .with_debug_name("cb_primary"));

    checkbox(context, mk(left_col.ent(), 2), labeled_secondary,
             ComponentConfig{}
                 .with_label("Secondary")
                 .with_size(ComponentSize{percent(1.0f), pixels(CHECKBOX_HEIGHT)})
                 .with_background(Theme::Usage::Secondary)
                 .with_font(UIComponent::DEFAULT_FONT, CHECKBOX_FONT)
                 .with_debug_name("cb_secondary"));

    checkbox(context, mk(left_col.ent(), 3), labeled_accent,
             ComponentConfig{}
                 .with_label("Accent")
                 .with_size(ComponentSize{percent(1.0f), pixels(CHECKBOX_HEIGHT)})
                 .with_background(Theme::Usage::Accent)
                 .with_font(UIComponent::DEFAULT_FONT, CHECKBOX_FONT)
                 .with_debug_name("cb_accent"));

    // Box Only section
    div(context, mk(left_col.ent(), 4),
        ComponentConfig{}
            .with_label("Box Only")
            .with_size(ComponentSize{percent(1.0f), pixels(HEADER_HEIGHT)})
            .with_background(Theme::Usage::Secondary)
            .with_auto_text_color(true)
            .with_padding(Spacing::xs)
            .with_font(UIComponent::DEFAULT_FONT, HEADER_FONT)
            .with_skip_tabbing(true)
            .with_debug_name("box_only_hdr"));

    auto no_label_row = div(context, mk(left_col.ent(), 5),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(CHECKBOX_HEIGHT + 8.0f * scale)})
            .with_flex_direction(FlexDirection::Row)
            .with_justify_content(JustifyContent::SpaceAround)
            .with_align_items(AlignItems::Center)
            .with_debug_name("no_label_row"));

    float box_size = 32.0f * scale;
    checkbox_no_label(context, mk(no_label_row.ent(), 0), no_label_1,
        ComponentConfig{}
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_background(Theme::Usage::Primary)
            .with_font(UIComponent::SYMBOL_FONT, 20.0f * scale)
            .with_debug_name("nl_1"));

    checkbox_no_label(context, mk(no_label_row.ent(), 1), no_label_2,
        ComponentConfig{}
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_background(Theme::Usage::Secondary)
            .with_font(UIComponent::SYMBOL_FONT, 20.0f * scale)
            .with_debug_name("nl_2"));

    checkbox_no_label(context, mk(no_label_row.ent(), 2), no_label_3,
        ComponentConfig{}
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_background(Theme::Usage::Accent)
            .with_font(UIComponent::SYMBOL_FONT, 20.0f * scale)
            .with_debug_name("nl_3"));

    checkbox_no_label(context, mk(no_label_row.ent(), 3), no_label_4,
        ComponentConfig{}
            .with_size(ComponentSize{pixels(box_size), pixels(box_size)})
            .with_background(Theme::Usage::Primary)
            .with_font(UIComponent::SYMBOL_FONT, 20.0f * scale)
            .with_debug_name("nl_4"));

    // Disabled section
    div(context, mk(left_col.ent(), 6),
        ComponentConfig{}
            .with_label("Disabled")
            .with_size(ComponentSize{percent(1.0f), pixels(HEADER_HEIGHT)})
            .with_background(Theme::Usage::Accent)
            .with_auto_text_color(true)
            .with_padding(Spacing::xs)
            .with_font(UIComponent::DEFAULT_FONT, HEADER_FONT)
            .with_skip_tabbing(true)
            .with_debug_name("disabled_hdr"));

    checkbox(context, mk(left_col.ent(), 7), disabled_checked,
             ComponentConfig{}
                 .with_label("Disabled ON")
                 .with_size(ComponentSize{percent(1.0f), pixels(CHECKBOX_HEIGHT)})
                 .with_background(Theme::Usage::Primary)
                 .with_disabled(true)
                 .with_font(UIComponent::DEFAULT_FONT, CHECKBOX_FONT)
                 .with_debug_name("cb_dis_on"));

    checkbox(context, mk(left_col.ent(), 8), disabled_unchecked,
             ComponentConfig{}
                 .with_label("Disabled OFF")
                 .with_size(ComponentSize{percent(1.0f), pixels(CHECKBOX_HEIGHT)})
                 .with_background(Theme::Usage::Primary)
                 .with_disabled(true)
                 .with_font(UIComponent::DEFAULT_FONT, CHECKBOX_FONT)
                 .with_debug_name("cb_dis_off"));

    // ========== RIGHT COLUMN ==========
    auto right_col = div(context, mk(content.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.48f), percent(1.0f)})
            .with_custom_background(afterhours::colors::darken(theme.surface, 0.95f))
            .with_padding(Spacing::sm)
            .with_flex_direction(FlexDirection::Column)
            .with_no_wrap()
            .with_debug_name("right_col"));

    // Multi-Select section
    div(context, mk(right_col.ent(), 0),
        ComponentConfig{}
            .with_label("Multi-Select Group")
            .with_size(ComponentSize{percent(1.0f), pixels(HEADER_HEIGHT)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_padding(Spacing::xs)
            .with_font(UIComponent::DEFAULT_FONT, HEADER_FONT)
            .with_skip_tabbing(true)
            .with_debug_name("multi_hdr"));

    checkbox_group(context, mk(right_col.ent(), 1), options_group,
        std::array<std::string_view, 4>{"Option A", "Option B", "Option C", "Option D"},
        {-1, -1},
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(GROUP_ROW_HEIGHT)})
            .with_flex_direction(FlexDirection::Column)
            .with_background(Theme::Usage::Primary)
            .with_font(UIComponent::DEFAULT_FONT, CHECKBOX_FONT)
            .with_debug_name("multi_group"));

    // Min/Max section
    div(context, mk(right_col.ent(), 2),
        ComponentConfig{}
            .with_label("Min/Max (1-2)")
            .with_size(ComponentSize{percent(1.0f), pixels(HEADER_HEIGHT)})
            .with_background(Theme::Usage::Secondary)
            .with_auto_text_color(true)
            .with_padding(Spacing::xs)
            .with_font(UIComponent::DEFAULT_FONT, HEADER_FONT)
            .with_skip_tabbing(true)
            .with_debug_name("minmax_hdr"));

    checkbox_group(context, mk(right_col.ent(), 3), min_max_group,
        std::array<std::string_view, 3>{"Choice 1", "Choice 2", "Choice 3"},
        {1, 2},
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(GROUP_ROW_HEIGHT)})
            .with_flex_direction(FlexDirection::Column)
            .with_background(Theme::Usage::Secondary)
            .with_font(UIComponent::DEFAULT_FONT, CHECKBOX_FONT)
            .with_debug_name("minmax_group"));

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
            .with_font(UIComponent::DEFAULT_FONT, STATUS_FONT)
            .with_alignment(TextAlignment::Center)
            .with_skip_tabbing(true)
            .with_debug_name("status"));
  }
};

REGISTER_EXAMPLE_SCREEN(checkboxes, "Component Galleries",
                        "All checkbox component types and variations",
                        CheckboxShowcase)
