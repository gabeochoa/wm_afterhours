#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <algorithm>
#include <array>

struct TransparentControlsLab
    : ScreenSystem<afterhours::ui::UIContext<InputAction>> {
  std::array<int, 3> additions{};

  void for_each_with(afterhours::Entity &entity,
                     afterhours::ui::UIContext<InputAction> &context,
                     float) override {
    using namespace afterhours::ui;
    using namespace afterhours::ui::imm;
    context.theme = theme_presets::ocean_navy();
    context.scaling_mode = ScalingMode::Proportional;
    const float s = std::min(context.screen_width / 1280.f,
                             context.screen_height / 720.f);
    const float left = (context.screen_width / s - 1144.f) / 2.f;
    const float top = (context.screen_height / s - 720.f) / 2.f;
    const afterhours::Color ink{23, 36, 53, 255};
    const afterhours::Color white{248, 250, 253, 255};
    const afterhours::Color muted{73, 91, 111, 255};
    context.theme.background = {236, 241, 247, 255};
    context.theme.focus = {38, 111, 229, 255};
    context.theme.highlight_mode = HighlightMode::Split;
    context.theme.disabled_opacity = .3f;
    context.theme.focus_ring_offset = -5 * s;
    context.theme.focus_ring_thickness = 3 * s;
    const auto box = [=](float x, float y, float w, float h) {
      return ComponentConfig{}
          .with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position((left + x) * s, (top + y) * s)
          .with_corner_radius(0).with_background(Theme::Usage::None);
    };
    auto root = div(context, mk(entity), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background(context.theme.background).with_corner_radius(0)
        .with_debug_name("transparent_canvas"));
    int id = 0;
    const auto label = [&](const std::string &text, float x, float y, float w,
                           float h, float size, afterhours::Color color,
                           const std::string &name, bool bold = false) {
      return div(context, mk(root.ent(), id++), box(x, y, w, h)
          .with_label(text).with_font(bold ? "AtkinsonMockBold" : "AtkinsonMock", pixels(size * s))
          .with_custom_text_color(color).with_alignment(TextAlignment::Left)
          .with_ignore_pointer_events().with_debug_name(name));
    };
    label("Clear controls on every background", 0, 26, 1144, 48,
          36, ink, "transparent_title", true);
    label("The background shows through. The action stays easy to find.",
          0, 84, 1144, 32, 22, muted, "transparent_subtitle");
    const std::array<const char *, 3> names{"Light", "Dark", "Pattern"};
    const std::array<afterhours::Color, 3> backings{{
        {242, 246, 250, 255}, {4, 10, 18, 255}, {202, 216, 231, 255}}};
    for (size_t i = 0; i < names.size(); ++i) {
      const float x = i * 392.f;
      const bool dark = i == 1;
      const auto foreground = dark ? white : ink;
      auto panel = box(x, 148, 360, 384).with_custom_background(backings[i])
          .with_debug_name("transparent_panel_" + std::to_string(i));
      if (i == 2) {
        panel.with_on_draw_fg([](RectangleType r) {
          const float cell = r.width / 15.f;
          for (int row = 0; row < 16; ++row)
            for (int column = 0; column < 15; ++column)
              afterhours::draw_rectangle(
                  {r.x + column * cell, r.y + row * cell, cell, cell},
                  (row + column) % 2 ? afterhours::Color{230, 237, 245, 255}
                                     : afterhours::Color{202, 216, 231, 255});
        });
      }
      div(context, mk(root.ent(), id++), panel.with_ignore_pointer_events());
      label(names[i], x + 28, 170, 304, 40, 28, foreground,
            "transparent_heading_" + std::to_string(i), true);
      label("Hover or focus, then add an item.", x + 28, 220, 304, 30,
            18, foreground, "transparent_hint_" + std::to_string(i));
      const afterhours::Color fill = dark ? afterhours::Color{215, 231, 250, 28}
                                          : afterhours::Color{36, 76, 114, 32};
      const afterhours::Color hover = dark ? afterhours::Color{215, 231, 250, 50}
                                           : afterhours::Color{36, 76, 114, 60};
      const auto action = [&](float y, bool disabled, const std::string &name) {
        return button(context, mk(root.ent(), id++), box(x + 28, y, 304, 56)
            .with_label("Add to list").with_font("AtkinsonMockBold", pixels(22 * s))
            .with_alignment(TextAlignment::Center).with_text_inset(12 * s, 4 * s)
            .with_custom_background(fill).with_custom_hover_bg(hover)
            .with_custom_text_color(foreground).with_border(foreground, 2 * s)
            .with_corner_radius(8 * s).with_disabled(disabled)
            .with_debug_name(name));
      };
      auto enabled = action(276, false, "transparent_add_" + std::to_string(i));
      if (enabled) ++additions[i];
      label(context.was_hot(enabled.ent().id) ? "Hovered" : "Enabled",
            x + 28, 344, 304, 28, 19, foreground,
            "transparent_state_" + std::to_string(i));
      if (action(402, true, "transparent_disabled_" + std::to_string(i)))
        ++additions[i];
      label("Unavailable", x + 28, 470, 304, 28, 19, foreground,
            "transparent_disabled_label_" + std::to_string(i));
      label(fmt::format("{}: {} added", names[i], additions[i]),
            x + 4, 548, 352, 34, 23, ink,
            "transparent_count_" + std::to_string(i), true);
    }
    if (button(context, mk(root.ent(), id++), box(904, 610, 240, 52)
        .with_label("Reset count").with_font("AtkinsonMock", pixels(22 * s))
        .with_custom_background(ink).with_custom_text_color(white)
        .with_corner_radius(8 * s).with_debug_name("transparent_reset")))
      additions.fill(0);
    label(fmt::format("Total added: {}", additions[0] + additions[1] + additions[2]),
          0, 616, 700, 40, 28, ink, "transparent_total", true);
    label("Tab selects a control. Enter adds an item. Unavailable controls keep the count unchanged.",
          0, 679, 1144, 25, 18, muted, "transparent_help");
  }
};

REGISTER_EXAMPLE_SCREEN(transparent_controls, "Component Galleries",
                        "Transparent native controls on light, dark and patterned backgrounds",
                        TransparentControlsLab)
