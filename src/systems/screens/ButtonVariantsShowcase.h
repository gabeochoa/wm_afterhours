#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <array>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ButtonVariantsShowcase : ScreenSystem<UIContext<InputAction>> {
  std::array<int, 25> click_counts{};
  std::string last_pressed;
  int last_count = 0;
  bool show_bounds = false;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    const auto theme = afterhours::ui::theme_presets::ocean_navy();
    context.theme = theme;
    context.scaling_mode = ScalingMode::Proportional;
    const float s = std::min(context.screen_height / 720.f, context.screen_width / 1280.f);
    const float top = (context.screen_height / s - 720.f) / 2.f;
    const float left = (context.screen_width / s - 1144.f) / 2.f;
    const afterhours::Color muted{183, 199, 216, 255};
    const afterhours::Color diagnostic{104, 125, 145, 255};
    const auto box = [s, top](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, (y + top) * s).with_corner_radius(0)
          .with_background(Theme::Usage::None);
    };
    auto root = div(context, mk(entity), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background(theme.background).with_corner_radius(0).with_debug_name("bv_root"));
    int id = 0;
    const auto label = [&](const std::string &value, float x, float y, float w, float h, float size,
                           afterhours::Color color, const std::string &name = "") {
      return div(context, mk(root.ent(), id++), box(left + x, y, w, h).with_label(value)
          .with_font(size >= 24 ? "AtkinsonMockBold" : "AtkinsonMock", pixels(size * s)).with_custom_text_color(color)
          .with_alignment(TextAlignment::Left).with_ignore_pointer_events().with_debug_name(name));
    };
    const auto pressed = [&](int index, const std::string &name) {
      last_pressed = name;
      last_count = ++click_counts[index];
    };
    const auto bounds = [&](float x, float y, float w, float h, bool visible) {
      div(context, mk(root.ent(), id++), box(left + x - 3, y - 3, w + 6, h + 6)
          .with_ignore_pointer_events().with_on_draw_fg([visible, diagnostic, s](RectangleType r) {
            if (!visible) return;
            afterhours::draw_rectangle_rounded_lines_ex(r, .3f, 16, s, diagnostic);
          }));
    };
    const auto specimen = [&](const std::string &caption, float x, float y, float w, float h,
                              float size, ButtonVariant variant, Theme::Usage role,
                              const std::string &name, bool disabled = false) {
      auto config = box(left + x, y, w, h).with_label(caption)
          .with_font("AtkinsonMock", pixels(size * s)).with_alignment(TextAlignment::Center)
          .with_background(role).with_auto_text_color(true).with_corner_radius(8 * s)
          .with_button_variant(variant).with_disabled(disabled).with_debug_name(name);
      if (variant == ButtonVariant::Outline)
        config.with_border(context.theme.from_usage(role), 2 * s);
      return config;
    };
    div(context, mk(root.ent(), id++), box(left, 24, 1144, 82)
        .with_custom_background(theme.surface).with_corner_radius(12 * s));
    label("Button variants", 20, 29, 336, 42, 34, theme.font);
    label("Ocean Navy", 352, 39, 270, 30, 22, muted);
    label("Variant = appearance     Role = color     Size = geometry     Availability = enabled or disabled",
          20, 77, 1104, 25, 19, muted);
    if (button(context, mk(root.ent(), id++), specimen(show_bounds ? "Hide bounds" : "Show bounds",
          950, 38, 174, 36, 20, ButtonVariant::Filled, Theme::Usage::Secondary, "bv_toggle_bounds")))
      show_bounds = !show_bounds;
    const std::array<ButtonVariant, 3> variants{ButtonVariant::Filled, ButtonVariant::Outline, ButtonVariant::Ghost};
    const std::array<const char *, 3> variant_names{"Filled", "Outline", "Ghost"};
    const std::array<Theme::Usage, 4> roles{Theme::Usage::Primary, Theme::Usage::Accent,
                                         Theme::Usage::Secondary, Theme::Usage::Primary};
    const std::array<const char *, 4> role_names{"Primary", "Accent", "Secondary", "Disabled primary"};
    label(fmt::format("{:.0f} x {:.0f}px", 214 * s, 40 * s), 12, 117, 128, 26, 18, muted);
    for (int column = 0; column < 4; ++column)
      label(role_names[column], 148 + column * 242.f, 116, 224, 28, 21, theme.font);

    // --- Row 1: Filled variant ---
    // --- Row 2: Outline variant ---
    // --- Row 3: Ghost variant ---
    for (int row = 0; row < 3; ++row) {
      const float y = 150 + row * 60.f;
      div(context, mk(root.ent(), id++), box(left, y, 1144, 50)
          .with_custom_background(theme.surface).with_corner_radius(8 * s)
          .with_debug_name("bv_row_" + std::string(row == 0 ? "filled" : row == 1 ? "outline" : "ghost")));
      label(variant_names[row], 20, y + 3, 116, 27, 22, theme.font);
      label(row == 1 ? fmt::format("req {:.0f}px / r{:.0f}", 2 * s, 8 * s) : row == 2 ? "No fill" : "Solid fill", 20, y + 29, 116, 18, 15, muted);
      for (int column = 0; column < 4; ++column) {
        const float x = 148 + column * 242.f;
        bounds(x, y + 5, 214, 40, show_bounds || column == 3);
        if (button(context, mk(root.ent(), id++), specimen(column == 3 ? "Disabled" : "Press",
              x, y + 5, 214, 40, 20, variants[row], roles[column],
              fmt::format("bv_{}_{}", row, column), column == 3)))
          pressed(row * 4 + column, std::string(variant_names[row]) + " / " + role_names[column]);
      }
    }

    // --- Row 4: Size variants ---
    div(context, mk(root.ent(), id++), box(left, 334, 1144, 106)
        .with_custom_background(theme.surface).with_corner_radius(8 * s).with_debug_name("bv_row_sizes"));
    label("Sizes", 20, 352, 116, 30, 22, theme.font);
    label("Role below", 20, 391, 116, 25, 17, muted);
    const std::array<float, 4> widths{100, 180, 214, 214};
    const std::array<float, 4> heights{32, 40, 48, 48};
    const std::array<float, 4> font_sizes{16, 20, 24, 24};
    const std::array<const char *, 4> size_names{"Small", "Medium", "Large", "Large outline"};
    for (int size = 0; size < 4; ++size) {
      const float x = 148 + size * 242.f;
      const float button_x = x + (214 - widths[size]) / 2;
      const float y = 348 + (48 - heights[size]) / 2;
      bounds(button_x, y, widths[size], heights[size], show_bounds);
      if (button(context, mk(root.ent(), id++), specimen(size_names[size], button_x, y,
            widths[size], heights[size], font_sizes[size], size == 3 ? ButtonVariant::Outline : ButtonVariant::Filled,
            size == 3 ? Theme::Usage::Accent : Theme::Usage::Primary, "bv_size_" + std::to_string(size))))
        pressed(12 + size, std::string("Size / ") + size_names[size] + (size == 3 ? " / Accent" : " / Primary"));
      label(fmt::format("{:.0f} x {:.0f}px / {:.0f}px text", widths[size] * s, heights[size] * s, font_sizes[size] * s),
            x, 398, 224, 22, 16, muted, "bv_size_token_" + std::to_string(size));
      label(size == 3 ? "Accent" : "Primary", x, 418, 224, 19, 16, muted);
    }

    // --- Row 5: Theme comparison ---
    label("Themes", 20, 454, 116, 30, 22, theme.font);
    label("Compact", 20, 489, 116, 24, 17, muted);
    label(fmt::format("{:.0f} x {:.0f}px", 280 * s, 30 * s), 12, 519, 128, 24, 17, muted);
    label("Filled color", 12, 591, 128, 24, 16, muted);
    label("Text / fill", 12, 618, 128, 24, 16, muted);
    struct ThemeDemo {
      const char *name;
      Theme theme;
    };
    const std::array<ThemeDemo, 3> themes{{
        {"Neon Dark", afterhours::ui::theme_presets::neon_dark()},
        {"Cozy Kraft", afterhours::ui::theme_presets::cozy_kraft()},
        {"Sage", afterhours::ui::theme_presets::sage_natural()}}};
    const auto hex = [](afterhours::Color color) {
      return fmt::format("#{:02X}{:02X}{:02X}", color.r, color.g, color.b);
    };
    for (int t = 0; t < 3; ++t) {
      const float x = 144 + t * 336.f;
      const auto &demo = themes[t].theme;
      const auto foreground = demo.auto_font_for(Theme::Usage::Primary);
      context.theme = demo;
      div(context, mk(root.ent(), id++), box(left + x, 448, 320, 204)
          .with_custom_background(demo.background).with_border(diagnostic, s).with_corner_radius(10 * s)
          .with_debug_name("bv_theme_col_" + std::string(themes[t].name)));
      label(themes[t].name, x + 16, 453, 288, 28, 24, demo.font);
      for (int variant = 0; variant < 3; ++variant) {
        const float y = 489 + variant * 38.f;
        auto config = specimen(variant_names[variant], x + 20, y, 280, 30, 18, variants[variant],
                               Theme::Usage::Primary, fmt::format("bv_theme_{}_{}", t, variant));
        if (variant == 0) config.with_auto_text_color(false).with_custom_text_color(foreground);
        if (button(context, mk(root.ent(), id++), config))
          pressed(16 + t * 3 + variant, std::string(themes[t].name) + " / " + variant_names[variant]);
      }
      label(hex(foreground) + " / " + hex(demo.primary), x + 16, 610, 288, 28, 18, demo.font,
            "bv_theme_pair_" + std::to_string(t));
      context.theme = theme;
    }

    // --- Click counter ---
    int total = 0;
    for (int count : click_counts) total += count;
    div(context, mk(root.ent(), id++), box(left, 666, 1144, 36)
        .with_custom_background(theme.surface).with_corner_radius(8 * s));
    label(last_pressed.empty() ? "Press any enabled specimen. Its name and count appear here."
                              : fmt::format("{}: {} press{} / {} total", last_pressed, last_count,
                                            last_count == 1 ? "" : "es", total),
          20, 669, 1104, 30, 20, theme.font, "bv_feedback");
  }
};

REGISTER_EXAMPLE_SCREEN(button_variants, "Component Galleries",
                        "Button variants: Filled, Outline, Ghost with sizes "
                        "and themes",
                        ButtonVariantsShowcase)
