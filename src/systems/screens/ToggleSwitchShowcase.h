#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ToggleSwitchShowcase : ScreenSystem<UIContext<InputAction>> {
  // Pill style toggles
  bool enable_notifications = true;
  bool enable_sound = false;
  bool enable_vibration = true;
  // Circle style toggles
  bool dark_mode = true;
  bool auto_save = false;
  bool cloud_sync = true;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    // Use neon_dark theme for better toggle visibility
    auto theme = afterhours::ui::theme_presets::neon_dark();
    context.theme = theme;

    // Main container - centered on screen, sized to fill more screen space
    auto root =
        div(context, mk(entity, 0),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(0.75f), screen_pct(0.95f)})
                .with_self_align(SelfAlign::Center)
                .with_custom_background(theme.background)
                .with_roundness(0.04f)
                .with_debug_name("toggle_bg"));

    auto main_container =
        div(context, mk(root.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), percent(1.0f)})
                .with_padding(Spacing::lg)
                .with_flex_direction(FlexDirection::Column)
                .with_debug_name("toggle_main"));

    // Title
    div(context, mk(main_container.ent(), 0),
        ComponentConfig{}
            .with_label("Toggle Switch Components")
            .with_size(ComponentSize{percent(1.0f), pixels(52)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_padding(Spacing::md)
            .with_font(UIComponent::DEFAULT_FONT, h720(28.0f))
            .with_margin(Margin{.bottom = DefaultSpacing::medium()})
            .with_skip_tabbing(true)
            .with_debug_name("main_title"));

    // Section: Pill Style
    div(context, mk(main_container.ent(), 1),
        ComponentConfig{}
            .with_label("Pill Style (iOS) - Slide to toggle ON/OFF")
            .with_size(ComponentSize{percent(1.0f), pixels(48)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, h720(20.0f))
            .with_margin(Margin{.bottom = DefaultSpacing::small()})
            .with_skip_tabbing(true)
            .with_debug_name("pill_title"));

    toggle_switch(context, mk(main_container.ent(), 2), enable_notifications,
                  ComponentConfig{}
                      .with_label(enable_notifications ? "Notifications: ON" : "Notifications: OFF")
                      .with_size(ComponentSize{percent(1.0f), pixels(52)})
                      .with_custom_background(afterhours::colors::lighten(theme.surface, 0.15f))
                      .with_font(UIComponent::DEFAULT_FONT, h720(20.0f))
                      .with_padding(Padding{.left = DefaultSpacing::small()})
                      .with_margin(Margin{.bottom = DefaultSpacing::tiny()})
                      .with_debug_name("notifications"),
                  ToggleSwitchStyle::Pill);

    toggle_switch(context, mk(main_container.ent(), 3), enable_sound,
                  ComponentConfig{}
                      .with_label(enable_sound ? "Sound Effects: ON" : "Sound Effects: OFF")
                      .with_size(ComponentSize{percent(1.0f), pixels(52)})
                      .with_custom_background(afterhours::colors::lighten(theme.surface, 0.15f))
                      .with_font(UIComponent::DEFAULT_FONT, h720(20.0f))
                      .with_padding(Padding{.left = DefaultSpacing::small()})
                      .with_margin(Margin{.bottom = DefaultSpacing::tiny()})
                      .with_debug_name("sound"),
                  ToggleSwitchStyle::Pill);

    toggle_switch(context, mk(main_container.ent(), 4), enable_vibration,
                  ComponentConfig{}
                      .with_label(enable_vibration ? "Vibration: ON" : "Vibration: OFF")
                      .with_size(ComponentSize{percent(1.0f), pixels(52)})
                      .with_custom_background(afterhours::colors::lighten(theme.surface, 0.15f))
                      .with_font(UIComponent::DEFAULT_FONT, h720(20.0f))
                      .with_padding(Padding{.left = DefaultSpacing::small()})
                      .with_margin(Margin{.bottom = DefaultSpacing::medium()})
                      .with_debug_name("vibration"),
                  ToggleSwitchStyle::Pill);

    // Section: Circle Style
    div(context, mk(main_container.ent(), 5),
        ComponentConfig{}
            .with_label("Circle Style (Checkbox) - Click to toggle ON/OFF")
            .with_size(ComponentSize{percent(1.0f), pixels(48)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, h720(20.0f))
            .with_margin(Margin{.bottom = DefaultSpacing::small()})
            .with_skip_tabbing(true)
            .with_debug_name("circle_title"));

    toggle_switch(context, mk(main_container.ent(), 6), dark_mode,
                  ComponentConfig{}
                      .with_label(dark_mode ? "Dark Mode: ON" : "Dark Mode: OFF")
                      .with_size(ComponentSize{percent(1.0f), pixels(52)})
                      .with_custom_background(afterhours::colors::lighten(theme.surface, 0.15f))
                      .with_font(UIComponent::DEFAULT_FONT, h720(20.0f))
                      .with_padding(Padding{.left = DefaultSpacing::small()})
                      .with_margin(Margin{.bottom = DefaultSpacing::tiny()})
                      .with_debug_name("dark_mode"),
                  ToggleSwitchStyle::Circle);

    toggle_switch(context, mk(main_container.ent(), 7), auto_save,
                  ComponentConfig{}
                      .with_label(auto_save ? "Auto-Save: ON" : "Auto-Save: OFF")
                      .with_size(ComponentSize{percent(1.0f), pixels(52)})
                      .with_custom_background(afterhours::colors::lighten(theme.surface, 0.15f))
                      .with_font(UIComponent::DEFAULT_FONT, h720(20.0f))
                      .with_padding(Padding{.left = DefaultSpacing::small()})
                      .with_margin(Margin{.bottom = DefaultSpacing::tiny()})
                      .with_debug_name("auto_save"),
                  ToggleSwitchStyle::Circle);

    toggle_switch(context, mk(main_container.ent(), 8), cloud_sync,
                  ComponentConfig{}
                      .with_label(cloud_sync ? "Cloud Sync: ON" : "Cloud Sync: OFF")
                      .with_size(ComponentSize{percent(1.0f), pixels(52)})
                      .with_custom_background(afterhours::colors::lighten(theme.surface, 0.15f))
                      .with_font(UIComponent::DEFAULT_FONT, h720(20.0f))
                      .with_padding(Padding{.left = DefaultSpacing::small()})
                      .with_margin(Margin{.bottom = DefaultSpacing::medium()})
                      .with_debug_name("cloud_sync"),
                  ToggleSwitchStyle::Circle);

    // Status bar showing current states
    std::string status = fmt::format(
        "Notifications: {}  |  Dark Mode: {}  |  Cloud Sync: {}",
        enable_notifications ? "ON" : "OFF", dark_mode ? "ON" : "OFF",
        cloud_sync ? "ON" : "OFF");

    div(context, mk(main_container.ent(), 9),
        ComponentConfig{}
            .with_label(status)
            .with_size(ComponentSize{percent(1.0f), pixels(48)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_skip_tabbing(true)
            .with_debug_name("status"));
  }
};

REGISTER_EXAMPLE_SCREEN(toggle_switches, "Component Galleries",
                        "Toggle switch: Pill (iOS) and Circle (X/V) styles",
                        ToggleSwitchShowcase)
