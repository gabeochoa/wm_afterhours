#pragma once

#include "../../external.h"
#include "../../frame_stats.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/ui/curved_text.h>
#include <chrono>
#include <cmath>
#include <string>
#include <unordered_map>
#include <vector>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct CurvedTextStressLab : ScreenSystem<UIContext<InputAction>> {
  static constexpr std::array<const char *, 2> TEXTS{"弧に沿った曲線テキスト",
                                                     "아크 위의 곡선 텍스트"};
  static constexpr std::array<const char *, 2> FONT_NAMES{"Sazanami", "NotoSansKR"};
  static constexpr std::array<float, 2> FONT_SCALES{0.92f, 0.95f};

  FrameStats stats;
  float stringsSlider = 0.35f;
  bool spin = true;
  bool cacheWidths = true;
  bool cacheLayout = false;
  bool paused = false;
  float spinDeg = 0.f;
  double measureMs = 0.0;
  double layoutMs = 0.0;
  double drawMs = 0.0;
  size_t glyphsLastFrame = 0;
  std::unordered_map<std::string, float> widthCache;
  std::unordered_map<int, std::vector<CurvedGlyph>> layoutCache;

  int strings() const { return 1 + static_cast<int>(std::lround(stringsSlider * 23.f)); }

  void reset() {
    stringsSlider = 0.35f;
    spin = true;
    cacheWidths = true;
    cacheLayout = false;
    paused = false;
    spinDeg = 0.f;
    widthCache.clear();
    layoutCache.clear();
  }

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context,
                     float dt) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();
    context.scaling_mode = ScalingMode::Proportional;
    if (!paused) {
      stats.push(dt);
      if (spin)
        spinDeg = std::fmod(spinDeg + dt * 18.f, 360.f);
    }
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
                        .with_debug_name("curved_stress_root"));
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
                    box(x, y, w, 40).with_label(text)
                        .with_font("AtkinsonMock", pixels(16 * s)).with_corner_radius(8 * s)
                        .with_custom_background(primary ? afterhours::Color{0, 105, 98, 255}
                                                        : afterhours::Color{44, 54, 74, 255})
                        .with_custom_text_color(ink).with_debug_name(debug));
    };

    label("Curved text stress: where do the milliseconds go?", 48, 28, 820, 26, false,
          "curved_stress_title");
    label(fmt::format("frame {:.1f} fps  avg {:.2f} ms  p95 {:.2f} ms  max {:.2f} ms", stats.fps,
                      stats.avgMs, stats.p95Ms, stats.maxMs),
          48, 66, 820, 16, true, "curved_stress_stats");
    label(fmt::format("stage ms (last frame): measure {:.3f}  layout {:.3f}  draw {:.3f}   "
                      "glyphs {}  strings {}",
                      measureMs, layoutMs, drawMs, glyphsLastFrame, strings()),
          48, 92, 1000, 16, false, "curved_stress_stages");
    label("Curved strings", 950, 36, 160, 14, true);
    slider(context, mk(root.ent(), id++), stringsSlider,
           box(950, 58, 220, 28).with_debug_name("curved_stress_strings"));
    if (control(spin ? "Spin: on" : "Spin: off", 950, 96, 104, "curved_stress_spin"))
      spin = !spin;
    if (control(cacheWidths ? "Width cache: on" : "Width cache: off", 1062, 96, 150,
                "curved_stress_widths"))
      cacheWidths = !cacheWidths;
    if (control(cacheLayout ? "Layout cache: on" : "Layout cache: off", 950, 144, 150,
                "curved_stress_layout"))
      cacheLayout = !cacheLayout;
    if (control(paused ? "Resume" : "Pause", 1108, 144, 104, "curved_stress_pause"))
      paused = !paused;
    if (control("Reset", 950, 192, 104, "curved_stress_reset", true))
      reset();

    auto *fonts = afterhours::EntityHelper::get_singleton_cmp<FontManager>();
    if (!fonts)
      return;
    const int count = strings();
    const float cellW = 296.f;
    const float cellH = 470.f / static_cast<float>((count + 2) / 3);
    div(context, mk(root.ent(), id++),
        box(48, 150, 880, 470).with_background(Theme::Usage::None).with_ignore_pointer_events()
            .with_debug_name("curved_stress_stage")
            .with_on_draw_fg([this, fonts, count, s, ink, accent, cellW,
                              cellH](RectangleType rect) {
              using Clock = std::chrono::steady_clock;
              double measure = 0.0, layoutRaw = 0.0, draw = 0.0;
              size_t glyphs = 0;
              const float fontSize = std::min(19.f, cellH * 0.16f) * s;
              for (int i = 0; i < count; ++i) {
                const size_t lang = static_cast<size_t>(i) % 2;
                const std::string text = TEXTS[lang];
                const auto font = fonts->get_font(FONT_NAMES[lang]);
                const float size = fontSize * FONT_SCALES[lang];
                const float cx = rect.x + (static_cast<float>(i % 3) + 0.5f) * cellW * s;
                const float cy = rect.y + (static_cast<float>(i / 3) + 0.5f) * cellH * s;
                const float radius = std::min(52.f, cellH * 0.40f) * s;
                std::vector<CurvedGlyph> placed;
                const auto cached = layoutCache.find(i);
                if (cacheLayout && !spin && cached != layoutCache.end()) {
                  placed = cached->second;
                } else {
                  const auto layoutStart = Clock::now();
                  placed = layout_text_on_arc(
                      text, cx, cy,
                      ArcText{.radius = radius, .start_angle_deg = -90.f + spinDeg, .clockwise = true},
                      [this, font, size, &measure](std::string_view sv) {
                        const auto mStart = Clock::now();
                        float w = 0.f;
                        const std::string glyph(sv);
                        auto it = widthCache.find(glyph);
                        if (cacheWidths && it != widthCache.end()) {
                          w = it->second;
                        } else {
                          w = afterhours::measure_text(font, glyph.c_str(), size, 1.f).x;
                          if (cacheWidths)
                            widthCache[glyph] = w;
                        }
                        measure += std::chrono::duration<double, std::milli>(Clock::now() - mStart).count();
                        return w;
                      });
                  layoutRaw += std::chrono::duration<double, std::milli>(Clock::now() - layoutStart).count();
                  if (cacheLayout && !spin)
                    layoutCache[i] = placed;
                }
                const auto drawStart = Clock::now();
                for (const CurvedGlyph &g : placed) {
                  const std::string ch = text.substr(g.span.begin, g.span.size());
                  if (ch == " " || ch == "," || ch == "、")
                    continue;
                  const auto gs = afterhours::measure_text(font, ch.c_str(), size, 1.f);
                  afterhours::draw_text_ex(font, ch.c_str(),
                                           {g.center_x - gs.x / 2.f, g.center_y - gs.y / 2.f},
                                           size, 1.f, (i % 2 ? accent : ink), g.rotation_deg,
                                           g.center_x, g.center_y);
                  ++glyphs;
                }
                draw += std::chrono::duration<double, std::milli>(Clock::now() - drawStart).count();
              }
              measureMs = measureMs * 0.9 + measure * 0.1;
              layoutMs = layoutMs * 0.9 + std::max(0.0, layoutRaw - measure) * 0.1;
              drawMs = drawMs * 0.9 + draw * 0.1;
              glyphsLastFrame = glyphs;
            }));

    label("Spin forces layout every frame. Width cache removes per-glyph font measuring; layout "
          "cache (spin off) reuses placements and leaves draw as the only stage. Draw includes a "
          "per-glyph measure for centring, so it is the floor.",
          48, 640, 1180, 15, true);
    label(paused ? "paused" : "running", 48, 668, 200, 15, true, "curved_stress_state");
  }
};

REGISTER_EXAMPLE_SCREEN(curved_text_stress_lab, "Animations",
                        "Stress: curved text stage timings", CurvedTextStressLab)
