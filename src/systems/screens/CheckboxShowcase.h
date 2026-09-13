#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <algorithm>
#include <array>
#include <bitset>
#include <string>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct CheckboxShowcase : ScreenSystem<UIContext<InputAction>> {
  // Checkbox with label values
  bool labeled_primary = true;
  bool labeled_secondary = false;
  bool labeled_accent = true;

  // No-label checkbox values
  bool no_label_1 = true;
  bool no_label_2 = false;
  bool no_label_3 = true;
  bool no_label_4 = false;

  // Disabled checkbox values
  bool disabled_checked = true;
  bool disabled_unchecked = false;

  // Checkbox group values
  std::bitset<4> options_group{0b0101};
  std::bitset<3> min_max_group{0b001};
  std::string last_change = "none";

  void reset() {
    labeled_primary = labeled_accent = no_label_1 = no_label_3 = disabled_checked = true;
    labeled_secondary = no_label_2 = no_label_4 = disabled_unchecked = false;
    options_group = 0b0101;
    min_max_group = 0b001;
    last_change = "Demo defaults restored";
  }

  static void draw_mark(RectangleType r, bool checked, afterhours::Color color) {
    const float stroke = r.width / 11.f;
    raylib::DrawRectangleLinesEx(r, stroke, color);
    if (!checked) return;
    raylib::DrawLineEx({r.x + r.width * .20f, r.y + r.height * .50f},
                      {r.x + r.width * .43f, r.y + r.height * .73f}, stroke, color);
    raylib::DrawLineEx({r.x + r.width * .43f, r.y + r.height * .73f},
                      {r.x + r.width * .82f, r.y + r.height * .25f}, stroke, color);
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    const auto theme = afterhours::ui::theme_presets::neon_dark();
    context.set_theme(theme);
    context.scaling_mode = ScalingMode::Proportional;
    const float s = std::min(context.screen_height / 720.f, context.screen_width / 1280.f);
    if (s <= 0.f) return;
    const float left = (context.screen_width - 1144 * s) / 2.f;
    const float top = (context.screen_height - 720 * s) / 2.f;
    const afterhours::Color ink{236, 242, 251, 255};
    const afterhours::Color muted{182, 197, 219, 255};
    const auto box = [s](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, y * s).with_background(Theme::Usage::None).with_corner_radius(0);
    };
    // Root - full screen centering container
    auto root = div(context, mk(entity), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_background(Theme::Usage::Background).with_corner_radius(0).with_debug_name("bg"));
    auto content = div(context, mk(root.ent(), 0), ComponentConfig{}
        .with_size({pixels(1144 * s), pixels(720 * s)}).with_absolute_position(left, top)
        .with_background(Theme::Usage::None).with_corner_radius(0).with_debug_name("card"));
    int id = 0;
    const auto label = [&](afterhours::Entity &parent, const std::string &text,
                           float x, float y, float w, float h, float size,
                           const std::string &name = "", bool subdued = false) {
      return div(context, mk(parent, id++), box(x, y, w, h).with_label(text)
          .with_font("AtkinsonMock", pixels(size * s)).with_custom_text_color(subdued ? muted : ink)
          .with_alignment(TextAlignment::Left).with_ignore_pointer_events().with_debug_name(name));
    };
    const auto mark = [&](afterhours::Entity &parent, float x, float y, bool checked, bool disabled) {
      div(context, mk(parent, id++), box(x, y, 22, 22).with_ignore_pointer_events()
          .with_on_draw_fg([checked, color = disabled ? afterhours::Color{139, 154, 177, 255} : ink](RectangleType r) {
            draw_mark(r, checked, color);
          }));
    };
    const auto check_row = [&](afterhours::Entity &parent, bool &value, const std::string &text,
                               float y, afterhours::Color background, const std::string &name,
                               bool disabled = false, const std::string &lock_reason = "") {
      auto pair = mk(parent, id++);
      auto &control = deref(pair).first;
      if (control.has<HasCheckboxState>()) control.get<HasCheckboxState>().on = value;
      const auto changed = checkbox(context, pair, value,
          box(20, y, 516, 44).with_label("").with_custom_background(background)
              .with_checkbox_indicators("", "").with_corner_radius(6 * s)
              .with_font("AtkinsonMock", pixels(23 * s)).with_disabled(disabled)
              .with_click_activation(ClickActivationMode::Release).with_debug_name(name));
      mark(parent, 34, y + 11, value, disabled);
      label(parent, text, 72, y + 7, 334, 30, 23, name + "_label");
      label(parent, disabled ? (lock_reason.empty() ? "Disabled" : lock_reason) : (value ? "Checked" : "Unchecked"),
            422, y + 10, 104, 26, 17, name + "_state", true);
      if (changed) last_change = text + (value ? " checked" : " unchecked");
      return static_cast<bool>(changed);
    };

    auto header = div(context, mk(content.ent(), id++), box(0, 24, 1144, 88)
        .with_custom_background({30, 42, 62, 255}).with_corner_radius(12 * s));
    // Title
    label(header.ent(), "Checkbox Components", 20, 8, 850, 40, 33, "checkbox_title");
    label(header.ent(), "Neon Dark / square marks, independent states, and group limits", 20, 52, 740, 26, 20, "", true);
    if (button(context, mk(header.ent(), id++), box(914, 10, 210, 36)
        .with_label("Reset demo").with_font("AtkinsonMock", pixels(21 * s))
        .with_custom_background({53, 70, 97, 255}).with_custom_text_color(ink)
        .with_corner_radius(7 * s).with_debug_name("checkbox_reset"))) reset();
    label(header.ent(), "Last: " + last_change, 774, 52, 350, 26, 18, "checkbox_feedback", true);

    // ========== LEFT COLUMN ==========
    auto left_col = div(context, mk(content.ent(), id++), box(0, 132, 556, 568)
        .with_background(Theme::Usage::Surface).with_corner_radius(12 * s).with_debug_name("left_col"));
    label(left_col.ent(), "Individual controls", 20, 12, 516, 33, 27);
    label(left_col.ent(), "Theme roles", 20, 54, 300, 27, 23);
    if (button(context, mk(left_col.ent(), id++), box(366, 52, 170, 30)
        .with_label("Match checked").with_font("AtkinsonMock", pixels(18 * s))
        .with_custom_background({53, 70, 97, 255}).with_custom_text_color(ink)
        .with_corner_radius(6 * s).with_debug_name("checkbox_match_roles"))) {
      labeled_primary = labeled_secondary = labeled_accent = true;
      last_change = "Role states matched";
    }
    check_row(left_col.ent(), labeled_primary, "Primary", 90, theme.primary, "labeled_primary");
    check_row(left_col.ent(), labeled_secondary, "Secondary", 142, theme.secondary, "labeled_secondary");
    check_row(left_col.ent(), labeled_accent, "Accent", 194, theme.accent, "labeled_accent");
    label(left_col.ent(), "Box Only / 44 x 44 design px", 20, 250, 516, 28, 23, "box_only_heading");
    std::array<bool *, 4> box_values{&no_label_1, &no_label_2, &no_label_3, &no_label_4};
    const std::array<Theme::Usage, 4> box_roles{Theme::Usage::Primary, Theme::Usage::Secondary,
        Theme::Usage::Accent, Theme::Usage::Primary};
    const std::array<const char *, 4> role_names{"Primary", "Secondary", "Accent", "Primary"};
    for (size_t i = 0; i < box_values.size(); ++i) {
      const float x = 20 + static_cast<float>(i) * 129;
      label(left_col.ent(), std::to_string(i + 1) + " / " + role_names[i], x, 282, 120, 23, 17, "", true);
      if (primitive::toggle_button(context, mk(left_col.ent(), id++), box(x + 38, 311, 44, 44)
          .with_label("").with_background(box_roles[i]).with_corner_radius(3 * s)
          .with_click_activation(ClickActivationMode::Release).with_debug_name("nl_option_" + std::to_string(i + 1)), *box_values[i]))
        last_change = "Box " + std::to_string(i + 1) + (*box_values[i] ? " checked" : " unchecked");
      mark(left_col.ent(), x + 49, 322, *box_values[i], false);
      label(left_col.ent(), *box_values[i] ? "Checked" : "Unchecked", x, 363, 120, 23, 18,
            "nl_state_" + std::to_string(i + 1), true);
    }
    label(left_col.ent(), "Disabled", 20, 395, 516, 28, 23, "disabled_heading");
    const auto disabled_bg = afterhours::colors::darken(theme.primary, .7f);
    check_row(left_col.ent(), disabled_checked, "Checked specimen", 426, disabled_bg, "disabled_checked", true);
    check_row(left_col.ent(), disabled_unchecked, "Unchecked specimen", 478, disabled_bg, "disabled_unchecked", true);
    label(left_col.ent(), std::string("Disabled values: ") + (disabled_checked ? "checked" : "unchecked") + " / " +
          (disabled_unchecked ? "checked" : "unchecked"), 20, 535, 516, 25, 19, "disabled_values", true);

    // ========== RIGHT COLUMN ==========
    auto right_col = div(context, mk(content.ent(), id++), box(588, 132, 556, 568)
        .with_background(Theme::Usage::Surface).with_corner_radius(12 * s).with_debug_name("right_col"));
    label(right_col.ent(), "Grouped preferences", 20, 12, 516, 33, 27);
    label(right_col.ent(), "News preferences", 20, 54, 344, 27, 23);
    auto multi_count = div(context, mk(right_col.ent(), id++), box(374, 51, 162, 31)
        .with_custom_background({40, 57, 80, 255}).with_corner_radius(6 * s));
    label(multi_count.ent(), std::to_string(options_group.count()) + " of 4 selected", 8, 2, 146, 27, 20, "options_count");
    const std::array<const char *, 4> option_names{"Product updates", "Security alerts", "Tips and guides", "Event reminders"};
    const std::array<const char *, 4> option_debug{"opt_a", "opt_b", "opt_c", "opt_d"};
    for (size_t i = 0; i < option_names.size(); ++i) {
      bool value = options_group.test(i);
      if (check_row(right_col.ent(), value, option_names[i], 90 + static_cast<float>(i) * 52,
                    theme.primary, option_debug[i])) options_group.set(i, value);
    }
    label(right_col.ent(), "Delivery channels", 20, 314, 330, 28, 23);
    auto limit_count = div(context, mk(right_col.ent(), id++), box(374, 311, 162, 31)
        .with_custom_background({40, 57, 80, 255}).with_corner_radius(6 * s));
    label(limit_count.ent(), std::to_string(min_max_group.count()) + " selected", 8, 2, 146, 27, 20, "channels_count");
    label(right_col.ent(), "Min 1, max 2 / Secondary fill marks this group", 20, 346, 516, 26, 19, "channel_rules", true);
    const std::array<const char *, 3> channel_names{"Email", "Push notifications", "SMS"};
    for (size_t i = 0; i < channel_names.size(); ++i) {
      bool value = min_max_group.test(i);
      const auto count = min_max_group.count();
      const bool disabled = (value && count <= 1) || (!value && count >= 2);
      const auto background = disabled ? afterhours::colors::darken(theme.secondary, .7f) : theme.secondary;
      if (check_row(right_col.ent(), value, channel_names[i], 378 + static_cast<float>(i) * 52,
                    background, "ch_" + std::to_string(i + 1), disabled, value ? "Min 1" : "Max 2"))
        min_max_group.set(i, value);
    }
    label(right_col.ent(), min_max_group.count() == 1 ? "Keep one channel selected. Add another to unlock it."
                                                     : "Maximum reached. Uncheck a channel to choose another.",
          20, 534, 516, 27, 18, "channel_reason", true);
  }
};

REGISTER_EXAMPLE_SCREEN(checkboxes, "Component Galleries",
                        "All checkbox component types and variations",
                        CheckboxShowcase)
