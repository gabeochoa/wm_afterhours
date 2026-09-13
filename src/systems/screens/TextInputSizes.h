#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/ui/text_input/text_input.h>
#include <algorithm>
#include <array>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct TextInputSizes : ScreenSystem<UIContext<InputAction>> {
  static constexpr int NUM_SIZES = 7;
  std::string values[NUM_SIZES] = {"", "", "", "", "", "", ""};
  bool midlines = false;
  bool height_derived = false;

  struct SizeSpec { const char *label; float height; };
  static constexpr SizeSpec specs[NUM_SIZES] = {
      {"24px", 24}, {"32px", 32}, {"40px", 40}, {"48px", 48},
      {"60px", 60}, {"80px", 80}, {"100px", 100}};

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = afterhours::ui::theme_presets::midnight();
    theme.secondary = {32, 34, 46, 255};
    context.theme = theme;
    context.scaling_mode = ScalingMode::Proportional;
    const float sx = std::min(1.f, context.screen_width / 1280.f);
    const float left = (context.screen_width - 1280 * sx) / 2;
    const float top = std::max(0.f, (context.screen_height - 720) / 2);
    const afterhours::Color border{119, 132, 161, 255}, panel{24, 29, 42, 255};
    const afterhours::Color muted{177, 194, 220, 255}, white{236, 242, 251, 255}, cyan{120, 212, 219, 255};
    UIStylingDefaults::get().set_default_font("AtkinsonMock", pixels(20));
    div(context, mk(entity, 900), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_absolute_position(0, 0).with_custom_background({12, 17, 27, 255}).with_corner_radius(0));
    auto root = div(context, mk(entity, 0), ComponentConfig{}
        .with_size({pixels(1280 * sx), pixels(720)})
        .with_absolute_position(left, top).with_transparent_bg().with_skip_grid_snap(true)
        .with_debug_name("sizes_root"));
    const auto box = [&](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * sx), pixels(h)})
          .with_absolute_position(x * sx, y).with_transparent_bg()
          .with_corner_radius(0).with_skip_grid_snap(true);
    };
    const auto text = [&](int id, std::string value, float x, float y, float w, float h,
                          float size, afterhours::Color color, const char *debug = "", bool bold = false) {
      div(context, mk(root.ent(), id), box(x, y, w, h).with_label(value)
          .with_font(bold ? "AtkinsonMockBold" : "AtkinsonMock", pixels(size * sx))
          .with_alignment(TextAlignment::Left).with_custom_text_color(color)
          .with_ignore_pointer_events().with_debug_name(debug));
    };
    text(1, "Text input sizes", 48, 20, 730, 44, 36, white, "sizes_title", true);
    text(2, "Exact field heights; native caret, selection and padding.", 48, 71, 730, 28, 19, muted);
    text(3, "Height", 48, 111, 96, 27, 19, white);
    text(4, fmt::format("Shared field width: {:.0f} px", 620 * sx), 164, 111, 620, 27, 19, white, "sizes_width");
    div(context, mk(root.ent(), 5), box(828, 110, 404, 510).with_custom_background(panel).with_corner_radius(10));
    int active_index = -1;
    float active_font = 0, active_padding_x = 0, active_padding_y = 0;
    size_t selected_bytes = 0;
    std::array<afterhours::EntityID, NUM_SIZES> field_ids{};
    float y = 150;
    for (int i = 0; i < NUM_SIZES; ++i) {
      const float h = specs[i].height;
      text(10 + i, fmt::format("{:.0f} px", h), 48, y, 96, h, 19, white);
      div(context, mk(root.ent(), 30 + i), box(146, y, 12, h).with_ignore_pointer_events()
          .with_on_draw_fg([cyan](RectangleType r) {
            afterhours::draw_rectangle({r.x + r.width / 2, r.y, 1, r.height}, cyan);
            afterhours::draw_rectangle({r.x, r.y, r.width, 1}, cyan);
            afterhours::draw_rectangle({r.x, r.y + r.height - 1, r.width, 1}, cyan);
          }));
      auto cfg = box(164, y, 620, h).with_placeholder("Type here...")
          .with_custom_background(panel).with_border(border, 1.f)
          .with_debug_name(std::string("input_") + specs[i].label);
      cfg.font_name = "AtkinsonMock";
      if (height_derived) cfg.with_autofit();
      auto field = text_input(context, mk(root.ent(), 50 + i), values[i], cfg);
      field_ids[static_cast<size_t>(i)] = field.ent().id;
      const auto &state = field.ent().get<afterhours::text_input::HasTextInputState>();
      if (state.is_focused) {
        active_index = i;
        selected_bytes = state.selection_end() - state.selection_start();
        for (auto child_id : field.cmp().children) {
          auto child = UICollectionHolder::getEntityForID(child_id);
          if (!child || !child.asE().has<InFocusCluster>()) continue;
          const auto &cmp = child.asE().get<UIComponent>();
          active_font = resolve_to_pixels(cmp.font_size, context.screen_height,
              cmp.resolved_scaling_mode, imm::ThemeDefaults::get().theme.ui_scale);
          active_padding_x = cmp.computed_padd[Axis::left];
          active_padding_y = cmp.computed_padd[Axis::top];
          break;
        }
      }
      if (midlines) {
        div(context, mk(root.ent(), 70 + i), box(790, y + h / 2, 24, 1).with_custom_background(cyan));
        div(context, mk(root.ent(), 80 + i), box(132, y + h / 2, 10, 1).with_custom_background(cyan));
      }
      y += h + 14;
    }
    const auto action = [&](int id, const char *label, float x, float width, const char *debug) {
      return button(context, mk(root.ent(), id), box(x, 24, width, 36).with_label(label)
          .with_font("AtkinsonMock", pixels(18)).with_custom_background(panel)
          .with_custom_text_color(white).with_corner_radius(6).with_debug_name(debug));
    };
    const bool fill = action(100, "Reset sample text", 828, 244, "sizes_sample");
    const bool clear = action(101, "Clear all", 1086, 146, "sizes_clear");
    if (fill || clear) {
      for (int i = 0; i < NUM_SIZES; ++i) {
        values[i] = fill ? "hello world" : "";
        auto field = UICollectionHolder::getEntityForID(field_ids[static_cast<size_t>(i)]);
        if (!field) continue;
        auto &state = field.asE().get<afterhours::text_input::HasTextInputState>();
        state.storage.clear();
        state.storage.insert(0, values[i]);
        state.cursor_position = values[i].size();
        state.clear_selection();
        state.scroll_offset_x = 0;
        state.undo_stack.clear();
        state.undo_index = 0;
      }
    }
    text(102, "Native sizing rules", 848, 127, 364, 34, 27, white, "", true);
    text(103, "Font: Atkinson", 848, 179, 364, 27, 20, white);
    text(104, height_derived ? "Font size = height × 0.5" : "Font: native type-scale default", 848, 215, 364, 27, 18, muted);
    text(105, "Requested inset X = height × 0.35", 848, 251, 364, 27, 17, muted);
    text(106, "Requested pad Y = height × 0.125", 848, 287, 364, 27, 17, muted);
    text(107, "Active field", 848, 337, 364, 32, 25, white, "", true);
    text(108, active_index >= 0 ? fmt::format("{:.0f} px field / font {:.1f} px", specs[active_index].height, active_font) : "Focus one field to inspect", 848, 382, 364, 28, 19, cyan, "sizes_active");
    text(109, fmt::format("Layout pad X {:.1f} / Y {:.1f} px", active_padding_x, active_padding_y), 848, 418, 364, 27, 18, muted, "sizes_padding");
    text(110, fmt::format("Selected bytes: {}", selected_bytes), 848, 454, 364, 27, 18, muted, "sizes_selection");
    text(111, "One focused field, one caret.", 848, 490, 364, 27, 17, muted);
    if (button(context, mk(root.ent(), 115), box(848, 522, 364, 34)
        .with_label(height_derived ? "Height-derived font: on" : "Height-derived font: off")
        .with_font("AtkinsonMock", pixels(18 * sx)).with_custom_background({63, 84, 114, 255})
        .with_custom_text_color(white).with_corner_radius(6).with_debug_name("sizes_font_mode"))) height_derived = !height_derived;
    if (button(context, mk(root.ent(), 112), box(848, 566, 364, 34)
        .with_label(midlines ? "Midline guides: on" : "Midline guides: off")
        .with_font("AtkinsonMock", pixels(18 * sx)).with_custom_background({63, 84, 114, 255})
        .with_custom_text_color(white).with_corner_radius(6).with_debug_name("sizes_guides"))) midlines = !midlines;
    text(113, "24 and 32 px are compact-size test cases. Row gutters are 14 px.", 48, 645, 1184, 28, 18, muted);
    text(114, "Load sample text, focus a field, then Ctrl/Cmd+A to compare selection heights.", 48, 682, 1184, 27, 18, muted);
  }
};

REGISTER_EXAMPLE_SCREEN(text_input_sizes, "Component Galleries",
                        "Text inputs at various heights showing cursor scaling",
                        TextInputSizes)
