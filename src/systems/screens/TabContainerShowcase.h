#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct TabContainerShowcase : ScreenSystem<UIContext<InputAction>> {
  size_t active_tab = 0;

  std::array<std::string_view, 3> tab_labels = {"Profile", "Account",
                                                 "Settings"};

  void render_profile_tab(UIContext<InputAction> &context, afterhours::Entity &root,
                          const Theme &theme) {
    // Section title - use larger font for clear hierarchy
    div(context, mk(root, 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(56)})
            .with_label("Profile Settings")
            .with_font(UIComponent::DEFAULT_FONT, h720(32.0f))  // font_size_lg for section headers
            .with_custom_text_color(theme.font)
            .with_margin(Margin{.bottom = pixels(16)})
            .with_debug_name("content_title"));

    // Content lines - use 18px minimum for body text
    div(context, mk(root, 2),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(44)})
            .with_label("Username: player_one")
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(theme.font)
            .with_margin(Margin{.bottom = pixels(8)})
            .with_debug_name("content_line1"));

    div(context, mk(root, 3),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(44)})
            .with_label("Email: player@example.com")
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(theme.font)
            .with_margin(Margin{.bottom = pixels(8)})
            .with_debug_name("content_line2"));

    div(context, mk(root, 4),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(44)})
            .with_label("Account created: January 2024")
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(theme.font)
            .with_margin(Margin{.bottom = pixels(8)})
            .with_debug_name("content_line3"));

    div(context, mk(root, 5),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(44)})
            .with_label("Subscription: Premium")
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(theme.font)
            .with_margin(Margin{.bottom = pixels(8)})
            .with_debug_name("content_line4"));

    div(context, mk(root, 6),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(44)})
            .with_label("Last login: Today at 10:30 AM")
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(theme.font)
            .with_debug_name("content_line5"));
  }

  void render_audio_tab(UIContext<InputAction> &context, afterhours::Entity &root,
                        const Theme &theme) {
    // Section title - use larger font for clear hierarchy
    div(context, mk(root, 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(56)})
            .with_label("Audio Settings")
            .with_font(UIComponent::DEFAULT_FONT, h720(32.0f))  // font_size_lg for section headers
            .with_custom_text_color(theme.font)
            .with_margin(Margin{.bottom = pixels(16)})
            .with_debug_name("content_title"));

    // Content lines - use 18px minimum for values
    div(context, mk(root, 2),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(44)})
            .with_label("Master Volume: 80%")
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(theme.font)
            .with_margin(Margin{.bottom = pixels(8)})
            .with_debug_name("content_line1"));

    div(context, mk(root, 3),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(44)})
            .with_label("Music Volume: 60%")
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(theme.font)
            .with_margin(Margin{.bottom = pixels(8)})
            .with_debug_name("content_line2"));

    div(context, mk(root, 4),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(44)})
            .with_label("Sound Effects: 75%")
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(theme.font)
            .with_margin(Margin{.bottom = pixels(16)})
            .with_debug_name("content_line3"));

    // Audio bars
    for (int i = 0; i < 5; ++i) {
      float bar_width = 120.0f + (static_cast<float>(i) * 90.0f);
      div(context, mk(root, 10 + i),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(bar_width), pixels(24)})
              .with_background(Theme::Usage::Accent)
              .with_roundness(0.15f)
              .with_margin(Margin{.bottom = pixels(10)})
              .with_debug_name(fmt::format("bar_{}", i)));
    }
  }

  void render_display_tab(UIContext<InputAction> &context, afterhours::Entity &root,
                          const Theme &theme) {
    // Section title - use larger font for clear hierarchy
    div(context, mk(root, 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(56)})
            .with_label("Display Settings")
            .with_font(UIComponent::DEFAULT_FONT, h720(32.0f))  // font_size_lg for section headers
            .with_custom_text_color(theme.font)
            .with_margin(Margin{.bottom = pixels(16)})
            .with_debug_name("content_title"));

    // Content lines - use 18px minimum for values
    div(context, mk(root, 2),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(44)})
            .with_label("Resolution: 1920x1080")
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(theme.font)
            .with_margin(Margin{.bottom = pixels(8)})
            .with_debug_name("content_line1"));

    div(context, mk(root, 3),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(44)})
            .with_label("Fullscreen: Enabled")
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(theme.font)
            .with_margin(Margin{.bottom = pixels(8)})
            .with_debug_name("content_line2"));

    div(context, mk(root, 4),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(44)})
            .with_label("VSync: On")
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(theme.font)
            .with_margin(Margin{.bottom = pixels(16)})
            .with_debug_name("content_line3"));

    // Color swatches row
    afterhours::Color swatches[] = {
        {255, 87, 87, 255}, {255, 189, 87, 255}, {255, 255, 87, 255},
        {87, 255, 87, 255}, {87, 189, 255, 255}, {189, 87, 255, 255},
    };

    auto swatch_row = div(context, mk(root, 10),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(70)})
            .with_flex_direction(FlexDirection::Row)
            .with_debug_name("swatch_row"));

    for (int i = 0; i < 6; ++i) {
      div(context, mk(swatch_row.ent(), i),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(60), pixels(60)})
              .with_custom_background(swatches[i])
              .with_rounded_corners(RoundedCorners().all_round())
              .with_roundness(0.2f)
              .with_margin(Margin{.right = pixels(16)})
              .with_debug_name(fmt::format("swatch_{}", i)));
    }
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    Theme theme;
    theme.background = afterhours::Color{230, 230, 230, 255};
    theme.surface = afterhours::Color{255, 255, 255, 255};
    theme.primary = afterhours::Color{100, 100, 100, 255};
    theme.accent = afterhours::Color{0, 120, 215, 255};
    theme.font = afterhours::Color{33, 33, 33, 255};
    theme.font_muted = afterhours::Color{55, 55, 55, 255};
    context.theme = theme;

    // Root container - centered on screen, expanded to fill more screen
    auto root = div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(0.85f), screen_pct(0.88f)})
            .with_self_align(SelfAlign::Center)
            .with_background(Theme::Usage::Background)
            .with_roundness(0.04f)
            .with_debug_name("root"));

    // Main container with padding to keep tabs inside bounds
    auto main_container = div(context, mk(root.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), percent(1.0f)})
            .with_padding(Spacing::lg)
            .with_flex_direction(FlexDirection::Column)
            .with_debug_name("main_container"));

    // Use tab_container component - check result for tab changes
    // Note: Small horizontal padding prevents floating-point rounding from
    // causing false layout overflow warnings when tab widths (percent(1/N))
    // sum to exactly the container width.
    if (auto result = tab_container(context, mk(main_container.ent(), 0), tab_labels, active_tab,
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), pixels(52)})
                .with_font(UIComponent::DEFAULT_FONT, h720(22.0f))
                .with_no_wrap()
                .with_padding(Padding{
                    .left = pixels(1),
                    .right = pixels(1)})
                .with_debug_name("tabs")); result) {
      // Tab changed - could log, play sound, etc.
    }

    // Content panel with proper padding
    auto content_panel = div(context, mk(main_container.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), percent(1.0f)})
            .with_background(Theme::Usage::Surface)
            .with_padding(Padding{
                .top = pixels(16),
                .left = pixels(24),
                .bottom = pixels(16),
                .right = pixels(24)})
            .with_flex_direction(FlexDirection::Column)
            .with_margin(Margin{.top = pixels(4)})
            .with_debug_name("content_panel"));

    // Render content based on active tab
    using RenderFn = void (TabContainerShowcase::*)(UIContext<InputAction>&, afterhours::Entity&, const Theme&);
    RenderFn render_fns[] = {
        &TabContainerShowcase::render_profile_tab,
        &TabContainerShowcase::render_audio_tab,
        &TabContainerShowcase::render_display_tab,
    };
    (this->*render_fns[active_tab])(context, content_panel.ent(), theme);
  }
};

REGISTER_EXAMPLE_SCREEN(tab_container, "Component Galleries",
                        "Tab container component demo", TabContainerShowcase)
