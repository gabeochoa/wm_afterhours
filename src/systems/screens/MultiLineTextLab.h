#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Every way a label can end up on more than one line, side by side, so the
// screenshot shows whether they agree with each other.
//
// Each block draws a thin box at its laid-out size with the text inside it.
// That pairing is the point: layout and rendering compute the line count
// separately, and when they disagree the text spills past its own outline --
// visible here, invisible in a test that only checks one of the two.
struct MultiLineTextLab : ScreenSystem<UIContext<InputAction>> {
  static constexpr float FS = 18.f;
  float box_w = 300.f;
  float box_h = 120.f;
  static constexpr float BOX_X = 48.f;
  static constexpr float BOX_Y = 256.f;
  static constexpr float HANDLE = 28.f;
  int page = 0;

  // Previous frame's handle id. hot/active are resolved after the screen is
  // built, so this frame's colour has to come from last frame's state -- the
  // same reason was_hot() exists.
  afterhours::EntityID handle_id = -1;
  int drag_count = 0;
  inline static const std::string wrapped = "the quick brown fox jumps over the lazy dog and then keeps on running well past the edge";
  inline static const std::string resized = "the quick brown fox jumps over the lazy dog and then keeps on running well past the edge of the box";

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();
    context.theme.text_inset = {5.f, 0.f};
    const float s = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const float ox = (context.screen_width - 1280 * s) / 2;
    const float oy = (context.screen_height - 720 * s) / 2;
    const afterhours::Color white{235, 242, 251, 255}, muted{170, 188, 211, 255}, box{60, 70, 100, 255};
    const auto at = [=](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(ox + x * s, oy + y * s).with_corner_radius(0);
    };
    const auto label = [&](int id, const std::string &text, float x, float y,
                           float w = 600, float size = 19) {
      return div(context, mk(entity, id), at(x, y, w, 30).with_label(text)
          .with_font("AtkinsonMock", pixels(size * s)).with_custom_text_color(muted)
          .with_alignment(TextAlignment::Left).with_background(Theme::Usage::None)
          .with_ignore_pointer_events());
    };
    div(context, mk(entity, 0), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_absolute_position(0, 0).with_background(Theme::Usage::Background).with_corner_radius(0));
    label(1, "Multi-line Text Lab", 48, 20, 1184, 30);
    label(2, "Text and measured bounds should agree.", 48, 60, 1184, 21);
    for (int i = 0; i < 2; ++i) {
      if (button(context, mk(entity, 3 + i), at(48 + static_cast<float>(i) * 604, 104, 580, 38)
          .with_label(i == 0 ? "1-7 / Line breaks and wrapping" : "8 / Drag to compare heights")
          .with_font("AtkinsonMock", pixels(20 * s)).with_custom_text_color(white)
          .with_custom_background(page == i ? afterhours::Color{45, 81, 119, 255} : afterhours::Color{30, 41, 57, 255})
          .with_corner_radius(6 * s).with_debug_name("ml_page_" + std::to_string(i)))) page = i;
    }
    const auto sample = [&](int id, const std::string &text, float x, float y,
                            float w, float h, const std::string &name, bool wrap,
                            bool pinned, bool content_height, afterhours::Color color) {
      auto config = at(x, y, w, h).with_label(text).with_font("AtkinsonMock", pixels(FS * s)).with_alignment(TextAlignment::Left)
          .with_custom_text_color(white).with_custom_background(color).with_debug_name(name);
      if (pinned) config.with_font_size(FS * s);
      else config.with_font("AtkinsonMock", pixels(FS * s)).with_autofit();
      if (wrap) config.with_text_overflow(TextOverflow::Wrap);
      if (content_height) config.with_size({pixels(w * s), Size{Dim::Text, 0.f, 1.f}});
      return div(context, mk(entity, id), config);
    };
    const auto metrics = [&](const ElementResult &result) {
      auto *cache = afterhours::EntityHelper::get_singleton_cmp<TextMeasureCache>();
      if (!cache || result.cmp().rect().width <= 0) return std::string("Measuring...");
      const auto &text = result.ent().get<HasLabel>();
      const auto lines = afterhours::ui::detail::wrap_text_to_width(text.label,
          result.cmp().rect().width - 10, [&](const std::string &line) {
            return cache->measure(line, "AtkinsonMock", FS * s, 1).x;
          }).size();
      return fmt::format("{} lines / height {:.0f}px", lines, result.cmp().rect().height);
    };
    if (page == 0) {
      label(10, "1 / Explicit line breaks / auto-fit", 48, 158, 300);
      label(11, "2 / Same text / fixed 18px font", 360, 158, 310);
      auto automatic = sample(20, "first line\nsecond line\nthird line", 48, 192, 280, 96, "ml_hard_autofit", false, false, false, box);
      sample(21, "first line\nsecond line\nthird line", 360, 192, 280, 96, "ml_hard_pinned", false, true, false, box);
      label(12, "3 / Wrap / 280 x 110px", 48, 330, 300);
      label(13, "4 / Content-derived height", 360, 330, 310);
      auto fixed = sample(22, wrapped, 48, 364, 280, 110, "ml_wrap_fixed", true, true, false, box);
      auto sized = sample(23, wrapped, 360, 364, 280, 0, "ml_wrap_selfsizing", true, true, true, box);
      label(14, metrics(fixed), 48, 478, 300, 18);
      label(15, "Dim::Text / " + metrics(sized), 360, 478, 330, 18);
      label(16, "5 / Styled runs / explicit breaks", 48, 530, 310);
      label(17, "6 / Two breaks / one blank line", 360, 530, 324);
      div(context, mk(entity, 24), at(48, 564, 280, 96)
          .with_styled_label({TextSpan{"- removed line\n", {230, 90, 90, 255}},
              TextSpan{"+ added line\n", {90, 210, 120, 255}}, TextSpan{"  context line", muted}})
          .with_alignment(TextAlignment::Left).with_custom_background(box).with_debug_name("ml_styled_break"));
      sample(25, "above the gap\n\nbelow the gap", 360, 564, 280, 96, "ml_blank_line", false, true, false, box);
      label(30, "Same text, different sizing modes", 704, 158, 528, 23);
      label(31, "Explicit line breaks (\\n) / 3 lines", 704, 200, 528);
      auto *fonts = afterhours::EntityHelper::get_singleton_cmp<FontManager>();
      if (fonts && automatic.cmp().rect().width > 0) {
        const auto previous = fonts->active_font;
        fonts->set_active("AtkinsonMock");
        const auto fit = afterhours::ui::position_text_ex(*fonts, "first line\nsecond line\nthird line",
            automatic.cmp().rect(), TextAlignment::Left, {5, 0}, 0, 0, TextOverflow::Clip, false);
        fonts->set_active(previous);
        label(32, fmt::format("Auto-fit result: {:.1f}px", fit.rect.height), 704, 240, 528);
      }
      label(33, fmt::format("Fixed font size: {:.1f}px / 18px at 720p", FS * s), 704, 276, 528);
      label(34, "Wrap / same width, same line count", 704, 330, 528, 23);
      label(35, "Fixed height leaves spare space below the text.", 704, 368, 528);
      label(36, "Content-derived height follows the text block.", 704, 404, 528);
      const auto bounds = sized.cmp().rect();
      div(context, mk(entity, 26), ComponentConfig{}
          .with_size({pixels(12 * s), pixels(bounds.height)})
          .with_absolute_position(bounds.x + bounds.width + 12 * s, bounds.y)
          .with_background(Theme::Usage::None).with_ignore_pointer_events()
          .with_on_draw_fg([s, muted](RectangleType r) {
            raylib::DrawLineEx({r.x + r.width, r.y}, {r.x + r.width, r.y + r.height}, s, muted);
            raylib::DrawLineEx({r.x, r.y}, {r.x + r.width, r.y}, s, muted);
            raylib::DrawLineEx({r.x, r.y + r.height}, {r.x + r.width, r.y + r.height}, s, muted);
          }));
      label(37, "7 / Monospaced companion / DGOne", 704, 470, 528, 22);
      div(context, mk(entity, 27), at(704, 506, 420, 96)
          .with_styled_label({TextSpan{"- removed line\n", {230, 90, 90, 255}},
              TextSpan{"+ added line\n", {90, 210, 120, 255}}, TextSpan{"  context line", muted}})
          .with_font("DGOne", pixels(18 * s)).with_alignment(TextAlignment::Left)
          .with_custom_background(box).with_debug_name("ml_monospace"));
      label(38, "Red: removed / Green: added / Grey: context", 704, 616, 528, 18);
      div(context, mk(entity, 28), at(656, 603, 12, 18).with_ignore_pointer_events()
          .with_on_draw_fg([s, muted](RectangleType r) {
            raylib::DrawLineEx({r.x, r.y}, {r.x, r.y + r.height}, s, muted);
            raylib::DrawLineEx({r.x, r.y}, {r.x + r.width, r.y}, s, muted);
            raylib::DrawLineEx({r.x, r.y + r.height}, {r.x + r.width, r.y + r.height}, s, muted);
          }));
      label(39, "Dimensions scale together with the viewport. Sample text uses Atkinson.", 48, 682, 1184, 18);
      return;
    }
    label(40, "Drag the corner grip. Both samples always use the same width and text.", 48, 160, 1184, 22);
    label(41, fmt::format("Width {:.0f}px / Height {:.0f}px", box_w * s, box_h * s), 48, 202, 548, 22);
    label(42, "Green / content-derived-height reference", 688, 202, 544, 22);
    sample(43, resized, BOX_X, BOX_Y, box_w, box_h, "ml_resizable", true, true, false, box);
    auto reference = sample(44, resized, 688, BOX_Y, box_w, 0, "ml_resizable_selfsized", true, true, true, {70, 100, 80, 255});
    const bool hot = handle_id >= 0 && context.was_hot(handle_id);
    const bool held = handle_id >= 0 && context.was_active(handle_id);
    const auto handle_color = held ? afterhours::Color{255, 230, 120, 255}
                            : hot ? afterhours::Color{190, 235, 255, 255}
                                  : afterhours::Color{120, 200, 255, 255};
    auto handle = div(context, mk(entity, 45), at(BOX_X + box_w - HANDLE, BOX_Y + box_h - HANDLE, HANDLE, HANDLE)
        .with_custom_background(handle_color).with_debug_name("ml_resize_handle")
        .with_on_draw_fg([s](RectangleType r) {
          for (int i = 0; i < 3; ++i) {
            const float n = (7 + static_cast<float>(i) * 6) * s;
            raylib::DrawLineEx({r.x + r.width - n, r.y + r.height - 4 * s},
                              {r.x + r.width - 4 * s, r.y + r.height - n}, 1.5f * s, {25, 45, 65, 255});
          }
        }));
    handle_id = handle.ent().id;
    const auto resize = [this, s, ox, oy](afterhours::Entity &) {
      const auto mouse = afterhours::input::get_mouse_position();
      box_w = std::clamp((mouse.x - ox) / s - BOX_X, 80.f, 540.f);
      box_h = std::clamp((mouse.y - oy) / s - BOX_Y, 40.f, 240.f);
      ++drag_count;
    };
    handle.ent().addComponentIfMissing<HasDragListener>(resize).cb = resize;
    div(context, mk(entity, 46), at(48, 244, 1184, 1).with_custom_background({92, 121, 145, 255}).with_ignore_pointer_events());
    label(47, "Reference: " + metrics(reference), 688, 582, 544);
    label(48, "A short fixed box intentionally clips text. The green box must fit its complete text.", 48, 622, 1184, 18);
    label(49, fmt::format("Hot: {} / Dragging: {} / Drag updates: {}", hot ? "Yes" : "No", held ? "Yes" : "No", drag_count), 48, 658, 1184, 19);
    label(50, fmt::format("Handle  X {:.0f}  Y {:.0f}  Width {:.0f}  Height {:.0f}",
        ox + (BOX_X + box_w - HANDLE) * s, oy + (BOX_Y + box_h - HANDLE) * s, HANDLE * s, HANDLE * s), 48, 688, 1184, 18);
  }
};

REGISTER_EXAMPLE_SCREEN(multiline_text_lab, "System Demos",
                        "Hard breaks, wrapping, and self-sizing text blocks",
                        MultiLineTextLab)
