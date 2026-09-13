#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <array>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ExampleTextOverflow : ScreenSystem<UIContext<InputAction>> {
  bool comparison = false;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = theme_presets::neon_dark();
    context.scaling_mode = ScalingMode::Proportional;
    const afterhours::Color ink{237, 243, 252, 255}, muted{183, 198, 219, 255};
    const afterhours::Color surface{35, 46, 63, 255}, edge{121, 141, 167, 255};
    const afterhours::Color green{130, 226, 171, 255}, orange{255, 199, 117, 255};
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
                           afterhours::Color color = afterhours::Color{183, 198, 219, 255}, const std::string &name = "") {
      return div(context, mk(root.ent(), id), box(x, y, w, h).with_label(text)
          .with_font("AtkinsonMock", pixels(size * scale)).with_custom_text_color(color)
          .with_text_overflow(TextOverflow::Wrap).with_alignment(TextAlignment::Left)
          .with_ignore_pointer_events().with_debug_name(name));
    };
    label(0, 0, 24, 825, 45, "Text overflow", 35, ink);
    label(1, 0, 78, 1144, 32, "Native text rendering / gray specimen frames / observed renderer limits are labeled.", 22);
    if (button(context, mk(root.ent(), 2), box(838, 27, 306, 44)
        .with_label(comparison ? "Show original specimens" : "Compare overflow modes")
        .with_font("AtkinsonMock", pixels(21 * scale)).with_custom_text_color(ink)
        .with_custom_background(surface).with_corner_radius(6 * scale).with_debug_name("overflow_compare"))) comparison = !comparison;
#ifdef AFTERHOURS_DEBUG_TEXT_OVERFLOW
    label(3, 0, 119, 264, 28, "Debug flag: ON", 21, green);
#else
    label(3, 0, 119, 264, 28, "Debug flag: OFF", 21, orange);
