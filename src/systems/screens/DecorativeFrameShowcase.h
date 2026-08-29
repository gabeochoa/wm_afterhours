#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct DecorativeFrameShowcase : ScreenSystem<UIContext<InputAction>> {

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    // Warm, craft-inspired theme
    Theme theme;
    theme.background = afterhours::Color{245, 240, 230, 255}; // Cream
    theme.surface = afterhours::Color{255, 250, 240, 255};    // Off-white
    theme.primary = afterhours::Color{139, 90, 43, 255};      // Brown
    theme.secondary = afterhours::Color{180, 140, 90, 255};   // Tan
    theme.accent = afterhours::Color{200, 80, 60, 255};       // Rust red
    theme.font = afterhours::Color{60, 40, 20, 255};          // Dark brown
    theme.font_muted = afterhours::Color{120, 100, 80, 255};
    context.theme = theme;
    context.scaling_mode = ScalingMode::Adaptive;

    // Absolute positions throughout: the frame's own layers are absolutely
    // placed, and a flow child of one lands outside it rather than inside.
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(1280), pixels(720)})
            .with_absolute_position(0.f, 0.f)
            .with_custom_background(theme.background)
            .with_debug_name("bg"));

    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_label("Decorative Frames")
            .with_size(ComponentSize{pixels(1280), pixels(38)})
            .with_absolute_position(0.f, 40.f)
            .with_font(UIComponent::DEFAULT_FONT, pixels(28.0f))
            .with_custom_text_color(theme.font)
            .with_alignment(TextAlignment::Center));

    div(context, mk(entity, 2),
        ComponentConfig{}
            .with_label("The same card in three border treatments")
            .with_size(ComponentSize{pixels(1280), pixels(26)})
            .with_absolute_position(0.f, 80.f)
            .with_font(UIComponent::DEFAULT_FONT, pixels(17.0f))
            .with_custom_text_color(theme.font_muted)
            .with_alignment(TextAlignment::Center));

    struct Variant {
      DecorativeFrameStyle style;
      const char *name;
      const char *desc;
      afterhours::Color border;
      afterhours::Color fill;
      float border_px;
      const char *heading;
      const char *line_a;
      const char *line_b;
      afterhours::Color ink;
    };

    // Each frame wraps a short card, because a frame around nothing shows only
    // its own edge and reads as a colour swatch.
    const Variant variants[3] = {
        {DecorativeFrameStyle::KraftPaper, "KraftPaper", "Layered + corners",
         afterhours::Color{101, 67, 33, 255},
         afterhours::Color{222, 184, 135, 255}, 8.0f, "Field Notes",
         "Pressed between two boards", "and tied with garden twine.",
         afterhours::Color{62, 40, 18, 255}},
        {DecorativeFrameStyle::Simple, "Simple", "Border + background",
         afterhours::Color{70, 130, 180, 255},
         afterhours::Color{240, 248, 255, 255}, 3.0f, "Tide Table",
         "High water at 06:14 and 18:41,", "two metres over the chart datum.",
         afterhours::Color{28, 52, 78, 255}},
        {DecorativeFrameStyle::Inset, "Inset", "Sunken shadow",
         afterhours::Color{128, 128, 128, 255},
         afterhours::Color{220, 220, 220, 255}, 5.0f, "Plate 14",
         "Cut from the 1908 survey and", "remounted on grey board.",
         afterhours::Color{45, 45, 45, 255}},
    };

    constexpr float FRAME_W = 344.f;
    constexpr float FRAME_H = 268.f;
    constexpr float FRAME_Y = 150.f;
    constexpr float GAP = 44.f;
    const float total_w = 3.f * FRAME_W + 2.f * GAP;
    const float start_x = (1280.f - total_w) * 0.5f;

    for (int i = 0; i < 3; i++) {
      const Variant &v = variants[i];
      const float fx = start_x + static_cast<float>(i) * (FRAME_W + GAP);

      decorative_frame(context, mk(entity, 10 + i),
                       ComponentConfig{}
                           .with_size(ComponentSize{pixels(FRAME_W),
                                                    pixels(FRAME_H)})
                           .with_absolute_position(fx, FRAME_Y)
                           .with_custom_background(v.fill)
                           .with_border(v.border, pixels(v.border_px))
                           .with_debug_name(fmt::format("frame_{}", i)),
                       v.style);

      // Inset past the frame band so the card never sits on the border art.
      const float pad = v.border_px * 3.f + 10.f;
      const float text_x = fx + pad;
      const float text_w = FRAME_W - 2.f * pad;

      div(context, mk(entity, 20 + i),
          ComponentConfig{}
              .with_label(v.heading)
              .with_size(ComponentSize{pixels(text_w), pixels(32)})
              .with_absolute_position(text_x, FRAME_Y + 78.f)
              .with_font(UIComponent::DEFAULT_FONT, pixels(21.0f))
              .with_custom_text_color(v.ink)
              .with_alignment(TextAlignment::Center));

      div(context, mk(entity, 30 + i),
          ComponentConfig{}
              .with_label(v.line_a)
              .with_size(ComponentSize{pixels(text_w), pixels(24)})
              .with_absolute_position(text_x, FRAME_Y + 122.f)
              .with_font(UIComponent::DEFAULT_FONT, pixels(14.0f))
              .with_custom_text_color(v.ink)
              .with_alignment(TextAlignment::Center));

      div(context, mk(entity, 40 + i),
          ComponentConfig{}
              .with_label(v.line_b)
              .with_size(ComponentSize{pixels(text_w), pixels(24)})
              .with_absolute_position(text_x, FRAME_Y + 148.f)
              .with_font(UIComponent::DEFAULT_FONT, pixels(14.0f))
              .with_custom_text_color(v.ink)
              .with_alignment(TextAlignment::Center));

      div(context, mk(entity, 50 + i),
          ComponentConfig{}
              .with_label(v.name)
              .with_size(ComponentSize{pixels(FRAME_W), pixels(28)})
              .with_absolute_position(fx, FRAME_Y + FRAME_H + 18.f)
              .with_font(UIComponent::DEFAULT_FONT, pixels(19.0f))
              .with_custom_text_color(theme.font)
              .with_alignment(TextAlignment::Center));

      div(context, mk(entity, 60 + i),
          ComponentConfig{}
              .with_label(v.desc)
              .with_size(ComponentSize{pixels(FRAME_W), pixels(22)})
              .with_absolute_position(fx, FRAME_Y + FRAME_H + 46.f)
              .with_font(UIComponent::DEFAULT_FONT, pixels(14.0f))
              .with_custom_text_color(theme.font_muted)
              .with_alignment(TextAlignment::Center));
    }

    div(context, mk(entity, 70),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(total_w), pixels(64)})
            .with_absolute_position(start_x, 542.f)
            .with_custom_background(theme.surface)
            .with_border(theme.secondary, pixels(2.0f))
            .with_roundness(0.25f)
            .with_debug_name("tips"));

    div(context, mk(entity, 71),
        ComponentConfig{}
            .with_label("Pick by weight: KraftPaper for warmth, Simple to stay "
                        "out of the way, Inset to sit a panel back")
            .with_size(ComponentSize{pixels(total_w), pixels(24)})
            .with_absolute_position(start_x, 562.f)
            .with_font(UIComponent::DEFAULT_FONT, pixels(15.0f))
            .with_custom_text_color(theme.font_muted)
            .with_alignment(TextAlignment::Center));
  }
};

REGISTER_EXAMPLE_SCREEN(decorative_frame, "Component Galleries",
                        "Decorative frame border styles",
                        DecorativeFrameShowcase)
