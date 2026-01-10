#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ThemesScreen : ScreenSystem<UIContext<InputAction>> {
  enum struct ThemeChoice {
    CozyKraft,
    NeonDark,
    OceanNavy,
    Midnight,
    SageNatural
  };

  ThemeChoice current_theme = ThemeChoice::OceanNavy;
  float slider_value = 0.5f;
  bool checkbox_state = true;
  size_t dropdown_index = 0;
  std::vector<std::string> dropdown_options = {"Option A", "Option B",
                                               "Option C"};

  Theme get_theme_for_choice(ThemeChoice choice) {
    switch (choice) {
    case ThemeChoice::CozyKraft:
      return theme_presets::cozy_kraft();
    case ThemeChoice::NeonDark:
      return theme_presets::neon_dark();
    case ThemeChoice::OceanNavy:
      return theme_presets::ocean_navy();
    case ThemeChoice::Midnight:
      return theme_presets::midnight();
    case ThemeChoice::SageNatural:
      return theme_presets::sage_natural();
    default:
      return theme_presets::ocean_navy();
    }
  }

  std::string get_theme_name(ThemeChoice choice) {
    switch (choice) {
    case ThemeChoice::CozyKraft:
      return "Cozy Kraft";
    case ThemeChoice::NeonDark:
      return "Neon Dark";
    case ThemeChoice::OceanNavy:
      return "Ocean Navy";
    case ThemeChoice::Midnight:
      return "Midnight";
    case ThemeChoice::SageNatural:
      return "Sage Natural";
    default:
      return "Unknown";
    }
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = get_theme_for_choice(current_theme);
    context.theme = theme;

    // Full screen background
    auto main =
        div(context, mk(entity, 0),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(1.0f), screen_pct(1.0f)})
                .with_custom_background(theme.background)
                .with_padding(Spacing::lg)
                .with_flex_direction(FlexDirection::Column)
                .with_debug_name("main"));

    // ========== HEADER ==========
    auto header =
        div(context, mk(main.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(1100), pixels(80)})
                .with_custom_background(theme.surface)
                .with_padding(Spacing::md)
                .with_flex_direction(FlexDirection::Row)
                .with_soft_shadow(3.0f, 4.0f, 10.0f)
                .with_debug_name("header"));

    div(context, mk(header.ent(), 0),
        ComponentConfig{}
            .with_label("Theme Switcher")
            .with_size(ComponentSize{pixels(350), pixels(55)})
            .with_background(Theme::Usage::Primary)
            .with_font(UIComponent::DEFAULT_FONT, 32.0f)
            .with_padding(Spacing::sm)
            .with_debug_name("title"));

    div(context, mk(header.ent(), 1),
        ComponentConfig{}
            .with_label("Current: " + get_theme_name(current_theme))
            .with_size(ComponentSize{pixels(300), pixels(45)})
            .with_background(Theme::Usage::Accent)
            .with_font(UIComponent::DEFAULT_FONT, 20.0f)
            .with_margin(Spacing::sm)
            .with_debug_name("current_theme"));

    // ========== MAIN CONTENT ==========
    auto content =
        div(context, mk(main.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(1100), pixels(420)})
                .with_custom_background(theme.background)
                .with_flex_direction(FlexDirection::Row)
                .with_margin(Margin{.top = DefaultSpacing::small()})
                .with_debug_name("content"));

    // LEFT - Theme Selection
    auto selector_panel =
        div(context, mk(content.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(280), pixels(400)})
                .with_custom_background(theme.surface)
                .with_padding(Spacing::md)
                .with_flex_direction(FlexDirection::Column)
                .with_margin(Spacing::sm)
                .with_soft_shadow(4.0f, 6.0f, 12.0f)
                .with_debug_name("selector_panel"));

    div(context, mk(selector_panel.ent(), 0),
        ComponentConfig{}
            .with_label("Select Theme")
            .with_size(ComponentSize{pixels(240), pixels(40)})
            .with_background(Theme::Usage::Primary)
            .with_font(UIComponent::DEFAULT_FONT, 22.0f)
            .with_padding(Spacing::xs)
            .with_debug_name("selector_title"));

    // Theme buttons
    auto theme_choices = {ThemeChoice::CozyKraft, ThemeChoice::NeonDark,
                          ThemeChoice::OceanNavy, ThemeChoice::Midnight,
                          ThemeChoice::SageNatural};

    int btn_idx = 1;
    for (auto choice : theme_choices) {
      bool selected = (choice == current_theme);
      if (button(
              context, mk(selector_panel.ent(), btn_idx),
              ComponentConfig{}
                  .with_label(get_theme_name(choice))
                  .with_size(ComponentSize{pixels(240), pixels(48)})
                  .with_background(selected ? Theme::Usage::Accent
                                             : Theme::Usage::Secondary)
                  .with_font(UIComponent::DEFAULT_FONT, 20.0f)
                  .with_margin(Spacing::sm)
                  .with_debug_name("theme_btn_" + std::to_string(btn_idx)))) {
        current_theme = choice;
      }
      btn_idx++;
    }

    // RIGHT - Component Preview
    auto preview_panel =
        div(context, mk(content.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(550), pixels(400)})
                .with_custom_background(theme.surface)
                .with_padding(Spacing::md)
                .with_flex_direction(FlexDirection::Column)
                .with_margin(Spacing::sm)
                .with_soft_shadow(4.0f, 6.0f, 12.0f)
                .with_debug_name("preview_panel"));

    div(context, mk(preview_panel.ent(), 0),
        ComponentConfig{}
            .with_label("Component Preview")
            .with_size(ComponentSize{pixels(480), pixels(40)})
            .with_background(Theme::Usage::Primary)
            .with_font(UIComponent::DEFAULT_FONT, 22.0f)
            .with_padding(Spacing::xs)
            .with_debug_name("preview_title"));

    // Buttons row
    auto btn_row = div(context, mk(preview_panel.ent(), 1),
                       ComponentConfig{}
                           .with_size(ComponentSize{pixels(500), pixels(60)})
                           .with_custom_background(theme.surface)
                           .with_flex_direction(FlexDirection::Row)
                           .with_margin(Spacing::sm)
                           .with_debug_name("btn_row"));

    button(context, mk(btn_row.ent(), 0),
           ComponentConfig{}
               .with_label("Primary")
               .with_size(ComponentSize{pixels(110), pixels(45)})
               .with_background(Theme::Usage::Primary)
               .with_font(UIComponent::DEFAULT_FONT, 20.0f)
               .with_margin(Spacing::xs)
               .with_debug_name("btn_primary"));

    button(context, mk(btn_row.ent(), 1),
           ComponentConfig{}
               .with_label("Secondary")
               .with_size(ComponentSize{pixels(110), pixels(45)})
               .with_background(Theme::Usage::Secondary)
               .with_font(UIComponent::DEFAULT_FONT, 20.0f)
               .with_margin(Spacing::xs)
               .with_debug_name("btn_secondary"));

    button(context, mk(btn_row.ent(), 2),
           ComponentConfig{}
               .with_label("Accent")
               .with_size(ComponentSize{pixels(110), pixels(45)})
               .with_background(Theme::Usage::Accent)
               .with_font(UIComponent::DEFAULT_FONT, 20.0f)
               .with_margin(Spacing::xs)
               .with_debug_name("btn_accent"));

    button(context, mk(btn_row.ent(), 3),
           ComponentConfig{}
               .with_label("Disabled")
               .with_size(ComponentSize{pixels(110), pixels(45)})
               .with_background(Theme::Usage::Primary)
               .with_font(UIComponent::DEFAULT_FONT, 20.0f)
               .with_margin(Spacing::xs)
               .with_disabled(true)
               .with_debug_name("btn_disabled"));

    // Slider
    slider(context, mk(preview_panel.ent(), 2), slider_value,
           ComponentConfig{}
               .with_label("Slider Value")
               .with_size(ComponentSize{pixels(400), pixels(50)})
               .with_background(Theme::Usage::Primary)
               .with_font(UIComponent::DEFAULT_FONT, 20.0f)
               .with_margin(Spacing::sm)
               .with_debug_name("preview_slider"),
           SliderHandleValueLabelPosition::WithLabel);

    // Checkbox
    checkbox(context, mk(preview_panel.ent(), 3), checkbox_state,
             ComponentConfig{}
                 .with_label("Checkbox Option")
                 .with_size(ComponentSize{pixels(400), pixels(45)})
                 .with_background(Theme::Usage::Primary)
                 .with_font(UIComponent::DEFAULT_FONT, 20.0f)
                 .with_margin(Spacing::sm)
                 .with_debug_name("preview_checkbox"));

    // Dropdown
    dropdown(context, mk(preview_panel.ent(), 4), dropdown_options,
             dropdown_index,
             ComponentConfig{}
                 .with_size(ComponentSize{pixels(400), pixels(45)})
                 .with_background(Theme::Usage::Secondary)
                 .with_font(UIComponent::DEFAULT_FONT, 20.0f)
                 .with_margin(Spacing::sm)
                 .with_debug_name("preview_dropdown"));

    // Cards with shadows demo
    auto cards_row =
        div(context, mk(preview_panel.ent(), 5),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(500), pixels(100)})
                .with_custom_background(theme.surface)
                .with_flex_direction(FlexDirection::Row)
                .with_margin(Margin{.top = DefaultSpacing::small()})
                .with_debug_name("cards_row"));

    div(context, mk(cards_row.ent(), 0),
        ComponentConfig{}
            .with_label("Hard Shadow")
            .with_size(ComponentSize{pixels(140), pixels(80)})
            .with_background(Theme::Usage::Primary)
            .with_font(UIComponent::DEFAULT_FONT, 20.0f)
            .with_margin(Spacing::sm)
            .with_hard_shadow(4.0f, 4.0f)
            .with_debug_name("card_hard"));

    div(context, mk(cards_row.ent(), 1),
        ComponentConfig{}
            .with_label("Soft Shadow")
            .with_size(ComponentSize{pixels(140), pixels(80)})
            .with_background(Theme::Usage::Secondary)
            .with_font(UIComponent::DEFAULT_FONT, 20.0f)
            .with_margin(Spacing::sm)
            .with_soft_shadow(4.0f, 6.0f, 12.0f)
            .with_debug_name("card_soft"));

    div(context, mk(cards_row.ent(), 2),
        ComponentConfig{}
            .with_label("Accent Card")
            .with_size(ComponentSize{pixels(140), pixels(80)})
            .with_background(Theme::Usage::Accent)
            .with_font(UIComponent::DEFAULT_FONT, 20.0f)
            .with_margin(Spacing::sm)
            .with_soft_shadow(3.0f, 5.0f, 8.0f)
            .with_debug_name("card_accent"));
  }
};

REGISTER_EXAMPLE_SCREEN(themes, "Tools", "Real-time theme switching demo",
                        ThemesScreen)
