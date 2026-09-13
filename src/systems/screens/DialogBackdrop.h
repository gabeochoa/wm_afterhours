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
            .with_corner_radius(8.f)
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
            .with_corner_radius(8.f)
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

struct ContextRow {
  std::string label;
  std::string value;
  bool highlighted = false;
};

inline void draw_context(afterhours::ui::UIContext<InputAction> &context,
                         afterhours::Entity &entity, const std::string &section,
                         std::initializer_list<ContextRow> rows,
                         const std::string &status) {
  using namespace afterhours::ui;
  using namespace afterhours::ui::imm;
  const float s = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
  int id = 200;
  const auto box = [&](float x, float y, float w, float h) {
    return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
        .with_absolute_position(x * s, y * s).with_corner_radius(0);
  };
  div(context, mk(entity, id++), ComponentConfig{}
      .with_size({pixels(context.screen_width), pixels(context.screen_height)})
      .with_custom_background({20, 26, 37, 255}).with_corner_radius(0));
  const auto text = [&](const std::string &value, float x, float y, float w, float h, float size) {
    div(context, mk(entity, id++), box(x, y, w, h).with_label(value)
        .with_font("AtkinsonMock", pixels(size * s)).with_alignment(TextAlignment::Left)
        .with_text_overflow(TextOverflow::Ellipsis)
        .with_custom_text_color({199, 212, 231, 255}).with_background(Theme::Usage::None));
  };
  text("HARBOUR STUDIO", 28, 18, 620, 36, 26);
  text(section, 264, 86, 936, 42, 32);
  text("Workspace", 28, 96, 190, 32, 21);
  div(context, mk(entity, id++), box(20, 146, 206, 44)
      .with_custom_background({49, 69, 101, 255}).with_corner_radius(8 * s));
  text(section, 30, 151, 190, 34, 23);
  text("Preferences", 30, 218, 190, 32, 21);
  int row = 0;
  for (const auto &value : rows) {
    const float y = 152 + row++ * 76.f;
    div(context, mk(entity, id++), box(264, y, 948, 60)
        .with_custom_background(value.highlighted ? afterhours::Color{47, 62, 83, 255}
                                                : afterhours::Color{29, 38, 53, 255})
        .with_corner_radius(8 * s));
    text(value.label, 280, y + 12, 356, 36, 22);
    text(value.value, 650, y + 12, 546, 36, 22);
  }
  text(status, 264, 636, 940, 36, 22);
}

} // namespace dialog_backdrop
