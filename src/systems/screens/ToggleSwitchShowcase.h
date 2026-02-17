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
  // Disabled toggles (non-interactive)
  bool disabled_on = true;
  bool disabled_off = false;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = afterhours::ui::theme_presets::neon_dark();
    context.theme = theme;
    context.scaling_mode = ScalingMode::Adaptive;

    // Full-screen dark background
    auto root =
        div(context, mk(entity, 0),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(1.0f), screen_pct(1.0f)})
                .with_background(Theme::Usage::Background)
                .with_debug_name("toggle_bg"));

    // Centered card (settings panel style)
    // Height increased from 0.85 to 0.95 to fit all sections (General,
    // Preferences, Disabled, Status) without overflowing the card bounds.
    auto card =
        vstack(context, mk(root.ent(), 0),
               ComponentConfig{}
                   .with_size(ComponentSize{screen_pct(0.5f), screen_pct(1.0f)})
                   .with_self_align(SelfAlign::Center)
                   .with_custom_background(
                       afterhours::colors::lighten(theme.background, 0.08f))
                   .with_rounded_corners(RoundedCorners())
                   .with_roundness(0.04f)
                   .with_padding(Padding{.top = DefaultSpacing::small(),
                                         .bottom = DefaultSpacing::small(),
                                         .left = DefaultSpacing::large(),
                                         .right = DefaultSpacing::large()})
                   .with_no_wrap()
                   .with_debug_name("toggle_card"));

    // Title
    div(context, mk(card.ent(), 0),
        ComponentConfig{}
            .with_label("Settings")
            .with_size(ComponentSize{percent(1.0f), pixels(44)})
            .with_font(UIComponent::DEFAULT_FONT, pixels(26.0f))
            .with_color_usage(Theme::Usage::None)
            .with_margin(Margin{.bottom = DefaultSpacing::medium()})
            .with_skip_tabbing(true));

    // ── Pill Style Section ──
    div(context, mk(card.ent(), 1),
        ComponentConfig{}
            .with_label("General")
            .with_size(ComponentSize{percent(1.0f), pixels(32)})
            .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
            .with_custom_text_color(theme.font_muted)
            .with_color_usage(Theme::Usage::None)
            .with_margin(Margin{.bottom = DefaultSpacing::small()})
            .with_skip_tabbing(true));

    auto make_toggle_row = [&](int idx, const std::string &lbl, bool &val) {
      toggle_switch(context, mk(card.ent(), idx), val,
                    ComponentConfig{}
                        .with_label(lbl)
                        .with_size(ComponentSize{percent(1.0f), pixels(42)})
                        .with_custom_background(
                            afterhours::colors::lighten(theme.surface, 0.06f))
                        .with_font(UIComponent::DEFAULT_FONT, pixels(17.0f))
                        .with_padding(Padding{.left = DefaultSpacing::small(),
                                              .right = DefaultSpacing::small()})
                        .with_margin(Margin{.bottom = DefaultSpacing::tiny()})
                        .with_rounded_corners(RoundedCorners())
                        .with_roundness(0.06f));
    };

    // Circle-style checkbox rows (round checkbox with check/X indicator)
    auto make_checkbox_circle_row =
        [&](int idx, const std::string &lbl, bool &val,
            ComponentConfig extra = ComponentConfig{}) {
          checkbox(context, mk(card.ent(), idx), val,
                   ComponentConfig{}
                       .with_label(lbl)
                       .with_size(ComponentSize{percent(1.0f), pixels(42)})
                       .with_custom_background(
                           afterhours::colors::lighten(theme.surface, 0.06f))
                       .with_font(UIComponent::DEFAULT_FONT, pixels(17.0f))
                       .with_margin(Margin{.bottom = DefaultSpacing::tiny()})
                       .with_rounded_corners(RoundedCorners().all_round())
                       .with_disabled(extra.disabled)
                       .with_opacity(extra.opacity));
        };

    // NOTE: Child IDs must be sequential to match visual layout order.
    // Afterhours sorts children by ID for flex layout.
    make_toggle_row(2, "Notifications", enable_notifications);
    make_toggle_row(3, "Sound Effects", enable_sound);
    make_toggle_row(4, "Vibration", enable_vibration);

    // Separator
    div(context, mk(card.ent(), 5),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(1)})
            .with_custom_background(afterhours::Color{255, 255, 255, 20})
            .with_margin(Margin{.top = DefaultSpacing::small(),
                                .bottom = DefaultSpacing::small()})
            .with_skip_tabbing(true));

    // ── Circle Style Section ──
    div(context, mk(card.ent(), 6),
        ComponentConfig{}
            .with_label("Preferences")
            .with_size(ComponentSize{percent(1.0f), pixels(32)})
            .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
            .with_custom_text_color(theme.font_muted)
            .with_color_usage(Theme::Usage::None)
            .with_margin(Margin{.bottom = DefaultSpacing::small()})
            .with_skip_tabbing(true));

    make_checkbox_circle_row(7, "Dark Mode", dark_mode);
    make_checkbox_circle_row(8, "Auto-Save", auto_save);
    make_checkbox_circle_row(9, "Cloud Sync", cloud_sync);

    // Separator
    div(context, mk(card.ent(), 10),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(1)})
            .with_custom_background(afterhours::Color{255, 255, 255, 20})
            .with_margin(Margin{.top = DefaultSpacing::small(),
                                .bottom = DefaultSpacing::small()})
            .with_skip_tabbing(true));

    // ── Disabled Section ──
    div(context, mk(card.ent(), 11),
        ComponentConfig{}
            .with_label("Disabled")
            .with_size(ComponentSize{percent(1.0f), pixels(32)})
            .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
            .with_custom_text_color(theme.font_muted)
            .with_color_usage(Theme::Usage::None)
            .with_margin(Margin{.bottom = DefaultSpacing::small()})
            .with_skip_tabbing(true));

    // Disabled pill toggle (ON state, non-interactive)
    toggle_switch(context, mk(card.ent(), 12), disabled_on,
                  ComponentConfig{}
                      .with_label("Locked Setting (ON)")
                      .with_size(ComponentSize{percent(1.0f), pixels(42)})
                      .with_custom_background(
                          afterhours::colors::lighten(theme.surface, 0.06f))
                      .with_font(UIComponent::DEFAULT_FONT, pixels(18.0f))
                      .with_padding(Padding{.left = DefaultSpacing::small(),
                                            .right = DefaultSpacing::small()})
                      .with_margin(Margin{.bottom = DefaultSpacing::tiny()})
                      .with_rounded_corners(RoundedCorners())
                      .with_roundness(0.06f)
                      .with_disabled(true)
                      .with_opacity(0.45f));

    // Disabled circle checkbox (OFF state, non-interactive)
    make_checkbox_circle_row(
        13, "Unavailable Option (OFF)", disabled_off,
        ComponentConfig{}.with_disabled(true).with_opacity(0.45f));

    // Status bar removed — toggle states are already visually clear from
    // the toggle controls themselves. The bar was being pushed outside
    // the card because the total content (title + sections + separators +
    // toggles + checkboxes + disabled row) exceeded the card height.
  }
};

REGISTER_EXAMPLE_SCREEN(toggle_switches, "Component Galleries",
                        "Toggle switch (Pill) and round checkbox styles",
                        ToggleSwitchShowcase)
