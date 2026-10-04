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

// imm::color_swatch_*(): three picker shapes for the same theme tokens
// (hanabi #58, mock in mocks/color_swatch.html), side by side in one
// card so the shapes compare directly. C expands channel sliders
// inline and writes through. B opens presets in a popover, previews
// the hovered preset without saving, and publishes on click. A's
// column holds its anchor only until the SV-square editor lands as
// its own commit. Every live variant edits the same accent and
// find-highlight values.
struct ExampleColorSwatch : ScreenSystem<UIContext<InputAction>> {
  afterhours::Color accent{60, 101, 156, 255};
  afterhours::Color highlight{255, 213, 79, 255};
  afterhours::Color border{107, 122, 145, 255};
  // Accent open by default: a screenshot baseline cannot click, and a closed
  // swatch is one button with nothing to review.
  bool accent_open = true;
  bool highlight_open = false;
  bool border_open = false;
  // B starts closed: its popover is opened by click, the path PopoverLab
  // uses. Left open from the first frame it is already gone by the time
  // the E2E runner looks for its chips.
  bool accent_b_open = false;
  bool highlight_b_open = false;
  bool border_b_open = false;

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
    text("One value, three picker shapes. C writes through as you drag; "
         "B previews on hover and publishes on click.",
         48, 64, 1100, 28, 20, muted, "cs_subtitle");

    // ---- one card, three tinted columns ----
    div(context, mk(root.ent(), id++), box(48, 110, 1184, 500)
        .with_custom_background({26, 33, 46, 255})
        .with_corner_radius(12 * s).with_debug_name("cs_card"));
    const auto column = [&](float x, afterhours::Color tint,
                            const std::string &name) {
      auto panel = div(context, mk(root.ent(), id++), box(x, 126, 376, 468)
          .with_custom_background(tint)
          .with_corner_radius(10 * s).with_debug_name(name));
      return vstack(context, mk(panel.ent(), 0), ComponentConfig{}
          .with_size({percent(1.f), percent(1.f)})
          .with_padding(Padding::all(pixels(16 * s)))
          .with_transparent_bg());
    };

