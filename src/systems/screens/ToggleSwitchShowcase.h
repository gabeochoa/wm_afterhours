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
    auto theme = afterhours::ui::theme_presets::neon_dark();
    context.theme = theme;

    // Full-screen dark background
    auto root =
        div(context, mk(entity, 0),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(1.0f), screen_pct(1.0f)})
                .with_background(Theme::Usage::Background)
                .with_debug_name("toggle_bg"));

    // Centered card (settings panel style)
    auto card =
        div(context, mk(root.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(0.5f), screen_pct(0.85f)})
                .with_self_align(SelfAlign::Center)
                .with_custom_background(
                    afterhours::colors::lighten(theme.background, 0.08f))
                .with_rounded_corners(RoundedCorners())
                .with_roundness(0.04f)
                .with_padding(Padding{
                    .top = DefaultSpacing::medium(),
                    .bottom = DefaultSpacing::medium(),
                    .left = DefaultSpacing::large(),
                    .right = DefaultSpacing::large()})
                .with_flex_direction(FlexDirection::Column)
                .with_debug_name("toggle_card"));

    // Title
    div(context, mk(card.ent(), 0),
        ComponentConfig{}
            .with_label("Settings")
            .with_size(ComponentSize{percent(1.0f), pixels(44)})
            .with_font(UIComponent::DEFAULT_FONT, h720(26.0f))
            .with_color_usage(Theme::Usage::None)
            .with_margin(Margin{.bottom = DefaultSpacing::medium()})
            .with_skip_tabbing(true));

    // ── Pill Style Section ──
    div(context, mk(card.ent(), 1),
        ComponentConfig{}
            .with_label("General")
            .with_size(ComponentSize{percent(1.0f), pixels(32)})
            .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
            .with_custom_text_color(theme.font_muted)
            .with_color_usage(Theme::Usage::None)
            .with_margin(Margin{.bottom = DefaultSpacing::small()})
            .with_skip_tabbing(true));

    auto make_toggle_row = [&](int idx, const std::string &lbl, bool &val,
                               ToggleSwitchStyle s = ToggleSwitchStyle::Pill) {
      toggle_switch(
          context, mk(card.ent(), idx), val,
          ComponentConfig{}
              .with_label(lbl)
              .with_size(ComponentSize{percent(1.0f), pixels(48)})
              .with_custom_background(
                  afterhours::colors::lighten(theme.surface, 0.06f))
              .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
              .with_padding(Padding{.left = DefaultSpacing::small(),
                                    .right = DefaultSpacing::small()})
              .with_margin(Margin{.bottom = DefaultSpacing::tiny()})
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.06f),
          s);
    };

    make_toggle_row(2, "Notifications", enable_notifications);
    make_toggle_row(3, "Sound Effects", enable_sound);
    make_toggle_row(4, "Vibration", enable_vibration);

    // Separator
    div(context, mk(card.ent(), 10),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(1)})
            .with_custom_background(
                afterhours::Color{255, 255, 255, 20})
            .with_margin(Margin{.top = DefaultSpacing::small(),
                                .bottom = DefaultSpacing::small()})
            .with_skip_tabbing(true));

    // ── Circle Style Section ──
    div(context, mk(card.ent(), 5),
        ComponentConfig{}
            .with_label("Preferences")
            .with_size(ComponentSize{percent(1.0f), pixels(32)})
            .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
            .with_custom_text_color(theme.font_muted)
            .with_color_usage(Theme::Usage::None)
            .with_margin(Margin{.bottom = DefaultSpacing::small()})
            .with_skip_tabbing(true));

    make_toggle_row(6, "Dark Mode", dark_mode, ToggleSwitchStyle::Circle);
    make_toggle_row(7, "Auto-Save", auto_save, ToggleSwitchStyle::Circle);
    make_toggle_row(8, "Cloud Sync", cloud_sync, ToggleSwitchStyle::Circle);

    // Status bar
    std::string status = fmt::format(
        "Notifications: {}   Sound: {}   Dark Mode: {}",
        enable_notifications ? "ON" : "OFF",
        enable_sound ? "ON" : "OFF",
        dark_mode ? "ON" : "OFF");

    div(context, mk(card.ent(), 9),
        ComponentConfig{}
            .with_label(status)
            .with_size(ComponentSize{percent(1.0f), pixels(40)})
            .with_custom_background(
                afterhours::colors::lighten(theme.background, 0.04f))
            .with_custom_text_color(theme.font_muted)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.04f)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_margin(Margin{.top = DefaultSpacing::medium()})
            .with_skip_tabbing(true));
  }
};

REGISTER_EXAMPLE_SCREEN(toggle_switches, "Component Galleries",
                        "Toggle switch: Pill (iOS) and Circle (X/V) styles",
                        ToggleSwitchShowcase)
