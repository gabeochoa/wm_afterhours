#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct LayoutBugRepros : ScreenSystem<UIContext<InputAction>> {

  // State for tab_container repro
  size_t active_tab = 0;

  // State for toggle_switch repro
  bool toggle_a = true;
  bool toggle_b = false;
  bool toggle_c = true;

  // Colors for bug signal indicators
  afterhours::Color bg_dark{30, 30, 35, 255};
  afterhours::Color panel_bg{50, 50, 58, 255};
  afterhours::Color label_color{200, 200, 210, 255};
  afterhours::Color muted_text{140, 140, 150, 255};

  // Repro colors — contrasting so bugs are visually obvious
  afterhours::Color red_bg{200, 50, 50, 255};
  afterhours::Color blue_bg{50, 80, 200, 255};
  afterhours::Color green_bg{50, 180, 80, 255};
  afterhours::Color yellow_bg{220, 200, 50, 255};
  afterhours::Color purple_bg{150, 60, 200, 255};
  afterhours::Color orange_bg{230, 140, 40, 255};
  afterhours::Color cyan_bg{40, 200, 210, 255};
  afterhours::Color white_fg{255, 255, 255, 255};

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::midnight();
    int sw = Settings::get().get_screen_width();
    int sh = Settings::get().get_screen_height();

    // Full-screen background
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(sw), pixels(sh)})
            .with_custom_background(bg_dark)
            .with_debug_name("bg"));

    // Title
    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_label("Layout Bug Repros")
            .with_size(ComponentSize{pixels(sw - 40.f), pixels(30)})
            .with_absolute_position(20.f, 8.f)
            .with_font(UIComponent::DEFAULT_FONT, h720(22.0f))
            .with_custom_text_color(label_color)
            .with_alignment(TextAlignment::Left));

    // =========================================================
    // Repro 1: percent() inside absolute parent
    // =========================================================
    float sec1_x = 20.f;
    float sec1_y = 45.f;

    // Section label
    div(context, mk(entity, 100),
        ComponentConfig{}
            .with_label("1: percent(1.0) inside absolute parent (300px wide)")
            .with_size(ComponentSize{pixels(600), pixels(18)})
            .with_absolute_position(sec1_x, sec1_y)
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_custom_text_color(muted_text)
            .with_alignment(TextAlignment::Left));

    // Absolute parent: 300x80, colored cyan
    auto r1_parent = div(context, mk(entity, 101),
                         ComponentConfig{}
                             .with_size(ComponentSize{pixels(300), pixels(80)})
                             .with_absolute_position(sec1_x, sec1_y + 22.f)
                             .with_custom_background(cyan_bg)
                             .with_debug_name("r1_parent"));

    // Child with percent(1.0) width — should be 300px, bug = 1280px (overflows)
    div(context, mk(r1_parent.ent(), 0),
        ComponentConfig{}
            .with_label("percent(1.0) child")
            .with_size(ComponentSize{percent(1.0f), pixels(60)})
            .with_custom_background(red_bg)
            .with_custom_text_color(white_fg)
            .with_font(UIComponent::DEFAULT_FONT, h720(13.0f))
            .with_alignment(TextAlignment::Center)
            .with_margin(Margin{.top = pixels(10)})
            .with_debug_name("r1_child"));

    // Expected indicator
    div(context, mk(entity, 102),
        ComponentConfig{}
            .with_label("Expected: red fits inside cyan | Bug: red overflows right")
            .with_size(ComponentSize{pixels(500), pixels(16)})
            .with_absolute_position(sec1_x + 320.f, sec1_y + 50.f)
            .with_font(UIComponent::DEFAULT_FONT, h720(12.0f))
            .with_custom_text_color(muted_text)
            .with_alignment(TextAlignment::Left));

    // =========================================================
    // Repro 2: Row children in absolute parent
    // =========================================================
    float sec2_x = 20.f;
    float sec2_y = 160.f;

    div(context, mk(entity, 200),
        ComponentConfig{}
            .with_label("2: Row children in absolute parent (600px wide)")
            .with_size(ComponentSize{pixels(600), pixels(18)})
            .with_absolute_position(sec2_x, sec2_y)
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_custom_text_color(muted_text)
            .with_alignment(TextAlignment::Left));

    // Absolute parent, Row direction, 600px wide
    auto r2_parent = hstack(context, mk(entity, 201),
                            ComponentConfig{}
                                .with_size(ComponentSize{pixels(600), pixels(80)})
                                .with_absolute_position(sec2_x, sec2_y + 22.f)
                                .with_custom_background(panel_bg)
                                .with_no_wrap()
                                .with_debug_name("r2_parent"));

    // Three children, each 100px wide, different colors
    div(context, mk(r2_parent.ent(), 0),
        ComponentConfig{}
            .with_label("A")
            .with_size(ComponentSize{pixels(100), pixels(60)})
            .with_custom_background(red_bg)
            .with_custom_text_color(white_fg)
            .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
            .with_alignment(TextAlignment::Center)
            .with_margin(Margin{.top = pixels(10)})
            .with_debug_name("r2_A"));

    div(context, mk(r2_parent.ent(), 1),
        ComponentConfig{}
            .with_label("B")
            .with_size(ComponentSize{pixels(100), pixels(60)})
            .with_custom_background(green_bg)
            .with_custom_text_color(white_fg)
            .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
            .with_alignment(TextAlignment::Center)
            .with_margin(Margin{.top = pixels(10)})
            .with_debug_name("r2_B"));

    div(context, mk(r2_parent.ent(), 2),
        ComponentConfig{}
            .with_label("C")
            .with_size(ComponentSize{pixels(100), pixels(60)})
            .with_custom_background(blue_bg)
            .with_custom_text_color(white_fg)
            .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
            .with_alignment(TextAlignment::Center)
            .with_margin(Margin{.top = pixels(10)})
            .with_debug_name("r2_C"));

    // Expected indicator
    div(context, mk(entity, 202),
        ComponentConfig{}
            .with_label("Expected: [A][B][C] side by side | Bug: overlapping or at screen origin")
            .with_size(ComponentSize{pixels(600), pixels(16)})
            .with_absolute_position(sec2_x, sec2_y + 106.f)
            .with_font(UIComponent::DEFAULT_FONT, h720(12.0f))
            .with_custom_text_color(muted_text)
            .with_alignment(TextAlignment::Left));

    // =========================================================
    // Repro 3: Flow children at (0,0) — translate vs computed_rel
    // =========================================================
    float sec3_x = 20.f;
    float sec3_y = 290.f;

    div(context, mk(entity, 300),
        ComponentConfig{}
            .with_label("3: Flow children inside absolute parent at (400, 310)")
            .with_size(ComponentSize{pixels(600), pixels(18)})
            .with_absolute_position(sec3_x, sec3_y)
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_custom_text_color(muted_text)
            .with_alignment(TextAlignment::Left));

    // Absolute parent at offset (400, sec3_y+22), Column direction
    float r3_px = 400.f;
    float r3_py = sec3_y + 22.f;

    auto r3_parent = vstack(context, mk(entity, 301),
                            ComponentConfig{}
                                .with_size(ComponentSize{pixels(300), pixels(100)})
                                .with_absolute_position(r3_px, r3_py)
                                .with_custom_background(panel_bg)
                                .with_border(yellow_bg, 2.f)
                                .with_no_wrap()
                                .with_debug_name("r3_parent"));

    // Two flow children
    div(context, mk(r3_parent.ent(), 0),
        ComponentConfig{}
            .with_label("Child 1")
            .with_size(ComponentSize{pixels(280), pixels(35)})
            .with_custom_background(orange_bg)
            .with_custom_text_color(white_fg)
            .with_font(UIComponent::DEFAULT_FONT, h720(13.0f))
            .with_alignment(TextAlignment::Center)
            .with_margin(Margin{.top = pixels(5), .left = pixels(10)})
            .with_debug_name("r3_child1"));

    div(context, mk(r3_parent.ent(), 1),
        ComponentConfig{}
            .with_label("Child 2")
            .with_size(ComponentSize{pixels(280), pixels(35)})
            .with_custom_background(purple_bg)
            .with_custom_text_color(white_fg)
            .with_font(UIComponent::DEFAULT_FONT, h720(13.0f))
            .with_alignment(TextAlignment::Center)
            .with_margin(Margin{.top = pixels(5), .left = pixels(10)})
            .with_debug_name("r3_child2"));

    // Crosshair marker at (0,0) — if children appear here, bug confirmed
    div(context, mk(entity, 302),
        ComponentConfig{}
            .with_label("+")
            .with_size(ComponentSize{pixels(20), pixels(20)})
            .with_absolute_position(0.f, 0.f)
            .with_custom_background(
                afterhours::colors::opacity_pct(red_bg, 0.5f))
            .with_custom_text_color(white_fg)
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_alignment(TextAlignment::Center)
            .with_debug_name("r3_origin_marker"));

    div(context, mk(entity, 303),
        ComponentConfig{}
            .with_label("Expected: children inside yellow border | Bug: children at top-left (0,0)")
            .with_size(ComponentSize{pixels(700), pixels(16)})
            .with_absolute_position(sec3_x, r3_py + 105.f)
            .with_font(UIComponent::DEFAULT_FONT, h720(12.0f))
            .with_custom_text_color(muted_text)
            .with_alignment(TextAlignment::Left));

    // =========================================================
    // Repro 4: Z-order by creation order (entity ID vs mk ID)
    // =========================================================
    float sec4_x = 20.f;
    float sec4_y = 440.f;

    div(context, mk(entity, 400),
        ComponentConfig{}
            .with_label("4: Z-order — mk(5000) RED created first, mk(2000) BLUE created second")
            .with_size(ComponentSize{pixels(700), pixels(18)})
            .with_absolute_position(sec4_x, sec4_y)
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_custom_text_color(muted_text)
            .with_alignment(TextAlignment::Left));

    // First created (mk ID 5000): RED box
    div(context, mk(entity, 5000),
        ComponentConfig{}
            .with_label("RED mk(5000)")
            .with_size(ComponentSize{pixels(200), pixels(70)})
            .with_absolute_position(sec4_x, sec4_y + 22.f)
            .with_custom_background(red_bg)
            .with_custom_text_color(white_fg)
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_alignment(TextAlignment::Center)
            .with_debug_name("r4_red"));

    // Second created (mk ID 2000): BLUE box, overlapping
    div(context, mk(entity, 2000),
        ComponentConfig{}
            .with_label("BLUE mk(2000) ON TOP?")
            .with_size(ComponentSize{pixels(200), pixels(70)})
            .with_absolute_position(sec4_x + 80.f, sec4_y + 42.f)
            .with_custom_background(blue_bg)
            .with_custom_text_color(white_fg)
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_alignment(TextAlignment::Center)
            .with_debug_name("r4_blue"));

    div(context, mk(entity, 402),
        ComponentConfig{}
            .with_label(
                "Expected: RED(5000) on top | Bug: BLUE(2000) on top (entity ID order, not mk ID)")
            .with_size(ComponentSize{pixels(700), pixels(16)})
            .with_absolute_position(sec4_x, sec4_y + 116.f)
            .with_font(UIComponent::DEFAULT_FONT, h720(12.0f))
            .with_custom_text_color(muted_text)
            .with_alignment(TextAlignment::Left));

    // =========================================================
    // Repro 5: children() text width miscalculation
    // =========================================================
    float sec5_x = 20.f;
    float sec5_y = 570.f;

    div(context, mk(entity, 500),
        ComponentConfig{}
            .with_label("5: children() width vs manual pixels() for text sizing")
            .with_size(ComponentSize{pixels(600), pixels(18)})
            .with_absolute_position(sec5_x, sec5_y)
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_custom_text_color(muted_text)
            .with_alignment(TextAlignment::Left));

    // Button with children() width — auto-sized to text content
    button(context, mk(entity, 501),
           ComponentConfig{}
               .with_label("Settings & Configuration")
               .with_size(ComponentSize{children(), pixels(40)})
               .with_absolute_position(sec5_x, sec5_y + 22.f)
               .with_custom_background(green_bg)
               .with_custom_text_color(white_fg)
               .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
               .with_alignment(TextAlignment::Center)
               .with_padding(Spacing::sm)
               .with_debug_name("r5_auto"));

    // Reference button with manually specified width
    button(context, mk(entity, 502),
           ComponentConfig{}
               .with_label("Settings & Configuration")
               .with_size(ComponentSize{pixels(250), pixels(40)})
               .with_absolute_position(sec5_x, sec5_y + 68.f)
               .with_custom_background(blue_bg)
               .with_custom_text_color(white_fg)
               .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
               .with_alignment(TextAlignment::Center)
               .with_padding(Spacing::sm)
               .with_debug_name("r5_manual"));

    div(context, mk(entity, 503),
        ComponentConfig{}
            .with_label("Green=children() auto | Blue=pixels(250) manual | Compare for clipping/padding mismatch")
            .with_size(ComponentSize{pixels(700), pixels(16)})
            .with_absolute_position(sec5_x, sec5_y + 114.f)
            .with_font(UIComponent::DEFAULT_FONT, h720(12.0f))
            .with_custom_text_color(muted_text)
            .with_alignment(TextAlignment::Left));

    // =========================================================
    // Repro 6: tab_container inside absolute parent
    // =========================================================
    float sec6_x = 700.f;
    float sec6_y = 45.f;

    div(context, mk(entity, 600),
        ComponentConfig{}
            .with_label("6: tab_container inside absolute parent")
            .with_size(ComponentSize{pixels(500), pixels(18)})
            .with_absolute_position(sec6_x, sec6_y)
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_custom_text_color(muted_text)
            .with_alignment(TextAlignment::Left));

    // Absolute parent for the tab_container
    auto r6_parent = div(context, mk(entity, 601),
                         ComponentConfig{}
                             .with_size(ComponentSize{pixels(500), pixels(60)})
                             .with_absolute_position(sec6_x, sec6_y + 22.f)
                             .with_custom_background(panel_bg)
                             .with_border(green_bg, 2.f)
                             .with_debug_name("r6_parent"));

    std::vector<std::string> tab_labels = {"Tab A", "Tab B", "Tab C"};
    tab_container(context, mk(r6_parent.ent(), 0), tab_labels, active_tab,
                  ComponentConfig{}
                      .with_size(ComponentSize{percent(1.0f), pixels(44)})
                      .with_no_wrap()
                      .with_debug_name("r6_tabs"));

    div(context, mk(entity, 602),
        ComponentConfig{}
            .with_label("Expected: tabs inside green border | Bug: tabs at screen origin")
            .with_size(ComponentSize{pixels(500), pixels(16)})
            .with_absolute_position(sec6_x, sec6_y + 86.f)
            .with_font(UIComponent::DEFAULT_FONT, h720(12.0f))
            .with_custom_text_color(muted_text)
            .with_alignment(TextAlignment::Left));

    // =========================================================
    // Repro 7: toggle_switch vertical space overflow
    // =========================================================
    float sec7_x = 700.f;
    float sec7_y = 160.f;

    div(context, mk(entity, 700),
        ComponentConfig{}
            .with_label("7: toggle_switch vertical space — 3 toggles in 150px container")
            .with_size(ComponentSize{pixels(500), pixels(18)})
            .with_absolute_position(sec7_x, sec7_y)
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_custom_text_color(muted_text)
            .with_alignment(TextAlignment::Left));

    // Container with exact height: 3 toggles at 42px each = 126px + margins
    // If toggles take more space than declared, they'll overflow the green border
    auto r7_parent = vstack(context, mk(entity, 701),
                            ComponentConfig{}
                                .with_size(ComponentSize{pixels(450), pixels(150)})
                                .with_absolute_position(sec7_x, sec7_y + 22.f)
                                .with_custom_background(panel_bg)
                                .with_border(green_bg, 2.f)
                                .with_no_wrap()
                                .with_debug_name("r7_parent"));

    toggle_switch(context, mk(r7_parent.ent(), 0), toggle_a,
                  ComponentConfig{}
                      .with_label("Toggle A")
                      .with_size(ComponentSize{percent(1.0f), pixels(42)})
                      .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
                      .with_margin(Margin{.bottom = pixels(4)})
                      .with_debug_name("r7_toggle_a"));

    toggle_switch(context, mk(r7_parent.ent(), 1), toggle_b,
                  ComponentConfig{}
                      .with_label("Toggle B")
                      .with_size(ComponentSize{percent(1.0f), pixels(42)})
                      .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
                      .with_margin(Margin{.bottom = pixels(4)})
                      .with_debug_name("r7_toggle_b"));

    toggle_switch(context, mk(r7_parent.ent(), 2), toggle_c,
                  ComponentConfig{}
                      .with_label("Toggle C")
                      .with_size(ComponentSize{percent(1.0f), pixels(42)})
                      .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
                      .with_debug_name("r7_toggle_c"));

    div(context, mk(entity, 702),
        ComponentConfig{}
            .with_label("Expected: 3 toggles fit inside green border | Bug: overflow bottom")
            .with_size(ComponentSize{pixels(500), pixels(16)})
            .with_absolute_position(sec7_x, sec7_y + 178.f)
            .with_font(UIComponent::DEFAULT_FONT, h720(12.0f))
            .with_custom_text_color(muted_text)
            .with_alignment(TextAlignment::Left));
  }
};

REGISTER_EXAMPLE_SCREEN(layout_bug_repros, "Tools",
                        "Repros for 5 layout engine issues",
                        LayoutBugRepros)
