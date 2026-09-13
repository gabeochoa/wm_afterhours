#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <array>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct LayoutBugRepros : ScreenSystem<UIContext<InputAction>> {

  // State for tab_container repro
  size_t active_tab = 0;
  int inspected_case = 0;

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
            .with_corner_radius(0)
            .with_debug_name("bg"));

    const auto text = [&](int id, const std::string &value, float x, float y,
                          float width, float height, float size = 18.f, bool strong = false) {
      return div(context, mk(entity, id), ComponentConfig{}
          .with_size({pixels(width), pixels(height)}).with_absolute_position(x, y)
          .with_background(Theme::Usage::None).with_corner_radius(0)
          .with_label(value).with_alignment(TextAlignment::Left)
          .with_font(strong ? "AtkinsonMockBold" : "AtkinsonMock", pixels(size))
          .with_custom_text_color(strong ? white_fg : label_color).with_ignore_pointer_events());
    };
    const auto status = [&](int id, bool retained, float x, float y) {
      return div(context, mk(entity, id), ComponentConfig{}
          .with_size({pixels(110), pixels(20)}).with_absolute_position(x, y)
          .with_label(retained ? "Expected" : "Fixed")
          .with_font("AtkinsonMock", pixels(15)).with_alignment(TextAlignment::Center)
          .with_custom_background(retained ? afterhours::Color{44, 68, 84, 255} : afterhours::Color{43, 76, 57, 255})
          .with_custom_text_color({211, 239, 224, 255}).with_corner_radius(4)
          .with_ignore_pointer_events());
    };
    // Title
    text(1, "Layout regression fixtures", 20, 7, 420, 32, 25, true);
    text(8000, sw < 1280 || sh < 720 ? "Fixed fixtures need 1280 x 720; enlarge the window." :
        "Retained cases / badges state current expectations", 452, 12, 590, 24, 18);
    text(8001, "Origin 0,0 at red +", 1048, 12, 222, 24, 17);

    // =========================================================
    // Repro 1: percent() inside absolute parent
    // =========================================================
    float sec1_x = 20.f;
    float sec1_y = 45.f;

    // Section label
    text(100, "1 / Percent width", 20, 44, 490, 22, 18, true);
    status(110, true, 520, 44);

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
            .with_font("AtkinsonMock", h720(13.0f))
            .with_alignment(TextAlignment::Center)
            .with_margin(Margin{.top = pixels(10)})
            .with_debug_name("r1_child"));

    // Expected indicator
    text(102, "Parent 300 x 80px / child 100% x 60px", 338, 74, 350, 25, 17);
    text(8002, "Expected: red width matches cyan.", 338, 103, 350, 24, 17);
    text(8003, "10px top margin / no horizontal overflow", 338, 130, 350, 22, 16);

    // =========================================================
    // Repro 2: Row children in absolute parent
    // =========================================================
    float sec2_x = 20.f;
    float sec2_y = 160.f;

    text(200, "2 / Fixed-width row", 20, 160, 490, 22, 18, true);
    status(210, false, 520, 160);

    // Absolute parent, Row direction, 600px wide
    auto r2_parent =
        hstack(context, mk(entity, 201),
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
            .with_font("AtkinsonMock", h720(16.0f))
            .with_alignment(TextAlignment::Center)
            .with_margin(Margin{.top = pixels(10)})
            .with_debug_name("r2_A"));

    div(context, mk(r2_parent.ent(), 1),
        ComponentConfig{}
            .with_label("B")
            .with_size(ComponentSize{pixels(100), pixels(60)})
            .with_custom_background(green_bg)
            .with_custom_text_color(white_fg)
            .with_font("AtkinsonMock", h720(16.0f))
            .with_alignment(TextAlignment::Center)
            .with_margin(Margin{.top = pixels(10)})
            .with_debug_name("r2_B"));

    div(context, mk(r2_parent.ent(), 2),
        ComponentConfig{}
            .with_label("C")
            .with_size(ComponentSize{pixels(100), pixels(60)})
            .with_custom_background(blue_bg)
            .with_custom_text_color(white_fg)
            .with_font("AtkinsonMock", h720(16.0f))
            .with_alignment(TextAlignment::Center)
            .with_margin(Margin{.top = pixels(10)})
            .with_debug_name("r2_C"));

    // Expected indicator
    text(202, "Expected: A, B, C in order / each 100px / parent 600px / 300px unused", 20, 267, 680, 22, 16);

    // =========================================================
    // Repro 3: Flow children at (0,0) — translate vs computed_rel
    // =========================================================
    float sec3_x = 20.f;
    float sec3_y = 290.f;

    text(300, "3 / Offset flow children", 20, 290, 490, 22, 18, true);
    status(310, false, 520, 290);

    // Absolute parent at offset (400, sec3_y+22), Column direction
    float r3_px = 400.f;
    float r3_py = sec3_y + 22.f;

    auto r3_parent =
        vstack(context, mk(entity, 301),
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
            .with_font("AtkinsonMock", h720(13.0f))
            .with_alignment(TextAlignment::Center)
            .with_margin(Margin{.top = pixels(5), .left = pixels(10)})
            .with_debug_name("r3_child1"));

    div(context, mk(r3_parent.ent(), 1),
        ComponentConfig{}
            .with_label("Child 2")
            .with_size(ComponentSize{pixels(280), pixels(35)})
            .with_custom_background(purple_bg)
            .with_custom_text_color(white_fg)
            .with_font("AtkinsonMock", h720(13.0f))
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
            .with_font("AtkinsonMock", h720(14.0f))
            .with_alignment(TextAlignment::Center)
            .with_debug_name("r3_origin_marker"));

    text(303, "Expected: both flow children stay inside the yellow parent.", 20, 416, 680, 22, 17);
    text(8004, "Parent x400 / y312 screen px", 20, 343, 365, 24, 18);
    text(8005, "Children 280 x 35px / top margin 5px", 20, 375, 365, 24, 17);
    div(context, mk(entity, 8006), ComponentConfig{}
        .with_size({pixels(370), pixels(2)}).with_absolute_position(20, 331)
        .with_custom_background({120, 132, 145, 255}).with_corner_radius(0).with_ignore_pointer_events());

    // =========================================================
    // Repro 4: Z-order by creation order (entity ID vs mk ID)
    // =========================================================
    float sec4_x = 20.f;
    float sec4_y = 440.f;

    text(400, "4 / Creation order", 20, 440, 490, 22, 18, true);
    status(410, true, 520, 440);

    // First created (mk ID 5000): RED box
    div(context, mk(entity, 5000),
        ComponentConfig{}
            .with_label("RED mk(5000)")
            .with_size(ComponentSize{pixels(200), pixels(70)})
            .with_absolute_position(sec4_x, sec4_y + 22.f)
            .with_custom_background(red_bg)
            .with_custom_text_color(white_fg)
            .with_font("AtkinsonMock", h720(14.0f))
            .with_alignment(TextAlignment::Center)
            .with_debug_name("r4_red"));

    // Second created (mk ID 2000): BLUE box, overlapping
    div(context, mk(entity, 2000),
        ComponentConfig{}
            .with_label("BLUE mk(2000)")
            .with_size(ComponentSize{pixels(200), pixels(70)})
            .with_absolute_position(sec4_x + 80.f, sec4_y + 42.f)
            .with_custom_background(blue_bg)
            .with_custom_text_color(white_fg)
            .with_font("AtkinsonMock", h720(14.0f))
            .with_alignment(TextAlignment::Center)
            .with_debug_name("r4_blue"));

    text(402, "Red / mk(5000) / created first", 318, 477, 370, 25, 18);
    text(8007, "Blue / mk(2000) / created second", 318, 506, 370, 25, 18);
    text(8008, "Expected: blue covers red in the overlap.", 318, 535, 370, 25, 17);

    // =========================================================
    // Repro 5: children() text width miscalculation
    // =========================================================
    float sec5_x = 20.f;
    float sec5_y = 570.f;

    text(500, "5 / Text-sized width", 20, 570, 490, 22, 18, true);
    status(510, false, 520, 570);

    // Button with children() width — auto-sized to text content
    auto auto_width = button(context, mk(entity, 501),
           ComponentConfig{}
               .with_label("Settings & Configuration")
               .with_size(ComponentSize{children(), pixels(40)})
               .with_absolute_position(sec5_x, sec5_y + 22.f)
               .with_custom_background(green_bg)
               .with_custom_text_color(white_fg)
               .with_font("AtkinsonMock", h720(16.0f))
               .with_alignment(TextAlignment::Center)
               .with_padding(Spacing::sm)
               .with_debug_name("r5_auto"));

    // Reference button with manually specified width
    auto fixed_width = button(context, mk(entity, 502),
           ComponentConfig{}
               .with_label("Settings & Configuration")
               .with_size(ComponentSize{pixels(250), pixels(40)})
               .with_absolute_position(sec5_x, sec5_y + 68.f)
               .with_custom_background(blue_bg)
               .with_custom_text_color(white_fg)
               .with_font("AtkinsonMock", h720(16.0f))
               .with_alignment(TextAlignment::Center)
               .with_padding(Spacing::sm)
               .with_debug_name("r5_manual"));

    const float auto_caption_x = std::max(298.f, auto_width.cmp().rect().x + auto_width.cmp().rect().width + 16.f);
    text(503, "Auto width / children()", auto_caption_x, 599, std::max(120.f, 700.f - auto_caption_x), 25, 18);
    text(8009, "250px reference / pixels(250)", 298, 645, 380, 25, 18);
    text(8010, "1280 x 720 reference / native pixel geometry is retained at every viewport size.", 20, 687, 690, 22, 16);

    // =========================================================
    // Repro 6: tab_container inside absolute parent
    // =========================================================
    float sec6_x = 700.f;
    float sec6_y = 45.f;

    text(600, "6 / Native tabs", 700, 44, 385, 22, 18, true);
    status(610, false, 1090, 44);

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

    text(602, "Selected: " + tab_labels[active_tab] + " / expected inside green bounds", 700, 131, 510, 23, 17);

    // =========================================================
    // Repro 7: toggle_switch vertical space overflow
    // =========================================================
    float sec7_x = 700.f;
    float sec7_y = 160.f;

    text(700, "7 / Toggle height", 700, 160, 385, 22, 18, true);
    status(710, true, 1090, 160);

    // Container with exact height: 3 toggles at 42px each = 126px + margins
    // If toggles take more space than declared, they'll overflow the green
    // border
    auto r7_parent =
        vstack(context, mk(entity, 701),
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(450), pixels(150)})
                   .with_absolute_position(sec7_x, sec7_y + 22.f)
                   .with_custom_background(panel_bg)
                   .with_border(green_bg, 2.f)
                   .with_no_wrap()
                   .with_debug_name("r7_parent"));

    // Use explicit pixel width (parent 450px - 2px border each side = 446px)
    // instead of percent(1.0f) to avoid expand() resolution issues
    // inside absolutely-positioned containers
    toggle_switch(context, mk(r7_parent.ent(), 0), toggle_a,
                  ComponentConfig{}
                      .with_label("Toggle A")
                      .with_size(ComponentSize{pixels(446), pixels(42)})
                      .with_font("AtkinsonMock", h720(14.0f))
                      .with_margin(Margin{.bottom = pixels(4)})
                      .with_debug_name("r7_toggle_a"));

    toggle_switch(context, mk(r7_parent.ent(), 1), toggle_b,
                  ComponentConfig{}
                      .with_label("Toggle B")
                      .with_size(ComponentSize{pixels(446), pixels(42)})
                      .with_font("AtkinsonMock", h720(14.0f))
                      .with_margin(Margin{.bottom = pixels(4)})
                      .with_debug_name("r7_toggle_b"));

    toggle_switch(context, mk(r7_parent.ent(), 2), toggle_c,
                  ComponentConfig{}
                      .with_label("Toggle C")
                      .with_size(ComponentSize{pixels(446), pixels(42)})
                      .with_font("AtkinsonMock", h720(14.0f))
                      .with_debug_name("r7_toggle_c"));

    text(702, fmt::format("A {} / B {} / C {} / expected inside green bounds",
        toggle_a ? "On" : "Off", toggle_b ? "On" : "Off", toggle_c ? "On" : "Off"),
        700, 337, 520, 24, 17);
    const std::array<std::string, 7> names{"Percent width", "Fixed-width row", "Offset flow", "Creation order",
        "Text-sized width", "Native tabs", "Toggle height"};
    const std::array<std::string, 7> geometry{
        "Parent 300 x 80px; child width is 100%, height 60px.",
        "Parent 600 x 80px; A, B and C are each 100 x 60px.",
        "Parent x400, y312, 300 x 100px; children 280 x 35px.",
        "Both boxes 200 x 70px; blue offset is +80, +20px.",
        "Both heights 40px; auto text width vs requested 250px.",
        "Parent 500 x 60px; tabs use 100% width and 44px height.",
        "Parent 450 x 150px; rows request 42px + 4px margins."};
    const std::array<std::string, 7> expected{
        "Red fills the cyan width, without extending beyond it.",
        "Three separate children use 300px; 300px remains free.",
        "Children follow the parent offset, away from the origin.",
        "Blue covers red. Creation order determines this overlap.",
        "Compare text fit; auto width need not match 250px.",
        "Tab changes stay inside the green parent outline.",
        "Declared sum: 3 x 42 + 2 x 4 = 134px, within 150px."};
    const std::array<std::string, 7> colors{
        "Cyan = parent; red = percentage-sized child.",
        "Gray = parent; red A, green B, blue C identify order.",
        "Yellow = parent outline; orange 1, purple 2 = children.",
        "Red = first entity; blue = second entity, lower mk ID.",
        "Green = automatic width; blue = fixed width reference.",
        "Green = parent outline; yellow underline = selection.",
        "Green = parent outline; yellow = on, gray = off."};
    auto inspector = div(context, mk(entity, 8100), ComponentConfig{}
        .with_size({pixels(530), pixels(330)}).with_absolute_position(720, 374)
        .with_custom_background({38, 44, 55, 255}).with_corner_radius(10)
        .with_debug_name("repro_inspector"));
    text(8101, "Case inspector / 1-5 left, 6-7 right", 738, 387, 495, 28, 21, true);
    for (int i = 0; i < 7; ++i) {
      if (!button(context, mk(inspector.ent(), i), ComponentConfig{}
          .with_size({pixels(60), pixels(32)}).with_absolute_position(18 + i * 70.f, 50)
          .with_label(std::to_string(i + 1)).with_font("AtkinsonMock", pixels(18))
          .with_alignment(TextAlignment::Center).with_corner_radius(5)
          .with_custom_background(inspected_case == i ? afterhours::Color{105, 192, 211, 255} : panel_bg)
          .with_custom_text_color(inspected_case == i ? afterhours::Color{0, 0, 0, 255} : white_fg)
          .with_debug_name("inspect_case_" + std::to_string(i + 1)))) continue;
      inspected_case = i;
    }
    text(8102, std::to_string(inspected_case + 1) + " / " + names[inspected_case], 738, 468, 490, 27, 23, true);
    text(8103, geometry[inspected_case], 738, 508, 490, 26, 17);
    text(8104, expected[inspected_case], 738, 541, 490, 26, 17);
    text(8105, colors[inspected_case], 738, 574, 490, 26, 17);
    const std::array<afterhours::EntityID, 7> measured_ids{r1_parent.ent().id, r2_parent.ent().id,
        r3_parent.ent().id, auto_width.ent().id, auto_width.ent().id, r6_parent.ent().id, r7_parent.ent().id};
    const auto measured = UICollectionHolder::getEntityForID(measured_ids[inspected_case]);
    if (measured && inspected_case != 3 && inspected_case != 4 && inspected_case != 6) {
      const auto r = measured.asE().get<UIComponent>().rect();
      text(8110, fmt::format("Drawn parent: {:.0f} x {:.0f}px at x{:.0f}, y{:.0f}", r.width, r.height, r.x, r.y),
           738, 609, 490, 24, 17);
    }
    if (inspected_case == 3)
      text(8110, "mk ID identifies an entity; it does not set render depth.", 738, 609, 490, 24, 17);
    if (inspected_case == 4)
      text(8110, fmt::format("Drawn widths: auto {:.0f}px / reference {:.0f}px", auto_width.cmp().rect().width, fixed_width.cmp().rect().width),
           738, 609, 490, 24, 17);
    if (inspected_case == 6) {
      float first_y = 0;
      float last_y = 0;
      for (size_t i = 0; i < r7_parent.cmp().children.size(); ++i) {
        const auto child = UICollectionHolder::getEntityForID(r7_parent.cmp().children[i]);
        if (!child) continue;
        const auto rect = child.asE().get<UIComponent>().rect();
        if (i == 0) first_y = rect.y;
        last_y = rect.y + rect.height;
      }
      text(8110, fmt::format("Drawn rows span {:.0f}px / parent {:.0f}px", last_y - first_y, r7_parent.cmp().rect().height),
           738, 609, 490, 24, 17);
    }
    if (inspected_case == 0) {
      div(context, mk(entity, 8111), ComponentConfig{}
          .with_size({pixels(150), pixels(26)}).with_absolute_position(743, 642)
          .with_custom_background(cyan_bg).with_corner_radius(0).with_ignore_pointer_events()
          .with_on_draw_fg([red = red_bg](RectangleType r) {
            afterhours::draw_rectangle({r.x, r.y + 4, r.width, r.height - 8}, red);
          }));
      text(8112, "Expected width match", 904, 642, 330, 26, 16);
    }
    div(context, mk(entity, 8120), ComponentConfig{}
        .with_size({pixels(680), pixels(520)}).with_absolute_position(16, 180)
        .with_background(Theme::Usage::None).with_ignore_pointer_events()
        .with_on_draw_fg([auto_id = auto_width.ent().id, fixed_id = fixed_width.ent().id,
                         toggle_id = r7_parent.ent().id](RectangleType) {
          const afterhours::Color guide{173, 186, 204, 180};
          for (const auto id : {auto_id, fixed_id}) {
            const auto item = UICollectionHolder::getEntityForID(id);
            if (!item) continue;
            const auto r = item.asE().get<UIComponent>().rect();
            const float y = r.y + r.height + 3;
            raylib::DrawLineEx({r.x, y}, {r.x + r.width, y}, 1.f, guide);
            raylib::DrawLineEx({r.x, y - 2}, {r.x, y + 2}, 1.f, guide);
            raylib::DrawLineEx({r.x + r.width, y - 2}, {r.x + r.width, y + 2}, 1.f, guide);
          }
          const auto toggles = UICollectionHolder::getEntityForID(toggle_id);
          if (!toggles) return;
          for (const auto id : toggles.asE().get<UIComponent>().children) {
            const auto row = UICollectionHolder::getEntityForID(id);
            if (!row) continue;
            const auto r = row.asE().get<UIComponent>().rect();
            const float y = r.y + r.height / 2;
            raylib::DrawLineEx({r.x - 14, y}, {r.x - 3, y}, 1.f, guide);
          }
        }));
    text(8106, "Fixed = historical regression; Expected = intentional behavior.", 738, 674, 490, 22, 15);
  }
};

REGISTER_EXAMPLE_SCREEN(layout_bug_repros, "Tools",
                        "Seven retained layout regression fixtures", LayoutBugRepros)
