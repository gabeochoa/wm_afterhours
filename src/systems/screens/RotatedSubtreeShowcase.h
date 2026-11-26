#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

struct RotatedSubtreeShowcase
    : ScreenSystem<afterhours::ui::UIContext<InputAction>> {
  int clicks_a = 0;
  int clicks_b = 0;
  std::string last_prompt = "None";

  void for_each_with(afterhours::Entity &entity,
                     afterhours::ui::UIContext<InputAction> &ctx,
                     float) override {
    using namespace afterhours::ui;
    using namespace afterhours::ui::imm;

    const afterhours::Color bg{244, 246, 248, 255};
    const afterhours::Color card{255, 255, 255, 255};
    const afterhours::Color border{206, 214, 222, 255};
    const afterhours::Color ink{26, 36, 48, 255};
    const afterhours::Color muted{84, 98, 114, 255};
    const afterhours::Color teal{25, 112, 96, 255};
    const afterhours::Color navy{34, 78, 134, 255};

    ctx.theme = Theme{};
    ctx.theme.background = bg;
    ctx.theme.surface = card;
    ctx.theme.font = ink;
    ctx.theme.font_muted = muted;
    ctx.theme.accent = navy;
    ctx.theme.focus = navy;
    ctx.scaling_mode = ScalingMode::Proportional;

    const float s =
        std::min(ctx.screen_width / 1280.f, ctx.screen_height / 720.f);
    auto page = div(
        ctx, mk(entity),
        ComponentConfig{}
            .with_size({pixels(1280.f * s), pixels(720.f * s)})
            .with_absolute_position((ctx.screen_width - 1280.f * s) / 2.f,
                                    (ctx.screen_height - 720.f * s) / 2.f)
            .with_custom_background(bg)
            .with_corner_radius(0)
            .with_debug_name("rot_sub_page"));

    auto label = [&](int id, float x, float y, float w, float h, float pt,
                     const std::string &text, afterhours::Color color,
                     bool bold = false, const char *name = "") {
      div(ctx, mk(page.ent(), id),
          ComponentConfig{}
              .with_absolute_position(x * s, y * s)
              .with_size({pixels(w * s), pixels(h * s)})
              .with_label(text)
              .with_alignment(TextAlignment::Left)
              .with_font(bold ? "AtkinsonMockBold" : "AtkinsonMock",
                         pixels(pt * s))
              .with_custom_text_color(color)
              .with_transparent_bg()
              .with_ignore_pointer_events()
              .with_debug_name(name));
    };

    label(1, 72, 40, 900, 52, 38, "Rotated subtree & hit areas", ink, true);
    label(2, 72, 98, 1100, 28, 19,
          "The whole panel rotates: children paint, clip and click where the panel puts them.",
          muted);

    auto panel = div(
        ctx, mk(page.ent(), 3),
        ComponentConfig{}
            .with_absolute_position(340.f * s, 210.f * s)
            .with_size({pixels(600.f * s), pixels(220.f * s)})
            .with_rotation(18.f)
            .with_glow(afterhours::Color{34, 78, 134, 90}, 24.f)
            .with_custom_background(navy)
            .with_flex_direction(FlexDirection::Row)
            .with_align_items(AlignItems::Center)
            .with_justify_content(JustifyContent::Center)
            .with_gap(pixels(24.f * s))
            .with_padding(Padding::all(pixels(24.f * s)))
            .with_corner_radius(10.f * s)
            .with_debug_name("rot_sub_panel"));

    if (button(ctx, mk(panel.ent(), 0),
               ComponentConfig{}
                   .with_size({pixels(220.f * s), pixels(64.f * s)})
                   .with_label("Inner A")
                   .with_font("AtkinsonMockBold", pixels(21 * s))
                   .with_custom_background(teal)
                   .with_custom_text_color(card)
                   .with_corner_radius(8.f * s)
                   .with_debug_name("rot_sub_a")))
      ++clicks_a;
    if (button(ctx, mk(panel.ent(), 1),
               ComponentConfig{}
                   .with_size({pixels(220.f * s), pixels(64.f * s)})
                   .with_label("Inner B")
                   .with_font("AtkinsonMockBold", pixels(21 * s))
                   .with_custom_background(card)
                   .with_custom_text_color(ink)
                   .with_corner_radius(8.f * s)
                   .with_debug_name("rot_sub_b")))
      ++clicks_b;

    label(10, 72, 520, 600, 28, 18,
          "Inner A: " + std::to_string(clicks_a) +
              "   Inner B: " + std::to_string(clicks_b),
          muted, false, "rot_sub_counts");
    label(11, 72, 552, 600, 28, 18, "Last prompt: " + last_prompt, muted,
          false, "rot_sub_prompt");

    prompt_bar(ctx, mk(page.ent(), 20),
               {{"Enter", "Select", [this] { last_prompt = "Select"; }},
                {"Esc", "Back", [this] { last_prompt = "Back"; }}},
               ComponentConfig{}
                   .with_absolute_position(72.f * s, 640.f * s)
                   .with_debug_name("rot_sub_prompts"));
  }
};

REGISTER_EXAMPLE_SCREEN(rotated_subtree, "Layout",
                        "Rotated container: children paint and click rotated",
                        RotatedSubtreeShowcase)
