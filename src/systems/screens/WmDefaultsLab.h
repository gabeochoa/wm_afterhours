#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// The values WM adopts over the library defaults (wm_defaults.h), shown
// live on a preset theme: release-activation clicks, an outset focus ring,
// and text-area rows that scale with the font. The library defaults
// upstream are unchanged; nothing here opts in per widget except the one
// button that deliberately opts back into Press.
struct WmDefaultsLab : ScreenSystem<UIContext<InputAction>> {
  int release_clicks = 0;
  int press_clicks = 0;
  bool small_checked = true;
  std::string draft = "First line\nSecond line\nThird line";
  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = afterhours::ui::theme_presets::neon_dark();
    theme.accent = {149, 169, 192, 255};
    context.theme = theme;
    context.scaling_mode = ScalingMode::Proportional;
    UIStylingDefaults::get().set_grid_snapping(false);
    const float scale = std::min(context.screen_width / 1280.f,
                                 context.screen_height / 720.f);
    UIStylingDefaults::get().set_default_font("AtkinsonMock",
                                               pixels(20 * scale));
    auto root = div(context, mk(entity), ComponentConfig{}
        .with_size({screen_pct(1), screen_pct(1)}).with_background(Theme::Usage::Background)
        .with_corner_radius(0).with_padding(Padding::all(w1280(16))).with_debug_name("wmd_root"));
    auto text = [&](int id, const std::string &label, float x, float y,
                    float w, float h, float size, const std::string &name = "") {
      return div(context, mk(root.ent(), id), ComponentConfig{}
          .with_size({pixels(w * scale), pixels(h * scale)})
          .with_absolute_position(x * scale, y * scale).with_label(label)
          .with_font("AtkinsonMock", pixels(size * scale)).with_custom_text_color(theme.font)
          .with_text_overflow(TextOverflow::Wrap).with_ignore_pointer_events().with_debug_name(name));
    };
    text(0, "WM defaults lab", 16, 0, 700, 44, 30, "wmd_title");
    text(1, "Adopted in WM over the library defaults. Upstream afterhours defaults are unchanged; values proven here can be promoted later.",
         16, 48, 1180, 28, 19);

    text(10, "Click activation (library default: Press)", 40, 112, 700, 30, 20);
    if (button(context, mk(root.ent(), 11), ComponentConfig{}
            .with_label("WM default (Release): " + std::to_string(release_clicks))
            .with_720p_size(320, 48)
            .with_absolute_position(40 * scale, 152 * scale)
            .with_font("AtkinsonMock", pixels(19 * scale)).with_background(Theme::Usage::Primary)
            .with_corner_radius(6 * scale).with_debug_name("wmd_release_btn")))
      ++release_clicks;
    if (button(context, mk(root.ent(), 12), ComponentConfig{}
            .with_label("Explicit Press opt-in: " + std::to_string(press_clicks))
            .with_720p_size(320, 48)
            .with_absolute_position(400 * scale, 152 * scale)
            .with_font("AtkinsonMock", pixels(19 * scale)).with_background(Theme::Usage::Surface)
            .with_corner_radius(6 * scale)
            .with_click_activation(ClickActivationMode::Press)
            .with_debug_name("wmd_press_btn")))
      ++press_clicks;
    text(13, "Buttons commit on mouse-up, so dragging off before release cancels. Game-input widgets opt back into Press per widget.",
         40, 214, 1100, 28, 17);

    text(20, "Focus ring (library: 3px at +4 inset)", 40, 272, 700, 30, 20);
    checkbox(context, mk(root.ent(), 21), small_checked, ComponentConfig{}
        .with_size({pixels(28 * scale), pixels(28 * scale)})
        .with_absolute_position(40 * scale, 314 * scale)
        .with_debug_name("wmd_small_checkbox"));
    text(22, "Tab to the small checkbox: the ring sits outside the box instead of over the check glyph.",
         92, 318, 1000, 28, 17);

    text(30, "Text area rows (library: fixed 20px)", 40, 382, 700, 30, 20);
    auto area = text_area(context, mk(root.ent(), 31), draft, ComponentConfig{}
        .with_size({pixels(560 * scale), pixels(118 * scale)})
        .with_absolute_position(40 * scale, 422 * scale)
        .with_font("AtkinsonMock", pixels(20 * scale))
        .with_debug_name("wmd_area"));
    const auto &area_state =
        area.ent().get<afterhours::text_input::HasTextAreaState>();
    const float line_h = area_state.area_config.line_height;
    const float font_px = area_state.render_font_size;
    text(32, fmt::format("area line height: {:.1f}px at {:.1f}px font (ratio {:.2f})",
                         line_h, font_px, font_px > 0.f ? line_h / font_px : 0.f),
         40, 554, 900, 28, 17, "wmd_line_readout");
    text(33, "This field sets no line height of its own; the row height follows the font instead of clipping larger text.",
         40, 586, 1100, 28, 17);

    text(40, fmt::format("theme: click {}, ring {:.1f} / {:+.0f}, text-area ratio {:.2f}",
                         context.theme.click_activation_mode ==
                                 ClickActivationMode::Release
                             ? "Release"
                             : "Press",
                         context.theme.focus_ring_thickness,
                         context.theme.focus_ring_offset,
                         context.theme.text_area_line_height_ratio),
         40, 644, 1100, 30, 19, "wmd_theme_readout");
  }
};

REGISTER_EXAMPLE_SCREEN(wm_defaults_lab, "System Demos",
                        "WM defaults: release clicks, outset focus ring, font-scaled text-area rows",
                        WmDefaultsLab)
