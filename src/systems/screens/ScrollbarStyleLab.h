#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <array>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// HasScrollView's colour knobs: thickness used to be the only one. Four panes
// -- the default, a theme usage, explicit colours, and a thick bar.
struct ScrollbarStyleLab : ScreenSystem<UIContext<InputAction>> {
  std::optional<float> pending_fraction;

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context,
                     float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();
    const float s = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const auto box = [s](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, y * s).with_corner_radius(0);
    };
    const afterhours::Color ink{231, 239, 251, 255}, muted{174, 190, 214, 255};
    auto root = div(context, mk(entity, 0), ComponentConfig{}
        .with_size({pixels(1280 * s), pixels(720 * s)})
        .with_absolute_position((context.screen_width - 1280 * s) / 2, (context.screen_height - 720 * s) / 2)
        .with_background(Theme::Usage::Background).with_debug_name("sbs_root"));
    const auto label = [&](int id, const std::string &text, float x, float y,
                           float w, float h, float size, bool dim = false) {
      return div(context, mk(root.ent(), id), box(x, y, w, h).with_label(text)
          .with_font("AtkinsonMock", pixels(size * s)).with_custom_text_color(dim ? muted : ink)
          .with_alignment(TextAlignment::Left).with_background(Theme::Usage::None).with_ignore_pointer_events());
    };
    label(0, "Scrollbar styling", 16, 12, 1248, 42, 30);
    label(1, "Compare color sources and thickness using the same 24 rows.", 16, 56, 900, 28, 20, true);
    const std::array<const char *, 3> presets{"Start", "Middle", "End"};
    for (int i = 0; i < 3; ++i) {
      if (button(context, mk(root.ent(), 2 + i), box(924 + static_cast<float>(i) * 114, 52, 104, 36)
          .with_label(presets[static_cast<size_t>(i)]).with_font("AtkinsonMock", pixels(18 * s))
          .with_custom_text_color(ink).with_custom_background({43, 65, 93, 255})
          .with_corner_radius(6 * s).with_debug_name("sbs_preset_" + std::to_string(i))))
        pending_fraction = static_cast<float>(i) / 2;
    }
    const std::array<const char *, 4> names{"Default theme", "Accent thumb", "Explicit colors", "Thick / no track"};
    const std::array<const char *, 4> tokens{"Track: Background / Thumb: FontMuted", "Thumb usage: Accent", "Explicit track and thumb RGBA", "Thickness: 14px / Track alpha: 0"};
    for (int i = 0; i < 4; ++i) {
      const float x = 16 + static_cast<float>(i) * 316;
      const int id = 20 + i * 20;
      label(id, names[static_cast<size_t>(i)], x, 104, 300, 30, 23);
      auto view = div(context, mk(root.ent(), id + 1), box(x, 144, 300, 340)
          .with_background(Theme::Usage::Surface).with_overflow(Overflow::Scroll, Axis::Y)
          .with_padding(Padding::all(pixels(6 * s))).with_debug_name("sbs_view_" + std::to_string(i)));
      auto &scroll = view.ent().get<HasScrollView>();
      scroll.show_scrollbar = true;
      if (i == 1) scroll.scrollbar_thumb_usage = Theme::Usage::Accent;
      if (i == 2) {
        scroll.scrollbar_track_color = afterhours::Color{60, 30, 30, 255};
        scroll.scrollbar_thumb_color = afterhours::Color{235, 120, 90, 255};
      }
      if (i == 3) {
        scroll.scrollbar_thickness = pixels(14 * s);
        scroll.scrollbar_track_color = afterhours::Color{0, 0, 0, 0};
        scroll.scrollbar_thumb_usage = Theme::Usage::Primary;
      }
      if (pending_fraction && scroll.viewport_size) {
        const float range = std::max(0.f, scroll.content_size.y - scroll.viewport_size->y);
        scroll.scroll_offset.y = *pending_fraction * range;
        scroll.scroll_target.y = scroll.scroll_offset.y;
      }
      for (int r = 0; r < 24; ++r)
        div(context, mk(view.ent(), r), ComponentConfig{}
            .with_label(fmt::format("Row {:02d}", r)).with_size({percent(1), pixels(24 * s)})
            .with_font("AtkinsonMock", pixels(14 * s)).with_custom_text_color(ink)
            .with_background(Theme::Usage::None).with_debug_name(fmt::format("sbs_{}_{}", i, r)));
      label(id + 2, fmt::format("24 rows / Offset {:.0f}px", scroll.scroll_offset.y), x, 488, 300, 28, 18);
      const float ratio = scroll.viewport_size && scroll.content_size.y > 0
          ? scroll.viewport_size->y / scroll.content_size.y : 0;
      label(id + 3, fmt::format("Viewport / content: {:.0f}%", ratio * 100), x, 518, 300, 28, 18, true);
      label(id + 4, tokens[static_cast<size_t>(i)], x, 556, 300, 48, 17, true)
          .ent().get<HasLabel>().text_overflow = TextOverflow::Wrap;
      const auto track = scroll.scrollbar_track_color.value_or(context.theme.from_usage(scroll.scrollbar_track_usage));
      const auto thumb = scroll.scrollbar_thumb_color.value_or(context.theme.from_usage(scroll.scrollbar_thumb_usage));
      label(id + 5, fmt::format("Track: {}, {}, {}, {}", track.r, track.g, track.b, track.a), x, 608, 300, 25, 17, true);
      label(id + 6, fmt::format("Thumb: {}, {}, {}, {}", thumb.r, thumb.g, thumb.b, thumb.a), x, 638, 300, 25, 17, true);
      div(context, mk(root.ent(), id + 7), box(x + 276, 641, 18, 18).with_custom_background(thumb));
    }
    pending_fraction.reset();
    label(110, "Track = full range / Thumb = visible fraction / Wheel scrolling stays independent", 16, 682, 996, 28, 18, true);
    div(context, mk(root.ent(), 111), box(1040, 682, 6, 26).with_custom_background(muted));
    div(context, mk(root.ent(), 112), box(1072, 682, 14, 26).with_custom_background(muted));
    label(113, "6px / 14px", 1104, 682, 160, 28, 18, true);
  }
};

REGISTER_EXAMPLE_SCREEN(scrollbar_style_lab, "System Demos",
                        "HasScrollView track and thumb colours",
                        ScrollbarStyleLab)
