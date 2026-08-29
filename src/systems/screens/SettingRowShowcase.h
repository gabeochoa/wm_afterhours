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

  // One colour: the green/blue/red headers marked no semantic difference.
  afterhours::Color section_header{150, 165, 190, 255};

  // Helper to build a slider row with percentage value label
  void slider_row_with_pct(UIContext<InputAction> &context,
                           EntityParent ep_pair, const char *label_text,
                           float &value, const char *debug_prefix) {
    // Row container
    auto row = hstack(context, ep_pair,
                      ComponentConfig{}
                          .with_size(ComponentSize{percent(1.0f), pixels(44)})
                          .with_align_items(AlignItems::Center)
                          .with_margin(Margin{.bottom = pixels(6)})
                          .with_debug_name(std::string(debug_prefix) + "_row"));

    // Label
    div(context, mk(row.ent(), 0),
        ComponentConfig{}
            .with_label(label_text)
            .with_size(ComponentSize{percent(0.35f), pixels(44)})
            .with_alignment(TextAlignment::Left)
            .with_background(Theme::Usage::None)
            .with_custom_text_color(context.theme.font)
            .with_font_size(pixels(22.0f))
            .with_debug_name(std::string(debug_prefix) + "_label"));

    // Slider control (no label on handle to avoid the raw number)
    float slider_value = value;
    if (auto result =
            slider(context, mk(row.ent(), 1), slider_value,
                   ComponentConfig{}
                       .with_size(ComponentSize{percent(0.45f), pixels(28)})
                       .with_debug_name(std::string(debug_prefix) + "_slider"),
                   SliderHandleValueLabelPosition::None);
        result) {
      value = slider_value;
    }

    // Percentage value label
    std::string pct_text = std::to_string(static_cast<int>(value * 100)) + "%";
    div(context, mk(row.ent(), 2),
        ComponentConfig{}
            .with_label(pct_text)
            .with_size(ComponentSize{pixels(52), pixels(44)})
            .with_alignment(TextAlignment::Right)
            .with_background(Theme::Usage::None)
            .with_custom_text_color(afterhours::Color{180, 200, 220, 255})
            .with_font_size(pixels(16.0f))
            .with_debug_name(std::string(debug_prefix) + "_pct"));
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = afterhours::ui::theme_presets::neon_dark();
    theme.primary = afterhours::Color{85, 175, 125, 255};
    theme.accent = afterhours::Color{65, 155, 220, 255};
    // Neutral dark gray for secondary so toggle OFF state is clearly muted
    theme.secondary = afterhours::Color{45, 48, 58, 255};
    theme.roundness = 0.12f;
    context.theme = theme;
    context.scaling_mode = ScalingMode::Adaptive;
    UIStylingDefaults::get().set_default_font(UIComponent::DEFAULT_FONT,
                                              pixels(16.0f));

    auto root = vstack(
        context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(0.76f), screen_pct(0.90f)})
            .with_self_align(SelfAlign::Center)
            .with_background(Theme::Usage::Background)
            .with_padding(Spacing::sm)
            .with_no_wrap()
            .with_debug_name("root"));

    // Title bar
    div(context, mk(root.ent(), 0),
        ComponentConfig{}
            .with_label("Setting Row Showcase")
            .with_size(ComponentSize{percent(0.95f), pixels(52)})
            .with_background(Theme::Usage::Surface)
            .with_auto_text_color(true)
            .with_padding(Spacing::md)
            .with_font_size(pixels(20.0f))
            .with_alignment(TextAlignment::Center)
            .with_roundness(0.1f));

    // Content panel - increased height to prevent clipping
    auto content =
        vstack(context, mk(root.ent(), 1),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(0.95f), percent(0.85f)})
                   .with_background(Theme::Usage::Surface)
                   .with_padding(Padding{.top = pixels(10),
                                         .left = pixels(16),
                                         .bottom = pixels(10),
                                         .right = pixels(16)})
                   .with_no_wrap()
                   .with_roundness(0.06f)
                   .with_debug_name("content"));

    // Toggle section label - tinted to match toggle green
    div(context, mk(content.ent(), 0),
        ComponentConfig{}
            .with_label("TOGGLES")
            .with_size(ComponentSize{pixels(140), pixels(28)})
            .with_custom_text_color(section_header)
            .with_font_size(pixels(18.0f))
            .with_alignment(TextAlignment::Left)
            .with_margin(Margin{.bottom = DefaultSpacing::tiny()}));

    // Toggle rows - use full API with icons for clear visual context
    setting_row_toggle(context, mk(content.ent(), 1), "Music", music_enabled);
    setting_row_toggle(context, mk(content.ent(), 2), "Sound Effects",
                       sound_effects);
    setting_row_toggle(context, mk(content.ent(), 3), "Vibration", vibration);

    // Spacer
    div(context, mk(content.ent(), 4),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(24)})
            .with_debug_name("spacer1"));

    // Stepper section label - tinted to match stepper blue
    div(context, mk(content.ent(), 5),
        ComponentConfig{}
            .with_label("OPTIONS")
            .with_size(ComponentSize{pixels(140), pixels(28)})
            .with_custom_text_color(section_header)
            .with_font_size(pixels(18.0f))
            .with_alignment(TextAlignment::Left)
            .with_margin(Margin{.bottom = DefaultSpacing::tiny()}));

    // Stepper rows
    setting_row_stepper(context, mk(content.ent(), 6), "Language", language_idx,
                        languages);
    setting_row_stepper(context, mk(content.ent(), 7), "Graphics", quality_idx,
                        quality_options);

    // Spacer
    div(context, mk(content.ent(), 8),
        ComponentConfig{}.with_size(ComponentSize{percent(1.0f), pixels(24)}));

    // Slider section label - tinted warm to match volume/slider theme
    div(context, mk(content.ent(), 9),
        ComponentConfig{}
            .with_label("VOLUME")
            .with_size(ComponentSize{pixels(140), pixels(28)})
            .with_custom_text_color(section_header)
            .with_font_size(pixels(18.0f))
            .with_alignment(TextAlignment::Left)
            .with_margin(Margin{.bottom = DefaultSpacing::tiny()}));

    // Slider rows - custom rows with percentage value labels
    slider_row_with_pct(context, mk(content.ent(), 10), "Master Volume",
                        master_volume, "master_vol");
    slider_row_with_pct(context, mk(content.ent(), 11), "Music Volume",
                        music_volume, "music_vol");

    // Footer - increased text size
    div(context, mk(root.ent(), 2),
        ComponentConfig{}
            .with_label("Toggle, Stepper, Slider, Dropdown, Display")
            .with_size(ComponentSize{percent(0.95f), pixels(36)})
            .with_custom_text_color(afterhours::Color{100, 110, 130, 255})
            .with_font_size(pixels(18.0f))
            .with_alignment(TextAlignment::Center));
  }
};

REGISTER_EXAMPLE_SCREEN(setting_row_showcase, "Component Galleries",
                        "Setting Row - labeled controls for settings screens",
                        SettingRowShowcase)
