#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Configurable radio button styling parameters
struct RadioButtonConfig {
  float visual_circle_size =
      28.0f;                   // Outer ring size (larger for better visibility)
  float dot_size = 18.0f;      // Inner dot size (larger for clearer indicator)
  float border_width = 3.0f;   // Border thickness
  float group_spacing = 16.0f; // Extra spacing between groups
  bool use_accent_color = true; // Use accent color for selected indicator
};

// Custom radio group with configurable styling
// This allows per-showcase customization without modifying vendor code
template <size_t N>
ElementResult custom_radio_group(
    HasUIContext auto &ctx, EntityParent ep_pair,
    const std::array<std::string_view, N> &labels, size_t &selected_index,
    ComponentConfig config,
    const RadioButtonConfig &radio_config = RadioButtonConfig(), float scale = 1.f) {
  bool changed = false;

  // Wrap in a tray for single-tab-stop, arrow-key navigation
  auto t =
      tray(ctx, ep_pair,
           ComponentConfig{}
               .with_size(config.size.x_axis.value > 0
                              ? ComponentSize{config.size.x_axis, children()}
                              : ComponentSize{percent(1.0f), children()})
               .with_flex_direction(FlexDirection::Column)
               .with_debug_name(config.debug_name + "_tray"));

  constexpr float touch_target_sz = MIN_TOUCH_TARGET;
  const float visual_circle_sz = radio_config.visual_circle_size;
  const float dot_sz = radio_config.dot_size;
  const float border_w = radio_config.border_width;

  for (size_t i = 0; i < N; ++i) {
    bool is_selected = (i == selected_index);

    // Row button - transparent, for click handling
    auto row_size = config.size;
    if (row_size.y_axis.dim == Dim::Pixels &&
        row_size.y_axis.value < touch_target_sz) {
      row_size.y_axis = pixels(touch_target_sz);
    }
    auto row = button(
        ctx, mk(t.ent(), 100 + static_cast<int>(i)),
        ComponentConfig{}
            .with_size(row_size)
            .with_label("")
            .with_background(Theme::Usage::None)
            .with_flex_direction(FlexDirection::Row)
            .with_align_items(AlignItems::Center)
            .with_padding(Padding{.left = pixels(16 * scale)})
            .with_debug_name(fmt::format("{}_{}", config.debug_name, i)));

    if (row) {
      selected_index = i;
      changed = true;
    }

    // Outer circle ring - use accent color when selected for better visibility
    afterhours::Color ring_color =
        is_selected ? ctx.theme.accent : ctx.theme.font_muted;
    // Make the ring thicker when selected for additional emphasis
    float effective_border = is_selected ? border_w + 1.0f : border_w;

    auto ring = div(ctx, mk(row.ent(), 0),
                    ComponentConfig{}
                        .with_size(ComponentSize{pixels(visual_circle_sz),
                                                 pixels(visual_circle_sz)})
                        .with_background(Theme::Usage::Background)
                        .with_border(ring_color, effective_border)
                        .with_rounded_corners(RoundedCorners().all_round())
                        .with_roundness(1.0f)
                        .with_margin(Margin{.right = pixels(16 * scale)})
                        .with_skip_tabbing(true)
                        .with_ignore_pointer_events().with_debug_name(fmt::format("{}_ring_{}", config.debug_name, i)));

    // Inner filled dot when selected - larger and with accent color
    if (is_selected) {
      float offset = (visual_circle_sz - dot_sz) / 2.0f;
      div(ctx, mk(ring.ent(), 0),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(dot_sz), pixels(dot_sz)})
              .with_absolute_position(offset, offset)
              .with_background(Theme::Usage::Accent)
              .with_rounded_corners(RoundedCorners().all_round())
              .with_roundness(1.0f)
              .with_skip_tabbing(true)
              .with_ignore_pointer_events().with_debug_name(fmt::format("{}_dot_{}", config.debug_name, i)));
    }

    // Label - positioned after circle
    auto label_ent =
        div(ctx, mk(row.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(198 * scale), row_size.y_axis})
                .with_label(std::string(labels[i]))
                .with_font(config.font_name, config.font_size)
                .with_custom_text_color(ctx.theme.font)
                .with_skip_tabbing(true)
                .with_ignore_pointer_events().with_debug_name(fmt::format("{}_label_{}", config.debug_name, i)));

    // Force left alignment
    if (label_ent.ent().template has<HasLabel>()) {
      label_ent.ent().template get<HasLabel>().set_alignment(
          TextAlignment::Left);
    }
  }

  return {changed, t.ent(), static_cast<int>(selected_index)};
}

