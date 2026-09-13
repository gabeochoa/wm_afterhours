#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <array>
#include <cmath>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct MetersGaugesShowcase : ScreenSystem<UIContext<InputAction>> {
  float control_value = 0.65f;
  float anim_progress = 0.f;
  bool anim_forward = true;
  bool anim_paused = false;

  void advance(float dt) {
    if (anim_paused) return;
    // Animate progress (bounces 0 -> 1 -> 0 over ~3s each way)
    if (anim_forward) {
      anim_progress += dt / 3.0f;
      if (anim_progress >= 1.0f) {
        anim_progress = 1.0f;
        anim_forward = false;
      }
      return;
    }
    anim_progress -= dt / 3.0f;
    if (anim_progress <= 0.0f) {
      anim_progress = 0.0f;
      anim_forward = true;
    }
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float dt) override {
    advance(dt);
    context.theme = afterhours::ui::theme_presets::ocean_navy();
    context.scaling_mode = ScalingMode::Proportional;
    const float scale = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const float left = (context.screen_width / scale - 1144) / 2;
    const float top = (context.screen_height / scale - 720) / 2;

    // Theme-complementary colors (softer, work with ocean_navy palette)
    // Instead of harsh saturated RGB, use muted tones that harmonize
    const afterhours::Color hp_color{190, 65, 65, 255};      // muted red
    const afterhours::Color mp_color{65, 110, 190, 255};     // muted blue
    const afterhours::Color xp_color{190, 155, 50, 255};     // muted gold
    const afterhours::Color health_color{60, 170, 80, 255};  // muted green
    const afterhours::Color shield_color{70, 130, 200, 230}; // muted blue
    const afterhours::Color track_color{86, 103, 127, 255};   // visible track
    const afterhours::Color ink{236, 243, 252, 255};
    const afterhours::Color muted{180, 196, 220, 255};
    const afterhours::Color panel{25, 39, 59, 255};
    const auto box = [scale, left, top](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * scale), pixels(h * scale)})
          .with_absolute_position((left + x) * scale, (top + y) * scale)
          .with_background(Theme::Usage::None).with_corner_radius(0);
    };
    // Main container
    auto root = div(context, mk(entity), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background({14, 25, 41, 255}).with_corner_radius(0).with_debug_name("mg_root"));
    const auto label = [&](int id, const std::string &text, float x, float y, float w, float h,
                           float size, afterhours::Color color, const std::string &name = "",
                           TextAlignment alignment = TextAlignment::Left) {
      return div(context, mk(root.ent(), id), box(x, y, w, h).with_label(text)
          .with_font("AtkinsonMock", pixels(size * scale)).with_custom_text_color(color)
          .with_alignment(alignment).with_ignore_pointer_events().with_debug_name(name));
    };
    const auto percent_text = [](float value) {
      return fmt::format("{:.2f}%", value * 100);
    };
    const auto panel_at = [&](int id, float x, float y, float w, float h, const std::string &name) {
      div(context, mk(root.ent(), id), box(x, y, w, h).with_custom_background(panel)
          .with_corner_radius(10 * scale).with_debug_name(name));
    };
    const auto meter = [&](int id, float value, afterhours::Color color, float x, float y,
                           float width, const std::string &name) {
      auto result = progress_bar(context, mk(root.ent(), id), value,
          box(x, y, width, 18).with_corner_radius(4 * scale).with_debug_name(name), ProgressBarLabelStyle::None);
      for (const auto track_id : result.cmp().children) {
        auto &track = UICollectionHolder::getEntityForIDEnforce(track_id);
        track.get<afterhours::HasColor>().set(value > 0.999f ? color : track_color);
        for (const auto fill_id : track.get<UIComponent>().children) {
          auto &fill = UICollectionHolder::getEntityForIDEnforce(fill_id);
          if (!fill.has<afterhours::HasColor>()) continue;
          fill.get<afterhours::HasColor>().set(color);
        }
      }
    };

    // Title
    panel_at(0, 0, 24, 1144, 84, "mg_header");
    label(1, "Meters & Gauges", 20, 31, 1104, 43, 34, ink, "mg_title");
    label(2, "Control-linked meters, an animated pair, and fixed circular samples.", 20, 77, 1104, 26, 21, muted);
    panel_at(3, 0, 126, 702, 202, "mg_row_meters");
    label(4, "Inline Meters", 20, 135, 662, 32, 25, ink);
    const std::array<const char *, 3> names{"HP / Health", "MP / Mana", "XP / Experience"};
    const std::array<const char *, 3> ids{"mg_hp", "mg_mp", "mg_xp"};
    const std::array<float, 3> values{control_value, control_value * .6f, std::min(control_value * 1.3f, 1.f)};
    const std::array<afterhours::Color, 3> colors{hp_color, mp_color, xp_color};
    for (size_t i = 0; i < names.size(); ++i) {
      const float y = 174 + static_cast<float>(i) * 41;
      label(10 + static_cast<int>(i), names[i], 20, y, 178, 28, 21, ink, std::string(ids[i]) + "_label");
      meter(20 + static_cast<int>(i), values[i], colors[i], 204, y + 6, 360, std::string(ids[i]) + "_bar");
      label(30 + static_cast<int>(i), percent_text(values[i]), 586, y, 96, 28, 24, ink,
            std::string(ids[i]) + "_value", TextAlignment::Right);
    }
    label(40, "HP = input / MP = 0.60x / XP = 1.30x, capped at 100%", 20, 298, 662, 24, 19, muted);
    panel_at(41, 0, 342, 702, 180, "mg_row_stacked");
    label(42, "Paired bars / Health + Shield", 20, 352, 662, 32, 25, ink);
    label(43, "Health", 20, 395, 178, 28, 22, ink);
    meter(44, control_value, health_color, 204, 401, 360, "mg_health_bar");
    label(45, percent_text(control_value), 586, 395, 96, 28, 24, ink, "mg_health_value", TextAlignment::Right);
    label(46, "Shield", 20, 438, 178, 28, 22, ink);
    meter(47, control_value * .45f, shield_color, 204, 444, 360, "mg_shield_bar");
    label(48, percent_text(control_value * .45f), 586, 438, 96, 28, 24, ink, "mg_shield_value", TextAlignment::Right);
    label(49, "Independent meters, not a shared total. Shield = 0.45x input.", 20, 484, 662, 27, 19, muted);

    // =================================================================
    // Row 3: Animated Progress
    // =================================================================
    panel_at(50, 726, 126, 418, 252, "mg_row_animated");
    label(51, "Animated Progress", 746, 135, 378, 32, 25, ink);
    meter(52, anim_progress, {74, 150, 220, 255}, 746, 195, 258, "mg_anim_bar");
    label(53, percent_text(anim_progress), 1022, 183, 102, 40, 25, ink, "mg_anim_value", TextAlignment::Right);
    circular_progress(context, mk(root.ent(), 54), anim_progress,
        box(746, 245, 96, 96).with_custom_background({74, 150, 220, 255})
            .with_border(track_color, pixels(9 * scale)).with_debug_name("mg_anim_circle"));
    label(55, "Circular companion", 860, 242, 264, 28, 21, ink);
    label(56, "0-100-0% / 6s loop", 860, 275, 264, 25, 20, muted);
    label(57, "3 seconds each way", 860, 305, 264, 25, 20, muted);
    if (button(context, mk(root.ent(), 58), box(860, 337, 126, 32)
        .with_label(anim_paused ? "Resume" : "Pause").with_font("AtkinsonMock", pixels(20 * scale))
        .with_custom_background({47, 72, 103, 255}).with_custom_text_color(ink)
        .with_corner_radius(6 * scale).with_debug_name("mg_pause"))) anim_paused = !anim_paused;
    if (button(context, mk(root.ent(), 59), box(998, 337, 126, 32)
        .with_label("Restart").with_font("AtkinsonMock", pixels(20 * scale))
        .with_custom_background({47, 72, 103, 255}).with_custom_text_color(ink)
        .with_corner_radius(6 * scale).with_debug_name("mg_restart"))) {
      anim_progress = 0;
      anim_forward = true;
    }

    // =================================================================
    // Row 4: Circular Gauges (larger, centered, with visible labels)
    // =================================================================
    panel_at(60, 726, 392, 418, 300, "mg_row_circular");
    label(61, "Circular Gauges", 746, 402, 378, 32, 25, ink);
    label(62, "Fixed samples / slider-independent", 746, 441, 378, 26, 19, muted);
    const std::array<float, 3> gauge_values{.25f, .5f, .9f};
    const std::array<afterhours::Color, 3> gauge_colors{hp_color, xp_color, health_color};
    const std::array<const char *, 3> gauge_ids{"mg_gauge_25", "mg_gauge_50", "mg_gauge_90"};
    const std::array<const char *, 3> gauge_names{"Low", "Half", "High"};
    for (size_t i = 0; i < gauge_values.size(); ++i) {
      const float x = 750 + static_cast<float>(i) * 132;
      circular_progress(context, mk(root.ent(), 70 + static_cast<int>(i)), gauge_values[i],
          box(x, 483, 104, 104).with_custom_background(gauge_colors[i])
              .with_border(track_color, pixels(9 * scale)).with_debug_name(std::string(gauge_ids[i]) + "_ring"));
      label(80 + static_cast<int>(i), fmt::format("{}%", std::lround(gauge_values[i] * 100)),
            x + 10, 518, 84, 34, 25, ink, std::string(gauge_ids[i]) + "_label", TextAlignment::Center);
      label(90 + static_cast<int>(i), gauge_names[i], x, 594, 104, 30, 22, ink, "", TextAlignment::Center);
    }
    label(100, "Red: low / Gold: half / Green: high", 746, 638, 378, 25, 18, muted);
    label(101, "Color names these fixtures; no live thresholds.", 746, 666, 378, 21, 17, muted);

    // =================================================================
    // Row 5: Interactive Slider
    // =================================================================
    panel_at(110, 0, 536, 702, 156, "mg_row_slider");
    label(111, "Shared control", 20, 546, 480, 32, 25, ink);
    label(112, "Adjusts inline and paired meters / drag or Left-Right", 20, 582, 662, 25, 19, muted);
    auto input = slider(context, mk(root.ent(), 113), control_value,
        box(30, 612, 642, 32).with_custom_background({0, 0, 0, 0})
            .with_debug_name("mg_slider"), SliderHandleValueLabelPosition::None);
    for (const auto child_id : input.cmp().children) {
      auto &child = UICollectionHolder::getEntityForIDEnforce(child_id);
      if (!child.has<HasSliderState>()) continue;
      child.addComponentIfMissing<UIComponentDebug>("mg_slider_input").set("mg_slider_input");
      child.addComponentIfMissing<HasOnDraw>().fg = [scale, value = control_value, track_color, ink](RectangleType r) {
        const float y = r.y + r.height / 2;
        const float x = r.x + value * r.width;
        afterhours::draw_rectangle_rounded({r.x, y - 3 * scale, r.width, 6 * scale}, 1, 8, track_color, RoundedCorners());
        if (value > 0)
          afterhours::draw_rectangle_rounded({r.x, y - 3 * scale, r.width * value, 6 * scale}, 1, 8,
              afterhours::Color{91, 178, 239, 255}, RoundedCorners());
        afterhours::draw_circle(static_cast<int>(x), static_cast<int>(y), 12 * scale, {9, 22, 39, 255});
        afterhours::draw_circle(static_cast<int>(x), static_cast<int>(y), 9 * scale, ink);
        afterhours::draw_circle(static_cast<int>(x), static_cast<int>(y), 3 * scale, {28, 95, 148, 255});
      };
    }
    label(114, percent_text(control_value), 552, 544, 130, 36, 28, ink, "mg_control_value", TextAlignment::Right);
    label(115, "0%", 20, 653, 98, 26, 20, muted);
    label(116, "100%", 578, 653, 104, 26, 20, muted, "", TextAlignment::Right);
    label(120, "Native progress bars + circular progress + slider / animation and fixed gauges stay independent", 0, 696, 1144, 23, 18, muted);
  }
};

REGISTER_EXAMPLE_SCREEN(meters_gauges, "Component Galleries",
                        "Meters & gauges: inline meters, paired bars, "
                        "circular progress, animated values",
                        MetersGaugesShowcase)
