#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/ui/curved_text.h>
#include <array>
#include <cmath>
#include <string>
#include <vector>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct CurvedTextLab : ScreenSystem<UIContext<InputAction>> {
  struct Language {
    const char *name;
    const char *headline;
    const char *font_name;
    float font_scale;
  };
  static constexpr std::array<Language, 2> LANGUAGES{{
      {"Japanese", "弧に沿った曲線テキスト", "Sazanami", 0.92f},
      {"Korean", "아크 위의 곡선 텍스트", "NotoSansKR", 0.95f},
  }};
  static constexpr float ARC_SIZE_MIN = 110.f;
  static constexpr float ARC_SIZE_MAX = 220.f;

  float arcSlider = 0.727f;
  size_t language = 0;
  float spin_deg = 0.f;
  bool clockwise = true;
  bool spin = false;
  float flat_rotation = -8.f;

  float radius() const { return ARC_SIZE_MIN + arcSlider * (ARC_SIZE_MAX - ARC_SIZE_MIN); }

  void reset() {
    arcSlider = 0.727f;
    language = 0;
    spin_deg = 0.f;
    clockwise = true;
    spin = false;
    flat_rotation = -8.f;
  }

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context,
                     float dt) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();
    context.scaling_mode = ScalingMode::Proportional;
    if (spin)
      spin_deg = std::fmod(spin_deg + dt * 12.f, 360.f);
    const float s = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const afterhours::Color ink{232, 238, 248, 255}, muted{160, 172, 196, 255},
        accent{64, 200, 190, 255};
    const auto box = [s](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, y * s).with_corner_radius(0);
    };
    auto root = div(context, mk(entity), ComponentConfig{}
                        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
                        .with_custom_background({16, 20, 30, 255}).with_corner_radius(0)
                        .with_debug_name("curved_root"));
    int id = 0;
    const auto label = [&](const std::string &text, float x, float y, float w, float size,
                           bool dim = false, const std::string &debug = "") {
      return div(context, mk(root.ent(), id++),
                 box(x, y, w, size * 1.4f).with_label(text)
                     .with_font("AtkinsonMock", pixels(size * s))
                     .with_custom_text_color(dim ? muted : ink)
                     .with_alignment(TextAlignment::Left).with_background(Theme::Usage::None)
                     .with_ignore_pointer_events().with_debug_name(debug));
    };
    const auto control = [&](const std::string &text, float x, float y, float w,
                             const std::string &debug, bool primary = false) {
      return button(context, mk(root.ent(), id++),
                    box(x, y, w, 44).with_label(text)
                        .with_font("AtkinsonMock", pixels(19 * s)).with_corner_radius(8 * s)
                        .with_custom_background(primary ? afterhours::Color{0, 105, 98, 255}
                                                        : afterhours::Color{44, 54, 74, 255})
                        .with_custom_text_color(ink).with_debug_name(debug));
    };

    label("Curved text: grapheme-aware arc layout", 60, 36, 900, 30, false, "curved_title");
    label("Native rotation turns a whole label; layout_text_on_arc places each grapheme on a "
          "circle with tangent rotation. Combining marks and flags stay one glyph.",
          60, 82, 1160, 16, true);

    auto *fonts = afterhours::EntityHelper::get_singleton_cmp<FontManager>();
    if (!fonts)
      return;
    const auto arc_font = fonts->get_font(LANGUAGES[language].font_name);
    const std::string headline = LANGUAGES[language].headline;
    const float headline_size = 34.f * LANGUAGES[language].font_scale * s;
    const bool cw = clockwise;
    const float r = radius() * s;
    float headline_width = 0.f;
    {
      std::vector<UnitSpan> hw;
      split_graphemes(headline, hw);
      for (const UnitSpan &sp : hw)
        headline_width += afterhours::measure_text(
                              arc_font, headline.substr(sp.begin, sp.size()).c_str(),
                              headline_size, 1.f)
                              .x;
    }
    const float span_deg = headline_width / r * RAD2DEG;
    const float start = -90.f - (cw ? span_deg : -span_deg) / 2.f + spin_deg;
    div(context, mk(root.ent(), id++),
        box(60, 150, 760, 410).with_background(Theme::Usage::None).with_ignore_pointer_events()
            .with_debug_name("curved_stage")
            .with_on_draw_fg([headline, arc_font, headline_size, r, start, cw, accent,
                              ink](RectangleType rect) {
              const float cx = rect.x + rect.width / 2.f, cy = rect.y + rect.height / 2.f;
              const auto glyphs = layout_text_on_arc(
                  headline, cx, cy, ArcText{.radius = r, .start_angle_deg = start, .clockwise = cw},
                  [arc_font, headline_size](std::string_view sv) {
                    return afterhours::measure_text(arc_font, std::string(sv).c_str(), headline_size, 1.f).x;
                  });
              for (const CurvedGlyph &g : glyphs) {
                const std::string ch = headline.substr(g.span.begin, g.span.size());
                if (ch == " ")
                  continue;
                const auto size = afterhours::measure_text(arc_font, ch.c_str(), headline_size, 1.f);
                afterhours::draw_text_ex(arc_font, ch.c_str(),
                             {g.center_x - size.x / 2.f, g.center_y - size.y / 2.f},
                             headline_size, 1.f, ink, g.rotation_deg, g.center_x, g.center_y);
              }
              (void)accent;
            }));

    const std::string graphemes = "Cafe\xcc\x81 \xf0\x9f\x87\xba\xf0\x9f\x87\xb8 ok";
    std::vector<UnitSpan> spans;
    split_graphemes(graphemes, spans);
    label(fmt::format("Grapheme demo: \"{}\" = {} glyphs, {} bytes (mock font draws missing "
                      "glyphs as ?)",
                      graphemes, spans.size(), graphemes.size()),
          60, 576, 900, 16, true, "curved_grapheme_count");
    const float grapheme_size = 24.f * s;
    const auto demo_font = fonts->get_font("AtkinsonMockBold");
    div(context, mk(root.ent(), id++),
        box(860, 492, 360, 150).with_background(Theme::Usage::None).with_ignore_pointer_events()
            .with_debug_name("curved_grapheme_stage")
            .with_on_draw_fg([graphemes, demo_font, grapheme_size, accent](RectangleType rect) {
              const float cx = rect.x + rect.width / 2.f, cy = rect.y + rect.height / 2.f;
              const auto glyphs = layout_text_on_arc(
                  graphemes, cx, cy,
                  ArcText{.radius = rect.height * 0.44f, .start_angle_deg = -160.f, .clockwise = true},
                  [demo_font, grapheme_size](std::string_view sv) {
                    return afterhours::measure_text(demo_font, std::string(sv).c_str(), grapheme_size, 1.f).x;
                  });
              for (const CurvedGlyph &g : glyphs) {
                const std::string ch = graphemes.substr(g.span.begin, g.span.size());
                if (ch == " ")
                  continue;
                const auto size = afterhours::measure_text(demo_font, ch.c_str(), grapheme_size, 1.f);
                afterhours::draw_text_ex(demo_font, ch.c_str(),
                             {g.center_x - size.x / 2.f, g.center_y - size.y / 2.f},
                             grapheme_size, 1.f, accent, g.rotation_deg, g.center_x, g.center_y);
              }
            }));

    auto flat = label("Straight label, native rotation", 880, 150, 360, 20, false, "curved_flat");
    flat.ent().addComponentIfMissing<HasUIModifiers>().rotation = flat_rotation;
    label(fmt::format("rotation {:.0f} deg via HasUIModifiers", flat_rotation), 880, 190, 360,
          15, true);

    {
      std::vector<std::string> language_names;
      for (const Language &l : LANGUAGES)
        language_names.emplace_back(l.name);
      dropdown(context, mk(root.ent(), id++), language_names, language,
               box(880, 240, 320, 44).with_font("AtkinsonMock", pixels(19 * s))
                   .with_corner_radius(8 * s)
                   .with_custom_background(afterhours::Color{44, 54, 74, 255})
                   .with_custom_text_color(ink).with_render_layer(10)
                   .with_debug_name("curved_language"));
    }
    label(fmt::format("Arc size: {:.0f}", radius()), 880, 300, 320, 16, true);
    slider(context, mk(root.ent(), id++), arcSlider,
           box(880, 328, 320, 32).with_debug_name("curved_arc_size"));
    if (control(clockwise ? "Direction: clockwise" : "Direction: counter", 880, 376, 240,
                "curved_direction"))
      clockwise = !clockwise;
    if (control(spin ? "Spin: on" : "Spin: off", 880, 432, 240, "curved_spin"))
      spin = !spin;
    if (control("Rotate straight label", 60, 624, 280, "curved_rotate"))
      flat_rotation = flat_rotation <= -24.f ? 16.f : flat_rotation - 8.f;
    if (control("Reset", 356, 624, 160, "curved_reset", true))
      reset();
    label(spin ? "spinning" : "at rest", 540, 634, 200, 16, true, "curved_state");
  }
};

REGISTER_EXAMPLE_SCREEN(curved_text_lab, "Animations",
                        "Grapheme-aware curved text and native rotation", CurvedTextLab)
