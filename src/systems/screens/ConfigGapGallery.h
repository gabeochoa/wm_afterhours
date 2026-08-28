#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/ui/measure_config.h>
#include <afterhours/src/plugins/ui/text_input/text_input.h>
#include <afterhours/src/polyline.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// ComponentConfig options nothing in wm was setting. Each panel is the option
// next to a plain control, so the difference is the thing you see rather than
// something to take on trust.
struct ConfigGapGallery : ScreenSystem<UIContext<InputAction>> {
  float marquee = 0.f;
  bool hide_the_middle = false;
  std::string field = "focus me, all selected";
  int press_count = 0;
  int release_count = 0;

  static void panel_title(UIContext<InputAction> &context,
                          afterhours::Entity &parent, int idx,
                          const char *text) {
    div(context, mk(parent, idx),
        ComponentConfig{}
            .with_label(text)
            .with_size(ComponentSize{percent(1.f), pixels(26)})
            .with_background(Theme::Usage::None)
            .with_custom_text_color(afterhours::Color{140, 200, 255, 255})
            .with_font_size(pixels(16.f))
            .with_alignment(TextAlignment::Left)
            .with_text_inset(4.f, 0.f));
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float dt) override {
    marquee += dt * 40.f;

    auto theme = afterhours::ui::theme_presets::neon_dark();
    theme.roundness = 0.12f;
    context.theme = theme;
    context.scaling_mode = ScalingMode::Adaptive;
    UIStylingDefaults::get().set_default_font(UIComponent::DEFAULT_FONT,
                                              pixels(17.0f));

    auto root = vstack(context, mk(entity),
                       ComponentConfig{}
                           .with_size(ComponentSize{screen_pct(0.96f),
                                                    screen_pct(0.96f)})
                           .with_self_align(SelfAlign::Center)
                           .with_background(Theme::Usage::Background)
                           .with_padding(Spacing::sm)
                           .with_no_wrap()
                           .with_debug_name("cg_root"));

    div(context, mk(root.ent(), 0),
        ComponentConfig{}
            .with_label("Config options that had no caller")
            .with_size(ComponentSize{percent(1.f), pixels(40)})
            .with_background(Theme::Usage::Surface)
            .with_auto_text_color(true)
            .with_font_size(pixels(25.f))
            .with_alignment(TextAlignment::Center)
            .with_corner_radius(10.f)
            .with_margin(Margin{.bottom = DefaultSpacing::small()}));

    auto grid = vstack(context, mk(root.ent(), 1),
                       ComponentConfig{}
                           .with_size(ComponentSize{percent(1.f), expand()})
                           .with_no_wrap()
                           .with_debug_name("cg_grid"));

    auto make_row = [&](int r) {
      return hstack(context, mk(grid.ent(), r),
                    ComponentConfig{}
                        .with_size(ComponentSize{percent(1.f), percent(0.5f)})
                        .with_no_wrap()
                        // Logs why this row wrapped or did not, which is the
                        // only way to see a NoWrap decision from outside.
                        .with_debug_wrap(r == 1)
                        .with_debug_name(fmt::format("cg_row_{}", r)));
    };

    auto make_panel = [&](auto &row, int c, const char *title) {
      auto p = vstack(context, mk(row.ent(), c),
                      ComponentConfig{}
                          .with_size(ComponentSize{expand(), percent(0.96f)})
                          .with_background(Theme::Usage::Surface)
                          .with_padding(Spacing::sm)
                          .with_margin(Margin{.right = pixels(6)})
                          .with_corner_radius(9.f)
                          .with_no_wrap()
                          .with_debug_name(fmt::format("cg_panel_{}", title)));
      panel_title(context, p.ent(), 0, title);
      return p;
    };

    auto row0 = make_row(0);

    // --- shadow + corner_radius + segments ---------------------------
    {
      auto p = make_panel(row0, 0, "shadow, corner_radius, segments");
      auto strip = hstack(context, mk(p.ent(), 1),
                          ComponentConfig{}
                              .with_size(ComponentSize{percent(1.f), expand()})
                              .with_align_items(AlignItems::Center)
                              .with_no_wrap());
      div(context, mk(strip.ent(), 0),
          ComponentConfig{}
              .with_label("hard")
              .with_size(ComponentSize{pixels(96), pixels(64)})
              .with_custom_background(afterhours::Color{70, 110, 170, 255})
              .with_shadow(ShadowStyle::Hard, 5.f, 5.f)
              .with_corner_radius(4.f)
              .with_margin(Margin{.right = pixels(10)}));
      div(context, mk(strip.ent(), 1),
          ComponentConfig{}
              .with_label("soft")
              .with_size(ComponentSize{pixels(96), pixels(64)})
              .with_custom_background(afterhours::Color{70, 150, 120, 255})
              .with_shadow(ShadowStyle::Soft, 0.f, 8.f)
              .with_corner_radius(18.f)
              .with_margin(Margin{.right = pixels(10)}));
      // Three segments is a visibly faceted corner, which is what the option
      // is for: cheap corners, or a deliberately chunky look.
      div(context, mk(strip.ent(), 2),
          ComponentConfig{}
              .with_label("3 seg")
              .with_size(ComponentSize{pixels(96), pixels(64)})
              .with_custom_background(afterhours::Color{160, 110, 70, 255})
              .with_corner_radius(20.f)
              .with_segments(3)
              // Opted out of grid snapping, so this one keeps its exact
              // position where its neighbours are rounded to the grid.
              .with_skip_grid_snap(true));
    }

    // --- min/max width and height ------------------------------------
    {
      auto p = make_panel(row0, 1, "min and max width clamp a percent");
      for (int i = 0; i < 3; i++) {
        const float pct = 0.25f + 0.3f * (float)i;
        div(context, mk(p.ent(), 1 + i),
            ComponentConfig{}
                .with_label(fmt::format("{:.0f}% clamped 90..200", pct * 100.f))
                .with_size(ComponentSize{percent(pct), pixels(34)})
                .with_min_width(pixels(90))
                .with_max_width(pixels(200))
                .with_min_height(pixels(30))
                .with_max_height(pixels(40))
                .with_custom_background(afterhours::Color{56, 62, 82, 255})
                .with_font_size(pixels(14.f))
                .with_corner_radius(6.f)
                .with_margin(Margin{.bottom = pixels(6)}));
      }
    }

    // --- clip_children + hidden --------------------------------------
    {
      auto p = make_panel(row0, 2, "clip_children, hidden");
      auto clip = vstack(context, mk(p.ent(), 1),
                         ComponentConfig{}
                             .with_size(ComponentSize{percent(1.f), pixels(76)})
                             .with_custom_background(afterhours::Color{30, 34, 46, 255})
                             .with_clip_children(true)
                             .with_corner_radius(8.f)
                             .with_no_wrap()
                             .with_debug_name("cg_clip"));
      for (int i = 0; i < 4; i++)
        div(context, mk(clip.ent(), i),
            ComponentConfig{}
                .with_label(fmt::format("row {} clipped", i))
                .with_size(ComponentSize{percent(1.f), pixels(30)})
                .with_custom_background(afterhours::Color{64, 78, 110, 255})
                .with_font_size(pixels(14.f))
                .with_hidden(i == 2 && hide_the_middle)
                .with_margin(Margin{.bottom = pixels(2)}));

      if (button(context, mk(p.ent(), 2),
                 ComponentConfig{}
                     .with_label(hide_the_middle ? "show row 2" : "hide row 2")
                     .with_size(ComponentSize{percent(1.f), pixels(34)})
                     .with_font_size(pixels(15.f))
                     .with_corner_radius(6.f)
                     .with_debug_name("cg_hide")))
        hide_the_middle = !hide_the_middle;
    }

    auto row1 = make_row(1);

    // --- on_draw_bg / on_draw_fg, drawing a dashed marquee ------------
    {
      auto p = make_panel(row1, 0, "on_draw_bg, on_draw_fg, dashed polyline");
      const float phase = marquee;
      div(context, mk(p.ent(), 1),
          ComponentConfig{}
              .with_label("selection")
              .with_size(ComponentSize{percent(1.f), expand()})
              .with_background(Theme::Usage::None)
              .with_custom_text_color(afterhours::Color{210, 220, 240, 255})
              .with_on_draw_bg([](RectangleType rr) {
                afterhours::draw_rectangle(
                    rr, afterhours::Color{38, 44, 60, 255});
              })
              .with_on_draw_fg([phase](RectangleType rr) {
                const float in = 6.f;
                std::vector<Vector2Type> box{
                    {rr.x + in, rr.y + in},
                    {rr.x + rr.width - in, rr.y + in},
                    {rr.x + rr.width - in, rr.y + rr.height - in},
                    {rr.x + in, rr.y + rr.height - in},
                    {rr.x + in, rr.y + in}};
                afterhours::polyline::draw_dashed(
                    box, 2.f, afterhours::Color{120, 220, 255, 255}, 10.f, 7.f,
                    -phase);
              })
              .with_debug_name("cg_marquee"));
    }

    // --- text_color, text_inset, wrap ---------------------------------
    {
      auto p = make_panel(row1, 1, "text_color, text_inset, wrap");
      div(context, mk(p.ent(), 1),
          ComponentConfig{}
              .with_label("with_text_color paints the label from a theme slot")
              .with_size(ComponentSize{percent(1.f), pixels(52)})
              .with_custom_background(afterhours::Color{40, 46, 62, 255})
              .with_text_color(Theme::Usage::Accent)
              .with_auto_text_color(false)
              .with_font_size(pixels(14.f))
              .with_wrap()
              .with_corner_radius(6.f)
              .with_margin(Margin{.bottom = pixels(6)}));
      div(context, mk(p.ent(), 2),
          ComponentConfig{}
              .with_label("inset 24px")
              .with_size(ComponentSize{percent(1.f), pixels(34)})
              .with_custom_background(afterhours::Color{40, 46, 62, 255})
              .with_alignment(TextAlignment::Left)
              .with_text_inset(24.f, 0.f)
              .with_font_size(pixels(14.f))
              .with_corner_radius(6.f)
              .with_margin(Margin{.bottom = pixels(6)}));
      div(context, mk(p.ent(), 3),
          ComponentConfig{}
              .with_label("inset 2px")
              .with_size(ComponentSize{percent(1.f), pixels(34)})
              .with_custom_background(afterhours::Color{40, 46, 62, 255})
              .with_alignment(TextAlignment::Left)
              .with_text_inset(2.f, 0.f)
              .with_font_size(pixels(14.f))
              .with_corner_radius(6.f));
    }

    // --- click_activation + measure_config ----------------------------
    {
      auto p = make_panel(row1, 2, "click_activation, measure_config");

      if (button(context, mk(p.ent(), 1),
                 ComponentConfig{}
                     .with_label(fmt::format("on Press: {}", press_count))
                     .with_size(ComponentSize{percent(1.f), pixels(38)})
                     .with_click_activation(ClickActivationMode::Press)
                     .with_font_size(pixels(15.f))
                     .with_corner_radius(6.f)
                     .with_margin(Margin{.bottom = pixels(6)})
                     .with_debug_name("cg_press")))
        press_count++;

      if (button(context, mk(p.ent(), 2),
                 ComponentConfig{}
                     .with_label(fmt::format("on Release: {}", release_count))
                     .with_size(ComponentSize{percent(1.f), pixels(38)})
                     .with_click_activation(ClickActivationMode::Release)
                     .with_font_size(pixels(15.f))
                     .with_corner_radius(6.f)
                     .with_margin(Margin{.bottom = pixels(10)})
                     .with_debug_name("cg_release")))
        release_count++;

      // Sizing a box to text without building the box first, which is what
      // windowing a variable-height list needs.
      ComponentConfig probe = ComponentConfig{}
                                  .with_label("measured, not guessed")
                                  .with_size(ComponentSize{children(), children()})
                                  .with_font_size(pixels(15.f));
      const auto m = measure_config(probe, 320.f);
      div(context, mk(p.ent(), 3),
          ComponentConfig{}
              .with_label(fmt::format("measure_config says {:.0f}x{:.0f}",
                                      m.size.x, m.size.y))
              .with_size(ComponentSize{percent(1.f), pixels(34)})
              .with_custom_background(afterhours::Color{46, 62, 52, 255})
              .with_custom_text_color(afterhours::Color{170, 230, 180, 255})
              .with_font_size(pixels(14.f))
              .with_corner_radius(6.f)
              .with_margin(Margin{.bottom = pixels(6)})
              .with_debug_name("cg_measure"));

      // select_on_focus selects the whole value when focus lands, so typing
      // replaces rather than appends. consumes_directional_input keeps the
      // arrow keys in the field instead of letting them move focus.
      text_input(context, mk(p.ent(), 4), field,
                 ComponentConfig{}
                     .with_size(ComponentSize{percent(1.f), pixels(34)})
                     .with_font_size(pixels(14.f))
                     .with_select_on_focus(true)
                     .with_consumes_directional_input(true)
                     .with_corner_radius(6.f)
                     .with_debug_name("cg_field"));
    }
  }
};

REGISTER_EXAMPLE_SCREEN(config_gap_gallery, "Component Galleries",
                        "shadow, clip, insets, on_draw hooks, click activation",
                        ConfigGapGallery)
