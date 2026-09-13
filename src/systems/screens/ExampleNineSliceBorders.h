#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>
#include <array>
#include <cmath>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ExampleNineSliceBordersScreen : ScreenSystem<UIContext<InputAction>> {
  // Fantasy parchment aesthetic
  afterhours::Color bg_dark{35, 28, 22, 255};      // Dark wood/leather
  afterhours::Color bg_medium{55, 45, 38, 255};    // Medium brown
  afterhours::Color parchment{245, 235, 220, 255}; // Parchment background
  afterhours::Color gold_accent{230, 185, 60,
                                255}; // Gold (brightened for contrast)
  afterhours::Color text_dark{45, 35, 25, 255};     // Dark text
  afterhours::Color text_light{235, 225, 210, 255}; // Light text

  // High-contrast text colors for tinted panels (accessibility)
  afterhours::Color text_on_gold{30, 20, 10, 255};    // Dark brown on gold
  afterhours::Color text_on_blue{20, 30, 50, 255};    // Dark blue on blue
  afterhours::Color text_on_red{255, 255, 255, 255};  // White on red
  afterhours::Color text_on_green{20, 40, 20, 255};   // Dark green on green

  size_t section = 0;
  size_t inspected_panel = 1;
  bool show_guides = false;
  int clicks = 0;
  bool textures_loaded = false;
  std::string base_path;

  // Panel textures - different styles
  raylib::Texture2D panel_000; // Simple rounded
  raylib::Texture2D panel_005; // Ornate corners
  raylib::Texture2D panel_010; // Double border
  raylib::Texture2D panel_015; // Thick border
  raylib::Texture2D panel_020; // Decorative
  raylib::Texture2D panel_025; // Heavy ornate

  // Border-only textures (transparent center)
  raylib::Texture2D border_000;
  raylib::Texture2D border_005;
  raylib::Texture2D border_010;

  // Transparent border assets (opaque white centers)
  raylib::Texture2D trans_border_000;
  raylib::Texture2D trans_border_010;

  // Double-width versions
  raylib::Texture2D double_panel_000;
  raylib::Texture2D double_panel_010;

  void load_textures_if_needed() {
    if (textures_loaded)
      return;
    textures_loaded = true;

    base_path = afterhours::files::get_resource_path(
                    "kenney", "kenney_fantasy-ui-borders/PNG")
                    .string();

    // Default panels
    panel_000 = raylib::LoadTexture(
        (base_path + "/Default/Panel/panel-000.png").c_str());
    panel_005 = raylib::LoadTexture(
        (base_path + "/Default/Panel/panel-005.png").c_str());
    panel_010 = raylib::LoadTexture(
        (base_path + "/Default/Panel/panel-010.png").c_str());
    panel_015 = raylib::LoadTexture(
        (base_path + "/Default/Panel/panel-015.png").c_str());
    panel_020 = raylib::LoadTexture(
        (base_path + "/Default/Panel/panel-020.png").c_str());
    panel_025 = raylib::LoadTexture(
        (base_path + "/Default/Panel/panel-025.png").c_str());

    // Border-only (transparent center)
    border_000 = raylib::LoadTexture(
        (base_path + "/Default/Border/panel-border-000.png").c_str());
    border_005 = raylib::LoadTexture(
        (base_path + "/Default/Border/panel-border-005.png").c_str());
    border_010 = raylib::LoadTexture(
        (base_path + "/Default/Border/panel-border-010.png").c_str());

    // Transparent border
    trans_border_000 = raylib::LoadTexture(
        (base_path +
         "/Default/Transparent border/panel-transparent-border-000.png")
            .c_str());
    trans_border_010 = raylib::LoadTexture(
        (base_path +
         "/Default/Transparent border/panel-transparent-border-010.png")
            .c_str());

    // Double-width panels (thicker borders)
    double_panel_000 = raylib::LoadTexture(
        (base_path + "/Double/Panel/panel-000.png").c_str());
    double_panel_010 = raylib::LoadTexture(
        (base_path + "/Double/Panel/panel-010.png").c_str());
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    load_textures_if_needed();
    Theme theme;
    theme.font = text_light;
    theme.darkfont = text_dark;
    theme.background = bg_dark;
    theme.surface = bg_medium;
    theme.primary = gold_accent;
    theme.secondary = {139, 90, 43, 255};
    theme.accent = gold_accent;
    theme.corner_radius = 0;
    context.theme = theme;
    context.scaling_mode = ScalingMode::Proportional;
    const float scale = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const float left = (context.screen_width / scale - 1144) / 2;
    const float top = (context.screen_height / scale - 720) / 2;
    const afterhours::Color muted{205, 190, 171, 255};
    const auto box = [scale, left, top](float x, float y, float width, float height) {
      return ComponentConfig{}.with_size({pixels(width * scale), pixels(height * scale)})
          .with_absolute_position((left + x) * scale, (top + y) * scale)
          .with_background(Theme::Usage::None).with_corner_radius(0);
    };
    // Background
    auto root = div(context, mk(entity), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background(bg_dark).with_corner_radius(0).with_debug_name("bg"));
    const auto label = [&](int id, const std::string &text, float x, float y, float width, float height,
                           float size, afterhours::Color color, const std::string &name = "") {
      return div(context, mk(root.ent(), id < 30 ? id : static_cast<int>(section) * 1000 + id), box(x, y, width, height).with_label(text)
          .with_font("AtkinsonMock", pixels(size * scale)).with_custom_text_color(color)
          .with_alignment(TextAlignment::Left).with_text_overflow(TextOverflow::Wrap)
          .with_ignore_pointer_events().with_debug_name(name));
    };
    const auto dimensions = [scale](float width, float height) {
      return fmt::format("{} x {}px", std::lround(width * scale), std::lround(height * scale));
    };
    const auto hex = [](afterhours::Color color) {
      return fmt::format("#{:02X}{:02X}{:02X}", color.r, color.g, color.b);
    };
    label(0, "9-slice borders", 0, 24, 1144, 46, 36, text_light, "ns_title");
    label(1, "Inspect source slices, transparent pixels, resizing, and tint independently.", 0, 78, 1144, 29, 22, muted);
    const std::array<const char *, 5> sections{"Panels", "Transparency", "Stretching", "Width & tint", "Examples"};
    for (size_t i = 0; i < sections.size(); ++i) {
      if (button(context, mk(root.ent(), 10 + static_cast<int>(i)), box(static_cast<float>(i) * 231, 121, 220, 44)
          .with_label(sections[i]).with_font("AtkinsonMock", pixels(23 * scale))
          .with_custom_background(section == i ? gold_accent : bg_medium)
          .with_custom_text_color(section == i ? text_dark : text_light)
          .with_corner_radius(7 * scale).with_debug_name("ns_tab_" + std::to_string(i)))) section = i;
    }
    div(context, mk(root.ent(), 20), box(0, 183, 1144, 490)
        .with_custom_background({45, 37, 30, 255}).with_corner_radius(10 * scale));
    label(21, "Kenney Fantasy UI / native nine-slice rendering / source corners keep their pixel dimensions", 0, 685, 1144, 27, 19, muted);
    const auto specimen = [&](int id, raylib::Texture2D texture, int slice, float x, float y,
                               float width, float height, const std::string &name,
                               afterhours::Color tint = afterhours::Color{255, 255, 255, 255}) {
      auto result = div(context, mk(root.ent(), id < 30 ? id : static_cast<int>(section) * 1000 + id), box(x, y, width, height)
          .with_nine_slice_border(texture, slice, tint).with_ignore_pointer_events().with_debug_name(name));
      result.ent().addComponentIfMissing<afterhours::HasColor>(afterhours::Color{0, 0, 0, 0}).set({0, 0, 0, 0});
      return result;
    };
    const auto checker = [](RectangleType r) {
      for (int y = 0; y < 8; ++y)
        for (int x = 0; x < 10; ++x)
          afterhours::draw_rectangle({r.x + static_cast<float>(x) * r.width / 10,
                                      r.y + static_cast<float>(y) * r.height / 8, r.width / 10, r.height / 8},
              (x + y) % 2 == 0 ? afterhours::Color{86, 97, 111, 255} : afterhours::Color{153, 161, 172, 255});
    };
    if (section == 0) {
      label(30, "Panel styles / filled centers", 20, 194, 690, 36, 28, gold_accent);
      label(31, "Default sources: 48 x 48px / uniform source slices: 16px", 20, 237, 690, 28, 20, muted);
      const std::array<raylib::Texture2D, 6> textures{panel_000, panel_005, panel_010, panel_015, panel_020, panel_025};
      const std::array<const char *, 6> names{"Simple", "Ornate", "Double Line", "Thick", "Decorative", "Heavy"};
      const std::array<const char *, 6> ids{"000", "005", "010", "015", "020", "025"};
      for (size_t i = 0; i < textures.size(); ++i) {
        const float x = 20 + static_cast<float>(i % 3) * 228;
        const float y = i < 3 ? 281.f : 471.f;
        auto config = box(x, y, 208, 100).with_label(names[i]).with_nine_slice_border(textures[i], 16)
            .with_font("AtkinsonMock", pixels(24 * scale)).with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center).with_debug_name("panel_" + std::to_string(i));
        config.with_on_draw_fg([guides = show_guides, chosen = inspected_panel == i, scale](RectangleType r) {
          if (guides) afterhours::draw_rectangle_outline({r.x + 16, r.y + 16, r.width - 32, r.height - 32}, {36, 90, 150, 255}, scale);
          if (chosen) afterhours::draw_rectangle_outline({r.x - 3 * scale, r.y - 3 * scale, r.width + 6 * scale, r.height + 6 * scale}, {230, 185, 60, 255}, 2 * scale);
        });
        if (button(context, mk(root.ent(), static_cast<int>(section) * 1000 + 40 + static_cast<int>(i)), config)) inspected_panel = i;
        label(50 + static_cast<int>(i), "panel-" + std::string(ids[i]) + ".png", x, y + 112, 208, 27, 21, text_light);
        label(60 + static_cast<int>(i), dimensions(208, 100), x, y + 143, 208, 24, 18, muted);
      }
      label(70, "Corner inspection", 744, 194, 380, 36, 28, gold_accent);
      label(71, std::string(names[inspected_panel]) + " / panel-" + ids[inspected_panel], 744, 237, 380, 29, 23, text_light, "ns_inspected");
      sprite(context, mk(root.ent(), static_cast<int>(section) * 1000 + 72), textures[inspected_panel], {0, 0, 16, 16},
          box(744, 286, 96, 96).with_ignore_pointer_events().with_debug_name("ns_corner_zoom"));
      label(73, "16 x 16 source pixels", 862, 286, 262, 29, 21, text_light);
      label(74, dimensions(96, 96) + " display", 862, 321, 262, 28, 20, muted);
      label(75, fmt::format("{:.1f}x magnification", 6 * scale), 862, 355, 262, 29, 20, muted);
      if (button(context, mk(root.ent(), static_cast<int>(section) * 1000 + 76), box(744, 416, 380, 44)
          .with_label(show_guides ? "Hide slice guides" : "Show slice guides")
          .with_font("AtkinsonMock", pixels(23 * scale)).with_custom_background(bg_medium)
          .with_custom_text_color(text_light).with_corner_radius(7 * scale).with_debug_name("ns_guides"))) show_guides = !show_guides;
      label(77, "Guides mark the 16px slice boundary, not the visible stroke width.", 744, 483, 380, 76, 23, text_light);
      label(78, "Select a panel to inspect its top-left corner. The same guide also exposes usable center space.", 744, 570, 380, 76, 21, muted);
      return;
    }
    if (section == 1) {
      label(30, "Transparency / inspect the actual pixels", 20, 194, 1104, 36, 28, gold_accent);
      label(31, "Checkerboards show through transparent pixels. White centers belong to the source image.", 20, 237, 1104, 29, 21, muted);
      label(32, "Border only / transparent centers", 20, 287, 646, 32, 25, text_light);
      label(33, "Transparent border assets", 692, 287, 432, 32, 25, text_light);
      const std::array<raylib::Texture2D, 5> textures{border_000, border_005, border_010, trans_border_000, trans_border_010};
      const std::array<const char *, 5> names{"Border 000 / simple", "Border 005 / ornate", "Border 010 / double", "Transparent border 000", "Transparent border 010"};
      for (size_t i = 0; i < textures.size(); ++i) {
        const float x = 20 + static_cast<float>(i) * 224;
        div(context, mk(root.ent(), static_cast<int>(section) * 1000 + 40 + static_cast<int>(i)), box(x, 336, 208, 164)
            .with_on_draw_bg(checker).with_nine_slice_border(textures[i], 16)
            .with_ignore_pointer_events().with_debug_name("border_" + std::to_string(i)));
        label(50 + static_cast<int>(i), names[i], x, 516, 208, 54, 21, text_light);
        label(60 + static_cast<int>(i), i < 3 ? "Center alpha: 0" : "Center alpha: 255", x, 578, 208, 27, 19, muted);
      }
      label(70, "All samples: 16px source slices. 'Transparent border' is an asset folder name, not a transparent-center guarantee.", 20, 622, 1104, 43, 20, muted);
      return;
    }
    if (section == 2) {
      label(30, "Stretching / original sizes, aligned bottoms", 20, 194, 1104, 36, 28, gold_accent);
      label(31, "Small, Medium, Large: panel-010 / Wide: panel-015 / Tall: panel-005", 20, 237, 1104, 28, 21, muted);
      const std::array<float, 5> widths{80, 130, 180, 350, 70};
      const std::array<float, 5> heights{55, 70, 85, 55, 110};
      const std::array<float, 5> xs{20, 132, 292, 504, 924};
      const std::array<const char *, 5> names{"Small", "Medium", "Large", "Wide / menu-shaped example", "Tall"};
      const std::array<raylib::Texture2D, 5> textures{panel_010, panel_010, panel_010, panel_015, panel_005};
      for (size_t i = 0; i < widths.size(); ++i) {
        specimen(40 + static_cast<int>(i), textures[i], 16, xs[i], 390 - heights[i], widths[i], heights[i], "ns_original_size_" + std::to_string(i));
        label(50 + static_cast<int>(i), names[i], xs[i], 398, std::max(widths[i], 100.f), 27, 21, text_light);
        label(60 + static_cast<int>(i), dimensions(widths[i], heights[i]), xs[i], 428, std::max(widths[i], 100.f), 24, 18, muted);
      }
      label(70, "Same-source comparison / panel-010 everywhere / 16px source slices", 20, 473, 1104, 32, 25, gold_accent);
      for (size_t i = 0; i < widths.size(); ++i) {
        specimen(80 + static_cast<int>(i), panel_010, 16, xs[i], 623 - heights[i], widths[i], heights[i], "ns_same_size_" + std::to_string(i));
        label(90 + static_cast<int>(i), dimensions(widths[i], heights[i]), xs[i], 635, std::max(widths[i], 100.f), 25, 18, muted);
      }
      return;
    }
    if (section == 3) {
      label(30, "Width / 16px default and 32px double source slices", 20, 194, 1104, 36, 28, gold_accent);
      const std::array<raylib::Texture2D, 3> widths{panel_000, double_panel_000, double_panel_010};
      const std::array<const char *, 3> width_names{"Default / panel-000", "Double / panel-000", "Double / panel-010"};
      for (size_t i = 0; i < widths.size(); ++i) {
        const float x = 20 + static_cast<float>(i) * 244;
        specimen(40 + static_cast<int>(i), widths[i], i == 0 ? 16 : 32, x, 246, 220, 116, "ns_width_" + std::to_string(i));
        label(50 + static_cast<int>(i), width_names[i], x, 373, 220, 28, 21, text_light);
        label(60 + static_cast<int>(i), i == 0 ? "48 x 48 source / 16px" : "96 x 96 source / 32px", x, 405, 220, 25, 18, muted);
      }
      label(70, "Same destination size", 768, 254, 356, 33, 25, gold_accent);
      label(71, dimensions(220, 116), 768, 298, 356, 30, 23, text_light);
      label(72, "Double sources keep larger corner and edge slices.", 768, 346, 356, 67, 22, muted);
      label(73, "Tint / border color and text color are chosen separately", 20, 451, 1104, 34, 26, gold_accent);
      const std::array<afterhours::Color, 4> tints{gold_accent, afterhours::Color{100, 150, 220, 255}, afterhours::Color{200, 80, 80, 255}, afterhours::Color{80, 180, 100, 255}};
      const std::array<afterhours::Color, 4> text_colors{text_on_gold, text_on_blue, text_on_red, text_on_green};
      const std::array<const char *, 4> names{"Gold Tint", "Blue Tint", "Red Tint", "Green Tint"};
      for (size_t i = 0; i < tints.size(); ++i) {
        const float x = 20 + static_cast<float>(i) * 278;
        specimen(80 + static_cast<int>(i), panel_010, 16, x, 497, 254, 90,
            "ns_tint_" + std::to_string(i), tints[i]);
        label(90 + static_cast<int>(i), names[i], x + 24, 523, 206, 36, 25, text_colors[i]);
        label(100 + static_cast<int>(i), "Tint " + hex(tints[i]), x, 597, 254, 27, 20, text_light);
        label(110 + static_cast<int>(i), "Text " + hex(text_colors[i]), x, 630, 254, 26, 19, muted);
      }
      return;
    }

    // Interactive button example
    label(30, "Examples / controls, dialogs, and real nested panels", 20, 194, 1104, 36, 28, gold_accent);
    if (button(context, mk(root.ent(), static_cast<int>(section) * 1000 + 40), box(20, 253, 264, 88).with_label("Click Me!")
        .with_nine_slice_border(panel_020, 16).with_font("AtkinsonMock", pixels(26 * scale))
        .with_custom_text_color(text_dark).with_alignment(TextAlignment::Center).with_debug_name("ns_button"))) {
      ++clicks;
      log_info("Nine-slice button clicked!");
    }
    label(41, "Each click increments the counter.", 20, 362, 290, 64, 23, text_light);
    label(42, "Button clicks: " + std::to_string(clicks), 20, 442, 290, 40, 29, gold_accent, "ns_clicks");
    label(43, "Tab + Enter works too.\nSource: panel-020 / 16px slices", 20, 510, 290, 82, 21, muted);
    div(context, mk(root.ent(), static_cast<int>(section) * 1000 + 50), box(332, 253, 372, 220)
        .with_custom_background(parchment).with_nine_slice_border(border_010, 16)
        .with_debug_name("dialog_box"));
    label(51, "Bordered dialog", 358, 275, 320, 39, 28, text_dark);
    label(52, "Corners keep their shape as this frame grows.", 358, 334, 320, 92, 24, text_dark);
    label(53, "panel-border-010 / 16px slices", 332, 488, 372, 30, 21, muted);
    label(54, "Separate parchment fill beneath a transparent-center border.", 332, 531, 372, 87, 22, text_light);
    auto outer = div(context, mk(root.ent(), static_cast<int>(section) * 1000 + 60), box(752, 253, 372, 220)
        .with_custom_background(parchment).with_nine_slice_border(border_010, 16)
        .with_padding(Padding::all(pixels(20 * scale))).with_flex_direction(FlexDirection::Column)
        .with_debug_name("nested_outer"));
    div(context, mk(outer.ent(), 0), ComponentConfig{}.with_size({percent(1.f), pixels(42 * scale)})
        .with_label("Nested panels").with_font("AtkinsonMock", pixels(27 * scale))
        .with_custom_text_color(text_dark).with_background(Theme::Usage::None).with_corner_radius(0));
    auto children_row = hstack(context, mk(outer.ent(), 1), ComponentConfig{}
        .with_size({percent(1.f), pixels(108 * scale)}).with_gap(pixels(16 * scale))
        .with_margin(Margin{.top = pixels(12 * scale)}).with_background(Theme::Usage::None));
    div(context, mk(children_row.ent(), 0), ComponentConfig{}.with_size({pixels(146 * scale), pixels(104 * scale)})
        .with_label("Gold child").with_nine_slice_border(border_005, 16, {180, 140, 80, 255})
        .with_font("AtkinsonMock", pixels(22 * scale)).with_custom_text_color(text_dark)
        .with_alignment(TextAlignment::Center).with_background(Theme::Usage::None).with_corner_radius(0).with_debug_name("ns_gold_child"));
    div(context, mk(children_row.ent(), 1), ComponentConfig{}.with_size({pixels(146 * scale), pixels(104 * scale)})
        .with_label("Blue child").with_nine_slice_border(border_010, 16, {100, 140, 180, 255})
        .with_font("AtkinsonMock", pixels(22 * scale)).with_custom_text_color(text_dark)
        .with_alignment(TextAlignment::Center).with_background(Theme::Usage::None).with_corner_radius(0).with_debug_name("ns_blue_child"));
    label(70, "Light backing", 752, 492, 176, 28, 21, text_light);
    label(71, "Dark backing", 948, 492, 176, 28, 21, text_light);
    specimen(72, border_010, 16, 752, 532, 176, 96, "ns_light_backing")
        .ent().addComponentIfMissing<afterhours::HasColor>(parchment).set(parchment);
    specimen(73, border_010, 16, 948, 532, 176, 96, "ns_dark_backing")
        .ent().addComponentIfMissing<afterhours::HasColor>(bg_medium).set(bg_medium);
    label(74, "Same border texture / only the fill changes", 752, 640, 372, 27, 18, muted);
  }
};

REGISTER_EXAMPLE_SCREEN(nine_slice_borders, "Component Galleries",
                        "9-slice border textures",
                        ExampleNineSliceBordersScreen)
