#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../rolling_number.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"

struct RollingNumberLab : ScreenSystem<afterhours::ui::UIContext<InputAction>> {
  rolling_number::Number number;
  bool reduced_motion = false;
  bool blur = true;
  bool paused = false;

  RollingNumberLab() { rolling_number::retarget(number, 99, true); }

  void for_each_with(afterhours::Entity &entity,
                     afterhours::ui::UIContext<InputAction> &context,
                     float dt) override {
    using namespace afterhours::ui;
    using namespace afterhours::ui::imm;
    context.theme = theme_presets::ocean_navy();
    context.scaling_mode = ScalingMode::Proportional;
    if (!paused) rolling_number::advance(number, dt, reduced_motion);
    const float s = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const float left = (context.screen_width - 1144.f * s) / 2.f;
    const float top = (context.screen_height - 720.f * s) / 2.f;
    const afterhours::Color ink{23, 36, 53, 255};
    const afterhours::Color teal{0, 101, 94, 255};
    const auto box = [=](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(left + x * s, top + y * s)
          .with_corner_radius(0).with_background(Theme::Usage::None);
    };
    auto root = div(context, mk(entity), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background({240, 244, 247, 255}).with_corner_radius(0)
        .with_debug_name("rolling_root"));
    int id = 0;
    const auto label = [&](const std::string &text, float x, float y, float w,
                           float h, float size, const std::string &name, bool bold = false) {
      return div(context, mk(root.ent(), id++), box(x, y, w, h).with_label(text)
          .with_font(bold ? "AtkinsonMockBold" : "AtkinsonMock", pixels(size * s))
          .with_custom_text_color(ink).with_alignment(TextAlignment::Left)
          .with_ignore_pointer_events().with_debug_name(name));
    };
    const auto control = [&](const std::string &text, float x, float y, float w,
                             const std::string &name, bool primary = false) {
      return button(context, mk(root.ent(), id++), box(x, y, w, 48).with_label(text)
          .with_font("AtkinsonMock", pixels(21 * s)).with_corner_radius(8 * s)
          .with_custom_background(primary ? teal : afterhours::Color{222, 231, 237, 255})
          .with_custom_text_color(primary ? afterhours::Color{255, 255, 255, 255} : ink)
          .with_debug_name(name));
    };
    label("Numbers in motion", 0, 28, 1144, 48, 36, "rolling_title", true);
    label("A new total, with a softer landing.", 0, 86, 1144, 30, 22, "rolling_subtitle");
    div(context, mk(root.ent(), id++), box(0, 144, 1144, 270)
        .with_custom_background({221, 233, 230, 255}).with_corner_radius(12 * s)
        .with_ignore_pointer_events());
    label("Total collected", 32, 162, 1080, 32, 22, "rolling_heading");
    if (control("+1", 0, 444, 112, "rolling_add", true))
      rolling_number::retarget(number, number.target + 1, reduced_motion);
    if (control("-1", 128, 444, 112, "rolling_subtract"))
      rolling_number::retarget(number, number.target - 1, reduced_motion);
    if (control("+25", 256, 444, 112, "rolling_jump"))
      rolling_number::retarget(number, number.target + 25, reduced_motion);
    if (control("Set 99", 384, 444, 160, "rolling_99"))
      rolling_number::retarget(number, 99, reduced_motion);
    if (control("Set 999", 560, 444, 160, "rolling_999"))
      rolling_number::retarget(number, 999, reduced_motion);
    if (control("Reset", 736, 444, 160, "rolling_reset"))
      rolling_number::retarget(number, 0, reduced_motion);
    if (control(paused ? "Resume" : "Pause", 912, 444, 232, "rolling_pause")) paused = !paused;
    if (control(reduced_motion ? "Reduced motion: on" : "Reduced motion: off",
                0, 524, 320, "rolling_reduced")) {
      reduced_motion = !reduced_motion;
      if (reduced_motion) rolling_number::settle(number);
    }
    if (control(blur ? "Motion blur: on" : "Motion blur: off", 336, 524, 284, "rolling_blur")) blur = !blur;
    label(fmt::format("Target: {:06}", number.target), 648, 532, 270, 32, 22, "rolling_target");
    label(paused ? "Paused" : number.elapsed < rolling_number::duration ? "Rolling" : "At rest",
          944, 532, 200, 32, 22, "rolling_phase");
    label("Try 99 + 1 or 100 - 1. Change the target while the digits are moving.",
          0, 610, 1144, 30, 21, "rolling_help");
    label("Reduced motion changes the total immediately. Pause holds the current position.",
          0, 654, 1144, 28, 19, "rolling_help_motion");
    auto *fonts = afterhours::EntityHelper::get_singleton_cmp<FontManager>();
    if (!fonts) return;
    const auto font = fonts->get_font("AtkinsonMockBold");
    const auto positions = number.position;
    const float remaining = std::max(0.f, 1.f - number.elapsed / rolling_number::duration);
    std::array<float, 6> smears{};
    for (std::size_t i = 0; i < smears.size(); ++i)
      smears[i] = blur && !reduced_motion
          ? std::min(7.f, static_cast<float>(std::abs(number.to[i] - number.from[i])) * remaining * remaining * 3.f) : 0.f;
    div(context, mk(root.ent(), id++), box(32, 212, 1080, 154)
        .with_debug_name("rolling_digits").with_ignore_pointer_events()
        .with_on_draw_fg([positions, smears, font, ink, s](RectangleType r) {
          afterhours::begin_scissor_mode(static_cast<int>(r.x), static_cast<int>(r.y),
                                        static_cast<int>(r.width), static_cast<int>(r.height));
          const float cell = r.width / 6.f;
          const float size = 116.f * s;
          for (std::size_t i = 0; i < positions.size(); ++i) {
            const double base = std::floor(positions[i]);
            const float fraction = static_cast<float>(positions[i] - base);
            const bool softened = smears[i] > .1f;
            for (int row = -1; row <= 1; ++row) {
              const int digit = static_cast<int>(rolling_number::wrap(base + row));
              const auto text = std::to_string(digit);
              const auto measured = afterhours::measure_text(font, text.c_str(), size, 0.f);
              const float x = r.x + static_cast<float>(i) * cell + (cell - measured.x) / 2.f;
              const float y = r.y + (r.height - measured.y) / 2.f + (static_cast<float>(row) - fraction) * r.height;
              constexpr std::array<float, 5> weights{.06136f, .24477f, .38774f, .24477f, .06136f};
              for (int tap = softened ? -2 : 0; tap <= (softened ? 2 : 0); ++tap) {
                auto color = ink;
                color.a = softened ? static_cast<unsigned char>(255 * weights[static_cast<std::size_t>(tap + 2)]) : 255;
                afterhours::draw_text_ex(font, text.c_str(), {x, y + static_cast<float>(tap) * smears[i] * s}, size, 0.f, color);
              }
            }
          }
          afterhours::end_scissor_mode();
        }));
  }
};

REGISTER_EXAMPLE_SCREEN(rolling_number, "System Demos",
                        "Rolling digits with motion blur and reduced motion", RollingNumberLab)
