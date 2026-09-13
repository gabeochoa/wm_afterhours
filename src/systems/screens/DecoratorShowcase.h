#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/ui/ui_decorators.h>
#include <array>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct DecoratorShowcase : ScreenSystem<UIContext<InputAction>> {
  bool show_measurements = true;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    // Dark sci-fi theme
    Theme theme;
    theme.background = afterhours::Color{10, 12, 16, 255};
    theme.surface = afterhours::Color{20, 24, 30, 255};
    theme.primary = afterhours::Color{70, 160, 160, 255};
    theme.secondary = afterhours::Color{50, 120, 120, 255};
    theme.accent = afterhours::Color{200, 160, 80, 255};
    theme.font = afterhours::Color{210, 220, 230, 255};
    theme.font_muted = afterhours::Color{160, 175, 185, 255};
    context.set_theme(theme);
    context.scaling_mode = ScalingMode::Proportional;
    const float s = std::min(context.screen_height / 720.f, context.screen_width / 1280.f);
    const float left = (context.screen_width - 1144 * s) / 2;
    const float top = (context.screen_height - 720 * s) / 2;
    UIStylingDefaults::get().set_default_font("AtkinsonMock", pixels(18 * s));
    using C = afterhours::Color;
    const C teal{65, 150, 150, 255};
    const C amber{200, 160, 80, 255};
    const C green{100, 200, 120, 255};
    const C panel_bg{16, 22, 26, 255};
    const C panel_border{35, 50, 55, 255};
    const auto box = [s](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, y * s).with_background(Theme::Usage::None).with_corner_radius(0);
    };
    auto canvas = div(context, mk(entity, 0), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background(theme.background).with_corner_radius(0).with_debug_name("decorators_canvas"));
    auto root = div(context, mk(canvas.ent(), 0), box(0, 0, 1144, 720)
        .with_absolute_position(left, top).with_debug_name("root"));
    const auto label = [&](afterhours::Entity &parent, int id, const std::string &value,
                           float x, float y, float w, float h, float size, C color,
                           const std::string &name = "", bool strong = false) {
      return div(context, mk(parent, id), box(x, y, w, h).with_label(value)
          .with_font(strong ? "AtkinsonMockBold" : "AtkinsonMock", pixels(size * s))
          .with_custom_text_color(color).with_alignment(TextAlignment::Left)
          .with_ignore_pointer_events().with_debug_name(name));
    };
    const auto centered = [&](afterhours::Entity &parent, int id, const std::string &value,
                              float x, float y, float w, float h, float size, C color,
                              const std::string &name = "") {
      return div(context, mk(parent, id), box(x, y, w, h).with_label(value)
          .with_font("AtkinsonMock", pixels(size * s)).with_custom_text_color(color)
          .with_alignment(TextAlignment::Center).with_ignore_pointer_events().with_debug_name(name));
    };
    div(context, mk(root.ent(), 0), box(0, 24, 1144, 80)
        .with_custom_background(theme.surface).with_corner_radius(12 * s));
    label(root.ent(), 1, "UI decorators", 20, 29, 724, 38, 34, theme.font, "", true);
    label(root.ent(), 2, "Add corner brackets, grid lines and quote bars to existing components.",
          20, 72, 1104, 25, 19, theme.font_muted);
    if (button(context, mk(root.ent(), 3), box(878, 37, 246, 34)
        .with_label(show_measurements ? "Hide measurements" : "Show measurements")
        .with_font("AtkinsonMock", pixels(18 * s)).with_background(Theme::Usage::Primary)
        .with_auto_text_color(true).with_corner_radius(8 * s).with_alignment(TextAlignment::Center)
        .with_debug_name("decorator_measurements"))) show_measurements = !show_measurements;
    const std::array<const char *, 3> headings{"Corner brackets", "Grid backgrounds", "Quote bars"};
    const std::array<const char *, 3> api{"with_brackets()", "with_grid_bg()", "with_quote()"};
    for (int i = 0; i < 3; ++i) {
      const float x = i * 396.f;
      label(root.ent(), 10 + i, headings[i], x + 8, 116, 336, 31, 26, theme.font, "", true);
      div(context, mk(root.ent(), 20 + i), box(x + 8, 149, 336, 27)
          .with_label(std::string(".decorate(") + api[i] + ")")
          .with_font("AtkinsonMock", pixels(20 * s)).with_custom_text_color(teal)
          .with_alignment(TextAlignment::Left).with_ignore_pointer_events());
    }
    const auto panel_config = [&](float x, float y, float h, C fill, C border, const std::string &name) {
      return box(x, y, 328, h).with_custom_background(fill).with_border(border, 1.f)
          .with_padding(Padding::all(pixels(0))).with_debug_name(name);
    };
    {
      auto panel = div(context, mk(root.ent(), 30), panel_config(12, 182, 100, panel_bg, panel_border, "bracket_panel_1"))
          .decorate(with_brackets(context, teal, 18 * s, 2 * s));
      centered(panel.ent(), 10, "SYSTEM STATUS", 16, 13, 296, 32, 24, teal);
      div(context, mk(panel.ent(), 11), box(72, 51, 184, 2).with_custom_background({teal.r, teal.g, teal.b, 80}));
      centered(panel.ent(), 12, "All systems operational", 16, 63, 296, 27, 18, theme.font_muted);
    }
    {
      auto panel = div(context, mk(root.ent(), 31), panel_config(12, 348, 100, {25, 20, 14, 255}, {55, 45, 25, 255}, "bracket_panel_2"))
          .decorate(with_brackets(context, amber, 22 * s, 3 * s));
      div(context, mk(panel.ent(), 10), box(70, 14, 24, 26).with_ignore_pointer_events()
          .with_on_draw_fg([amber, s](RectangleType r) {
            const raylib::Vector2 a{r.x + r.width / 2, r.y};
            const raylib::Vector2 b{r.x + r.width, r.y + r.height};
            const raylib::Vector2 c{r.x, r.y + r.height};
            raylib::DrawLineEx(a, b, 2 * s, amber);
            raylib::DrawLineEx(b, c, 2 * s, amber);
            raylib::DrawLineEx(c, a, 2 * s, amber);
            raylib::DrawLineEx({a.x, r.y + 9 * s}, {a.x, r.y + 16 * s}, 2 * s, amber);
            raylib::DrawCircleV({a.x, r.y + 21 * s}, 1.4f * s, amber);
          }));
      label(panel.ent(), 11, "WARNING", 102, 10, 190, 35, 24, amber);
      label(panel.ent(), 12, "Hull integrity", 28, 61, 184, 26, 20, {210, 180, 110, 255});
      label(panel.ent(), 13, "47%", 218, 49, 92, 44, 34, amber, "hull_integrity", true);
    }
    {
      auto panel = div(context, mk(root.ent(), 32), panel_config(12, 536, 56, {12, 22, 16, 255}, {30, 55, 35, 255}, "bracket_panel_3"))
          .decorate(with_brackets(context, green, 10 * s, 2 * s));
      div(context, mk(panel.ent(), 10), box(108, 22, 12, 12).with_custom_background(green)
          .with_corner_radius(6 * s).with_debug_name("status_dot"));
      label(panel.ent(), 11, "ONLINE", 128, 12, 140, 32, 24, green);
    }
    const std::array<float, 3> bracket_lengths{18, 22, 10};
    const std::array<float, 3> bracket_strokes{2, 3, 2};
    const std::array<const char *, 3> status_notes{"Teal / system information", "Amber / warning", "Green / available"};
    for (int i = 0; i < 3; ++i) {
      const float y = 292 + i * 166.f;
      label(root.ent(), 40 + i, status_notes[i], 8, y, 336, 25, 18, theme.font_muted);
      label(root.ent(), 50 + i, show_measurements ? fmt::format("Bracket {:.0f}px / stroke {:.0f}px{}", bracket_lengths[i],
            bracket_strokes[i], i == 2 ? " / 56px panel" : "") : "Color carries meaning; brackets add shape.",
            8, y + 27, 336, 22, 16, theme.font_muted, "bracket_caption_" + std::to_string(i));
    }
    const std::array<float, 3> cell_sizes{20, 12, 24};
    const std::array<C, 3> grid_colors{{{35, 75, 75, 180}, {50, 130, 130, 120}, {40, 65, 70, 100}}};
    const std::array<C, 3> grid_fills{{{10, 18, 20, 255}, {8, 14, 16, 255}, panel_bg}};
    const std::array<const char *, 3> grid_names{"grid_panel_1", "grid_panel_2", "combo_panel"};
    for (int i = 0; i < 3; ++i) {
      const float y = 182 + i * 166.f;
      auto panel = div(context, mk(root.ent(), 60 + i), panel_config(408, y, 100, grid_fills[i],
          i == 0 ? C{30, 65, 65, 255} : i == 1 ? teal : panel_border, grid_names[i]))
          .decorate(with_grid_bg(context, cell_sizes[i] * s, grid_colors[i], s));
      if (i == 0)
        div(context, mk(panel.ent(), 100), box(161, 47, 6, 6).with_custom_background(teal)
            .with_corner_radius(3 * s).with_debug_name("radar_dot"));
      if (i == 2) {
        panel.decorate(with_brackets(context, amber, 16 * s, 2 * s));
        centered(panel.ent(), 101, "SECTOR 7-G", 16, 33, 296, 34, 24, amber);
      }
      label(root.ent(), 70 + i, i == 0 ? "Coarse 20px / center marker" : i == 1 ? "Dense 12px grid" : "Grid + brackets / 24px",
            404, y + 110, 336, 25, 18, theme.font_muted);
      label(root.ent(), 80 + i, show_measurements ? fmt::format("Line #{:02X}{:02X}{:02X} / alpha {}/255", grid_colors[i].r,
            grid_colors[i].g, grid_colors[i].b, grid_colors[i].a) : "Grid spacing changes density, not state.",
            404, y + 137, 336, 22, 16, theme.font_muted, "grid_caption_" + std::to_string(i));
    }
    const std::array<C, 3> quote_colors{teal, amber, green};
    const std::array<float, 3> quote_widths{4, 5, 4};
    const std::array<C, 3> quote_fills{panel_bg, C{22, 18, 12, 255}, C{12, 20, 14, 255}};
    const std::array<C, 3> quote_borders{panel_border, C{50, 40, 25, 255}, C{30, 50, 32, 255}};
    for (int i = 0; i < 3; ++i) {
      const float y = 182 + i * 166.f;
      auto panel = hstack(context, mk(root.ent(), 90 + i), panel_config(804, y, 100, quote_fills[i], quote_borders[i],
          "quote_panel_" + std::to_string(i + 1)))
          .decorate(with_quote(context, QuoteStyle{.accent_color = quote_colors[i], .accent_width = quote_widths[i] * s}));
      if (i == 2) panel.decorate(with_brackets(context, green, 12 * s, 2 * s));
      auto inner = div(context, mk(panel.ent(), 100), box(24, 12, 280, 76)
          .with_debug_name("quote_inner_" + std::to_string(i + 1)));
      if (i == 0) {
        div(context, mk(inner.ent(), 0), box(0, 0, 280, 76)
            .with_label("The only way to do great work\nis to love what you do.")
            .with_font("AtkinsonMock", pixels(19 * s)).with_custom_text_color(theme.font)
            .with_text_overflow(TextOverflow::Wrap).with_alignment(TextAlignment::Left));
      } else {
        label(inner.ent(), 1, i == 1 ? "I should go." : "The cake is a lie.", 0, 5, 280, 34, 22, theme.font);
        label(inner.ent(), 2, i == 1 ? "Commander Shepard" : "GLaDOS", 0, 46, 280, 26, 18,
              {quote_colors[i].r, quote_colors[i].g, quote_colors[i].b, 220});
      }
      label(root.ent(), 100 + i, i == 0 ? "No attribution" : i == 1 ? "Quote + attribution" : "Quote + brackets + attribution",
            800, y + 110, 336, 25, 18, theme.font_muted);
      label(root.ent(), 110 + i, show_measurements ? fmt::format("Accent {:.0f}px / wrapper inset 24px", quote_widths[i])
            : "Color is independent of quotation style.", 800, y + 137, 336, 22, 16, theme.font_muted,
            "quote_caption_" + std::to_string(i));
    }
    div(context, mk(root.ent(), 120), box(0, 683, 1144, 25)
        .with_custom_background(theme.surface).with_corner_radius(6 * s));
    label(root.ent(), 121, "720p parameters; quote text adds 5 screen px. Colors supply meaning; decorators add visuals. Ruler: 12px.",
          8, 684, 974, 23, 16, theme.font_muted);
    div(context, mk(root.ent(), 122), box(986, 688, 144, 13).with_ignore_pointer_events()
        .with_on_draw_fg([s, teal, visible = show_measurements](RectangleType r) {
          if (!visible) return;
          raylib::DrawLineEx({r.x, r.y + 6 * s}, {r.x + r.width, r.y + 6 * s}, s, teal);
          for (float x = r.x; x <= r.x + r.width; x += 12 * s)
            raylib::DrawLineEx({x, r.y}, {x, r.y + 12 * s}, s, teal);
        }));
  }
};

REGISTER_EXAMPLE_SCREEN(decorators, "Component Galleries",
                        "UI decorator higher-order components",
                        DecoratorShowcase)
