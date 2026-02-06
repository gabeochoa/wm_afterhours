#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct SettingRowShowcase : ScreenSystem<UIContext<InputAction>> {
  // Toggle values
  bool music_enabled = true;
  bool sound_effects = true;
  bool vibration = false;

  // Stepper values
  size_t language_idx = 0;
  std::vector<std::string> languages = {"English", "Spanish", "French",
                                        "German", "Japanese"};

  size_t quality_idx = 2;
  std::vector<std::string> quality_options = {"Low", "Medium", "High", "Ultra"};

  // Slider values
  float master_volume = 0.8f;
  float music_volume = 0.65f;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = afterhours::ui::theme_presets::neon_dark();
    theme.primary = afterhours::Color{85, 175, 125, 255};
    theme.accent = afterhours::Color{65, 155, 220, 255};
    theme.roundness = 0.12f;
    context.theme = theme;

    auto root =
        div(context, mk(entity, 0),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(0.70f), screen_pct(0.80f)})
                .with_self_align(SelfAlign::Center)
                .with_background(Theme::Usage::Background)
                .with_padding(Spacing::lg)
                .with_flex_direction(FlexDirection::Column)
                .with_translate(0.0f, -40.0f)
                .with_debug_name("root"));

    // Title bar
    div(context, mk(root.ent(), 0),
        ComponentConfig{}
            .with_label("Setting Row Showcase")
            .with_size(ComponentSize{percent(0.95f), pixels(52)})
            .with_background(Theme::Usage::Surface)
            .with_auto_text_color(true)
            .with_padding(Spacing::md)
            .with_font(UIComponent::DEFAULT_FONT, h720(20.0f))
            .with_alignment(TextAlignment::Center)
            .with_roundness(0.1f)
            .with_debug_name("title"));

    // Content panel - increased height to prevent clipping
    auto content = div(context, mk(root.ent(), 1),
                       ComponentConfig{}
                           .with_size(ComponentSize{percent(0.95f), pixels(520)})
                           .with_background(Theme::Usage::Surface)
                           .with_padding(Padding{
                               .top = pixels(16),
                               .left = pixels(24),
                               .bottom = pixels(20),
                               .right = pixels(24)})
                           .with_flex_direction(FlexDirection::Column)
                           .with_roundness(0.06f)
                           .with_debug_name("content"));

    // Toggle section label - increased size
    div(context, mk(content.ent(), 0),
        ComponentConfig{}
            .with_label("TOGGLES")
            .with_size(ComponentSize{pixels(140), pixels(28)})
            .with_custom_text_color(afterhours::Color{160, 170, 190, 255})
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_margin(Margin{.bottom = DefaultSpacing::tiny()})
            .with_debug_name("toggle_label"));

    // Toggle rows - simple API (zero config)
    setting_row_toggle(context, mk(content.ent(), 1), "Music", music_enabled);
    setting_row_toggle(context, mk(content.ent(), 2), "Sound Effects", sound_effects);
    setting_row_toggle(context, mk(content.ent(), 3), "Vibration", vibration);

    // Spacer
    div(context, mk(content.ent(), 4),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(8)})
            .with_debug_name("spacer1"));

    // Stepper section label - increased size
    div(context, mk(content.ent(), 5),
        ComponentConfig{}
            .with_label("OPTIONS")
            .with_size(ComponentSize{pixels(140), pixels(28)})
            .with_custom_text_color(afterhours::Color{160, 170, 190, 255})
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_margin(Margin{.bottom = DefaultSpacing::tiny()})
            .with_debug_name("stepper_label"));

    // Stepper rows
    setting_row_stepper(context, mk(content.ent(), 6), "Language", language_idx, languages);
    setting_row_stepper(context, mk(content.ent(), 7), "Graphics", quality_idx, quality_options);

    // Spacer
    div(context, mk(content.ent(), 8),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(8)})
            .with_debug_name("spacer2"));

    // Slider section label - increased size
    div(context, mk(content.ent(), 9),
        ComponentConfig{}
            .with_label("VOLUME")
            .with_size(ComponentSize{pixels(140), pixels(28)})
            .with_custom_text_color(afterhours::Color{160, 170, 190, 255})
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_margin(Margin{.bottom = DefaultSpacing::tiny()})
            .with_debug_name("slider_label"));

    // Slider rows - simple API
    setting_row_slider(context, mk(content.ent(), 10), "Master Volume", master_volume);
    setting_row_slider(context, mk(content.ent(), 11), "Music Volume", music_volume);

    // Footer - increased text size
    div(context, mk(root.ent(), 2),
        ComponentConfig{}
            .with_label("Toggle, Stepper, Slider, Dropdown, Display")
            .with_size(ComponentSize{percent(0.95f), pixels(36)})
            .with_custom_text_color(afterhours::Color{100, 110, 130, 255})
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_alignment(TextAlignment::Center)
            .with_debug_name("footer"));
  }
};

REGISTER_EXAMPLE_SCREEN(setting_row_showcase, "Components",
                        "Setting Row - labeled controls for settings screens",
                        SettingRowShowcase)
