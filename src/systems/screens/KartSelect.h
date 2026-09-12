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
        .with_background(Theme::Usage::None);
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
              afterhours::Color{255, 255, 255, 7});

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
    for (float x = 25.f; x < 326.f; x += 28.f) {
      const bool filled = x < 326.f * value;
      afterhours::draw_rectangle(
          {r.x + x * scale, r.y, 3.f * scale, r.height},
          filled ? afterhours::Color{255, 237, 143, 255}
                 : afterhours::Color{99, 147, 180, 255});
    }
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    const float scale =
        context.screen_height > 0.f ? context.screen_height / 720.f : 1.f;
    Theme theme;
    theme.font = white;
    theme.darkfont = navy;
    theme.background = deep_blue;
    theme.surface = deep_blue;
    theme.primary = yellow;
    theme.secondary = bright_blue;
    theme.accent = yellow;
    theme.roundness = 0.f;
    theme.segments = 16;
    context.set_theme(theme);
    context.scaling_mode = ScalingMode::Adaptive;
    UIStylingDefaults::get().set_default_font("ArchivoMockBold", pixels(20.f));

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

    auto root =
        div(context, mk(entity, 0),
            box(scale, 0.f, 0.f, 1280.f, 720.f)
                .with_on_draw_bg([scale](RectangleType r) {
                  draw_background(r, scale);
                })
                .with_debug_name("kart_root"));

    auto text = [&](int id, const std::string &label, float x, float y,
                    float width, float height, float size,
                    afterhours::Color color,
                    TextAlignment alignment = TextAlignment::Left,
                    const std::string &font = "ArchivoMockBold") {
      return div(context, mk(root.ent(), id),
                 box(scale, x, y, width, height)
                     .with_label(label)
                     .with_font(font, pixels(size * scale))
                     .with_custom_text_color(color)
                     .with_alignment(alignment)
                     .with_text_inset(0.f, 0.f));
    };

    text(1, "SELECT YOUR RACER", 44.f, 10.f, 430.f, 68.f, 46.f, navy,
         TextAlignment::Left);
    text(2, "Grand Prix · Mushroom Cup", 900.f, 17.f, 268.f, 52.f, 25.f,
         white, TextAlignment::Right);
    div(context, mk(root.ent(), 3),
        box(scale, 1191.f, 20.f, 47.f, 45.f)
            .with_label("P1")
            .with_font("ArchivoMockBold", pixels(26.f * scale))
            .with_custom_background(afterhours::Color{255, 228, 65, 255})
            .with_custom_text_color(afterhours::Color{38, 60, 85, 255})
            .with_alignment(TextAlignment::Center)
            .with_text_inset(0.f, 0.f)
            .with_rounded_corners(RoundedCorners().all_sharp())
            .with_debug_name("kart_player"));

    auto portrait = [&](int id, size_t index, float x, float y) {
      const float source_x = static_cast<float>(index % 4) * 230.f;
      const float source_y = static_cast<float>(index / 4) * 250.f;
      sprite(context, mk(root.ent(), id), portraits_texture,
             {source_x, source_y, 230.f, 250.f},
             box(scale, x, y, 115.f, 125.f).with_ignore_pointer_events());
    };
    auto kart_thumb = [&](int id, size_t index, float x, float y) {
      const float source_x = static_cast<float>(index % 4) * 240.f;
      const float source_y = static_cast<float>(index / 4) * 150.f;
      sprite(context, mk(root.ent(), id), karts_texture,
             {source_x, source_y, 240.f, 150.f},
             box(scale, x, y, 120.f, 75.f).with_ignore_pointer_events());
    };

    for (size_t i = 0; i < characters.size(); ++i) {
      const float x = 40.f + static_cast<float>(i % 4) * 148.f;
      const float base_y = 117.f + static_cast<float>(i / 4) * 164.f;
      const bool selected = selected_character == i;
      const float y = base_y - (selected ? 2.f : 0.f);
      auto card =
          button(context, mk(root.ent(), 20 + static_cast<int>(i)),
                 box(scale, x, y, 139.f, 155.f)
                     .with_click_activation(ClickActivationMode::Release)
                     .with_on_draw_bg([selected, scale](RectangleType r) {
                       draw_driver_card(r, selected, scale);
                     })
                     .with_debug_name("kart_driver_" + std::to_string(i)));
      portrait(40 + static_cast<int>(i), i, x + 12.f, y);
      div(context, mk(root.ent(), 60 + static_cast<int>(i)),
          box(scale, x + 3.f, y + 128.f, 133.f, 24.f)
              .with_custom_background(afterhours::Color{217, 242, 250, 238})
              .with_rounded_corners(RoundedCorners().all_sharp())
              .with_corner_radius(0.f)
              .with_ignore_pointer_events());
      text(80 + static_cast<int>(i), characters[i].name, x + 3.f, y + 127.f,
           133.f, 26.f, 20.f, navy, TextAlignment::Center);
      if (selected)
        div(context, mk(root.ent(), 100 + static_cast<int>(i)),
            box(scale, x - 3.f, y - 9.f, 30.f, 31.f)
                .with_label("P1")
                .with_font("ArchivoMockBold", pixels(19.f * scale))
                .with_custom_background(afterhours::Color{255, 213, 42, 255})
                .with_border(white, 2.f * scale)
                .with_custom_text_color(afterhours::Color{54, 71, 81, 255})
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

    text(120, "CHOOSE YOUR VEHICLE", 40.f, 451.f, 360.f, 35.f, 20.f, white);
    for (size_t i = 0; i < kart_names.size(); ++i) {
      const float x = 40.f + static_cast<float>(i) * 148.f;
      const bool selected = selected_kart == i;
      auto card =
          button(context, mk(root.ent(), 130 + static_cast<int>(i)),
                 box(scale, x, 489.f, 139.f, 110.f)
                     .with_click_activation(ClickActivationMode::Release)
                     .with_on_draw_bg([selected, scale](RectangleType r) {
                       draw_vehicle_card(r, selected, scale);
                     })
                     .with_debug_name("kart_vehicle_" + std::to_string(i)));
      kart_thumb(140 + static_cast<int>(i), i, x + 10.f, 491.f);
      text(150 + static_cast<int>(i), kart_names[i], x + 3.f, 572.f, 133.f,
           25.f, 18.f, navy, TextAlignment::Center);
      if (card) {
        selected_kart = i;
        ready = false;
        status = selected ? "" : "Selected " + kart_names[i];
      }
    }

    div(context, mk(root.ent(), 170),
        box(scale, 648.f, 366.f, 592.f, 72.f)
            .with_on_draw_bg([scale](RectangleType r) {
              const float cx = r.x + r.width * .5f;
              afterhours::draw_ellipse(
                  static_cast<int>(cx),
                  static_cast<int>(r.y + 52.f * scale), 276.f * scale,
                  28.f * scale, afterhours::Color{22, 95, 142, 255});
              afterhours::draw_ellipse(
                  static_cast<int>(cx),
                  static_cast<int>(r.y + 42.f * scale), 276.f * scale,
                  28.f * scale, afterhours::Color{107, 172, 205, 255});
              afterhours::draw_ellipse(
                  static_cast<int>(cx),
                  static_cast<int>(r.y + 28.f * scale), 276.f * scale,
                  28.f * scale, afterhours::Color{234, 250, 255, 255});
            })
            .with_ignore_pointer_events()
            .with_debug_name("kart_platform"));

    const float big_source_x =
        static_cast<float>(selected_character % 4) * 1184.f;
    const float big_source_y =
        static_cast<float>(selected_character / 4) * 582.f;
    sprite(context, mk(root.ent(), 171), big_karts_texture,
           {big_source_x, big_source_y, 1184.f, 582.f},
           box(scale, 648.f, 116.f, 592.f, 291.f)
               .with_ignore_pointer_events()
               .with_debug_name("kart_preview"));
    text(172, characters[selected_character].name, 648.f, 423.f, 592.f, 58.f,
         49.f, white, TextAlignment::Center, "FredokaMockBold");

    div(context, mk(root.ent(), 180),
        box(scale, 710.f, 489.f, 468.f, 148.f)
            .with_custom_background(afterhours::Color{0, 57, 101, 166})
            .with_on_draw_fg([scale](RectangleType r) {
              afterhours::draw_rectangle(
                  {r.x, r.y, r.width, 2.f * scale},
                  afterhours::Color{116, 195, 238, 255});
              afterhours::draw_rectangle(
                  {r.x, r.y + r.height - 2.f * scale, r.width, 2.f * scale},
                  afterhours::Color{116, 195, 238, 255});
            })
            .with_debug_name("kart_stats"));

    const auto &selected = characters[selected_character];
    const std::array<std::string, 4> stat_names{"Speed", "Acceleration",
                                                 "Handling", "Weight"};
    const std::array<float, 4> stat_values{
        selected.speed, selected.acceleration, selected.handling,
        selected.weight};
    for (size_t i = 0; i < stat_names.size(); ++i) {
      const float y = 501.f + static_cast<float>(i) * 31.f;
      text(190 + static_cast<int>(i), stat_names[i], 734.f, y, 92.f, 27.f,
           20.f, white);
      div(context, mk(root.ent(), 200 + static_cast<int>(i)),
          box(scale, 828.f, y + 9.f, 326.f, 12.f)
              .with_on_draw_bg([value = stat_values[i],
                                scale](RectangleType r) {
                draw_stat_bar(r, value, scale);
              })
              .with_debug_name("kart_stat_" + std::to_string(i)));
    }

    auto back =
        button(context, mk(root.ent(), 230),
               box(scale, 37.f, 666.f, 99.f, 47.f)
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
    text(231, "Back", 69.f, 666.f, 58.f, 47.f, 25.f, white,
         TextAlignment::Center);
    if (back) {
      ready = false;
      status = "Back selected";
    }

    text(232, "Arrows to choose", 158.f, 671.f, 110.f, 40.f, 18.f,
         afterhours::Color{57, 101, 123, 255});
    div(context, mk(root.ent(), 233),
        box(scale, 271.f, 678.f, 18.f, 18.f)
            .with_label("A")
            .with_font("ArchivoMockBold", pixels(14.f * scale))
            .with_custom_text_color(afterhours::Color{57, 101, 123, 255})
            .with_alignment(TextAlignment::Center)
            .with_text_inset(0.f, 0.f)
            .with_on_draw_bg([scale](RectangleType r) {
              afterhours::draw_circle_lines(
                  static_cast<int>(r.x + r.width * .5f),
                  static_cast<int>(r.y + r.height * .5f), 8.f * scale,
                  afterhours::Color{57, 101, 123, 255});
            }));
    text(234, "Confirm", 293.f, 671.f, 52.f, 40.f, 18.f,
         afterhours::Color{57, 101, 123, 255});
    div(context, mk(root.ent(), 235),
        box(scale, 350.f, 678.f, 18.f, 18.f)
            .with_label("B")
            .with_font("ArchivoMockBold", pixels(14.f * scale))
            .with_custom_text_color(afterhours::Color{57, 101, 123, 255})
            .with_alignment(TextAlignment::Center)
            .with_text_inset(0.f, 0.f)
            .with_on_draw_bg([scale](RectangleType r) {
              afterhours::draw_circle_lines(
                  static_cast<int>(r.x + r.width * .5f),
                  static_cast<int>(r.y + r.height * .5f), 8.f * scale,
                  afterhours::Color{57, 101, 123, 255});
            }));
    text(236, "Back", 372.f, 671.f, 45.f, 40.f, 18.f,
         afterhours::Color{57, 101, 123, 255});

    auto ready_button =
        button(context, mk(root.ent(), 240),
               box(scale, 1080.f, 662.f, 163.f, 56.f)
                   .with_on_draw_bg([scale](RectangleType r) {
                     draw_skew_plate(r, 7.f * scale,
                                     afterhours::Color{255, 224, 59, 255});
                     afterhours::draw_rectangle(
                         {r.x, r.y + r.height - 3.f * scale, r.width,
                          3.f * scale},
                         afterhours::Color{205, 158, 9, 255});
                     const afterhours::Color arrow{30, 63, 84, 255};
                     const float x = r.x + 145.f * scale;
                     const float cy = r.y + r.height * .5f;
                     afterhours::draw_line_ex(
                         {x - 6.f * scale, cy - 7.f * scale}, {x, cy},
                         2.5f * scale, arrow);
                     afterhours::draw_line_ex(
                         {x, cy}, {x - 6.f * scale, cy + 7.f * scale},
                         2.5f * scale, arrow);
                   })
                   .with_debug_name("kart_ready"));
    text(241, ready ? "RACING!" : "READY!", 1096.f, 662.f, 120.f, 56.f,
         29.f, afterhours::Color{30, 63, 84, 255}, TextAlignment::Center);
    if (ready_button) {
      ready = true;
      status = "Ready to race: " + characters[selected_character].name +
               " + " + kart_names[selected_kart];
    }

    if (!status.empty())
      text(250, status, 435.f, 672.f, 610.f, 36.f, 17.f,
           afterhours::Color{57, 101, 123, 255}, TextAlignment::Center);
  }
};

REGISTER_EXAMPLE_SCREEN(kart_select, "Game Mockups",
                        "Racing game character and kart selection",
                        KartSelectScreen)
