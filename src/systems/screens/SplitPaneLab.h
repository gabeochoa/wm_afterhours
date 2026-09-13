#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Resizable split panes (D30). Drag the vertical bar to resize the sidebar,
// the horizontal one to resize the preview -- nested panes divide only the
// region they sit in.
struct SplitPaneLab : ScreenSystem<UIContext<InputAction>> {
  // The ratios and limits live here; the native panes enforce the limits while dragging.
  float sidebar_ratio = 0.28f;
  float preview_ratio = 0.62f;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();
    const float s = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const float left = (context.screen_width - 1280 * s) / 2;
    const float top = (context.screen_height - 720 * s) / 2;
    const auto white = afterhours::Color{235, 240, 250, 255};
    const auto muted = afterhours::Color{183, 198, 222, 255};
    const auto bar = afterhours::Color{104, 130, 177, 255};
    const auto panel_a = afterhours::Color{46, 56, 80, 255};
    const auto panel_b = afterhours::Color{28, 34, 50, 255};
    const auto panel_c = afterhours::Color{18, 21, 32, 255};
    UIStylingDefaults::get().set_default_font("AtkinsonMock", pixels(20 * s));
    div(context, mk(entity, 900), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_absolute_position(0, 0).with_custom_background(panel_c).with_corner_radius(0));
    auto root = div(context, mk(entity, 901), ComponentConfig{}
        .with_size({pixels(1280 * s), pixels(720 * s)})
        .with_absolute_position(left, top).with_transparent_bg().with_skip_grid_snap(true)
        .with_debug_name("sp_root"));
    auto box = [&](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, y * s).with_transparent_bg()
          .with_corner_radius(0).with_skip_grid_snap(true);
    };
    auto text = [&](int id, std::string value, float x, float y, float w, float h,
                    float size, afterhours::Color color, const char *debug = "", bool bold = false) {
      div(context, mk(root.ent(), id), box(x, y, w, h).with_label(value)
          .with_font(bold ? "AtkinsonMockBold" : "AtkinsonMock", pixels(size * s))
          .with_alignment(TextAlignment::Left).with_custom_text_color(color)
          .with_ignore_pointer_events().with_debug_name(debug));
    };
    text(0, "Split panes", 48, 10, 620, 40, 36, white, "sp_title", true);
    text(1, "Drag either divider. Preview and Details share the right pane.", 48, 50, 700, 26, 19, muted);
    auto preset = [&](int id, const char *label, float x, float w, const char *debug) {
      return button(context, mk(root.ent(), id), box(x, 20, w, 36).with_label(label)
          .with_font("AtkinsonMock", pixels(18 * s)).with_custom_background(bar)
          .with_custom_text_color(white).with_corner_radius(6 * s).with_debug_name(debug));
    };
    if (preset(10, "Reset 28% / 62%", 728, 180, "sp_reset")) {
      sidebar_ratio = 0.28f;
      preview_ratio = 0.62f;
    }
    if (preset(11, "Narrow sidebar", 916, 148, "sp_narrow")) sidebar_ratio = 0.15f;
    if (preset(12, "Tall preview", 1072, 156, "sp_tall")) preview_ratio = 0.85f;

    // The limits go to the widget so it pins mid drag; clamping after the call
    // let the bar overshoot and snap back on alternate frames.
    auto [sidebar, vbar, content] = hsplit_pane(context, mk(root.ent(), 2), sidebar_ratio,
        box(48, 84, 1180, 560).with_custom_background(panel_c).with_debug_name("sp_outer"), 0.15f, 0.55f);
    // restyle carries colour but not roundness -- roundness only lands
    // alongside an explicit corner set, so it is not worth passing here.
    vbar.restyle(context, ComponentConfig{}.with_custom_background(bar));
    vbar.ent().addComponentIfMissing<HasOnDraw>().fg = [white](RectangleType r) {
          afterhours::draw_rectangle({r.x + 1, r.y + r.height / 2 - 16, std::max(1.f, r.width - 2), 32}, white);
        };
    vbar.ent().get<UIComponentDebug>().set("sp_vertical_divider");
    sidebar.restyle(context, ComponentConfig{}.with_custom_background(panel_a));
    auto [preview, hbar, details] = vsplit_pane(context, mk(content.ent(), 0), preview_ratio,
        ComponentConfig{}.with_debug_name("sp_inner"), 0.2f, 0.85f);
    hbar.restyle(context, ComponentConfig{}.with_custom_background(bar));
    hbar.ent().addComponentIfMissing<HasOnDraw>().fg = [white](RectangleType r) {
          afterhours::draw_rectangle({r.x + r.width / 2 - 16, r.y + 1, 32, std::max(1.f, r.height - 2)}, white);
        };
    hbar.ent().get<UIComponentDebug>().set("sp_horizontal_divider");
    preview.restyle(context, ComponentConfig{}.with_custom_background(panel_b));
    details.restyle(context, ComponentConfig{}.with_custom_background(panel_a));
    auto label_pane = [&](ElementResult pane, const char *name, const char *debug, const char *metrics) {
      const auto r = pane.cmp().rect();
      div(context, mk(pane.ent(), 0), ComponentConfig{}
          .with_size({pixels(std::max(0.f, r.width - 24 * s)), pixels(30 * s)})
          .with_absolute_position(12 * s, 8 * s).with_label(name)
          .with_font("AtkinsonMockBold", pixels(24 * s)).with_alignment(TextAlignment::Left)
          .with_custom_text_color(white).with_transparent_bg().with_ignore_pointer_events().with_debug_name(debug));
      div(context, mk(pane.ent(), 1), ComponentConfig{}
          .with_size({pixels(std::max(0.f, r.width - 24 * s)), pixels(30 * s)})
          .with_absolute_position(12 * s, std::max(40 * s, (r.height - 30 * s) / 2))
          .with_label(fmt::format("{:.0f} × {:.0f} px", r.width, r.height))
          .with_font("AtkinsonMock", pixels(24 * s)).with_alignment(TextAlignment::Center)
          .with_custom_text_color(muted).with_transparent_bg().with_ignore_pointer_events().with_debug_name(metrics));
    };
    label_pane(sidebar, "Sidebar", "sp_sidebar_label", "sp_sidebar_size");
    label_pane(preview, "Preview", "sp_preview_label", "sp_preview_size");
    label_pane(details, "Details", "sp_details_label", "sp_details_size");
    const auto content_rect = content.cmp().rect();
    const float details_share = content_rect.height > 0 ? 100.f * details.cmp().rect().height / content_rect.height : 0.f;
    text(20, fmt::format("Sidebar width: {:.0f}% of workspace", sidebar_ratio * 100), 48, 650, 380, 24, 18, white, "sp_readout");
    text(21, fmt::format("Preview height: {:.0f}% of right pane", preview_ratio * 100), 442, 650, 402, 24, 18, white, "sp_preview_ratio");
    text(22, fmt::format("Details height: {:.1f}% of right pane", details_share), 846, 650, 382, 24, 18, white, "sp_details_ratio");
    text(23, "Sidebar limit 15–55%", 48, 677, 380, 22, 16, muted);
    text(24, "Preview limit 20–85%", 442, 677, 380, 22, 16, muted);
    text(25, "Details take the rest after the 4 px divider.", 846, 677, 382, 22, 16, muted);
    text(26, "Hover for resize arrows. Both divider hit regions are 4 screen px wide; white grips mark their centers.",
         48, 700, 1180, 20, 15, muted, "sp_hint");
  }
};

REGISTER_EXAMPLE_SCREEN(split_pane_lab, "System Demos",
                        "Resizable split panes with draggable dividers",
                        SplitPaneLab)
