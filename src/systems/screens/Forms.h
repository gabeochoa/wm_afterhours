#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/clipboard.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct FormsGallery : ScreenSystem<UIContext<InputAction>> {
  // Slider values
  float volume_slider = 0.75f;
  float brightness_slider = 0.5f;
  float difficulty_slider = 0.3f;

  // Checkbox values
  bool enable_music = true;
  bool enable_sfx = true;
  bool fullscreen = false;
  bool vsync = true;
  bool show_fps = false;

  // Dropdown values
  size_t resolution_index = 2;
  size_t quality_index = 1;
  size_t language_index = 0;

  std::vector<std::string> resolutions = {"640x480", "1280x720", "1920x1080",
                                          "2560x1440", "3840x2160"};
  std::vector<std::string> quality_options = {"Low", "Medium", "High", "Ultra"};
  std::vector<std::string> languages = {"English", "Spanish",  "French",
                                        "German",  "Japanese", "Korean"};

  // Clipboard demo state
  std::string clipboard_display = "(clipboard empty)";

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    // Apply neon dark theme for this screen
    // TODO: Add font configuration when fonts are selected
    auto theme = afterhours::ui::theme_presets::neon_dark();
    context.theme = theme;

    // Main container background (no padding so it stays in bounds)
    auto root =
        div(context, mk(entity, 0),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(0.92f), screen_pct(0.88f)})
                .with_custom_background(theme.background)
                .with_roundness(0.08f)
                .with_debug_name("forms_bg"));

    // Content container with padding
    auto main_container =
        div(context, mk(root.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), percent(1.0f)})
                .with_padding(Spacing::lg)
                .with_flex_direction(FlexDirection::Column)
                .with_debug_name("forms_main"));

    // Title
    div(context, mk(main_container.ent(), 0),
        ComponentConfig{}
            .with_label("Form Components Gallery")
            .with_size(ComponentSize{percent(1.0f), pixels(50)})
            .with_custom_background(theme.surface)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, 28.0f)
            .with_margin(Margin{.top = pixels(0),
                                .bottom = DefaultSpacing::medium(),
                                .left = pixels(0),
                                .right = pixels(0)})
            .with_debug_name("title"));

    // Content area - two columns
    auto content =
        div(context, mk(main_container.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{percent(0.95f), pixels(480)})
                .with_custom_background(theme.surface)
                .with_padding(Spacing::md)
                .with_flex_direction(FlexDirection::Row)
                .with_justify_content(JustifyContent::Center)
                .with_debug_name("content"));

    // Left column - Sliders
    auto left_col = div(
        context, mk(content.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(450), pixels(430)})
            .with_custom_background(afterhours::colors::darken(theme.surface, 0.9f))
            .with_padding(Spacing::sm)
            .with_flex_direction(FlexDirection::Column)
            .with_margin(Margin{.top = pixels(0),
                                .bottom = pixels(0),
                                .left = pixels(0),
                                .right = DefaultSpacing::small()})
            .with_debug_name("left_column"));

    // Sliders section header
    div(context, mk(left_col.ent(), 0),
        ComponentConfig{}
            .with_label("Sliders")
            .with_size(ComponentSize{pixels(300), pixels(32)})
            .with_background(Theme::Usage::Primary)
            .with_padding(Spacing::xs)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_skip_tabbing(true)
            .with_margin(Spacing::xs)
            .with_debug_name("sliders_header"));

    // Volume slider - use pixels for better control
    slider(context, mk(left_col.ent(), 1), volume_slider,
           ComponentConfig{}
               .with_label("Volume")
               .with_size(ComponentSize{pixels(300), pixels(40)})
               .with_background(Theme::Usage::Primary)
               .with_font(UIComponent::DEFAULT_FONT, 18.0f)
               .with_margin(Spacing::sm)
               .with_debug_name("volume_slider"),
           SliderHandleValueLabelPosition::WithLabel);

    // Brightness slider
    slider(context, mk(left_col.ent(), 2), brightness_slider,
           ComponentConfig{}
               .with_label("Brightness")
               .with_size(ComponentSize{pixels(300), pixels(40)})
               .with_background(Theme::Usage::Accent)
               .with_font(UIComponent::DEFAULT_FONT, 18.0f)
               .with_margin(Spacing::sm)
               .with_debug_name("brightness_slider"),
           SliderHandleValueLabelPosition::WithLabel);

    // Difficulty slider
    slider(context, mk(left_col.ent(), 3), difficulty_slider,
           ComponentConfig{}
               .with_label("Difficulty")
               .with_size(ComponentSize{pixels(300), pixels(40)})
               .with_background(Theme::Usage::Secondary)
               .with_font(UIComponent::DEFAULT_FONT, 18.0f)
               .with_margin(Spacing::sm)
               .with_debug_name("difficulty_slider"),
           SliderHandleValueLabelPosition::WithLabel);

    // Progress Bars section header
    div(context, mk(left_col.ent(), 4),
        ComponentConfig{}
            .with_label("Progress Bars")
            .with_size(ComponentSize{pixels(300), pixels(32)})
            .with_background(Theme::Usage::Accent)
            .with_padding(Spacing::xs)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_skip_tabbing(true)
            .with_margin(Margin{.top = DefaultSpacing::small(),
                                .bottom = DefaultSpacing::small(),
                                .left = pixels(0),
                                .right = pixels(0)})
            .with_debug_name("progress_header"));

    // Progress bar showing volume value (dynamic)
    progress_bar(context, mk(left_col.ent(), 5), volume_slider,
                 ComponentConfig{}
                     .with_label("Audio Level")
                     .with_size(ComponentSize{pixels(300), pixels(30)})
                     .with_font(UIComponent::DEFAULT_FONT, 14.0f)
                     .with_margin(Spacing::xs)
                     .with_debug_name("volume_progress"),
                 ProgressBarLabelStyle::Percentage);

    // Progress bar with custom range
    progress_bar(context, mk(left_col.ent(), 6), 75.f,
                 ComponentConfig{}
                     .with_label("Level Progress")
                     .with_size(ComponentSize{pixels(300), pixels(30)})
                     .with_font(UIComponent::DEFAULT_FONT, 14.0f)
                     .with_margin(Spacing::xs)
                     .with_debug_name("level_progress"),
                 ProgressBarLabelStyle::Fraction, 0.f, 100.f);

    // Dropdowns section header
    div(context, mk(left_col.ent(), 7),
        ComponentConfig{}
            .with_label("Dropdowns")
            .with_size(ComponentSize{pixels(300), pixels(32)})
            .with_background(Theme::Usage::Primary)
            .with_padding(Spacing::xs)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_skip_tabbing(true)
            .with_margin(Margin{.top = DefaultSpacing::small(),
                                .bottom = DefaultSpacing::small(),
                                .left = pixels(0),
                                .right = pixels(0)})
            .with_debug_name("dropdowns_header"));

    // Resolution dropdown
    dropdown(context, mk(left_col.ent(), 8), resolutions, resolution_index,
             ComponentConfig{}
                 .with_label("Resolution")
                 .with_size(ComponentSize{pixels(300), pixels(40)})
                 .with_background(Theme::Usage::Primary)
                 .with_font(UIComponent::DEFAULT_FONT, 18.0f)
                 .with_margin(Spacing::sm)
                 .with_debug_name("resolution_dropdown"));

    // Quality dropdown
    dropdown(context, mk(left_col.ent(), 9), quality_options, quality_index,
             ComponentConfig{}
                 .with_label("Quality")
                 .with_size(ComponentSize{pixels(300), pixels(40)})
                 .with_background(Theme::Usage::Accent)
                 .with_font(UIComponent::DEFAULT_FONT, 18.0f)
                 .with_margin(Spacing::sm)
                 .with_debug_name("quality_dropdown"));

    // Right column - Checkboxes
    auto right_col = div(
        context, mk(content.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(350), pixels(430)})
            .with_custom_background(afterhours::colors::darken(theme.surface, 0.9f))
            .with_padding(Spacing::sm)
            .with_flex_direction(FlexDirection::Column)
            .with_debug_name("right_column"));

    // Checkboxes section header
    div(context, mk(right_col.ent(), 0),
        ComponentConfig{}
            .with_label("Checkboxes")
            .with_size(ComponentSize{pixels(280), pixels(32)})
            .with_background(Theme::Usage::Primary)
            .with_padding(Spacing::xs)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_skip_tabbing(true)
            .with_margin(Spacing::xs)
            .with_debug_name("checkboxes_header"));

    // Audio checkboxes - use fixed pixel sizes for consistency
    checkbox(context, mk(right_col.ent(), 1), enable_music,
             ComponentConfig{}
                 .with_label("Enable Music")
                 .with_size(ComponentSize{pixels(280), pixels(34)})
                 .with_background(Theme::Usage::Primary)
                 .with_font(UIComponent::DEFAULT_FONT, 16.0f)
                 .with_margin(Spacing::xs)
                 .with_debug_name("music_checkbox"));

    checkbox(context, mk(right_col.ent(), 2), enable_sfx,
             ComponentConfig{}
                 .with_label("Enable SFX")
                 .with_size(ComponentSize{pixels(280), pixels(34)})
                 .with_background(Theme::Usage::Primary)
                 .with_font(UIComponent::DEFAULT_FONT, 16.0f)
                 .with_margin(Spacing::xs)
                 .with_debug_name("sfx_checkbox"));

    // Display checkboxes
    checkbox(context, mk(right_col.ent(), 3), fullscreen,
             ComponentConfig{}
                 .with_label("Fullscreen")
                 .with_size(ComponentSize{pixels(280), pixels(34)})
                 .with_background(Theme::Usage::Accent)
                 .with_font(UIComponent::DEFAULT_FONT, 16.0f)
                 .with_margin(Spacing::xs)
                 .with_debug_name("fullscreen_checkbox"));

    checkbox(context, mk(right_col.ent(), 4), vsync,
             ComponentConfig{}
                 .with_label("V-Sync")
                 .with_size(ComponentSize{pixels(280), pixels(34)})
                 .with_background(Theme::Usage::Accent)
                 .with_font(UIComponent::DEFAULT_FONT, 16.0f)
                 .with_margin(Spacing::xs)
                 .with_debug_name("vsync_checkbox"));

    checkbox(context, mk(right_col.ent(), 5), show_fps,
             ComponentConfig{}
                 .with_label("Show FPS")
                 .with_size(ComponentSize{pixels(280), pixels(34)})
                 .with_background(Theme::Usage::Secondary)
                 .with_font(UIComponent::DEFAULT_FONT, 16.0f)
                 .with_margin(Spacing::xs)
                 .with_debug_name("fps_checkbox"));

    // Disabled checkbox example
    bool disabled_value = true;
    checkbox(context, mk(right_col.ent(), 6), disabled_value,
             ComponentConfig{}
                 .with_label("Disabled Option")
                 .with_size(ComponentSize{pixels(280), pixels(34)})
                 .with_background(Theme::Usage::Primary)
                 .with_disabled(true)
                 .with_font(UIComponent::DEFAULT_FONT, 16.0f)
                 .with_margin(Spacing::xs)
                 .with_debug_name("disabled_checkbox"));

    // Language dropdown
    dropdown(context, mk(right_col.ent(), 7), languages, language_index,
             ComponentConfig{}
                 .with_label("Language")
                 .with_size(ComponentSize{pixels(280), pixels(40)})
                 .with_background(Theme::Usage::Secondary)
                 .with_font(UIComponent::DEFAULT_FONT, 18.0f)
                 .with_margin(Margin{.top = DefaultSpacing::small(),
                                     .bottom = pixels(0),
                                     .left = pixels(0),
                                     .right = pixels(0)})
                 .with_debug_name("language_dropdown"));

    // Status display
    std::string status =
        "Volume: " + std::to_string(static_cast<int>(volume_slider * 100)) +
        "% | Resolution: " + resolutions[resolution_index] +
        " | Quality: " + quality_options[quality_index];

    div(context, mk(main_container.ent(), 2),
        ComponentConfig{}
            .with_label(status)
            .with_size(ComponentSize{percent(1.0f), pixels(40)})
            .with_custom_background(theme.surface)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, 20.0f)
            .with_margin(Margin{.top = DefaultSpacing::small(),
                                .bottom = pixels(0),
                                .left = pixels(0),
                                .right = pixels(0)})
            .with_debug_name("status"));

    // Clipboard demo section
    auto clipboard_row =
        div(context, mk(main_container.ent(), 3),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), pixels(50)})
                .with_flex_direction(FlexDirection::Row)
                .with_justify_content(JustifyContent::SpaceAround)
                .with_margin(Margin{.top = DefaultSpacing::small(),
                                    .bottom = pixels(0),
                                    .left = pixels(0),
                                    .right = pixels(0)})
                .with_debug_name("clipboard_row"));

    // Copy button - copies current status to clipboard
    if (button(context, mk(clipboard_row.ent(), 0),
               ComponentConfig{}
                   .with_label("Copy Status")
                   .with_size(ComponentSize{pixels(150), pixels(40)})
                   .with_background(Theme::Usage::Primary)
                   .with_font(UIComponent::DEFAULT_FONT, 16.0f)
                   .with_debug_name("copy_btn"))) {
      afterhours::clipboard::set_text(status);
      clipboard_display = "Copied!";
    }

    // Paste button - reads from clipboard
    if (button(context, mk(clipboard_row.ent(), 1),
               ComponentConfig{}
                   .with_label("Paste")
                   .with_size(ComponentSize{pixels(150), pixels(40)})
                   .with_background(Theme::Usage::Accent)
                   .with_font(UIComponent::DEFAULT_FONT, 16.0f)
                   .with_debug_name("paste_btn"))) {
      if (afterhours::clipboard::has_text()) {
        clipboard_display = afterhours::clipboard::get_text();
      } else {
        clipboard_display = "(clipboard empty)";
      }
    }

    // Display clipboard contents (truncated if too long)
    std::string display_text = clipboard_display;
    if (display_text.length() > 60) {
      display_text = display_text.substr(0, 57) + "...";
    }

    div(context, mk(clipboard_row.ent(), 2),
        ComponentConfig{}
            .with_label(display_text)
            .with_size(ComponentSize{pixels(400), pixels(40)})
            .with_custom_background(theme.surface)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_debug_name("clipboard_display"));
  }
};

REGISTER_EXAMPLE_SCREEN(forms, "Component Galleries",
                        "Form components: sliders, checkboxes, dropdowns",
                        FormsGallery)
