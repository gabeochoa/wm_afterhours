#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include <afterhours/ah.h>

// A plausible app for the dialog screens to dim. Each of them used to draw a
// full-screen div whose only content was one word, which since Div labels
// default to left alignment landed at x=0 halfway down an otherwise empty
// screen and read as a clipped orphan.
namespace dialog_backdrop {

inline void draw(afterhours::ui::UIContext<InputAction> &context,
                 afterhours::Entity &entity, int base_index,
                 const char *app_name, const char *section) {
  using namespace afterhours::ui;
  using namespace afterhours::ui::imm;

  const auto page = afterhours::Color{24, 27, 38, 255};
  const auto chrome = afterhours::Color{32, 36, 50, 255};
  const auto rail = afterhours::Color{28, 32, 45, 255};
  const auto row = afterhours::Color{37, 42, 58, 255};
  const auto ink = afterhours::Color{196, 204, 222, 255};
  const auto dim = afterhours::Color{132, 142, 166, 255};

  int i = base_index;
  const auto at = [&](float x, float y, float w, float h) {
    return ComponentConfig{}
        .with_size(ComponentSize{pixels(w), pixels(h)})
        .with_absolute_position(x, y);
  };

  div(context, mk(entity, i++),
      at(0.f, 0.f, 1280.f, 720.f).with_custom_background(page).with_debug_name(
          "bd_page"));

  div(context, mk(entity, i++),
      at(0.f, 0.f, 1280.f, 52.f)
          .with_custom_background(chrome)
          .with_debug_name("bd_titlebar"));

  div(context, mk(entity, i++),
      at(24.f, 12.f, 400.f, 28.f)
          .with_label(app_name)
          .with_alignment(TextAlignment::Left)
          .with_font_size(pixels(18.f))
          .with_custom_text_color(ink)
          .with_debug_name("bd_appname"));

  div(context, mk(entity, i++),
      at(0.f, 52.f, 232.f, 668.f)
          .with_custom_background(rail)
          .with_debug_name("bd_rail"));

  static constexpr const char *NAV[5] = {"General", "Display", "Audio",
                                         "Controls", "Account"};
  for (int n = 0; n < 5; n++) {
    div(context, mk(entity, i++),
        at(16.f, 80.f + (float)n * 44.f, 200.f, 36.f)
            .with_label(NAV[n])
            .with_alignment(TextAlignment::Left)
            .with_font_size(pixels(16.f))
            .with_custom_text_color(n == 1 ? ink : dim)
            .with_custom_background(n == 1 ? row : page)
            .with_roundness(0.2f)
            .with_debug_name(fmt::format("bd_nav_{}", n)));
  }

  div(context, mk(entity, i++),
      at(264.f, 84.f, 500.f, 30.f)
          .with_label(section)
          .with_alignment(TextAlignment::Left)
          .with_font_size(pixels(20.f))
          .with_custom_text_color(ink)
          .with_debug_name("bd_section"));

  struct Row {
    const char *label;
    const char *value;
  };
  // Nine rows, not six: at six the page stopped at y=470 and the bottom third
  // of the thing being dimmed was empty.
  static constexpr Row ROWS[9] = {
      {"Resolution", "2560 x 1440"},   {"Refresh rate", "120 Hz"},
      {"Window mode", "Borderless"},   {"Vertical sync", "On"},
      {"Render scale", "100%"},        {"Colour profile", "sRGB"},
      {"Texture quality", "High"},     {"Shadow detail", "Medium"},
      {"Anti-aliasing", "TAA"},
  };
  for (int r = 0; r < 9; r++) {
    const float y = 136.f + (float)r * 58.f;
    div(context, mk(entity, i++),
        at(264.f, y, 976.f, 46.f)
            .with_custom_background(row)
            .with_roundness(0.14f)
            .with_debug_name(fmt::format("bd_row_{}", r)));
    div(context, mk(entity, i++),
        at(288.f, y, 500.f, 46.f)
            .with_label(ROWS[r].label)
            .with_alignment(TextAlignment::Left)
            .with_font_size(pixels(16.f))
            .with_custom_text_color(ink)
            .with_debug_name(fmt::format("bd_rowlabel_{}", r)));
    div(context, mk(entity, i++),
        at(816.f, y, 400.f, 46.f)
            .with_label(ROWS[r].value)
            .with_alignment(TextAlignment::Right)
            .with_font_size(pixels(16.f))
            .with_custom_text_color(dim)
            .with_debug_name(fmt::format("bd_rowvalue_{}", r)));
  }
}

} // namespace dialog_backdrop