    // ---- C: inline editor ----
    auto body_c = column(64, {31, 44, 68, 255}, "cs_c_panel");
    const auto flow_label = [&](afterhours::Entity &body, int child,
                                const std::string &value, float h, float size,
                                afterhours::Color color,
                                const std::string &name = "",
                                bool bold = false) {
      div(context, mk(body, child),
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
    flow_label(body_c.ent(), 0, "C - inline editor", 30, 24, white,
               "cs_c_title", true);
    flow_label(body_c.ent(), 1,
               "Expands in place into H, S, V and A sliders plus a hex "
               "field. Only one field stays open at a time.",
               44, 17, muted);

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
    const bool was_accent_b_open = accent_b_open;
    const bool was_highlight_b_open = highlight_b_open;
    flow_label(body_c.ent(), 2, "Accent", 24, 19, white);
    color_swatch_inline(context, mk(body_c.ent(), 3), accent, accent_open,
                        swatch_cfg("cs_c_accent"));
    flow_label(body_c.ent(), 4, "Find highlight", 24, 19, white);
    color_swatch_inline(context, mk(body_c.ent(), 5), highlight,
                        highlight_open, swatch_cfg("cs_c_highlight"));
    flow_label(body_c.ent(), 6, "Border (locked)", 24, 19, muted);
    color_swatch_inline(context, mk(body_c.ent(), 7), border, border_open,
                        swatch_cfg("cs_c_border").with_disabled(true));

    // ---- B: preset grid popover ----
    auto body_b = column(452, {27, 50, 45, 255}, "cs_b_panel");
    flow_label(body_b.ent(), 0, "B - preset grid", 30, 24, white,
               "cs_b_title", true);
    flow_label(body_b.ent(), 1,
               "Hovering a preset previews it. Clicking publishes the "
               "pick and closes.",
               44, 17, muted);
    flow_label(body_b.ent(), 2, "Accent", 24, 19, white);
    auto accent_b_result = color_swatch_presets(
        context, mk(body_b.ent(), 3), accent, accent_b_open,
        afterhours::ui::default_color_presets(), swatch_cfg("cs_b_accent"));
    flow_label(body_b.ent(), 4, "Find highlight", 24, 19, white);
    auto highlight_b_result = color_swatch_presets(
        context, mk(body_b.ent(), 5), highlight, highlight_b_open,
        afterhours::ui::default_color_presets(),
        swatch_cfg("cs_b_highlight"));
    flow_label(body_b.ent(), 6, "Border (locked)", 24, 19, muted);
    color_swatch_presets(context, mk(body_b.ent(), 7), border, border_b_open,
                         afterhours::ui::default_color_presets(),
                         swatch_cfg("cs_b_border").with_disabled(true));

    // ---- A: popover editor, anchor only until its own commit ----
    auto body_a = column(840, {46, 34, 60, 255}, "cs_a_panel");
    flow_label(body_a.ent(), 0, "A - popover editor", 30, 24, white,
               "cs_a_title", true);
    flow_label(body_a.ent(), 1,
               "A saturation/value square with hue and alpha strips in "
               "a popover. Lands next as its own commit; only the "
               "anchor is here.",
               66, 17, muted);
    flow_label(body_a.ent(), 2, "Accent", 24, 19, muted);
    color_swatch_button(context, mk(body_a.ent(), 3), accent,
                        swatch_cfg("cs_a_accent").with_disabled(true));
    flow_label(body_a.ent(), 4, "Find highlight", 24, 19, muted);
    color_swatch_button(context, mk(body_a.ent(), 5), highlight,
                        swatch_cfg("cs_a_highlight").with_disabled(true));
    flow_label(body_a.ent(), 6,
               "It will edit these same values, so all three columns "
               "stay in step.",
               44, 17, muted);

    // The mock's mitigation for the slider wall and duplicate hex fields:
    // opening one field closes every other field. Each widget only toggles
    // its own bool; the accordion is a policy of the page that owns them.
    if (accent_open && !was_accent_open) {
      highlight_open = false;
      accent_b_open = false;
      highlight_b_open = false;
    } else if (highlight_open && !was_highlight_open) {
      accent_open = false;
      accent_b_open = false;
      highlight_b_open = false;
    } else if (accent_b_open && !was_accent_b_open) {
      accent_open = false;
      highlight_open = false;
      highlight_b_open = false;
    } else if (highlight_b_open && !was_highlight_b_open) {
      accent_open = false;
      highlight_open = false;
      accent_b_open = false;
    }

    // ---- live values, so a reader (and the E2E) can see write-through ----
    // The chips follow B's hover preview; the hex text stays on the
    // saved value until a click publishes. C edits the saved value
    // directly, so chip and text move together there.
    const auto shown_of = [&](ElementResult &result,
                              const afterhours::Color &saved) {
      if (result.ent().template has<HasColorSwatchState>()) {
        const auto &state =
            result.ent().template get<HasColorSwatchState>();
        if (state.hover_preview.has_value())
          return state.hover_preview.value();
      }
      return saved;
    };
    div(context, mk(root.ent(), id++), box(48, 626, 1184, 70)
        .with_custom_background({26, 33, 46, 255})
        .with_corner_radius(12 * s).with_debug_name("cs_preview_panel"));
    text("Current values", 72, 647, 200, 28, 22, white, "", true);
    const auto preview = [&](float x, const char *label,
                             const afterhours::Color &shown,
                             const afterhours::Color &saved,
                             const std::string &chip_name,
                             const std::string &text_name) {
      div(context, mk(root.ent(), id++), box(x, 641, 72, 40)
          .with_custom_background(shown)
          .with_border({112, 124, 143, 255}, s)
          .with_ignore_pointer_events().with_debug_name(chip_name));
      text(fmt::format("{} #{}", label,
                       afterhours::colors::to_hex_string(saved,
                                                         saved.a < 255)),
           x + 84, 649, 300, 28, 20, white, text_name);
    };
    preview(280, "Accent", shown_of(accent_b_result, accent), accent,
            "cs_preview_accent_chip", "cs_preview_accent");
    preview(700, "Find highlight", shown_of(highlight_b_result, highlight),
            highlight, "cs_preview_highlight_chip", "cs_preview_highlight");
  }
};

REGISTER_EXAMPLE_SCREEN(color_swatch_lab, "System Demos",
                        "color_swatch picker variants: inline, presets, "
                        "popover editor",
                        ExampleColorSwatch)
