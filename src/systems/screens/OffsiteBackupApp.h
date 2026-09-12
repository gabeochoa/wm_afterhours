#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <algorithm>
#include <array>
#include <bitset>
#include <cmath>
#include <string>
#include <vector>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct OffsiteBackupApp : ScreenSystem<UIContext<InputAction>> {
  struct Folder {
    const char *name;
    const char *size;
    float target;
  };

  static constexpr std::array<Folder, 5> folders{{
      {"Documents", "12.4 GB", 1.00f}, {"Photos", "184.2 GB", 0.71f},
      {"Projects", "46.8 GB", 1.00f},  {"Music", "31.0 GB", 0.34f},
      {"Archive", "512.6 GB", 0.08f},
  }};

  float overall = 0.f;
  std::array<float, 5> folder_now{0.f, 0.f, 0.f, 0.f, 0.f};
  bool seeded = false;
  bool paused = false;
  int focused_control = 0;
  float bandwidth = 0.62f;
  size_t schedule_idx = 1;
  size_t retention_idx = 2;
  bool pause_on_battery = true;
  bool backup_externals = false;
  std::string action_status = "Overview";

  std::vector<std::string> schedules = {"Hourly", "Every 4 hours", "Daily", "Weekly"};
  std::vector<std::string> retentions = {"7 days", "30 days", "1 year", "Forever"};

  static void ease(float &now, float target, float dt) {
    now += (target - now) * std::min(1.f, dt * 3.f);
    if (std::fabs(target - now) < 0.001f)
      now = target;
  }

  int complete_count() const {
    int n = 0;
    for (float value : folder_now) {
      if (value >= .995f)
        ++n;
    }
    return n;
  }

  float total_uploaded_gb() const { return overall * 787.f; }

  ComponentConfig box(float scale, float x, float y, float w, float h) const {
    return ComponentConfig{}
        .with_size({pixels(w * scale), pixels(h * scale)})
        .with_absolute_position(x * scale, y * scale)
        .with_background(Theme::Usage::None)
        .with_corner_radius(0.f);
  }

  static std::string pct_text(float value) {
    if (value >= .995f)
      return "done";
    return fmt::format("{:.0f}%", value * 100.f);
  }

  void change_schedule(int delta) {
    schedule_idx = (schedule_idx + schedules.size() + delta) % schedules.size();
    action_status = "Run backup: " + schedules[schedule_idx];
  }

  void change_retention(int delta) {
    retention_idx = (retention_idx + retentions.size() + delta) % retentions.size();
    action_status = "Keep file versions: " + retentions[retention_idx];
  }

  void change_bandwidth(int delta) {
    const float next = std::clamp(bandwidth + static_cast<float>(delta) * .10f, 0.f, 1.f);
    bandwidth = next;
    action_status = fmt::format("Bandwidth limit {:.0f}%", bandwidth * 100.f);
  }

  void activate_focused_control(int delta) {
    if (focused_control == 0)
      change_schedule(delta);
    else if (focused_control == 1)
      change_retention(delta);
    else if (focused_control == 2)
      change_bandwidth(delta);
    else if (focused_control == 3) {
      pause_on_battery = !pause_on_battery;
      action_status = pause_on_battery ? "Battery pause enabled" : "Battery pause disabled";
    } else if (focused_control == 4) {
      backup_externals = !backup_externals;
      action_status = backup_externals ? "External drives included" : "External drives excluded";
    }
  }

  static void draw_background(RectangleType r) {
    afterhours::draw_rectangle_gradient_v(r, afterhours::Color{145, 178, 185, 255},
                                          afterhours::Color{170, 178, 166, 255});
    afterhours::draw_rectangle_gradient_h({r.x, r.y, r.width, r.height},
                                          afterhours::Color{130, 167, 176, 80},
                                          afterhours::Color{226, 223, 214, 115});
  }

  static void draw_menubar(RectangleType r) {
    afterhours::draw_rectangle(r, afterhours::Color{239, 242, 239, 228});
    afterhours::draw_circle(static_cast<int>(r.x + 23.f), static_cast<int>(r.y + 14.f), 3.f,
                            afterhours::Color{43, 58, 58, 255});
  }

  static void draw_window(RectangleType r) {
    afterhours::draw_rectangle_rounded(r, .012f, 12, afterhours::Color{249, 249, 249, 255},
                                       std::bitset<4>().set());
    afterhours::draw_rectangle_rounded_lines_ex(r, .012f, 12, 1.f,
                                                afterhours::Color{45, 55, 58, 82});
  }

  static void draw_titlebar(RectangleType r) {
    afterhours::draw_rectangle_gradient_v(r, afterhours::Color{241, 241, 241, 255},
                                          afterhours::Color{224, 224, 224, 255});
    afterhours::draw_rectangle({r.x, r.y + r.height - 1.f, r.width, 1.f},
                               afterhours::Color{197, 197, 197, 255});
    const std::array<afterhours::Color, 3> traffic{{
        {242, 101, 94, 255}, {237, 191, 71, 255}, {98, 196, 83, 255}}};
    for (int i = 0; i < 3; ++i) {
      afterhours::draw_circle(static_cast<int>(r.x + 18.f + i * 18.f),
                              static_cast<int>(r.y + 17.f), 5.5f, traffic[i]);
    }
  }

  static void draw_brand_mark(RectangleType r) {
    const auto red = afterhours::Color{223, 87, 75, 255};
    const float cx = r.x + r.width * .5f;
    afterhours::draw_line_ex({cx, r.y + 9.f}, {cx, r.y + 39.f}, 5.f, red);
    afterhours::draw_line_ex({cx, r.y + 9.f}, {cx - 8.f, r.y + 20.f}, 5.f, red);
    afterhours::draw_line_ex({cx, r.y + 9.f}, {cx + 8.f, r.y + 20.f}, 5.f, red);
    afterhours::draw_line_ex({cx - 8.f, r.y + 25.f}, {cx, r.y + 17.f}, 4.f, red);
    afterhours::draw_line_ex({cx + 8.f, r.y + 25.f}, {cx, r.y + 17.f}, 4.f, red);
  }

  static void draw_computer(RectangleType r) {
    afterhours::draw_rectangle_rounded({r.x + 7.f, r.y + 2.f, 40.f, 28.f}, .08f, 5,
                                       afterhours::Color{70, 83, 91, 255},
                                       std::bitset<4>().set());
    afterhours::draw_rectangle_gradient_h({r.x + 11.f, r.y + 6.f, 32.f, 20.f},
                                          afterhours::Color{61, 111, 140, 255},
                                          afterhours::Color{147, 182, 204, 255});
    afterhours::draw_rectangle_rounded({r.x + 2.f, r.y + 32.f, 50.f, 4.f}, .35f, 8,
                                       afterhours::Color{137, 148, 155, 255},
                                       std::bitset<4>().set());
  }

  static void draw_folder_icon(RectangleType r) {
    const auto fill = afterhours::Color{122, 169, 194, 255};
    afterhours::draw_triangle({r.x, r.y + r.height}, {r.x + r.width * .22f, r.y},
                              {r.x + r.width, r.y}, fill);
    afterhours::draw_triangle({r.x, r.y + r.height}, {r.x + r.width, r.y},
                              {r.x + r.width * .78f, r.y + r.height}, fill);
  }

  static void draw_ring(RectangleType r, float value) {
    const float cx = r.x + r.width * .5f;
    const float cy = r.y + r.height * .5f;
    const float outer = std::min(r.width, r.height) * .5f;
    const float inner = outer - 12.f;
    afterhours::draw_ring(cx, cy, inner, outer, 72, afterhours::Color{238, 237, 237, 255});
    afterhours::draw_ring_segment(cx, cy, inner, outer, -90.f, -90.f + 360.f * value, 72,
                                  afterhours::Color{232, 95, 84, 255});
  }

  static void draw_track(RectangleType r, float value, afterhours::Color fill) {
    afterhours::draw_rectangle_rounded(r, .5f, 8, afterhours::Color{237, 240, 242, 255},
                                       std::bitset<4>().set());
    afterhours::draw_rectangle_rounded({r.x, r.y, r.width * std::clamp(value, 0.f, 1.f), r.height},
                                       .5f, 8, fill, std::bitset<4>().set());
  }

  static void draw_storage(RectangleType r) {
    afterhours::draw_rectangle_rounded(r, .5f, 8, afterhours::Color{227, 231, 235, 255},
                                       std::bitset<4>().set());
    afterhours::draw_rectangle_rounded({r.x, r.y, r.width * .39f, r.height}, .5f, 8,
                                       afterhours::Color{144, 158, 172, 255},
                                       std::bitset<4>().set());
  }

  static void draw_slider(RectangleType r, float value) {
    afterhours::draw_rectangle_rounded({r.x, r.y + r.height * .5f - 3.f, r.width, 6.f},
                                       .6f, 8, afterhours::Color{67, 70, 74, 255},
                                       std::bitset<4>().set());
    afterhours::draw_rectangle_rounded({r.x, r.y + r.height * .5f - 3.f, r.width * value, 6.f},
                                       .6f, 8, afterhours::Color{221, 105, 94, 255},
                                       std::bitset<4>().set());
    afterhours::draw_circle(static_cast<int>(r.x + r.width * value),
                            static_cast<int>(r.y + r.height * .5f), 7.f,
                            afterhours::Color{216, 103, 91, 255});
  }

  static void draw_toggle(RectangleType r, bool on) {
    afterhours::draw_rectangle_rounded(r, .5f, 16,
                                       on ? afterhours::Color{116, 178, 235, 255}
                                          : afterhours::Color{132, 146, 166, 255},
                                       std::bitset<4>().set());
    const float knob_x = on ? r.x + r.width - r.height * .5f : r.x + r.height * .5f;
    afterhours::draw_circle(static_cast<int>(knob_x), static_cast<int>(r.y + r.height * .5f),
                            r.height * .38f, afterhours::Color{255, 255, 255, 255});
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

    if (!seeded) {
      seeded = true;
      for (size_t i = 0; i < folders.size(); ++i)
        folder_now[i] = folders[i].target;
      overall = .626f;
    }

    if (context.pressed(InputAction::WidgetDown))
      focused_control = (focused_control + 1) % 5;
    if (context.pressed(InputAction::WidgetUp))
      focused_control = (focused_control + 4) % 5;
    if (context.pressed(InputAction::WidgetRight))
      activate_focused_control(1);
    if (context.pressed(InputAction::WidgetLeft))
      activate_focused_control(-1);

    float sum = 0.f;
    for (size_t i = 0; i < folders.size(); ++i) {
      if (!paused)
        ease(folder_now[i], folders[i].target, dt);
      sum += folder_now[i];
    }
    if (!paused)
      ease(overall, sum / static_cast<float>(folders.size()), dt);

    Theme theme;
    theme.font = {53, 58, 64, 255};
    theme.darkfont = {53, 58, 64, 255};
    theme.font_muted = {135, 144, 154, 255};
    theme.background = {249, 249, 249, 255};
    theme.surface = {255, 255, 255, 255};
    theme.primary = {223, 87, 75, 255};
    theme.secondary = {104, 114, 124, 255};
    theme.accent = {223, 87, 75, 255};
    theme.roundness = .02f;
    context.theme = theme;
    context.scaling_mode = ScalingMode::Proportional;
    UIStylingDefaults::get().set_default_font("Atkinson", pixels(13.f * scale));

    const auto text = [&](int id, std::string label, float x, float y, float w, float h,
                          float size, afterhours::Color color,
                          TextAlignment align = TextAlignment::Left,
                          const std::string &debug = "", const char *font = "Atkinson") {
      div(context, mk(entity, id),
          box(scale, x, y, w, std::max(h, size * 1.62f))
              .with_label(label)
              .with_font(font, pixels(size * scale * 1.35f))
              .with_custom_text_color(color)
              .with_alignment(align)
              .with_letter_spacing(0.f)
              .with_debug_name(debug));
    };
    const auto panel = [&](int id, float x, float y, float w, float h,
                           afterhours::Color fill, const char *debug = "") {
      div(context, mk(entity, id),
          box(scale, x, y, w, h).with_custom_background(fill).with_debug_name(debug));
    };

    const afterhours::Color red{223, 87, 75, 255};
    const afterhours::Color ink{53, 58, 64, 255};
    const afterhours::Color mid{103, 114, 126, 255};
    const afterhours::Color muted{145, 153, 163, 255};
    const afterhours::Color line{226, 226, 226, 255};

    auto root = div(context, mk(entity, 0),
                    box(scale, 0, 0, 1280, 720)
                        .with_on_draw_bg([](RectangleType r) { draw_background(r); })
                        .with_debug_name("bk_root"));

    div(context, mk(root.ent(), 1),
        box(scale, 0, 0, 1280, 28)
            .with_on_draw_bg([](RectangleType r) { draw_menubar(r); })
            .with_debug_name("bk_menubar"));
    text(2, "Offsite Backup", 37, 7, 100, 16, 13, afterhours::Color{38, 48, 48, 255},
         TextAlignment::Left, "bk_menu_app", "Archivo@bold");
    text(3, "File   Edit   Window   Help", 147, 7, 230, 16, 12,
         afterhours::Color{49, 62, 62, 255});
    text(4, "Sat 2:03 PM", 1176, 7, 82, 16, 12, afterhours::Color{28, 39, 39, 255},
         TextAlignment::Right, "bk_menu_time");

    div(context, mk(root.ent(), 10),
        box(scale, 90, 65, 1100, 622)
            .with_on_draw_bg([](RectangleType r) { draw_window(r); })
            .with_debug_name("bk_window"));
    div(context, mk(root.ent(), 11),
        box(scale, 90, 65, 1100, 35)
            .with_on_draw_bg([](RectangleType r) { draw_titlebar(r); })
            .with_debug_name("bk_titlebar"));
    text(12, "Offsite Backup", 587, 78, 106, 14, 12, afterhours::Color{82, 82, 82, 255},
         TextAlignment::Center, "bk_window_title", "Archivo@bold");

    panel(20, 90, 100, 1100, 76, afterhours::Color{255, 255, 255, 255}, "bk_nav");
    panel(21, 90, 175, 1100, 1, line);
    div(context, mk(root.ent(), 22), box(scale, 126, 117, 24, 42)
                                      .with_on_draw_bg([](RectangleType r) { draw_brand_mark(r); }));
    text(23, "offsite", 166, 116, 130, 35, 31, red, TextAlignment::Left,
         "bk_brand", "Archivo@bold");
    text(24, "COMPUTER BACKUP", 166, 151, 150, 14, 9, muted, TextAlignment::Left,
         "bk_brand_sub", "Archivo");

    if (button(context, mk(root.ent(), 30),
               box(scale, 910, 100, 64, 76)
                   .with_label("Overview")
                   .with_font("Atkinson", pixels(16.f * scale))
                   .with_custom_text_color(red)
                   .with_custom_background(afterhours::Color{0, 0, 0, 0})
                   .with_alignment(TextAlignment::Center)
                   .with_debug_name("bk_tab_overview"))) {
      action_status = "Overview";
    }
    panel(31, 910, 172, 64, 3, red);
    if (button(context, mk(root.ent(), 32),
               box(scale, 1000, 100, 58, 76)
                   .with_label("Settings")
                   .with_font("Atkinson", pixels(16.f * scale))
                   .with_custom_text_color(afterhours::Color{126, 126, 126, 255})
                   .with_custom_background(afterhours::Color{0, 0, 0, 0})
                   .with_alignment(TextAlignment::Center)
                   .with_debug_name("bk_tab_settings"))) {
      action_status = "Settings selected";
      focused_control = 0;
    }
    if (button(context, mk(root.ent(), 33),
               box(scale, 1080, 100, 88, 76)
                   .with_label("Restore files")
                   .with_font("Atkinson", pixels(16.f * scale))
                   .with_custom_text_color(afterhours::Color{126, 126, 126, 255})
                   .with_custom_background(afterhours::Color{0, 0, 0, 0})
                   .with_alignment(TextAlignment::Center)
                   .with_debug_name("bk_restore"))) {
      action_status = "Restore options opened";
    }

    panel(40, 90, 176, 697, 474, afterhours::Color{255, 255, 255, 255}, "bk_main");
    panel(41, 786, 176, 1, 474, afterhours::Color{229, 229, 229, 255});
    panel(42, 787, 176, 403, 474, afterhours::Color{248, 249, 250, 255}, "bk_aside");

    div(context, mk(root.ent(), 50), box(scale, 117, 205, 52, 38)
                                      .with_on_draw_fg([](RectangleType r) { draw_computer(r); }));
    text(51, "MacBook Pro", 175, 199, 180, 25, 20, ink, TextAlignment::Left,
         "bk_device", "Archivo@bold");
    text(52, "Personal Backup - 2 TB plan", 175, 228, 210, 16, 11, muted,
         TextAlignment::Left, "bk_plan_line");
    panel(53, 688, 208, 65, 24, afterhours::Color{239, 246, 242, 255}, "bk_health_badge");
    text(54, "Healthy", 701, 214, 44, 13, 11, afterhours::Color{82, 149, 118, 255},
         TextAlignment::Center, "bk_health");
    panel(55, 696, 219, 4, 4, afterhours::Color{82, 149, 118, 255});

    div(context, mk(root.ent(), 60),
        box(scale, 129, 251, 125, 125)
            .with_on_draw_fg([value = overall](RectangleType r) { draw_ring(r, value); })
            .with_debug_name("bk_ring"));
    text(61, fmt::format("{:.0f}", overall * 100.f), 161, 291, 54, 37, 37, ink,
         TextAlignment::Right, "bk_ring_pct");
    text(62, "%", 216, 304, 17, 21, 17, ink, TextAlignment::Left, "bk_ring_percent");
    text(63, "COMPLETE", 164, 334, 58, 11, 8, muted, TextAlignment::Center,
         "bk_ring_caption", "Archivo");

    text(64, paused ? "Backup paused" : "Backing up your files", 281, 263, 260, 24, 20, ink,
         TextAlignment::Left, "bk_progress_title");
    text(65, fmt::format("{:.0f} GB of 787 GB uploaded", total_uploaded_gb()), 281, 294,
         220, 17, 13, mid, TextAlignment::Left, "bk_bytes");
    text(66, fmt::format("{} of 5 folders complete", complete_count()), 281, 318, 180, 15,
         11, muted, TextAlignment::Left, "bk_folders_done");
    if (button(context, mk(root.ent(), 67),
               box(scale, 281, 339, 105, 28)
                   .with_label(paused ? "> Resume backup" : "II Pause backup")
                   .with_font("Atkinson", pixels(13.f * scale))
                   .with_custom_text_color(afterhours::Color{103, 108, 115, 255})
                   .with_custom_background(afterhours::Color{248, 248, 248, 255})
                   .with_debug_name("bk_pause"))) {
      paused = !paused;
      action_status = paused ? "Backup paused" : "Backup resumed";
    }

    panel(70, 122, 387, 629, 1, afterhours::Color{238, 238, 238, 255});
    panel(71, 122, 423, 629, 1, afterhours::Color{238, 238, 238, 255});
    text(72, "24.8 MB/s upload speed", 128, 399, 170, 13, 11, muted, TextAlignment::Left,
         "bk_speed");
    text(73, "About 3 hours remaining", 592, 399, 160, 13, 11, muted, TextAlignment::Right,
         "bk_eta");
    text(80, "FOLDERS", 122, 442, 80, 12, 9, muted, TextAlignment::Left,
         "bk_folders_hdr", "Archivo@bold");
    if (button(context, mk(root.ent(), 82),
               box(scale, 700, 435, 52, 22)
                   .with_label("Rescan")
                   .with_font("Atkinson", pixels(13.f * scale))
                   .with_custom_text_color(red)
                   .with_custom_background(afterhours::Color{0, 0, 0, 0})
                   .with_alignment(TextAlignment::Right)
                   .with_debug_name("bk_rescan"))) {
      overall = 0.f;
      for (float &value : folder_now)
        value = 0.f;
      paused = false;
      action_status = "Rescan started";
    }

    for (size_t i = 0; i < folders.size(); ++i) {
      const float y = 469.f + static_cast<float>(i) * 29.f;
      div(context, mk(root.ent(), 100 + static_cast<int>(i)),
          box(scale, 121, y + 3, 16, 11)
              .with_on_draw_fg([](RectangleType r) { draw_folder_icon(r); })
              .with_debug_name("bk_icon_" + std::to_string(i)));
      text(110 + static_cast<int>(i), folders[i].name, 146, y, 94, 15, 12, ink,
           TextAlignment::Left, "bk_name_" + std::to_string(i));
      div(context, mk(root.ent(), 120 + static_cast<int>(i)),
          box(scale, 251, y + 3, 367, 5)
              .with_on_draw_fg([value = folder_now[i]](RectangleType r) {
                draw_track(r, value, value >= .995f ? afterhours::Color{103, 169, 135, 255}
                                                    : afterhours::Color{230, 102, 91, 255});
              })
              .with_debug_name("bk_bar_" + std::to_string(i)));
      text(130 + static_cast<int>(i), pct_text(folder_now[i]), 636, y, 42, 15, 10,
           folder_now[i] >= .995f ? afterhours::Color{94, 155, 127, 255} : muted,
           TextAlignment::Right, "bk_pct_" + std::to_string(i));
      text(140 + static_cast<int>(i), folders[i].size, 691, y, 64, 15, 10, muted,
           TextAlignment::Right, "bk_size_" + std::to_string(i));
    }

    text(160, "Last completed backup", 132, 618, 135, 15, 11, muted, TextAlignment::Left,
         "bk_last_label");
    text(161, "2 hours ago", 282, 618, 92, 15, 11, mid, TextAlignment::Left,
         "bk_last_value");

    text(200, "Backup settings", 815, 200, 190, 22, 17, ink, TextAlignment::Left,
         "bk_settings_title");
    text(201, "Run backup", 815, 247, 120, 17, 12, mid, TextAlignment::Left,
         "bk_run_label");
    if (button(context, mk(root.ent(), 202),
               box(scale, 1025, 238, 137, 27)
                   .with_label(schedules[schedule_idx])
                   .with_font("Atkinson", pixels(13.f * scale))
                   .with_custom_text_color(afterhours::Color{81, 91, 102, 255})
                   .with_custom_background(afterhours::Color{255, 255, 255, 255})
                   .with_alignment(TextAlignment::Center)
                   .with_debug_name("bk_schedule"))) {
      focused_control = 0;
      change_schedule(1);
    }
    text(203, "Keep file versions", 815, 293, 150, 17, 12, mid, TextAlignment::Left,
         "bk_keep_label");
    if (button(context, mk(root.ent(), 204),
               box(scale, 1025, 284, 137, 27)
                   .with_label(retentions[retention_idx])
                   .with_font("Atkinson", pixels(13.f * scale))
                   .with_custom_text_color(afterhours::Color{81, 91, 102, 255})
                   .with_custom_background(afterhours::Color{255, 255, 255, 255})
                   .with_alignment(TextAlignment::Center)
                   .with_debug_name("bk_retention"))) {
      focused_control = 1;
      change_retention(1);
    }
    text(205, "Bandwidth limit", 815, 334, 150, 17, 12, mid, TextAlignment::Left,
         "bk_bandwidth_label");
    text(206, fmt::format("{:.0f}%", bandwidth * 100.f), 1124, 334, 38, 17, 12, red,
         TextAlignment::Right, "bk_bandwidth_pct");
    if (button(context, mk(root.ent(), 207),
               box(scale, 816, 356, 344, 16)
                   .with_label("")
                   .with_custom_background(afterhours::Color{0, 0, 0, 0})
                   .with_on_draw_fg([value = bandwidth](RectangleType r) { draw_slider(r, value); })
                   .with_debug_name("bk_bandwidth"))) {
      focused_control = 2;
      change_bandwidth(1);
    }
    text(208, "Slower", 816, 378, 45, 11, 9, muted);
    text(209, "Faster", 1134, 378, 45, 11, 9, muted, TextAlignment::Right);
    text(210, fmt::format("Up to {:.1f} MB/s", bandwidth * 40.f), 815, 397, 130, 13, 10,
         muted, TextAlignment::Left, "bk_readout");

    if (button(context, mk(root.ent(), 211),
               box(scale, 833, 418, 180, 18)
                   .with_label("Pause on battery power")
                   .with_font("Atkinson", pixels(13.f * scale))
                   .with_custom_text_color(afterhours::Color{114, 124, 134, 255})
                   .with_custom_background(afterhours::Color{0, 0, 0, 0})
                   .with_alignment(TextAlignment::Left)
                   .with_debug_name("bk_battery_label"))) {
      focused_control = 3;
      pause_on_battery = !pause_on_battery;
      action_status = pause_on_battery ? "Battery pause enabled" : "Battery pause disabled";
    }
    div(context, mk(root.ent(), 212),
        box(scale, 815, 419, 12, 12)
            .with_custom_background(pause_on_battery ? red : afterhours::Color{255, 255, 255, 255})
            .with_debug_name("bk_battery_box"));
    if (button(context, mk(root.ent(), 214),
               box(scale, 833, 444, 180, 18)
                   .with_label("Include external drives")
                   .with_font("Atkinson", pixels(13.f * scale))
                   .with_custom_text_color(afterhours::Color{114, 124, 134, 255})
                   .with_custom_background(afterhours::Color{0, 0, 0, 0})
                   .with_alignment(TextAlignment::Left)
                   .with_debug_name("bk_external_label"))) {
      focused_control = 4;
      backup_externals = !backup_externals;
      action_status = backup_externals ? "External drives included" : "External drives excluded";
    }
    div(context, mk(root.ent(), 215),
        box(scale, 815, 445, 12, 12)
            .with_custom_background(backup_externals ? red : afterhours::Color{255, 255, 255, 255})
            .with_debug_name("bk_external_box"));
    panel(220, 815, 471, 347, 1, afterhours::Color{221, 225, 229, 255});
    text(221, "ACCOUNT STORAGE", 815, 492, 130, 12, 9, afterhours::Color{141, 152, 164, 255},
         TextAlignment::Left, "bk_account_hdr", "Archivo");
    text(222, "787 GB of 2 TB", 815, 517, 130, 14, 11, afterhours::Color{100, 111, 124, 255},
         TextAlignment::Left, "bk_plan");
    div(context, mk(root.ent(), 223), box(scale, 815, 538, 347, 5)
                                      .with_on_draw_fg([](RectangleType r) { draw_storage(r); })
                                      .with_debug_name("bk_storage"));
    if (button(context, mk(root.ent(), 224),
               box(scale, 815, 559, 120, 16)
                   .with_label("Manage account")
                   .with_font("Atkinson", pixels(12.f * scale))
                   .with_custom_text_color(afterhours::Color{185, 114, 103, 255})
                   .with_custom_background(afterhours::Color{0, 0, 0, 0})
                   .with_alignment(TextAlignment::Left)
                   .with_debug_name("bk_manage"))) {
      action_status = "Plan details opened";
    }
    panel(240, 90, 650, 1100, 37, afterhours::Color{250, 250, 250, 255}, "bk_footer");
    panel(241, 90, 650, 1100, 1, afterhours::Color{228, 228, 228, 255});
    panel(242, 117, 664, 8, 10, afterhours::Color{181, 193, 111, 255});
    text(243, "Your files are encrypted and stored safely offsite.", 130, 665, 330, 13, 10,
         muted, TextAlignment::Left, "bk_footer_text");
    if (button(context, mk(root.ent(), 244),
               box(scale, 1145, 658, 20, 20)
                   .with_label("?")
                   .with_font("Atkinson", pixels(14.f * scale))
                   .with_custom_text_color(afterhours::Color{170, 170, 170, 255})
                   .with_custom_background(afterhours::Color{255, 255, 255, 255})
                   .with_alignment(TextAlignment::Center)
                   .with_debug_name("bk_help"))) {
      action_status = "Backup help opened";
    }
    text(245, action_status, 915, 665, 210, 13, 10, muted, TextAlignment::Right,
         "bk_action_status");
  }
};

REGISTER_EXAMPLE_SCREEN(offsite_backup, "App Mockups",
                        "Backup client: desktop backup window with live controls",
                        OffsiteBackupApp)
