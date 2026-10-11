#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/ui/grid.h>
#include <array>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Dynamic option sets: shrink resets selections; forget and the library errors.
struct StaleIndexLab : ScreenSystem<UIContext<InputAction>> {
  bool shrunk = false;
  size_t dropdown_idx = 2;
  size_t nav_idx = 2;
  size_t tab_idx = 2;
  size_t radio_idx = 2;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();
    context.scaling_mode = ScalingMode::Proportional;
    const float s = std::min(context.screen_width / 1280.f,
                             context.screen_height / 720.f);
    UIStylingDefaults::get().set_default_font("AtkinsonMock",
                                              pixels(20 * s));
    const auto box = [s](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, y * s).with_corner_radius(0);
    };
    auto root = div(context, mk(entity), ComponentConfig{}
        .with_size({screen_pct(1), screen_pct(1)})
        .with_background(Theme::Usage::Background).with_corner_radius(0)
        .with_debug_name("stale_root"));
    auto text = [&](int id, const std::string &label, float x, float y,
                    float w, float h, float size,
                    const std::string &name = "") {
      return div(context, mk(root.ent(), id), box(x, y, w, h)
          .with_label(label).with_font("AtkinsonMock", pixels(size * s))
          .with_custom_text_color(context.theme.font)
          .with_text_overflow(TextOverflow::Wrap)
          .with_ignore_pointer_events().with_debug_name(name));
    };

    text(0, "Stale index lab", 40, 16, 700, 44, 30, "stale_title");
    text(1, "Every selector starts on the last of three options. Shrink "
            "resets them to 0 first; forget that and the library errors.",
         40, 64, 1180, 28, 19);

    if (button(context, mk(root.ent(), 2), box(40, 108, 300, 44)
            .with_label(shrunk ? "Restore 3 options" : "Shrink to 1 option")
            .with_font("AtkinsonMock", pixels(19 * s))
            .with_background(Theme::Usage::Primary)
            .with_corner_radius(6 * s).with_debug_name("stale_shrink_btn"))) {
      shrunk = !shrunk;
      if (shrunk)
        dropdown_idx = nav_idx = tab_idx = radio_idx = 0;
    }
    if (button(context, mk(root.ent(), 3), box(360, 108, 240, 44)
            .with_label("Select last")
            .with_font("AtkinsonMock", pixels(19 * s))
            .with_background(Theme::Usage::Surface)
            .with_corner_radius(6 * s).with_debug_name("stale_last_btn")) &&
        !shrunk)
      dropdown_idx = nav_idx = tab_idx = radio_idx = 2;

    const std::vector<std::string> options =
        shrunk ? std::vector<std::string>{"Only"}
               : std::vector<std::string>{"Alpha", "Beta", "Gamma"};

    text(10, "Dropdown", 40, 176, 300, 26, 19);
    dropdown(context, mk(root.ent(), 11), options, dropdown_idx,
             box(40, 204, 300, 44)
                 .with_font("AtkinsonMock", pixels(19 * s))
                 .with_debug_name("stale_dropdown"));
    text(12, "Navigation bar", 380, 176, 380, 26, 19);
    navigation_bar(context, mk(root.ent(), 13), options, nav_idx,
                   box(380, 204, 380, 44)
                       .with_font("AtkinsonMock", pixels(19 * s))
                       .with_debug_name("stale_nav"));
    text(14, "Radio group", 800, 176, 300, 26, 19);
    auto radio_wrap = div(context, mk(root.ent(), 15),
                          box(800, 204, 300, 150)
                              .with_background(Theme::Usage::None)
                              .with_debug_name("stale_radio_wrap"));
    const auto radio_cfg = ComponentConfig{}
        .with_size(ComponentSize{pixels(300 * s), pixels(44 * s)})
        .with_font("AtkinsonMock", pixels(19 * s))
        .with_debug_name("stale_radio");
    if (shrunk) {
      static constexpr std::array<std::string_view, 1> one{"Only"};
      radio_group(context, mk(radio_wrap.ent(), 0), one, radio_idx,
                  radio_cfg);
    } else {
      static constexpr std::array<std::string_view, 3> three{"Alpha",
                                                             "Beta", "Gamma"};
      radio_group(context, mk(radio_wrap.ent(), 0), three, radio_idx,
                  radio_cfg);
    }

    text(20, "Tabs", 40, 276, 300, 26, 19);
    tab_container(context, mk(root.ent(), 21), options, tab_idx,
                  box(40, 304, 720, 44)
                      .with_font("AtkinsonMock", pixels(19 * s))
                      .with_debug_name("stale_tabs"));

    text(30, "Grid: 3 columns, 1 declared track width", 40, 380, 720, 26,
         19);
    auto tbl = grid(context, mk(root.ent(), 31),
                    GridConfig{}.with_rows(1).with_cols(3)
                        .with_col_widths({pixels(80 * s)})
                        .with_row_height(pixels(40 * s)),
                    box(40, 408, 720, 1)
                        .with_size({pixels(720 * s), children()})
                        .with_debug_name("stale_grid"));
    const auto missing = grid_track(tbl, 2);
    static constexpr std::array<const char *, 3> cells{"80px", "expand",
                                                       "expand"};
    for (int c = 0; c < 3; ++c)
      grid_cell(context, tbl, 0, c,
                ComponentConfig{}.with_label(cells[(size_t)c])
                    .with_font("AtkinsonMock", pixels(18 * s))
                    .with_alignment(TextAlignment::Center)
                    .with_custom_text_color(context.theme.font)
                    .with_debug_name(fmt::format("stale_cell_{}", c)));

    text(40, fmt::format("indices: dropdown {} nav {} tab {} radio {} "
                         "({} option{})",
                         dropdown_idx, nav_idx, tab_idx, radio_idx,
                         options.size(), options.size() == 1 ? "" : "s"),
         40, 484, 1100, 30, 21, "stale_readout");
    text(41, fmt::format("grid col 2 track: {} (no declared width)",
                         missing.x_axis.dim == Dim::Expand ? "expand"
                                                           : "fixed"),
         40, 522, 1100, 28, 19, "stale_grid_readout");
    text(42, "Backstop: a stale index hits log_error at the widget (an "
             "assert in WM builds), then clamps in release. Saved "
             "resolution indices fall back to the current resolution.",
         40, 566, 1180, 56, 17);
  }
};

REGISTER_EXAMPLE_SCREEN(stale_index_lab, "System Demos",
                        "stale selection indices clamp when options shrink",
                        StaleIndexLab)
