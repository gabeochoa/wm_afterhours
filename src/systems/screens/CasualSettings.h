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
  std::array<raylib::Texture2D, 10> art{};
  bool loaded = false;
  static constexpr std::array<const char *, 3> languages{"English", "Spanish", "French"};
  const afterhours::Color cream{255, 248, 230, 255};
  const afterhours::Color brown{137, 85, 62, 255};
  const afterhours::Color muted{163, 104, 77, 255};

  void load() {
    if (loaded) return;
    constexpr std::array<const char *, 10> names{
        "forest", "board", "pill_blue", "music_on", "music_off",
        "sound_on", "sound_off", "vibration_on", "vibration_off", "close"};
    for (size_t i = 0; i < names.size(); ++i) {
      art[i] = raylib::LoadTexture(afterhours::files::get_resource_path(
          "images", std::string(i >= 2 && i <= 8 ? "casual_settings/" : "mobile_settings/") + names[i] + ".png").string().c_str());
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
    const float scale = std::min(context.screen_height / 720.f, context.screen_width / 1280.f);
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
    UIStylingDefaults::get().set_default_font("FredokaMockBold", pixels(23 * scale));

    if (!dialog_open && context.pressed(InputAction::MenuBack))
      settings_open = !settings_open;

    auto canvas = div(context, mk(entity, 0), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_corner_radius(0).with_debug_name("casual_canvas")
        .with_on_draw_bg([texture = art[0]](RectangleType r) { paint(texture, r); }));
    auto root = div(context, mk(canvas.ent(), 0), box(scale, 0, 0, 1280, 720)
        .with_absolute_position((context.screen_width - 1280 * scale) / 2,
                                (context.screen_height - 720 * scale) / 2)
        .with_debug_name("casual_root"));
    auto label = [&](afterhours::Entity &parent, int id, float x, float y,
                     float w, float h, const std::string &text, float size,
                     afterhours::Color color, const std::string &name = "", bool shadow = false) {
      return div(context, mk(parent, id), box(scale, x, y, w, h).with_label(text)
          .with_font("FredokaMockBold", pixels(size * 1.25f * scale))
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
          .with_label(text).with_font("FredokaMockBold", pixels(size * 1.25f * scale))
          .with_custom_text_color(cream).with_alignment(TextAlignment::Center)
          .with_text_inset(4 * scale, 2 * scale).with_corner_radius(35 * scale)
          .with_text_shadow({67, 128, 144, 255}, 0, 1.5f * scale)
          .with_click_activation(ClickActivationMode::Release)
          .with_on_draw_bg([texture, base_w, base_h, image_w, image_h](RectangleType r) {
            const float sx = r.width / base_w, sy = r.height / base_h;
            paint(texture, {r.x - 8 * sx, r.y - 8 * sy, image_w * sx, image_h * sy});
          }).with_debug_name(name));
    };

    div(context, mk(root.ent(), 1), box(scale, 1068, 28, 184, 52)
        .with_custom_background({255, 240, 207, 255}).with_corner_radius(26 * scale)
        .with_border({217, 176, 101, 255}, 3 * scale).with_ignore_pointer_events()
        .with_on_draw_fg([scale](RectangleType r) {
          const raylib::Vector2 center{r.x + 27 * scale, r.y + r.height / 2};
          raylib::DrawCircleV(center, 14 * scale, {244, 187, 50, 255});
          raylib::DrawCircleLinesV(center, 11 * scale, {165, 108, 31, 255});
          raylib::DrawLineEx({center.x, center.y - 7 * scale}, {center.x, center.y + 7 * scale},
                             3 * scale, {255, 231, 132, 255});
        }));
    label(root.ent(), 2, 1115, 31, 130, 44, std::to_string(progress.coins) + " +", 24, brown, "casual_coins");
    if (settings_open) {
      div(context, mk(root.ent(), 3), box(scale, 0, 0, 1280, 720)
          .with_custom_background({6, 22, 12, 194}).with_ignore_pointer_events());
      auto board = div(context, mk(root.ent(), 4), box(scale, 170, 46, 940, 627)
          .with_debug_name("casual_board")
          .with_on_draw_bg([texture = art[1], scale](RectangleType r) {
            paint(texture, {r.x - scale, r.y - scale, 942 * scale, 640 * scale});
          }));
      label(board.ent(), 1, 0, 18, 940, 80, "SETTINGS", 57.5f, cream, "casual_title", true);
      label(board.ent(), 2, 61, 106, 396, 25, "AUDIO", 18, brown);
      const std::array<bool *, 3> values{&music_on, &sound_on, &vibrate_on};
      constexpr std::array<const char *, 3> names{"Music", "Sound", "Vibration"};
      constexpr std::array<const char *, 3> captions{"MUSIC", "SOUND EFFECTS", "VIBRATION"};
      constexpr std::array<const char *, 3> ids{"casual_music", "casual_sound", "casual_vibration"};
      for (size_t i = 0; i < values.size(); ++i) {
        const float x = 74 + 132.f * i;
        if (action(board.ent(), 10 + static_cast<int>(i), "", x, 139, 102, 70, ids[i],
                   20, 3 + static_cast<int>(i) * 2 + (*values[i] ? 0 : 1))) {
          *values[i] = !*values[i];
          status = std::string(names[i]) + (*values[i] ? " on" : " off");
        }
        label(board.ent(), 60 + static_cast<int>(i), x - 27, 216, 156, 25, captions[i], 14.5f, brown);
        label(board.ent(), 65 + static_cast<int>(i), x - 13, 240, 128, 22,
              *values[i] ? "ON" : "OFF", 16, brown, std::string(ids[i]) + "_state");
      }
      if (action(board.ent(), 20, "", 484, 137, 396, 77, "casual_progress", 26)) open(Dialog::Progress);
      label(board.ent(), 40, 494, 143, 306, 35, "PROGRESS SAVE", 26, cream);
      label(board.ent(), 42, 494, 178, 306, 26, "Save or restore this session", 15.5f, cream);
      div(context, mk(board.ent(), 41), box(scale, 812, 153, 44, 44)
          .with_ignore_pointer_events().with_on_draw_bg([scale](RectangleType r) {
            afterhours::draw_rectangle_rounded(r, .12f, 8, {255, 248, 230, 255});
            afterhours::draw_rectangle({r.x + 10 * scale, r.y, 24 * scale, 17 * scale}, {34, 161, 215, 255});
            afterhours::draw_rectangle({r.x + 14 * scale, r.y + 3 * scale, 6 * scale, 11 * scale}, {255, 248, 230, 255});
            afterhours::draw_rectangle({r.x + 9 * scale, r.y + 25 * scale, 26 * scale, 19 * scale}, {34, 161, 215, 255});
          }));
      label(board.ent(), 43, 484, 220, 396, 28,
            "Saved slot: level " + std::to_string(saved_progress.level) + " / " + std::to_string(saved_progress.coins) + " coins",
            17, brown, "casual_saved_slot");
      if (action(board.ent(), 21, "", 61, 286, 396, 66, "casual_notifications", 24)) {
        notifications_on = !notifications_on;
        status = notifications_on ? "Notifications enabled" : "Notifications disabled";
      }
      label(board.ent(), 44, 72, 290, 280, 32, "NOTIFICATIONS", 23, cream);
      label(board.ent(), 45, 72, 320, 280, 23, notifications_on ? "ON" : "OFF", 16, cream, "casual_notifications_state");
      div(context, mk(board.ent(), 46), box(scale, 367, 305, 64, 30)
          .with_custom_background(notifications_on ? afterhours::Color{91, 176, 45, 255} : afterhours::Color{35, 112, 145, 255})
          .with_corner_radius(15 * scale).with_border({255, 248, 230, 220}, scale).with_ignore_pointer_events()
          .with_on_draw_fg([scale, enabled = notifications_on](RectangleType r) {
            raylib::DrawCircleV({r.x + (enabled ? 49 : 15) * scale, r.y + r.height / 2}, 11 * scale,
                                {255, 248, 230, 255});
          }));
      if (action(board.ent(), 23, "", 61, 369, 396, 66, "casual_language")) open(Dialog::Language);
      label(board.ent(), 47, 61, 371, 396, 32, "LANGUAGE", 24, cream);
      label(board.ent(), 48, 61, 403, 396, 24, languages[language], 18, cream, "casual_language_value");
      label(board.ent(), 49, 484, 267, 396, 27, "HELP & ABOUT", 18, brown);
      if (action(board.ent(), 22, "CREDITS", 484, 305, 190, 57, "casual_credits", 24)) open(Dialog::Credits);
      if (action(board.ent(), 24, "SUPPORT", 690, 305, 190, 57, "casual_support", 24)) open(Dialog::Support);
      if (button(context, mk(board.ent(), 25), box(scale, 484, 387, 396, 44)
          .with_label("TERMS & PRIVACY").with_font("FredokaMockBold", pixels(25 * scale))
          .with_custom_text_color(brown).with_alignment(TextAlignment::Center)
          .with_custom_hover_bg({255, 237, 199, 255}).with_corner_radius(10 * scale)
          .with_click_activation(ClickActivationMode::Release).with_debug_name("casual_terms"))) open(Dialog::Terms);
      div(context, mk(board.ent(), 50), box(scale, 569, 425, 226, 1)
          .with_custom_background({137, 85, 62, 120}).with_ignore_pointer_events());
      if (button(context, mk(board.ent(), 26), box(scale, 61, 453, 819, 126)
          .with_custom_hover_bg({255, 237, 199, 70}).with_corner_radius(12 * scale)
          .with_click_activation(ClickActivationMode::Release).with_debug_name("casual_about"))) open(Dialog::About);
      div(context, mk(board.ent(), 51), box(scale, 77, 458, 750, 25).with_label("ABOUT  /  OPEN DETAILS")
          .with_font("FredokaMockBold", pixels(18 * scale)).with_custom_text_color(brown)
          .with_alignment(TextAlignment::Left).with_ignore_pointer_events());
      constexpr std::array<const char *, 3> metadata_labels{"Build", "Version", "Player ID"};
      constexpr std::array<const char *, 3> metadata_values{"15555-1-114203-20-10200-01", "1.11.0.12346", "281 676 956 389"};
      for (int i = 0; i < 3; ++i) {
        div(context, mk(board.ent(), 70 + i), box(scale, 77, 485 + i * 30.f, 104, 30)
            .with_label(metadata_labels[i]).with_font("FredokaMockBold", pixels(19 * scale))
            .with_custom_text_color(brown).with_alignment(TextAlignment::Left).with_ignore_pointer_events());
        div(context, mk(board.ent(), 75 + i), box(scale, 182, 485 + i * 30.f, 646, 30)
            .with_label(metadata_values[i]).with_font("FredokaMockBold", pixels(27 * scale))
            .with_custom_text_color(brown).with_alignment(TextAlignment::Left).with_ignore_pointer_events()
            .with_debug_name("casual_metadata_" + std::to_string(i)));
      }
      label(board.ent(), 30, 70, 580, 800, 26,
            status.empty() ? "Preferences apply immediately. Kept for this session." : status,
            16.5f, brown, "casual_status");
      if (action(root.ent(), 50, "", 1022, 104, 62, 62, "casual_close", 20, 9)) settings_open = false;
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
        afterhours::ModalConfig{}.with_size(pixels(650 * scale), pixels(440 * scale))
            .with_show_close_button(false).with_closed_by(afterhours::ClosedBy::CloseRequest));
    if (!modal) return;
    modal.cmp().set_desired_padding(pixels(0), Axis::X)
        .set_desired_padding(pixels(0), Axis::Y);
    auto &panel = modal.ent();
    label(panel, 10, 25, 17, 600, 55, titles[static_cast<size_t>(dialog)], 30, brown, "casual_dialog_title");
    auto body = [&](int id, const std::string &text, float y, float height) {
      div(context, mk(panel, id), box(scale, 34, y, 582, height).with_label(text)
          .with_font("Fredoka", pixels(23 * scale)).with_custom_text_color(brown)
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
          "Audio: open Settings and choose the music or speaker button. Green means on; blue means off. The labels show the current state.",
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
          : "Your privacy\nYour preferences and progress last for this demo session. No account, advertising or cloud sync is used.\nUse Save / Load Progress to manage your saved game.";
      body(11, content, 86, 225);
      if (modal_action(31, "CLOSE", 185, 348, 280, "casual_dialog_close")) dialog_open = false;
    }
  }
};

REGISTER_EXAMPLE_SCREEN(casual_settings, "Game Mockups",
                        "Casual mobile settings with local progress and preferences",
                        CasualSettingsScreen)
