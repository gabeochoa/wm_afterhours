#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <algorithm>
#include <array>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Per-span font weight in styled labels (gap D31).
//
// DGOne / DGOne@bold resolves a real same-family weight pair. The Gaegu-Bold
// fallback specimens have no @bold variant registered, so both requested
// weights resolve to Gaegu-Bold.
struct StyledTextLab : ScreenSystem<UIContext<InputAction>> {
  static constexpr float FS = 20.f;
  bool guides = false;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();
    const float s = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const float left = (context.screen_width - 1280 * s) / 2;
    const float top = (context.screen_height - 720 * s) / 2;
    const auto white = afterhours::Color{235, 240, 250, 255};
    const auto muted = afterhours::Color{180, 196, 218, 255};
    const auto box_color = afterhours::Color{48, 61, 85, 255};
    const auto amber = afterhours::Color{240, 200, 110, 255};
    const auto green = afterhours::Color{120, 220, 150, 255};
    using afterhours::colors::FontWeight;
    UIStylingDefaults::get().set_default_font("AtkinsonMock", pixels(20 * s));
    div(context, mk(entity, 900), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_absolute_position(0, 0).with_custom_background({16, 23, 36, 255}).with_corner_radius(0));
    auto root = div(context, mk(entity, 901), ComponentConfig{}
        .with_size({pixels(1280 * s), pixels(720 * s)})
        .with_absolute_position(left, top).with_transparent_bg().with_skip_grid_snap(true)
        .with_debug_name("st_root"));
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
    auto specimen = [&](int id, float x, float y, float w, float h, const char *font, float size, const char *debug) {
      div(context, mk(root.ent(), 500 + id), box(x - 12, y - 8, w + 24, h + 16).with_custom_background(box_color));
      return box(x, y, w, h).with_font(font, pixels(size * s))
          .with_alignment(TextAlignment::Left).with_custom_text_color(white).with_letter_spacing(s - 1.f).with_debug_name(debug);
    };
    text(0, "Styled text", 48, 18, 720, 46, 38, white, "st_title", true);
    text(1, "Native span weights, wrapping and explicit line breaks", 48, 65, 900, 28, 22, muted);
    if (button(context, mk(root.ent(), 2), box(1012, 24, 216, 38)
        .with_label(guides ? "Run widths: on" : "Run widths: off")
        .with_font("AtkinsonMock", pixels(19 * s)).with_custom_background(box_color)
        .with_custom_text_color(white).with_corner_radius(6 * s).with_debug_name("st_guides"))) guides = !guides;
    // --- The one that actually shows a weight change: DGOne has @bold.
    text(10, "01 / Weight + color", 48, 105, 560, 30, 24, white, "", true);
    div(context, mk(root.ent(), 11), specimen(11, 48, 146, 560, 60, "DGOne", FS, "st_real_pair")
        .with_styled_label({
                TextSpan{"regular ", white, FontWeight::Regular},
                TextSpan{"BOLD", amber, FontWeight::Bold},
                TextSpan{" regular again", white, FontWeight::Regular},
            }));
    text(12, "DGOne + DGOne@bold / BOLD requests Bold", 48, 222, 560, 27, 18, muted);
    // --- Weight is independent of colour: same colour, different weights.
    text(20, "02 / Weight only", 660, 105, 560, 30, 24, white, "", true);
    div(context, mk(root.ent(), 21), specimen(21, 660, 146, 560, 60, "DGOne", FS, "st_same_colour")
        .with_styled_label({
                TextSpan{"one colour ", white, FontWeight::Regular},
                TextSpan{"two weights", white, FontWeight::Bold},
            }));
    text(22, "one colour = Regular / two weights = Bold", 660, 222, 560, 27, 18, muted);
    // --- Bold inside a wrapping paragraph. Both DGOne weights have the same
    // advance width; the wrapper still measures every run at its own weight.
    text(30, "03 / Wrapped spans", 48, 269, 360, 30, 24, white, "", true);
    div(context, mk(root.ent(), 31), specimen(31, 48, 316, 360, 140, "DGOne", 16, "st_wrap_bold")
        .with_styled_label({
                TextSpan{"the quick brown ", white, FontWeight::Regular},
                TextSpan{"fox jumps over", green, FontWeight::Bold},
                TextSpan{" the lazy dog and keeps running", white,
                         FontWeight::Regular},
            }).with_text_overflow(TextOverflow::Wrap));
    text(32, fmt::format("Wrap width {:.0f} screen px", 360 * s - 10), 48, 472, 360, 26, 18, muted, "st_wrap_width");
    // --- Weight survives a hard break, the markdown/diff shape.
    text(40, "04 / Explicit newline", 440, 269, 360, 30, 24, white, "", true);
    div(context, mk(root.ent(), 41), specimen(41, 440, 316, 360, 140, "DGOne", 16, "st_break_bold")
        .with_styled_label({
                TextSpan{"Heading\n", amber, FontWeight::Bold},
                TextSpan{"body text on the next line\n", white,
                         FontWeight::Regular},
                TextSpan{"Another heading\n", amber, FontWeight::Bold},
                TextSpan{"more body text", white, FontWeight::Regular},
            }).with_text_overflow(TextOverflow::Wrap));
    text(42, "Heading\\n body\\n Another heading\\n", 440, 472, 370, 26, 17, muted);
    text(50, "Span source and expected result", 840, 269, 388, 30, 23, white, "", true);
    text(51, "03  Bold: fox jumps over", 840, 311, 388, 26, 19, green);
    text(52, "Wraps continue in the same weight.", 840, 342, 388, 26, 18, muted);
    text(53, "04  Bold: both Heading spans", 840, 383, 388, 26, 19, amber);
    text(54, "Each \\n starts a new line; long lines wrap.", 840, 414, 388, 26, 17, muted);
    text(55, "Native line spacing is fixed.", 840, 455, 388, 26, 19, muted, "st_line_spacing");
    text(56, "No styled-label line-height option.", 840, 481, 388, 26, 17, muted);
    // --- The fallback: Gaegu-Bold has no @bold registered, so a Bold span
    // renders the same face rather than failing to find a font.
    text(60, "05 / Missing bold fallback", 48, 529, 560, 30, 24, white, "", true);
    div(context, mk(root.ent(), 61), specimen(61, 48, 573, 560, 60, "Gaegu-Bold", FS, "st_fallback")
        .with_styled_label({
                TextSpan{"asks for regular ", white, FontWeight::Regular},
                TextSpan{"asks for bold", amber, FontWeight::Bold},
            }));
    text(62, "Gaegu-Bold / no Gaegu-Bold@bold registered", 48, 648, 560, 26, 18, muted, "st_fallback_font");
    text(63, "Expected: both requests use the same face.", 48, 679, 560, 26, 18, white);
    text(70, "06 / Fallback, same color", 660, 529, 560, 30, 24, white, "", true);
    div(context, mk(root.ent(), 71), specimen(71, 660, 573, 560, 60, "Gaegu-Bold", FS, "st_fallback_same_color")
        .with_styled_label({TextSpan{"asks for regular ", white, FontWeight::Regular},
                            TextSpan{"asks for bold", white, FontWeight::Bold}}));
    text(72, "Identical face and color / requested Regular, Bold", 660, 648, 560, 26, 18, muted);
    text(73, "DGOne weights have equal glyph advances.", 660, 679, 560, 26, 18, white);
    if (!guides) return;
    auto *fonts = afterhours::EntityHelper::get_singleton_cmp<FontManager>();
    if (!fonts) return;
    const auto regular = fonts->get_font("DGOne");
    const auto bold = fonts->get_font("DGOne@bold");
    for (int i = 0; i < 2; ++i) {
      div(context, mk(root.ent(), 80 + i), box(i == 0 ? 48 : 660, 210, 560, 8)
          .with_ignore_pointer_events().with_on_draw_fg([s, i, regular, bold, white, amber](RectangleType r) {
            const std::array<const char *, 3> words = i == 0 ? std::array<const char *, 3>{"regular ", "BOLD", " regular again"}
                                                            : std::array<const char *, 3>{"one colour ", "two weights", ""};
            float x = r.x + 5;
            for (size_t n = 0; n < words.size(); ++n) {
              if (words[n][0] == '\0') continue;
              const float width = afterhours::measure_text(n == 1 ? bold : regular, words[n], FS * s, s).x;
              const auto color = n == 1 ? amber : white;
              afterhours::draw_rectangle({x, r.y + 3 * s, width, 1}, color);
              afterhours::draw_rectangle({x, r.y, 1, 7 * s}, color);
              afterhours::draw_rectangle({x + width, r.y, 1, 7 * s}, color);
              x += width;
            }
          }));
    }
  }
};

REGISTER_EXAMPLE_SCREEN(styled_text_lab, "System Demos",
                        "Per-span font weight in styled labels",
                        StyledTextLab)
