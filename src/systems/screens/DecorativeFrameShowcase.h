#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <array>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct DecorativeFrameShowcase : ScreenSystem<UIContext<InputAction>> {
  bool show_content_bounds = false;

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
    theme.corner_radius = 0;
    theme.roundness = 0;
    context.set_theme(theme);
    context.scaling_mode = ScalingMode::Proportional;
    const float s = std::min(context.screen_height / 720.f, context.screen_width / 1280.f);
    const float left = (context.screen_width - 1144 * s) / 2;
    const float top = (context.screen_height - 720 * s) / 2;
    UIStylingDefaults::get().set_default_font("AtkinsonMock", pixels(18 * s));
    const auto box = [s](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, y * s).with_background(Theme::Usage::None).with_corner_radius(0);
    };
    auto canvas = div(context, mk(entity, 0), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background(theme.background).with_corner_radius(0).with_debug_name("bg"));
    auto root = div(context, mk(canvas.ent(), 0), box(0, 0, 1144, 720)
        .with_absolute_position(left, top).with_debug_name("frames_root"));
    const auto label = [&](afterhours::Entity &parent, int id, const std::string &value,
                           float x, float y, float w, float h, float size = 18.f,
                           bool strong = false, const std::string &name = "") {
      return div(context, mk(parent, id), box(x, y, w, h).with_label(value)
          .with_font(strong ? "AtkinsonMockBold" : "AtkinsonMock", pixels(size * s))
          .with_custom_text_color(strong ? theme.font : theme.font_muted)
          .with_alignment(TextAlignment::Left).with_ignore_pointer_events().with_debug_name(name));
    };
    div(context, mk(root.ent(), 0), box(0, 24, 1144, 80)
        .with_custom_background(theme.surface).with_corner_radius(12 * s));
    label(root.ent(), 1, "Decorative frames", 20, 28, 790, 40, 34, true);
    label(root.ent(), 2, "Identical content and dimensions. Original colors above; neutral colors below.",
          20, 72, 1104, 25, 19);
    if (button(context, mk(root.ent(), 3), box(874, 37, 250, 34)
        .with_label(show_content_bounds ? "Hide content bounds" : "Show content bounds")
        .with_font("AtkinsonMock", pixels(18 * s)).with_alignment(TextAlignment::Center)
        .with_background(Theme::Usage::Primary).with_auto_text_color(true).with_corner_radius(8 * s)
        .with_debug_name("frames_toggle_guides"))) show_content_bounds = !show_content_bounds;
    label(root.ent(), 4, "Outer 344 x 268px / content padding 32px / content 280 x 204px. Values at 720p.",
          8, 112, 1128, 26, 19, false, "frame_dimensions");

    struct Variant {
      DecorativeFrameStyle style;
      const char *name;
      const char *api;
      afterhours::Color border;
      afterhours::Color fill;
      float border_px;
      afterhours::Color ink;
      const char *use;
    };
    const std::array<Variant, 3> variants{{
        {DecorativeFrameStyle::KraftPaper, "Kraft paper", "KraftPaper", {167, 140, 103, 255},
         {251, 244, 226, 255}, 8.f, {62, 40, 18, 255}, "Journal pages and field notes"},
        {DecorativeFrameStyle::Simple, "Simple", "Simple", {117, 143, 134, 255},
         {245, 250, 247, 255}, 3.f, {28, 52, 78, 255}, "Reference cards and dashboards"},
        {DecorativeFrameStyle::Inset, "Inset", "Inset", {174, 177, 171, 255},
         {241, 242, 237, 255}, 5.f, {45, 45, 45, 255}, "Recessed inspector panels"}}};
    for (int i = 0; i < 3; ++i) {
      const auto &v = variants[i];
      const float x = 12 + i * 388.f;
      auto frame = decorative_frame(context, mk(root.ent(), 10 + i),
          box(x, 144, 344, 268).with_custom_background(v.fill)
              .with_border(v.border, pixels(v.border_px * s)).with_debug_name("frame_" + std::to_string(i)),
          v.style);
      auto card = vstack(context, mk(frame.ent(), 100), ComponentConfig{}
          .with_size({percent(1.f), percent(1.f)}).with_skip_grid_snap(true).with_padding(Padding{.top = pixels(32 * s),
              .left = pixels(32 * s), .bottom = pixels(32 * s), .right = pixels(32 * s)})
          .with_background(Theme::Usage::None).with_no_wrap().with_debug_name("card_" + std::to_string(i)));
      const auto content = [&](int id, const std::string &value, float height, float size, bool strong = false) {
        return div(context, mk(card.ent(), id), ComponentConfig{}
            .with_size({percent(1.f), pixels(height * s)}).with_skip_grid_snap(true).with_label(value)
            .with_font(strong ? "AtkinsonMockBold" : "AtkinsonMock", pixels(size * s))
            .with_custom_text_color(v.ink).with_alignment(TextAlignment::Left)
            .with_background(Theme::Usage::None).with_text_overflow(TextOverflow::Wrap));
      };
      content(0, "Field notes", 34, 26, true);
      content(1, "Botanical collection", 28, 14);
      div(context, mk(card.ent(), 2), ComponentConfig{}
          .with_size({percent(1.f), pixels(s)}).with_skip_grid_snap(true).with_custom_background({v.ink.r, v.ink.g, v.ink.b, 70})
          .with_margin(Margin{.bottom = pixels(12 * s)}).with_corner_radius(0));
      content(3, "Sample 07 / pressed fern", 28, 19);
      content(4, "Collected in the north meadow.\nKeep flat, dry and out of sunlight.", 69, 18);
      content(5, "Filed 06 September 2026", 24, 16);
      div(context, mk(frame.ent(), 101), box(32, 32, 280, 204).with_ignore_pointer_events()
          .with_debug_name("frame_content_bounds_" + std::to_string(i))
          .with_on_draw_fg([visible = show_content_bounds, s](RectangleType r) {
            if (!visible) return;
            afterhours::draw_rectangle_rounded_lines_ex(r, 0, 4, s, {183, 76, 54, 255});
          }));
      label(root.ent(), 20 + i, v.name, x, 422, 344, 31, 26, true);
      div(context, mk(root.ent(), 30 + i), box(x, 455, 344, 27)
          .with_label(std::string("DecorativeFrameStyle::") + v.api)
          .with_font("AtkinsonMock", pixels(20 * s)).with_custom_text_color(theme.font_muted)
          .with_alignment(TextAlignment::Left).with_ignore_pointer_events());
      label(root.ent(), 40 + i, v.use, x, 498, 344, 28, 19, true);
      const std::array<const char *, 3> details{
          "Layered paper with corner tabs",
          "A single, consistent border",
          "A quiet recessed edge"};
      label(root.ent(), 50 + i, details[i], x, 534, 344, 26, 18);
      label(root.ent(), 60 + i, fmt::format("Border {:.0f}px / padding 32px", v.border_px),
            x, 566, 344, 24, 17);
      auto neutral = decorative_frame(context, mk(root.ent(), 110 + i),
          box(x, 620, 344, 64).with_custom_background({235, 235, 232, 255})
              .with_border({110, 110, 108, 255}, pixels(v.border_px * s))
              .with_debug_name("neutral_frame_" + std::to_string(i)), v.style);
      div(context, mk(neutral.ent(), 100), box(24, 20, 296, 25).with_label("Same neutral colors")
          .with_font("AtkinsonMock", pixels(17 * s)).with_custom_text_color({50, 50, 48, 255})
          .with_alignment(TextAlignment::Center).with_ignore_pointer_events());
    }
    label(root.ent(), 5, "Neutral palette / the same frame styles without color", 8, 594, 1128, 20, 16);
  }
};

REGISTER_EXAMPLE_SCREEN(decorative_frame, "Component Galleries",
                        "Decorative frame border styles",
                        DecorativeFrameShowcase)
