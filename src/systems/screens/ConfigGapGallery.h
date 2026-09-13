#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/ui/measure_config.h>
#include <afterhours/src/plugins/ui/text_input/text_input.h>
#include <afterhours/src/polyline.h>
#include <array>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ConfigGapGallery : ScreenSystem<UIContext<InputAction>> {
  float marquee = 0.f;
  bool animate_marquee = true;
  bool hide_the_middle = false;
  std::string field = "focus me, all selected";
  int press_count = 0;
  int release_count = 0;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float dt) override {
    if (animate_marquee) marquee += dt * 40.f;
    const auto theme = afterhours::ui::theme_presets::neon_dark();
    context.set_theme(theme);
    context.scaling_mode = ScalingMode::Proportional;
    const float s = std::min(context.screen_height / 720.f, context.screen_width / 1280.f);
    const float left = (context.screen_width - 1144 * s) / 2;
    const float top = (context.screen_height - 720 * s) / 2;
    const afterhours::Color muted{184, 197, 214, 255};
    const afterhours::Color cyan{120, 220, 255, 255};
    const afterhours::Color specimen_bg{40, 46, 62, 255};
    UIStylingDefaults::get().set_default_font("AtkinsonMock", pixels(18 * s));
    const auto box = [s](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, y * s).with_corner_radius(0)
          .with_background(Theme::Usage::None);
    };
    auto canvas = div(context, mk(entity), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background(theme.background).with_corner_radius(0).with_debug_name("cg_canvas"));
    auto root = div(context, mk(canvas.ent(), 0), box(0, 0, 1144, 720)
        .with_absolute_position(left, top).with_debug_name("cg_root"));
    const auto label = [&](afterhours::Entity &parent, int id, const std::string &value,
                           float x, float y, float w, float h, float size = 18.f,
                           const std::string &name = "", bool emphasis = false) {
      return div(context, mk(parent, id), box(x, y, w, h).with_label(value)
          .with_font(emphasis ? "AtkinsonMockBold" : "AtkinsonMock", pixels(size * s))
          .with_custom_text_color(muted).with_alignment(TextAlignment::Left)
          .with_ignore_pointer_events().with_debug_name(name));
    };
    const auto action = [&](afterhours::Entity &parent, int id, const std::string &value,
                            float x, float y, float w, float h, const std::string &name,
                            ClickActivationMode mode = ClickActivationMode::Release) {
      return button(context, mk(parent, id), box(x, y, w, h).with_label(value)
          .with_font("AtkinsonMock", pixels(18 * s)).with_background(Theme::Usage::Primary)
          .with_auto_text_color(true).with_alignment(TextAlignment::Center)
          .with_corner_radius(8 * s).with_click_activation(mode).with_debug_name(name));
    };
    div(context, mk(root.ent(), 0), box(0, 24, 1144, 72)
        .with_custom_background(theme.surface).with_corner_radius(12 * s));
    div(context, mk(root.ent(), 1), box(20, 28, 1088, 39)
        .with_label("Component configuration examples").with_font("AtkinsonMockBold", pixels(32 * s))
        .with_custom_text_color(theme.font).with_alignment(TextAlignment::Left).with_ignore_pointer_events());
    label(root.ent(), 2, "Compare requested settings with visible output. Measurements use 720p design pixels unless marked screen px.",
          20, 69, 1104, 23, 18);
    const auto make_row = [&](int index) {
      return hstack(context, mk(root.ent(), 10 + index), box(0, 112 + index * 302.f, 1144, 286)
          .with_no_wrap().with_gap(pixels(20 * s)).with_debug_wrap(index == 1)
          .with_debug_name("cg_row_" + std::to_string(index)));
    };
    const auto panel = [&](afterhours::Entity &row, int id, const std::string &title, const std::string &name) {
      auto result = div(context, mk(row, id), ComponentConfig{}
          .with_size({pixels(368 * s), pixels(286 * s)})
          .with_custom_background(theme.surface).with_corner_radius(12 * s).with_debug_name(name));
      label(result.ent(), 0, title, 12, 8, 344, 31, 20, "", true);
      return result;
    };
    auto first = make_row(0);
    {
      auto p = panel(first.ent(), 0, "shadow / corners", "cg_panel_shadow");
      const std::array<afterhours::Color, 3> fills{{{70, 110, 170, 255}, {70, 150, 120, 255}, {160, 110, 70, 255}}};
      for (int i = 0; i < 3; ++i) {
        const float x = 18 + i * 116.f;
        auto config = box(x, 47, 96, 64).with_custom_background(fills[i])
            .with_corner_radius((i == 0 ? 4 : i == 1 ? 18 : 20) * s)
            .with_debug_name("cg_shadow_" + std::to_string(i));
        if (i == 0) config.with_shadow(ShadowStyle::Hard, 5 * s, 5 * s, 8 * s);
        if (i == 1) config.with_shadow(ShadowStyle::Soft, 0, 8 * s, 8 * s);
        if (i == 2) config.with_segments(3).with_skip_grid_snap(true);
        div(context, mk(p.ent(), 10 + i), config);
        label(p.ent(), 20 + i, i == 0 ? "Hard / r4" : i == 1 ? "Soft / r18" : "3 requested",
              x - 4, 114, 116, 23, 15);
        label(p.ent(), 30 + i, i == 0 ? "offset 5, 5" : i == 1 ? "0, 8 / blur 8" : "Auto / r20",
              x - 4, 137, 116, 20, 15);
      }
      label(p.ent(), 40, "Matched pair: r18, offset 5, 5; soft blur 8", 14, 160, 340, 22, 16);
      for (int i = 0; i < 3; ++i) {
        const float x = 18 + i * 116.f;
        auto config = box(x, 187, 96, 64).with_custom_background(i == 2 ? fills[2] : fills[0])
            .with_corner_radius((i == 2 ? 20 : 18) * s).with_segments(12)
            .with_debug_name("cg_reference_" + std::to_string(i));
        if (i < 2) config.with_shadow(i == 0 ? ShadowStyle::Hard : ShadowStyle::Soft, 5 * s, 5 * s, 8 * s);
        div(context, mk(p.ent(), 50 + i), config);
        label(p.ent(), 60 + i, i == 0 ? "Hard" : i == 1 ? "Soft / blur 8" : "12 segments",
              x - 4, 257, 116, 22, 15);
      }
    }
    {
      auto p = panel(first.ent(), 1, "min / max width", "cg_panel_clamp");
      label(p.ent(), 1, "Min 90px / max 200px / parent 320px", 18, 40, 332, 24, 17);
      label(p.ent(), 2, "Line = requested; clamp skips grid snapping", 18, 64, 332, 21, 15);
      for (int i = 0; i < 3; ++i) {
        const float pct = .25f + .3f * static_cast<float>(i);
        const float y = 90 + i * 62.f;
        auto host = div(context, mk(p.ent(), 10 + i), box(24, y, 320, 58));
        div(context, mk(host.ent(), 0), box(0, 35, 320 * pct, 2)
            .with_custom_background({128, 146, 174, 255}).with_ignore_pointer_events());
        auto sample = div(context, mk(host.ent(), 1), ComponentConfig{}
            .with_size({percent(pct), pixels(34 * s)}).with_min_width(pixels(90 * s))
            .with_max_width(pixels(200 * s)).with_skip_grid_snap(true).with_min_height(pixels(30 * s)).with_max_height(pixels(40 * s))
            .with_label(fmt::format("{:.0f}%", pct * 100)).with_font("AtkinsonMock", pixels(18 * s))
            .with_custom_text_color(theme.font).with_alignment(TextAlignment::Center)
            .with_custom_background({56, 62, 82, 255}).with_corner_radius(6 * s)
            .with_debug_name("cg_clamp_" + std::to_string(i)));
        label(host.ent(), 2, fmt::format("{:.0f}px -> {:.0f}px / {}", 320 * pct,
              sample.cmp().rect().width / s, i == 0 ? "Min-clamped" : i == 2 ? "Max-clamped" : "Within bounds"),
              -4, 38, 328, 21, 16, "cg_clamp_readout_" + std::to_string(i));
      }
    }
    {
      auto p = panel(first.ent(), 2, "clip_children / hidden", "cg_panel_clip");
      label(p.ent(), 1, "4 source rows / viewport 76px", 18, 40, 332, 25, 18);
      div(context, mk(p.ent(), 2), box(18, 78, 188, 80).with_ignore_pointer_events()
          .with_on_draw_fg([cyan, s](RectangleType r) { afterhours::draw_rectangle_rounded_lines_ex(r, 0, 4, s, cyan); }));
      auto clip = vstack(context, mk(p.ent(), 3), box(20, 80, 184, 76)
          .with_custom_background({30, 34, 46, 255}).with_clip_children(true)
          .with_corner_radius(8 * s).with_no_wrap().with_debug_name("cg_clip"));
      for (int i = 0; i < 4; ++i) {
        div(context, mk(clip.ent(), i), ComponentConfig{}
            .with_label(fmt::format("row {} clipped", i)).with_size({percent(1.f), pixels(30 * s)})
            .with_custom_background({64, 78, 110, 255}).with_custom_text_color(theme.font)
            .with_font("AtkinsonMock", pixels(17 * s)).with_hidden(i == 2 && hide_the_middle)
            .with_margin(Margin{.bottom = pixels(2 * s)}).with_corner_radius(0)
            .with_debug_name("cg_clip_row_" + std::to_string(i)));
        label(p.ent(), 10 + i, std::to_string(i) + (i == 2 && hide_the_middle ? "  hidden" : "  source row"),
              218, 77 + i * 27.f, 132, 25, 16, "cg_source_" + std::to_string(i));
      }
      label(p.ent(), 20, hide_the_middle ? "Row 2 is removed from layout." : "Row 2 starts below the first two rows.",
            18, 191, 332, 25, 17, "cg_hidden_status");
      if (action(p.ent(), 21, hide_the_middle ? "Show row 2" : "Hide row 2", 20, 230, 328, 38, "cg_hide"))
        hide_the_middle = !hide_the_middle;
    }
    auto second = make_row(1);
    {
      auto p = panel(second.ent(), 0, "draw_bg / draw_fg", "cg_panel_draw");
      label(p.ent(), 1, "Background fill", 18, 43, 155, 24, 18);
      label(p.ent(), 2, "Foreground stroke", 183, 43, 166, 24, 18);
      const float phase = std::round(marquee * s);
      const float dash = std::max(1.f, std::round(10 * s));
      const float gap = std::max(1.f, std::round(7 * s));
      div(context, mk(p.ent(), 3), box(20, 77, 328, 106).with_label("selection")
          .with_font("AtkinsonMock", pixels(24 * s)).with_custom_text_color({210, 220, 240, 255})
          .with_alignment(TextAlignment::Center)
          .with_on_draw_bg([](RectangleType r) { afterhours::draw_rectangle(r, {38, 44, 60, 255}); })
          .with_on_draw_fg([phase, dash, gap, s, cyan](RectangleType r) {
            const float inset = 6 * s;
            const std::vector<Vector2Type> path{{r.x + inset, r.y + inset}, {r.x + r.width - inset, r.y + inset},
                {r.x + r.width - inset, r.y + r.height - inset}, {r.x + inset, r.y + r.height - inset},
                {r.x + inset, r.y + inset}};
            afterhours::polyline::draw_dashed(path, 2 * s, cyan, dash, gap, -phase);
          }).with_debug_name("cg_marquee"));
      label(p.ent(), 4, fmt::format("Dash {:.0f} / gap {:.0f} screen px / stroke {:.1f}", dash, gap, 2 * s),
            18, 193, 332, 25, 17);
      if (action(p.ent(), 5, animate_marquee ? "Pause marquee" : "Resume marquee", 20, 230, 328, 38, "cg_pause"))
        animate_marquee = !animate_marquee;
    }
    {
      auto p = panel(second.ent(), 1, "text / inset / wrap", "cg_panel_text");
      div(context, mk(p.ent(), 1), box(20, 43, 328, 29).with_label("Theme accent text")
          .with_custom_background(specimen_bg).with_text_color(Theme::Usage::Accent).with_auto_text_color(false)
          .with_font("AtkinsonMock", pixels(18 * s)).with_debug_name("cg_text_color"));
      label(p.ent(), 2, fmt::format("Theme accent text / #{:02X}{:02X}{:02X}", theme.accent.r, theme.accent.g, theme.accent.b),
            16, 75, 336, 23, 16);
      for (int i = 0; i < 2; ++i) {
        const float inset = i == 0 ? 24.f : 2.f;
        const float x = 20 + i * 172.f;
        div(context, mk(p.ent(), 10 + i), box(x, 105, 156, 32).with_label("Inset probe")
            .with_custom_background(specimen_bg).with_custom_text_color(theme.font)
            .with_alignment(TextAlignment::Left).with_text_inset(inset, 0)
            .with_font("AtkinsonMock", pixels(18 * s)).with_debug_name("cg_inset_" + std::to_string(i))
            .with_on_draw_fg([inset, cyan](RectangleType r) {
              afterhours::draw_rectangle({r.x + inset, r.y, 1, r.height}, {249, 182, 93, 255});
              afterhours::draw_rectangle({r.x + 5, r.y, 1, r.height}, cyan);
            }));
        label(p.ent(), 20 + i, fmt::format("Req. {:.0f} screen px", inset), x - 4, 140, 164, 21, 15);
      }
      label(p.ent(), 30, "Gold = requested; cyan = drawn 5 screen px", 16, 163, 336, 22, 15, "cg_inset_status");
      label(p.ent(), 31, "Child layout wrap", 16, 189, 164, 23, 17);
      label(p.ent(), 32, "Text overflow wrap", 188, 189, 164, 23, 17);
      auto wrap = hstack(context, mk(p.ent(), 33), box(20, 218, 156, 54).with_wrap()
          .with_custom_background(specimen_bg).with_debug_name("cg_flex_wrap"));
      for (int i = 0; i < 2; ++i)
        div(context, mk(wrap.ent(), i), ComponentConfig{}
            .with_size({pixels(90 * s), pixels(27 * s)}).with_label("Child " + std::to_string(i + 1))
            .with_font("AtkinsonMock", pixels(16 * s)).with_custom_text_color(theme.font)
            .with_custom_background({56, 62, 82, 255}).with_corner_radius(0)
            .with_debug_name("cg_wrap_child_" + std::to_string(i)));
      div(context, mk(p.ent(), 34), box(192, 218, 156, 54).with_label("Narrow text wraps onto two lines.")
          .with_font("AtkinsonMock", pixels(16 * s)).with_custom_text_color(theme.font)
          .with_custom_background(specimen_bg).with_text_overflow(TextOverflow::Wrap).with_debug_name("cg_text_wrap"));
    }
    {
      auto p = panel(second.ent(), 2, "activation / measure", "cg_panel_activation");
      if (action(p.ent(), 1, fmt::format("On pointer down: {}", press_count), 20, 45, 328, 36,
                 "cg_press", ClickActivationMode::Press)) ++press_count;
      if (action(p.ent(), 2, fmt::format("On pointer up: {}", release_count), 20, 88, 328, 36,
                 "cg_release", ClickActivationMode::Release)) ++release_count;
      if (action(p.ent(), 3, "Reset counters", 202, 131, 146, 29, "cg_reset")) {
        press_count = 0;
        release_count = 0;
      }
      label(p.ent(), 4, "measure_config", 16, 132, 182, 25, 18, "", true);
      ComponentConfig probe = ComponentConfig{}.with_label("measured, not guessed")
          .with_size({Size{Dim::Text}, Size{Dim::Text}}).with_font("AtkinsonMock", pixels(17 * s))
          .with_text_inset(5, 0).with_custom_text_color(theme.font);
      const auto measured = measure_config(probe, 328 * s);
      auto children_probe = probe;
      children_probe.with_size({children(), children()});
      const auto without_tree = measure_config(children_probe, 328 * s);
      div(context, mk(p.ent(), 5), probe.with_absolute_position(20 * s, 165 * s)
          .with_custom_background({46, 62, 52, 255}).with_corner_radius(0).with_debug_name("cg_probe"));
      label(p.ent(), 6, fmt::format("Text {:.0f}x{:.0f} / children {:.0f}x{:.0f} screen px", measured.size.x,
            measured.size.y, without_tree.size.x, without_tree.size.y), 16, 187, 336, 20, 16, "cg_measure");
      label(p.ent(), 7, "Text uses the label; children needs a tree.", 16, 207, 336, 20, 16);
      label(p.ent(), 8, "Tab selects all / arrow keys edit text", 16, 228, 336, 20, 16);
      text_input(context, mk(p.ent(), 9), field, box(20, 250, 328, 32)
          .with_font("AtkinsonMock", pixels(17 * s))
          .with_consumes_directional_input(true).with_corner_radius(6 * s).with_debug_name("cg_field"));
    }
  }
};

REGISTER_EXAMPLE_SCREEN(config_gap_gallery, "Component Galleries",
                        "shadow, clip, insets, on_draw hooks, click activation",
                        ConfigGapGallery)