struct RadioGroupShowcase : ScreenSystem<UIContext<InputAction>> {
  size_t fruit_idx = 1;
  size_t size_idx = 0;
  size_t color_idx = 2;
  const std::array<std::string_view, 3> fruit_options{"Apple", "Orange", "Watermelon"};
  const std::array<std::string_view, 3> size_options{"Small", "Medium", "Large"};
  const std::array<std::string_view, 4> color_options{"Red", "Green", "Blue", "Yellow"};

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context,
                     float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();
    context.scaling_mode = ScalingMode::Proportional;
    const float s = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const afterhours::Color ink{234, 239, 249, 255}, muted{176, 191, 212, 255};
    const auto box = [s](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, y * s).with_corner_radius(0);
    };
    div(context, mk(entity, 0), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_absolute_position(0, 0).with_background(Theme::Usage::Background).with_corner_radius(0));
    auto root = div(context, mk(entity, 1), ComponentConfig{}
        .with_size({pixels(1160 * s), pixels(628 * s)})
        .with_absolute_position((context.screen_width - 1160 * s) / 2, (context.screen_height - 628 * s) / 2)
        .with_background(Theme::Usage::None).with_debug_name("radio_bg"));
    const auto label = [&](afterhours::Entity &p, int id, const std::string &text,
                           float x, float y, float w, float h, float size, bool dim = false) {
      return div(context, mk(p, id), box(x, y, w, h).with_label(text)
          .with_font("AtkinsonMock", pixels(size * s)).with_custom_text_color(dim ? muted : ink)
          .with_alignment(TextAlignment::Left).with_background(Theme::Usage::None).with_ignore_pointer_events());
    };
    label(root.ent(), 0, "Radio Button Groups", 16, 0, 1128, 44, 32);
    label(root.ent(), 1, "Choose one value in each independent group.", 16, 50, 1128, 30, 21, true);
    const RadioButtonConfig style{.visual_circle_size = 28 * s, .dot_size = 18 * s,
        .border_width = 3 * s, .group_spacing = 20 * s, .use_accent_color = true};
    const auto group = [&](int id, float x, const std::string &title, const auto &options,
                           size_t &selected, size_t initial, const std::string &name) {
      auto panel = div(context, mk(root.ent(), id), box(x, 104, 368, 380)
          .with_background(Theme::Usage::Surface).with_border({71, 84, 103, 255}, 1)
          .with_corner_radius(10 * s).with_debug_name(name + "_panel"));
      label(panel.ent(), 0, title, 16, 12, 336, 32, 24);
      label(panel.ent(), 1, std::to_string(options.size()) + " options / independent choice", 16, 48, 336, 26, 17, true);
      auto rows = custom_radio_group(context, mk(panel.ent(), 2), options, selected,
          ComponentConfig{}.with_size({pixels(336 * s), pixels(46 * s)})
              .with_font("AtkinsonMock", pixels(21 * s)).with_debug_name(name), style, s);
      rows.cmp().absolute = true;
      rows.cmp().absolute_pos_x = 16 * s;
      rows.cmp().absolute_pos_y = 80 * s;
      label(panel.ent(), 3, "Selected: " + std::string(options[selected]), 16, 304, 336, 30, 21);
      label(panel.ent(), 4, std::string("Fixture default: ") + std::string(options[initial]), 16, 344, 336, 24, 17, true);
      return panel;
    };
    group(10, 0, "Favorite fruit", fruit_options, fruit_idx, 1, "fruit_radios");
    group(11, 396, "Item size", size_options, size_idx, 0, "size_radios");
    auto colors = group(12, 792, "Color", color_options, color_idx, 2, "color_radios");
    const std::array<afterhours::Color, 4> swatches{{{230, 100, 112, 255}, {96, 190, 129, 255}, {94, 145, 239, 255}, {234, 198, 76, 255}}};
    for (int i = 0; i < 4; ++i)
      div(context, mk(colors.ent(), 10 + i), box(308, 80 + (static_cast<float>(i) + 0.5f) * std::max(46.f, MIN_TOUCH_TARGET / s) - 9, 18, 18)
          .with_custom_background(swatches[static_cast<size_t>(i)]).with_border(ink, 1)
          .with_corner_radius(3 * s).with_ignore_pointer_events());
    label(root.ent(), 20, "Watermelon tests a longer label.", 16, 492, 560, 26, 18, true);
    label(root.ent(), 21, "Ring 28px / dot 18px at 720p", 660, 492, 484, 26, 18, true);
    div(context, mk(root.ent(), 22), box(0, 530, 1160, 40)
        .with_custom_background({35, 45, 63, 255}).with_corner_radius(8 * s));
    label(root.ent(), 23, "Fruit: " + std::string(fruit_options[fruit_idx]), 16, 534, 352, 30, 20);
    label(root.ent(), 24, "Size: " + std::string(size_options[size_idx]), 412, 534, 352, 30, 20);
    label(root.ent(), 25, "Color: " + std::string(color_options[color_idx]), 808, 534, 336, 30, 20);
    label(root.ent(), 26, "Tab enters each group. Arrows move focus; Enter selects the focused option.", 16, 588, 1128, 32, 19, true);
  }
};

REGISTER_EXAMPLE_SCREEN(radio_buttons, "Component Galleries",
                        "Radio button group with single-select behavior",
                        RadioGroupShowcase)
