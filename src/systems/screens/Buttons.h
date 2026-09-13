#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <array>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ButtonsGallery : ScreenSystem<UIContext<InputAction>> {
  std::array<int, 14> click_counts{};
  int selected_group = -1; // Track which group button is selected
  bool show_guides = false;
  std::string last_button;
  int last_count = 0;

  void activated(size_t index, const std::string &name) {
    last_count = ++click_counts[index];
    last_button = name;
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    // Apply ocean navy theme with explicit specimen fonts
    const auto theme = afterhours::ui::theme_presets::ocean_navy();
    context.theme = theme;
    context.scaling_mode = ScalingMode::Proportional;
    const float s = std::min(context.screen_height / 720.f, context.screen_width / 1280.f);
    const float top = (context.screen_height / s - 720.f) / 2.f;
    const float left = (context.screen_width / s - 1144.f) / 2.f;
    const auto text = afterhours::Color{242, 247, 255, 255};
    const auto muted = afterhours::Color{185, 200, 222, 255};
    const auto line = afterhours::Color{81, 109, 146, 255};
    const auto box = [s, top](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, (y + top) * s).with_corner_radius(0)
          .with_background(Theme::Usage::None);
    };
    const auto hex = [](afterhours::Color color) {
      return fmt::format("#{:02X}{:02X}{:02X}", color.r, color.g, color.b);
    };
    auto root = div(context, mk(entity), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background(theme.background).with_corner_radius(0).with_debug_name("buttons_canvas"));
    int id = 0;
    const auto label = [&](const std::string &value, float x, float y, float w, float h,
                           float size, afterhours::Color color, const std::string &name = "") {
      return div(context, mk(root.ent(), id++), box(left + x, y, w, h).with_label(value)
          .with_font("AtkinsonMock", pixels(size * s)).with_custom_text_color(color)
          .with_alignment(TextAlignment::Left).with_ignore_pointer_events().with_debug_name(name));
    };
    const auto specimen = [&](float x, float y, float w, float h, const std::string &value,
                              const std::string &name, float size = 20.f) {
      return box(left + x, y, w, h).with_label(value).with_font("AtkinsonMock", pixels(size * s))
          .with_background(Theme::Usage::Primary).with_auto_text_color(true)
          .with_alignment(TextAlignment::Center).with_corner_radius(8 * s)
          .with_cursor(CursorType::Pointer).with_debug_name(name);
    };
    // Main container background - centered on screen with padding
    div(context, mk(root.ent(), id++), box(left, 24, 1144, 676)
        .with_custom_background(theme.background).with_corner_radius(12 * s).with_debug_name("buttons_bg"));
    // Title
    div(context, mk(root.ent(), id++), box(left, 24, 1144, 94)
        .with_custom_background(theme.surface).with_corner_radius(12 * s));
    label("Button Gallery", 20, 34, 1104, 42, 34, text, "buttons_title");
    label("Compare theme roles, dimensions, single selection, and custom colors or corners.",
          20, 80, 894, 28, 20, muted);
    if (button(context, mk(root.ent(), id++), specimen(950, 76, 174, 32,
        show_guides ? "Guides: on" : "Guides: off", "buttons_guides", 18))) show_guides = !show_guides;

    // Row 1: Basic button roles and availability
    div(context, mk(root.ent(), id++), box(left, 128, 1144, 116)
        .with_custom_background(theme.surface).with_corner_radius(10 * s).with_debug_name("row1_states"));
    label("Roles", 20, 140, 172, 30, 25, text);
    label("and availability", 20, 173, 172, 25, 17, muted);
    label(fmt::format("{:g} x {:g} px", 120 * s, 45 * s), 20, 206, 172, 23, 18, muted);
    const std::array<const char *, 3> roles{"Primary", "Accent", "Secondary"};
    const std::array<const char *, 3> role_names{"btn_normal", "btn_accent", "btn_secondary"};
    const std::array<Theme::Usage, 3> usages{Theme::Usage::Primary, Theme::Usage::Accent, Theme::Usage::Secondary};
    for (size_t i = 0; i < roles.size(); ++i) {
      const float x = 206 + static_cast<float>(i) * 222;
      if (button(context, mk(root.ent(), id++), specimen(x + 40, 158, 120, 45, roles[i], role_names[i])
          .with_background(usages[i]))) activated(i, roles[i]);
      label(hex(theme.from_usage(usages[i])), x, 209, 208, 24, 18, muted);
    }
    // Disabled button
    // Enhanced disabled styling: reduced opacity + muted border + custom text
    // color makes the disabled state more visually obvious per accessibility
    // guidelines.
    constexpr float disabled_opacity = .45f;
    div(context, mk(root.ent(), id++), box(left + 908, 154, 128, 53)
        .with_on_draw_fg([s, line](RectangleType r) {
          afterhours::draw_rectangle_rounded_lines_ex(r, .2f, 12, s, line);
        }).with_ignore_pointer_events().with_debug_name("disabled_reference"));
    button(context, mk(root.ent(), id++), specimen(912, 158, 120, 45, "Disabled", "btn_disabled")
        .with_custom_background({60, 65, 80, 255}).with_custom_text_color({140, 140, 150, 255})
        .with_auto_text_color(false).with_disabled(true).with_opacity(disabled_opacity)
        .with_border({100, 100, 110, 180}, 2 * s));
    label("Disabled / opacity 45%", 872, 209, 208, 24, 17, text, "disabled_caption");

    // Row 2: Different sizes - controls share a vertical center
    div(context, mk(root.ent(), id++), box(left, 254, 1144, 122)
        .with_custom_background(theme.surface).with_corner_radius(10 * s).with_debug_name("row2_sizes"));
    label("Sizes", 20, 269, 172, 30, 25, text);
    label("Same test label", 20, 302, 172, 24, 17, muted);
    label("Vertical center", 20, 338, 172, 23, 17, muted);
    div(context, mk(root.ent(), id++), box(left + 206, 301, 652, 30)
          .with_on_draw_fg([s, line, visible = show_guides](RectangleType r) {
            if (!visible) return;
            for (float x = r.x; x < r.x + r.width; x += 12 * s) {
              afterhours::draw_rectangle({x, r.y, std::min(6 * s, r.x + r.width - x), s}, line);
              afterhours::draw_rectangle({x, r.y + r.height, std::min(6 * s, r.x + r.width - x), s}, line);
            }
          }).with_ignore_pointer_events().with_debug_name("size_guides"));
    const std::array<const char *, 3> sizes{"Small", "Medium", "Large"};
    const std::array<const char *, 3> size_names{"btn_small", "btn_medium", "btn_large"};
    const std::array<float, 3> widths{90, 120, 180};
    const std::array<float, 3> heights{45, 45, 60};
    const std::array<float, 3> font_sizes{16, 20, 20};
    for (size_t i = 0; i < sizes.size(); ++i) {
      const float x = 206 + static_cast<float>(i) * 222;
      if (button(context, mk(root.ent(), id++), specimen(x + (200 - widths[i]) / 2, 301 - heights[i] / 2,
          widths[i], heights[i], "Action", size_names[i], font_sizes[i]))) activated(i + 3, sizes[i]);
      label(fmt::format("{} / {:g} x {:g}", sizes[i], widths[i] * s, heights[i] * s),
            x, 337, 208, 23, 18, text);
      label(fmt::format("Text {:g}px / padding 0", font_sizes[i] * s), x, 358, 208, 18, 15, muted);
    }
    label("Small and Medium", 872, 275, 238, 24, 18, muted);
    label("share a height.", 872, 300, 238, 24, 18, muted);
    label("Text inset: 5 screen px", 872, 335, 238, 25, 17, muted);

    // Row 3: Button group
    div(context, mk(root.ent(), id++), box(left, 386, 1144, 102)
        .with_custom_background(theme.surface).with_corner_radius(10 * s).with_debug_name("row3_group"));
    label("View selection", 20, 398, 184, 30, 23, text);
    label("Choose one view", 20, 435, 184, 26, 17, muted);
    const std::array<const char *, 4> group_labels{"Day", "Week", "Month", "Year"};
    for (size_t i = 0; i < group_labels.size(); ++i) {
      auto corners = RoundedCorners().all_sharp();
      if (i == 0) corners = RoundedCorners().sharp(TOP_RIGHT).sharp(BOTTOM_RIGHT);
      if (i == group_labels.size() - 1) corners = RoundedCorners().sharp(TOP_LEFT).sharp(BOTTOM_LEFT);
      const bool selected = selected_group == static_cast<int>(i);
      auto config = specimen(246 + static_cast<float>(i) * 90, 407, 90, 45, group_labels[i],
                             "btn_group_" + std::to_string(i))
          .with_rounded_corners(corners).with_background(selected ? Theme::Usage::Accent : Theme::Usage::Primary);
      if (selected) config.with_border({255, 255, 255, 220}, 2 * s);
      if (button(context, mk(root.ent(), id++), config)) {
        selected_group = static_cast<int>(i);
        activated(6 + i, group_labels[i]);
      }
    }
    label(fmt::format("Outer corners: {:g}px / inner: 0 / gap: 0", 8 * s), 226, 457, 432, 24, 17, muted);
    label("Selected view: " + std::string(selected_group < 0 ? "None" : group_labels[static_cast<size_t>(selected_group)]),
          690, 408, 414, 27, 23, text, "selected_view");
    label("Selection stays until another view is chosen.", 690, 444, 414, 26, 18, muted);

    // Row 4: Custom styled buttons
    div(context, mk(root.ent(), id++), box(left, 498, 1144, 136)
        .with_custom_background(theme.surface).with_corner_radius(10 * s).with_debug_name("row4_custom"));
    label("Custom", 20, 514, 172, 30, 25, text);
    label("Color / shape", 20, 550, 172, 25, 18, muted);
    const std::array<afterhours::Color, 2> custom_colors{{{255, 127, 80, 255}, {0, 100, 100, 255}}};
    const std::array<afterhours::Color, 2> custom_hover{{{255, 160, 120, 255}, {0, 140, 140, 255}}};
    const std::array<const char *, 2> custom_labels{"Coral", "Teal"};
    for (size_t i = 0; i < custom_colors.size(); ++i) {
      const float x = 206 + static_cast<float>(i) * 222;
      if (button(context, mk(root.ent(), id++), specimen(x + 40, 524, 120, 45, custom_labels[i],
          i == 0 ? "btn_coral" : "btn_teal").with_custom_background(custom_colors[i]).with_custom_hover_bg(custom_hover[i])))
        activated(10 + i, custom_labels[i]);
      const auto foreground = afterhours::colors::auto_text_color(custom_colors[i], theme.font, theme.darkfont);
      label(hex(custom_colors[i]), x, 577, 208, 23, 18, text);
      label("Auto text " + hex(foreground), x, 602, 208, 23, 17, muted);
    }
    div(context, mk(root.ent(), id++), box(left + 642, 513, 1, 108)
        .with_custom_background(line).with_ignore_pointer_events());
    if (button(context, mk(root.ent(), id++), specimen(690, 524, 120, 45, "Rounded", "btn_rounded")))
      activated(12, "Rounded");
    // Sharp corners button - demonstrates .disable_rounded_corners()
    if (button(context, mk(root.ent(), id++), specimen(912, 524, 120, 45, "Sharp", "btn_sharp")
        .disable_rounded_corners())) activated(13, "Sharp");
    label(fmt::format("Radius {:g}px", 8 * s), 650, 577, 208, 23, 18, text);
    label("Radius 0px", 872, 577, 208, 23, 18, text);
    label("Matched fill and dimensions", 650, 602, 430, 23, 17, muted);

    // Click counter display
    int total_clicks = 0;
    for (int count : click_counts) total_clicks += count;
    div(context, mk(root.ent(), id++), box(left, 644, 1144, 62)
        .with_custom_background({33, 59, 95, 255}).with_corner_radius(10 * s));
    const std::string counter_text = total_clicks == 0 ? "Try any button above" : fmt::format("{} pressed so far", total_clicks);
    label(counter_text, 20, 650, 362, 28, 23, text, "buttons_counter");
    label(last_button.empty() ? "Last activation: none" : fmt::format("Last activation: {} / {} {}", last_button, last_count, last_count == 1 ? "press" : "presses"),
          404, 650, 720, 28, 22, text, "buttons_feedback");
    label("Click or press Enter. Feedback reports the last button and its count; disabled controls do not activate.",
          20, 679, 1104, 23, 18, muted);
  }
};

REGISTER_EXAMPLE_SCREEN(buttons, "Component Galleries",
                        "Button component gallery with states and sizes",
                        ButtonsGallery)
