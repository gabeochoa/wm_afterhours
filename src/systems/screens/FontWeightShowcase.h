#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <array>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// with_font_weight across the whole enum. floatinghotel wanted semi-bold diff
// headers; this is the screen that says whether the weights are distinguishable
// with the fonts actually loaded, which is the part a unit test cannot answer.
struct FontWeightShowcase : ScreenSystem<UIContext<InputAction>> {
  bool show_guides = false;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    UIStylingDefaults::get().default_font_name = "Archivo";
    context.theme = afterhours::ui::theme_presets::neon_dark();
    context.scaling_mode = ScalingMode::Proportional;
    const float scale = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const float left = (context.screen_width / scale - 1144) / 2;
    const float top = (context.screen_height / scale - 720) / 2;
    const afterhours::Color white{235, 240, 250, 255};
    const afterhours::Color muted{174, 190, 215, 255};
    const afterhours::Color accent{142, 199, 251, 255};
    const auto box = [scale, top, left](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * scale), pixels(h * scale)})
          .with_absolute_position((left + x) * scale, (top + y) * scale)
          .with_background(Theme::Usage::None).with_corner_radius(0);
    };
    auto root = div(context, mk(entity, 0), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background({18, 25, 37, 255}).with_corner_radius(0).with_debug_name("fw_root"));
    const auto label = [&](int id, const std::string &text, float x, float y, float w, float h,
                           float size, afterhours::Color color, const std::string &name = "") {
      div(context, mk(root.ent(), id), box(x, y, w, h).with_label(text)
          .with_font("Archivo", pixels(size * scale)).with_custom_text_color(color)
          .with_alignment(TextAlignment::Left).with_ignore_pointer_events().with_debug_name(name));
    };
    div(context, mk(root.ent(), 1), box(0, 24, 1144, 98)
        .with_custom_background({32, 44, 62, 255}).with_corner_radius(12 * scale));
    label(2, "Font weight", 20, 34, 800, 44, 34, white, "fw_title");
    label(3, "Archivo Narrow / identical text at 22px / native loaded faces", 20, 82, 1104, 30, 21, muted);
    if (button(context, mk(root.ent(), 4), box(946, 42, 178, 40)
        .with_label(show_guides ? "Bounds: on" : "Bounds: off").with_font("Archivo", pixels(22 * scale))
        .with_custom_text_color(white).with_custom_background({46, 68, 100, 255})
        .with_corner_radius(7 * scale).with_debug_name("fw_guides"))) show_guides = !show_guides;
    label(5, "Requested weight", 36, 138, 158, 30, 20, muted);
    label(6, "Rendered weight", 218, 138, 156, 30, 20, muted);
    label(7, "Rendered sample / 22px", 384, 138, 440, 30, 20, muted);

    struct Row {
      const char *name;
      int number;
      afterhours::colors::FontWeight weight;
    };
    // Archivo Narrow ships no Light face, so Light resolves to Regular and
    // warns. Said out loud here, or the row reads as the feature not working.
    const std::array<Row, 5> rows{{
        {"Light", 300, afterhours::colors::FontWeight::Light},
        {"Regular", 400, afterhours::colors::FontWeight::Regular},
        {"Medium", 500, afterhours::colors::FontWeight::Medium},
        {"Semibold", 600, afterhours::colors::FontWeight::SemiBold},
        {"Bold", 700, afterhours::colors::FontWeight::Bold},
    }};
    auto *fonts = afterhours::EntityHelper::get_singleton_cmp<FontManager>();
    const auto guide = [scale](RectangleType r) {
      afterhours::draw_line_ex({r.x, r.y + r.height}, {r.x + r.width, r.y + r.height},
                               scale, {68, 95, 128, 255});
    };
    for (size_t i = 0; i < rows.size(); ++i) {
      const auto &row = rows[i];
      const float y = 178 + static_cast<float>(i) * 64;
      const auto face = fonts ? fonts->resolve_weighted("Archivo", row.weight) : std::string("Archivo");
      const bool fallback = row.number != 400 && face == "Archivo";
      div(context, mk(root.ent(), 20 + static_cast<int>(i)), box(16, y, 822, 58)
          .with_custom_background(i % 2 == 0 ? afterhours::Color{28, 38, 54, 255} : afterhours::Color{23, 32, 47, 255})
          .with_corner_radius(5 * scale));
      label(30 + static_cast<int>(i), std::to_string(row.number) + " " + row.name,
            36, y + 5, 158, 28, 22, white, "fw_name_" + std::to_string(row.number));
      label(40 + static_cast<int>(i), fallback ? "400 / fallback" : std::to_string(row.number),
            218, y + 5, 156, 28, 21, fallback ? accent : muted, "fw_rendered_" + std::to_string(row.number));
      auto sample = div(context, mk(root.ent(), 50 + static_cast<int>(i)), box(384, y + 3, 440, 28)
          .with_label("Sphinx of black quartz, judge my vow 0123")
          .with_alignment(TextAlignment::Left).with_custom_text_color(white)
          .with_font("Archivo", pixels(22 * scale)).with_font_weight(row.weight)
          .with_debug_name("fw_sample_" + std::to_string(row.number)));
      if (show_guides) sample.ent().addComponentIfMissing<HasOnDraw>().fg = guide;
      auto numbers = div(context, mk(root.ent(), 60 + static_cast<int>(i)), box(384, y + 31, 440, 24)
          .with_label("0123456789   ! ? : ; , . ( ) [ ]")
          .with_alignment(TextAlignment::Left).with_custom_text_color(muted)
          .with_font("Archivo", pixels(22 * scale)).with_font_weight(row.weight)
          .with_debug_name("fw_numbers_" + std::to_string(row.number)));
      if (show_guides) numbers.ent().addComponentIfMissing<HasOnDraw>().fg = guide;
    }
    div(context, mk(root.ent(), 70), box(8, 186, 16, 294)
        .with_ignore_pointer_events().with_on_draw_fg([scale, accent](RectangleType r) {
          const float x = r.x + 6 * scale;
          afterhours::draw_line_ex({x, r.y}, {x, r.y + 256 * scale}, scale, accent);
          for (int i = 0; i < 5; ++i)
            afterhours::draw_line_ex({x - 3 * scale, r.y + i * 64 * scale},
                                     {x + 3 * scale, r.y + i * 64 * scale}, scale, accent);
        }));
    div(context, mk(root.ent(), 71), box(198, 190, 12, 90)
        .with_ignore_pointer_events().with_on_draw_fg([scale, accent](RectangleType r) {
          afterhours::draw_line_ex({r.x + r.width, r.y}, {r.x, r.y}, scale, accent);
          afterhours::draw_line_ex({r.x, r.y}, {r.x, r.y + r.height}, scale, accent);
          afterhours::draw_line_ex({r.x, r.y + r.height}, {r.x + r.width, r.y + r.height}, scale, accent);
        }));
    div(context, mk(root.ent(), 72), box(858, 138, 286, 356)
        .with_custom_background({29, 42, 61, 255}).with_corner_radius(10 * scale).with_debug_name("fw_closeup"));
    label(73, "500 / 600 close-up", 874, 151, 254, 32, 24, white);
    for (int i = 0; i < 2; ++i) {
      const auto weight = i == 0 ? afterhours::colors::FontWeight::Medium : afterhours::colors::FontWeight::SemiBold;
      const float y = 206 + i * 112.f;
      label(74 + i, i == 0 ? "Medium 500 / 36px" : "Semibold 600 / 36px", 874, y, 254, 27, 20, muted);
      auto sample = div(context, mk(root.ent(), 76 + i), box(874, y + 32, 254, 54)
          .with_label("Aa Bb 0123").with_font("Archivo", pixels(36 * scale)).with_font_weight(weight)
          .with_custom_text_color(white).with_alignment(TextAlignment::Left)
          .with_debug_name(i == 0 ? "fw_closeup_500" : "fw_closeup_600"));
      if (show_guides) sample.ent().addComponentIfMissing<HasOnDraw>().fg = guide;
    }
    label(78, "Same family / two weights", 874, 452, 254, 30, 17, muted);

    // The case that motivated the request: a heavier run has to sit next to a
    // regular one on the same row and still line up.
    div(context, mk(root.ent(), 80), box(0, 522, 1144, 150)
        .with_custom_background({27, 38, 55, 255}).with_corner_radius(10 * scale));
    label(81, "Mixed weights on one baseline", 20, 536, 1104, 34, 26, white);
    auto mixed = div(context, mk(root.ent(), 82), box(20, 578, 1104, 32)
        .with_flex_direction(FlexDirection::Row).with_transparent_bg().with_debug_name("fw_mixed"));
    div(context, mk(mixed.ent(), 0), ComponentConfig{}
        .with_size({pixels(68 * scale), percent(1.f)}).with_label("theme.h")
        .with_font("Archivo", pixels(18 * scale)).with_font_weight(afterhours::colors::FontWeight::SemiBold)
        .with_custom_text_color(white).with_background(Theme::Usage::None).with_alignment(TextAlignment::Left)
        .with_debug_name("fw_mixed_bold"));
    div(context, mk(mixed.ent(), 1), ComponentConfig{}
        .with_size({expand(), percent(1.f)}).with_label("src/ui")
        .with_font("Archivo", pixels(18 * scale)).with_custom_text_color(muted)
        .with_background(Theme::Usage::None).with_alignment(TextAlignment::Left).with_debug_name("fw_mixed_regular"));
    if (show_guides) mixed.ent().addComponentIfMissing<HasOnDraw>().fg = guide;
    label(83, "18px semibold filename + regular path / guides mark the specimen boxes.", 20, 626, 1104, 28, 21, muted);
    std::string loaded_faces = "Loaded: ";
    for (const auto &name : {"Archivo", "Archivo@medium", "Archivo@semibold", "Archivo@bold"}) {
      if (!fonts || !fonts->fonts.contains(name)) continue;
      if (loaded_faces != "Loaded: ") loaded_faces += ", ";
      loaded_faces += name;
    }
    loaded_faces += " / sizes scale with viewport";
    label(84, loaded_faces, 20, 682, 1104, 24, 18, muted, "fw_faces");
  }
};

REGISTER_EXAMPLE_SCREEN(font_weight, "Component Galleries",
                        "Every FontWeight, plus mixed weights on one row",
                        FontWeightShowcase)
