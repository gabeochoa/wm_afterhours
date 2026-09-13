#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <array>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Demonstrates the hstack() primitive.
// Default: percent(1.0) width, children() height -- fills parent, shrinks to
// content.
struct HStackShowcase : ScreenSystem<UIContext<InputAction>> {
  int page = 0;
  std::string toolbar_feedback = "Activate a command to test the toolbar.";

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = afterhours::ui::theme_presets::neon_dark();
    theme.background = {22, 28, 40, 255};
    theme.font = {239, 244, 251, 255};
    theme.font_muted = {174, 191, 215, 255};
    context.set_theme(theme);
    context.scaling_mode = ScalingMode::Proportional;
    const float s = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const float left = (context.screen_width - 1160 * s) / 2;
    const float top = (context.screen_height - 720 * s) / 2;
    UIStylingDefaults::get().set_default_font("AtkinsonMock", pixels(18 * s));
    // Bright, high-contrast colors for demo boxes
    const auto teal = afterhours::Color{0, 180, 220, 255};
    const auto pink = afterhours::Color{220, 60, 120, 255};
    const auto green = afterhours::Color{50, 210, 130, 255};
    const auto orange = afterhours::Color{250, 140, 40, 255};
    const auto purple = afterhours::Color{160, 100, 255, 255};
    const auto blue = afterhours::Color{70, 140, 255, 255};
    const auto panel_bg = afterhours::Color{37, 47, 65, 255};
    const auto box = [s](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, y * s).with_background(Theme::Usage::None)
          .with_corner_radius(0).with_skip_grid_snap(true);
    };
    // Full-screen root
    auto canvas = div(context, mk(entity, 0), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background(theme.background).with_corner_radius(0).with_debug_name("bg"));
    auto root = div(context, mk(canvas.ent(), 0), box(0, 0, 1160, 720)
        .with_absolute_position(left, top).with_debug_name("root"));
    const auto label = [&](afterhours::Entity &parent, int id, const std::string &value,
                           float x, float y, float w, float h, float size = 18.f, bool strong = false) {
      return div(context, mk(parent, id), box(x, y, w, h).with_label(value)
          .with_font(strong ? "AtkinsonMockBold" : "AtkinsonMock", pixels(size * s))
          .with_custom_text_color(strong ? theme.font : theme.font_muted)
          .with_alignment(TextAlignment::Left).with_ignore_pointer_events());
    };
    // Title
    label(root.ent(), 0, "hstack()", 0, 18, 1160, 43, 34, true);
    label(root.ent(), 1, "Horizontal stack layout / compare native distribution, alignment and practical patterns.",
          0, 69, 1160, 28, 21);
    for (int i = 0; i < 2; ++i) {
      if (!button(context, mk(root.ent(), 2 + i), box(i * 362.f, 112, 346, 38)
          .with_label(i == 0 ? "Width and distribution" : "Alignment and patterns")
          .with_font("AtkinsonMock", pixels(20 * s)).with_alignment(TextAlignment::Center)
          .with_custom_background(page == i ? teal : panel_bg)
          .with_custom_text_color(page == i ? afterhours::Color{0, 0, 0, 255} : theme.font)
          .with_corner_radius(8 * s).with_debug_name("hstack_page_" + std::to_string(i)))) continue;
      page = i;
    }
    if (page == 0) {
      // == Row 1: Basic hstack with 3 large boxes ==
      // Note: hstack auto-fills parent width; only height needs specifying
      label(root.ent(), 10, "Equal-width children", 0, 174, 1160, 31, 26, true);
      label(root.ent(), 11, "Each child takes 30% of content width. SpaceAround distributes the remaining width; explicit gap is 0.",
            0, 210, 1160, 27, 19);
      auto row1 = hstack(context, mk(root.ent(), 12), box(0, 246, 1160, 88)
          .with_custom_background(panel_bg).with_padding(Padding::all(pixels(16 * s)))
          .with_align_items(AlignItems::Center).with_justify_content(JustifyContent::SpaceAround)
          .with_debug_name("basic_row"));
      const std::array<afterhours::Color, 3> colors{teal, pink, green};
      for (int i = 0; i < 3; ++i)
        div(context, mk(row1.ent(), i), ComponentConfig{}.with_size({percent(.30f), percent(.85f)})
            .with_label("Item " + std::to_string(i + 1)).with_alignment(TextAlignment::Center)
            .with_font("AtkinsonMock", pixels(23 * s)).with_custom_background(colors[i])
            .with_custom_text_color({0, 0, 0, 255}).with_roundness(.08f).with_skip_grid_snap(true)
            .with_debug_name("box_" + std::to_string(i)));
      label(root.ent(), 13, "Content 1128px / child 338.4px / edge 18.8px / internal gap 37.6px.",
            0, 343, 1160, 26, 18);
      // == Row 2: Five JustifyContent demos side-by-side ==
      label(root.ent(), 14, "Main-axis distribution", 0, 390, 1160, 31, 26, true);
      label(root.ent(), 15, "JustifyContent / identical 208 x 140px frames / three 30 x 30px children / padding 12px / gap 0.",
            0, 426, 1160, 26, 18);
      const std::array<const char *, 5> names{"Start", "End", "Center", "Space between", "Space around"};
      const std::array<JustifyContent, 5> modes{JustifyContent::FlexStart, JustifyContent::FlexEnd,
          JustifyContent::Center, JustifyContent::SpaceBetween, JustifyContent::SpaceAround};
      for (int j = 0; j < 5; ++j) {
        const float x = j * 236.f;
        label(root.ent(), 20 + j, names[j], x, 468, 216, 29, 22, true);
        // hstack container showing this justify mode
        auto frame = hstack(context, mk(root.ent(), 30 + j), box(x, 509, 208, 140)
            .with_custom_background(panel_bg).with_padding(Padding::all(pixels(12 * s)))
            .with_justify_content(modes[j]).with_align_items(AlignItems::Center)
            .with_debug_name("jc_row_" + std::to_string(j)));
        for (int k = 0; k < 3; ++k)
          div(context, mk(frame.ent(), k), ComponentConfig{}.with_size({pixels(30 * s), pixels(30 * s)})
              .with_label(std::to_string(k + 1)).with_font("AtkinsonMock", pixels(18 * s))
              .with_alignment(TextAlignment::Center).with_custom_text_color({0, 0, 0, 255})
              .with_custom_background(teal).with_roundness(.15f).with_skip_grid_snap(true)
              .with_debug_name("jc_box_" + std::to_string(j) + "_" + std::to_string(k)));
        div(context, mk(root.ent(), 40 + j), box(x, 509, 208, 140).with_ignore_pointer_events()
            .with_on_draw_fg([j, s, eid = frame.ent().id](RectangleType r) {
              const afterhours::Color ink{170, 204, 226, 145};
              if (j < 3) {
                const float xline = j == 0 ? r.x + 12 * s : j == 1 ? r.x + r.width - 12 * s : r.x + r.width / 2;
                raylib::DrawLineEx({xline, r.y + 8 * s}, {xline, r.y + r.height - 8 * s}, s, ink);
                return;
              }
              const auto parent = UICollectionHolder::getEntityForID(eid);
              if (!parent) return;
              const auto &children = parent.asE().get<UIComponent>().children;
              const auto bracket = [&](float start, float end) {
                const float yline = r.y + r.height - 16 * s;
                raylib::DrawLineEx({start, yline}, {end, yline}, s, ink);
                raylib::DrawLineEx({start, yline - 4 * s}, {start, yline + 4 * s}, s, ink);
                raylib::DrawLineEx({end, yline - 4 * s}, {end, yline + 4 * s}, s, ink);
              };
              float previous_end = r.x + 12 * s;
              for (size_t i = 0; i < children.size(); ++i) {
                const auto child = UICollectionHolder::getEntityForID(children[i]);
                if (!child) continue;
                const auto rect = child.asE().get<UIComponent>().rect();
                if (i > 0 || j == 4) bracket(previous_end, rect.x);
                previous_end = rect.x + rect.width;
              }
              if (j == 4) bracket(previous_end, r.x + r.width - 12 * s);
            }));
      }
      label(root.ent(), 16, "Between has equal gaps; Around has half-sized edge gaps. Values use 720p design pixels.",
            0, 672, 1160, 29, 18);
      return;
    }
    // LEFT: 3 AlignItems demos
    label(root.ent(), 50, "Cross-axis alignment", 0, 174, 730, 31, 25, true);
    label(root.ent(), 51, "AlignItems / heights 85%, 50%, 65% of content height / widths 28px", 0, 210, 730, 25, 17);
    const std::array<afterhours::Color, 3> align_colors{green, orange, purple};
    const std::array<float, 3> heights{.85f, .50f, .65f};
    const std::array<const char *, 3> align_names{"Start", "Center", "End"};
    const std::array<AlignItems, 3> aligns{AlignItems::FlexStart, AlignItems::Center, AlignItems::FlexEnd};
    for (int a = 0; a < 3; ++a) {
      const float x = a * 248.f;
      label(root.ent(), 60 + a, align_names[a], x, 250, 228, 28, 22, true);
      auto frame = hstack(context, mk(root.ent(), 70 + a), box(x, 291, 228, 224)
          .with_custom_background(panel_bg).with_padding(Padding::all(pixels(12 * s)))
          .with_align_items(aligns[a]).with_justify_content(JustifyContent::SpaceAround)
          .with_debug_name("ai_row_" + std::to_string(a))
          .with_on_draw_fg([a, s](RectangleType r) {
            const float yline = a == 0 ? r.y + 12 * s : a == 2 ? r.y + r.height - 12 * s : r.y + r.height / 2;
            raylib::DrawLineEx({r.x, yline}, {r.x + r.width, yline}, s, {181, 211, 236, 155});
          }));
      for (int k = 0; k < 3; ++k)
        div(context, mk(frame.ent(), k), ComponentConfig{}.with_size({pixels(28 * s), percent(heights[k])})
            .with_label(std::to_string(k + 1)).with_font("AtkinsonMock", pixels(18 * s)).with_alignment(TextAlignment::Center)
            .with_custom_background(align_colors[k]).with_custom_text_color({0, 0, 0, 255})
            .with_corner_radius(4 * s).with_skip_grid_snap(true).with_debug_name("ai_box_" + std::to_string(a) + "_" + std::to_string(k)));
    }
    // RIGHT: Real-world toolbar pattern
    label(root.ent(), 80, "Toolbar pattern", 760, 174, 400, 31, 25, true);
    label(root.ent(), 81, "SpaceBetween / trailing Settings action", 760, 210, 400, 25, 17);
    auto toolbar = hstack(context, mk(root.ent(), 82), box(760, 249, 400, 50)
        .with_custom_background(panel_bg).with_padding(Padding{.left = pixels(6 * s), .right = pixels(6 * s)})
        .with_align_items(AlignItems::Center).with_justify_content(JustifyContent::SpaceBetween).with_debug_name("toolbar"));
    // Nested hstack: shrinks to content since parent already constrains width
    auto commands = hstack(context, mk(toolbar.ent(), 0), ComponentConfig{}
        .with_size({children(), children()}).with_align_items(AlignItems::Center).with_gap(pixels(4 * s)));
    const std::array<const char *, 3> command_names{"File", "Edit", "View"};
    for (int i = 0; i < 3; ++i)
      if (button(context, mk(commands.ent(), i), ComponentConfig{}.with_size({pixels(60 * s), pixels(36 * s)})
          .with_label(command_names[i]).with_font("AtkinsonMock", pixels(18 * s)).with_alignment(TextAlignment::Center)
          .with_custom_text_color(theme.font).with_button_variant(ButtonVariant::Ghost).with_debug_name("toolbar_" + std::to_string(i))))
        toolbar_feedback = std::string(command_names[i]) + " command activated.";
    if (button(context, mk(toolbar.ent(), 1), ComponentConfig{}.with_size({pixels(96 * s), pixels(36 * s)})
        .with_label("Settings").with_font("AtkinsonMock", pixels(18 * s)).with_alignment(TextAlignment::Center)
        .with_custom_text_color(theme.font).with_button_variant(ButtonVariant::Outline).with_debug_name("toolbar_settings")))
      toolbar_feedback = "Trailing Settings action activated.";
    label(root.ent(), 83, toolbar_feedback, 760, 315, 400, 48, 18);
    // Card row
    label(root.ent(), 84, "Four equal cards", 760, 380, 400, 31, 25, true);
    label(root.ent(), 85, "Each 22% wide / SpaceAround / gap 0", 760, 415, 400, 25, 17);
    auto cards = hstack(context, mk(root.ent(), 86), box(760, 452, 400, 64)
        .with_justify_content(JustifyContent::SpaceAround).with_align_items(AlignItems::Center).with_debug_name("cards"));
    const std::array<afterhours::Color, 4> card_colors{teal, green, orange, purple};
    for (int i = 0; i < 4; ++i)
      div(context, mk(cards.ent(), i), ComponentConfig{}.with_size({percent(.22f), percent(.85f)})
          .with_label(std::string(1, static_cast<char>('A' + i))).with_alignment(TextAlignment::Center)
          .with_font("AtkinsonMock", pixels(24 * s)).with_custom_background(card_colors[i])
          .with_custom_text_color({0, 0, 0, 255}).with_roundness(.12f).with_skip_grid_snap(true)
          .with_debug_name("card_" + std::to_string(i)));
    // == Row 4: Dashboard cards spanning full width ==
    label(root.ent(), 90, "Six expanding cards", 0, 553, 1160, 31, 25, true);
    label(root.ent(), 91, "expand() divides remaining width equally / five fixed 40px gaps / card width 160px at 720p", 0, 590, 1160, 26, 18);
    auto dashboard = hstack(context, mk(root.ent(), 92), box(0, 628, 1160, 60)
        .with_gap(pixels(40 * s)).with_align_items(AlignItems::Center).with_debug_name("dashboard"));
    const std::array<afterhours::Color, 6> dash_colors{blue, pink, green, orange, purple, teal};
    const std::array<const char *, 6> names{"Users", "Revenue", "Orders", "Growth", "Active", "Churn"};
    const std::array<const char *, 6> values{"1,248", "$8,420", "326", "+12.4%", "842", "2.1%"};
    for (int i = 0; i < 6; ++i) {
      auto card = div(context, mk(dashboard.ent(), i), ComponentConfig{}.with_size({expand(), percent(1.f)})
          .with_custom_background(dash_colors[i]).with_roundness(.10f).with_skip_grid_snap(true)
          .with_debug_name("metric_" + std::to_string(i)));
      div(context, mk(card.ent(), 0), box(10, 5, 140, 22).with_label(names[i])
          .with_font("AtkinsonMock", pixels(17 * s)).with_alignment(TextAlignment::Left)
          .with_custom_text_color({0, 0, 0, 255}).with_ignore_pointer_events());
      div(context, mk(card.ent(), 1), box(10, 28, 140, 28).with_label(values[i])
          .with_font("AtkinsonMockBold", pixels(23 * s)).with_alignment(TextAlignment::Left)
          .with_custom_text_color({0, 0, 0, 255}).with_ignore_pointer_events());
    }
    label(root.ent(), 93, "Sample values. Colors identify cards; they do not encode success, failure or a trend. Dimensions use 720p design pixels.",
          0, 695, 1160, 22, 17);
  }
};

REGISTER_EXAMPLE_SCREEN(hstack_showcase, "Component Galleries",
                        "hstack() primitive showcase", HStackShowcase)
