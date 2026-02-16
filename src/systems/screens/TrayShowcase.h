#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct TrayShowcase : ScreenSystem<UIContext<InputAction>> {
  std::vector<int> h_clicks = {0, 0, 0};
  std::vector<int> v_clicks = {0, 0, 0};

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = afterhours::ui::theme_presets::neon_dark();
    theme.focus = afterhours::Color{255, 255, 100, 255};
    theme.focus_ring_thickness = 4.0f;
    theme.focus_ring_offset = 5.0f;
    context.theme = theme;
    context.scaling_mode = ScalingMode::Adaptive;

    int screen_width = Settings::get().get_screen_width();
    int screen_height = Settings::get().get_screen_height();

    // Full-screen background
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(
                ComponentSize{pixels(screen_width), pixels(screen_height)})
            .with_background(Theme::Usage::Background)
            .with_debug_name("tray_bg"));

    // Card container
    auto card = vstack(
        context, mk(entity, 1),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(500), pixels(500)})
            .with_absolute_position(
                (screen_width - 500.0f) / 2.0f,
                (screen_height - 500.0f) / 2.0f)
            .with_background(Theme::Usage::Surface)
            .with_roundness(0.04f)
            .with_padding(Spacing::md)
            .with_debug_name("card"));

    // Title
    div(context, mk(card.ent(), 0),
        ComponentConfig{}
            .with_label("Tray Navigation")
            .with_size(ComponentSize{percent(1.0f), pixels(40)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, pixels(22.0f))
            .with_alignment(TextAlignment::Center)
            .with_skip_tabbing(true)
            .with_debug_name("title"));

    // Section label - horizontal
    div(context, mk(card.ent(), 1),
        ComponentConfig{}
            .with_label("Horizontal Tray (Left/Right)")
            .with_size(ComponentSize{percent(1.0f), pixels(30)})
            .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
            .with_skip_tabbing(true)
            .with_debug_name("h_label"));

    // Horizontal tray
    auto h_tray = tray(
        context, mk(card.ent(), 2),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(60)})
            .with_flex_direction(FlexDirection::Row)
            .with_flex_wrap(FlexWrap::NoWrap)
            .with_justify_content(JustifyContent::SpaceAround)
            .with_align_items(AlignItems::Center)
            .with_gap(pixels(10.0f))
            .with_debug_name("h_tray"));

    std::string h_labels[] = {"H-Alpha", "H-Beta", "H-Gamma"};
    afterhours::Color h_colors[] = {
        afterhours::Color{165, 55, 140, 255},
        afterhours::Color{55, 195, 195, 255},
        afterhours::Color{145, 215, 65, 255},
    };

    for (int i = 0; i < 3; i++) {
      std::string label = h_labels[i];
      if (h_clicks[i] > 0) {
        label += " (" + std::to_string(h_clicks[i]) + ")";
      }
      auto btn = button(
          context, mk(h_tray.ent(), i),
          ComponentConfig{}
              .with_label(label)
              .with_size(ComponentSize{percent(0.30f), pixels(50)})
              .with_custom_background(h_colors[i])
              .with_auto_text_color(true)
              .with_font(UIComponent::DEFAULT_FONT, pixels(18.0f))
              .with_alignment(TextAlignment::Center)
              .with_debug_name("h_btn_" + std::to_string(i)));
      if (btn) {
        h_clicks[i]++;
      }
    }

    // Spacer
    div(context, mk(card.ent(), 3),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(20)})
            .with_skip_tabbing(true)
            .with_debug_name("spacer"));

    // Section label - vertical
    div(context, mk(card.ent(), 4),
        ComponentConfig{}
            .with_label("Vertical Tray (Up/Down)")
            .with_size(ComponentSize{percent(1.0f), pixels(30)})
            .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
            .with_skip_tabbing(true)
            .with_debug_name("v_label"));

    // Vertical tray
    auto v_tray = tray(
        context, mk(card.ent(), 5),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(170)})
            .with_flex_direction(FlexDirection::Column)
            .with_flex_wrap(FlexWrap::NoWrap)
            .with_gap(pixels(10.0f))
            .with_debug_name("v_tray"));

    std::string v_labels[] = {"V-Alpha", "V-Beta", "V-Gamma"};
    afterhours::Color v_colors[] = {
        afterhours::Color{235, 175, 55, 255},
        afterhours::Color{55, 130, 195, 255},
        afterhours::Color{195, 85, 85, 255},
    };

    for (int i = 0; i < 3; i++) {
      std::string label = v_labels[i];
      if (v_clicks[i] > 0) {
        label += " (" + std::to_string(v_clicks[i]) + ")";
      }
      auto btn = button(
          context, mk(v_tray.ent(), i),
          ComponentConfig{}
              .with_label(label)
              .with_size(ComponentSize{percent(1.0f), pixels(50)})
              .with_custom_background(v_colors[i])
              .with_auto_text_color(true)
              .with_font(UIComponent::DEFAULT_FONT, pixels(18.0f))
              .with_alignment(TextAlignment::Center)
              .with_debug_name("v_btn_" + std::to_string(i)));
      if (btn) {
        v_clicks[i]++;
      }
    }
  }
};

REGISTER_EXAMPLE_SCREEN(tray, "System Demos",
                        "Tray navigation — single tab stop with arrow key "
                        "selection among children",
                        TrayShowcase)
