#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ParcelCorpsSettingsScreen : ScreenSystem<UIContext<InputAction>> {
  int language_idx = 0;
  bool subtitles = true;
  bool fullscreen = true;
  bool motion_blur = true;
  bool vsync = true;
  float audio_volume = 0.85f;
  float music_volume = 0.75f;

  // Colors matching Parcel Corps - dark phone UI with green toggles
  afterhours::Color bg_dark{18, 22, 28, 255};     // Very dark background
  afterhours::Color phone_black{12, 14, 18, 255}; // Phone frame black
  afterhours::Color row_dark{35, 40, 50,
                             180}; // Row background (semi-transparent)
  afterhours::Color row_separator{55, 60, 70, 255};   // Row separator
  afterhours::Color text_white{245, 248, 250, 255};   // White text
  afterhours::Color text_muted{145, 155, 165, 255};   // Muted text
  afterhours::Color toggle_green{75, 195, 95, 255};   // iOS-style green toggle
  afterhours::Color toggle_track{85, 90, 100, 255};   // Toggle track (off)
  afterhours::Color slider_green{95, 185, 85, 255};   // Volume slider green
  afterhours::Color slider_orange{235, 155, 65, 255}; // Music slider orange
  afterhours::Color slider_track{55, 60, 70, 255};    // Slider track background
  afterhours::Color icon_purple{145, 95, 185, 255};   // Purple icon (language)
  afterhours::Color icon_blue{65, 145, 215, 255};     // Blue icon (MSAA)
  afterhours::Color icon_red{215, 85, 85,
                             255}; // Red icon (motion blur, volume)
  afterhours::Color icon_green{85, 175, 125,
                               255}; // Green icon (subtitles, texture)
  afterhours::Color icon_rainbow1{255, 120, 120,
                                  255}; // Rainbow icon color 1 (resolution)
  afterhours::Color icon_rainbow2{120, 255, 120, 255};  // Rainbow icon color 2
  afterhours::Color icon_rainbow3{120, 120, 255, 255};  // Rainbow icon color 3
  afterhours::Color notch_gray{45, 50, 60, 255};        // Phone notch
  afterhours::Color quest_blue{65, 155, 220, 255};      // Quest panel blue
  afterhours::Color quest_blue_dark{45, 120, 180, 255}; // Quest panel border

  std::vector<std::string> languages = {"English UK", "English US", "Deutsch",
                                        "Francais"};

  // MSAA options and current selection
  std::vector<std::string> msaa_options = {"Off", "2x", "4x", "8x"};
  size_t msaa_idx = 1; // Default: 2x

  // Texture Quality options and current selection
  std::vector<std::string> texture_options = {"Low", "Medium", "High", "Ultra"};
  size_t texture_idx = 2; // Default: High

  // Chat messages
  std::vector<std::pair<std::string, std::string>> chat_messages = {
      {"reveredsoup", "you did it :)"},
      {"untrackedjackstay", "Cool :)"},
      {"NimbleGrimble", "YES :)"},
      {"untrackedjackstay", "ZZZzzzz"},
      {"sniprit09", "Zzzz"},
  };

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    Theme theme;
    theme.font = text_white;
    theme.darkfont = phone_black;
    theme.font_muted = text_muted;
    theme.background = bg_dark;
    theme.surface = row_dark;
    theme.primary = toggle_green;
    theme.secondary = text_muted;
    theme.accent = toggle_green;
    theme.error = icon_red;
    theme.roundness = 0.10f;
    theme.segments = 10;
    context.theme = theme;
    UIStylingDefaults::get().set_default_font("EqProRounded", h720(16.0f));

    auto pxf = [](float v) { return pixels(static_cast<int>(v)); };

    int screen_w = Settings::get().get_screen_width();
    int screen_h = Settings::get().get_screen_height();

    // ========== BACKGROUND (game scene visible) ==========
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_w), pixels(screen_h)})
            .with_custom_background(bg_dark)
            .with_debug_name("bg"));

    // ========== PHONE FRAME ==========
    // Expanded settings panel width
    float phone_w = 380.0f;
    float phone_h = 680.0f;
    float phone_x = 50.0f;
    float phone_y = ((float)screen_h - phone_h) / 2.0f;

    // Phone outer frame
    div(context, mk(entity, 10),
        ComponentConfig{}
            .with_720p_size(phone_w, phone_h)
            .with_absolute_position(phone_x, phone_y)
            .with_custom_background(phone_black)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.12f)
            .with_soft_shadow(6.0f, 8.0f, 25.0f,
                              afterhours::Color{0, 0, 0, 120})
            .with_debug_name("phone_frame"));

    // Phone screen area
    float screen_margin = 8.0f;
    float screen_x = phone_x + screen_margin;
    float screen_y = phone_y + screen_margin;
    float screen_inner_w = phone_w - screen_margin * 2;
    float screen_inner_h = phone_h - screen_margin * 2;

    div(context, mk(entity, 11),
        ComponentConfig{}
            .with_720p_size(screen_inner_w, screen_inner_h)
            .with_absolute_position(screen_x, screen_y)
            .with_custom_background(afterhours::Color{22, 26, 32, 255})
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.08f)
            .with_debug_name("phone_screen"));

    // ========== STATUS BAR ==========
    float status_y = screen_y + 12.0f;

    // Time - increased size for better visibility (game time label for clarity)
    div(context, mk(entity, 19),
        ComponentConfig{}
            .with_label("Game Time:")
            .with_size(ComponentSize{pixels(80), pixels(18)})
            .with_absolute_position(screen_x + 15.0f, status_y - 2.0f)
            .with_font_size(h720(14.0f))
            .with_custom_text_color(text_muted));

    div(context, mk(entity, 20),
        ComponentConfig{}
            .with_label("23:45")
            .with_size(ComponentSize{pixels(70), pixels(26)})
            .with_absolute_position(screen_x + 15.0f, status_y + 12.0f)
            .with_font_size(h720(20.0f))
            .with_custom_text_color(text_white));

    // Signal bars
    div(context, mk(entity, 21),
        ComponentConfig{}
            .with_label("||||")
            .with_size(ComponentSize{pixels(40), pixels(22)})
            .with_absolute_position(screen_x + screen_inner_w - 95.0f, status_y)
            .with_font_size(h720(14.0f))
            .with_custom_text_color(text_white));

    // Battery
    div(context, mk(entity, 22),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(32), pixels(14)})
            .with_absolute_position(screen_x + screen_inner_w - 48.0f,
                                    status_y + 4.0f)
            .with_custom_background(toggle_green)
            .with_border(text_white, 1.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.3f)
            .with_debug_name("battery"));

    // ========== SETTINGS TITLE ==========
    div(context, mk(entity, 30),
        ComponentConfig{}
            .with_label("Settings")
            .with_size(ComponentSize{pixels(170), pixels(44)})
            .with_absolute_position(screen_x + 15.0f, status_y + 35.0f)
            .with_font_size(h720(32.0f))
            .with_custom_text_color(text_white));

    // ========== SETTINGS ROWS ==========
    float row_x = screen_x + 8.0f;
    float row_start_y = status_y + 85.0f;
    float row_w = screen_inner_w - 16.0f;
    float row_h = 44.0f; // Minimum 44px for touch targets
    float row_gap = 2.0f;

    // Language selector row (purple globe icon)
    render_language_row(context, entity, 100, row_x, row_start_y, row_w, row_h);

    // Subtitles toggle (green speech bubble icon)
    render_toggle_row_with_icon(context, entity, 110, row_x,
                                row_start_y + row_h + row_gap, row_w, row_h,
                                "Subtitles", subtitles, icon_green, "...");

    // Separator
    div(context, mk(entity, 115),
        ComponentConfig{}
            .with_size(ComponentSize{pxf(row_w), pixels(1)})
            .with_absolute_position(row_x, row_start_y + 2 * (row_h + row_gap))
            .with_custom_background(row_separator)
            .with_debug_name("separator1"));

    float section2_y = row_start_y + 2 * (row_h + row_gap) + 6.0f;

    // Resolution (rainbow/multicolor icon)
    render_display_row(context, entity, 120, row_x, section2_y, row_w, row_h,
                       "Resolution", "2560 x 1440", icon_rainbow1, true);

    // Full Screen toggle (rainbow icon)
    render_toggle_row_with_icon(
        context, entity, 130, row_x, section2_y + row_h + row_gap, row_w, row_h,
        "Full Screen", fullscreen, icon_rainbow1, "", true);

    // MSAA (blue icon) - interactive
    render_selector_row(context, entity, 140, row_x,
                        section2_y + 2 * (row_h + row_gap), row_w, row_h,
                        "MSAA", msaa_options, msaa_idx, icon_blue);

    // Texture Quality (green gem icon) - interactive
    render_selector_row(
        context, entity, 150, row_x, section2_y + 3 * (row_h + row_gap), row_w,
        row_h, "Texture Quality", texture_options, texture_idx, icon_green);

    // Motion Blur toggle (red/pink X icon)
    render_toggle_row_with_icon(
        context, entity, 160, row_x, section2_y + 4 * (row_h + row_gap), row_w,
        row_h, "Motion Blur", motion_blur, icon_red, "X");

    // VSync toggle (purple icon)
    render_toggle_row_with_icon(context, entity, 170, row_x,
                                section2_y + 5 * (row_h + row_gap), row_w,
                                row_h, "VSync", vsync, icon_purple, "~");

    // ========== VOLUME SLIDERS ==========
    float slider_y = section2_y + 6 * (row_h + row_gap) + 8.0f;

    render_volume_slider(context, entity, 200, row_x, slider_y, row_w, row_h,
                         "Audio Volume", audio_volume, icon_red);

    render_volume_slider(context, entity, 210, row_x,
                         slider_y + row_h + row_gap, row_w, row_h,
                         "Music Volume", music_volume, icon_red);

    // ========== ICON LEGEND (explains icon meanings) ==========
    float legend_y = slider_y + 2 * (row_h + row_gap) + 6.0f;

    // Legend title
    div(context, mk(entity, 220),
        ComponentConfig{}
            .with_label("Icon Legend:")
            .with_size(ComponentSize{pixels(110), pixels(20)})
            .with_absolute_position(row_x + 8.0f, legend_y)
            .with_font_size(h720(14.0f))
            .with_custom_text_color(text_muted));

    // Legend items - data-driven loop
    float legend_item_y = legend_y + 22.0f;
    float legend_col_w = (row_w - 16.0f) / 2.0f;

    struct LegendItem {
      const char *icon;
      const char *label;
      afterhours::Color color;
    };
    LegendItem legend_items[] = {
        {"@", "Language", icon_purple}, {"~", "Sync", icon_purple},
        {"X", "Effects", icon_red},     {"*", "Quality", icon_blue},
        {"...", "Text", icon_green},    {"", "Display", icon_rainbow1},
    };

    for (int i = 0; i < 6; i++) {
      int col = i % 2, row = i / 2;
      float lx = row_x + (float)col * legend_col_w + 8.0f;
      float ly = legend_item_y + (float)row * 22.0f;
      int base = 221 + i * 2;

      // Icon circle
      auto icon_cfg = ComponentConfig{}
                          .with_size(ComponentSize{pixels(18), pixels(18)})
                          .with_absolute_position(lx, ly)
                          .with_custom_background(legend_items[i].color)
                          .with_rounded_corners(RoundedCorners())
                          .with_roundness(1.0f);
      if (legend_items[i].icon[0] != '\0') {
        icon_cfg.with_label(legend_items[i].icon)
            .with_font_size(h720(14.0f))
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center);
      }
      div(context, mk(entity, base), icon_cfg);

      // Label text
      div(context, mk(entity, base + 1),
          ComponentConfig{}
              .with_label(legend_items[i].label)
              .with_size(ComponentSize{pixels(80), pixels(18)})
              .with_absolute_position(lx + 22.0f, ly)
              .with_font_size(h720(14.0f))
              .with_custom_text_color(text_muted));
    }

    // ========== SEPARATOR BEFORE FOOTER ==========
    float footer_sep_y = legend_item_y + 3 * 22.0f + 8.0f;
    div(context, mk(entity, 240),
        ComponentConfig{}
            .with_size(ComponentSize{pixels((int)(row_w - 16.0f)), pixels(1)})
            .with_absolute_position(row_x + 8.0f, footer_sep_y)
            .with_custom_background(afterhours::Color{255, 255, 255, 40})
            .with_debug_name("section_separator_footer"));

    // ========== FOOTER: OK / Cancel / Apply ==========
    float phone_footer_y = footer_sep_y + 8.0f;
    float phone_footer_x = row_x + row_w - 230.0f;

    button(context, mk(entity, 241),
           ComponentConfig{}
               .with_label("OK")
               .with_size(ComponentSize{pixels(65), pixels(28)})
               .with_absolute_position(phone_footer_x, phone_footer_y)
               .with_custom_background(slider_orange)
               .with_font_size(h720(14.0f))
               .with_custom_text_color(text_white)
               .with_alignment(TextAlignment::Center)
               .with_rounded_corners(RoundedCorners())
               .with_roundness(0.3f)
               .with_debug_name("btn_ok"));

    button(context, mk(entity, 242),
           ComponentConfig{}
               .with_label("Cancel")
               .with_size(ComponentSize{pixels(65), pixels(28)})
               .with_absolute_position(phone_footer_x + 72.0f, phone_footer_y)
               .with_custom_background(afterhours::Color{60, 65, 75, 255})
               .with_font_size(h720(14.0f))
               .with_custom_text_color(text_white)
               .with_alignment(TextAlignment::Center)
               .with_rounded_corners(RoundedCorners())
               .with_roundness(0.3f)
               .with_debug_name("btn_cancel"));

    button(context, mk(entity, 243),
           ComponentConfig{}
               .with_label("Apply")
               .with_size(ComponentSize{pixels(65), pixels(28)})
               .with_absolute_position(phone_footer_x + 144.0f, phone_footer_y)
               .with_custom_background(afterhours::Color{60, 65, 75, 255})
               .with_font_size(h720(14.0f))
               .with_custom_text_color(text_white)
               .with_alignment(TextAlignment::Center)
               .with_rounded_corners(RoundedCorners())
               .with_roundness(0.3f)
               .with_debug_name("btn_apply"));

    // ========== QUEST PANEL (moved to right side to avoid overlapping phone)
    // ==========
    float quest_x = (float)screen_w - 370.0f;
    float quest_y = 180.0f;
    float quest_w = 340.0f;
    float quest_h = 85.0f;

    // Quest panel background
    div(context, mk(entity, 250),
        ComponentConfig{}
            .with_720p_size(quest_w, quest_h)
            .with_absolute_position(quest_x, quest_y)
            .with_custom_background(quest_blue)
            .with_border(quest_blue_dark, 3.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.15f)
            .with_debug_name("quest_panel"));

    // Quest items
    struct QuestItem {
      int check_id;
      const char *check;
      float y_off;
      int text_id;
      const char *text;
      float text_x_off;
      int text_w;
    };
    QuestItem quests[] = {
        {251, "[X]", 14.0f, 252, "SIGN UP A BUSINESS TO YOUR APP",
         40.0f, 280},
        {253, "[ ]", 48.0f, 255, "SIGN UP 4 BUSINESSES TO YOUR APP",
         78.0f, 240},
    };
    for (auto &q : quests) {
      div(context, mk(entity, q.check_id),
          ComponentConfig{}
              .with_label(q.check)
              .with_size(ComponentSize{pixels(24), pixels(24)})
              .with_absolute_position(quest_x + 12.0f, quest_y + q.y_off)
              .with_font_size(h720(14.0f))
              .with_custom_text_color(text_white)
              .with_alignment(TextAlignment::Center));
      div(context, mk(entity, q.text_id),
          ComponentConfig{}
              .with_label(q.text)
              .with_size(ComponentSize{pixels(q.text_w), pixels(22)})
              .with_absolute_position(quest_x + q.text_x_off, quest_y + q.y_off)
              .with_font_size(h720(14.0f))
              .with_custom_text_color(text_white));
    }

    // Level badge for quest 2
    div(context, mk(entity, 254),
        ComponentConfig{}
            .with_label("Lv0")
            .with_size(ComponentSize{pixels(32), pixels(20)})
            .with_absolute_position(quest_x + 40.0f, quest_y + 50.0f)
            .with_custom_background(slider_orange)
            .with_font_size(h720(14.0f))
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.3f));

    // ========== HUD ELEMENTS (right side) ==========
    struct HudElement {
      int id;
      const char *label;
      int w;
      int h;
      float x_off;
      float y;
      float font;
      afterhours::Color text_color;
      bool has_bg;
      afterhours::Color bg;
      bool rounded;
    };
    float sw = (float)screen_w;
    HudElement hud_elems[] = {
        {300, "LIVE", 55, 28, -300.0f, 25.0f, 14.0f, text_white, true, icon_red,
         true},
        {301, "O", 22, 22, -235.0f, 28.0f, 16.0f, text_white, false, {}, false},
        {302, "8", 20, 22, -212.0f, 28.0f, 16.0f, text_white, false, {}, false},
        {303,
         "<>",
         22,
         22,
         -185.0f,
         28.0f,
         14.0f,
         text_white,
         false,
         {},
         false},
        {304,
         "10",
         25,
         22,
         -162.0f,
         28.0f,
         16.0f,
         text_white,
         false,
         {},
         false},
        {305,
         "EDDCOATES",
         100,
         28,
         -120.0f,
         25.0f,
         14.0f,
         text_white,
         false,
         {},
         false},
    };
    for (auto &he : hud_elems) {
      auto cfg = ComponentConfig{}
                     .with_label(he.label)
                     .with_size(ComponentSize{pixels(he.w), pixels(he.h)})
                     .with_absolute_position(sw + he.x_off, he.y)
                     .with_font_size(h720(he.font))
                     .with_custom_text_color(he.text_color);
      if (he.has_bg)
        cfg.with_custom_background(he.bg);
      if (he.rounded) {
        cfg.with_alignment(TextAlignment::Center)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.3f);
      }
      div(context, mk(entity, he.id), cfg);
    }

    // ========== CHAT MESSAGES (right side) ==========
    float chat_x = (float)screen_w - 250.0f;
    float chat_start_y = 60.0f;
    float chat_line_h = 20.0f;

    for (size_t i = 0; i < chat_messages.size(); i++) {
      auto &[username, message] = chat_messages[i];
      float chat_y = chat_start_y + (float)i * chat_line_h;

      // Username
      div(context, mk(entity, 320 + static_cast<int>(i) * 2),
          ComponentConfig{}
              .with_label(username)
              .with_size(ComponentSize{pixels(150), pixels(18)})
              .with_absolute_position(chat_x, chat_y)
              .with_font_size(h720(14.0f))
              .with_custom_text_color(slider_orange)
              .with_debug_name("chat_user_" + std::to_string(i)));

      // Message
      div(context, mk(entity, 321 + static_cast<int>(i) * 2),
          ComponentConfig{}
              .with_label(message)
              .with_size(ComponentSize{pixels(85), pixels(18)})
              .with_absolute_position(chat_x + 148.0f, chat_y)
              .with_font_size(h720(14.0f))
              .with_custom_text_color(text_white)
              .with_debug_name("chat_msg_" + std::to_string(i)));
    }

    // ========== SPEEDOMETER (bottom right) ==========
    float sh = (float)screen_h;
    div(context, mk(entity, 400),
        ComponentConfig{}
            .with_label("009")
            .with_size(ComponentSize{pixels(85), pixels(55)})
            .with_absolute_position(sw - 120.0f, sh - 90.0f)
            .with_custom_background(afterhours::Color{25, 30, 38, 230})
            .with_font_size(h720(36.0f))
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.2f));

    struct SpeedLabel {
      int id;
      const char *text;
      int w;
      int h;
      float x;
      float y;
      float font;
      afterhours::Color color;
    };
    SpeedLabel speed_labels[] = {
        {401, "M/h", 50, 20, sw - 90.0f, sh - 45.0f, 12.0f, text_muted},
        {402, "GAMON", 70, 18, sw - 100.0f, sh - 28.0f, 12.0f, slider_orange},
    };
    for (auto &sl : speed_labels) {
      div(context, mk(entity, sl.id),
          ComponentConfig{}
              .with_label(sl.text)
              .with_size(ComponentSize{pixels(sl.w), pixels(sl.h)})
              .with_absolute_position(sl.x, sl.y)
              .with_font_size(h720(sl.font))
              .with_custom_text_color(sl.color)
              .with_alignment(TextAlignment::Center));
    }
  }

  // ---- Shared helpers to reduce boilerplate ----

  // Row background (rounded, dark, absolute-positioned)
  void render_row_bg(UIContext<InputAction> &context,
                     afterhours::Entity &entity, int id, float x, float y,
                     float w, float h, const std::string &name) {
    div(context, mk(entity, id),
        ComponentConfig{}
            .with_720p_size(w, h)
            .with_absolute_position(x, y)
            .with_custom_background(row_dark)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.15f)
            .with_debug_name(name));
  }

  // Clickable row background (returns true when clicked)
  bool render_row_bg_button(UIContext<InputAction> &context,
                            afterhours::Entity &entity, int id, float x,
                            float y, float w, float h,
                            const std::string &name) {
    return button(context, mk(entity, id),
                  ComponentConfig{}
                      .with_720p_size(w, h)
                      .with_absolute_position(x, y)
                      .with_custom_background(row_dark)
                      .with_rounded_corners(RoundedCorners())
                      .with_roundness(0.15f)
                      .with_debug_name(name));
  }

  // 28x28 colored circle icon with optional text symbol
  void render_icon(UIContext<InputAction> &context, afterhours::Entity &entity,
                   int id, float x, float y, afterhours::Color color,
                   const std::string &symbol, const std::string &name) {
    auto cfg = ComponentConfig{}
                   .with_size(ComponentSize{pixels(28), pixels(28)})
                   .with_absolute_position(x, y)
                   .with_custom_background(color)
                   .with_rounded_corners(RoundedCorners())
                   .with_roundness(1.0f)
                   .with_debug_name(name);
    if (!symbol.empty()) {
      cfg.with_label(symbol)
          .with_font_size(h720(14.0f))
          .with_custom_text_color(text_white)
          .with_alignment(TextAlignment::Center);
    }
    div(context, mk(entity, id), cfg);
  }

  // Rainbow icon (3 concentric circles)
  void render_rainbow_icon(UIContext<InputAction> &context,
                           afterhours::Entity &entity, int base_id, float x,
                           float y, const std::string &prefix) {
    struct Ring {
      int sz;
      float off;
      afterhours::Color c;
      int id_off;
    };
    Ring rings[] = {
        {28, 0, icon_rainbow1, 1},
        {16, 6, icon_rainbow2, 5},
        {8, 10, icon_rainbow3, 6},
    };
    for (auto &r : rings) {
      div(context, mk(entity, base_id + r.id_off),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(r.sz), pixels(r.sz)})
              .with_absolute_position(x + r.off, y + r.off)
              .with_custom_background(r.c)
              .with_rounded_corners(RoundedCorners())
              .with_roundness(1.0f)
              .with_debug_name(prefix + std::to_string(base_id)));
    }
  }

  // Row label text
  void render_row_label(UIContext<InputAction> &context,
                        afterhours::Entity &entity, int id, float x, float y,
                        float h, int label_w, const std::string &label,
                        const std::string &name) {
    auto pxf = [](float v) { return pixels(static_cast<int>(v)); };
    div(context, mk(entity, id),
        ComponentConfig{}
            .with_label(label)
            .with_size(ComponentSize{pixels(label_w), pxf(h)})
            .with_absolute_position(x + 42.0f, y + 12.0f)
            .with_font_size(h720(18.0f))
            .with_custom_text_color(text_white)
            .with_debug_name(name));
  }

  // Pill toggle: track (52×28) + sliding white knob (22×22)
  static constexpr float pc_track_w = 52.0f, pc_track_h = 28.0f;
  static constexpr float pc_knob_pad = 4.0f;
  static constexpr float pc_knob_sz = pc_track_h - pc_knob_pad * 2.0f; // 20px
  static constexpr float pc_knob_travel =
      pc_track_w - pc_knob_sz - pc_knob_pad * 2.0f;

  void render_toggle(UIContext<InputAction> &context,
                     afterhours::Entity &entity, int base_id, float x, float y,
                     float w, bool value) {
    afterhours::Color track_color = value ? toggle_green : toggle_track;
    float track_x = x + w - pc_track_w - 8.0f;
    float track_y = y + 8.0f;
    div(context, mk(entity, base_id),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(pc_track_w), pixels(pc_track_h)})
            .with_absolute_position(track_x, track_y)
            .with_custom_background(track_color)
            .with_rounded_corners(RoundedCorners().all_round())
            .with_roundness(0.5f)
            .with_debug_name("toggle_track_" + std::to_string(base_id)));
    float knob_x = track_x + pc_knob_pad + (value ? pc_knob_travel : 0.0f);
    float knob_y = track_y + pc_knob_pad;
    div(context, mk(entity, base_id + 1),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(pc_knob_sz), pixels(pc_knob_sz)})
            .with_absolute_position(knob_x, knob_y)
            .with_custom_background(text_white)
            .with_border(afterhours::Color{0, 0, 0, 40}, 1.0f)
            .with_rounded_corners(RoundedCorners().all_round())
            .with_roundness(1.0f)
            .with_debug_name("toggle_knob_" + std::to_string(base_id)));
  }

  // Chevron button (< or >)
  bool render_chevron(UIContext<InputAction> &context,
                      afterhours::Entity &entity, int id, float x, float y,
                      const std::string &symbol, const std::string &name) {
    return button(
        context, mk(entity, id),
        ComponentConfig{}
            .with_label(symbol)
            .with_size(ComponentSize{pixels(44), pixels(44)})
            .with_absolute_position(x, y)
            .with_font_size(h720(22.0f))
            .with_custom_text_color(text_muted)
            .with_custom_background(afterhours::Color{55, 60, 70, 180})
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.3f)
            .with_debug_name(name));
  }

  // ---- Row renderers (now using shared helpers) ----

  void render_language_row(UIContext<InputAction> &context,
                           afterhours::Entity &entity, int base_id, float x,
                           float y, float w, float h) {
    auto pxf = [](float v) { return pixels(static_cast<int>(v)); };
    render_row_bg(context, entity, base_id, x, y, w, h, "lang_row");
    render_icon(context, entity, base_id + 1, x + 8.0f, y + 8.0f, icon_purple,
                "@", "lang_icon");
    render_row_label(context, entity, base_id + 2, x, y, h, 90, "Language",
                     "lang_label");

    if (render_chevron(context, entity, base_id + 3, x + w - 190.0f, y, "<",
                       "lang_left")) {
      language_idx = (language_idx == 0)
                         ? static_cast<int>(languages.size()) - 1
                         : language_idx - 1;
    }

    div(context, mk(entity, base_id + 4),
        ComponentConfig{}
            .with_label(languages[language_idx])
            .with_size(ComponentSize{pixels(115), pxf(h)})
            .with_absolute_position(x + w - 145.0f, y + 12.0f)
            .with_font_size(h720(16.0f))
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center));

    if (render_chevron(context, entity, base_id + 5, x + w - 44.0f, y, ">",
                       "lang_right")) {
      language_idx = (language_idx + 1) % languages.size();
    }
  }

  // Unified toggle row: pass rainbow=true for multicolor icon, else solid icon
  void render_toggle_row_with_icon(UIContext<InputAction> &context,
                                   afterhours::Entity &entity, int base_id,
                                   float x, float y, float w, float h,
                                   const std::string &label, bool &value,
                                   afterhours::Color icon_color,
                                   const std::string &icon_symbol,
                                   bool rainbow = false) {
    if (render_row_bg_button(context, entity, base_id, x, y, w, h,
                             "toggle_row_" + std::to_string(base_id))) {
      value = !value;
    }
    if (rainbow) {
      render_rainbow_icon(context, entity, base_id, x + 8.0f, y + 8.0f,
                          "rainbow_");
    } else {
      render_icon(context, entity, base_id + 1, x + 8.0f, y + 8.0f, icon_color,
                  icon_symbol, "toggle_icon_" + std::to_string(base_id));
    }
    render_row_label(context, entity, base_id + 2, x, y, h, 140, label,
                     "toggle_label_" + std::to_string(base_id));
    render_toggle(context, entity, base_id + 3, x, y, w, value);
  }

  void render_selector_row(UIContext<InputAction> &context,
                           afterhours::Entity &entity, int base_id, float x,
                           float y, float w, float h, const std::string &label,
                           const std::vector<std::string> &options,
                           size_t &option_idx, afterhours::Color icon_color) {
    auto pxf = [](float v) { return pixels(static_cast<int>(v)); };
    render_row_bg(context, entity, base_id, x, y, w, h,
                  "selector_row_" + std::to_string(base_id));
    render_icon(context, entity, base_id + 1, x + 8.0f, y + 8.0f, icon_color,
                "*", "selector_icon_" + std::to_string(base_id));
    render_row_label(context, entity, base_id + 2, x, y, h, 100, label,
                     "selector_label_" + std::to_string(base_id));

    if (render_chevron(context, entity, base_id + 3, x + w - 130.0f, y, "<",
                       "selector_left_" + std::to_string(base_id))) {
      option_idx = (option_idx == 0) ? options.size() - 1 : option_idx - 1;
    }

    div(context, mk(entity, base_id + 4),
        ComponentConfig{}
            .with_label(options[option_idx])
            .with_size(ComponentSize{pixels(50), pxf(h)})
            .with_absolute_position(x + w - 85.0f, y + 9.0f)
            .with_font_size(h720(16.0f))
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("selector_value_" + std::to_string(base_id)));

    if (render_chevron(context, entity, base_id + 5, x + w - 44.0f, y, ">",
                       "selector_right_" + std::to_string(base_id))) {
      option_idx = (option_idx + 1) % options.size();
    }
  }

  // Unified display row: pass rainbow=true for multicolor icon
  void render_display_row(UIContext<InputAction> &context,
                          afterhours::Entity &entity, int base_id, float x,
                          float y, float w, float h, const std::string &label,
                          const std::string &value,
                          afterhours::Color icon_color, bool rainbow = false) {
    auto pxf = [](float v) { return pixels(static_cast<int>(v)); };
    render_row_bg(context, entity, base_id, x, y, w, h,
                  "display_row_" + std::to_string(base_id));
    if (rainbow) {
      render_rainbow_icon(context, entity, base_id, x + 8.0f, y + 8.0f,
                          "rainbow_");
    } else {
      render_icon(context, entity, base_id + 1, x + 8.0f, y + 8.0f, icon_color,
                  "*", "display_icon_" + std::to_string(base_id));
    }
    render_row_label(context, entity, base_id + 2, x, y, h, 130, label,
                     "display_label_" + std::to_string(base_id));

    div(context, mk(entity, base_id + 3),
        ComponentConfig{}
            .with_label(value)
            .with_size(ComponentSize{pixels(85), pxf(h)})
            .with_absolute_position(x + w - 92.0f, y + 12.0f)
            .with_font_size(h720(16.0f))
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Right)
            .with_debug_name("display_value_" + std::to_string(base_id)));
  }

  void render_volume_slider(UIContext<InputAction> &context,
                            afterhours::Entity &entity, int base_id, float x,
                            float y, float w, float h, const std::string &label,
                            float &value, afterhours::Color icon_color) {
    render_row_bg(context, entity, base_id, x, y, w, h,
                  "volume_row_" + std::to_string(base_id));
    render_icon(context, entity, base_id + 1, x + 8.0f, y + 8.0f, icon_color,
                "*", "volume_icon_" + std::to_string(base_id));
    render_row_label(context, entity, base_id + 2, x, y, h, 110, label,
                     "volume_label_" + std::to_string(base_id));

    float slider_w = 130.0f, slider_x = x + w - slider_w - 10.0f;
    float slider_y_pos = y + 10.0f, slider_h = 20.0f;

    div(context, mk(entity, base_id + 3),
        ComponentConfig{}
            .with_720p_size(slider_w, slider_h)
            .with_absolute_position(slider_x, slider_y_pos)
            .with_custom_background(slider_track)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.5f)
            .with_debug_name("volume_track_" + std::to_string(base_id)));

    float fill_w = slider_w * value;
    if (fill_w > 2.0f) {
      div(context, mk(entity, base_id + 4),
          ComponentConfig{}
              .with_720p_size(fill_w, slider_h)
              .with_absolute_position(slider_x, slider_y_pos)
              .with_custom_background(slider_green)
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.5f)
              .with_debug_name("volume_fill_" + std::to_string(base_id)));
    }

    float handle_w = 28.0f;
    float handle_x = slider_x + (slider_w - handle_w) * value;
    div(context, mk(entity, base_id + 5),
        ComponentConfig{}
            .with_720p_size(handle_w, slider_h + 8)
            .with_absolute_position(handle_x, slider_y_pos - 4.0f)
            .with_custom_background(text_white)
            .with_border(slider_green, 2.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.5f)
            .with_debug_name("volume_handle_" + std::to_string(base_id)));
  }
};

REGISTER_EXAMPLE_SCREEN(parcel_corps_settings, "Game Mockups",
                        "Phone UI settings overlay (Parcel Corps style)",
                        ParcelCorpsSettingsScreen)
