#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <string>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct KartSelectScreen : ScreenSystem<UIContext<InputAction>> {
  struct Character {
    std::string name;
    float speed;
    float acceleration;
    float handling;
    float weight;
  };

  size_t selected_character = 2;
  size_t selected_kart = 0;
  bool ready = false;
  bool loaded = false;
  std::string status;
  raylib::Texture2D portraits_texture{};
  raylib::Texture2D karts_texture{};
  raylib::Texture2D big_karts_texture{};

  const std::array<Character, 8> characters{{
      {"Turbo", .9f, .5f, .4f, .7f},
      {"Blaze", .7f, .8f, .6f, .5f},
      {"Drift", .6f, .6f, .9f, .4f},
      {"Tank", .4f, .3f, .5f, 1.f},
      {"Zippy", .8f, .9f, .7f, .3f},
      {"Rumble", .5f, .4f, .7f, .9f},
      {"Flash", 1.f, .6f, .3f, .6f},
      {"Pebble", .3f, .7f, 1.f, .5f},
  }};
  const std::array<std::string, 4> kart_names{"Standard", "Speedster",
                                               "Off-Road", "Classic"};

  const afterhours::Color deep_blue{6, 71, 125, 255};
  const afterhours::Color bright_blue{58, 179, 230, 255};
  const afterhours::Color navy{22, 71, 117, 255};
  const afterhours::Color white{255, 255, 255, 255};
  const afterhours::Color yellow{255, 221, 54, 255};
  const afterhours::Color cyan_line{140, 224, 255, 255};

  ComponentConfig box(float scale, float x, float y, float width,
                      float height) const {
    return ComponentConfig{}
        .with_size({pixels(width * scale), pixels(height * scale)})
        .with_absolute_position(pixels(x * scale), pixels(y * scale))
        .with_background(Theme::Usage::None).with_corner_radius(0);
  }

  static void draw_skew_plate(RectangleType r, float offset,
                              afterhours::Color color) {
    Vector2Type a{r.x + offset, r.y};
    Vector2Type b{r.x + r.width, r.y};
    Vector2Type c{r.x + r.width - offset, r.y + r.height};
    Vector2Type d{r.x, r.y + r.height};
    afterhours::draw_triangle(a, d, b, color);
    afterhours::draw_triangle(b, d, c, color);
  }

  static void draw_background(RectangleType r, float scale) {
    afterhours::draw_rectangle_gradient_h(
        {r.x, r.y, r.width, 654.f * scale},
        afterhours::Color{5, 69, 123, 255},
        afterhours::Color{54, 174, 224, 255});
    for (int row = 0; row < 26; ++row)
      for (int col = 0; col < 50; ++col)
        if ((row + col) % 2 == 0)
          afterhours::draw_rectangle(
              {r.x + static_cast<float>(col) * 26.f * scale,
               r.y + static_cast<float>(row) * 26.f * scale,
               26.f * scale, 26.f * scale},
              afterhours::Color{255, 255, 255, 3});

    afterhours::draw_rectangle(
        {r.x, r.y, 730.f * scale, 84.f * scale},
        afterhours::Color{244, 247, 249, 255});
    afterhours::draw_triangle(
        {r.x + 730.f * scale, r.y},
        {r.x + 730.f * scale, r.y + 84.f * scale},
        {r.x + 759.f * scale, r.y}, afterhours::Color{244, 247, 249, 255});
    afterhours::draw_rectangle(
        {r.x, r.y + 84.f * scale, 1280.f * scale, 4.f * scale},
        afterhours::Color{140, 224, 255, 255});

    afterhours::draw_rectangle(
        {r.x, r.y + 654.f * scale, 1280.f * scale, 66.f * scale},
        afterhours::Color{244, 246, 247, 255});
    afterhours::draw_rectangle(
        {r.x, r.y + 654.f * scale, 1280.f * scale, 4.f * scale},
        afterhours::Color{192, 237, 255, 255});
  }

  static void draw_driver_card(RectangleType r, bool selected, float scale) {
    if (selected) {
      afterhours::draw_rectangle_rounded(
          {r.x - 2.f * scale, r.y - 2.f * scale, r.width + 4.f * scale,
           r.height + 4.f * scale},
          .04f, 8, afterhours::Color{255, 255, 255, 255},
          RoundedCorners().all_round());
      afterhours::draw_rectangle_gradient_h(
          r, afterhours::Color{255, 255, 255, 255},
          afterhours::Color{255, 244, 180, 255});
      afterhours::draw_rectangle_rounded_lines_ex(
          r, .04f, 8, 5.f * scale, afterhours::Color{255, 213, 42, 255});
      return;
    }
    afterhours::draw_rectangle_gradient_h(
        r, afterhours::Color{159, 216, 238, 255},
        afterhours::Color{108, 162, 203, 255});
    afterhours::draw_rectangle_rounded_lines_ex(
        r, .04f, 8, 3.f * scale, afterhours::Color{218, 246, 255, 255});
  }

  static void draw_vehicle_card(RectangleType r, bool selected, float scale) {
    afterhours::draw_rectangle(
        r, selected ? afterhours::Color{255, 221, 54, 255}
                    : afterhours::Color{228, 244, 250, 255});
    afterhours::draw_rectangle_rounded_lines_ex(
        r, .03f, 6, 2.f * scale,
        selected ? afterhours::Color{255, 244, 172, 255}
                 : afterhours::Color{255, 255, 255, 255});
  }

  static void draw_stat_bar(RectangleType r, float value, float scale) {
    afterhours::draw_rectangle(r, afterhours::Color{0, 44, 75, 255});
    afterhours::draw_rectangle(
        {r.x, r.y, r.width * std::clamp(value, 0.f, 1.f), r.height},
        afterhours::Color{255, 214, 56, 255});
    for (int i = 1; i < 10; ++i)
      afterhours::draw_rectangle(
          {r.x + r.width * static_cast<float>(i) / 10 - scale, r.y, 2 * scale, r.height},
          afterhours::Color{0, 44, 75, 255});
  }

  static void draw_portrait_detail(RectangleType r, size_t index) {
    const float s = r.width / 230;
    const auto point = [&](float x, float y) { return Vector2Type{r.x + x * s, r.y + y * s}; };
    const afterhours::Color dark{35, 46, 64, 255};
    const auto line = [&](float x1, float y1, float x2, float y2, float width) {
      afterhours::draw_line_ex(point(x1, y1), point(x2, y2), width * s, dark);
    };
    const auto dot = [&](float x, float y, float radius, afterhours::Color color) {
      const auto p = point(x, y);
      afterhours::draw_circle(static_cast<int>(p.x), static_cast<int>(p.y), radius * s, color);
    };
    if (index == 0 || index == 5) {
      for (float x : {102.f, 114.f, 126.f}) dot(x, 157, index == 0 ? 6.f : 8.f, dark);
      return;
    }
    if (index == 1 || index == 7) {
      for (float x : {91.f, 136.f}) {
        const auto p = point(x, 120);
        afterhours::draw_circle_lines(static_cast<int>(p.x), static_cast<int>(p.y), 18 * s, dark);
      }
      line(109, 120, 118, 120, 3);
      if (index == 7) line(67, 101, 156, 101, 5);
      return;
    }
    if (index == 2) {
      for (float x : {77.f, 85.f, 143.f, 151.f}) dot(x, 143, 2.5f, {166, 81, 55, 255});
      return;
    }
    if (index == 3) {
      line(76, 99, 105, 104, 5);
      line(124, 104, 153, 99, 5);
      return;
    }
    if (index == 4) {
      line(58, 198, 83, 214, 7);
      line(58, 210, 78, 226, 7);
      return;
    }
    line(153, 39, 142, 58, 5);
    line(142, 58, 157, 55, 5);
    line(157, 55, 144, 77, 5);
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    UIStylingDefaults::get().default_font_name = "ArchivoMockBold";
    const float scale = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    Theme theme;
    theme.font = white;
    theme.darkfont = navy;
    theme.background = deep_blue;
    theme.surface = deep_blue;
    theme.primary = yellow;
    theme.secondary = bright_blue;
    theme.accent = yellow;
    theme.roundness = 0.f;
    theme.corner_radius = 0.f;
    theme.segments = 16;
    context.set_theme(theme);
    context.scaling_mode = ScalingMode::Proportional;

    if (!loaded) {
      loaded = true;
      portraits_texture = raylib::LoadTexture(
          afterhours::files::get_resource_path(
              "images", "kart_select/portraits.png")
              .string()
              .c_str());
      karts_texture = raylib::LoadTexture(
          afterhours::files::get_resource_path("images",
                                                "kart_select/karts.png")
              .string()
              .c_str());
      big_karts_texture = raylib::LoadTexture(
          afterhours::files::get_resource_path(
              "images", "kart_select/big_karts.png")
              .string()
              .c_str());
      raylib::SetTextureFilter(portraits_texture,
                               raylib::TEXTURE_FILTER_BILINEAR);
      raylib::SetTextureFilter(karts_texture, raylib::TEXTURE_FILTER_BILINEAR);
      raylib::SetTextureFilter(big_karts_texture,
                               raylib::TEXTURE_FILTER_BILINEAR);
    }

    if (context.pressed(InputAction::WidgetRight)) {
      selected_character = (selected_character + 1) % characters.size();
      ready = false;
      status = "Selected " + characters[selected_character].name;
    }
    if (context.pressed(InputAction::WidgetLeft)) {
      selected_character =
          (selected_character + characters.size() - 1) % characters.size();
      ready = false;
      status = "Selected " + characters[selected_character].name;
    }
    if (context.pressed(InputAction::WidgetDown)) {
      selected_kart = (selected_kart + 1) % kart_names.size();
      ready = false;
      status = "Selected " + kart_names[selected_kart];
    }
    if (context.pressed(InputAction::WidgetUp)) {
      selected_kart =
          (selected_kart + kart_names.size() - 1) % kart_names.size();
      ready = false;
      status = "Selected " + kart_names[selected_kart];
    }

    auto background = div(context, mk(entity, 0), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background(deep_blue).with_corner_radius(0).with_debug_name("kart_root"));
    auto root =
        div(context, mk(background.ent(), 0),
            box(scale, 0.f, 0.f, 1280.f, 720.f)
                .with_absolute_position((context.screen_width - 1280 * scale) / 2,
                                        (context.screen_height - 720 * scale) / 2)
                .with_on_draw_bg([scale](RectangleType r) {
                  draw_background(r, scale);
                })
                .with_debug_name("kart_content"));

    auto text = [&](int id, const std::string &label, float x, float y,
                    float width, float height, float size,
                    afterhours::Color color,
                    TextAlignment alignment = TextAlignment::Left,
                    const std::string &font = "ArchivoMockBold",
                    const std::string &debug_name = "") {
      return div(context, mk(root.ent(), id),
                 box(scale, x, y, width, height)
                     .with_label(label)
                     .with_font(font, pixels(size * scale))
                     .with_custom_text_color(color)
                     .with_alignment(alignment)
                     .with_text_inset(0.f, 0.f).with_ignore_pointer_events().with_debug_name(debug_name));
    };

    text(1, "SELECT YOUR RACER", 44.f, 4.f, 620.f, 48.f, 42.f, navy,
         TextAlignment::Left);
    text(2, "Grand Prix · Mushroom Cup", 44.f, 51.f, 660.f, 27.f, 23.f,
         navy, TextAlignment::Left);
    div(context, mk(root.ent(), 3),
        box(scale, 1198.f, 27.f, 32.f, 32.f)
            .with_label("P1")
            .with_font("ArchivoMockBold", pixels(22.f * scale))
            .with_custom_background(yellow).with_border(navy, 2 * scale)
            .with_custom_text_color(afterhours::Color{38, 60, 85, 255})
            .with_alignment(TextAlignment::Center)
            .with_text_inset(0.f, 0.f)
            .with_rounded_corners(RoundedCorners().all_sharp())
            .with_debug_name("kart_player"));

    auto portrait = [&](int id, size_t index, float x, float y, float width, float height) {
      const float source_x = static_cast<float>(index % 4) * 230.f;
      const float source_y = static_cast<float>(index / 4) * 250.f;
      sprite(context, mk(root.ent(), id), portraits_texture,
             {source_x, source_y, 230.f, 250.f},
             box(scale, x, y, width, height).with_ignore_pointer_events()
                 .with_on_draw_fg([index](RectangleType r) { draw_portrait_detail(r, index); }));
    };
    auto kart_thumb = [&](int id, size_t index, float x, float y) {
      portrait(id + 300, selected_character, x + 47, y + 14, 30, 32.61f);
      sprite(context, mk(root.ent(), id), karts_texture,
             {static_cast<float>(index) * 240, 60, 240, 90},
             box(scale, x + 10, y + 37.5f, 100, 37.5f).with_ignore_pointer_events());
    };

    for (size_t i = 0; i < characters.size(); ++i) {
      const float x = 40.f + static_cast<float>(i % 4) * 148.f;
      const float base_y = 117.f + static_cast<float>(i / 4) * 164.f;
      const bool selected = selected_character == i;
      const float y = base_y;
      auto card =
          button(context, mk(root.ent(), 20 + static_cast<int>(i)),
                 box(scale, x, y, 139.f, 155.f)
                     .with_click_activation(ClickActivationMode::Release)
                     .with_on_draw_bg([selected, scale](RectangleType r) {
                       draw_driver_card(r, selected, scale);
                     })
                     .with_debug_name("kart_driver_" + std::to_string(i)));
      portrait(40 + static_cast<int>(i), i, x + 28.1f, y + 30, 82.8f, 90);
      div(context, mk(root.ent(), 60 + static_cast<int>(i)),
          box(scale, x + 3.f, y + 119.f, 133.f, 33.f)
              .with_custom_background(afterhours::Color{217, 242, 250, 238})
              .with_rounded_corners(RoundedCorners().all_sharp())
              .with_corner_radius(0.f)
              .with_ignore_pointer_events());
      text(80 + static_cast<int>(i), characters[i].name, x + 3.f, y + 120.f,
           133.f, 30.f, 24.f, navy, TextAlignment::Center);
      if (selected)
        div(context, mk(root.ent(), 100 + static_cast<int>(i)),
            box(scale, x + 7.f, y + 5.f, 32.f, 32.f)
                .with_label("P1")
                .with_font("ArchivoMockBold", pixels(22.f * scale))
                .with_custom_background(yellow)
                .with_border(navy, 2.f * scale)
                .with_custom_text_color(afterhours::Color{38, 60, 85, 255})
                .with_alignment(TextAlignment::Center)
                .with_text_inset(0.f, 0.f)
                .with_ignore_pointer_events()
                .with_debug_name("kart_selected_badge"));
      if (card) {
        selected_character = i;
        ready = false;
        status = selected ? "" : "Selected " + characters[i].name;
      }
    }

    text(120, "CHOOSE YOUR VEHICLE", 40.f, 450.f, 430.f, 35.f, 26.f, white);
    for (size_t i = 0; i < kart_names.size(); ++i) {
      const float x = 40.f + static_cast<float>(i) * 148.f;
      const bool selected = selected_kart == i;
      auto card =
          button(context, mk(root.ent(), 130 + static_cast<int>(i)),
                 box(scale, x, 491.f, 139.f, 117.f)
                     .with_click_activation(ClickActivationMode::Release)
                     .with_on_draw_bg([selected, scale](RectangleType r) {
                       draw_vehicle_card(r, selected, scale);
                     })
                     .with_debug_name("kart_vehicle_" + std::to_string(i)));
      kart_thumb(140 + static_cast<int>(i), i, x + 10.f, 501.f);
      text(150 + static_cast<int>(i), kart_names[i], x + 3.f, 578.f, 133.f,
           27.f, 22.f, navy, TextAlignment::Center);
      if (selected)
        div(context, mk(root.ent(), 160 + static_cast<int>(i)), box(scale, x + 5, 496, 87, 21)
            .with_label("SELECTED").with_font("ArchivoMockBold", pixels(15 * scale))
            .with_custom_background(navy).with_custom_text_color(white).with_ignore_pointer_events()
            .with_alignment(TextAlignment::Center).with_text_inset(0, 0).with_debug_name("kart_vehicle_selected"));
      if (card) {
        selected_kart = i;
        ready = false;
        status = selected ? "" : "Selected " + kart_names[i];
      }
    }

    text(169, "Selected racer and vehicle", 648, 98, 592, 32, 25, white, TextAlignment::Center);
    div(context, mk(root.ent(), 170), box(scale, 648, 398, 592, 34)
        .with_on_draw_bg([scale](RectangleType r) {
          const float cx = r.x + r.width * .5f;
          afterhours::draw_ellipse(static_cast<int>(cx), static_cast<int>(r.y + 21 * scale),
              258 * scale, 13 * scale, afterhours::Color{24, 105, 153, 255});
          afterhours::draw_ellipse(static_cast<int>(cx), static_cast<int>(r.y + 13 * scale),
              258 * scale, 13 * scale, afterhours::Color{223, 247, 255, 255});
        }).with_ignore_pointer_events().with_debug_name("kart_platform"));
    portrait(173, selected_character, 855, 133, 150, 163.04f);
    sprite(context, mk(root.ent(), 171), big_karts_texture,
           {static_cast<float>(selected_kart) * 1184, 240, 1184, 342},
           box(scale, 648, 253, 592, 171).with_ignore_pointer_events().with_debug_name("kart_preview"));
    text(172, characters[selected_character].name + " / " + kart_names[selected_kart],
         648, 431, 592, 39, 32, white, TextAlignment::Center, "ArchivoMockBold", "kart_preview_identity");
    div(context, mk(root.ent(), 180), box(scale, 662, 477, 578, 166)
        .with_custom_background(afterhours::Color{0, 47, 86, 230})
        .with_debug_name("kart_stats"));
    text(181, "Driver ratings / 0-10", 680, 482, 542, 27, 22, white);
    const auto &selected = characters[selected_character];
    const std::array<std::string, 4> stat_names{"Speed", "Acceleration", "Handling", "Weight"};
    const std::array<float, 4> stat_values{selected.speed, selected.acceleration, selected.handling, selected.weight};
    for (size_t i = 0; i < stat_names.size(); ++i) {
      const float y = 511 + static_cast<float>(i) * 27;
      text(190 + static_cast<int>(i), stat_names[i], 680, y, 146, 26, 22, white);
      div(context, mk(root.ent(), 200 + static_cast<int>(i)), box(scale, 843, y + 8, 276, 12)
          .with_on_draw_bg([value = stat_values[i], scale](RectangleType r) { draw_stat_bar(r, value, scale); })
          .with_debug_name("kart_stat_" + std::to_string(i)));
      text(210 + static_cast<int>(i), std::to_string(std::lround(stat_values[i] * 10)) + "/10",
           1135, y, 84, 26, 22, white, TextAlignment::Right, "ArchivoMockBold",
           "kart_stat_value_" + std::to_string(i));
    }
    text(220, "Weight is mass, not a handling bonus. Karts are cosmetic.", 680, 619, 542, 22, 18, cyan_line);

    auto back =
        button(context, mk(root.ent(), 230),
               box(scale, 37.f, 666.f, 156.f, 47.f)
                   .with_on_draw_bg(
                       [scale, arrow_color = afterhours::Color{255, 255, 255,
                                                               255}](
                           RectangleType r) {
                     draw_skew_plate(r, 7.f * scale,
                                     afterhours::Color{19, 104, 157, 255});
                     const float x = r.x + 25.f * scale;
                     const float cy = r.y + r.height * .5f;
                     afterhours::draw_line_ex(
                         {x + 6.f * scale, cy - 7.f * scale}, {x, cy},
                         2.5f * scale, arrow_color);
                     afterhours::draw_line_ex(
                         {x, cy}, {x + 6.f * scale, cy + 7.f * scale},
                         2.5f * scale, arrow_color);
                       })
                   .with_debug_name("kart_back"));
    text(231, "Back / Esc", 71.f, 666.f, 110.f, 47.f, 23.f, white,
         TextAlignment::Center);
    if (back || context.pressed(InputAction::MenuBack)) {
      ready = false;
      status = "Back selected";
    }

    text(232, "Left / Right: Racer", 219, 663, 252, 27, 22, navy);
    text(233, "Up / Down: Vehicle", 219, 691, 252, 27, 22, navy);
    text(234, "Tab / Enter: Activate", 490, 675, 314, 32, 22, navy);
    text(235, "Confirm this lineup", 842, 663, 188, 24, 18, navy, TextAlignment::Right);
    text(236, "Selection demo", 842, 691, 188, 23, 18, navy, TextAlignment::Right);

    auto ready_button =
        button(context, mk(root.ent(), 240),
               box(scale, 1048.f, 662.f, 195.f, 56.f)
                   .with_on_draw_bg([scale](RectangleType r) {
                     draw_skew_plate(r, 7.f * scale,
                                     afterhours::Color{255, 224, 59, 255});
                     afterhours::draw_rectangle(
                         {r.x, r.y + r.height - 3.f * scale, r.width,
                          3.f * scale},
                         afterhours::Color{205, 158, 9, 255});
                     const afterhours::Color arrow{30, 63, 84, 255};
                     const float x = r.x + 177.f * scale;
                     const float cy = r.y + r.height * .5f;
                     afterhours::draw_line_ex(
                         {x - 6.f * scale, cy - 7.f * scale}, {x, cy},
                         2.5f * scale, arrow);
                     afterhours::draw_line_ex(
                         {x, cy}, {x - 6.f * scale, cy + 7.f * scale},
                         2.5f * scale, arrow);
                   })
                   .with_debug_name("kart_ready"));
    text(241, ready ? "CONFIRMED" : "READY!", 1060.f, 662.f, 148.f, 56.f,
         26.f, afterhours::Color{30, 63, 84, 255}, TextAlignment::Center);
    if (ready_button) {
      ready = true;
      status = "Ready to race: " + characters[selected_character].name +
               " + " + kart_names[selected_kart];
    }

    if (!status.empty())
      text(250, status, 40.f, 614.f, 588.f, 32.f, 20.f,
           white, TextAlignment::Left);
  }
};

REGISTER_EXAMPLE_SCREEN(kart_select, "Game Mockups",
                        "Racing game character and kart selection",
                        KartSelectScreen)
