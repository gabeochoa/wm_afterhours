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
        {DecorativeFrameStyle::KraftPaper, "Kraft paper", "KraftPaper", {101, 67, 33, 255},
         {222, 184, 135, 255}, 8.f, {62, 40, 18, 255}, "Journal pages and field notes"},
        {DecorativeFrameStyle::Simple, "Simple", "Simple", {70, 130, 180, 255},
         {240, 248, 255, 255}, 3.f, {28, 52, 78, 255}, "Reference cards and dashboards"},
        {DecorativeFrameStyle::Inset, "Inset", "Inset", {128, 128, 128, 255},
         {220, 220, 220, 255}, 5.f, {45, 45, 45, 255}, "Recessed inspector panels"}}};
    const float edge_px = context.screen_height / 720.f * 3.f;
    const float tab_px = context.screen_height / 720.f * 8.f;
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
      content(1, "BOTANICAL COLLECTION", 28, 14);
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
      div(context, mk(root.ent(), 40 + i), box(x, 493, 344, 111)
          .with_custom_background(theme.surface).with_corner_radius(8 * s));
      div(context, mk(root.ent(), 50 + i), box(x + 8, 501, 94, 94).with_ignore_pointer_events()
          .with_on_draw_fg([v, i, s](RectangleType r) {
            const float unit = 3 * s;
            afterhours::draw_rectangle(r, v.border);
            if (i == 0) {
              const float step = 2.4f * unit;
              afterhours::draw_rectangle({r.x + step, r.y + step, r.width - step, r.height - step},
                                         afterhours::colors::lighten(v.border, .1f));
              afterhours::draw_rectangle({r.x + 8 * unit, r.y + 8 * unit,
                  r.width - 8 * unit, r.height - 8 * unit}, v.fill);
              afterhours::draw_rectangle({r.x + 8 * unit, r.y + 8 * unit, 8 * unit, 8 * unit},
                                         afterhours::colors::darken(v.border, .85f));
              return;
            }
            if (i == 1) {
              afterhours::draw_rectangle(r, v.fill);
              raylib::DrawLineEx({r.x + 12 * s, r.y + r.height}, {r.x + 12 * s, r.y + 12 * s}, unit, v.border);
              raylib::DrawLineEx({r.x + 12 * s, r.y + 12 * s}, {r.x + r.width, r.y + 12 * s}, unit, v.border);
              return;
            }
            const auto shadow = afterhours::colors::opacity_pct(afterhours::colors::darken(v.border, .8f), .6f);
            const auto highlight = afterhours::colors::lighten(v.border, .2f);
            afterhours::draw_rectangle({r.x, r.y, r.width, 3 * unit}, shadow);
            afterhours::draw_rectangle({r.x, r.y, 3 * unit, r.height}, shadow);
            afterhours::draw_rectangle({r.x, r.y + r.height - 3 * unit, r.width, 3 * unit}, highlight);
            afterhours::draw_rectangle({r.x + r.width - 3 * unit, r.y, 3 * unit, r.height}, highlight);
            afterhours::draw_rectangle({r.x + 5 * unit, r.y + 5 * unit, r.width - 10 * unit, r.height - 10 * unit}, v.fill);
            const raylib::Vector2 start{r.x + 24 * s, r.y + 24 * s};
            const raylib::Vector2 end{r.x + 51 * s, r.y + 51 * s};
            raylib::DrawLineEx(start, end, 2 * s, {80, 80, 80, 255});
            raylib::DrawLineEx(end, {end.x - 10 * s, end.y}, 2 * s, {80, 80, 80, 255});
            raylib::DrawLineEx(end, {end.x, end.y - 10 * s}, 2 * s, {80, 80, 80, 255});
          }));
      if (i == 0) {
        label(root.ent(), 60 + i, "3x layer diagram", x + 108, 501, 228, 24, 18, true);
        label(root.ent(), 70 + i, "Band 8px / inner step 2.4px", x + 108, 528, 228, 22, 16);
        label(root.ent(), 80 + i, fmt::format("Corner tab {:.1f} screen px", tab_px), x + 108, 551, 228, 22, 16);
        label(root.ent(), 90 + i, "Tabs accent all four corners.", x + 108, 574, 228, 22, 16);
      } else if (i == 1) {
        label(root.ent(), 60 + i, "3x border diagram", x + 108, 501, 228, 24, 18, true);
        label(root.ent(), 70 + i, "Requested border: 3px", x + 108, 528, 228, 22, 16);
        label(root.ent(), 80 + i, "Drawn border: 1 screen px", x + 108, 551, 228, 22, 16);
        label(root.ent(), 90 + i, "Fill stays inside the frame.", x + 108, 574, 228, 22, 16);
      } else {
        label(root.ent(), 60 + i, "3x lighting diagram", x + 108, 501, 228, 24, 18, true);
        label(root.ent(), 70 + i, "Band 5px / light: upper left", x + 108, 528, 228, 22, 16);
        label(root.ent(), 80 + i, fmt::format("Edge {:.1f} screen px", edge_px), x + 108, 551, 228, 22, 16);
        label(root.ent(), 90 + i, "Dark top/left; light bottom/right", x + 108, 574, 228, 22, 15);
      }
      label(root.ent(), 100 + i, v.use, x, 612, 344, 24, 18, true);
      auto neutral = decorative_frame(context, mk(root.ent(), 110 + i),
          box(x, 651, 344, 43).with_custom_background({235, 235, 232, 255})
              .with_border({110, 110, 108, 255}, pixels(v.border_px * s))
              .with_debug_name("neutral_frame_" + std::to_string(i)), v.style);
      div(context, mk(neutral.ent(), 100), box(24, 9, 296, 25).with_label("Same neutral colors")
          .with_font("AtkinsonMock", pixels(17 * s)).with_custom_text_color({50, 50, 48, 255})
          .with_alignment(TextAlignment::Center).with_ignore_pointer_events());
    }
    label(root.ent(), 5, "Neutral palette / same styles and requested bands. Diagrams use 720p values at 3x.", 8, 633, 1128, 20, 16);
  }
};

REGISTER_EXAMPLE_SCREEN(decorative_frame, "Component Galleries",
                        "Decorative frame border styles",
                        DecorativeFrameShowcase)
