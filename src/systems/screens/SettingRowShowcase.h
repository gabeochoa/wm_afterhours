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

    // Main container - centered panel
    auto root =
        div(context, mk(entity, 0),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(0.5f), screen_pct(0.85f)})
                .with_custom_background(theme.background)
                .with_padding(Spacing::lg)
                .with_flex_direction(FlexDirection::Column)
                .with_debug_name("root"));

    // Title bar
    div(context, mk(root.ent(), 0),
        ComponentConfig{}
            .with_label("Setting Row Showcase")
            .with_size(ComponentSize{percent(0.95f), pixels(45)})
            .with_custom_background(theme.surface)
            .with_auto_text_color(true)
            .with_padding(Spacing::md)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_alignment(TextAlignment::Center)
            .with_margin(Margin{.bottom = DefaultSpacing::medium()})
            .with_roundness(0.1f)
            .with_debug_name("title"));

    // Content panel - use explicit padding to contain all children
    auto content = div(context, mk(root.ent(), 1),
                       ComponentConfig{}
                           .with_size(ComponentSize{percent(0.95f), children()})
                           .with_custom_background(theme.surface)
                           .with_padding(Padding{
                               .top = pixels(20),
                               .left = pixels(30),
                               .bottom = pixels(20),
                               .right = pixels(30)})
                           .with_flex_direction(FlexDirection::Column)
                           .with_roundness(0.06f)
                           .with_debug_name("content"));

    // Toggle section label
    div(context, mk(content.ent(), 0),
        ComponentConfig{}
            .with_label("TOGGLES")
            .with_size(ComponentSize{pixels(120), pixels(28)})
            .with_custom_text_color(afterhours::Color{160, 170, 190, 255})
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_margin(Margin{.bottom = pixels(10)})
            .with_debug_name("toggle_label"));

    // Toggle rows - simple API (zero config)
    setting_row_toggle(context, mk(content.ent(), 1), "Music", music_enabled);
    setting_row_toggle(context, mk(content.ent(), 2), "Sound Effects", sound_effects);

    // Toggle with custom label styling via slot config (purple text)
    setting_row(
        context, mk(content.ent(), 3),
        SettingRowConfig{}
            .with_label("Vibration")
            .with_control_type(SettingRowControlType::Toggle)
            .with_label_config(ComponentConfig{}
                .with_custom_text_color(afterhours::Color{200, 120, 255, 255})),
        &vibration);

    // Spacer
    div(context, mk(content.ent(), 4),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(16)})
            .with_debug_name("spacer1"));

    // Stepper section label
    div(context, mk(content.ent(), 5),
        ComponentConfig{}
            .with_label("OPTIONS")
            .with_size(ComponentSize{pixels(120), pixels(28)})
            .with_custom_text_color(afterhours::Color{160, 170, 190, 255})
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_margin(Margin{.bottom = pixels(10)})
            .with_debug_name("stepper_label"));

    // Stepper rows
    setting_row_stepper(context, mk(content.ent(), 6), "Language", language_idx, languages);
    setting_row_stepper(context, mk(content.ent(), 7), "Graphics", quality_idx, quality_options);

    // Spacer
    div(context, mk(content.ent(), 8),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(16)})
            .with_debug_name("spacer2"));

    // Slider section label
    div(context, mk(content.ent(), 9),
        ComponentConfig{}
            .with_label("VOLUME")
            .with_size(ComponentSize{pixels(120), pixels(28)})
            .with_custom_text_color(afterhours::Color{160, 170, 190, 255})
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_margin(Margin{.bottom = pixels(10)})
            .with_debug_name("slider_label"));

    // Slider rows - simple API
    setting_row_slider(context, mk(content.ent(), 10), "Master Volume", master_volume);

    // Slider with custom label styling via slot config (orange text)
    setting_row(
        context, mk(content.ent(), 11),
        SettingRowConfig{}
            .with_label("Music Volume")
            .with_control_type(SettingRowControlType::Slider)
            .with_slider_width(180.0f)
            .with_label_config(ComponentConfig{}
                .with_custom_text_color(afterhours::Color{255, 180, 80, 255})),
        &music_volume);

    // Footer
    div(context, mk(root.ent(), 2),
        ComponentConfig{}
            .with_label("setting_row: Toggle, Stepper, Slider, Dropdown, Display")
            .with_size(ComponentSize{percent(0.95f), pixels(28)})
            .with_custom_text_color(afterhours::Color{100, 110, 130, 255})
            .with_font(UIComponent::DEFAULT_FONT, 12.0f)
            .with_margin(Margin{.top = DefaultSpacing::small()})
            .with_alignment(TextAlignment::Center)
            .with_debug_name("footer"));
  }
};

REGISTER_EXAMPLE_SCREEN(setting_row_showcase, "Components",
                        "Setting Row - labeled controls for settings screens",
                        SettingRowShowcase)
