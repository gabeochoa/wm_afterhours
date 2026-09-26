#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// with_tooltip. Triggers sit in the corners and along the edges on purpose:
// placement goes through the same flipping a dropdown uses, so the interesting
// case is the one with no room below.
struct TooltipLab : ScreenSystem<UIContext<InputAction>> {
  bool show_bounds = false;
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
        .with_corner_radius(0).with_padding(Padding::all(w1280(16))).with_debug_name("ttl_root"));
    auto at = [scale](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * scale), pixels(h * scale)})
          .with_absolute_position(x * scale, y * scale).with_corner_radius(0)
          .with_background(Theme::Usage::None);
    };
    auto text = [&](int id, const std::string &label, float x, float y, float w, float h,
                    float size, const std::string &name = "") {
      return div(context, mk(root.ent(), id), at(x, y, w, h).with_label(label)
          .with_font("AtkinsonMock", pixels(size * scale)).with_custom_text_color(theme.font)
          .with_text_overflow(TextOverflow::Wrap).with_ignore_pointer_events().with_debug_name(name));
    };
    text(0, "Tooltip placement lab", 16, 0, 630, 44, 30, "ttl_title");
    text(1, "Hover a trigger. Default delay: 400 ms / instant: 0 ms.", 16, 48, 1080, 28, 19);
    if (button(context, mk(root.ent(), 2), at(16, 203, 214, 36)
        .with_label(show_bounds ? "Hide edge guide" : "Show edge guide")
        .with_font("AtkinsonMock", pixels(18 * scale)).with_background(Theme::Usage::Surface)
        .with_debug_name("ttl_bounds"))) show_bounds = !show_bounds;
    if (show_bounds)
      div(context, mk(entity, 90), ComponentConfig{}
          .with_size({pixels(context.screen_width - 4), pixels(context.screen_height - 4)})
          .with_absolute_position(2, 2).with_custom_background(afterhours::colors::transparent()).with_corner_radius(0)
          .with_border(theme.accent, 1).with_render_layer(20).with_ignore_pointer_events().with_debug_name("ttl_edge_guide"));
    struct Spot {
      const char *label;
      const char *tip;
      const char *expectation;
      float x, y; // fractions of the screen
    };
    static const Spot spots[] = {
        {"top left", "Plenty of room below this one", "Normal below", .04f, .16f},
        {"top right", "Clamped so it stays on screen near this right edge", "Clamp to right edge", .80f, .16f},
        {"middle", "A longer tooltip, to show the box grows with the text", "Long content", .38f, .46f},
        {"bottom left", "No room below, so this one flips above", "Flip above", .04f, .90f},
        {"bottom right", "Flipped and clamped at once near the viewport corner", "Flip above and clamp", .80f, .90f},
        {"Instant, 0 ms", "delay 0, shows the moment you arrive", "Instant / 0 ms", .38f, .70f},
    };
    std::string active = "None";
    for (int i = 0; i < 6; ++i) {
      const auto &spot = spots[i];
      auto trigger = button(context, mk(root.ent(), 10 + i), ComponentConfig{}
          .with_label(spot.label).with_720p_size(160, 40)
          .with_absolute_position(spot.x * context.screen_width, spot.y * context.screen_height)
          .with_font("AtkinsonMock", pixels(19 * scale)).with_background(Theme::Usage::Primary)
          .with_corner_radius(6 * scale).with_tooltip(spot.tip, i == 5 ? 0.f : .4f)
          .with_debug_name("ttl_btn_" + std::to_string(i)));
      if (context.prev_hot_id == trigger.ent().id) active = spot.expectation;
      const float x = spot.x * context.screen_width;
      const float y = spot.y * context.screen_height + (i == 3 || i == 4 ? -32 : 45) * scale;
      div(context, mk(root.ent(), 30 + i), ComponentConfig{}
          .with_size({pixels(230 * scale), pixels(27 * scale)}).with_absolute_position(x, y)
          .with_label(spot.expectation).with_font("AtkinsonMock", pixels(16 * scale))
          .with_custom_text_color(theme.font_muted).with_ignore_pointer_events());
    }
    text(40, "Active case: " + active, 644, 0, 580, 44, 21, "ttl_active");
    text(69, "Sizes: same words, caller font / padding / gap", 730, 374, 520, 30, 20);
    struct SizeSpot {
      const char *label;
      const char *caption;
      float font, pad, gap;
      bool custom;
    };
    static const SizeSpot size_spots[] = {
        {"Small tip", "font 12 / pad 4 / gap 2", 12, 4, 2, true},
        {"Default tip", "library defaults", 0, 0, 0, false},
        {"Large tip", "font 30 / pad 18 / gap 14", 30, 18, 14, true},
    };
    for (int i = 0; i < 3; ++i) {
      const auto &spot = size_spots[i];
      auto config = ComponentConfig{}
          .with_label(spot.label).with_720p_size(170, 40)
          .with_absolute_position((640 + static_cast<float>(i) * 190) * scale, 418 * scale)
          .with_font("AtkinsonMock", pixels(19 * scale)).with_background(Theme::Usage::Primary)
          .with_corner_radius(6 * scale)
          .with_tooltip("Same words, different tooltip", .2f)
          .with_debug_name("ttl_size_" + std::to_string(i));
      if (spot.custom)
        config.with_tooltip_font_size(pixels(spot.font * scale))
            .with_tooltip_padding(spot.pad)
            .with_tooltip_gap(spot.gap);
      button(context, mk(root.ent(), 70 + i), config);
      div(context, mk(root.ent(), 80 + i), ComponentConfig{}
          .with_size({pixels(190 * scale), pixels(27 * scale)})
          .with_absolute_position((640 + static_cast<float>(i) * 190) * scale, 463 * scale)
          .with_label(spot.caption).with_font("AtkinsonMock", pixels(16 * scale))
          .with_custom_text_color(theme.font_muted).with_ignore_pointer_events());
    }
    div(context, mk(root.ent(), 41), at(16, 257, 420, 328)
        .with_custom_background(theme.surface).with_corner_radius(8 * scale));
    text(42, "Expected placement", 32, 269, 388, 30, 22);
    const char *rows[] = {
        "Top left / below / Plenty of room below",
        "Top right / clamp / Stay on screen",
        "Middle / below / Box grows with content",
        "Bottom left / above / No room below",
        "Bottom right / above + clamp / Corner",
        "Instant / below / Shows on arrival"};
    for (int i = 0; i < 6; ++i)
      text(50 + i, rows[i], 32, 311 + static_cast<float>(i) * 32, 388, 28, 17);
    text(60, "Triggers: 160 × 40 at 1280 × 720.\nEdge positions follow the viewport.",
         32, 516, 388, 52, 17, "ttl_geometry");
    text(61, "Native tooltip: 14 px text / 8 × 5 px padding / 4 px gap", 466, 571, 714, 32, 16);
  }
};

REGISTER_EXAMPLE_SCREEN(tooltip_lab, "System Demos",
                        "with_tooltip, including edge flipping", TooltipLab)
