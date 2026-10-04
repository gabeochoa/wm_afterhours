#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/ui/color_swatch.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// imm::color_swatch_*(): three picker shapes for the same two theme tokens
// (hanabi #58, mock in mocks/color_swatch.html). C expands channel sliders
// inline; B and A get their own columns when they land. Every variant edits
// the same accent / find-highlight values and writes through immediately.
struct ExampleColorSwatch : ScreenSystem<UIContext<InputAction>> {
  afterhours::Color accent{60, 101, 156, 255};
  afterhours::Color highlight{255, 213, 79, 255};
  afterhours::Color border{107, 122, 145, 255};
  // Accent open by default: a screenshot baseline cannot click, and a closed
  // swatch is one button with nothing to review.
  bool accent_open = true;
  bool highlight_open = false;
  bool border_open = false;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    Theme theme;
    theme.background = {18, 23, 33, 255};
    theme.surface = {36, 43, 55, 255};
    // Primary is the slider handle colour; keep it clearly off the track
    // (secondary) or the four channel rows merge into one block.
    theme.primary = {94, 130, 182, 255};
    theme.secondary = {48, 60, 78, 255};
    theme.accent = {60, 101, 156, 255};
    theme.font = {240, 245, 253, 255};
    theme.font_muted = {165, 177, 193, 255};
    theme.focus = {165, 201, 248, 255};
    context.set_theme(theme);
    context.scaling_mode = ScalingMode::Proportional;
    const float s = std::min(context.screen_width / 1280.f,
                             context.screen_height / 720.f);
    const float left = (context.screen_width - 1280 * s) / 2;
    const float top = (context.screen_height - 720 * s) / 2;
    const auto white = afterhours::Color{240, 245, 253, 255};
    const auto muted = afterhours::Color{181, 195, 216, 255};
    UIStylingDefaults::get().set_default_font("AtkinsonMock",
                                              pixels(20 * s));

    auto root = div(context, mk(entity), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background(theme.background).with_corner_radius(0)
        .with_debug_name("cs_root"));
    auto box = [s, left, top](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(left + x * s, top + y * s)
          .with_transparent_bg().with_corner_radius(6 * s)
          .with_skip_grid_snap(true);
    };
    int id = 0;
    const auto text = [&](const std::string &value, float x, float y, float w,
                          float h, float size, afterhours::Color color,
                          const std::string &name = "", bool bold = false) {
      div(context, mk(root.ent(), id++), box(x, y, w, h).with_label(value)
          .with_font(bold ? "AtkinsonMockBold" : "AtkinsonMock",
                     pixels(size * s))
          .with_alignment(TextAlignment::Left).with_custom_text_color(color)
          .with_ignore_pointer_events().with_debug_name(name));
    };
    text("Color swatches", 48, 16, 700, 46, 36, white, "cs_title", true);
    text("One value, three picker shapes. Edits write through immediately.",
         48, 64, 900, 28, 20, muted, "cs_subtitle");

    // ---- C: inline editor ----
    auto panel_c = div(context, mk(root.ent(), id++), box(48, 110, 400, 500)
        .with_custom_background({26, 33, 46, 255})
        .with_corner_radius(12 * s).with_debug_name("cs_c_panel"));
    auto body_c = vstack(context, mk(panel_c.ent(), 0), ComponentConfig{}
        .with_size({percent(1.f), percent(1.f)})
        .with_padding(Padding::all(pixels(16 * s)))
        .with_transparent_bg().with_debug_name("cs_c_body"));
    const auto flow_label = [&](int child, const std::string &value, float h,
                                float size, afterhours::Color color,
                                const std::string &name = "",
                                bool bold = false) {
      div(context, mk(body_c.ent(), child),
          ComponentConfig{}.with_size({percent(1.f), pixels(h * s)})
              .with_label(value)
              .with_font(bold ? "AtkinsonMockBold" : "AtkinsonMock",
                         pixels(size * s))
              .with_alignment(TextAlignment::Left)
              .with_custom_text_color(color)
              .with_text_overflow(TextOverflow::Wrap)
              .with_transparent_bg().with_skip_tabbing(true)
              .with_debug_name(name));
    };
    flow_label(0, "C - inline editor", 32, 24, white, "cs_c_title", true);
    flow_label(1, "The swatch expands in place into H, S, V and A sliders "
                  "plus a hex field. Only one field stays open at a time.",
               58, 17, muted);

    const auto swatch_cfg = [s](const std::string &name) {
      // Surface on the button itself: the chip is the data on this page and
      // a primary-coloured button body outshouts it. A usage (not a custom
      // colour) does not inherit into the channel rows, so the sliders keep
      // their own track/handle theme colours.
      return ComponentConfig{}.with_size({percent(1.f), pixels(36 * s)})
          .with_font("AtkinsonMock", pixels(19 * s))
          .with_background(Theme::Usage::Surface)
          .with_custom_text_color(afterhours::Color{240, 245, 253, 255})
          .with_debug_name(name);
    };
    const bool was_accent_open = accent_open;
    const bool was_highlight_open = highlight_open;
    flow_label(2, "Accent", 28, 19, white);
    color_swatch_inline(context, mk(body_c.ent(), 3), accent, accent_open,
                        swatch_cfg("cs_c_accent"));
    flow_label(4, "Find highlight", 28, 19, white);
    color_swatch_inline(context, mk(body_c.ent(), 5), highlight,
                        highlight_open, swatch_cfg("cs_c_highlight"));
    flow_label(6, "Border (locked)", 28, 19, muted);
    color_swatch_inline(context, mk(body_c.ent(), 7), border, border_open,
                        swatch_cfg("cs_c_border").with_disabled(true));
    // The mock's mitigation for the slider wall: opening one field closes
    // the other. The widget only toggles its own bool; the accordion is a
    // policy of the page that owns the bools.
    if (accent_open && !was_accent_open)
      highlight_open = false;
    else if (highlight_open && !was_highlight_open)
      accent_open = false;

    // ---- live values, so a reader (and the E2E) can see write-through ----
    div(context, mk(root.ent(), id++), box(480, 110, 752, 220)
        .with_custom_background({26, 33, 46, 255})
        .with_corner_radius(12 * s).with_debug_name("cs_preview_panel"));
    text("Current values", 504, 126, 700, 32, 24, white, "", true);
    text("Read back from the value each frame: a slider drag or a hex edit",
         504, 162, 704, 24, 17, muted);
    text("shows up here without an apply step.", 504, 188, 704, 24, 17,
         muted);
    const auto preview = [&](float y, const char *label,
                             const afterhours::Color &value,
                             const std::string &chip_name,
                             const std::string &text_name) {
      div(context, mk(root.ent(), id++), box(504, y, 88, 40)
          .with_custom_background(value)
          .with_border({112, 124, 143, 255}, s)
          .with_ignore_pointer_events().with_debug_name(chip_name));
      text(fmt::format("{} #{}", label,
                       afterhours::colors::to_hex_string(value, value.a < 255)),
           608, y + 4, 600, 32, 22, white, text_name);
    };
    preview(228, "Accent", accent, "cs_preview_accent_chip",
            "cs_preview_accent");
    preview(280, "Find highlight", highlight, "cs_preview_highlight_chip",
            "cs_preview_highlight");
  }
};

REGISTER_EXAMPLE_SCREEN(color_swatch_lab, "System Demos",
                        "color_swatch picker variants: inline, presets, "
                        "popover editor",
                        ExampleColorSwatch)
