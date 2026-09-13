#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>
#include <array>
#include <string>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct AngryBirdsSettingsScreen : ScreenSystem<UIContext<InputAction>> {
  enum struct AudioIcon { Music, Sound, Vibration };
  enum struct DetailPanel { None, Progress, Language, Credits, Support, Terms };

  size_t active_tab = 0;
  bool music_on = true;
  bool sound_on = true;
  bool vibration_on = false;
  bool notifications_off = true;
  bool settings_open = true;
  DetailPanel detail_panel = DetailPanel::None;
  int current_level = 12;
  int saved_level = 9;
  size_t language = 0, saved_language = 0;
  float layout_scale = 1.f;

  bool saved_music_on = true;
  bool saved_sound_on = true;
  bool saved_vibration_on = false;
  bool saved_notifications_off = true;
  std::string status_message;

  const afterhours::Color cream{255, 248, 225, 255};
  const afterhours::Color brown{111, 65, 44, 255};
  const afterhours::Color brown_muted{132, 78, 53, 255};
  const afterhours::Color green{75, 213, 6, 255};
  const afterhours::Color blue{34, 161, 215, 255};
  const afterhours::Color gray{148, 157, 139, 255};

  enum ArtIndex : size_t {
    Forest,
    Board,
    PillBlue,
    PillGreen,
    PillFooterBlue,
    PillFooterGreen,
    PillTabBlue,
    PillTabGreen,
    MusicOn,
    MusicOff,
    SoundOn,
    SoundOff,
    VibrationOn,
    VibrationOff,
    Close,
    Wifi,
    ArtCount,
  };

  std::array<raylib::Texture2D, ArtCount> art{};
  bool art_loaded = false;

  void load_art() {
    if (art_loaded)
      return;
    constexpr std::array<const char *, ArtCount> names{
        "forest",
        "board",
        "pill_blue",
        "pill_green",
        "pill_footer_blue",
        "pill_footer_green",
        "pill_tab_blue",
        "pill_tab_green",
        "music_on",
        "music_off",
        "sound_on",
        "sound_off",
        "vibration_on",
        "vibration_off",
        "close",
        "wifi",
    };
    for (size_t i = 0; i < names.size(); ++i) {
      const std::string path =
          afterhours::files::get_resource_path(
              "images", std::string(i == Forest || i == Close || i == Wifi
                                        ? "mobile_settings/" : "angry_settings/") + names[i] + ".png")
              .string();
      art[i] = raylib::LoadTexture(path.c_str());
      raylib::SetTextureFilter(art[i], raylib::TEXTURE_FILTER_BILINEAR);
    }
    art_loaded = true;
  }

  static void paint(raylib::Texture2D texture, RectangleType destination) {
    raylib::DrawTexturePro(texture,
                           {0.f, 0.f, static_cast<float>(texture.width),
                            static_cast<float>(texture.height)},
                           destination, {0.f, 0.f}, 0.f, raylib::WHITE);
  }

  static void paint_plate(raylib::Texture2D texture, RectangleType r,
                          float base_width, float base_height) {
    const float sx = r.width / base_width;
    const float sy = r.height / base_height;
    paint(texture, {r.x - 8.f * sx, r.y - 8.f * sy,
                    static_cast<float>(texture.width) * 0.5f * sx,
                    static_cast<float>(texture.height) * 0.5f * sy});
  }

  ElementResult pill_button(UIContext<InputAction> &context,
                            afterhours::Entity &parent, int id,
                            const std::string &label, float x, float y, float w,
                            float h, bool selected, const std::string &name,
                            float font_size = 24.f, bool disabled = false) {
    const bool footer_plate = h == 55.f && w <= 176.f;
    const bool tab_plate = w == 262.f && h == 55.f;
    const auto texture =
        art[footer_plate ? (selected ? PillFooterGreen : PillFooterBlue)
            : tab_plate  ? (selected ? PillTabGreen : PillTabBlue)
                         : (selected ? PillGreen : PillBlue)];
    const float plate_width = footer_plate ? 144.f : tab_plate ? 262.f : 400.f;
    const float plate_height = (footer_plate || tab_plate) ? 55.f : 77.f;
    return button(context, mk(parent, id),
                  ComponentConfig{}
                      .with_label(label)
                      .with_size(ComponentSize{pixels(w * layout_scale),
                                               pixels(h * layout_scale)})
                      .with_absolute_position(pixels(x * layout_scale),
                                              pixels(y * layout_scale))
                      .with_background(Theme::Usage::None)
                      .with_corner_radius(h * layout_scale / 2.f)
                      .with_custom_text_color(name == "ab_cancel" || disabled ? brown : cream)
                      .with_font("FredokaMockBold",
                                 pixels(font_size * layout_scale * 1.2f))
                      .with_alignment(TextAlignment::Center)
                      .with_text_stroke(afterhours::Color{119, 82, 46, 255},
                                        name == "ab_cancel" || disabled ? 0.f : 0.6f * layout_scale)
                      .with_text_shadow(afterhours::Color{112, 78, 49, 120},
                                        0.f, name == "ab_cancel" || disabled ? 0.f : layout_scale)
                      .with_click_activation(ClickActivationMode::Release)
                      .with_disabled(disabled)
                      .with_on_draw_bg([texture, plate_width, plate_height, disabled,
                                        quiet = name == "ab_cancel", scale = layout_scale](RectangleType r) {
                        if (!quiet && !disabled) {
                          paint_plate(texture, r, plate_width, plate_height);
                          return;
                        }
                        afterhours::draw_rectangle_rounded(r, 1.f, 24,
                            disabled ? afterhours::Color{226, 207, 178, 255} : afterhours::Color{255, 233, 195, 255},
                            RoundedCorners().all_round());
                        afterhours::draw_rectangle_rounded_lines_ex(r, 1.f, 24, 2.f * scale,
                            afterhours::Color{164, 127, 89, 255});
                      })
                      .with_debug_name(name));
  }

  ElementResult audio_button(UIContext<InputAction> &context,
                             afterhours::Entity &parent, int id, AudioIcon icon,
                             bool enabled, float x, const std::string &name) {
    const size_t texture_index =
        icon == AudioIcon::Music   ? (enabled ? MusicOn : MusicOff)
        : icon == AudioIcon::Sound ? (enabled ? SoundOn : SoundOff)
                                   : (enabled ? VibrationOn : VibrationOff);
    const auto texture = art[texture_index];
    return button(context, mk(parent, id),
                  ComponentConfig{}
                      .with_size(ComponentSize{pixels(148.f * layout_scale),
                                               pixels(107.f * layout_scale)})
                      .with_absolute_position(pixels(x * layout_scale),
                                              pixels(235.f * layout_scale))
                      .with_background(Theme::Usage::None)
                      .with_corner_radius(53.5f * layout_scale)
                      .with_click_activation(ClickActivationMode::Release)
                      .with_on_draw_bg([texture](RectangleType r) {
                        paint_plate(texture, r, 119.f, 82.f);
                      })
                      .with_debug_name(name));
  }

  void action_label(UIContext<InputAction> &context,
                    afterhours::Entity &board) {
    div(context, mk(board, 90),
        ComponentConfig{}
            .with_label(std::string(has_changes() ? "Unsaved changes" : "All changes saved") +
                        (status_message.empty() ? "" : "\n" + status_message))
            .with_size(ComponentSize{pixels(300.f * layout_scale),
                                     pixels(55.f * layout_scale)})
            .with_absolute_position(pixels(64.f * layout_scale),
                                    pixels(484.f * layout_scale))
            .with_background(Theme::Usage::None)
            .with_custom_text_color(brown_muted)
            .with_font("FredokaMockBold", pixels(20.f * layout_scale))
            .with_alignment(TextAlignment::Left)
            .with_debug_name("ab_status"));
  }

  bool has_changes() const {
    return music_on != saved_music_on || sound_on != saved_sound_on ||
           vibration_on != saved_vibration_on || notifications_off != saved_notifications_off ||
           language != saved_language;
  }

  void save_settings(const std::string &message) {
    saved_music_on = music_on;
    saved_sound_on = sound_on;
    saved_vibration_on = vibration_on;
    saved_notifications_off = notifications_off;
    saved_language = language;
    status_message = message;
  }

  void restore_settings() {
    music_on = saved_music_on;
    sound_on = saved_sound_on;
    vibration_on = saved_vibration_on;
    notifications_off = saved_notifications_off;
    language = saved_language;
    status_message = "Changes restored";
  }

  void render_tabs(UIContext<InputAction> &context, afterhours::Entity &board) {
    constexpr std::array<const char *, 3> labels{"AUDIO", "GENERAL", "ABOUT"};
    constexpr std::array<const char *, 3> names{
        "ab_tab_audio", "ab_tab_general", "ab_tab_info"};
    for (int i = 0; i < 3; i++) {
      if (pill_button(context, board, 10 + i, labels[(size_t)i],
                      61.f + static_cast<float>(i) * 277.f, 120.f, 262.f, 55.f,
                      active_tab == static_cast<size_t>(i), names[(size_t)i])) {
        active_tab = static_cast<size_t>(i);
        status_message.clear();
      }
      if (active_tab != static_cast<size_t>(i)) continue;
      div(context, mk(board, 13 + i), ComponentConfig{}
          .with_size({pixels(42.f * layout_scale), pixels(3.f * layout_scale)})
          .with_absolute_position(pixels((171.f + i * 277.f) * layout_scale), pixels(166.f * layout_scale))
          .with_custom_background(cream).with_corner_radius(0).with_ignore_pointer_events());
    }
  }

  void render_audio(UIContext<InputAction> &context,
                    afterhours::Entity &board) {
    div(context, mk(board, 20),
        ComponentConfig{}
            .with_label("Tap an icon to toggle. Your changes apply when saved.")
            .with_size(ComponentSize{pixels(700.f * layout_scale),
                                     pixels(30.f * layout_scale)})
            .with_absolute_position(pixels(120.f * layout_scale),
                                    pixels(183.f * layout_scale))
            .with_background(Theme::Usage::None)
            .with_custom_text_color(brown_muted)
            .with_font("FredokaMockBold", pixels(22.f * layout_scale))
            .with_alignment(TextAlignment::Center)
            .with_debug_name("ab_audio_hint"));

    const std::array<float, 3> xs{179.f, 396.f, 614.f};
    const std::array<const char *, 3> labels{"MUSIC", "SOUND EFFECTS", "VIBRATION"};
    const std::array<const char *, 3> names{"ab_music", "ab_sound",
                                            "ab_vibration"};
    const std::array<AudioIcon, 3> icons{AudioIcon::Music, AudioIcon::Sound,
                                         AudioIcon::Vibration};
    std::array<bool *, 3> values{&music_on, &sound_on, &vibration_on};

    for (int i = 0; i < 3; i++) {
      const bool enabled = *values[(size_t)i];
      const auto toggle = [&] {
        *values[(size_t)i] = !enabled;
        const char *display = i == 0 ? "Music" : i == 1 ? "Sound" : "Vibration";
        status_message = std::string(display) + (!enabled ? " enabled" : " disabled");
      };
      if (audio_button(context, board, 30 + i, icons[(size_t)i], enabled,
                       xs[(size_t)i], names[(size_t)i])) toggle();

      div(context, mk(board, 40 + i),
          ComponentConfig{}
              .with_label(labels[(size_t)i])
              .with_size(ComponentSize{pixels(216.f * layout_scale),
                                       pixels(34.f * layout_scale)})
              .with_absolute_position(pixels((xs[(size_t)i] - 34.f) * layout_scale),
                                      pixels(354.f * layout_scale))
              .with_background(Theme::Usage::None)
              .with_custom_text_color(brown)
              .with_font("FredokaMockBold", pixels(27.f * layout_scale))
              .with_alignment(TextAlignment::Center));
      div(context, mk(board, 50 + i),
          ComponentConfig{}
              .with_label(enabled ? "ON" : "OFF")
              .with_size(ComponentSize{pixels(74.f * layout_scale),
                                       pixels(29.f * layout_scale)})
              .with_absolute_position(pixels((xs[(size_t)i] + 64.f) * layout_scale),
                                      pixels(395.f * layout_scale))
              .with_background(Theme::Usage::None)
              .with_custom_text_color(brown_muted)
              .with_font("FredokaMockBold", pixels(24.f * layout_scale))
              .with_alignment(TextAlignment::Center)
              .with_debug_name(std::string(names[(size_t)i]) + "_state"));
      if (button(context, mk(board, 60 + i), ComponentConfig{}
          .with_size({pixels(148.f * layout_scale), pixels(32.f * layout_scale)})
          .with_absolute_position(pixels(xs[(size_t)i] * layout_scale), pixels(394.f * layout_scale))
          .with_background(Theme::Usage::None).with_corner_radius(16.f * layout_scale)
          .with_click_activation(ClickActivationMode::Release)
          .with_debug_name(std::string(names[(size_t)i]) + "_toggle")
          .with_on_draw_bg([enabled, scale = layout_scale](RectangleType r) {
            r = {r.x + 18.f * scale, r.y + 4.f * scale, 46.f * scale, 24.f * scale};
            afterhours::draw_rectangle_rounded(r, 1.f, 20,
                enabled ? afterhours::Color{68, 137, 22, 255} : afterhours::Color{123, 101, 74, 255},
                RoundedCorners().all_round());
            raylib::DrawCircleV({r.x + (enabled ? 34.f : 12.f) * scale, r.y + r.height / 2},
                               9.f * scale, {255, 248, 225, 255});
          }))) toggle();
    }
  }

  void render_general(UIContext<InputAction> &context,
                      afterhours::Entity &board) {
    if (pill_button(context, board, 20, "SAVE/LOAD PROGRESS", 61.f, 226.f,
                    817.f, 75.f, false, "ab_save_progress", 27.f)) {
      detail_panel = DetailPanel::Progress;
      status_message.clear();
    }

    const std::string notification_label =
        notifications_off ? "NOTIFICATIONS: OFF" : "NOTIFICATIONS: ON";
    if (pill_button(context, board, 21, notification_label, 61.f, 322.f, 400.f,
                    65.f, false, "ab_notifications", 23.f)) {
      notifications_off = !notifications_off;
      status_message = notifications_off ? "Notifications disabled"
                                         : "Notifications enabled";
    }
    if (pill_button(context, board, 22, "LANGUAGE", 479.f, 322.f, 399.f, 65.f,
                    false, "ab_language", 24.f)) {
      detail_panel = DetailPanel::Language;
      status_message.clear();
    }
  }

  void render_info(UIContext<InputAction> &context, afterhours::Entity &board) {
    constexpr std::array<const char *, 3> labels{"CREDITS", "SUPPORT",
                                                 "TERMS & PRIVACY"};
    constexpr std::array<const char *, 3> names{"ab_credits", "ab_support",
                                                "ab_terms"};
    constexpr std::array<DetailPanel, 3> panels{
        DetailPanel::Credits, DetailPanel::Support, DetailPanel::Terms};
    for (int i = 0; i < 3; i++) {
      if (pill_button(context, board, 20 + i, labels[(size_t)i],
                      61.f + static_cast<float>(i) * 273.f, 238.f, 260.f, 65.f,
                      false, names[(size_t)i], 22.f)) {
        detail_panel = panels[(size_t)i];
        status_message.clear();
      }
    }

    div(context, mk(board, 30),
        ComponentConfig{}
            .with_label("Version 1.11.0.12346\nBuild 15555.1.114203\nPlayer "
                        "#281-676-956")
            .with_size(ComponentSize{pixels(440.f * layout_scale),
                                     pixels(90.f * layout_scale)})
            .with_absolute_position(pixels(250.f * layout_scale),
                                    pixels(333.f * layout_scale))
            .with_background(Theme::Usage::None)
            .with_custom_text_color(brown_muted)
            .with_font("FredokaMockBold", pixels(22.f * layout_scale))
            .with_alignment(TextAlignment::Center)
            .with_debug_name("ab_build_info"));
  }

  void detail_text(UIContext<InputAction> &context, afterhours::Entity &board,
                   int id, const std::string &label, float x, float y, float w,
                   float h, float font_size, const std::string &name,
                   afterhours::Color color) {
    div(context, mk(board, id),
        ComponentConfig{}
            .with_label(label)
            .with_size(ComponentSize{pixels(w * layout_scale),
                                     pixels(h * layout_scale)})
            .with_absolute_position(pixels(x * layout_scale),
                                    pixels(y * layout_scale))
            .with_background(Theme::Usage::None)
            .with_custom_text_color(color)
            .with_font("FredokaMockBold",
                       pixels(font_size * layout_scale * 1.2f))
            .with_text_overflow(TextOverflow::Wrap)
            .with_alignment(TextAlignment::Center)
            .with_debug_name(name));
  }

  void render_detail(UIContext<InputAction> &context,
                     afterhours::Entity &board) {
    div(context, mk(board, 100),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(786.f * layout_scale),
                                     pixels(390.f * layout_scale)})
            .with_absolute_position(pixels(77.f * layout_scale),
                                    pixels(130.f * layout_scale))
            .with_background(Theme::Usage::None)
            .with_on_draw_bg([scale = layout_scale](RectangleType r) {
              afterhours::draw_rectangle_rounded(
                  {r.x, r.y + 5.f * scale, r.width, r.height}, 0.16f, 20,
                  afterhours::Color{139, 89, 70, 120},
                  RoundedCorners().all_round());
              afterhours::draw_rectangle_rounded(
                  r, 0.16f, 20, afterhours::Color{255, 232, 190, 255},
                  RoundedCorners().all_round());
              afterhours::draw_rectangle_rounded_lines_ex(
                  r, 0.16f, 20, 3.f * scale,
                  afterhours::Color{193, 137, 101, 255});
            })
            .with_debug_name("ab_detail_panel"));

    std::string title;
    std::string body;
    switch (detail_panel) {
    case DetailPanel::Progress:
      title = "SAVE / LOAD PROGRESS";
      body = "CURRENT CAMPAIGN: LEVEL " + std::to_string(current_level) +
             " / 30\nSAVED SLOT: LEVEL " + std::to_string(saved_level) +
             " / 30";
      break;
    case DetailPanel::Language:
      title = "SELECT LANGUAGE";
      body = "CURRENT LANGUAGE: " + std::string(language == 0   ? "ENGLISH"
                                                : language == 1 ? "ESPAÑOL"
                                                                : "DEUTSCH");
      break;
    case DetailPanel::Credits:
      title = "CREDITS";
      body = "ROVIO ENTERTAINMENT\nGAME DESIGN • ART • ENGINEERING\n"
             "Thank you for playing!";
      break;
    case DetailPanel::Support:
      title = "HELP & SUPPORT";
      body = "OFFLINE HELP CENTER\nPlayer #281-676-956\n"
             "Progress and settings are stored on this device.";
      break;
    case DetailPanel::Terms:
      title = "TERMS & PRIVACY";
      body = "Your settings stay on this device. This demo does not connect "
             "to a network or collect personal data.";
      break;
    case DetailPanel::None:
      return;
    }

    detail_text(context, board, 101, title, 120.f, 156.f, 700.f, 45.f, 30.f,
                "ab_detail_title", brown);

    if (detail_panel == DetailPanel::Progress) {
      detail_text(context, board, 102, body, 145.f, 218.f, 650.f, 62.f, 20.f,
                  "ab_progress_state", brown_muted);
      if (pill_button(context, board, 103, "PLAY NEXT", 112.f, 307.f, 218.f,
                      61.f, false, "ab_progress_next", 20.f)) {
        current_level = current_level == 30 ? 1 : current_level + 1;
        status_message = "Current campaign advanced";
      }
      if (pill_button(context, board, 104, "SAVE CURRENT", 361.f, 307.f, 218.f,
                      61.f, true, "ab_progress_save", 20.f)) {
        saved_level = current_level;
        status_message = "Progress saved to local slot";
      }
      if (pill_button(context, board, 105, "LOAD SAVED", 610.f, 307.f, 218.f,
                      61.f, false, "ab_progress_load", 20.f)) {
        current_level = saved_level;
        status_message = "Saved progress loaded";
      }
    } else if (detail_panel == DetailPanel::Language) {
      detail_text(context, board, 102, body, 145.f, 224.f, 650.f, 35.f, 20.f,
                  "ab_language_state", brown_muted);
      constexpr std::array<const char *, 3> labels{"ENGLISH", "ESPAÑOL",
                                                   "DEUTSCH"};
      constexpr std::array<const char *, 3> names{
          "ab_language_english", "ab_language_spanish", "ab_language_german"};
      for (int i = 0; i < 3; i++) {
        if (pill_button(context, board, 103 + i, labels[(size_t)i],
                        112.f + static_cast<float>(i) * 249.f, 292.f, 218.f,
                        61.f, language == static_cast<size_t>(i),
                        names[(size_t)i], 20.f)) {
          language = static_cast<size_t>(i);
          status_message =
              "Language set to " + std::string(i == 0   ? "English"
                                               : i == 1 ? "Spanish"
                                                        : "German");
        }
      }
    } else {
      detail_text(context, board, 102, body, 145.f, 222.f, 650.f, 130.f, 20.f,
                  "ab_detail_body", brown_muted);
    }

    if (!status_message.empty()) {
      detail_text(context, board, 107, status_message, 195.f, 382.f, 550.f,
                  28.f, 16.f, "ab_detail_status", brown_muted);
    }
    if (pill_button(context, board, 108, "BACK", 335.f, 427.f, 270.f, 55.f,
                    false, "ab_detail_back", 22.f)) {
      detail_panel = DetailPanel::None;
      status_message.clear();
    }
  }

  void render_footer(UIContext<InputAction> &context,
                     afterhours::Entity &board) {
    div(context, mk(board, 80),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(817.f * layout_scale),
                                     pixels(2.f * layout_scale)})
            .with_absolute_position(pixels(61.f * layout_scale),
                                    pixels(461.f * layout_scale))
            .with_custom_background(afterhours::Color{168, 112, 73, 210})
            .with_debug_name("ab_footer_rule"));

    if (pill_button(context, board, 81, "SAVE & CLOSE", 704.f, 484.f, 176.f, 55.f, true,
                    "ab_ok", 19.f)) {
      save_settings("Settings saved");
      settings_open = false;
    }
    if (pill_button(context, board, 82, "CANCEL", 382.f, 484.f, 144.f, 55.f,
                    false, "ab_cancel", 23.f)) {
      restore_settings();
    }
    if (pill_button(context, board, 83, "APPLY", 543.f, 484.f, 144.f, 55.f,
                    false, "ab_apply", 23.f, !has_changes())) {
      save_settings("Settings applied");
    }
    action_label(context, board);
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    load_art();
    const float s =
        context.screen_height > 0.f ? context.screen_height / 720.f : 1.f;
    layout_scale = s;
    UIStylingDefaults::get().set_default_font("FredokaMockBold",
                                              pixels(22.f * s));
    Theme theme;
    theme.font = cream;
    theme.darkfont = brown;
    theme.font_muted = brown_muted;
    theme.background = afterhours::Color{12, 43, 28, 255};
    theme.surface = afterhours::Color{247, 216, 176, 255};
    theme.primary = green;
    theme.secondary = blue;
    theme.accent = green;
    theme.error = afterhours::Color{239, 48, 66, 255};
    theme.roundness = 0.75f;
    theme.segments = 20;
    context.set_theme(theme);
    context.scaling_mode = ScalingMode::Adaptive;

    if (context.pressed(InputAction::MenuBack)) {
      if (detail_panel != DetailPanel::None) {
        detail_panel = DetailPanel::None;
        status_message.clear();
      } else if (settings_open) {
        settings_open = false;
      }
    }

    const auto forest_texture = art[Forest];
    auto root =
        vstack(context, mk(entity),
               ComponentConfig{}
                   .with_size(ComponentSize{screen_pct(1.f), screen_pct(1.f)})
                   .with_background(Theme::Usage::None)
                   .with_corner_radius(0.f)
                   .with_align_items(AlignItems::Center)
                   .with_justify_content(JustifyContent::Center)
                   .with_no_wrap()
                   .with_on_draw_bg([forest_texture](RectangleType r) {
                     paint(forest_texture, r);
                   })
                   .with_debug_name("ab_root"));

    div(context, mk(root.ent(), 30), ComponentConfig{}
        .with_size({pixels(220.f * s), pixels(36.f * s)})
        .with_absolute_position(pixels(56.f * s), pixels(12.f * s))
        .with_label("5 / 5 lives").with_font("FredokaMockBold", pixels(27.f * s))
        .with_custom_text_color(cream).with_background(Theme::Usage::None).with_ignore_pointer_events());
    div(context, mk(root.ent(), 31), ComponentConfig{}
        .with_size({pixels(220.f * s), pixels(36.f * s)})
        .with_absolute_position(pixels(1004.f * s), pixels(12.f * s))
        .with_label("1000 coins").with_font("FredokaMockBold", pixels(27.f * s))
        .with_custom_text_color(cream).with_background(Theme::Usage::None).with_ignore_pointer_events());

    if (!settings_open) {
      div(context, mk(root.ent(), 20),
          ComponentConfig{}
              .with_label("SETTINGS CLOSED")
              .with_size(ComponentSize{pixels(440.f * s), pixels(55.f * s)})
              .with_absolute_position(pixels(420.f * s), pixels(255.f * s))
              .with_background(Theme::Usage::None)
              .with_custom_text_color(cream)
              .with_font("FredokaMockBold", pixels(38.f * s))
              .with_alignment(TextAlignment::Center)
              .with_text_stroke(afterhours::Color{61, 71, 54, 255}, 2.f * s)
              .with_debug_name("ab_closed_state"));
      if (pill_button(context, root.ent(), 21, "OPEN SETTINGS", 474.f, 326.f,
                      332.f, 68.f, true, "ab_reopen", 24.f)) {
        settings_open = true;
        status_message.clear();
      }
      return;
    }

    div(context, mk(root.ent(), 10),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(1.f), screen_pct(1.f)})
            .with_absolute_position(pixels(0.f), pixels(0.f))
            .with_custom_background(afterhours::Color{6, 22, 12, 218})
            .with_ignore_pointer_events()
            .with_debug_name("ab_shade"));

    const auto board_texture = art[Board];
    auto board = div(
        context, mk(root.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(940.f * s), pixels(600.f * s)})
            .with_absolute_position(pixels(170.f * s), pixels(60.f * s))
            .with_background(Theme::Usage::None)
            .with_on_draw_bg([board_texture, s](RectangleType r) {
              paint(board_texture, {r.x - s, r.y - s, 942.f * s, 612.f * s});
            })
            .with_debug_name("ab_board"));

    div(context, mk(board.ent(), 1),
        ComponentConfig{}
            .with_label("SETTINGS")
            .with_size(ComponentSize{pixels(450.f * s), pixels(72.f * s)})
            .with_absolute_position(pixels(245.f * s), pixels(18.f * s))
            .with_background(Theme::Usage::None)
            .with_custom_text_color(cream)
            .with_font("FredokaMockBold", pixels(66.f * s))
            .with_alignment(TextAlignment::Center)
            .with_letter_spacing(2.f)
            .with_text_stroke(afterhours::Color{133, 86, 54, 255}, 2.f * s)
            .with_text_shadow(afterhours::Color{166, 103, 59, 255}, 0.f,
                              3.f * s)
            .with_debug_name("ab_title"));

    if (detail_panel == DetailPanel::None) {
      render_tabs(context, board.ent());
      if (active_tab == 0)
        render_audio(context, board.ent());
      else if (active_tab == 1)
        render_general(context, board.ent());
      else
        render_info(context, board.ent());
      render_footer(context, board.ent());
    } else {
      render_detail(context, board.ent());
    }

    const auto close_texture = art[Close];
    if (button(
            context, mk(root.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(60.f * s), pixels(60.f * s)})
                .with_absolute_position(pixels(1016.f * s), pixels(104.f * s))
                .with_overlay(2).with_roundness(1.f)
                .with_background(Theme::Usage::None)
                .with_click_activation(ClickActivationMode::Release)
                .with_on_draw_bg([close_texture](RectangleType r) {
                  paint_plate(close_texture, r, 74.f, 74.f);
                })
                .with_debug_name("ab_close"))) {
      settings_open = false;
      detail_panel = DetailPanel::None;
      status_message.clear();
    }


  }
};

REGISTER_EXAMPLE_SCREEN(angry_birds_settings, "Game Mockups",
                        "Mobile game settings (Angry Birds style)",
                        AngryBirdsSettingsScreen)
