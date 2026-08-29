#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/clipboard.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

namespace forms_presets {

inline ComponentConfig CheckboxConfig(const std::string &label) {
  return ComponentConfig{}
      .with_label(label)
      .with_size(ComponentSize{percent(0.92f), pixels(34)})
      .with_background(Theme::Usage::Primary)
      .with_font_size(14.0f)
      // The library default is "V", which this font draws as a literal
      // capital V floating in the row.
      .with_checkbox_indicators("[x]", "[ ]")
      .with_margin(Spacing::xs);
}

inline ComponentConfig SectionHeaderConfig(
    const std::string &label,
    Theme::Usage bg = Theme::Usage::Primary) {
  return ComponentConfig{}
      .with_label(label)
      .with_size(ComponentSize{percent(0.95f), pixels(36)})
      .with_background(bg)
      .with_auto_text_color(true)
      .with_padding(Spacing::xs)
      .with_font_size(20.0f)
      .with_skip_tabbing(true);
}

inline ComponentConfig SliderConfig(const std::string &label,
                                    Theme::Usage bg = Theme::Usage::Primary) {
  return ComponentConfig{}
      .with_label(label)
      .with_size(ComponentSize{percent(0.95f), pixels(44)})
      .with_background(bg)
      .with_font_size(14.0f)
      .with_margin(Spacing::xs);
}

inline ComponentConfig ProgressBarConfig(const std::string &label) {
  return ComponentConfig{}
      .with_label(label)
      .with_size(ComponentSize{percent(0.95f), pixels(28)})
      .with_font_size(pixels(16.0f))
      .with_margin(Spacing::xs);
}

} // namespace forms_presets

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
  bool show_hud = true;
  bool auto_save = true;
  bool subtitles = false;
  bool motion_blur = true;
  bool anti_aliasing = true;
  bool ambient_occlusion = false;
  bool bloom_effect = true;

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
    // Apply neon dark theme with default font for this screen
    auto theme = afterhours::ui::theme_presets::neon_dark();
    context.theme = theme;
    context.scaling_mode = ScalingMode::Adaptive;
    UIStylingDefaults::get().set_default_font(UIComponent::DEFAULT_FONT,
                                              pixels(16.0f));

    using namespace forms_presets;

    // Main container background - centered on screen
    auto root =
        div(context, mk(entity, 0),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(0.78f), screen_pct(0.86f)})
                .with_self_align(SelfAlign::Center)
                .with_background(Theme::Usage::Background)
                .with_roundness(0.08f)
                .with_debug_name("forms_bg"));

    // Content container with padding
    auto main_container =
        vstack(context, mk(root.ent(), 0),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.0f), percent(1.0f)})
                   // Any vertical padding here pushes the clipboard row out.
                   .with_padding(Padding{.left = DefaultSpacing::small(),
                                         .right = DefaultSpacing::small()})
                   .with_no_wrap()
                   .with_debug_name("forms_main"));

    // Title - use font_size_xl for screen title (largest in hierarchy)
    div(context, mk(main_container.ent(), 0),
        ComponentConfig{}
            .with_label("Form Components")
            .with_size(ComponentSize{percent(1.0f), pixels(50)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_padding(Spacing::sm)
            .with_font_size(theme.font_size_xl()) // 42px - screen title
            .with_margin(Margin{.top = pixels(0),
                                .bottom = DefaultSpacing::small(),
                                .left = pixels(0),
                                .right = pixels(0)}));

    // Content area - two columns - adjusted height for all content
    auto content =
        hstack(context, mk(main_container.ent(), 1),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.0f), percent(0.66f)})
                   .with_background(Theme::Usage::Surface)
                   .with_padding(Spacing::sm)
                   .with_justify_content(JustifyContent::Center)
                   .with_debug_name("content"));

    // Left column - Sliders
    auto left_col =
        vstack(context, mk(content.ent(), 0),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(0.45f), percent(1.0f)})
                   .with_custom_background(
                       afterhours::colors::darken(theme.surface, 0.9f))
                   // Same: level_progress needed 340 in a 323.6 content box.
                   .with_padding(Spacing::xs)
                   .with_debug_name("left_column"));

    // Sliders section header - font_size_md for section headers
    div(context, mk(left_col.ent(), 0), SectionHeaderConfig("Sliders"));

    // Volume slider - use font_size_sm for control labels, increased handle
    // visibility
    slider(context, mk(left_col.ent(), 1), volume_slider,
           SliderConfig("Volume").with_debug_name("volume_slider"),
           SliderHandleValueLabelPosition::WithLabel);

    // Brightness slider - increased handle visibility
    slider(context, mk(left_col.ent(), 2), brightness_slider,
           SliderConfig("Brightness", Theme::Usage::Accent)
               .with_debug_name("brightness_slider"),
           SliderHandleValueLabelPosition::WithLabel);

    // Difficulty slider - increased handle visibility
    slider(context, mk(left_col.ent(), 3), difficulty_slider,
           SliderConfig("Difficulty", Theme::Usage::Secondary)
               .with_debug_name("difficulty_slider"),
           SliderHandleValueLabelPosition::WithLabel);

    // Progress Bars section header - font_size_md for section headers
    div(context, mk(left_col.ent(), 4),
        SectionHeaderConfig("Progress Bars", Theme::Usage::Accent));

    // Progress bar showing volume value (dynamic)
    progress_bar(context, mk(left_col.ent(), 5), volume_slider,
                 ProgressBarConfig("Audio Level")
                     .with_debug_name("volume_progress"),
                 ProgressBarLabelStyle::Percentage);

    // Progress bar with custom range
    progress_bar(context, mk(left_col.ent(), 6), 75.f,
                 ProgressBarConfig("Level Progress")
                     .with_debug_name("level_progress"),
                 ProgressBarLabelStyle::Fraction, 0.f, 100.f);

    // Right column - Checkboxes - scrollable when content overflows
    auto right_col =
        vstack(context, mk(content.ent(), 1),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(0.42f), percent(1.0f)})
                   .with_custom_background(
                       afterhours::colors::darken(theme.surface, 0.9f))
                   .with_padding(Spacing::sm)
                   .with_no_wrap()
                   .with_overflow(Overflow::Scroll, Axis::Y));

    // Checkboxes section header - font_size_md for section headers
    div(context, mk(right_col.ent(), 0), SectionHeaderConfig("Checkboxes"));

    // Audio checkboxes - font_size_sm for checkbox labels, standardized
    // checkmark color via Primary
    checkbox(context, mk(right_col.ent(), 1), enable_music,
             CheckboxConfig("Music"));
    checkbox(context, mk(right_col.ent(), 2), enable_sfx,
             CheckboxConfig("SFX"));
    // Display checkboxes - standardized checkmark color via Primary
    checkbox(context, mk(right_col.ent(), 3), fullscreen,
             CheckboxConfig("Fullscreen"));
    checkbox(context, mk(right_col.ent(), 4), vsync,
             CheckboxConfig("V-Sync"));
    checkbox(context, mk(right_col.ent(), 5), show_fps,
             CheckboxConfig("Show FPS"));
    // Additional graphics/gameplay checkboxes (make content overflow for
    // scroll)
    checkbox(context, mk(right_col.ent(), 6), show_hud,
             CheckboxConfig("Show HUD"));
    checkbox(context, mk(right_col.ent(), 7), auto_save,
             CheckboxConfig("Auto Save"));
    checkbox(context, mk(right_col.ent(), 8), subtitles,
             CheckboxConfig("Subtitles"));
    checkbox(context, mk(right_col.ent(), 9), motion_blur,
             CheckboxConfig("Motion Blur"));
    checkbox(context, mk(right_col.ent(), 10), anti_aliasing,
             CheckboxConfig("Anti-Aliasing"));
    checkbox(context, mk(right_col.ent(), 11), ambient_occlusion,
             CheckboxConfig("Ambient Occlusion"));
    checkbox(context, mk(right_col.ent(), 12), bloom_effect,
             CheckboxConfig("Bloom Effect"));

    // Disabled checkbox example - standardized checkmark color via Primary
    // Enhanced disabled styling: reduced opacity for stronger visual
    // differentiation
    bool disabled_value = true;
    checkbox(context, mk(right_col.ent(), 13), disabled_value,
             CheckboxConfig("Disabled")
                 .with_disabled(true));

    // Language dropdown - font_size_sm for dropdown values
    dropdown(context, mk(right_col.ent(), 14), languages, language_index,
             ComponentConfig{}
                 .with_size(ComponentSize{percent(0.92f), pixels(34)})
                 .with_background(Theme::Usage::Secondary)
                 .with_font_size(theme.font_size_sm()) // 16px - values
                 .with_margin(Spacing::xs)
                 .with_debug_name("language_dropdown"));

    // Status display - font_size_sm for body text
    std::string status =
        "Volume: " + std::to_string(static_cast<int>(volume_slider * 100)) +
        "% | Resolution: " + resolutions[resolution_index] +
        " | Quality: " + quality_options[quality_index];

    div(context, mk(main_container.ent(), 2),
        ComponentConfig{}
            .with_label(status)
            .with_size(ComponentSize{percent(1.0f), pixels(40)})
            .with_custom_background(
                afterhours::colors::lighten(theme.background, 0.08f))
            .with_auto_text_color(true)
            .with_padding(Spacing::sm)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.1f)
            .with_font_size(theme.font_size_sm()) // 16px - body text
            .with_margin(Margin{.top = DefaultSpacing::tiny(),
                                .bottom = pixels(0),
                                .left = pixels(0),
                                .right = pixels(0)}));

    // Clipboard demo section with help text
    div(context, mk(main_container.ent(), 3),
        ComponentConfig{}
            .with_label("Clipboard Demo: Copy saves current settings, Paste "
                        "retrieves saved text")
            .with_size(ComponentSize{percent(1.0f), pixels(44)})
            .with_custom_background(
                afterhours::colors::darken(theme.surface, 0.9f))
            .with_custom_text_color(theme.font_muted)
            .with_padding(Spacing::xs)
            .with_font_size(theme.font_size_sm() - 2.0f) // 12px - help text
            .with_margin(Margin{.top = DefaultSpacing::tiny(),
                                .bottom = pixels(0),
                                .left = pixels(0),
                                .right = pixels(0)}));

    auto clipboard_row =
        hstack(context, mk(main_container.ent(), 4),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.0f), pixels(44)})
                   .with_justify_content(JustifyContent::SpaceAround)
                   // No top margin: the row holds 44px buttons and was the
                   // last child, so the margin was what pushed it out.
                   .with_debug_name("clipboard_row"));

    // Copy button - copies current status to clipboard
    if (button(context, mk(clipboard_row.ent(), 0),
               ComponentConfig{}
                   .with_label("Copy Status")
                   .with_size(ComponentSize{pixels(150), pixels(44)})
                   .with_background(Theme::Usage::Primary)
                   .with_font_size(theme.font_size_sm()) // 16px - button text
                   .with_debug_name("copy_btn"))) {
      afterhours::clipboard::set_text(status);
      clipboard_display = "Copied!";
    }

    // Paste button - reads from clipboard
    if (button(context, mk(clipboard_row.ent(), 1),
               ComponentConfig{}
                   .with_label("Paste")
                   .with_size(ComponentSize{pixels(150), pixels(44)})
                   .with_background(Theme::Usage::Accent)
                   .with_font_size(theme.font_size_sm()) // 16px - button text
                   .with_debug_name("paste_btn"))) {
      if (afterhours::clipboard::has_text()) {
        clipboard_display = afterhours::clipboard::get_text();
      } else {
        clipboard_display = "(clipboard empty)";
      }
    }

    // Display clipboard contents (truncated if too long) - smaller font for
    // secondary info
    std::string display_text = clipboard_display;
    if (display_text.length() > 60) {
      display_text = display_text.substr(0, 57) + "...";
    }

    div(context, mk(clipboard_row.ent(), 2),
        ComponentConfig{}
            .with_label(display_text)
            .with_size(ComponentSize{expand(), pixels(44)})
            .with_background(Theme::Usage::Surface)
            .with_padding(Spacing::sm)
            .with_font_size(theme.font_size_sm() -
                            2.0f)); // 14px - secondary text
  }
};

REGISTER_EXAMPLE_SCREEN(forms, "Component Galleries",
                        "Form components: sliders, checkboxes, dropdowns",
                        FormsGallery)
