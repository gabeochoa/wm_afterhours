#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <array>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ExampleTextStroke : ScreenSystem<UIContext<InputAction>> {
  int detail = 0;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = theme_presets::neon_dark();
    context.scaling_mode = ScalingMode::Proportional;
    const afterhours::Color white{255, 255, 255, 255}, muted{181, 197, 218, 255};
    const afterhours::Color panel{37, 47, 64, 255};
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
        .with_custom_background({19, 28, 43, 255}).with_corner_radius(0).with_debug_name("bg"));
    const auto label = [&](int id, float x, float y, float w, float h, const std::string &text, float size,
                           afterhours::Color color, const std::string &name = "") {
      return div(context, mk(root.ent(), id), box(x, y, w, h).with_label(text)
          .with_font("AtkinsonMock", pixels(size * scale)).with_custom_text_color(color)
          .with_text_overflow(TextOverflow::Wrap).with_alignment(TextAlignment::Left)
          .with_ignore_pointer_events().with_debug_name(name));
    };
    const auto sample = [&](int id, float x, float y, float w, float h, const std::string &text, float size,
                            afterhours::Color color, afterhours::Color outline, float thickness, const std::string &name) {
      return div(context, mk(root.ent(), id), box(x, y, w, h).with_label(text)
          .with_font("AtkinsonMock", pixels(size * scale)).with_custom_text_color(color)
          .with_text_stroke(outline, thickness * scale).with_alignment(TextAlignment::Left).with_debug_name(name));
    };
    label(0, 0, 24, 884, 45, "Text stroke / outline", 35, white);
    label(1, 0, 79, 1144, 31, "Atkinson / native eight-direction glyph copies / no blurred glow", 22, muted);
    const std::array<const char *, 3> detail_labels{"Glyph detail: Off", "Glyph detail: 5px", "Glyph detail: 10px"};
    if (button(context, mk(root.ent(), 2), box(910, 29, 234, 43).with_label(detail_labels[static_cast<size_t>(detail)])
        .with_font("AtkinsonMock", pixels(21 * scale)).with_custom_background(panel).with_custom_text_color(white)
        .with_corner_radius(6 * scale).with_debug_name("stroke_detail_toggle"))) detail = (detail + 1) % 3;
    const auto backing = [&](int id, float y, float h) {
      div(context, mk(root.ent(), id), box(0, y, 680, h).with_custom_background(panel).with_corner_radius(8 * scale));
    };
    backing(10, 137, 98);
    sample(11, 20, 146, 306, 55, "STROKE", 36, {255, 220, 80, 255}, {20, 15, 0, 255}, 0, "stroke_none");
    sample(12, 356, 146, 304, 55, "STROKE", 36, {255, 220, 80, 255}, {20, 15, 0, 255}, 5, "stroke_comparison");
    label(13, 20, 201, 306, 28, "T1 / No stroke / 36px at 1×", 19, muted);
    label(14, 356, 201, 304, 28, fmt::format("T2 / {:.1f}px stroke / 13.9% of font", 5 * scale), 18, muted);
    backing(20, 249, 140);
    sample(21, 20, 258, 306, 91, "BOLD", 72, {255, 50, 150, 255}, {40, 0, 60, 255}, 8, "stroke_bold");
    sample(22, 326, 270, 334, 78, "CONTRAST", 56, {80, 255, 255, 255}, {180, 20, 20, 255}, 8, "stroke_contrast");
    label(23, 20, 348, 306, 36, fmt::format("T3 / {:.1f}px font, {:.1f}px stroke\nOutline is 11.1% of font size", 72 * scale, 8 * scale), 18, muted);
    label(24, 356, 348, 304, 36, fmt::format("T4 / #50FFFF + #B41414\nFont {:.1f}px / stroke {:.1f}px", 56 * scale, 8 * scale), 18, muted);
    backing(30, 403, 112);
    sample(31, 20, 414, 306, 77, "GLOW", 56, {20, 20, 40, 255}, {80, 220, 255, 255}, 8, "stroke_bright");
    label(32, 356, 419, 304, 84, fmt::format("T5 / Bright outline\n#141428 + #50DCFF\nFont {:.1f}px / stroke {:.1f}px", 56 * scale, 8 * scale), 21, muted);
    div(context, mk(root.ent(), 40), box(0, 529, 680, 111).with_custom_background({220, 225, 235, 255}).with_corner_radius(8 * scale).with_debug_name("light_bg"));
    sample(41, 20, 535, 306, 58, "WHITE", 40, white, {0, 0, 0, 255}, 0, "stroke_white_none");
    sample(42, 356, 535, 304, 58, "WHITE", 40, white, {0, 0, 0, 255}, 4, "stroke_white");
    label(43, 20, 603, 306, 27, "T6 / Low contrast / no stroke", 20, {40, 52, 71, 255});
    label(44, 356, 603, 304, 27, fmt::format("T7 / Outlined / {:.1f}px black", 4 * scale), 20, {40, 52, 71, 255});
    div(context, mk(root.ent(), 50), box(704, 137, 440, 503).with_custom_background(panel).with_corner_radius(8 * scale));
    if (detail == 0) {
      label(51, 724, 146, 400, 33, "Thickness ladder / 40px font at 1×", 23, white);
      const std::array<float, 6> thicknesses{0, 2, 4, 6, 8, 10};
      for (size_t i = 0; i < thicknesses.size(); ++i) {
        const float y = 185 + static_cast<float>(i) * 70;
        sample(60 + static_cast<int>(i), 724, y, 247, 62, "STROKE", 40, {255, 180, 60, 255}, {80, 40, 0, 255}, thicknesses[i],
               i == 0 ? "stroke_zero" : "thickness_" + std::to_string(i - 1));
        label(70 + static_cast<int>(i), 987, y + 6, 137, 49,
              fmt::format("{:.1f} px\n{:.0f}% of font", thicknesses[i] * scale, thicknesses[i] / 40 * 100), 20, muted);
      }
      label(80, 724, 611, 400, 23, "10px at 1× is an extreme specimen.", 18, muted);
    }
    if (detail != 0) {
      const float font = detail == 1 ? 180.f : 200.f;
      const float width = detail == 1 ? 25.f : 50.f;
      label(81, 724, 149, 400, 58, detail == 1 ? "T2 glyph detail / 5× redraw" : "Extreme glyph detail / 5× redraw", 26, white);
      auto glyph = sample(82, 744, 216, 360, 300, "A", font, detail == 1 ? afterhours::Color{255, 220, 80, 255} : afterhours::Color{255, 180, 60, 255},
                          detail == 1 ? afterhours::Color{20, 15, 0, 255} : afterhours::Color{80, 40, 0, 255}, width, "stroke_glyph_detail");
      glyph.ent().get<HasLabel>().alignment = TextAlignment::Center;
      label(83, 724, 530, 400, 89, fmt::format("Native glyph redrawn at {:.0f}px\nOutline {:.0f}px / {:.1f}% of font\nEight offset copies, then foreground.", font * scale, width * scale, width / font * 100), 22, muted);
    }
    label(90, 0, 650, 1144, 31, "Light panel: #DCE1EB / foreground #FFFFFF. Stroke copies extend in eight directions; thick outlines can form gaps.", 20, muted);
    label(91, 0, 686, 1144, 26, fmt::format("{:.0f} × {:.0f} / UI scale {:.2f}× / Labels show rendered stroke pixels. Compare stroke size with its font size.", context.screen_width, context.screen_height, scale), 19, muted);
  }
};

REGISTER_EXAMPLE_SCREEN(text_stroke, "System Demos",
                        "Demonstrates native text stroke/outline rendering",
                        ExampleTextStroke)