#endif
    div(context, mk(root.ent(), 4), box(289, 120, 26, 26).with_ignore_pointer_events()
        .with_custom_background({255, 50, 50, 60}).with_border(afterhours::Color{255, 50, 50, 200}, 2));
    label(5, 326, 119, 818, 28, "Debug border key / the batched renderer currently omits this overlay.", 19);
    auto &fonts = *afterhours::EntityHelper::get_singleton_cmp<FontManager>();
    const auto specimen = [&](int id, float x, float y, float w, float h, const std::string &text, float size,
                               TextOverflow mode, const std::string &name, const char *font = "AtkinsonMock") {
      return div(context, mk(root.ent(), id), box(x, y, w, h).with_label(text)
          .with_font(font, pixels(size * scale)).with_custom_background(surface)
          .with_custom_text_color(ink).with_alignment(TextAlignment::Left).with_text_overflow(mode)
          .with_border(edge, 1).with_debug_name(name));
    };
    const auto measurements = [&](const std::string &text, float w, float h, float size, const char *font = "AtkinsonMock") {
      const auto measured = afterhours::measure_text(fonts.get_font(font), text.c_str(), size * scale, 1.f);
      const auto inset = resolve_text_inset(context.theme);
      return fmt::format("Text {:.0f} × {:.0f}px / usable {:.0f} × {:.0f}px / font {:.1f}px", measured.x, measured.y,
                         w * scale - inset.x * 2, h * scale - inset.y * 2, size * scale);
    };
    if (!comparison) {
      label(10, 0, 166, 552, 34, "Pass specimens / content fits", 26, green);
      label(11, 592, 166, 552, 34, "Constrained specimens / ellipsis enabled", 25, orange);
      div(context, mk(root.ent(), 12), box(0, 210, 380, 100).with_custom_background(surface).with_border(edge, 1).with_debug_name("card_ok_1"));
      specimen(13, 10, 220, 360, 80, "This text fits perfectly", 22, TextOverflow::Ellipsis, "fit_large", "Gaegu-Bold");
      label(14, 0, 318, 552, 54, "F1 / Expected: fits\n" + measurements("This text fits perfectly", 360, 80, 22, "Gaegu-Bold"), 19);
      div(context, mk(root.ent(), 15), box(0, 383, 380, 60).with_custom_background(surface).with_border(edge, 1).with_debug_name("card_ok_2"));
      specimen(16, 10, 388, 360, 50, "Short text", 20, TextOverflow::Ellipsis, "fit_short");
      label(17, 0, 451, 552, 54, "F2 / Expected: fits\n" + measurements("Short text", 360, 50, 20), 19);
      div(context, mk(root.ent(), 18), box(0, 522, 52, 52).with_custom_background(surface).with_border(edge, 1).with_debug_name("card_ok_3"));
      specimen(19, 4, 526, 44, 44, "OK", 16, TextOverflow::Ellipsis, "fit_ok");
      label(20, 70, 526, 482, 56, "F3 / OK / 52 × 52px outer box at 1×\n" + measurements("OK", 44, 44, 16), 19);
      const std::array<std::string, 4> texts{"This is way too much text for this tiny container!", "Height is too small!", "No way this fits", "Long text in narrow space"};
      const std::array<float, 4> widths{140, 360, 36, 90}, heights{20, 40, 36, 34}, sizes{16, 20, 16, 14};
      const std::array<float, 4> outer_w{150, 380, 44, 100}, outer_h{30, 50, 44, 44};
      for (size_t i = 0; i < texts.size(); ++i) {
        const float y = 209 + static_cast<float>(i) * 96;
        const auto measured = afterhours::measure_text(fonts.get_font("AtkinsonMock"), texts[i].c_str(), sizes[i] * scale, 1.f);
        const bool width_fits = measured.x <= widths[i] * scale - 2 * resolve_text_inset(context.theme).x;
        div(context, mk(root.ent(), 30 + static_cast<int>(i)), box(592, y, outer_w[i], outer_h[i]).with_custom_background(surface)
            .with_border(edge, 1).with_debug_name("card_overflow_" + std::to_string(i + 1)));
        specimen(40 + static_cast<int>(i), 592 + (outer_w[i] - widths[i]) / 2, y + (outer_h[i] - heights[i]) / 2,
                 widths[i], heights[i], texts[i], sizes[i], TextOverflow::Ellipsis, "limit_" + std::to_string(i));
        label(50 + static_cast<int>(i), 592, y + 53, 552, 39,
              i == 1 ? "C2 / Expected: fits / " + measurements(texts[i], widths[i], heights[i], sizes[i])
                     : fmt::format("C{} / {} / box {:.0f} × {:.0f}px, font {:.1f}px", i + 1, width_fits ? "Fits" : "Source exceeds width; ellipsis", outer_w[i] * scale, outer_h[i] * scale, sizes[i] * scale), 18);
      }
      label(60, 0, 603, 1144, 61, "C2 retains its original source phrase, but the text fits this height. Compare modes isolates clipping and ellipsis at matching dimensions.", 21);
    }
    if (comparison) {
      const std::string long_text = "This is way too much text for this tiny container!";
      label(70, 0, 165, 1144, 30, "M1 / Same source, 150 × 40px label boxes at 1×, Atkinson 16px", 23, ink);
      label(71, 0, 199, 1144, 29, "Source: " + long_text, 21);
      specimen(72, 0, 242, 150, 40, long_text, 16, TextOverflow::Clip, "mode_clip");
      specimen(73, 340, 242, 150, 40, long_text, 16, TextOverflow::Ellipsis, "card_ellipsis_1");
      label(74, 0, 292, 326, 52, "Clip / expected: containment\nObserved: text escapes its box.", 20, orange);
      label(75, 340, 292, 248, 52, "Ellipsis / truncated\nWarning still emitted.", 20, green);
      label(76, 592, 242, 552, 92, measurements(long_text, 150, 40, 16) + "\nWrapping is preferable when all text must remain visible.", 21);
      label(77, 0, 360, 1144, 30, "M2 / Source: Long text in narrow space gets truncated nicely", 22, ink);
      specimen(78, 0, 400, 90, 34, "Long text in narrow space gets truncated nicely", 14, TextOverflow::Clip, "narrow_clip");
      specimen(79, 340, 400, 90, 34, "Long text in narrow space gets truncated nicely", 14, TextOverflow::Ellipsis, "card_ellipsis_2");
      label(80, 0, 443, 326, 28, "Clip / observed: no clipping", 20, orange);
      label(81, 340, 443, 248, 28, "Ellipsis / 90 × 34px at 1×", 20, green);
      label(82, 592, 390, 552, 70, measurements("Long text in narrow space gets truncated nicely", 90, 34, 14), 21);
      const std::array<float, 5> widths{70, 55, 44, 44, 44}, sizes{16, 14, 12, 12, 12};
      label(83, 0, 487, 1144, 30, "A1–A5 / ABC sequence / both dimensions and requested font size change", 22, green);
      for (size_t i = 0; i < widths.size(); ++i) {
        const float x = static_cast<float>(i) * 230;
        div(context, mk(root.ent(), 90 + static_cast<int>(i)), box(x, 531, widths[i], widths[i])
            .with_custom_background(surface).with_border(edge, 1).with_debug_name("shrink_box_" + std::to_string(i)));
        specimen(100 + static_cast<int>(i), x + 4, 535, widths[i] - 8, widths[i] - 8, "ABC", sizes[i], TextOverflow::Ellipsis,
                 "shrink_text_" + std::to_string(i));
        const float inner = (widths[i] - 8) * scale;
        const auto inset = resolve_text_inset(context.theme);
        const auto text = afterhours::measure_text(fonts.get_font("AtkinsonMock"), "ABC", sizes[i] * scale, 1.f);
        const bool fits = text.x <= inner - 2 * inset.x && text.y <= inner - 2 * inset.y;
        label(110 + static_cast<int>(i), x, 609, 220, 51,
              fmt::format("A{} / {:.0f} × {:.0f}px\nFont {:.1f}px / {}", i + 1, widths[i] * scale, widths[i] * scale, sizes[i] * scale, fits ? "Fits" : "Ellipsis expected"), 19);
      }
    }
    label(120, 0, 679, 1144, 28, fmt::format("{:.0f} × {:.0f} / UI scale {:.2f}× / Native font measurement uses rendered pixels. Touch target guidance is separate.", context.screen_width, context.screen_height, scale), 19);
  }
};

REGISTER_EXAMPLE_SCREEN(text_overflow, "Tools",
                        "Demonstrates text overflow debug indicators",
                        ExampleTextOverflow)
