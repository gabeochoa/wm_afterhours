#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <algorithm>
#include <array>
#include <string>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct CardsGallery : ScreenSystem<UIContext<InputAction>> {
  bool show_guides = true;
  bool compare_opaque = false;

  static std::string hex(afterhours::Color color) {
    return fmt::format("#{:02X}{:02X}{:02X}", color.r, color.g, color.b);
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    const auto theme = afterhours::ui::theme_presets::cozy_kraft();
    context.set_theme(theme);
    context.scaling_mode = ScalingMode::Proportional;
    const float s = std::min(context.screen_height / 720.f, context.screen_width / 1280.f);
    if (s <= 0.f) return;
    const float left = (context.screen_width - 1144.f * s) / 2.f;
    const float top = (context.screen_height - 720.f * s) / 2.f;
    const afterhours::Color card_border{150, 130, 115, 255};
    const auto box = [s](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, y * s).with_background(Theme::Usage::None).with_corner_radius(0);
    };
    auto root = div(context, mk(entity), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_background(Theme::Usage::Background).with_corner_radius(0).with_debug_name("cards_bg"));
    auto content = div(context, mk(root.ent(), 0), ComponentConfig{}
        .with_size({pixels(1144.f * s), pixels(720.f * s)})
        .with_absolute_position(left, top).with_background(Theme::Usage::None)
        .with_corner_radius(0).with_debug_name("cards_main"));
    int id = 0;
    const auto label = [&](afterhours::Entity &parent, const std::string &text,
                           float x, float y, float w, float h, float size,
                           afterhours::Color color, const std::string &name = "", bool emphasis = false) {
      return div(context, mk(parent, id++), box(x, y, w, h).with_label(text)
          .with_font(emphasis ? "AtkinsonMockBold" : "AtkinsonMock", pixels(size * s))
          .with_custom_text_color(color).with_alignment(TextAlignment::Left)
          .with_ignore_pointer_events().with_debug_name(name));
    };
    const auto child_label = [&](afterhours::Entity &parent, const std::string &text,
                                 float height, float size, afterhours::Color color,
                                 const std::string &name = "", bool strong = false) {
      return div(context, mk(parent, id++), ComponentConfig{}
          .with_size({percent(1.f), pixels(height * s)}).with_label(text)
          .with_font(strong ? "AtkinsonMockBold" : "AtkinsonMock", pixels(size * s)).with_custom_text_color(color)
          .with_background(Theme::Usage::None).with_alignment(TextAlignment::Left)
          .with_ignore_pointer_events().with_debug_name(name));
    };
    auto header = div(context, mk(content.ent(), id++), box(0, 24, 1144, 80)
        .with_background(Theme::Usage::Primary).with_corner_radius(12 * s));
    const auto header_ink = afterhours::colors::auto_text_color(theme.primary, theme.font, theme.darkfont);
    // Title - larger font for better visibility
    label(header.ent(), "Cards & Panels Gallery", 20, 8, 870, 40, 33, header_ink, "cards_title");
    label(header.ent(), "Cozy Kraft / cards, nesting, alpha. Measurements use 720p design pixels.", 20, 49, 1104, 25, 21, header_ink);

    label(content.ent(), "Styles", 0, 120, 280, 28, 25, theme.font);
    label(content.ent(), "140 x 100 px / requested 1 px border #968273 / fills and corners vary",
          312, 123, 832, 27, 20, theme.font_muted, "cards_style_dimensions");
    // Row 1: Basic cards - all simple like theme swatches
    auto styles = div(context, mk(content.ent(), id++), box(0, 158, 1144, 180).with_debug_name("row1_cards"));
    const std::array<const char *, 4> style_names{"Surface", "Primary", "Sharp", "Custom"};
    const std::array<Theme::Usage, 4> style_roles{Theme::Usage::Surface, Theme::Usage::Primary,
        Theme::Usage::Accent, Theme::Usage::Secondary};
    const std::array<const char *, 4> role_names{"Surface", "Primary", "Accent", "Secondary"};
    for (size_t i = 0; i < style_names.size(); ++i) {
      const float x = static_cast<float>(i) * 290.f;
      div(context, mk(styles.ent(), id++), box(x + 63, 4, 148, 108).with_ignore_pointer_events()
          .with_on_draw_fg([visible = show_guides, s, color = theme.font_muted](RectangleType r) {
            if (!visible) return;
            raylib::DrawRectangleLinesEx(r, s, color);
          }));
      auto config = box(x + 67, 8, 140, 100).with_background(style_roles[i])
          .with_border(card_border, s).with_padding(Padding::all(pixels(12 * s)))
          .with_corner_radius(i == 2 ? 0.f : 6.f * s)
          .with_debug_name("style_card_" + std::to_string(i));
      if (i == 2) config.disable_rounded_corners();
      if (i == 3)
        config.with_rounded_corners(RoundedCorners().all_sharp().round(TOP_LEFT).round(BOTTOM_RIGHT));
      auto card = div(context, mk(styles.ent(), id++), config);
      child_label(card.ent(), style_names[i], 30, 23,
          afterhours::colors::auto_text_color(theme.from_usage(style_roles[i]), theme.font, theme.darkfont),
          "style_label_" + std::to_string(i));
      label(styles.ent(), std::string(role_names[i]) + " / " + hex(theme.from_usage(style_roles[i])),
            x, 121, 274, 26, 21, theme.font, "style_fill_" + std::to_string(i));
      label(styles.ent(), i == 2 ? "All corners sharp / 0 px" : i == 3 ? "TL + BR: 6 px; others sharp" : "All corners rounded / 6 px",
            x, 151, 274, 26, 19, theme.font_muted, "style_corners_" + std::to_string(i));
    }

    // Row 2: Nested cards
    label(content.ent(), "Nested cards and transparent panels", 0, 350, 900, 28, 25, theme.font);
    auto panels = div(context, mk(content.ent(), id++), box(0, 386, 1144, 170).with_debug_name("row2_nested"));
    auto nested = vstack(context, mk(panels.ent(), id++), box(0, 0, 556, 170)
        .with_background(Theme::Usage::Surface).with_corner_radius(6 * s)
        .with_border(card_border, 2 * s)
        .with_padding(Padding::all(pixels(16 * s))).with_gap(pixels(12 * s))
        .with_no_wrap().with_debug_name("nested_container"));
    child_label(nested.ent(), "Nested Cards", 36, 28, theme.font, "nested_title", true);
    // Inner cards row - prevent wrapping, ensure enough height for cards
    auto inner_row = hstack(context, mk(nested.ent(), id++), ComponentConfig{}
        .with_size({percent(1.f), pixels(55 * s)}).with_gap(pixels(12 * s))
        .with_background(Theme::Usage::None).with_no_wrap().with_debug_name("inner_row"));
    const std::array<Theme::Usage, 3> item_roles{Theme::Usage::Primary, Theme::Usage::Secondary, Theme::Usage::Accent};
    for (size_t i = 0; i < item_roles.size(); ++i) {
      auto item = div(context, mk(inner_row.ent(), id++), ComponentConfig{}
          .with_size({pixels((500.f / 3.f) * s), pixels(55 * s)})
          .with_background(item_roles[i]).with_corner_radius(1.65f * s)
          .with_padding(Padding::all(pixels(12 * s))).with_debug_name("nested_item_" + std::to_string(i + 1)));
      child_label(item.ent(), "Item " + std::to_string(i + 1), 28, 22,
          afterhours::colors::auto_text_color(theme.from_usage(item_roles[i]), theme.font, theme.darkfont),
          "nested_item_label_" + std::to_string(i + 1));
    }
    child_label(nested.ent(), "Parent padding: 16 px / item gap: 12 px", 23, 19, theme.font_muted, "nested_metrics");
    div(context, mk(panels.ent(), id++), box(0, 0, 556, 170).with_ignore_pointer_events()
        .with_on_draw_fg([visible = show_guides, s, color = theme.font_muted](RectangleType r) {
          if (!visible) return;
          raylib::DrawLineEx({r.x, r.y + 10 * s}, {r.x + 16 * s, r.y + 10 * s}, s, color);
          raylib::DrawLineEx({r.x + 10 * s, r.y}, {r.x + 10 * s, r.y + 16 * s}, s, color);
        }));

    div(context, mk(panels.ent(), id++), box(588, 0, 556, 170).with_ignore_pointer_events()
        .with_on_draw_bg([s](RectangleType r) {
          const float cell = 20 * s;
          for (float y = 0; y < r.height; y += cell)
            for (float x = 0; x < r.width; x += cell) {
              const bool alternate = (static_cast<int>(x / cell) + static_cast<int>(y / cell)) % 2;
              afterhours::draw_rectangle({r.x + x, r.y + y, std::min(cell, r.width - x), std::min(cell, r.height - y)},
                  alternate ? afterhours::Color{210, 188, 164, 255} : afterhours::Color{255, 250, 242, 255});
            }
        }));
    // Info panel with different opacity
    const float opacity = compare_opaque ? 1.f : .3f;
    const auto fill = afterhours::colors::opacity_pct(theme.primary, opacity);
    auto info = vstack(context, mk(panels.ent(), id++), box(588, 0, 556, 170)
        .with_custom_background(fill).with_corner_radius(6 * s)
        .with_padding(Padding::all(pixels(16 * s))).with_gap(pixels(12 * s))
        .with_no_wrap().with_debug_name("info_panel"));
    const auto info_ink = compare_opaque ? header_ink : theme.font;
    child_label(info.ent(), compare_opaque ? "Opaque Comparison" : "Semi-Transparent", 36, 28, info_ink, "info_title", true);
    div(context, mk(info.ent(), id++), ComponentConfig{}.with_size({percent(1.f), pixels(55 * s)})
        .with_label("Checkerboard reveals blending.\nText stays opaque.")
        .with_font("AtkinsonMock", pixels(21 * s)).with_custom_text_color(info_ink)
        .with_background(Theme::Usage::None).with_text_overflow(TextOverflow::Wrap)
        .with_alignment(TextAlignment::Left).with_ignore_pointer_events());
    child_label(info.ent(), fmt::format("Primary fill: {:.0f}% / alpha {}/255", opacity * 100, fill.a),
                23, 19, info_ink, "cards_opacity_value");

    if (button(context, mk(content.ent(), id++), box(0, 568, 182, 34)
        .with_label(show_guides ? "Hide guides" : "Show guides")
        .with_font("AtkinsonMock", pixels(21 * s)).with_background(Theme::Usage::Primary)
        .with_custom_text_color(header_ink).with_corner_radius(7 * s).with_debug_name("cards_guides")))
      show_guides = !show_guides;
    label(content.ent(), show_guides ? "Guides on / content padding: 12 px" : "Guides off / content padding: 12 px",
          198, 572, 374, 27, 19, theme.font_muted, "cards_guide_status");
    if (button(context, mk(content.ent(), id++), box(588, 568, 260, 34)
        .with_label(compare_opaque ? "Restore 30% fill" : "Compare opaque fill")
        .with_font("AtkinsonMock", pixels(21 * s)).with_background(Theme::Usage::Primary)
        .with_custom_text_color(header_ink).with_corner_radius(7 * s).with_debug_name("cards_opacity")))
      compare_opaque = !compare_opaque;
    label(content.ent(), "Fill alpha only; text is 100%.", 864, 572, 280, 27, 18, theme.font_muted);

    // Row 3: Theme comparison
    label(content.ent(), "Theme palette / Cozy Kraft", 0, 614, 1144, 28, 25, theme.font);
    auto palette = div(context, mk(content.ent(), id++), box(0, 651, 1144, 58).with_debug_name("row3_themes"));
    const std::array<Theme::Usage, 5> roles{Theme::Usage::Primary, Theme::Usage::Secondary,
        Theme::Usage::Accent, Theme::Usage::Surface, Theme::Usage::Background};
    const std::array<const char *, 5> names{"Primary", "Secondary", "Accent", "Surface", "Background"};
    // Color swatches - with gaps between them
    for (size_t i = 0; i < roles.size(); ++i) {
      const float x = static_cast<float>(i) * 232.f;
      auto swatch = div(context, mk(palette.ent(), id++), box(x, 0, 216, 32)
          .with_background(roles[i]).with_border(card_border, s).with_corner_radius(4 * s)
          .with_debug_name("cards_swatch_" + std::to_string(i)));
      label(swatch.ent(), names[i], 12, 2, 192, 28, 22,
            afterhours::colors::auto_text_color(theme.from_usage(roles[i]), theme.font, theme.darkfont));
      label(palette.ent(), hex(theme.from_usage(roles[i])), x, 35, 216, 23, 18, theme.font_muted,
            "cards_palette_value_" + std::to_string(i), true);
    }
  }
};

REGISTER_EXAMPLE_SCREEN(cards, "Component Galleries",
                        "Card layouts, nesting, and corner styles",
                        CardsGallery)
