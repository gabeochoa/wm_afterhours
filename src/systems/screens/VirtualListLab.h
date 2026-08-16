#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// virtual_list() over a count no one would build entities for. floatinghotel
// (10k commits) and hanabi (#23) both asked for this; the thing worth seeing on
// screen is that the scrollbar still describes the WHOLE list while only the
// visible window exists as entities.
struct VirtualListLab : ScreenSystem<UIContext<InputAction>> {
  static constexpr size_t kCount = 10000;
  static constexpr float kRowHeight = 26.f;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();

    const auto white = afterhours::Color{235, 240, 250, 255};
    const auto muted = afterhours::Color{150, 160, 185, 255};
    const auto odd = afterhours::Color{30, 36, 52, 255};
    const auto even = afterhours::Color{38, 46, 66, 255};

    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(1180), pixels(30)})
            .with_absolute_position(48.f, 20.f)
            .with_label(fmt::format("Virtual List - {} rows, only the visible "
                                    "window is built",
                                    kCount))
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(white)
            .with_debug_name("vl_title"));

    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(1180), pixels(24)})
            .with_absolute_position(48.f, 50.f)
            .with_label("scroll to page through all 10000; entity count stays "
                        "flat, and the bar spans the whole list")
            .with_alignment(TextAlignment::Left)
            .with_custom_text_color(muted)
            .with_font_size(13.f)
            .with_debug_name("vl_hint"));

    virtual_list(
        context, mk(entity, 2), kCount, kRowHeight,
        [&](size_t i, afterhours::Entity &row) {
          div(context, mk(row, 0),
              ComponentConfig{}
                  .with_size(ComponentSize{percent(1.f), pixels(kRowHeight)})
                  .with_label(fmt::format("row {:05}  -  {}", i,
                                          (i % 7 == 0) ? "marked" : "plain"))
                  .with_alignment(TextAlignment::Left)
                  .with_custom_background(i % 2 ? odd : even)
                  .with_custom_text_color(white)
                  .with_font_size(13.f)
                  .with_roundness(0.f)
                  .with_debug_name(fmt::format("vl_row_{}", i)));
        },
        ComponentConfig{}
            .with_size(ComponentSize{pixels(1180), pixels(560)})
            .with_absolute_position(48.f, 84.f)
            .with_custom_background(afterhours::Color{18, 21, 32, 255})
            .with_debug_name("vl_list"));
  }
};

REGISTER_EXAMPLE_SCREEN(virtual_list_lab, "System Demos",
                        "10k-row windowed list; only the visible rows exist",
                        VirtualListLab)
