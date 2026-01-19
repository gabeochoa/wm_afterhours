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

  std::array<std::string_view, 3> tab_labels = {"Tab one", "Tab two",
                                                 "Tab three"};

  void render_profile_tab(UIContext<InputAction> &context, afterhours::Entity &root,
                          const Theme &theme) {
    div(context, mk(root, 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.90f), pixels(50)})
            .with_label("Profile Settings")
            .with_font(UIComponent::DEFAULT_FONT, 24.0f)
            .with_custom_text_color(theme.font)
            .with_margin(Margin{.top = pixels(20), .left = pixels(20)})
            .with_debug_name("content_title"));

    div(context, mk(root, 2),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.90f), pixels(36)})
            .with_label("Username: player_one")
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_custom_text_color(theme.font)
            .with_margin(Margin{.top = pixels(12), .left = pixels(20)})
            .with_debug_name("content_line1"));

    div(context, mk(root, 3),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.90f), pixels(36)})
            .with_label("Email: player@example.com")
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_custom_text_color(theme.font)
            .with_margin(Margin{.top = pixels(8), .left = pixels(20)})
            .with_debug_name("content_line2"));
  }

  void render_audio_tab(UIContext<InputAction> &context, afterhours::Entity &root,
                        const Theme &theme) {
    div(context, mk(root, 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.90f), pixels(50)})
            .with_label("Audio Settings")
            .with_font(UIComponent::DEFAULT_FONT, 24.0f)
            .with_custom_text_color(theme.font)
            .with_margin(Margin{.top = pixels(20), .left = pixels(20)})
            .with_debug_name("content_title"));

    div(context, mk(root, 2),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.90f), pixels(36)})
            .with_label("Volume: 80%")
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_custom_text_color(theme.font)
            .with_margin(Margin{.top = pixels(12), .left = pixels(20)})
            .with_debug_name("content_line1"));

    div(context, mk(root, 3),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.90f), pixels(36)})
            .with_label("Music: 60%")
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_custom_text_color(theme.font)
            .with_margin(Margin{.top = pixels(8), .left = pixels(20)})
            .with_debug_name("content_line2"));

    // Audio bars
    for (int i = 0; i < 5; ++i) {
      float bar_width = 100.0f + (static_cast<float>(i) * 80.0f);
      div(context, mk(root, 10 + i),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(bar_width), pixels(20)})
              .with_custom_background(theme.accent)
              .with_roundness(0.15f)
              .with_margin(Margin{.top = pixels(8), .left = pixels(20)})
              .with_debug_name(fmt::format("bar_{}", i)));
    }
  }

  void render_display_tab(UIContext<InputAction> &context, afterhours::Entity &root,
                          const Theme &theme) {
    div(context, mk(root, 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.90f), pixels(50)})
            .with_label("Display Settings")
            .with_font(UIComponent::DEFAULT_FONT, 24.0f)
            .with_custom_text_color(theme.font)
            .with_margin(Margin{.top = pixels(20), .left = pixels(20)})
            .with_debug_name("content_title"));

    div(context, mk(root, 2),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.90f), pixels(36)})
            .with_label("Resolution: 1920x1080")
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_custom_text_color(theme.font)
            .with_margin(Margin{.top = pixels(12), .left = pixels(20)})
            .with_debug_name("content_line1"));

    div(context, mk(root, 3),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.90f), pixels(36)})
            .with_label("Fullscreen: Enabled")
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_custom_text_color(theme.font)
            .with_margin(Margin{.top = pixels(8), .left = pixels(20)})
            .with_debug_name("content_line2"));

    // Color swatches row
    afterhours::Color swatches[] = {
        {255, 87, 87, 255}, {255, 189, 87, 255}, {255, 255, 87, 255},
        {87, 255, 87, 255}, {87, 189, 255, 255}, {189, 87, 255, 255},
    };

    auto swatch_row = div(context, mk(root, 10),
        ComponentConfig{}
            .with_size(ComponentSize{percent(0.80f), pixels(60)})
            .with_flex_direction(FlexDirection::Row)
            .with_margin(Margin{.top = pixels(16), .left = pixels(20)})
            .with_debug_name("swatch_row"));

    for (int i = 0; i < 6; ++i) {
      div(context, mk(swatch_row.ent(), i),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(50), pixels(50)})
              .with_custom_background(swatches[i])
              .with_rounded_corners(RoundedCorners().all_round())
              .with_roundness(0.2f)
              .with_margin(Margin{.right = pixels(12)})
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
    theme.font_muted = afterhours::Color{180, 180, 180, 255};
    context.theme = theme;

    // Root container - centered on screen
    auto root = div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(0.70f), screen_pct(0.75f)})
            .with_self_align(SelfAlign::Center)
            .with_custom_background(theme.background)
            .with_roundness(0.04f)
            .with_flex_direction(FlexDirection::Column)
            .with_debug_name("root"));

    // Use tab_container component - check result for tab changes
    if (auto result = tab_container(context, mk(root.ent(), 0), tab_labels, active_tab,
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), pixels(50)})
                .with_font(UIComponent::DEFAULT_FONT, 20.0f)
                .with_debug_name("tabs")); result) {
      // Tab changed - could log, play sound, etc.
    }

    // Render content based on active tab
    using RenderFn = void (TabContainerShowcase::*)(UIContext<InputAction>&, afterhours::Entity&, const Theme&);
    RenderFn render_fns[] = {
        &TabContainerShowcase::render_profile_tab,
        &TabContainerShowcase::render_audio_tab,
        &TabContainerShowcase::render_display_tab,
    };
    (this->*render_fns[active_tab])(context, root.ent(), theme);
  }
};

REGISTER_EXAMPLE_SCREEN(tab_container, "Component Galleries",
                        "Tab container component demo", TabContainerShowcase)
