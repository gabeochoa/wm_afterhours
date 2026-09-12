#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>
#include <afterhours/src/plugins/modal.h>
#include <array>
#include <string>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct CasualSettingsScreen : ScreenSystem<UIContext<InputAction>> {
  enum class Dialog { About, Language, Credits, Support, Terms, Progress };
  struct Progress { int level; int coins; };
  bool music_on = true, sound_on = true, vibrate_on = false;
  bool notifications_on = false, settings_open = true, dialog_open = false;
  bool focus_dialog = false;
  Dialog dialog = Dialog::About;
  size_t language = 0, pending_language = 0;
  int help_topic = 0;
  Progress progress{12, 1000}, saved_progress{8, 650};
  std::string status;
  std::array<raylib::Texture2D, 11> art{};
  bool loaded = false;
  static constexpr std::array<const char *, 3> languages{"English", "Spanish", "French"};
  const afterhours::Color cream{255, 248, 230, 255};
  const afterhours::Color brown{137, 85, 62, 255};
  const afterhours::Color muted{163, 104, 77, 255};

  void load() {
    if (loaded) return;
    constexpr std::array<const char *, 11> names{
        "forest", "board", "pill_blue", "music_on", "music_off",
        "sound_on", "sound_off", "vibration_on", "vibration_off", "close", "wifi"};
    for (size_t i = 0; i < names.size(); ++i) {
      art[i] = raylib::LoadTexture(afterhours::files::get_resource_path(
          "images", std::string("mobile_settings/") + names[i] + ".png").string().c_str());
      raylib::SetTextureFilter(art[i], raylib::TEXTURE_FILTER_BILINEAR);
    }
    loaded = true;
  }

  void open(Dialog next) {
    dialog = next;
    dialog_open = true;
    focus_dialog = true;
    pending_language = language;
    help_topic = 0;
    status.clear();
  }

  static void paint(raylib::Texture2D texture, RectangleType r) {
    raylib::DrawTexturePro(texture,
        {0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height)},
        r, {0, 0}, 0, raylib::WHITE);
  }

  ComponentConfig box(float scale, float x, float y, float w, float h) const {
    return ComponentConfig{}.with_size({pixels(w * scale), pixels(h * scale)})
        .with_absolute_position(x * scale, y * scale).with_corner_radius(0)
        .with_background(Theme::Usage::None);
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    load();
    const float scale = context.screen_height / 720.f;
    Theme theme;
    theme.font = brown;
    theme.darkfont = cream;
    theme.font_muted = muted;
    theme.background = {46, 102, 76, 255};
    theme.surface = {250, 222, 178, 255};
    theme.primary = {34, 161, 215, 255};
    theme.accent = {75, 213, 6, 255};
    theme.corner_radius = 18;
    theme.roundness = 0;
    context.set_theme(theme);
    context.scaling_mode = ScalingMode::Proportional;
    UIStylingDefaults::get().set_default_font("FredokaMockBold", h720(23));

    if (!dialog_open && context.pressed(InputAction::MenuBack))
      settings_open = !settings_open;

    auto root = div(context, mk(entity, 0),
        box(scale, 0, 0, 1280, 720).with_debug_name("casual_root")
        .with_on_draw_bg([texture = art[0]](RectangleType r) { paint(texture, r); }));
    auto label = [&](afterhours::Entity &parent, int id, float x, float y,
                     float w, float h, const std::string &text, float size,
                     afterhours::Color color, const std::string &name = "", bool shadow = false) {
      return div(context, mk(parent, id), box(scale, x, y, w, h).with_label(text)
          .with_font("FredokaMockBold", h720(size * 1.25f))
          .with_custom_text_color(color).with_alignment(TextAlignment::Center)
          .with_text_shadow(shadow ? brown : afterhours::Color{0, 0, 0, 0},
                            0, 1.5f * scale)
          .with_text_stroke(shadow ? brown : afterhours::Color{0, 0, 0, 0},
                            shadow ? scale : 0.f)
          .with_ignore_pointer_events().with_debug_name(name));
    };
    auto action = [&](afterhours::Entity &parent, int id, const std::string &text,
                      float x, float y, float w, float h, const std::string &name,
                      float size = 29.f, int texture_index = 2) {
      const auto texture = art[static_cast<size_t>(texture_index)];
      const float base_w = texture_index == 2 ? 400.f : texture_index == 9 ? 74.f : 119.f;
      const float base_h = texture_index == 2 ? 77.f : texture_index == 9 ? 74.f : 82.f;
      const float image_w = texture.width / 2.f, image_h = texture.height / 2.f;
      return button(context, mk(parent, id), box(scale, x, y, w, h)
          .with_label(text).with_font("FredokaMockBold", h720(size * 1.25f))
          .with_custom_text_color(cream).with_alignment(TextAlignment::Center)
          .with_text_inset(4 * scale, 2 * scale).with_corner_radius(35 * scale)
          .with_text_shadow({67, 128, 144, 255}, 0, 1.5f * scale)
          .with_click_activation(ClickActivationMode::Release)
          .with_on_draw_bg([texture, base_w, base_h, image_w, image_h](RectangleType r) {
            const float sx = r.width / base_w, sy = r.height / base_h;
            paint(texture, {r.x - 8 * sx, r.y - 8 * sy, image_w * sx, image_h * sy});
          }).with_debug_name(name));
    };

    label(root.ent(), 1, 825, 37, 230, 53, "Unlimited lives", 24, cream);
    label(root.ent(), 2, 1075, 37, 180, 53,
          std::to_string(progress.coins) + " coins", 24, cream, "casual_coins");
    if (settings_open) {
      div(context, mk(root.ent(), 3), box(scale, 0, 0, 1280, 720)
          .with_custom_background({6, 22, 12, 194}).with_ignore_pointer_events());
      auto board = div(context, mk(root.ent(), 4), box(scale, 170, 46, 940, 627)
          .with_debug_name("casual_board")
          .with_on_draw_bg([texture = art[1], scale](RectangleType r) {
            paint(texture, {r.x - scale, r.y - scale, 942 * scale, 640 * scale});
          }));
      label(board.ent(), 1, 0, 18, 940, 80, "SETTINGS", 57.5f, cream, "casual_title", true);
      const std::array<bool *, 3> values{&music_on, &sound_on, &vibrate_on};
      constexpr std::array<const char *, 3> names{"Music", "Sound", "Vibration"};
      constexpr std::array<const char *, 3> ids{"casual_music", "casual_sound", "casual_vibration"};
      for (size_t i = 0; i < values.size(); ++i) {
        if (action(board.ent(), 10 + static_cast<int>(i), "", 61 + 138.f * i,
                   134, 119, 82, ids[i], 20, 3 + static_cast<int>(i) * 2 + (*values[i] ? 0 : 1))) {
          *values[i] = !*values[i];
          status = std::string(names[i]) + (*values[i] ? " on" : " off");
        }
      }
      if (action(board.ent(), 20, "", 484, 137, 396, 77,
                 "casual_progress", 29)) open(Dialog::Progress);
      label(board.ent(), 40, 500, 138, 280, 40, "SAVE/LOAD", 29, cream);
      label(board.ent(), 42, 500, 168, 280, 40, "PROGRESS", 29, cream);
      div(context, mk(board.ent(), 41), box(scale, 742, 142, 67, 75)
          .with_ignore_pointer_events().with_on_draw_bg([texture = art[10]](RectangleType r) { paint(texture, r); }));
      if (action(board.ent(), 21, notifications_on ? "NOTIFICATIONS: ON" : "NOTIFICATIONS: OFF",
                 61, 243, 396, 77, "casual_notifications", 24)) {
        notifications_on = !notifications_on;
        status = notifications_on ? "Notifications enabled" : "Notifications disabled";
      }
      if (action(board.ent(), 22, "CREDITS", 484, 243, 396, 77, "casual_credits")) open(Dialog::Credits);
      if (action(board.ent(), 23, "LANGUAGE", 61, 346, 396, 77, "casual_language")) open(Dialog::Language);
      if (action(board.ent(), 24, "SUPPORT", 484, 346, 396, 77, "casual_support")) open(Dialog::Support);
      if (action(board.ent(), 25, "TERMS AND PRIVACY", 484, 449, 396, 77,
                 "casual_terms", 27)) open(Dialog::Terms);
      if (button(context, mk(board.ent(), 26), box(scale, 61, 443, 396, 94)
          .with_click_activation(ClickActivationMode::Release)
          .with_debug_name("casual_about"))) open(Dialog::About);
      label(board.ent(), 43, 61, 446, 396, 25, "15555-1-114203-20-10200-01", 16.5f, muted);
      label(board.ent(), 44, 61, 471, 396, 25, "Version 1.11.0.12346", 16.5f, muted);
      label(board.ent(), 45, 61, 496, 396, 25, "Player ID: 281676956389", 16.5f, muted);
      if (!status.empty()) label(board.ent(), 30, 70, 566, 800, 30, status, 17, brown, "casual_status");
      if (action(root.ent(), 50, "", 1047, 106, 74, 74, "casual_close", 20, 9))
        settings_open = false;
      div(context, mk(root.ent(), 51), box(scale, 210, 698, 860, 4)
          .with_custom_background({255, 255, 255, 160}).with_ignore_pointer_events());
    } else {
      label(root.ent(), 60, 340, 230, 600, 70, "Ready for another adventure?", 36, cream);
      label(root.ent(), 61, 340, 315, 600, 50,
            "Level " + std::to_string(progress.level) + "  /  " + std::to_string(progress.coins) + " coins",
            28, cream, "casual_progress_summary");
      if (action(root.ent(), 62, "SETTINGS", 440, 414, 400, 77, "casual_reopen"))
        settings_open = true;
    }

    constexpr std::array<const char *, 6> titles{
        "About", "Language", "Credits", "Support", "Terms and privacy", "Save / load progress"};
    auto modal = afterhours::modal(context, mk(entity, 500), dialog_open,
        afterhours::ModalConfig{}.with_size(h720(650), h720(440))
            .with_show_close_button(false).with_closed_by(afterhours::ClosedBy::CloseRequest));
    if (!modal) return;
    modal.cmp().set_desired_padding(pixels(0), Axis::X)
        .set_desired_padding(pixels(0), Axis::Y);
    auto &panel = modal.ent();
    label(panel, 10, 25, 17, 600, 55, titles[static_cast<size_t>(dialog)], 30, brown, "casual_dialog_title");
    auto body = [&](int id, const std::string &text, float y, float height) {
      div(context, mk(panel, id), box(scale, 34, y, 582, height).with_label(text)
          .with_font("Fredoka", h720(23)).with_custom_text_color(brown)
          .with_text_overflow(TextOverflow::Wrap).with_alignment(TextAlignment::Center)
          .with_ignore_pointer_events().with_debug_name("casual_dialog_body_" + std::to_string(id)));
    };
    auto modal_action = [&](int id, const std::string &text, float x, float y,
                            float w, const std::string &name) {
      auto result = action(panel, id, text, x, y, w, 52, name, 20);
      if (focus_dialog) { context.set_focus(result.ent().id); focus_dialog = false; }
      return static_cast<bool>(result);
    };
    if (dialog == Dialog::Language) {
      body(11, "Choose your preferred language.", 81, 40);
      for (size_t i = 0; i < languages.size(); ++i)
        if (modal_action(20 + static_cast<int>(i), std::string(pending_language == i ? "> " : "") + languages[i],
                         35 + i * 198.f, 144, 185, "casual_language_" + std::to_string(i)))
          pending_language = i;
      body(12, std::string("Selected: ") + languages[pending_language], 218, 40);
      if (modal_action(30, "APPLY", 335, 348, 280, "casual_language_apply")) {
        language = pending_language;
        status = std::string("Language: ") + languages[language];
        dialog_open = false;
      }
      if (modal_action(31, "CANCEL", 35, 348, 280, "casual_dialog_close")) dialog_open = false;
    } else if (dialog == Dialog::Progress) {
      body(11, "Current progress: level " + std::to_string(progress.level) + " / " +
                   std::to_string(progress.coins) + " coins\nSaved progress: level " +
                   std::to_string(saved_progress.level) + " / " + std::to_string(saved_progress.coins) + " coins",
           83, 100);
      if (modal_action(20, "SAVE PROGRESS", 35, 204, 280, "casual_save")) {
        saved_progress = progress;
        status = "Progress saved: level " + std::to_string(progress.level);
      }
      if (modal_action(21, "LOAD PROGRESS", 335, 204, 280, "casual_load")) {
        progress = saved_progress;
        status = "Progress loaded: level " + std::to_string(progress.level);
      }
      body(12, status, 273, 36);
      if (modal_action(31, "DONE", 185, 348, 280, "casual_dialog_close")) dialog_open = false;
    } else if (dialog == Dialog::Support) {
      constexpr std::array<const char *, 3> help{
          "How can we help? Choose a topic below.",
          "Audio: open Settings and choose the music or speaker button. Green means on; gray means off.",
          "Progress: Save Progress stores your current level and coins. Load Progress restores the last saved slot."};
      body(11, help[static_cast<size_t>(help_topic)], 83, 132);
      if (modal_action(20, "AUDIO HELP", 35, 234, 280, "casual_help_audio")) help_topic = 1;
      if (modal_action(21, "SAVE HELP", 335, 234, 280, "casual_help_save")) help_topic = 2;
      if (modal_action(31, "CLOSE", 185, 348, 280, "casual_dialog_close")) dialog_open = false;
    } else {
      const std::string content = dialog == Dialog::About
          ? "Build: 15555-1-114203-20-10200-01\nVersion: 1.11.0.12346\nPlayer ID: 281676956389\nLanguage: " + std::string(languages[language])
          : dialog == Dialog::Credits
          ? "Made with afterhours\nInterface and artwork: wm_afterhours\nFredoka typeface: The Fredoka Project Authors\nThank you for playing!"
          : "Your privacy\nYour preferences and progress stay on this device. No account, advertising or cloud sync is used.\nUse Save / Load Progress to manage your saved game.";
      body(11, content, 86, 225);
      if (modal_action(31, "CLOSE", 185, 348, 280, "casual_dialog_close")) dialog_open = false;
    }
  }
};

REGISTER_EXAMPLE_SCREEN(casual_settings, "Game Mockups",
                        "Casual mobile settings with local progress and preferences",
                        CasualSettingsScreen)
