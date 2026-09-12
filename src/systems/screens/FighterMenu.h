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
#include <vector>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct FighterMenuScreen : ScreenSystem<UIContext<InputAction>> {
  size_t selected_tab = 3;
  size_t active_option = 0;
  int currency = 25000;
  bool texture_loaded = false;
  raylib::Texture2D lobby_texture{};
  std::array<raylib::Texture2D, 7> icons{};

  const afterhours::Color black{3, 4, 4, 255};
  const afterhours::Color deep_black{0, 0, 0, 235};
  const afterhours::Color green{40, 238, 36, 255};
  const afterhours::Color lime{165, 255, 42, 255};
  const afterhours::Color yellow{255, 225, 51, 255};
  const afterhours::Color white{255, 255, 248, 255};
  const afterhours::Color muted{166, 166, 160, 255};
  const afterhours::Color ink{15, 15, 15, 255};
  const afterhours::Color icon_panel{37, 48, 54, 245};
  const afterhours::Color icon_light{232, 251, 255, 255};
  const afterhours::Color holo{120, 255, 220, 155};
  const afterhours::Color holo_edge{129, 215, 71, 255};

  std::array<std::string, 4> tabs = {"Offline", "Online", "Customize",
                                     "Options"};

  struct MenuOption {
    const char *icon;
    const char *label;
    const char *short_label;
    const char *description;
  };

  std::array<MenuOption, 6> menu_options = {{
      {"S", "System Options", "SYSTEM",
       "Change various settings in the game, such as vibration, save, and "
       "autosave functions."},
      {"G", "Game Options", "GAME",
       "Adjust gameplay difficulty, match rounds, and training settings."},
      {"D", "Display Options", "DISPLAY",
       "Modify screen resolution, window mode, and visual effects."},
      {"M", "Sound & Language", "SOUND",
       "Set music and sound levels, voices, and language preferences."},
      {"N", "Network Options", "NETWORK",
       "Configure online play and connection settings."},
      {"B", "Button Settings", "BUTTONS",
       "Remap your controller layout and keyboard bindings."},
  }};

  void load_texture_if_needed() {
    if (texture_loaded)
      return;
    texture_loaded = true;
    lobby_texture = raylib::LoadTexture(
        afterhours::files::get_resource_path("images", "fighter_menu/lobby.png")
            .string()
            .c_str());
    const std::array<const char *, 7> names{
        "save", "gamepad", "monitor", "music", "globe", "buttons", "gear"};
    for (size_t i = 0; i < names.size(); ++i) {
      icons[i] = raylib::LoadTexture(
          afterhours::files::get_resource_path(
              "images", std::string("fighter_menu/") + names[i] + ".png")
              .string()
              .c_str());
      raylib::SetTextureFilter(icons[i], raylib::TEXTURE_FILTER_BILINEAR);
    }
  }

  static void paint(raylib::Texture2D texture, RectangleType r) {
    if (texture.id == 0)
      return;
    raylib::DrawTexturePro(texture,
                           {0, 0, static_cast<float>(texture.width),
                            static_cast<float>(texture.height)},
                           r, {0, 0}, 0, raylib::WHITE);
  }

  static void draw_triangle_ccw(Vector2Type a, Vector2Type b, Vector2Type c,
                                afterhours::Color color) {
    if ((b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x) > 0)
      std::swap(b, c);
    afterhours::draw_triangle(a, b, c, color);
  }

  static void draw_slanted_bar(RectangleType r, afterhours::Color color,
                               afterhours::Color bottom, float left_cut,
                               float right_cut) {
    afterhours::draw_rectangle(
        {r.x + left_cut, r.y, r.width - left_cut - right_cut, r.height}, color);
    draw_triangle_ccw({r.x, r.y + r.height * 0.65f}, {r.x + left_cut, r.y},
                      {r.x + left_cut, r.y + r.height}, color);
    draw_triangle_ccw({r.x + r.width - right_cut, r.y}, {r.x + r.width, r.y},
                      {r.x + r.width - right_cut, r.y + r.height}, color);
    afterhours::draw_rectangle({r.x + left_cut, r.y + r.height - 6.f,
                                r.width - left_cut - right_cut, 6.f},
                               bottom);
  }

  static void draw_tab_shape(RectangleType r, afterhours::Color fill,
                             afterhours::Color outline) {
    afterhours::draw_rectangle({r.x + 8.f, r.y, r.width - 8.f, r.height}, fill);
    draw_triangle_ccw({r.x, r.y + r.height * 0.32f}, {r.x + 8.f, r.y},
                      {r.x + 8.f, r.y + r.height}, fill);
    afterhours::draw_rectangle_outline(r, outline, 2.f);
  }

  ComponentConfig box(float scale, float x, float y, float w, float h) const {
    return ComponentConfig{}
        .with_size({pixels(w * scale), pixels(h * scale)})
        .with_absolute_position(x * scale, y * scale)
        .with_background(Theme::Usage::None)
        .with_corner_radius(0);
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    load_texture_if_needed();

    const float screen_w =
        context.screen_width > 0.f
            ? context.screen_width
            : static_cast<float>(Settings::get().get_screen_width());
    const float screen_h =
        context.screen_height > 0.f
            ? context.screen_height
            : static_cast<float>(Settings::get().get_screen_height());
    const float scale = std::min(screen_w / 1280.f, screen_h / 720.f);
    auto pxf = [scale](float v) { return pixels(v * scale); };
    auto X = [scale](float v) { return v * scale; };
    auto Y = [scale](float v) { return v * scale; };

    if (context.pressed(InputAction::WidgetDown))
      active_option = (active_option + 1) % menu_options.size();
    if (context.pressed(InputAction::WidgetUp))
      active_option =
          (active_option + menu_options.size() - 1) % menu_options.size();
    if (context.pressed(InputAction::WidgetRight))
      selected_tab = (selected_tab + 1) % tabs.size();
    if (context.pressed(InputAction::WidgetLeft))
      selected_tab = (selected_tab + tabs.size() - 1) % tabs.size();

    Theme theme;
    theme.font = white;
    theme.darkfont = ink;
    theme.font_muted = muted;
    theme.background = black;
    theme.surface = deep_black;
    theme.primary = green;
    theme.secondary = holo;
    theme.accent = yellow;
    theme.roundness = 0.0f;
    theme.segments = 4;
    context.set_theme(theme);
    UIStylingDefaults::get().set_default_font("Garamond", h720(25.f));

    auto root =
        div(context, mk(entity, 0),
            box(scale, 0, 0, 1280, 720)
                .with_on_draw_bg([texture = lobby_texture](RectangleType r) {
                  afterhours::draw_rectangle(
                      r, afterhours::Color{155, 150, 119, 255});
                  paint(texture, r);
                })
                .with_debug_name("fighter_root"));

    div(context, mk(root.ent(), 1),
        box(scale, 0, 0, 620, 160)
            .with_on_draw_bg([c = black](RectangleType r) {
              afterhours::draw_rectangle({r.x, r.y, r.width * 0.48f, r.height},
                                         c);
              draw_triangle_ccw({r.x + r.width * 0.48f, r.y},
                                {r.x + r.width, r.y}, {r.x, r.y + r.height}, c);
            })
            .with_ignore_pointer_events()
            .with_debug_name("title_wedge"));

    div(context, mk(root.ent(), 2),
        box(scale, 26, 7, 390, 120)
            .with_label("MainMenu")
            .with_font("ArchivoMockBold", h720(110.f))
            .with_custom_text_color(white)
            .with_text_shadow(afterhours::Color{125, 230, 45, 255}, 5.f * scale,
                              8.f * scale)
            .with_letter_spacing(-4.f * scale)
            .with_debug_name("fighter_title"));

    div(context, mk(root.ent(), 10),
        box(scale, 942, 48, 282, 52)
            .with_on_draw_bg([bg = deep_black](RectangleType r) {
              afterhours::draw_rectangle(
                  {r.x + 20.f, r.y, r.width - 40.f, r.height}, bg);
              draw_triangle_ccw({r.x + 20.f, r.y}, {r.x, r.y + r.height},
                                {r.x + 20.f, r.y + r.height}, bg);
              draw_triangle_ccw({r.x + r.width - 20.f, r.y},
                                {r.x + r.width, r.y},
                                {r.x + r.width - 20.f, r.y + r.height}, bg);
              afterhours::draw_line_ex({r.x + 24.f, r.y + 3.f},
                                       {r.x + r.width - 24.f, r.y + 3.f}, 3.f,
                                       afterhours::Color{105, 110, 100, 255});
            })
            .with_debug_name("currency_panel"));
    div(context, mk(root.ent(), 11),
        box(scale, 990, 54, 194, 44)
            .with_label(std::to_string(currency) + "  P$")
            .with_font("ArchivoMockBold", h720(39.f))
            .with_custom_text_color(afterhours::Color{241, 255, 199, 255})
            .with_alignment(TextAlignment::Right)
            .with_debug_name("fighter_currency"));

    div(context, mk(root.ent(), 20),
        box(scale, 73, 130, 1135, 53)
            .with_custom_background(afterhours::Color{0, 0, 0, 0})
            .with_border(afterhours::Color{54, 85, 104, 255}, 0.f)
            .with_on_draw_bg([](RectangleType r) {
              afterhours::draw_line_ex({r.x, r.y + r.height - 5.f},
                                       {r.x + r.width, r.y + r.height - 5.f},
                                       4.f,
                                       afterhours::Color{54, 85, 104, 255});
              afterhours::draw_line_ex({r.x, r.y + r.height - 1.f},
                                       {r.x + r.width, r.y + r.height - 1.f},
                                       2.f,
                                       afterhours::Color{255, 255, 255, 220});
            })
            .with_ignore_pointer_events()
            .with_debug_name("fighter_tabs"));

    auto bumper = [&](int id, const char *text, float x) {
      div(context, mk(root.ent(), id),
          box(scale, x, 136, 37, 37)
              .with_label(text)
              .with_custom_background(white)
              .with_border(black, 3.f)
              .with_font("ArchivoMockBold", h720(33.f))
              .with_custom_text_color(black)
              .with_alignment(TextAlignment::Center));
    };
    bumper(21, "L", 82);
    bumper(22, "R", 1172);

    for (size_t i = 0; i < tabs.size(); ++i) {
      const bool active = i == selected_tab;
      const float x = 130.f + static_cast<float>(i) * 258.f;
      const float y = active ? 122.f : 132.f;
      const float h = active ? 53.f : 43.f;
      auto cfg =
          box(scale, x, y, 256, h)
              .with_label(tabs[i])
              .with_font("Garamond", h720(30.f))
              .with_letter_spacing(3.f * scale)
              .with_custom_text_color(active ? yellow : muted)
              .with_text_shadow(active ? afterhours::Color{217, 70, 21, 255}
                                       : afterhours::Color{0, 0, 0, 0},
                                2.f * scale, 1.f * scale)
              .with_alignment(TextAlignment::Center)
              .with_on_draw_bg([active](RectangleType r) {
                if (active) {
                  draw_tab_shape(r, afterhours::Color{232, 248, 255, 255},
                                 afterhours::Color{156, 157, 149, 255});
                  afterhours::draw_rectangle(
                      {r.x, r.y + r.height - 8.f, r.width, 8.f},
                      afterhours::Color{54, 239, 25, 255});
                } else {
                  draw_tab_shape(r, afterhours::Color{4, 4, 4, 245},
                                 afterhours::Color{156, 157, 149, 255});
                }
              })
              .with_debug_name("fighter_tab_" + std::to_string(i));
      if (button(context, mk(root.ent(), 30 + static_cast<int>(i)), cfg))
        selected_tab = i;
    }

    div(context, mk(root.ent(), 45),
        box(scale, 574, 254, 212, 37)
            .with_label("Offline Mode")
            .with_custom_background(afterhours::Color{238, 233, 203, 245})
            .with_border(afterhours::Color{175, 163, 132, 255}, 2.f)
            .with_corner_radius(7.f * scale)
            .with_font("Garamond", h720(30.f))
            .with_custom_text_color(afterhours::Color{121, 119, 94, 255})
            .with_letter_spacing(2.f * scale)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("fighter_mode_tag"));

    const auto &current = menu_options[active_option];
    for (size_t i = 0; i < menu_options.size(); ++i) {
      const bool active = i == active_option;
      const auto &opt = menu_options[i];
      const float y = 256.f + static_cast<float>(i) * 58.f;
      const float w = active ? 657.f : 592.f;
      if (button(context, mk(root.ent(), 100 + static_cast<int>(i)),
                 box(scale, 0, y, w, 48)
                     .with_label("")
                     .with_custom_text_color(active ? white : muted)
                     .with_alignment(TextAlignment::Left)
                     .with_on_draw_bg([active](RectangleType r) {
                       draw_slanted_bar(
                           r,
                           active ? afterhours::Color{40, 238, 36, 255}
                                  : afterhours::Color{0, 0, 0, 245},
                           active ? afterhours::Color{0, 0, 0, 255}
                                  : afterhours::Color{119, 123, 119, 255},
                           24.f, 66.f);
                     })
                     .with_debug_name("fighter_option_" + std::to_string(i)))) {
        active_option = i;
      }

      div(context, mk(root.ent(), 130 + static_cast<int>(i)),
          box(scale, 149, y + 2.f, 490, 46)
              .with_label(opt.label)
              .with_font("Garamond", h720(27.f))
              .with_letter_spacing(4.f * scale)
              .with_custom_text_color(active ? white : muted)
              .with_alignment(TextAlignment::Left)
              .with_ignore_pointer_events()
              .with_debug_name("fighter_option_label_" + std::to_string(i)));

      div(context, mk(root.ent(), 150 + static_cast<int>(i)),
          box(scale, 55, y, 76, 48)
              .with_label("")
              .with_font("ArchivoMockBold", h720(32.f))
              .with_custom_text_color(icon_light)
              .with_alignment(TextAlignment::Center)
              .with_on_draw_bg([bg = icon_panel](RectangleType r) {
                afterhours::draw_rectangle(
                    {r.x + 8.f, r.y, r.width - 16.f, r.height}, bg);
                draw_triangle_ccw({r.x, r.y + r.height}, {r.x + 8.f, r.y},
                                  {r.x + 8.f, r.y + r.height}, bg);
                draw_triangle_ccw({r.x + r.width - 8.f, r.y},
                                  {r.x + r.width, r.y},
                                  {r.x + r.width - 8.f, r.y + r.height}, bg);
                afterhours::draw_rectangle_outline(
                    r, afterhours::Color{55, 71, 82, 255}, 2.f);
              })
              .with_ignore_pointer_events());
      const auto texture = icons[i];
      sprite(context, mk(root.ent(), 170 + static_cast<int>(i)), texture,
             {0, 0, (float)texture.width, (float)texture.height},
             box(scale, 62, y + 1, 50, 46).with_ignore_pointer_events());
    }

    div(context, mk(root.ent(), 230),
        box(scale, 878, 196, 350, 300)
            .with_on_draw_bg([holo = holo, edge = holo_edge](RectangleType r) {
              afterhours::draw_rectangle(
                  {r.x + 40.f, r.y, r.width - 40.f, r.height - 18.f}, holo);
              draw_triangle_ccw({r.x + 40.f, r.y}, {r.x, r.y + 86.f},
                                {r.x + 40.f, r.y + r.height}, holo);
              draw_triangle_ccw(
                  {r.x + r.width - 40.f, r.y}, {r.x + r.width, r.y},
                  {r.x + r.width - 40.f, r.y + r.height - 18.f}, holo);
              afterhours::draw_rectangle_outline(
                  {r.x + 18.f, r.y + 17.f, r.width - 36.f, r.height - 38.f},
                  afterhours::Color{167, 255, 234, 210}, 2.f);
              afterhours::draw_line_ex({r.x + 40.f, r.y},
                                       {r.x + r.width - 18.f, r.y}, 4.f, edge);
              afterhours::draw_line_ex({r.x, r.y + 86.f}, {r.x + 40.f, r.y},
                                       4.f, edge);
              afterhours::draw_line_ex(
                  {r.x + r.width - 18.f, r.y},
                  {r.x + r.width - 18.f, r.y + r.height - 70.f}, 4.f, edge);
            })
            .with_debug_name("fighter_holo_card"));

    div(context, mk(root.ent(), 231),
        box(scale, 965, 218, 150, 28)
            .with_label(current.short_label)
            .with_font("ArchivoMockBold", h720(18.f))
            .with_letter_spacing(8.f * scale)
            .with_custom_text_color(afterhours::Color{202, 255, 236, 255})
            .with_debug_name("fighter_holo_label"));

    const auto gear = icons[6];
    sprite(context, mk(root.ent(), 232), gear,
           {0, 0, (float)gear.width, (float)gear.height},
           box(scale, 949, 259, 206, 197)
               .with_ignore_pointer_events()
               .with_debug_name("fighter_holo_gear"));

    div(context, mk(root.ent(), 240),
        box(scale, 808, 388, 455, 82)
            .with_label(current.label)
            .with_font("ArchivoMockBold", h720(55.f))
            .with_letter_spacing(-2.f * scale)
            .with_custom_text_color(white)
            .with_text_shadow(lime, 0.f, 7.f * scale)
            .with_debug_name("fighter_selected_name"));

    div(context, mk(root.ent(), 250),
        box(scale, 754, 458, 526, 204)
            .with_on_draw_bg([bg = deep_black](RectangleType r) {
              afterhours::draw_rectangle(
                  {r.x + r.width * .19f, r.y, r.width * .81f, r.height}, bg);
              draw_triangle_ccw({r.x + r.width * .19f, r.y},
                                {r.x, r.y + r.height * .67f},
                                {r.x + r.width * .19f, r.y + r.height}, bg);
            })
            .with_debug_name("fighter_description"));
    div(context, mk(root.ent(), 251),
        box(scale, 840, 458, 440, 30)
            .with_label(current.label)
            .with_custom_background(lime)
            .with_font("Garamond", h720(25.f))
            .with_custom_text_color(black)
            .with_letter_spacing(2.f * scale)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("fighter_description_title"));
    div(context, mk(root.ent(), 252),
        box(scale, 884, 501, 325, 115)
            .with_label(current.description)
            .with_font("Garamond", h720(30.f))
            .with_text_overflow(TextOverflow::Wrap)
            .with_custom_text_color(white)
            .with_letter_spacing(1.4f * scale)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("fighter_description_text"));

    div(context, mk(root.ent(), 300),
        box(scale, 0, 672, 1280, 48)
            .with_on_draw_bg([](RectangleType r) {
              afterhours::draw_rectangle_gradient_v(
                  r, afterhours::Color{0, 0, 0, 0},
                  afterhours::Color{5, 5, 5, 210});
            })
            .with_ignore_pointer_events());

    struct Prompt {
      const char *key;
      const char *label;
      float x;
      float w;
    };
    std::array<Prompt, 4> prompts = {
        {{"+", ": Select", 302.f, 120.f},
         {"A", ": Confirm", 436.f, 146.f},
         {"B", ": Return", 603.f, 130.f},
         {"L  R", ": Change Entry", 752.f, 230.f}}};
    for (size_t i = 0; i < prompts.size(); ++i) {
      const auto &p = prompts[i];
      div(context, mk(root.ent(), 310 + static_cast<int>(i) * 2),
          box(scale, p.x, 679, i == 0 ? 40.f : (i == 3 ? 65.f : 28.f), 32)
              .with_label(p.key)
              .with_custom_background(i == 0 ? afterhours::Color{0, 0, 0, 0}
                                             : white)
              .with_border(black, i == 0 ? 0.f : 2.f)
              .with_font("ArchivoMockBold", h720(i == 0 ? 32.f : 27.f))
              .with_custom_text_color(i == 0 ? white : black)
              .with_alignment(TextAlignment::Center)
              .with_debug_name("fighter_prompt_key_" + std::to_string(i)));
      div(context, mk(root.ent(), 311 + static_cast<int>(i) * 2),
          box(scale, p.x + (i == 3 ? 73.f : 42.f), 682, p.w, 31)
              .with_label(p.label)
              .with_font("Garamond", h720(30.f))
              .with_custom_text_color(white)
              .with_text_shadow(black, 2.f * scale, 2.f * scale)
              .with_debug_name("fighter_prompt_label_" + std::to_string(i)));
    }
  }
};

REGISTER_EXAMPLE_SCREEN(fighter_menu, "Game Mockups",
                        "Anime fighting game menu (Cross Tag style)",
                        FighterMenuScreen)
