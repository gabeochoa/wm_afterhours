#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/modal.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <string>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct MiniMotorwaysSettingsScreen : ScreenSystem<UIContext<InputAction>> {
  size_t active_tab = 0;
  bool settings_open = true;
  bool tutorial_open = false;
  bool night_mode = false;
  bool vibration = false;
  bool hold_to_draw = true;
  bool colorblind_mode = false;
  bool fullscreen = true;
  bool account_connected = false;
  int sensitivity_level = 1;
  int resolution_index = 0;
  int language_index = 0;
  float music_volume = .70f;
  float sound_volume = .90f;
  std::string feedback;

  const std::array<std::string, 7> categories{
      "Game", "Video", "Audio", "Language", "Controls", "Cross-Save",
      "Credits"};
  const std::array<std::string, 3> sensitivity{"Low", "Default", "High"};
  const std::array<std::string, 2> resolutions{"1920 × 1080",
                                                "2560 × 1440"};
  const std::array<std::string, 4> languages{"English", "Deutsch", "Français",
                                             "日本語"};

  const afterhours::Color day_edge{211, 220, 228, 255};
  const afterhours::Color day_center{240, 245, 237, 255};
  const afterhours::Color day_road{250, 252, 251, 255};
  const afterhours::Color day_grid{203, 219, 231, 24};
  const afterhours::Color day_ink{46, 53, 57, 255};
  const afterhours::Color night_edge{29, 42, 47, 255};
  const afterhours::Color night_center{51, 67, 70, 255};
  const afterhours::Color night_road{74, 91, 94, 255};
  const afterhours::Color night_grid{109, 135, 140, 20};
  const afterhours::Color night_ink{237, 243, 241, 255};
  const afterhours::Color yellow{255, 183, 77, 255};
  const afterhours::Color teal{142, 187, 188, 255};
  const afterhours::Color pale_teal{152, 194, 199, 128};

  ComponentConfig box(float scale, float x, float y, float width,
                      float height) const {
    return ComponentConfig{}
        .with_size({pixels(width * scale), pixels(height * scale)})
        .with_absolute_position(pixels(x * scale), pixels(y * scale))
        .with_background(Theme::Usage::None).with_corner_radius(0);
  }

  static afterhours::Color mix(afterhours::Color a, afterhours::Color b,
                               float amount) {
    auto channel = [amount](unsigned char x, unsigned char y) {
      return static_cast<unsigned char>(
          std::round(static_cast<float>(x) +
                     (static_cast<float>(y) - static_cast<float>(x)) *
                         amount));
    };
    return {channel(a.r, b.r), channel(a.g, b.g), channel(a.b, b.b),
            channel(a.a, b.a)};
  }

  void draw_backdrop(RectangleType r, float scale) const {
    const auto edge = night_mode ? night_edge : day_edge;
    const auto center = night_mode ? night_center : day_center;
    const auto road = mix(center, night_mode ? night_road : day_road, .42f);
    const auto grid = night_mode ? night_grid : day_grid;
    afterhours::draw_rectangle(r, edge);

    for (int layer = 72; layer >= 1; --layer) {
      const float radius = static_cast<float>(layer) / 72.f;
      const float toward_center = 1.f - radius;
      afterhours::draw_ellipse(
          static_cast<int>(r.x + 640.f * scale),
          static_cast<int>(r.y + 360.f * scale), 820.f * scale * radius,
          500.f * scale * radius, mix(edge, center, toward_center));
    }

    for (float x = 98.f; x < 1280.f; x += 157.f)
      afterhours::draw_rectangle(
          {r.x + x * scale, r.y, 2.f * scale, 720.f * scale}, grid);
    for (float y = 94.f; y < 720.f; y += 157.f)
      afterhours::draw_rectangle(
          {r.x, r.y + y * scale, 1280.f * scale, 2.f * scale}, grid);

    afterhours::draw_rectangle(
        {r.x + 187.f * scale, r.y - 20.f * scale, 36.f * scale,
         453.f * scale},
        road);
    afterhours::draw_ring_segment(
        r.x + 176.f * scale, r.y + 433.f * scale, 11.f * scale,
        47.f * scale, 0.f, 90.f, 32, road);
    afterhours::draw_rectangle(
        {r.x - 40.f * scale, r.y + 445.f * scale, 216.f * scale,
         36.f * scale},
        road);
  }

  static void draw_back_arrow(RectangleType r, afterhours::Color color) {
    const float unit = r.height / 70.f;
    const float center_y = r.y + 39.f * unit;
    const float tip_x = r.x + 15.f * unit;
    const float right_x = r.x + 63.f * unit;
    const float stroke = 10.f * unit;
    const afterhours::Color shadow{0, 0, 0, 45};
    afterhours::draw_line_ex({tip_x + 2.f * unit, center_y + 2.f * unit},
                             {right_x + 2.f * unit, center_y + 2.f * unit},
                             stroke, shadow);
    afterhours::draw_line_ex(
        {tip_x + 2.f * unit, center_y + 2.f * unit},
        {r.x + 34.f * unit, r.y + 23.f * unit}, stroke, shadow);
    afterhours::draw_line_ex(
        {tip_x + 2.f * unit, center_y + 2.f * unit},
        {r.x + 34.f * unit, r.y + 57.f * unit}, stroke, shadow);
    afterhours::draw_line_ex({tip_x, center_y}, {right_x, center_y}, stroke,
                             color);
    afterhours::draw_line_ex({tip_x, center_y},
                             {r.x + 34.f * unit, r.y + 21.f * unit}, stroke,
                             color);
    afterhours::draw_line_ex({tip_x, center_y},
                             {r.x + 34.f * unit, r.y + 55.f * unit}, stroke,
                             color);
  }

  static void draw_toggle(RectangleType r, bool on, afterhours::Color ink,
                          afterhours::Color canvas) {
    const float unit = r.width / 65.f;
    const int cx = static_cast<int>(r.x + r.width * .5f);
    const int cy = static_cast<int>(r.y + r.height * .5f);
    afterhours::draw_circle(cx, cy + static_cast<int>(2.f * unit),
                            32.f * unit, afterhours::Color{0, 0, 0, 45});
    afterhours::draw_circle(cx, cy, 32.f * unit, ink);
    if (!on) {
      afterhours::draw_circle(cx, cy, 25.f * unit, canvas);
      afterhours::draw_line_ex(
          {r.x + 21.f * unit, r.y + 21.f * unit},
          {r.x + 44.f * unit, r.y + 44.f * unit}, 9.f * unit, ink);
      afterhours::draw_line_ex(
          {r.x + 44.f * unit, r.y + 21.f * unit},
          {r.x + 21.f * unit, r.y + 44.f * unit}, 9.f * unit, ink);
      return;
    }
    afterhours::draw_circle_lines(cx, cy, 26.f * unit,
                                  afterhours::Color{255, 255, 255, 100});
    const afterhours::Color check =
        canvas.r < 100 ? afterhours::Color{237, 243, 241, 255}
                       : afterhours::Color{242, 245, 244, 255};
    afterhours::draw_line_ex(
        {r.x + 15.f * unit, r.y + 34.f * unit},
        {r.x + 27.f * unit, r.y + 46.f * unit}, 8.f * unit, check);
    afterhours::draw_line_ex(
        {r.x + 27.f * unit, r.y + 46.f * unit},
        {r.x + 51.f * unit, r.y + 22.f * unit}, 8.f * unit, check);
  }

  static void draw_cycle_arrow(RectangleType r, bool points_right,
                               afterhours::Color color) {
    const float mid = r.y + r.height * .5f;
    const float tip = r.x + r.width * (points_right ? .67f : .33f);
    const float edge = r.x + r.width * (points_right ? .33f : .67f);
    afterhours::draw_line_ex({edge, r.y + r.height * .25f}, {tip, mid}, r.width * .065f, color);
    afterhours::draw_line_ex({tip, mid}, {edge, r.y + r.height * .75f}, r.width * .065f, color);
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    const float scale =
        std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const auto ink = night_mode ? night_ink : day_ink;
    const auto canvas = night_mode ? night_center : day_center;

    Theme theme;
    theme.font = ink;
    theme.darkfont = afterhours::Color{255, 255, 255, 255};
    theme.font_muted = night_mode ? afterhours::Color{185, 202, 202, 255}
                                  : afterhours::Color{91, 109, 113, 255};
    theme.background = canvas;
    theme.surface = canvas;
    theme.primary = ink;
    theme.secondary = night_mode ? afterhours::Color{91, 119, 122, 255}
                                 : afterhours::Color{157, 181, 184, 255};
    theme.accent = teal;
    theme.roundness = 0.f;
    theme.segments = 24;
    context.set_theme(theme);
    context.scaling_mode = ScalingMode::Proportional;
    UIStylingDefaults::get().set_grid_snapping(false);
    UIStylingDefaults::get().set_default_font("AtkinsonMock", pixels(30.f));

    if (!tutorial_open && context.pressed(InputAction::MenuBack)) settings_open = false;

    div(context, mk(entity), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background(night_mode ? night_edge : day_edge)
        .with_corner_radius(0).with_debug_name("mm_canvas"));
    auto root =
        div(context, mk(entity),
            box(scale, 0.f, 0.f, 1280.f, 720.f)
                .with_absolute_position((context.screen_width - 1280 * scale) / 2,
                                        (context.screen_height - 720 * scale) / 2)
                .with_on_draw_bg([this, scale](RectangleType r) {
                  draw_backdrop(r, scale);
                })
                .with_debug_name("mm_root"));

    if (!settings_open) {
      if (button(
              context, mk(root.ent()),
              box(scale, 510.f, 320.f, 260.f, 70.f)
                  .with_label("Open Settings")
                  .with_font("AtkinsonMock", pixels(33.f * scale))
                  .with_custom_background(teal)
                  .with_custom_text_color(afterhours::Color{255, 255, 255, 255})
                  .with_alignment(TextAlignment::Center)
                  .with_text_stroke(afterhours::Color{255, 255, 255, 255},
                                    .4f * scale)
                  .with_debug_name("mm_reopen"))) {
        settings_open = true;
        feedback = "Settings reopened.";
      }
      return;
    }

    auto back = button(context, mk(root.ent()), box(scale, 28, 24, 78, 78)
        .with_custom_background(afterhours::Color{0, 0, 0, 0})
        .with_on_draw_fg([ink](RectangleType r) { draw_back_arrow(r, ink); })
        .with_debug_name("mm_back"));
    if (back) {
      settings_open = false;
      tutorial_open = false;
    }
    for (size_t i = 0; i < categories.size(); ++i) {
      const bool active = active_tab == i;
      const float tab_w = 92.f + static_cast<float>(categories[i].size()) * 21.f;
      auto tab = button(context, mk(root.ent(), 20 + static_cast<int>(i)),
          box(scale, 388 - tab_w, 138 + static_cast<float>(i) * 57, tab_w, 52)
              .with_custom_background(active ? yellow : afterhours::Color{168, 200, 203, 255})
              .with_click_activation(ClickActivationMode::Release)
              .with_debug_name("mm_tab_" + std::to_string(i)));
      div(context, mk(root.ent(), 60 + static_cast<int>(i)),
          box(scale, 388 - tab_w, 138 + static_cast<float>(i) * 57, tab_w - 16, 52)
              .with_label(categories[i]).with_font("AtkinsonMock", pixels(38 * scale))
              .with_custom_text_color(afterhours::Color{255, 255, 255, 255}).with_alignment(TextAlignment::Right)
              .with_text_inset(0, 0).with_letter_spacing(0).with_ignore_pointer_events());
      if (tab) {
        active_tab = i;
        tutorial_open = false;
        feedback.clear();
      }
    }

    auto text = [&](int id, const std::string &label, float x, float y,
                    float width, float height, float size,
                    TextAlignment alignment = TextAlignment::Left,
                    const std::string &font = "AtkinsonMock") {
      return div(context, mk(root.ent(), id),
                 box(scale, x, y, width, height)
                     .with_label(label)
                     .with_font(font, pixels(size * scale))
                     .with_custom_text_color(ink)
                     .with_alignment(alignment)
                     .with_text_inset(0.f, 0.f)
                     .with_letter_spacing(0).with_ignore_pointer_events());
    };

    auto toggle = [&](int id, const char *label, bool &value, float row_y,
                      const std::string &debug_name) {
      text(id, label, 500, row_y, 360, 46, 32, TextAlignment::Right);
      if (button(context, mk(root.ent(), id + 1), box(scale, 884, row_y, 58, 58)
          .with_click_activation(ClickActivationMode::Release)
          .with_roundness(1.f)
          .with_on_draw_fg([value, ink, canvas](RectangleType r) {
            draw_toggle(r, value, ink, canvas);
          }).with_debug_name(debug_name))) {
        value = !value;
        feedback = std::string(label) + (value ? " on" : " off");
      }
    };

    auto cycle = [&](int id, const std::string &label, std::string value,
                     float row_y, const std::string &debug_prefix,
                     auto previous, auto next,
                     const std::string &value_font = "AtkinsonMock") {
      text(id, label, 500, row_y + 6, 360, 42, 26, TextAlignment::Right);
      if (button(context, mk(root.ent(), id + 1),
                 box(scale, 884, row_y, 52, 52)
                     .with_click_activation(ClickActivationMode::Release)
                     .with_on_draw_fg([ink](RectangleType r) {
                       // solid left triangle
                       afterhours::draw_triangle({r.x + 14, r.y + 26}, {r.x + 38, r.y + 10}, {r.x + 38, r.y + 42}, ink);
                     })
                     .with_debug_name(debug_prefix + "_left")))
        previous();
      text(id + 2, value, 948, row_y + 4, 150, 44, 29,
           TextAlignment::Center, value_font);
      if (button(context, mk(root.ent(), id + 3),
                 box(scale, 1110, row_y, 52, 52)
                     .with_click_activation(ClickActivationMode::Release)
                     .with_on_draw_fg([ink](RectangleType r) {
                       afterhours::draw_triangle({r.x + 38, r.y + 26}, {r.x + 14, r.y + 10}, {r.x + 14, r.y + 42}, ink);
                     })
                     .with_debug_name(debug_prefix + "_right")))
        next();
    };

    if (active_tab == 0) {
      toggle(100, "Night Mode", night_mode, 150.f, "mm_night");
      toggle(110, "Vibration", vibration, 242.f, "mm_vibration");
      toggle(120, "Hold to Draw/Delete", hold_to_draw, 334.f, "mm_hold");
      cycle(
          130, "Controller Cursor Sensitivity",
          sensitivity[static_cast<size_t>(sensitivity_level)], 426.f,
          "mm_sensitivity",
          [&] {
            sensitivity_level =
                (sensitivity_level + static_cast<int>(sensitivity.size()) - 1) %
                static_cast<int>(sensitivity.size());
            feedback = "Sensitivity " +
                       sensitivity[static_cast<size_t>(sensitivity_level)];
          },
          [&] {
            sensitivity_level =
                (sensitivity_level + 1) %
                static_cast<int>(sensitivity.size());
            feedback = "Sensitivity " +
                       sensitivity[static_cast<size_t>(sensitivity_level)];
          });
      div(context, mk(root.ent()),
          box(scale, 500, 500, 662, 2)
              .with_custom_background(night_mode
                                          ? afterhours::Color{105, 128, 132, 255}
                                          : afterhours::Color{183, 195, 199, 255})
              .with_debug_name("mm_separator"));
      toggle(141, "Colorblind Mode", colorblind_mode, 522.f,
             "mm_colorblind");
    } else if (active_tab == 1) {
      toggle(200, "Fullscreen", fullscreen, 151.f, "mm_fullscreen");
      cycle(
          210, "Resolution",
          resolutions[static_cast<size_t>(resolution_index)], 253.f,
          "mm_resolution",
          [&] {
            resolution_index =
                (resolution_index + static_cast<int>(resolutions.size()) - 1) %
                static_cast<int>(resolutions.size());
            feedback = "Resolution " +
                       resolutions[static_cast<size_t>(resolution_index)];
          },
          [&] {
            resolution_index =
                (resolution_index + 1) %
                static_cast<int>(resolutions.size());
            feedback = "Resolution " +
                       resolutions[static_cast<size_t>(resolution_index)];
          });
    } else if (active_tab == 2) {
      text(300, "Music", 507.f, 151.f, 220.f, 70.f, 31.f);
      slider(context, mk(root.ent(), 301), music_volume,
             box(scale, 760.f, 171.f, 210.f, 24.f)
                 .with_background(Theme::Usage::Secondary)
                 .with_corner_radius(12.f * scale)
                 .with_debug_name("mm_music_slider"),
             SliderHandleValueLabelPosition::None);
      text(302,
           std::to_string(static_cast<int>(std::round(music_volume * 100))) +
               "%",
           992.f, 151.f, 90.f, 70.f, 28.f, TextAlignment::Left);
      text(310, "Sound Effects", 507.f, 247.f, 220.f, 70.f, 31.f);
      slider(context, mk(root.ent(), 311), sound_volume,
             box(scale, 760.f, 267.f, 210.f, 24.f)
                 .with_background(Theme::Usage::Secondary)
                 .with_corner_radius(12.f * scale)
                 .with_debug_name("mm_sound_slider"),
             SliderHandleValueLabelPosition::None);
      text(312,
           std::to_string(static_cast<int>(std::round(sound_volume * 100))) +
               "%",
           992.f, 247.f, 90.f, 70.f, 28.f, TextAlignment::Left);
    } else if (active_tab == 3) {
      const std::string font =
          language_index == 3 ? "Sazanami" : "AtkinsonMock";
      cycle(
          400, "Language", languages[static_cast<size_t>(language_index)],
          184.f, "mm_language",
          [&] {
            language_index =
                (language_index + static_cast<int>(languages.size()) - 1) %
                static_cast<int>(languages.size());
            feedback =
                "Language " + languages[static_cast<size_t>(language_index)];
          },
          [&] {
            language_index =
                (language_index + 1) % static_cast<int>(languages.size());
            feedback =
                "Language " + languages[static_cast<size_t>(language_index)];
          },
          font);
    } else if (active_tab == 4) {
      toggle(500, "Hold to draw or delete", hold_to_draw, 151.f,
             "mm_controls_hold");
      toggle(510, "Vibration", vibration, 247.f, "mm_controls_vibration");
    } else if (active_tab == 5) {
      text(600, "Cross-Save", 507.f, 151.f, 540.f, 65.f, 38.f);
      div(context, mk(root.ent(), 601),
          box(scale, 507.f, 219.f, 570.f, 105.f)
              .with_label("Local account connection demo. No account is contacted "
                          "and no progress is uploaded.")
              .with_font("AtkinsonMock", pixels(25.f * scale))
              .with_custom_text_color(ink)
              .with_text_overflow(TextOverflow::Wrap)
              .with_text_inset(0.f, 0.f)
              .with_letter_spacing(-.4f * scale));
      if (button(
              context, mk(root.ent(), 602),
              box(scale, 507.f, 348.f, 390.f, 58.f)
                  .with_label(account_connected ? "Disconnect demo account"
                                                : "Connect demo account")
                  .with_font("AtkinsonMock", pixels(27.f * scale))
                  .with_custom_background(teal)
                  .with_custom_text_color(afterhours::Color{255, 255, 255, 255})
                  .with_alignment(TextAlignment::Center)
                  .with_text_inset(0.f, 0.f)
                  .with_debug_name("mm_connect"))) {
        account_connected = !account_connected;
        feedback =
            account_connected ? "Demo account connected" : "Demo account disconnected";
      }
    } else {
      text(700, "Mini Motorways", 507.f, 151.f, 540.f, 65.f, 38.f);
      text(701, "Made by Dinosaur Polo Club", 507.f, 219.f, 570.f, 45.f,
           27.f);
      text(702, "Wellington, New Zealand", 507.f, 260.f, 570.f, 45.f, 27.f);
    }

    text(800, "Mini Motorways release-10-patch-2 (202207010917)", 24, 664, 620, 30, 19);
    auto tutorial = button(context, mk(root.ent()), box(scale, 1042, 642, 196, 52)
        .with_label("Tutorial").with_font("AtkinsonMock", pixels(31 * scale))
        .with_custom_background(teal).with_custom_text_color({255, 255, 255, 255})
        .with_alignment(TextAlignment::Left).with_text_inset(16 * scale, 0)
        .with_on_draw_fg([scale](RectangleType r) {
          afterhours::draw_triangle({r.x + 168 * scale, r.y + 26 * scale}, {r.x + 150 * scale, r.y + 14 * scale}, {r.x + 150 * scale, r.y + 38 * scale}, {255,255,255,255});
          afterhours::draw_rectangle({r.x + 138 * scale, r.y + 23 * scale, 16 * scale, 6 * scale}, {255,255,255,255});
        }).with_debug_name("mm_tutorial"));
    tutorial.ent().get<HasLabel>().text_x_offset = 16 * scale;
    if (tutorial) {
      tutorial_open = true;
      feedback.clear();
    }
    if (!feedback.empty() && !tutorial_open)
      div(context, mk(root.ent()), box(scale, 440, 658, 510, 36)
          .with_label(feedback)
          .with_font(feedback == "Language " + languages[3] ? "Sazanami" : "AtkinsonMock",
                     pixels(18 * scale))
          .with_custom_text_color(ink).with_alignment(TextAlignment::Center)
          .with_text_inset(0, 0).with_debug_name("mm_feedback"));

    auto tutorial_panel = afterhours::modal(context, mk(root.ent(), 901), tutorial_open,
        afterhours::ModalConfig{}.with_backdrop_color({27, 39, 42, 105})
            .with_render_layer(41).with_show_close_button(false)
            .with_panel(box(scale, 420, 145, 620, 390)
                .with_custom_background(canvas).with_border(ink, 4 * scale)
                .with_debug_name("mm_tutorial_panel")));
    if (tutorial_panel) {
      div(context, mk(tutorial_panel.ent()),
          box(scale, 42.f, 30.f, 536.f, 65.f)
              .with_label("Build Your First Motorway")
              .with_font("AtkinsonMock", pixels(37.f * scale))
              .with_custom_text_color(ink)
              .with_alignment(TextAlignment::Center)
              .with_text_inset(0.f, 0.f)
              .with_render_layer(42));
      div(context, mk(tutorial_panel.ent()),
          box(scale, 55.f, 107.f, 510.f, 155.f)
              .with_label("1. Drag between matching homes and destinations.\n"
                          "2. Hold to remove a road.\n"
                          "3. Keep every building connected as your city grows.")
              .with_font("AtkinsonMock", pixels(25.f * scale))
              .with_custom_text_color(ink)
              .with_text_overflow(TextOverflow::Wrap)
              .with_text_inset(0.f, 0.f)
              .with_render_layer(42));
      if (button(context, mk(tutorial_panel.ent()),
                 box(scale, 215.f, 297.f, 190.f, 58.f)
                     .with_label("Got it")
                     .with_font("AtkinsonMock", pixels(27.f * scale))
                     .with_custom_background(teal)
                     .with_custom_text_color(
                         afterhours::Color{255, 255, 255, 255})
                     .with_alignment(TextAlignment::Center)
                     .with_text_inset(0.f, 0.f)
                     .with_render_layer(43)
                     .with_debug_name("mm_tutorial_start"))) {
        tutorial_open = false;
        feedback = "Tutorial guide closed";
      }
    }
  }
};

REGISTER_EXAMPLE_SCREEN(mini_motorways_settings, "Game Mockups",
                        "Minimalist settings (Mini Motorways style)",
                        MiniMotorwaysSettingsScreen)
