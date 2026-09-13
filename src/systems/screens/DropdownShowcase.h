#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <array>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct DropdownShowcase : ScreenSystem<UIContext<InputAction>> {
  // Selection indices for each dropdown
  size_t basic_idx = 0;
  size_t labeled_idx = 1;
  size_t many_idx = 0;
  size_t custom_indicator_idx = 0;
  size_t disabled_idx = 2;
  size_t styled_idx = 0;

  // Option data
  std::vector<std::string> basic_options = {"Alpha", "Beta", "Gamma"};
  std::vector<std::string> labeled_options = {"Dog", "Cat", "Bird", "Fish"};
  std::vector<std::string> many_options = {
      "January", "February", "March",     "April",   "May",      "June",
      "July",    "August",   "September", "October", "November", "December"};
  std::vector<std::string> indicator_options = {"Option A", "Option B",
                                                "Option C"};
  std::vector<std::string> disabled_options = {"Locked", "Sealed", "Frozen"};
  std::vector<std::string> styled_options = {"Ruby", "Emerald", "Sapphire",
                                             "Topaz"};

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();
    context.theme.primary = {47, 74, 109, 255};
    context.theme.surface = {27, 36, 51, 255};
    context.theme.font = {236, 241, 250, 255};
    context.theme.font_muted = {171, 188, 212, 255};
    context.scaling_mode = ScalingMode::Proportional;
    const float s = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const float top = (context.screen_height / s - 720.f) / 2;
    const float left = (context.screen_width / s - 1144.f) / 2;
    const auto ink = context.theme.font;
    const auto muted = context.theme.font_muted;
    const auto box = [s, top](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, (top + y) * s).with_corner_radius(0)
          .with_background(Theme::Usage::None);
    };

    // Root container
    auto root = div(context, mk(entity, 0), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background({17, 23, 34, 255}).with_corner_radius(0).with_debug_name("dropdown_bg"));
    const auto label = [&](int id, const std::string &text, float x, float y, float w, float h,
                           float size, afterhours::Color color, const std::string &name = "") {
      div(context, mk(root.ent(), id), box(left + x, y, w, h).with_label(text)
          .with_font("AtkinsonMock", pixels(size * s)).with_custom_text_color(color)
          .with_alignment(TextAlignment::Left).with_text_overflow(TextOverflow::Ellipsis)
          .with_ignore_pointer_events().with_debug_name(name));
    };

    // Title
    div(context, mk(root.ent(), 1), box(left, 24, 1144, 92)
        .with_custom_background({31, 42, 59, 255}).with_corner_radius(12 * s));
    label(2, "Dropdown Showcase", 20, 34, 1104, 42, 34, ink, "dropdown_title");
    label(3, "Choose a value. Compare labels, markers, disabled state, and custom styling.",
          20, 79, 1104, 28, 21, muted);

    // Content area - two rows of three columns
    const std::array<const char *, 6> titles{"Greek letter", "Pet / built-in label", "Month / 12 options",
        "Preset / custom markers", "Disabled example", "Gemstone / style comparison"};
    const std::array<const char *, 6> card_names{"col_basic", "col_labeled", "col_many",
        "col_indicators", "col_disabled", "col_styled"};
    for (size_t i = 0; i < titles.size(); ++i) {
      const float x = static_cast<float>(i % 3) * 388;
      const float y = i < 3 ? 140 : 350;
      div(context, mk(root.ent(), 10 + static_cast<int>(i)), box(left + x, y, 368, 188)
          .with_custom_background({27, 36, 51, 255}).with_corner_radius(10 * s)
          .with_debug_name(card_names[i]));
      label(20 + static_cast<int>(i), titles[i], x + 16, y + 12, 336, 32, 24, ink);
    }

    const auto specimen = [&](int id, const auto &options, size_t &selected, float x, float y, float w,
                              const std::string &name, bool disabled = false, bool custom = false,
                              bool labeled = false, bool markers = false) {
      auto cfg = box(left + x, y, w, 44).with_font("AtkinsonMock", pixels(22 * s))
          .with_custom_background(custom ? afterhours::Color{218, 180, 117, 255}
                                        : afterhours::Color{47, 74, 109, 255})
          .with_custom_text_color(custom ? afterhours::Color{30, 35, 44, 255} : ink)
          .with_corner_radius((custom ? 12.f : 6.f) * s).with_disabled(disabled)
          .with_dropdown_indicators(markers ? " [+]" : "", markers ? " [-]" : "")
          .with_debug_name(name).with_render_layer(10);
      if (custom) cfg.with_border({240, 223, 192, 255}, 2 * s);
      if (labeled) cfg.with_label("Pet");
      ElementResult result = dropdown(context, mk(root.ent(), id), options, selected, cfg);
      auto &outer = result.ent();
      const bool expanded = outer.get<HasDropdownState>().on;
      for (const auto child_id : result.cmp().children) {
        auto child = UICollectionHolder::getEntityForID(child_id);
        if (!child.valid() || !child.asE().has<UIComponent>()) continue;
        auto &node = child.asE();
        auto &cmp = node.get<UIComponent>();
        const auto debug = node.has<UIComponentDebug>() ? node.get<UIComponentDebug>().name() : "";
        if (debug == "dropdown_label") {
          cmp.desired[Axis::X] = pixels(84 * s);
          node.get<HasLabel>().alignment = TextAlignment::Left;
          node.get<HasLabel>().text_x_offset = 10 * s;
          node.get<afterhours::HasColor>().set({36, 47, 65, 255});
          continue;
        }
        if (debug != "option 1") continue;
        node.get<HasLabel>().alignment = TextAlignment::Left;
        node.get<HasLabel>().text_x_offset = 12 * s;
        if (labeled) cmp.desired[Axis::X] = pixels((w - 84) * s);
        node.addComponentIfMissing<UIComponentDebug>(name + "_trigger").set(name + "_trigger");
        if (disabled) node.removeComponentIfExists<HasClickListener>();
        if (markers) continue;
        node.addComponentIfMissing<HasOnDraw>().fg = [s, expanded, disabled, custom, ink, muted](RectangleType r) {
          const float center_x = r.x + r.width - 20 * s;
          const float center_y = r.y + r.height / 2;
          const float direction = expanded ? -1.f : 1.f;
          const auto color = disabled ? muted : custom ? afterhours::Color{30, 35, 44, 255} : ink;
          afterhours::draw_line_ex({center_x - 5 * s, center_y - direction * 2 * s}, {center_x, center_y + direction * 3 * s}, 2 * s, color);
          afterhours::draw_line_ex({center_x, center_y + direction * 3 * s}, {center_x + 5 * s, center_y - direction * 2 * s}, 2 * s, color);
        };
      }
    };
    specimen(40, basic_options, basic_idx, 16, 198, 336, "basic_dropdown");
    specimen(41, labeled_options, labeled_idx, 404, 198, 336, "labeled_dropdown", false, false, true);
    specimen(42, many_options, many_idx, 792, 198, 336, "many_dropdown");
    specimen(43, indicator_options, custom_indicator_idx, 16, 408, 336, "indicator_dropdown", false, false, false, true);
    specimen(44, disabled_options, disabled_idx, 404, 408, 336, "disabled_dropdown", true);
    specimen(45, styled_options, styled_idx, 792, 408, 162, "default_gem_dropdown");
    specimen(46, styled_options, styled_idx, 966, 408, 162, "styled_dropdown", false, true);
    label(60, "Basic selection / 3 options", 16, 253, 336, 28, 20, muted);
    label(61, "Value: " + basic_options[basic_idx], 16, 284, 336, 28, 20, ink);
    label(62, "Separate label and selectable value", 404, 253, 336, 28, 19, muted);
    label(63, "Value: " + labeled_options[labeled_idx], 404, 284, 336, 28, 20, ink);
    label(64, "12 rows at once / no scrolling", 792, 253, 336, 28, 20, muted);
    label(65, "Open to inspect screen-edge clamping", 792, 284, 336, 28, 18, ink);
    label(66, "[+] closed / [-] open", 16, 463, 336, 28, 20, muted);
    label(67, "Three selectable preset slots", 16, 494, 336, 28, 20, ink);
    label(68, "Frozen / unavailable", 404, 463, 336, 28, 21, ink);
    label(69, "Read-only example; selection is fixed", 404, 494, 336, 28, 19, muted);
    label(70, "Default", 792, 455, 162, 24, 18, muted);
    label(71, "Custom", 966, 455, 162, 24, 18, muted);
    label(72, "#DAB475 fill / #F0DFC0 border", 792, 484, 336, 24, 18, ink);
    label(73, "2px requested border / 12px corners", 792, 510, 336, 24, 17, muted);

    // Status bar
    div(context, mk(root.ent(), 80), box(left, 558, 1144, 138)
        .with_custom_background({30, 40, 57, 255}).with_corner_radius(10 * s));
    label(81, "Current selections", 20, 568, 720, 30, 22, ink);
    label(82, "Basic: " + basic_options[basic_idx], 20, 609, 244, 28, 21, ink);
    label(83, "Pet: " + labeled_options[labeled_idx], 276, 609, 244, 28, 21, ink);
    label(84, "Month: " + many_options[many_idx], 532, 609, 244, 28, 21, ink);
    label(85, "Preset: " + indicator_options[custom_indicator_idx], 20, 651, 244, 28, 21, ink);
    label(86, "Disabled: " + disabled_options[disabled_idx], 276, 651, 244, 28, 21, ink);
    label(87, "Gemstone: " + styled_options[styled_idx], 532, 651, 244, 28, 21, ink);
    label(88, "Placement: below, flip, then clamp", 808, 568, 320, 28, 18, muted);
    div(context, mk(root.ent(), 89), box(left + 824, 608, 286, 63)
        .with_ignore_pointer_events().with_on_draw_fg([s](RectangleType r) {
          const afterhours::Color line{138, 169, 210, 255};
          for (int i = 0; i < 2; ++i) {
            const float x = r.x + i * 160 * s;
            const float y = r.y + (i == 0 ? 0 : 42 * s);
            afterhours::draw_rectangle_outline({x, y, 105 * s, 18 * s}, line, s);
            const float popup_y = i == 0 ? y + 24 * s : y - 39 * s;
            afterhours::draw_rectangle_outline({x, popup_y, 105 * s, 33 * s}, line, s);
          }
        }).with_debug_name("dropdown_placement"));
  }
};

REGISTER_EXAMPLE_SCREEN(dropdowns, "Component Galleries",
                        "Dropdown variations: basic, labeled, many options, "
                        "custom indicators, disabled, styled",
                        DropdownShowcase)
