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

    // Main container - centered on screen, increased height to fit all content
    auto root =
        div(context, mk(entity, 0),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(0.55f), pixels(600)})
                .with_self_align(SelfAlign::Center)
                .with_custom_background(theme.background)
                .with_roundness(0.08f)
                .with_debug_name("toggle_bg"));

    auto main_container =
        div(context, mk(root.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), percent(1.0f)})
                .with_padding(Spacing::sm)
                .with_flex_direction(FlexDirection::Column)
                .with_debug_name("toggle_main"));

    // Title - must be first (lowest ID)
    div(context, mk(main_container.ent(), 0),
        ComponentConfig{}
            .with_label("Toggle Switch Components")
            .with_size(ComponentSize{percent(1.0f), pixels(44)})
            .with_custom_background(theme.surface)
            .with_auto_text_color(true)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, 24.0f)
            .with_margin(Margin{.bottom = DefaultSpacing::small()})
            .with_skip_tabbing(true)
            .with_debug_name("main_title"));

    // Section: Pill Style
    div(context, mk(main_container.ent(), 1),
        ComponentConfig{}
            .with_label("Pill Style (iOS)")
            .with_size(ComponentSize{percent(1.0f), pixels(28)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_padding(Spacing::xs)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_margin(Margin{.bottom = DefaultSpacing::small()})
            .with_skip_tabbing(true)
            .with_debug_name("pill_title"));

    toggle_switch(context, mk(main_container.ent(), 2), enable_notifications,
                  ComponentConfig{}
                      .with_label("Notifications")
                      .with_size(ComponentSize{percent(0.95f), pixels(36)})
                      .with_background(Theme::Usage::Surface)
                      .with_font(UIComponent::DEFAULT_FONT, 16.0f)
                      .with_margin(Spacing::xs)
                      .with_debug_name("notifications"),
                  ToggleSwitchStyle::Pill);

    toggle_switch(context, mk(main_container.ent(), 3), enable_sound,
                  ComponentConfig{}
                      .with_label("Sound Effects")
                      .with_size(ComponentSize{percent(0.95f), pixels(36)})
                      .with_background(Theme::Usage::Surface)
                      .with_font(UIComponent::DEFAULT_FONT, 16.0f)
                      .with_margin(Spacing::xs)
                      .with_debug_name("sound"),
                  ToggleSwitchStyle::Pill);

    toggle_switch(context, mk(main_container.ent(), 4), enable_vibration,
                  ComponentConfig{}
                      .with_label("Vibration")
                      .with_size(ComponentSize{percent(0.95f), pixels(36)})
                      .with_background(Theme::Usage::Surface)
                      .with_font(UIComponent::DEFAULT_FONT, 16.0f)
                      .with_margin(Spacing::xs)
                      .with_debug_name("vibration"),
                  ToggleSwitchStyle::Pill);

    // Section: Circle Style
    div(context, mk(main_container.ent(), 5),
        ComponentConfig{}
            .with_label("Circle Style (Checkbox)")
            .with_size(ComponentSize{percent(1.0f), pixels(28)})
            .with_background(Theme::Usage::Secondary)
            .with_auto_text_color(true)
            .with_padding(Spacing::xs)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_margin(Margin{.top = DefaultSpacing::small(), .bottom = DefaultSpacing::small()})
            .with_skip_tabbing(true)
            .with_debug_name("circle_title"));

    toggle_switch(context, mk(main_container.ent(), 6), dark_mode,
                  ComponentConfig{}
                      .with_label("Dark Mode")
                      .with_size(ComponentSize{percent(0.95f), pixels(36)})
                      .with_background(Theme::Usage::Surface)
                      .with_font(UIComponent::DEFAULT_FONT, 16.0f)
                      .with_margin(Spacing::xs)
                      .with_debug_name("dark_mode"),
                  ToggleSwitchStyle::Circle);

    toggle_switch(context, mk(main_container.ent(), 7), auto_save,
                  ComponentConfig{}
                      .with_label("Auto-Save")
                      .with_size(ComponentSize{percent(0.95f), pixels(36)})
                      .with_background(Theme::Usage::Surface)
                      .with_font(UIComponent::DEFAULT_FONT, 16.0f)
                      .with_margin(Spacing::xs)
                      .with_debug_name("auto_save"),
                  ToggleSwitchStyle::Circle);

    // Status (removed cloud_sync toggle to fit)
    std::string status = fmt::format(
        "Notif:{} Sound:{} Vib:{} Dark:{} Save:{}",
        enable_notifications ? "ON" : "OFF", enable_sound ? "ON" : "OFF",
        enable_vibration ? "ON" : "OFF", dark_mode ? "ON" : "OFF",
        auto_save ? "ON" : "OFF");

    div(context, mk(main_container.ent(), 8),
        ComponentConfig{}
            .with_label(status)
            .with_size(ComponentSize{percent(0.95f), pixels(30)})
            .with_custom_background(theme.surface)
            .with_auto_text_color(true)
            .with_padding(Spacing::xs)
            .with_font(UIComponent::DEFAULT_FONT, 12.0f)
            .with_margin(Margin{.top = DefaultSpacing::small()})
            .with_skip_tabbing(true)
            .with_debug_name("status"));
  }
};

REGISTER_EXAMPLE_SCREEN(toggle_switches, "Component Galleries",
                        "Toggle switch: Pill (iOS) and Circle (X/V) styles",
                        ToggleSwitchShowcase)
