#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Prepared-text family: the wrap memo keys on face + size + spacing (the
// same styled text at two sizes wraps for its own size), styled runs take
// their offsets from the joined line's measurement (columns hold), a font
// reload drops cached measurements, and FontManager answers coverage --
// which codepoints a face cannot draw at all (hanabi #48). Per-label
// text_inset is the last row.
struct TextCacheLab : ScreenSystem<UIContext<InputAction>> {
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
        .with_corner_radius(0).with_padding(Padding::all(w1280(16))).with_debug_name("tcl_root"));
    auto text = [&](int id, const std::string &label, float x, float y, float w, float h,
                    float size, const char *font = "AtkinsonMock") {
      return div(context, mk(root.ent(), id), ComponentConfig{}
          .with_size({pixels(w * scale), pixels(h * scale)})
          .with_absolute_position(x * scale, y * scale).with_label(label)
          .with_font(font, pixels(size * scale)).with_custom_text_color(theme.font)
          .with_text_overflow(TextOverflow::Wrap).with_ignore_pointer_events());
    };
    text(0, "Text cache lab", 16, 0, 700, 44, 30);
    text(1, "Same styled text, two sizes, one memo: each box wraps for its own size. "
            "Styled runs are placed by the joined line's measurement, so the code row's columns hold.",
         16, 48, 1180, 28, 19);

    const std::vector<TextSpan> probe_text{
        {"alpha beta gamma delta epsilon", theme.font}};
    auto wrap_box = [&](int id, float x, float font_px) {
      div(context, mk(root.ent(), id), ComponentConfig{}
          .with_size({pixels(210 * scale), pixels(120 * scale)})
          .with_absolute_position(x * scale, 150 * scale)
          .with_background(Theme::Usage::Primary).with_corner_radius(6 * scale));
      div(context, mk(root.ent(), id + 1), ComponentConfig{}
          .with_size({pixels(194 * scale), pixels(104 * scale)})
          .with_absolute_position((x + 8) * scale, 158 * scale)
          .with_styled_label(probe_text)
          .with_font("AtkinsonMock", pixels(font_px * scale))
          .with_text_overflow(TextOverflow::Wrap)
          .with_text_inset(0).with_ignore_pointer_events());
    };
    text(2, "Wrap at 24px", 32, 116, 210, 30, 21);
    wrap_box(3, 32, 24);
    text(5, "Wrap at 14px", 290, 116, 210, 30, 21);
    wrap_box(6, 290, 14);

    text(8, "Styled columns (DGOne)", 32, 292, 500, 30, 21);
    text(9, "min(base * 2 ** attempt, 30000)", 32, 330, 700, 30, 21, "DGOne");
    div(context, mk(root.ent(), 10), ComponentConfig{}
        .with_size({pixels(700 * scale), pixels(30 * scale)})
        .with_absolute_position(32 * scale, 362 * scale)
        .with_styled_label({{"min", {120, 200, 255, 255}},
                            {"(base * ", theme.font},
                            {"2", {255, 180, 100, 255}},
                            {" ** attempt, ", theme.font},
                            {"30000", {255, 180, 100, 255}},
                            {")", theme.font}})
        .with_font("DGOne", pixels(21 * scale))
        .with_text_inset(0).with_ignore_pointer_events());
    text(11, "The coloured row is six runs; its characters land on the plain row's columns.",
         32, 398, 900, 28, 17);

    text(12, "Coverage: what AtkinsonMock cannot draw", 660, 116, 560, 30, 21);
    auto *fonts = afterhours::EntityHelper::get_singleton_cmp<FontManager>();
    struct Probe { uint32_t cp; const char *glyph; };
    const Probe probes[] = {{0x41, "A"}, {0xE9, "\xc3\xa9"}, {0x21B5, "\xe2\x86\xb5"},
                            {0x2605, "\xe2\x98\x85"}, {0x2191, "\xe2\x86\x91"},
                            {0x2713, "\xe2\x9c\x93"}};
    int pid = 13;
    float py = 150;
    for (const auto &p : probes) {
      char verdict[64];
      const bool covered = fonts && fonts->has_glyph("AtkinsonMock", p.cp);
      std::snprintf(verdict, sizeof(verdict), "U+%04X %s %s", p.cp, p.glyph,
                    covered ? "covered" : "missing");
      text(pid++, verdict, 660, py, 300, 28, 19);
      py += 32;
    }
    text(pid++, "A missing glyph draws nothing -- no box, no warning. The query is how an app checks before shipping the label.",
         660, 398, 560, 52, 17);

    text(30, "Per-label inset", 32, 440, 400, 30, 21);
    auto inset_box = [&](int id, float x, float inset, const char *label) {
      div(context, mk(root.ent(), id), ComponentConfig{}
          .with_size({pixels(210 * scale), pixels(44 * scale)})
          .with_absolute_position(x * scale, 478 * scale)
          .with_background(Theme::Usage::Primary).with_corner_radius(6 * scale));
      div(context, mk(root.ent(), id + 1), ComponentConfig{}
          .with_size({pixels(210 * scale), pixels(44 * scale)})
          .with_absolute_position(x * scale, 478 * scale)
          .with_label(label)
          .with_font("AtkinsonMock", pixels(19 * scale))
          .with_custom_text_color(theme.font)
          .with_text_inset(inset * scale, 0).with_ignore_pointer_events());
    };
    inset_box(31, 32, 0, "inset 0");
    inset_box(33, 290, 24, "inset 24");
  }
};

REGISTER_EXAMPLE_SCREEN(text_cache_lab, "System Demos",
                        "text caches: wrap memo key, styled-run columns, glyph coverage, label inset",
                        TextCacheLab)
