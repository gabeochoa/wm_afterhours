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
  const afterhours::Color muted{216, 219, 211, 255};
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

  const std::array<std::array<MenuOption, 6>, 3> tab_options{{
      {{{"G", "Arcade", "ARCADE", "Fight a series of opponents and climb to the final match."},
        {"G", "Versus", "VERSUS", "Challenge a friend in a local head-to-head match."},
        {"G", "Training", "TRAINING", "Practice combos, movement, and matchups at your own pace."},
        {"G", "Survival", "SURVIVAL", "Win consecutive matches with one shared health bar."},
        {"G", "Missions", "MISSIONS", "Learn techniques through focused combat challenges."},
        {"G", "Replay Theater", "REPLAYS", "Watch saved matches and study your best rounds."}}},
      {{{"N", "Ranked Match", "RANKED", "Find an opponent at your level and compete for rank points."},
        {"N", "Player Match", "CASUAL", "Play an unranked match with another online fighter."},
        {"N", "Create Lobby", "HOST", "Create a room and invite friends to play together."},
        {"N", "Find Lobby", "LOBBIES", "Browse public rooms and join an open match."},
        {"N", "Rankings", "RANKINGS", "Compare your standing with fighters around the world."},
        {"N", "Friends", "FRIENDS", "See your friends and manage match invitations."}}},
      {{{"D", "Character Color", "COLORS", "Choose a color palette for your favorite fighter."},
        {"D", "Player Card", "CARD", "Personalize the player card shown before every match."},
        {"D", "Avatar", "AVATAR", "Choose the avatar that represents you in online lobbies."},
        {"D", "Titles", "TITLES", "Equip a title earned through your achievements."},
        {"D", "Gallery", "GALLERY", "Browse unlocked artwork and character illustrations."},
        {"D", "Music Library", "MUSIC", "Choose the soundtrack for your menu and matches."}}},
  }};

  void choose_tab(size_t index) {
    selected_tab = index;
    active_option = 0;
  }

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
    afterhours::draw_rectangle({r.x + left_cut, r.y + r.height - r.height / 16,
                                r.width - left_cut - right_cut, r.height / 16},
                               bottom);
  }

  static void draw_tab_shape(RectangleType r, afterhours::Color fill,
                             afterhours::Color outline) {
    const float scale = r.height / 46.f;
    afterhours::draw_rectangle({r.x + 8 * scale, r.y, r.width - 8 * scale, r.height}, fill);
    draw_triangle_ccw({r.x, r.y + r.height * .32f}, {r.x + 8 * scale, r.y},
                      {r.x + 8 * scale, r.y + r.height}, fill);
    afterhours::draw_rectangle_outline(r, outline, 2 * scale);
  }

  const std::array<afterhours::texture_manager::Rectangle, 7> icon_sources{{
      {29, 19, 142, 162}, {13, 50, 174, 118}, {19, 25, 162, 146},
      {22, 22, 137, 137}, {19, 19, 162, 162}, {13, 50, 174, 118},
      {10, 0, 175, 177}}};

  static void draw_binding_icon(RectangleType r) {
    const float size = r.width * .39f;
    for (int i = 0; i < 4; ++i) {
      const float x = r.x + static_cast<float>(i % 2) * r.width * .61f;
      const float y = r.y + static_cast<float>(i / 2) * r.height * .61f;
      raylib::DrawRectangleRounded({x, y, size, size}, .2f, 4,
                                   i == 0 ? raylib::Color{165, 255, 42, 255}
                                          : raylib::Color{232, 251, 255, 255});
    }
    raylib::DrawLineEx({r.x + r.width * .22f, r.y + r.height * .5f},
                       {r.x + r.width * .78f, r.y + r.height * .5f},
                       r.width * .05f, {17, 52, 43, 255});
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

    if (context.pressed(InputAction::WidgetDown))
      active_option = (active_option + 1) % menu_options.size();
    if (context.pressed(InputAction::WidgetUp))
      active_option =
          (active_option + menu_options.size() - 1) % menu_options.size();
    if (context.pressed(InputAction::WidgetRight))
      choose_tab((selected_tab + 1) % tabs.size());
    if (context.pressed(InputAction::WidgetLeft))
      choose_tab((selected_tab + tabs.size() - 1) % tabs.size());

    if (context.pressed(InputAction::MenuBack)) {
      active_option = 0;
      selected_tab = 3;
    }

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
    UIStylingDefaults::get().set_default_font("Garamond", pixels(25.f * scale));
    div(context, mk(entity, 999), ComponentConfig{}
        .with_size({pixels(screen_w), pixels(screen_h)})
        .with_custom_background(black).with_corner_radius(0)
        .with_debug_name("fighter_canvas"));

    auto root =
        div(context, mk(entity, 0),
            box(scale, 0, 0, 1280, 720)
                .with_absolute_position((screen_w - 1280 * scale) / 2,
                                        (screen_h - 720 * scale) / 2)
                .with_on_draw_bg([texture = lobby_texture](RectangleType r) {
                  afterhours::draw_rectangle(
                      r, afterhours::Color{155, 150, 119, 255});
                  paint(texture, r);
                  afterhours::draw_rectangle(
                      {r.x, r.y, r.width, r.height * .29f},
                      {155, 150, 119, 85});
                })
                .with_debug_name("fighter_root"));

    div(context, mk(root.ent(), 1),
        box(scale, 0, 0, 700, 150)
            .with_on_draw_bg([c = black](RectangleType r) {
              afterhours::draw_rectangle({r.x, r.y, r.width * 0.65f, r.height},
                                         c);
              draw_triangle_ccw({r.x + r.width * 0.65f, r.y},
                                {r.x + r.width, r.y}, {r.x, r.y + r.height}, c);
            })
            .with_ignore_pointer_events()
            .with_debug_name("title_wedge"));

    div(context, mk(root.ent(), 2),
        box(scale, 26, 13, 480, 105)
            .with_label("Main Menu")
            .with_font("Garamond", pixels(96.f * scale))
            .with_custom_text_color(white)
            .with_text_shadow(afterhours::Color{125, 230, 45, 255}, 2.f * scale,
                              3.f * scale)
            .with_letter_spacing(-1.f * scale)
            .with_debug_name("fighter_title"));

    div(context, mk(root.ent(), 10),
        box(scale, 942, 48, 282, 52)
            .with_on_draw_bg([bg = black](RectangleType r) {
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
        box(scale, 976, 52, 224, 44)
            .with_label(fmt::format("{},{:03} P$", currency / 1000, currency % 1000))
            .with_font("Garamond", pixels(39.f * scale))
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

    auto bumper = [&](int id, const char *text, float x, int direction) {
      if (button(context, mk(root.ent(), id),
          box(scale, x, 136, 38, 38)
              .with_label(text)
              .with_custom_background(white)
              .with_border(black, 3.f)
              .with_font("Garamond", pixels(33.f * scale))
              .with_custom_text_color(black)
              .with_alignment(TextAlignment::Center)
              .with_debug_name(direction < 0 ? "fighter_previous_tab" : "fighter_next_tab"))) {
        choose_tab(direction < 0 ? (selected_tab + tabs.size() - 1) % tabs.size()
                                 : (selected_tab + 1) % tabs.size());
      }
    };
    bumper(21, "L", 78, -1);
    bumper(22, "R", 1172, 1);

    for (size_t i = 0; i < tabs.size(); ++i) {
      const bool active = i == selected_tab;
      const float x = 128.f + static_cast<float>(i) * 260.f;
      const float y = 130.f;
      const float h = 46.f;
      auto cfg =
          box(scale, x, y, 252, h)
              .with_label(tabs[i])
              .with_font("Garamond", pixels(30.f * scale))
              .with_letter_spacing(1.f * scale)
              .with_custom_text_color(active ? ink : muted)
              .with_alignment(TextAlignment::Center)
              .with_on_draw_bg([active, scale](RectangleType r) {
                if (active) {
                  draw_tab_shape(r, afterhours::Color{232, 248, 255, 255},
                                 afterhours::Color{156, 157, 149, 255});
                  afterhours::draw_rectangle(
                      {r.x, r.y + r.height - 5.f * scale, r.width, 5.f * scale},
                      afterhours::Color{54, 239, 25, 255});
                } else {
                  draw_tab_shape(r, afterhours::Color{4, 4, 4, 245},
                                 afterhours::Color{156, 157, 149, 255});
                }
              })
              .with_debug_name("fighter_tab_" + std::to_string(i));
      if (button(context, mk(root.ent(), 30 + static_cast<int>(i)), cfg))
        choose_tab(i);
    }

    div(context, mk(root.ent(), 45),
        box(scale, 596, 214, 218, 34)
            .with_label(selected_tab == 1 ? "Online Mode" : "Offline Mode")
            .with_custom_background(afterhours::Color{238, 233, 203, 245})
            .with_border(afterhours::Color{175, 163, 132, 255}, 2.f)
            .with_corner_radius(7.f * scale)
            .with_font("Garamond", pixels(25.f * scale))
            .with_custom_text_color(afterhours::Color{56, 58, 46, 255})
            .with_letter_spacing(2.f * scale)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("fighter_mode_tag"));

    const auto &options = selected_tab == 3 ? menu_options : tab_options[selected_tab];
    for (size_t i = 0; i < options.size(); ++i) {
      const bool active = i == active_option;
      const auto &opt = options[i];
      const float y = 256.f + static_cast<float>(i) * 58.f;
      const float w = 558.f;
      if (button(context, mk(root.ent(), 100 + static_cast<int>(i)),
                 box(scale, 0, y, w, 48)
                     .with_label("")
                     .with_custom_text_color(active ? ink : muted)
                     .with_alignment(TextAlignment::Left)
                     .with_on_draw_bg([active, scale](RectangleType r) {
                       draw_slanted_bar(
                           r,
                           active ? afterhours::Color{40, 238, 36, 255}
                                  : afterhours::Color{0, 0, 0, 245},
                           active ? afterhours::Color{0, 0, 0, 255}
                                  : afterhours::Color{119, 123, 119, 255},
                           20.f * scale, 28.f * scale);
                     })
                     .with_debug_name("fighter_option_" + std::to_string(i)))) {
        active_option = i;
      }

      div(context, mk(root.ent(), 130 + static_cast<int>(i)),
          box(scale, 140, y + 2.f, 388, 44)
              .with_label(opt.label)
              .with_font("Garamond", pixels(27.f * scale))
              .with_letter_spacing(.6f * scale)
              .with_custom_text_color(active ? ink : muted)
              .with_alignment(TextAlignment::Left)
              .with_ignore_pointer_events()
              .with_debug_name("fighter_option_label_" + std::to_string(i)));

      div(context, mk(root.ent(), 150 + static_cast<int>(i)),
          box(scale, 58, y, 64, 48)
              .with_label("")
              .with_font("Garamond", pixels(32.f * scale))
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
      if (i == 5) {
        div(context, mk(root.ent(), 170 + static_cast<int>(i)),
            box(scale, 72, y + 6, 36, 36)
                .with_on_draw_fg(draw_binding_icon)
                .with_ignore_pointer_events().with_debug_name("fighter_binding_icon"));
      } else {
        const auto texture = icons[i];
        const auto source = icon_sources[i];
        const float icon_scale = 38.f / std::max(source.width, source.height);
        const float width = source.width * icon_scale;
        const float height = source.height * icon_scale;
        sprite(context, mk(root.ent(), 170 + static_cast<int>(i)), texture, source,
               box(scale, 90 - width / 2, y + (48 - height) / 2, width, height)
                   .with_ignore_pointer_events());
      }
    }

    const auto &current = options[active_option];
    div(context, mk(root.ent(), 230),
        box(scale, 850, 204, 376, 286)
            .with_on_draw_bg([scale, fill = holo](RectangleType r) {
              const float cut = 24 * scale;
              afterhours::draw_rectangle({r.x + cut, r.y, r.width - 2 * cut, r.height}, fill);
              draw_triangle_ccw({r.x + cut, r.y}, {r.x, r.y + r.height},
                                {r.x + cut, r.y + r.height}, fill);
              draw_triangle_ccw({r.x + r.width - cut, r.y}, {r.x + r.width, r.y},
                                {r.x + r.width - cut, r.y + r.height}, fill);
              const afterhours::Color frame{209, 255, 230, 255};
              afterhours::draw_line_ex({r.x + cut, r.y}, {r.x + r.width, r.y}, 2 * scale, frame);
              afterhours::draw_line_ex({r.x + r.width, r.y},
                                       {r.x + r.width - cut, r.y + r.height}, 2 * scale, frame);
              afterhours::draw_line_ex({r.x + r.width - cut, r.y + r.height},
                                       {r.x, r.y + r.height}, 2 * scale, frame);
              afterhours::draw_line_ex({r.x, r.y + r.height}, {r.x + cut, r.y}, 2 * scale, frame);
              afterhours::draw_line_ex({r.x + cut, r.y - 5 * scale},
                                       {r.x + r.width, r.y - 5 * scale}, 2 * scale,
                                       {129, 215, 71, 200});
            }).with_debug_name("fighter_holo_card"));
    div(context, mk(root.ent(), 231), box(scale, 916, 225, 254, 37)
        .with_label(current.short_label).with_font("Garamond", pixels(29 * scale))
        .with_letter_spacing(2 * scale).with_custom_text_color({17, 52, 43, 255})
        .with_alignment(TextAlignment::Center).with_debug_name("fighter_holo_label"));
    if (active_option == 5) {
      div(context, mk(root.ent(), 232), box(scale, 953, 280, 140, 140)
          .with_on_draw_fg(draw_binding_icon).with_ignore_pointer_events()
          .with_debug_name("fighter_holo_gear"));
    } else {
      const size_t icon = active_option == 0 ? 6 : active_option;
      const auto source = icon_sources[icon];
      const float ratio = 148.f / std::max(source.width, source.height);
      const float width = source.width * ratio;
      const float height = source.height * ratio;
      sprite(context, mk(root.ent(), 232), icons[icon], source,
             box(scale, 1023 - width / 2, 345 - height / 2, width, height)
                 .with_ignore_pointer_events().with_debug_name("fighter_holo_gear"));
    }
    div(context, mk(root.ent(), 240), box(scale, 826, 418, 432, 70)
        .with_label(current.label).with_font("Garamond", pixels(54 * scale))
        .with_letter_spacing(-1 * scale).with_custom_text_color(white)
        .with_text_stroke(black, 3 * scale).with_text_shadow(lime, 0, 2 * scale)
        .with_debug_name("fighter_selected_name"));
    div(context, mk(root.ent(), 250), box(scale, 804, 492, 476, 170)
        .with_on_draw_bg([scale, bg = deep_black](RectangleType r) {
          afterhours::draw_rectangle({r.x + 24 * scale, r.y, r.width - 24 * scale, r.height}, bg);
          draw_triangle_ccw({r.x + 24 * scale, r.y}, {r.x, r.y + r.height / 2},
                            {r.x + 24 * scale, r.y + r.height}, bg);
        }).with_debug_name("fighter_description"));
    div(context, mk(root.ent(), 251), box(scale, 834, 501, 412, 30)
        .with_label(std::array<const char *, 4>{"OFFLINE / ", "ONLINE / ", "CUSTOMIZE / ", "OPTIONS / "}[selected_tab] + std::string(current.short_label))
        .with_custom_background(lime).with_font("Garamond", pixels(22 * scale))
        .with_custom_text_color(black).with_alignment(TextAlignment::Left)
        .with_debug_name("fighter_description_title"));
    div(context, mk(root.ent(), 252), box(scale, 841, 542, 383, 106)
        .with_label(current.description).with_font("Garamond", pixels(23 * scale))
        .with_text_overflow(TextOverflow::Wrap).with_custom_text_color(white)
        .with_letter_spacing(0).with_alignment(TextAlignment::Left)
        .with_debug_name("fighter_description_text"));

    div(context, mk(root.ent(), 300),
        box(scale, 0, 672, 1280, 48)
            .with_on_draw_bg([](RectangleType r) {
              afterhours::draw_rectangle_gradient_v(
                  r, afterhours::Color{0, 0, 0, 0},
                  afterhours::Color{5, 5, 5, 210});
            })
            .with_ignore_pointer_events());

    const auto keycap = [&](int id, const char *label, float x, float width) {
      return div(context, mk(root.ent(), id), box(scale, x, 679, width, 32)
          .with_label(label).with_custom_background(white).with_border(black, 2 * scale)
          .with_font("Garamond", pixels(23 * scale)).with_custom_text_color(black)
          .with_alignment(TextAlignment::Center).with_ignore_pointer_events()
          .with_debug_name("fighter_prompt_key_" + std::to_string(id)));
    };
    keycap(310, "", 250, 32);
    div(context, mk(root.ent(), 309), box(scale, 254, 683, 24, 24)
        .with_on_draw_fg([](RectangleType r) {
          const auto p = [=](float x, float y) { return Vector2Type{r.x + r.width * x, r.y + r.height * y}; };
          const afterhours::Color c{15, 15, 15, 255};
          draw_triangle_ccw(p(.5f, 0), p(.25f, .3f), p(.75f, .3f), c);
          draw_triangle_ccw(p(.5f, 1), p(.25f, .7f), p(.75f, .7f), c);
          draw_triangle_ccw(p(0, .5f), p(.3f, .25f), p(.3f, .75f), c);
          draw_triangle_ccw(p(1, .5f), p(.7f, .25f), p(.7f, .75f), c);
        }).with_ignore_pointer_events());
    keycap(312, "Enter", 431, 61);
    keycap(314, "Esc", 637, 45);
    keycap(316, "L", 839, 32);
    keycap(318, "R", 879, 32);
    const std::array<const char *, 4> labels{"Select", "Choose", "Reset", "Change tab"};
    const std::array<float, 4> positions{292, 502, 692, 921};
    for (size_t i = 0; i < labels.size(); ++i) {
      div(context, mk(root.ent(), 330 + static_cast<int>(i)),
          box(scale, positions[i], 681, i == 3 ? 185 : 129, 31)
              .with_label(labels[i]).with_font("Garamond", pixels(27 * scale))
              .with_custom_text_color(white).with_text_shadow(black, scale, scale)
              .with_debug_name("fighter_prompt_label_" + std::to_string(i)));
    }
  }
};

REGISTER_EXAMPLE_SCREEN(fighter_menu, "Game Mockups",
                        "Anime fighting game menu (Cross Tag style)",
                        FighterMenuScreen)
