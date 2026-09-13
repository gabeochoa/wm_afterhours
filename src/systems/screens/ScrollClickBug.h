#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ScrollClickBug : ScreenSystem<UIContext<InputAction>> {
  int last_clicked = -1;
  int expected_clicked = -1;
  int pending_pointer_row = -1;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    Theme theme;
    theme.background = afterhours::Color{239, 243, 248, 255};
    theme.surface = afterhours::Color{255, 255, 255, 255};
    theme.primary = afterhours::Color{43, 99, 149, 255};
    theme.secondary = afterhours::Color{220, 230, 240, 255};
    theme.accent = afterhours::Color{0, 100, 185, 255};
    theme.font = afterhours::Color{29, 43, 59, 255};
    theme.font_muted = afterhours::Color{75, 93, 114, 255};
    context.theme = theme;
    context.scaling_mode = ScalingMode::Adaptive;
    const float s = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const float left = (context.screen_width - 1280 * s) / 2;
    const float top = (context.screen_height - 720 * s) / 2;
    UIStylingDefaults::get().set_default_font("AtkinsonMock", pixels(20 * s));
    div(context, mk(entity, 900), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_absolute_position(0, 0).with_custom_background(theme.background).with_corner_radius(0));
    auto root = div(context, mk(entity, 100), ComponentConfig{}
        .with_size({pixels(1280 * s), pixels(720 * s)})
        .with_absolute_position(left, top).with_transparent_bg().with_skip_grid_snap(true)
        .with_debug_name("scroll_click_bg"));
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
    // Title
    text(0, "Scroll click verification", 48, 24, 1184, 48, 38, theme.font, "scroll_click_title", true);
    text(3, "After scrolling, the clicked row must match the reported button index.",
         48, 80, 1184, 30, 22, theme.font_muted);
    div(context, mk(root.ent(), 4), box(48, 124, 1184, 546)
        .with_custom_background(theme.surface).with_corner_radius(12 * s));
    bool reset = button(context, mk(root.ent(), 5), box(902, 144, 310, 44)
        .with_label("Reset scroll and result").with_font("AtkinsonMock", pixels(20 * s))
        .with_custom_background(theme.secondary).with_custom_text_color(theme.font)
        .with_corner_radius(6 * s).with_debug_name("scroll_click_reset"));
    if (reset) {
      last_clicked = -1;
      expected_clicked = -1;
      pending_pointer_row = -1;
    }

    // Scroll container - 300px tall
    auto scroll_area = vstack(context, mk(root.ent(), 2), box(64, 218, 810, 300)
        .with_size({pixels(810 * s), pixels(300)})
        .with_background(Theme::Usage::Surface).with_border(theme.font_muted, 1.f)
        .with_overflow(Overflow::Scroll, Axis::Y).with_padding(Padding::all(pixels(12)))
        .with_debug_name("scroll_click_area"));
    auto &scroll = scroll_area.ent().get<HasScrollView>();
    if (reset) {
      scroll.scroll_offset = {0, 0};
      scroll.scroll_target = {0, 0};
      scroll.last_eased_offset = {0, 0};
    }
    const auto viewport = scroll_area.ent().get<UIComponent>().rect();
    const auto mouse = context.mouse.pos;
    const bool pointer_in_view = mouse.x >= viewport.x && mouse.x < viewport.x + viewport.width &&
                                 mouse.y >= viewport.y && mouse.y < viewport.y + viewport.height;
    if (context.mouse.just_pressed) pending_pointer_row = -1;
    int first_visible = -1;
    int last_visible = -1;
    // 20 buttons, each 40px tall plus 4px margins = 880px total row content
    for (int i = 0; i < 20; i++) {
      auto btn = button(context, mk(scroll_area.ent(), i), ComponentConfig{}
          .with_size({percent(0.98f), pixels(40)})
          .with_label(fmt::format("{:02d}    Button {}", i, i)).with_alignment(TextAlignment::Left)
          .with_custom_background(i == last_clicked ? afterhours::Color{24, 110, 89, 255} : theme.primary)
          .with_custom_text_color(afterhours::Color{255, 255, 255, 255})
          .with_font("AtkinsonMock", pixels(20 * s)).with_skip_grid_snap(true)
          .with_margin(Margin{.top = pixels(2), .bottom = pixels(2)})
          .with_corner_radius(4).with_debug_name(fmt::format("scroll_btn_{}", i)));
      auto rendered = btn.ent().get<UIComponent>().rect();
      rendered.y -= scroll.scroll_offset.y;
      const bool visible = rendered.y < viewport.y + viewport.height && rendered.y + rendered.height > viewport.y;
      if (visible) {
        if (first_visible < 0) first_visible = i;
        last_visible = i;
      }
      const bool under_pointer = pointer_in_view && mouse.x >= rendered.x && mouse.x < rendered.x + rendered.width &&
                                 mouse.y >= rendered.y && mouse.y < rendered.y + rendered.height;
      if (context.mouse.just_pressed && under_pointer) pending_pointer_row = i;
      if (!btn) continue;
      last_clicked = i;
      expected_clicked = pending_pointer_row;
    }
    if (context.mouse.just_released) pending_pointer_row = -1;
    // Status label
    text(1, last_clicked >= 0 ? fmt::format("Last clicked: Button {}", last_clicked) : "Last clicked: none",
         64, 140, 410, 32, 23, theme.font, "scroll_click_status", true);
    text(6, "20 buttons / zero-based indices", 64, 184, 364, 26, 18, theme.font_muted);
    text(7, fmt::format("Scroll offset: {:.0f} px", scroll.scroll_offset.y), 430, 184, 260, 26, 18,
         theme.font_muted, "scroll_click_offset");
    text(8, fmt::format("Visible: {}–{}", first_visible, last_visible), 696, 184, 190, 26, 18,
         theme.font_muted, "scroll_click_visible");
    text(9, "Scroll, then click", 902, 216, 310, 36, 27, theme.font, "", true);
    text(10, "1. Move over the list and scroll.", 902, 266, 310, 28, 19, theme.font_muted);
    text(11, "2. Read a visible button's index.", 902, 302, 310, 28, 19, theme.font_muted);
    text(12, "3. Click that row and compare.", 902, 338, 310, 28, 19, theme.font_muted);
    text(13, "Expected / pointer row", 902, 390, 310, 27, 18, theme.font_muted);
    text(14, expected_clicked >= 0 ? fmt::format("Button {}", expected_clicked) : "No pointer result",
         902, 420, 310, 32, 23, theme.font, "scroll_click_expected", true);
    text(15, "Actual / button callback", 902, 465, 310, 27, 18, theme.font_muted);
    text(16, last_clicked >= 0 ? fmt::format("Button {}", last_clicked) : "No result",
         902, 495, 310, 32, 23, theme.font, "scroll_click_actual", true);
    text(17, expected_clicked < 0 ? "Pointer comparison pending" : expected_clicked == last_clicked ? "MATCH" : "MISMATCH",
         902, 547, 310, 32, 23, expected_clicked >= 0 && expected_clicked != last_clicked ? afterhours::Color{173, 37, 37, 255} : theme.font,
         "scroll_click_comparison", true);
    text(18, "300 px viewport / bottom edge intentionally clips partial rows", 64, 618, 810, 28, 18, theme.font_muted);
    text(19, "Keyboard activation reports an actual result; compare pointer clicks here.",
         48, 682, 1184, 28, 18, theme.font_muted);
  }
};

REGISTER_EXAMPLE_SCREEN(scroll_click_bug, "Tools",
                        "Scroll offset click test", ScrollClickBug)
