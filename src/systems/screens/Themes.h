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
    context.scaling_mode = ScalingMode::Adaptive;
    UIStylingDefaults::get().set_default_font(UIComponent::DEFAULT_FONT,
                                              pixels(16.0f));

    // Full screen background - uses normal flow layout (Column)
    auto background =
        div(context, mk(entity, 0),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(1.0f), screen_pct(1.0f)})
                .with_background(Theme::Usage::Background)
                .with_padding(Padding{.top = pixels(20),
                                      .left = pixels(40),
                                      .right = pixels(40),
                                      .bottom = pixels(10)})
                .with_align_items(AlignItems::Center)
                .with_no_wrap()
                .with_debug_name("main_bg"));

    // ========== HEADER ==========
    auto header =
        hstack(context, mk(background.ent(), 0),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.0f), pixels(52)})
                   .with_background(Theme::Usage::Surface)
                   .with_padding(Padding{.left = pixels(8), .right = pixels(8)})
                   .with_no_wrap()
                   .with_align_items(AlignItems::Center)
                   .with_soft_shadow(3.0f, 4.0f, 10.0f)
                   .with_margin(Margin{.bottom = DefaultSpacing::small()})
                   .with_debug_name("header"));

    div(context, mk(header.ent(), 0),
        ComponentConfig{}
            .with_label("Theme Switcher")
            .with_size(ComponentSize{pixels(220), pixels(38)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_font_size(pixels(22.0f))
            .with_padding(Spacing::xs));

    div(context, mk(header.ent(), 1),
        ComponentConfig{}
            .with_label("Active: " + get_theme_name(current_theme))
            .with_size(ComponentSize{pixels(260), pixels(38)})
            .with_background(Theme::Usage::Accent)
            .with_auto_text_color(true)
            .with_font_size(pixels(19.0f))
            .with_margin(Margin{.left = DefaultSpacing::medium()})
            .with_hard_shadow(3.0f, 3.0f));

    // ========== MAIN CONTENT ==========
    auto content =
        hstack(context, mk(background.ent(), 1),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.0f), percent(0.88f)})
                   .with_background(Theme::Usage::Background)
                   .with_no_wrap()
                   .with_debug_name("content"));

    // LEFT - Theme Selection
    auto selector_panel =
        vstack(context, mk(content.ent(), 0),
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(200), percent(1.0f)})
                   .with_background(Theme::Usage::Surface)
                   .with_padding(Spacing::sm)
                   .with_margin(Margin{.right = DefaultSpacing::small()})
                   .with_soft_shadow(3.0f, 4.0f, 8.0f)
                   .with_debug_name("selector_panel"));

    div(context, mk(selector_panel.ent(), 0),
        ComponentConfig{}
            .with_label("Select Theme")
            .with_size(ComponentSize{percent(0.95f), pixels(35)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_font_size(pixels(18.0f))
            .with_padding(Spacing::xs));

    // Theme buttons
    auto theme_choices = {ThemeChoice::CozyKraft, ThemeChoice::NeonDark,
                          ThemeChoice::OceanNavy, ThemeChoice::Midnight,
                          ThemeChoice::SageNatural};

    int btn_idx = 1;
    for (auto choice : theme_choices) {
      bool selected = (choice == current_theme);
      auto btn_config =
          ComponentConfig{}
              .with_label(selected ? "> " + get_theme_name(choice) + " <"
                                   : get_theme_name(choice))
              .with_size(ComponentSize{percent(0.95f), pixels(48)})
              .with_background(selected ? Theme::Usage::Accent
                                        : Theme::Usage::Secondary)
              .with_auto_text_color(true)
              .with_font_size(selected ? pixels(19.0f) : pixels(17.0f))
              .with_margin(Spacing::xs)
              .with_debug_name("theme_btn_" + std::to_string(btn_idx));

      if (selected) {
        // When selected, use accent background — ensure text contrasts with it
        auto accent = context.theme.accent;
        btn_config = btn_config.with_custom_text_color(
            afterhours::colors::auto_text_color(accent, context.theme.font,
                                                context.theme.darkfont));
        btn_config = btn_config.with_hard_shadow(3.0f, 3.0f);
      }

      if (button(context, mk(selector_panel.ent(), btn_idx), btn_config)) {
        current_theme = choice;
      }
      btn_idx++;
    }

    // RIGHT - Component Preview
    auto preview_panel =
        vstack(context, mk(content.ent(), 1),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(0.78f), percent(1.0f)})
                   .with_background(Theme::Usage::Surface)
                   .with_padding(Spacing::sm)
                   .with_no_wrap()
                   .with_soft_shadow(3.0f, 4.0f, 8.0f)
                   .with_debug_name("preview_panel"));

    div(context, mk(preview_panel.ent(), 0),
        ComponentConfig{}
            .with_label("Component Preview")
            .with_size(ComponentSize{percent(0.95f), pixels(32)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_font_size(pixels(16.0f))
            .with_padding(Spacing::xs));

    // Buttons row
    auto btn_row =
        hstack(context, mk(preview_panel.ent(), 1),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(0.95f), pixels(45)})
                   .with_background(Theme::Usage::Surface)
                   .with_no_wrap()
                   .with_margin(Spacing::xs)
                   .with_debug_name("btn_row"));

    button(context, mk(btn_row.ent(), 0),
           ComponentConfig{}
               .with_label("P")
               .with_size(ComponentSize{percent(0.23f), percent(0.85f)})
               .with_background(Theme::Usage::Primary)
               .with_auto_text_color(true)
               .with_font_size(pixels(18.0f)));

    button(context, mk(btn_row.ent(), 1),
           ComponentConfig{}
               .with_label("S")
               .with_size(ComponentSize{percent(0.23f), percent(0.85f)})
               .with_background(Theme::Usage::Secondary)
               .with_auto_text_color(true)
               .with_font_size(pixels(18.0f)));

    button(context, mk(btn_row.ent(), 2),
           ComponentConfig{}
               .with_label("A")
               .with_size(ComponentSize{percent(0.23f), percent(0.85f)})
               .with_background(Theme::Usage::Accent)
               .with_auto_text_color(true)
               .with_font_size(pixels(18.0f)));

    button(context, mk(btn_row.ent(), 3),
           ComponentConfig{}
               .with_label("D")
               .with_size(ComponentSize{percent(0.23f), percent(0.85f)})
               .with_background(Theme::Usage::Primary)
               .with_auto_text_color(true)
               .with_font_size(pixels(18.0f))
               .with_disabled(true)
               .with_opacity(0.5f));

    // Slider
    slider(context, mk(preview_panel.ent(), 2), slider_value,
           ComponentConfig{}
               .with_size(ComponentSize{percent(0.95f), pixels(40)})
               .with_background(Theme::Usage::Primary)
               .with_font_size(pixels(14.0f))
               .with_margin(Spacing::xs)
               .with_debug_name("preview_slider"),
           SliderHandleValueLabelPosition::None);

    // Checkbox
    checkbox(context, mk(preview_panel.ent(), 3), checkbox_state,
             ComponentConfig{}
                 .with_label("Checkbox")
                 .with_size(ComponentSize{percent(0.95f), pixels(36)})
                 .with_background(Theme::Usage::Primary)
                 .with_auto_text_color(true)
                 .with_font_size(pixels(14.0f))
                 .with_margin(Spacing::xs));

    // Toggle switch - increased height to fit toggle_track
    toggle_switch(context, mk(preview_panel.ent(), 4), toggle_state,
                  ComponentConfig{}
                      .with_label("Toggle")
                      .with_size(ComponentSize{percent(0.95f), pixels(48)})
                      .with_background(Theme::Usage::Secondary)
                      .with_auto_text_color(true)
                      .with_font_size(pixels(14.0f))
                      .with_margin(Spacing::xs));

    // Cards with shadows demo
    auto cards_row =
        hstack(context, mk(preview_panel.ent(), 5),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(0.95f), pixels(55)})
                   .with_background(Theme::Usage::Surface)
                   .with_no_wrap()
                   .with_margin(Margin{.top = DefaultSpacing::small()})
                   .with_debug_name("cards_row"));

    div(context, mk(cards_row.ent(), 0),
        ComponentConfig{}
            .with_label("Hard")
            .with_size(ComponentSize{percent(0.30f), percent(0.75f)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_font_size(pixels(14.0f))
            .with_hard_shadow(2.0f, 2.0f));

    div(context, mk(cards_row.ent(), 1),
        ComponentConfig{}
            .with_label("Soft")
            .with_size(ComponentSize{percent(0.30f), percent(0.75f)})
            .with_background(Theme::Usage::Secondary)
            .with_auto_text_color(true)
            .with_font_size(pixels(14.0f))
            .with_soft_shadow(2.0f, 3.0f, 6.0f));

    div(context, mk(cards_row.ent(), 2),
        ComponentConfig{}
            .with_label("Accent")
            .with_size(ComponentSize{percent(0.30f), percent(0.75f)})
            .with_background(Theme::Usage::Accent)
            .with_auto_text_color(true)
            .with_font_size(pixels(14.0f))
            .with_soft_shadow(2.0f, 2.0f, 5.0f));

    // Text display row for theme colors
    auto text_row =
        hstack(context, mk(preview_panel.ent(), 6),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(0.95f), pixels(50)})
                   .with_background(Theme::Usage::Surface)
                   .with_no_wrap()
                   .with_margin(Margin{.top = DefaultSpacing::small()})
                   .with_debug_name("text_row"));

    div(context, mk(text_row.ent(), 0),
        ComponentConfig{}
            .with_label("Primary Text")
            .with_size(ComponentSize{percent(0.32f), percent(0.85f)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_font_size(pixels(13.0f)));

    div(context, mk(text_row.ent(), 1),
        ComponentConfig{}
            .with_label("Secondary Text")
            .with_size(ComponentSize{percent(0.32f), percent(0.85f)})
            .with_background(Theme::Usage::Secondary)
            .with_auto_text_color(true)
            .with_font_size(pixels(13.0f)));

    div(context, mk(text_row.ent(), 2),
        ComponentConfig{}
            .with_label("Surface Text")
            .with_size(ComponentSize{percent(0.32f), percent(0.85f)})
            .with_background(Theme::Usage::Surface)
            .with_auto_text_color(true)
            .with_font_size(pixels(13.0f))
            .with_soft_shadow(1.0f, 1.0f, 4.0f));

    // Labeled separator
    separator(context, mk(preview_panel.ent(), 7),
              SeparatorOrientation::Horizontal, ComponentConfig{});

    // Progress bar
    progress_bar(context, mk(preview_panel.ent(), 8), 0.72f,
                 ComponentConfig{}
                     .with_label("Loading")
                     .with_size(ComponentSize{percent(0.95f), pixels(28)})
                     .with_background(Theme::Usage::Primary)
                     .with_auto_text_color(true)
                     .with_font_size(pixels(13.0f))
                     .with_margin(Spacing::xs));
  }
};

REGISTER_EXAMPLE_SCREEN(themes, "Tools", "Real-time theme switching demo",
                        ThemesScreen)
