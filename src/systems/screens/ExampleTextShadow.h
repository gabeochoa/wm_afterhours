#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <array>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ExampleTextShadow : ScreenSystem<UIContext<InputAction>> {
  bool guides = false;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = theme_presets::neon_dark();
    context.scaling_mode = ScalingMode::Proportional;
    const afterhours::Color ink{28, 39, 57, 255}, muted{67, 84, 104, 255};
    const afterhours::Color panel{39, 47, 64, 255}, pale{192, 207, 230, 255}, white{255, 255, 255, 255};
    const float scale = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const float left = (context.screen_width / scale - 1144) / 2;
    const float top = (context.screen_height / scale - 720) / 2;
    const auto box = [scale, left, top](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * scale), pixels(h * scale)})
          .with_absolute_position((left + x) * scale, (top + y) * scale)
          .with_background(Theme::Usage::None).with_corner_radius(0);
    };
    auto root = div(context, mk(entity), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background({224, 231, 241, 255}).with_corner_radius(0).with_debug_name("bg"));
    const auto label = [&](int id, float x, float y, float w, float h, const std::string &text, float size,
                           afterhours::Color color, const std::string &name = "") {
      return div(context, mk(root.ent(), id), box(x, y, w, h).with_label(text)
          .with_font("AtkinsonMock", pixels(size * scale)).with_custom_text_color(color)
          .with_text_overflow(TextOverflow::Wrap).with_alignment(TextAlignment::Left)
          .with_ignore_pointer_events().with_debug_name(name));
    };
    const auto sample = [&](int id, float x, float y, float w, float h, const std::string &text, float size,
                            afterhours::Color color, TextShadow shadow, const std::string &name) {
      return div(context, mk(root.ent(), id), box(x, y, w, h).with_label(text)
          .with_font("AtkinsonMock", pixels(size * scale)).with_custom_text_color(color)
          .with_text_shadow(shadow).with_alignment(TextAlignment::Left).with_debug_name(name));
    };
    label(0, 0, 24, 884, 45, "Text drop shadow", 35, ink);
    label(1, 0, 79, 1144, 31, "Atkinson / native shadow rendering / metadata uses rendered pixel offsets", 22, muted);
    if (button(context, mk(root.ent(), 2), box(910, 29, 234, 43).with_label(guides ? "Box guides: On" : "Box guides: Off")
        .with_font("AtkinsonMock", pixels(21 * scale)).with_custom_background(white).with_custom_text_color(ink)
        .with_corner_radius(6 * scale).with_debug_name("shadow_guides"))) guides = !guides;
    const auto backing = [&](int id, float y, float h) {
      div(context, mk(root.ent(), id), box(0, y, 680, h).with_custom_background(panel).with_corner_radius(8 * scale));
    };
    const TextShadow none{{0, 0, 0, 0}, 0, 0};
    backing(10, 137, 108);
    sample(11, 20, 147, 306, 47, "SHADOW", 32, {255, 220, 80, 255}, none, "shadow_none");
    sample(12, 356, 147, 304, 47, "SHADOW", 32, {255, 220, 80, 255}, {{0, 0, 0, 255}, 6 * scale, 6 * scale}, "shadow_comparison");
    label(13, 20, 200, 306, 32, "S1 / No shadow / 32px at 1×", 19, pale);
    label(14, 356, 195, 304, 43, fmt::format("S2 / x +{:.1f}, y +{:.1f}px\nBlack / alpha 255 of 255", 6 * scale, 6 * scale), 19, pale);
    backing(20, 259, 118);
    sample(21, 20, 267, 306, 65, "SHADOW", 52, {255, 50, 150, 255}, TextShadow::soft(5 * scale, 5 * scale), "shadow_soft");
    sample(22, 356, 267, 304, 65, "SHADOW", 52, {255, 50, 150, 255}, TextShadow::hard(5 * scale, 5 * scale), "shadow_hard");
    label(23, 20, 329, 306, 41, fmt::format("S3 / Soft / offset +{:.1f}, +{:.1f}px\nAlpha 80 of 255 / no blur", 5 * scale, 5 * scale), 18, pale);
    label(24, 356, 329, 304, 41, fmt::format("S4 / Hard / offset +{:.1f}, +{:.1f}px\nAlpha 180 of 255 / no blur", 5 * scale, 5 * scale), 18, pale);
    backing(30, 391, 114);
    sample(31, 20, 400, 306, 62, "COLORED", 48, {80, 255, 255, 255}, {{200, 0, 0, 255}, 6 * scale, 6 * scale}, "shadow_colored");
    auto combo = sample(32, 356, 400, 304, 62, "COMBO", 48, {255, 180, 60, 255}, {{0, 0, 0, 120}, 5 * scale, 5 * scale}, "shadow_combo");
    combo.ent().get<HasLabel>().set_text_stroke(afterhours::Color{40, 20, 0, 255}, 3 * scale);
    label(33, 20, 460, 306, 40, fmt::format("S5 / #50FFFF + #C80000\nOffset +{:.1f}, +{:.1f}px / alpha 255", 6 * scale, 6 * scale), 18, pale);
    label(34, 356, 460, 304, 40, fmt::format("S6 / Outline {:.1f}px / x,y +{:.1f}px\nShadow alpha 120 of 255", 3 * scale, 5 * scale), 18, pale);
    backing(40, 519, 117);
    sample(41, 20, 532, 306, 50, "LIGHT", 34, white, none, "shadow_light_none");
    sample(42, 356, 532, 304, 50, "LIGHT", 34, white, {{0, 0, 0, 180}, 3 * scale, 3 * scale}, "shadow_light");
    label(43, 20, 592, 306, 30, "S7 / No shadow / 34px at 1×", 19, pale);
    label(44, 356, 586, 304, 41, fmt::format("S8 / offset +{:.1f}, +{:.1f}px\nBlack / alpha 180 of 255", 3 * scale, 3 * scale), 19, pale);
    div(context, mk(root.ent(), 50), box(704, 137, 440, 499).with_custom_background(panel).with_corner_radius(8 * scale));
    label(51, 724, 145, 400, 30, "Offset ladder / +x right, +y down", 22, white);
    const std::array<float, 7> offsets{0, 1, 2, 4, 6, 8, 10};
    for (size_t i = 0; i < offsets.size(); ++i) {
      const float y = 184 + static_cast<float>(i) * 62;
      const float offset = offsets[i] * scale;
      sample(60 + static_cast<int>(i), 734, y, 256, 48, "SHADOW", 34, {180, 100, 255, 255},
             {{60, 20, 100, 200}, offset, offset}, i == 0 ? "offset_zero" : "offset_" + std::to_string(i - 1));
      label(70 + static_cast<int>(i), 1002, y + 4, 122, 44,
            fmt::format("{:.1f} px\nx / y", offset), 21, pale);
      if (!guides) continue;
      div(context, mk(root.ent(), 80 + static_cast<int>(i)), box(724, y, 278, 48).with_ignore_pointer_events()
          .with_on_draw_fg([scale](RectangleType r) {
            const afterhours::Color guide{130, 151, 180, 255};
            afterhours::draw_line_ex({r.x, r.y}, {r.x + r.width, r.y}, scale, guide);
            afterhours::draw_line_ex({r.x, r.y + r.height}, {r.x + 8 * scale, r.y + r.height}, scale, guide);
          }));
    }
    label(90, 0, 650, 1144, 31, "Soft and Hard change opacity, not blur. The ladder uses #B464FF text and #3C1464 shadow, alpha 200 of 255.", 20, muted);
    label(91, 0, 686, 1144, 26, fmt::format("{:.0f} × {:.0f} / UI scale {:.2f}× / Ladder font {:.1f}px / Box guides show the unchanged text container.", context.screen_width, context.screen_height, scale, 34 * scale), 19, muted);
  }
};

REGISTER_EXAMPLE_SCREEN(text_shadow, "System Demos",
                        "Demonstrates native text drop shadow rendering",
                        ExampleTextShadow)
