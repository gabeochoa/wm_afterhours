#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ToggleSwitchShowcase : ScreenSystem<UIContext<InputAction>> {
  // Settings toggles
  bool enable_notifications = true;
  bool enable_sound = false;
  bool enable_vibration = true;
  bool dark_mode = true;
  bool auto_save = false;
  bool cloud_sync = true;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = afterhours::ui::theme_presets::cozy_kraft();
    context.theme = theme;

    // Main container
    auto root =
        div(context, mk(entity, 0),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(0.50f), screen_pct(0.70f)})
                .with_custom_background(theme.background)
                .with_roundness(0.08f)
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
            .with_label("Toggle Switch (iOS-style)")
            .with_size(ComponentSize{percent(1.0f), pixels(50)})
            .with_custom_background(theme.surface)
            .with_auto_text_color(true)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, 24.0f)
            .with_margin(Margin{.top = pixels(0),
                                .bottom = DefaultSpacing::medium(),
                                .left = pixels(0),
                                .right = pixels(0)})
            .with_debug_name("title"));

    // Toggle rows
    toggle_switch(context, mk(main_container.ent(), 1), enable_notifications,
                  ComponentConfig{}
                      .with_label("Notifications")
                      .with_size(ComponentSize{percent(0.90f), pixels(40)})
                      .with_font(UIComponent::DEFAULT_FONT, 18.0f)
                      .with_margin(Spacing::sm)
                      .with_debug_name("notifications"));

    toggle_switch(context, mk(main_container.ent(), 2), enable_sound,
                  ComponentConfig{}
                      .with_label("Sound Effects")
                      .with_size(ComponentSize{percent(0.90f), pixels(40)})
                      .with_font(UIComponent::DEFAULT_FONT, 18.0f)
                      .with_margin(Spacing::sm)
                      .with_debug_name("sound"));

    toggle_switch(context, mk(main_container.ent(), 3), enable_vibration,
                  ComponentConfig{}
                      .with_label("Vibration")
                      .with_size(ComponentSize{percent(0.90f), pixels(40)})
                      .with_font(UIComponent::DEFAULT_FONT, 18.0f)
                      .with_margin(Spacing::sm)
                      .with_debug_name("vibration"));

    toggle_switch(context, mk(main_container.ent(), 4), dark_mode,
                  ComponentConfig{}
                      .with_label("Dark Mode")
                      .with_size(ComponentSize{percent(0.90f), pixels(40)})
                      .with_font(UIComponent::DEFAULT_FONT, 18.0f)
                      .with_margin(Spacing::sm)
                      .with_debug_name("dark_mode"));

    toggle_switch(context, mk(main_container.ent(), 5), auto_save,
                  ComponentConfig{}
                      .with_label("Auto-Save")
                      .with_size(ComponentSize{percent(0.90f), pixels(40)})
                      .with_font(UIComponent::DEFAULT_FONT, 18.0f)
                      .with_margin(Spacing::sm)
                      .with_debug_name("auto_save"));

    toggle_switch(context, mk(main_container.ent(), 6), cloud_sync,
                  ComponentConfig{}
                      .with_label("Cloud Sync")
                      .with_size(ComponentSize{percent(0.90f), pixels(40)})
                      .with_font(UIComponent::DEFAULT_FONT, 18.0f)
                      .with_margin(Spacing::sm)
                      .with_debug_name("cloud_sync"));

    // Status
    std::string status = fmt::format(
        "Notif:{} Sound:{} Vib:{} Dark:{} Save:{} Cloud:{}",
        enable_notifications ? "ON" : "OFF", enable_sound ? "ON" : "OFF",
        enable_vibration ? "ON" : "OFF", dark_mode ? "ON" : "OFF",
        auto_save ? "ON" : "OFF", cloud_sync ? "ON" : "OFF");

    div(context, mk(main_container.ent(), 7),
        ComponentConfig{}
            .with_label(status)
            .with_size(ComponentSize{percent(0.95f), pixels(40)})
            .with_custom_background(theme.surface)
            .with_auto_text_color(true)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_margin(Margin{.top = DefaultSpacing::medium(),
                                .bottom = pixels(0),
                                .left = pixels(0),
                                .right = pixels(0)})
            .with_skip_tabbing(true)
            .with_debug_name("status"));
  }
};

REGISTER_EXAMPLE_SCREEN(toggle_switches, "Component Galleries",
                        "iOS-style toggle switch with animated knob",
                        ToggleSwitchShowcase)
