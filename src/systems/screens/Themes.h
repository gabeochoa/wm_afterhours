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
  bool toggle_state = false;

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

    // Full screen background (no padding so it stays in bounds)
    auto background =
        div(context, mk(entity, 0),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(1.0f), screen_pct(1.0f)})
                .with_custom_background(theme.background)
                .with_debug_name("main_bg"));

    // Content container with padding
    auto main = div(context, mk(background.ent(), 0),
                    ComponentConfig{}
                        .with_size(ComponentSize{percent(1.0f), percent(1.0f)})
                        .with_padding(Spacing::lg)
                        .with_flex_direction(FlexDirection::Column)
                        .with_debug_name("main"));

    // ========== HEADER ==========
    auto header = div(context, mk(main.ent(), 0),
                      ComponentConfig{}
                          .with_size(ComponentSize{pixels(950), pixels(80)})
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
    auto content = div(context, mk(main.ent(), 1),
                       ComponentConfig{}
                           .with_size(ComponentSize{pixels(950), pixels(480)})
                           .with_custom_background(theme.background)
                           .with_flex_direction(FlexDirection::Row)
                           .with_margin(Margin{.top = DefaultSpacing::small()})
                           .with_debug_name("content"));

    // LEFT - Theme Selection
    auto selector_panel =
        div(context, mk(content.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(250), pixels(460)})
                .with_custom_background(theme.surface)
                .with_padding(Spacing::md)
                .with_flex_direction(FlexDirection::Column)
                .with_margin(Spacing::xs)
                .with_soft_shadow(4.0f, 6.0f, 12.0f)
                .with_debug_name("selector_panel"));

    div(context, mk(selector_panel.ent(), 0),
        ComponentConfig{}
            .with_label("Select Theme")
            .with_size(ComponentSize{pixels(210), pixels(40)})
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
                  .with_size(ComponentSize{pixels(210), pixels(48)})
                  .with_background(selected ? Theme::Usage::Accent
                                            : Theme::Usage::Secondary)
                  .with_font(UIComponent::DEFAULT_FONT, 18.0f)
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
                .with_size(ComponentSize{pixels(420), pixels(460)})
                .with_custom_background(theme.surface)
                .with_padding(Spacing::md)
                .with_flex_direction(FlexDirection::Column)
                .with_margin(Spacing::xs)
                .with_soft_shadow(4.0f, 6.0f, 12.0f)
                .with_debug_name("preview_panel"));

    div(context, mk(preview_panel.ent(), 0),
        ComponentConfig{}
            .with_label("Component Preview")
            .with_size(ComponentSize{pixels(340), pixels(40)})
            .with_background(Theme::Usage::Primary)
            .with_font(UIComponent::DEFAULT_FONT, 20.0f)
            .with_padding(Spacing::xs)
            .with_debug_name("preview_title"));

    // Buttons row - use percent sizing to fit within row
    auto btn_row = div(context, mk(preview_panel.ent(), 1),
                       ComponentConfig{}
                           .with_size(ComponentSize{pixels(360), pixels(55)})
                           .with_custom_background(theme.surface)
                           .with_flex_direction(FlexDirection::Row)
                           .with_no_wrap()
                           .with_margin(Spacing::xs)
                           .with_debug_name("btn_row"));

    button(context, mk(btn_row.ent(), 0),
           ComponentConfig{}
               .with_label("P")
               .with_size(ComponentSize{percent(0.24f), percent(0.90f)})
               .with_background(Theme::Usage::Primary)
               .with_auto_text_color(true)
               .with_font(UIComponent::DEFAULT_FONT, 12.0f)
               .with_debug_name("btn_primary"));

    button(context, mk(btn_row.ent(), 1),
           ComponentConfig{}
               .with_label("S")
               .with_size(ComponentSize{percent(0.24f), percent(0.90f)})
               .with_background(Theme::Usage::Secondary)
               .with_auto_text_color(true)
               .with_font(UIComponent::DEFAULT_FONT, 12.0f)
               .with_debug_name("btn_secondary"));

    button(context, mk(btn_row.ent(), 2),
           ComponentConfig{}
               .with_label("A")
               .with_size(ComponentSize{percent(0.24f), percent(0.90f)})
               .with_background(Theme::Usage::Accent)
               .with_auto_text_color(true)
               .with_font(UIComponent::DEFAULT_FONT, 12.0f)
               .with_debug_name("btn_accent"));

    button(context, mk(btn_row.ent(), 3),
           ComponentConfig{}
               .with_label("D")
               .with_size(ComponentSize{percent(0.24f), percent(0.90f)})
               .with_background(Theme::Usage::Primary)
               .with_auto_text_color(true)
               .with_font(UIComponent::DEFAULT_FONT, 12.0f)
               .with_disabled(true)
               .with_debug_name("btn_disabled"));

    // Slider - height 54 to avoid wrap, no label to give full width to background
    slider(context, mk(preview_panel.ent(), 2), slider_value,
           ComponentConfig{}
               .with_size(ComponentSize{pixels(360), pixels(54)})
               .with_background(Theme::Usage::Primary)
               .with_font(UIComponent::DEFAULT_FONT, 14.0f)
               .with_margin(Spacing::xs)
               .with_debug_name("preview_slider"),
           SliderHandleValueLabelPosition::None);

    // Checkbox
    checkbox(context, mk(preview_panel.ent(), 3), checkbox_state,
             ComponentConfig{}
                 .with_label("Checkbox")
                 .with_size(ComponentSize{pixels(360), pixels(50)})
                 .with_background(Theme::Usage::Primary)
                 .with_font(UIComponent::DEFAULT_FONT, 14.0f)
                 .with_margin(Spacing::xs)
                 .with_debug_name("preview_checkbox"));

    // Toggle switch instead of dropdown (dropdown has hardcoded internal sizing)
    toggle_switch(context, mk(preview_panel.ent(), 4), toggle_state,
                  ComponentConfig{}
                      .with_label("Toggle")
                      .with_size(ComponentSize{pixels(360), pixels(50)})
                      .with_background(Theme::Usage::Secondary)
                      .with_font(UIComponent::DEFAULT_FONT, 14.0f)
                      .with_margin(Spacing::xs)
                      .with_debug_name("preview_toggle"));

    // Cards with shadows demo - use percent sizing to fit within row
    auto cards_row =
        div(context, mk(preview_panel.ent(), 5),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(360), pixels(70)})
                .with_custom_background(theme.surface)
                .with_flex_direction(FlexDirection::Row)
                .with_no_wrap()
                .with_margin(Margin{.top = DefaultSpacing::small()})
                .with_debug_name("cards_row"));

    div(context, mk(cards_row.ent(), 0),
        ComponentConfig{}
            .with_label("Hard")
            .with_size(ComponentSize{percent(0.32f), percent(0.90f)})
            .with_background(Theme::Usage::Primary)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_hard_shadow(4.0f, 4.0f)
            .with_debug_name("card_hard"));

    div(context, mk(cards_row.ent(), 1),
        ComponentConfig{}
            .with_label("Soft")
            .with_size(ComponentSize{percent(0.32f), percent(0.90f)})
            .with_background(Theme::Usage::Secondary)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_soft_shadow(4.0f, 6.0f, 12.0f)
            .with_debug_name("card_soft"));

    div(context, mk(cards_row.ent(), 2),
        ComponentConfig{}
            .with_label("Accent")
            .with_size(ComponentSize{percent(0.32f), percent(0.90f)})
            .with_background(Theme::Usage::Accent)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_soft_shadow(3.0f, 5.0f, 8.0f)
            .with_debug_name("card_accent"));
  }
};

REGISTER_EXAMPLE_SCREEN(themes, "Tools", "Real-time theme switching demo",
                        ThemesScreen)
