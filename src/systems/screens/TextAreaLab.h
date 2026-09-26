#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// text_area parity with text_input: a placeholder in the empty field, a
// caller background (or transparent) instead of the hardcoded fill, the
// 2px accent border while focused, word motion that collapses a
// selection to its near edge, and auto-grow that resolves its line
// height and padding at the current resolution.
struct TextAreaLab : ScreenSystem<UIContext<InputAction>> {
  std::string draft;
  std::string custom_text = "Custom background, kept";
  std::string ghost_text = "Transparent over the panel";
  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = afterhours::ui::theme_presets::neon_dark();
    theme.accent = {149, 169, 192, 255};
    context.theme = theme;
    context.scaling_mode = ScalingMode::Proportional;
    UIStylingDefaults::get().set_grid_snapping(false);
    const float scale = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    UIStylingDefaults::get().set_default_font("AtkinsonMock", pixels(20 * scale));
    auto root = div(context, mk(entity), ComponentConfig{}
        .with_size({screen_pct(1), screen_pct(1)}).with_background(Theme::Usage::Background)
        .with_corner_radius(0).with_padding(Padding::all(w1280(16))).with_debug_name("tal_root"));
    auto text = [&](int id, const std::string &label, float x, float y, float w, float h,
                    float size) {
      return div(context, mk(root.ent(), id), ComponentConfig{}
          .with_size({pixels(w * scale), pixels(h * scale)})
          .with_absolute_position(x * scale, y * scale).with_label(label)
          .with_font("AtkinsonMock", pixels(size * scale)).with_custom_text_color(theme.font)
          .with_text_overflow(TextOverflow::Wrap).with_ignore_pointer_events());
    };
    text(0, "Text area lab", 16, 0, 700, 44, 30);
    text(1, "Click or Tab into a field: it takes the accent border text_input has. The composer auto-grows; Shift+Enter breaks a line.",
         16, 48, 1180, 28, 19);

    text(2, "Composer (placeholder + auto-grow)", 32, 108, 560, 30, 21);
    text_area(context, mk(root.ent(), 3), draft, ComponentConfig{}
        .with_size({pixels(560 * scale), pixels(40 * scale)})
        .with_absolute_position(32 * scale, 146 * scale)
        .with_font("AtkinsonMock", pixels(19 * scale))
        .with_line_height(pixels(26 * scale))
        .with_placeholder("Write a reply...")
        .with_auto_grow()
        .with_max_lines(6)
        .with_debug_name("composer"));

    text(4, "Caller background", 32, 330, 560, 30, 21);
    text_area(context, mk(root.ent(), 5), custom_text, ComponentConfig{}
        .with_size({pixels(560 * scale), pixels(84 * scale)})
        .with_absolute_position(32 * scale, 368 * scale)
        .with_font("AtkinsonMock", pixels(19 * scale))
        .with_line_height(pixels(26 * scale))
        .with_custom_background(afterhours::Color{88, 44, 120, 255})
        .with_debug_name("custom_area"));

    text(6, "Transparent background", 660, 108, 560, 30, 21);
    div(context, mk(root.ent(), 7), ComponentConfig{}
        .with_size({pixels(560 * scale), pixels(120 * scale)})
        .with_absolute_position(660 * scale, 146 * scale)
        .with_background(Theme::Usage::Primary)
        .with_corner_radius(8 * scale));
    text_area(context, mk(root.ent(), 8), ghost_text, ComponentConfig{}
        .with_size({pixels(528 * scale), pixels(88 * scale)})
        .with_absolute_position(676 * scale, 162 * scale)
        .with_font("AtkinsonMock", pixels(19 * scale))
        .with_line_height(pixels(26 * scale))
        .with_transparent_bg()
        .with_debug_name("ghost_area"));

    text(9, "Select a word and press Alt+Left: the selection collapses to its edge instead of jumping a word.",
         660, 300, 560, 52, 17);
    text(10, "Scripts can assert a multi-line field: expect_input_text composer \"hello\\nworld\".",
         660, 360, 560, 52, 17);
  }
};

REGISTER_EXAMPLE_SCREEN(text_area_lab, "System Demos",
                        "text_area parity: placeholder, backgrounds, focus ring, auto-grow",
                        TextAreaLab)
