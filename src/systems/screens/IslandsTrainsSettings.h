#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <algorithm>
#include <array>
#include <string>
#include <vector>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct IslandsTrainsSettingsScreen : ScreenSystem<UIContext<InputAction>> {
  int display_mode = 0;
  int resolution = 1;
  int cam_pan_speed = 5;
  int cam_rotate_speed = 5;
  int effects_volume = 5;
  int music_volume = 5;
  enum class View { Settings, Keyboard, Tutorial, Closed };
  View view = View::Settings;
  int tutorial_step = 0, camera_offset = 0;
  bool track_placed = false, train_running = false;
  float train_progress = 0;

  const afterhours::Color bg{137, 150, 130, 255};
  const afterhours::Color bg_dark{104, 119, 110, 255};
  const afterhours::Color paper{249, 245, 208, 255};
  const afterhours::Color row{218, 225, 200, 255};
  const afterhours::Color text{137, 135, 108, 255};
  const afterhours::Color heading{169, 211, 198, 255};
  const afterhours::Color filled{185, 217, 203, 255};
  const afterhours::Color empty{205, 194, 164, 255};

  std::array<std::string, 3> modes = {"Borderless", "Windowed", "Fullscreen"};
  std::array<std::string, 3> resolutions = {"1920x1080", "2560x1440", "3840x2160"};

  ComponentConfig box(float scale, float x, float y, float w, float h) const {
    return ComponentConfig{}
        .with_size({pixels(w * scale), pixels(h * scale)})
        .with_absolute_position(x * scale, y * scale)
        .with_background(Theme::Usage::None);
  }

  static void draw_background(RectangleType r) {
    afterhours::draw_rectangle(r, afterhours::Color{137, 150, 130, 255});
    afterhours::draw_circle(static_cast<int>(r.x + r.width * .5f),
                            static_cast<int>(r.y + r.height * .5f),
                            r.width * .55f, afterhours::Color{188, 190, 145, 62});
    afterhours::draw_rectangle_gradient_v(
        r, afterhours::Color{100, 114, 106, 95}, afterhours::Color{96, 109, 102, 120});
  }

  static void draw_paper(RectangleType r) {
    const afterhours::Color fill{249, 245, 208, 255};
    afterhours::draw_rectangle({r.x + 7.f, r.y, r.width - 14.f, r.height}, fill);
    afterhours::draw_triangle({r.x + 7.f, r.y}, {r.x + 1.f, r.y + 118.f},
                              {r.x + 7.f, r.y + 236.f}, fill);
    afterhours::draw_triangle({r.x + 7.f, r.y + 236.f}, {r.x, r.y + 470.f},
                              {r.x + 8.f, r.y + 672.f}, fill);
    afterhours::draw_triangle({r.x + r.width - 7.f, r.y},
                              {r.x + r.width - 1.f, r.y + 54.f},
                              {r.x + r.width - 7.f, r.y + 122.f}, fill);
    afterhours::draw_triangle({r.x + r.width - 8.f, r.y + 122.f},
                              {r.x + r.width, r.y + 376.f},
                              {r.x + r.width - 8.f, r.y + 664.f}, fill);
    afterhours::draw_rectangle({r.x + 4.f, r.y + r.height - 56.f,
                                r.width - 8.f, 56.f}, fill);
  }

  static void draw_close_shape(RectangleType r) {
    afterhours::draw_poly({r.x + r.width * .5f, r.y + r.height * .5f}, 12,
                          r.width * .48f, 15.f,
                          afterhours::Color{245, 242, 205, 255});
  }

  void reset_defaults() {
    display_mode = 0;
    resolution = 1;
    cam_pan_speed = 5;
    cam_rotate_speed = 5;
    effects_volume = 5;
    music_volume = 5;
    view = View::Settings;
  }

  void cycle_value(int &value, int delta, int count) {
    value = (value + delta + count) % count;
  }

  void render_section(UIContext<InputAction> &context, afterhours::Entity &parent,
                      float scale, int id, const std::string &label, float y) {
    div(context, mk(parent, id),
        box(scale, 456, y, 160, 22)
            .with_label(label)
            .with_font("Gaegu-Bold", h720(20.f))
            .with_letter_spacing(-1.f * scale)
            .with_custom_text_color(heading)
            .with_alignment(TextAlignment::Left));
  }

  void render_cycle_row(UIContext<InputAction> &context, afterhours::Entity &parent,
                        float scale, int id, const std::string &label,
                        int &value, const std::array<std::string, 3> &values,
                        float y, const std::string &debug_base) {
    div(context, mk(parent, id),
        box(scale, 456, y, 367, 26)
            .with_custom_background(row)
            .with_corner_radius(6.f * scale)
            .with_debug_name(debug_base + "_row"));
    div(context, mk(parent, id + 1),
        box(scale, 473, y + 2, 160, 22)
            .with_label(label)
            .with_font("Gaegu-Bold", h720(22.f))
            .with_letter_spacing(-.5f * scale)
            .with_custom_text_color(text)
            .with_alignment(TextAlignment::Left));
    if (button(context, mk(parent, id + 2),
               box(scale, 645, y, 24, 26)
                   .with_label("<")
                   .with_font("Archivo@bold", h720(22.f))
                   .with_custom_text_color(text)
                   .with_alignment(TextAlignment::Center)
                   .with_debug_name(debug_base + "_prev"))) {
      cycle_value(value, -1, static_cast<int>(values.size()));
    }
    div(context, mk(parent, id + 3),
        box(scale, 684, y + 2, 105, 22)
            .with_label(values[static_cast<size_t>(value)])
            .with_font("Gaegu-Bold", h720(21.f))
            .with_letter_spacing(-.5f * scale)
            .with_custom_text_color(text)
            .with_alignment(TextAlignment::Center)
            .with_debug_name(debug_base + "_value"));
    if (button(context, mk(parent, id + 4),
               box(scale, 794, y, 24, 26)
                   .with_label(">")
                   .with_font("Archivo@bold", h720(22.f))
                   .with_custom_text_color(text)
                   .with_alignment(TextAlignment::Center)
                   .with_debug_name(debug_base + "_next"))) {
      cycle_value(value, 1, static_cast<int>(values.size()));
    }
  }

  void render_step_row(UIContext<InputAction> &context, afterhours::Entity &parent,
                       float scale, int id, const std::string &label, int &value,
                       float y, const std::string &debug_base) {
    div(context, mk(parent, id),
        box(scale, 456, y, 367, 26)
            .with_custom_background(row)
            .with_corner_radius(6.f * scale)
            .with_debug_name(debug_base + "_row"));
    div(context, mk(parent, id + 1),
        box(scale, 473, y + 1, 218, 24)
            .with_label(label)
            .with_font("Gaegu-Bold", h720(22.f))
            .with_letter_spacing(-.5f * scale)
            .with_custom_text_color(text)
            .with_alignment(TextAlignment::Left));
    if (button(context, mk(parent, id + 2),
               box(scale, 662, y, 22, 26)
                   .with_label("<")
                   .with_font("Archivo@bold", h720(22.f))
                   .with_custom_text_color(text)
                   .with_alignment(TextAlignment::Center)
                   .with_debug_name(debug_base + "_prev"))) {
      value = std::max(0, value - 1);
    }
    for (int i = 0; i < 10; ++i) {
      const bool on = i < value;
      if (button(context, mk(parent, id + 10 + i),
                 box(scale, 686 + static_cast<float>(i) * 11.f, y + 7, 7, 14)
                     .with_label("")
                     .with_custom_background(on ? filled : empty)
                     .with_corner_radius(1.f * scale)
                     .with_debug_name(debug_base + "_segment_" + std::to_string(i + 1)))) {
        value = i + 1;
      }
    }
    if (button(context, mk(parent, id + 22),
               box(scale, 796, y, 22, 26)
                   .with_label(">")
                   .with_font("Archivo@bold", h720(22.f))
                   .with_custom_text_color(text)
                   .with_alignment(TextAlignment::Center)
                   .with_debug_name(debug_base + "_next"))) {
      value = std::min(10, value + 1);
    }
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float dt) override {
    const float screen_w = context.screen_width > 0.f
                               ? context.screen_width
                               : static_cast<float>(Settings::get().get_screen_width());
    const float screen_h = context.screen_height > 0.f
                               ? context.screen_height
                               : static_cast<float>(Settings::get().get_screen_height());
    const float scale = std::min(screen_w / 1280.f, screen_h / 720.f);

    Theme theme;
    theme.font = text;
    theme.darkfont = paper;
    theme.font_muted = text;
    theme.background = bg;
    theme.surface = paper;
    theme.primary = row;
    theme.secondary = heading;
    theme.accent = filled;
    theme.roundness = .12f;
    theme.segments = 8;
    context.set_theme(theme);
    context.scaling_mode = ScalingMode::Proportional;
    UIStylingDefaults::get().set_default_font("Gaegu-Bold", h720(21.f));

    auto root = div(context, mk(entity, 0),
                    box(scale, 0, 0, 1280, 720)
                        .with_on_draw_bg([](RectangleType r) { draw_background(r); })
                        .with_debug_name("it_root"));

    if (context.pressed(InputAction::MenuBack))
      view = view == View::Settings ? View::Closed : View::Settings;
    if (view == View::Closed) {
      detail_label(context, root.ent(), scale, 500, "Settings closed", 560, 308, 240, 40, 30);
      if (detail_button(context, root.ent(), scale, 501, "REOPEN SETTINGS", 500, 370, 280, 43, "it_reopen"))
        view = View::Settings;
      return;
    }

    div(context, mk(root.ent(), 1),
        box(scale, 416, 0, 447, 720)
            .with_on_draw_bg([](RectangleType r) { draw_paper(r); })
            .with_debug_name("it_paper"));

    if (button(context, mk(root.ent(), 2),
               box(scale, 1212, 12, 47, 48)
                   .with_label("X")
                   .with_on_draw_bg([](RectangleType r) { draw_close_shape(r); })
                   .with_font("Archivo@bold", h720(44.f))
                   .with_custom_text_color(text)
                   .with_alignment(TextAlignment::Center)
                   .with_debug_name("it_close"))) {
      view = View::Closed;
    }

    if (view == View::Keyboard || view == View::Tutorial) {
      render_detail(context, root.ent(), scale, dt);
      return;
    }

    div(context, mk(root.ent(), 10),
        box(scale, 592, 26, 130, 35)
            .with_label("SETTINGS")
            .with_font("Gaegu-Bold", h720(29.f))
            .with_letter_spacing(-1.f * scale)
            .with_custom_text_color(text)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("it_title"));

    render_section(context, root.ent(), scale, 20, "DISPLAY", 89);
    render_cycle_row(context, root.ent(), scale, 30, "Mode", display_mode, modes,
                     115, "it_mode");
    render_cycle_row(context, root.ent(), scale, 40, "Resolution", resolution,
                     resolutions, 149, "it_resolution");

    render_section(context, root.ent(), scale, 60, "CONTROLS", 198);
    render_step_row(context, root.ent(), scale, 70, "Cam panning speed",
                    cam_pan_speed, 223, "it_pan");
    render_step_row(context, root.ent(), scale, 100, "Cam rotating Speed",
                    cam_rotate_speed, 257, "it_rotate");
    if (button(context, mk(root.ent(), 130),
               box(scale, 456, 289, 367, 26)
                   .with_label("KEYBOARD")
                   .with_custom_background(row)
                   .with_corner_radius(5.f * scale)
                   .with_font("Gaegu-Bold", h720(21.f))
                   .with_custom_text_color(text)
                   .with_alignment(TextAlignment::Center)
                   .with_debug_name("it_keyboard"))) {
      view = View::Keyboard;
    }

    render_section(context, root.ent(), scale, 140, "AUDIO", 344);
    render_step_row(context, root.ent(), scale, 150, "Effects Volume",
                    effects_volume, 370, "it_effects");
    render_step_row(context, root.ent(), scale, 180, "Music Volume", music_volume,
                    404, "it_music");

    render_section(context, root.ent(), scale, 210, "TUTORIAL", 455);
    if (button(context, mk(root.ent(), 220),
               box(scale, 456, 475, 367, 26)
                   .with_label("PLAY TUTORIAL")
                   .with_custom_background(row)
                   .with_corner_radius(5.f * scale)
                   .with_font("Gaegu-Bold", h720(21.f))
                   .with_custom_text_color(text)
                   .with_alignment(TextAlignment::Center)
                   .with_debug_name("it_play_tutorial"))) {
      view = View::Tutorial;
      tutorial_step = 0; camera_offset = 0; track_placed = false;
      train_running = false; train_progress = 0;
    }

    if (button(context, mk(root.ent(), 230),
               box(scale, 520, 690, 240, 25)
                   .with_label("RESET TO DEFAULTS")
                   .with_font("Gaegu-Bold", h720(22.f))
                   .with_letter_spacing(-1.f * scale)
                   .with_custom_text_color(text)
                   .with_alignment(TextAlignment::Center)
                   .with_debug_name("it_reset"))) {
      reset_defaults();
    }


  }

  void detail_label(UIContext<InputAction> &c, afterhours::Entity &p, float scale,
                    int id, const std::string &label, float x, float y,
                    float w, float h, float size) {
    div(c, mk(p, id), box(scale, x, y, w, h).with_label(label)
        .with_font("GaeguMock", h720(size)).with_custom_text_color(text)
        .with_alignment(TextAlignment::Center).with_text_overflow(TextOverflow::Wrap)
        .with_ignore_pointer_events());
  }

  ElementResult detail_button(UIContext<InputAction> &c, afterhours::Entity &p,
                               float scale, int id, const std::string &label,
                               float x, float y, float w, float h,
                               const std::string &name, bool disabled = false) {
    return button(c, mk(p, id), box(scale, x, y, w, h).with_label(label)
        .with_font("GaeguMock", h720(24)).with_custom_text_color(text)
        .with_custom_background(row).with_corner_radius(5 * scale)
        .with_alignment(TextAlignment::Center).with_disabled(disabled)
        .with_click_activation(ClickActivationMode::Release).with_debug_name(name));
  }

  void render_detail(UIContext<InputAction> &c, afterhours::Entity &p, float scale, float dt) {
    detail_label(c, p, scale, 510, view == View::Keyboard ? "KEYBOARD" : "LOCAL TUTORIAL",
                 456, 28, 367, 43, 31);
    if (detail_button(c, p, scale, 511, "BACK TO SETTINGS", 490, 653, 300, 42, "it_detail_back")) {
      view = View::Settings; return;
    }
    if (view == View::Keyboard) {
      detail_label(c, p, scale, 512, "Keyboard controls for this mock", 456, 96, 367, 54, 23);
      const std::array<std::pair<const char *, const char *>, 5> bindings{{
          {"Choose a control", "Tab / Shift+Tab"}, {"Activate a control", "Enter"},
          {"Pan in the tutorial", "Left / Right"}, {"Next tutorial step", "Enter"},
          {"Back to settings", "Esc"}}};
      for (int i = 0; i < 5; ++i) {
        const float y = 170 + i * 63;
        div(c, mk(p, 520 + i), box(scale, 456, y, 367, 52).with_custom_background(row).with_corner_radius(5 * scale));
        detail_label(c, p, scale, 530 + i, bindings[i].first, 469, y + 2, 342, 25, 22);
        detail_label(c, p, scale, 540 + i, bindings[i].second, 469, y + 26, 342, 24, 21);
      }
      if (detail_button(c, p, scale, 550, "TRY THE TUTORIAL", 490, 538, 300, 43, "it_keyboard_practice")) {
        view = View::Tutorial; tutorial_step = 0; camera_offset = 0;
        track_placed = false; train_running = false; train_progress = 0;
      }
      return;
    }
    if (tutorial_step == 0) {
      if (c.pressed_or_repeat(InputAction::WidgetLeft)) camera_offset = std::max(-2, camera_offset - 1);
      if (c.pressed_or_repeat(InputAction::WidgetRight)) camera_offset = std::min(2, camera_offset + 1);
    }
    if (train_running) {
      train_progress = std::min(1.f, train_progress + std::min(dt, .1f) * .4f);
      if (train_progress >= 1) train_running = false;
    }
    const bool complete = tutorial_step == 0 ? camera_offset != 0 : tutorial_step == 1 ? track_placed : train_progress >= 1;
    const std::array<const char *, 3> titles{"1. Move the camera", "2. Lay the track", "3. Send a train"};
    const std::array<const char *, 3> instructions{
        "Use Left / Right or the buttons to pan the island.",
        "Place a track to connect the two stations.",
        "Start the train and watch it reach the second station."};
    detail_label(c, p, scale, 560, titles[tutorial_step], 456, 105, 367, 41, 27);
    detail_label(c, p, scale, 561, instructions[tutorial_step], 456, 153, 367, 77, 23);
    div(c, mk(p, 562), box(scale, 456, 251, 367, 180).with_custom_background(row)
        .with_corner_radius(10 * scale).with_ignore_pointer_events().with_debug_name("it_practice_scene")
        .with_on_draw_fg([offset = camera_offset, track = track_placed, progress = train_progress](RectangleType r) {
          const float s = r.width / 367;
          const float shift = offset * 12 * s;
          const float left = r.x + 67 * s + shift, right = r.x + 287 * s + shift, y = r.y + 91 * s;
          afterhours::draw_circle(static_cast<int>(r.x + r.width / 2 + shift), static_cast<int>(y), 76 * s, {185, 211, 170, 255});
          if (track) {
            afterhours::draw_line_ex({left, y - 4 * s}, {right, y - 4 * s}, 3 * s, {137, 135, 108, 255});
            afterhours::draw_line_ex({left, y + 4 * s}, {right, y + 4 * s}, 3 * s, {137, 135, 108, 255});
            for (int i = 0; i < 12; ++i) {
              const float x = left + (right - left) * i / 11;
              afterhours::draw_line_ex({x, y - 8 * s}, {x, y + 8 * s}, 3 * s, {137, 135, 108, 255});
            }
          }
          for (float x : {left, right}) {
            afterhours::draw_rectangle({x - 15 * s, y - 32 * s, 30 * s, 26 * s}, {249, 245, 208, 255});
            afterhours::draw_triangle({x - 20 * s, y - 32 * s}, {x + 20 * s, y - 32 * s}, {x, y - 47 * s}, {137, 135, 108, 255});
          }
          if (track) afterhours::draw_rectangle({left + (right - left) * progress - 11 * s, y - 10 * s, 22 * s, 16 * s}, {100, 151, 151, 255});
        }));
    if (tutorial_step == 0) {
      if (detail_button(c, p, scale, 563, "PAN LEFT", 470, 451, 163, 41, "it_tutorial_left")) camera_offset = std::max(-2, camera_offset - 1);
      if (detail_button(c, p, scale, 564, "PAN RIGHT", 647, 451, 163, 41, "it_tutorial_right")) camera_offset = std::min(2, camera_offset + 1);
    } else if (tutorial_step == 1) {
      if (detail_button(c, p, scale, 565, track_placed ? "TRACK CONNECTED" : "PLACE TRACK", 490, 451, 300, 41, "it_tutorial_track", track_placed)) track_placed = true;
    } else if (detail_button(c, p, scale, 566, train_running ? "TRAIN MOVING" : train_progress >= 1 ? "TRAIN ARRIVED" : "START TRAIN",
                            490, 451, 300, 41, "it_tutorial_train", train_running || train_progress >= 1)) train_running = true;
    detail_label(c, p, scale, 567, complete ? "Step complete!" : "Try the action above to continue.", 456, 511, 367, 39, 23);
    if (detail_button(c, p, scale, 568, tutorial_step == 2 ? "FINISH TUTORIAL" : "NEXT STEP", 490, 572, 300, 41, "it_tutorial_next", !complete) ||
        (complete && c.pressed(InputAction::WidgetPress))) {
      if (tutorial_step == 2) view = View::Settings; else ++tutorial_step;
    }
  }

};

REGISTER_EXAMPLE_SCREEN(islands_trains_settings, "Game Mockups",
                        "Calm puzzle game settings (Islands & Trains style)",
                        IslandsTrainsSettingsScreen)
