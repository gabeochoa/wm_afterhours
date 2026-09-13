#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <array>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Minimal fixed-size layout for agent E2E assert_ui checks.
struct LayoutPatterns : ScreenSystem<UIContext<InputAction>> {
  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();
    context.scaling_mode = ScalingMode::Adaptive;

    auto root =
        vstack(context, mk(entity, 0),
               ComponentConfig{}
                   .with_size(ComponentSize{screen_pct(1.0f), screen_pct(1.0f)})
                   .with_custom_background(afterhours::Color{30, 34, 44, 255})
                   .with_corner_radius(0)
                   .with_padding(Padding{.top = pixels(40),
                                         .left = pixels(40),
                                         .bottom = pixels(40),
                                         .right = pixels(40)})
                   .with_debug_name("pattern_root"));

    // 416 inner width fits three 120px boxes + 12px gaps + 16px padding/side.
    auto row = hstack(context, mk(root.ent(), 1),
                      ComponentConfig{}
                          .with_size(ComponentSize{pixels(416), pixels(112)})
                          .with_skip_grid_snap()
                          .with_custom_background(afterhours::Color{50, 60, 80, 255})
                          .with_padding(Padding::all(pixels(16)))
                          .with_gap(pixels(12))
                          .with_align_items(AlignItems::Center)
                          .with_debug_name("pattern_row"));

    const afterhours::Color box_colors[] = {
        {0, 180, 220, 255}, {220, 60, 120, 255}, {50, 210, 130, 255}};

    for (int i = 0; i < 3; ++i) {
      div(context, mk(row.ent(), i),
          ComponentConfig{}
              .with_label(std::to_string(i + 1))
              .with_size(ComponentSize{pixels(120), pixels(80)})
              .with_skip_grid_snap()
              .with_custom_background(box_colors[i])
              .with_auto_text_color(true)
              .with_font("AtkinsonMock", pixels(24.0f))
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.08f)
              .with_debug_name(fmt::format("pattern_box_{}", i)));
    }

    auto annotations = div(context, mk(entity, 1), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_absolute_position(0, 0).with_background(Theme::Usage::None).with_corner_radius(0)
        .with_debug_name("pattern_annotations"));
    const afterhours::Color ink{235, 242, 250, 255};
    const afterhours::Color muted{177, 194, 217, 255};
    const auto box = [](float x, float y, float width, float height) {
      return ComponentConfig{}.with_size({pixels(width), pixels(height)})
          .with_absolute_position(x, y).with_background(Theme::Usage::None).with_corner_radius(0)
          .with_skip_grid_snap().with_ignore_pointer_events();
    };
    const auto label = [&](int id, const std::string &text, float x, float y, float width, float height,
                           float size, afterhours::Color color, const std::string &name = "") {
      div(context, mk(annotations.ent(), id), box(x, y, width, height).with_label(text)
          .with_font("AtkinsonMock", pixels(size)).with_custom_text_color(color)
          .with_alignment(TextAlignment::Left).with_text_overflow(TextOverflow::Wrap).with_debug_name(name));
    };
    const float notes_width = std::min(context.screen_width - 544, 640.f);
    label(0, "Fixed horizontal layout", 504, 36, notes_width, 42, 32, ink, "pattern_title");
    label(1, "Three equal children share one row. The geometry stays fixed when the window changes.",
          504, 91, notes_width, 82, 23, muted);
    label(2, "Parent: 416 x 112px", 40, 0, 416, 32, 22, ink, "pattern_parent_size");
    label(3, "Fixed origin: x 40 / y 40", 40, 161, 416, 30, 22, ink, "pattern_origin");
    label(4, "Child: 120 x 80px", 40, 225, 250, 30, 22, ink);
    label(5, "Gap: 12px", 40, 266, 250, 30, 22, ink);
    label(6, "Padding: 16px on every side", 40, 307, 416, 30, 22, ink);
    label(7, "Three equal children fit without overflow.", 40, 368, 416, 68, 26, ink, "pattern_expected");
    label(8, "3 x 120 + 2 x 12 + 2 x 16 = 416px", 40, 449, 416, 30, 21, muted);
    label(9, "80 + 2 x 16 = 112px", 40, 487, 416, 30, 21, muted);
    label(10, "Colors identify child order", 504, 199, notes_width, 32, 25, ink);
    const std::array<const char *, 3> color_names{"1 / Cyan", "2 / Pink", "3 / Green"};
    for (int i = 0; i < 3; ++i) {
      const float x = 504 + static_cast<float>(i) * 152;
      div(context, mk(annotations.ent(), 20 + i), box(x, 247, 128, 82)
          .with_custom_background(box_colors[i]).with_corner_radius(8)
          .with_label(std::to_string(i + 1)).with_font("AtkinsonMock", pixels(42))
          .with_custom_text_color({12, 23, 32, 255}).with_alignment(TextAlignment::Center)
          .with_debug_name("pattern_legend_" + std::to_string(i)));
      label(30 + i, color_names[static_cast<size_t>(i)], x, 341, 132, 29, 22, ink);
    }
    label(40, "Numbers and colors are identifiers. They do not change the layout or represent status.",
          504, 398, notes_width, 88, 22, muted);
    label(41, "Measured fixture above; annotations remain outside its bounds.",
          504, 500, notes_width, 66, 21, muted);
    div(context, mk(annotations.ent(), 50), box(40, 193, 416, 20)
        .with_on_draw_fg([muted](RectangleType r) {
          const auto bracket = [&](float x, float width) {
            afterhours::draw_line_ex({x, r.y + 10}, {x + width, r.y + 10}, 1, muted);
            afterhours::draw_line_ex({x, r.y + 4}, {x, r.y + 16}, 1, muted);
            afterhours::draw_line_ex({x + width, r.y + 4}, {x + width, r.y + 16}, 1, muted);
          };
          bracket(r.x, 16);
          bracket(r.x + 16, 120);
          bracket(r.x + 136, 12);
          bracket(r.x + 148, 120);
          bracket(r.x + 268, 12);
          bracket(r.x + 280, 120);
          bracket(r.x + 400, 16);
        }));
  }
};

REGISTER_EXAMPLE_SCREEN(layout_patterns, "Tools",
                        "Minimal layout patterns for agent E2E",
                        LayoutPatterns)
