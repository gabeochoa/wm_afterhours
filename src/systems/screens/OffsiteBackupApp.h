#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include "DialogPresentation.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/modal.h>
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
    float size_gb;
    float target;
  };

  static constexpr std::array<Folder, 5> folders{{
      {"Documents", 12.4f, 1.00f}, {"Photos", 184.2f, 0.71f},
      {"Projects", 46.8f, 1.00f},  {"Music", 31.0f, 0.34f},
      {"Archive", 512.6f, 0.08f},
  }};

  float overall = 0.f;
  std::array<float, 5> folder_now{0.f, 0.f, 0.f, 0.f, 0.f};
  bool seeded = false;
  bool paused = false;
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

  float total_uploaded_gb() const {
    float total = 0;
    for (size_t i = 0; i < folders.size(); ++i) total += folders[i].size_gb * folder_now[i];
    return total;
  }
  static float total_gb() {
    float total = 0;
    for (const auto &folder : folders) total += folder.size_gb;
    return total;
  }
  enum class Detail { None, Restore, Account, Help };
  Detail detail = Detail::None;
  bool detail_open = false;


  ComponentConfig box(float scale, float x, float y, float w, float h) const {
    return ComponentConfig{}
        .with_size({pixels(w * scale), pixels(h * scale)})
        .with_absolute_position(x * scale, y * scale)
        .with_background(Theme::Usage::None)
        .with_corner_radius(0.f);
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
    const float inner = outer - r.width * (12.f / 132.f);
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
    afterhours::draw_rectangle_rounded({r.x, r.y, r.width * (787.f / 2000.f), r.height}, .5f, 8,
                                       afterhours::Color{144, 158, 172, 255},
                                       std::bitset<4>().set());
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
      overall = total_uploaded_gb() / total_gb();
    }

    for (size_t i = 0; i < folders.size(); ++i) {
      if (!paused) ease(folder_now[i], folders[i].target, dt);
    }
    overall = total_uploaded_gb() / total_gb();

    Theme theme;
    theme.font = {53, 58, 64, 255};
    theme.darkfont = {53, 58, 64, 255};
    theme.font_muted = {93, 104, 116, 255};
    theme.background = {249, 249, 249, 255};
    theme.surface = {255, 255, 255, 255};
    theme.primary = {223, 87, 75, 255};
    theme.secondary = {104, 114, 124, 255};
    theme.accent = {223, 87, 75, 255};
    theme.roundness = 0;
    theme.corner_radius = 0;
    context.theme = theme;
    context.scaling_mode = ScalingMode::Proportional;
    UIStylingDefaults::get().set_grid_snapping(false);
    UIStylingDefaults::get().set_default_font("AtkinsonMock", pixels(18.f * scale));

    div(context, mk(entity, 1), ComponentConfig{}
        .with_size({pixels(screen_w), pixels(screen_h)}).with_corner_radius(0)
        .with_on_draw_bg([](RectangleType r) { draw_background(r); }).with_debug_name("bk_canvas"));
    auto root = div(context, mk(entity, 0), box(scale, 0, 0, 1280, 720)
        .with_absolute_position((screen_w - 1280 * scale) / 2, (screen_h - 720 * scale) / 2)
        .with_debug_name("bk_root"));
    const auto text = [&](int id, std::string label, float x, float y, float w, float h,
                          float size, afterhours::Color color,
                          TextAlignment align = TextAlignment::Left,
                          const std::string &debug = "", const char *font = "AtkinsonMock") {
      div(context, mk(root.ent(), id), box(scale, x, y, w, h)
          .with_label(label).with_font(font, pixels(size * scale))
          .with_custom_text_color(color).with_alignment(align)
          .with_letter_spacing(0).with_text_inset(0, 0)
          .with_ignore_pointer_events().with_debug_name(debug));
    };
    const auto panel = [&](int id, float x, float y, float w, float h,
                           afterhours::Color fill, const char *debug = "") {
      div(context, mk(root.ent(), id), box(scale, x, y, w, h)
          .with_custom_background(fill).with_debug_name(debug));
    };
    const afterhours::Color red{179, 62, 52, 255};
    const afterhours::Color ink{45, 53, 61, 255};
    const afterhours::Color mid{82, 95, 107, 255};
    const afterhours::Color muted{92, 104, 117, 255};
    const afterhours::Color line{218, 224, 229, 255};

    div(context, mk(root.ent(), 1),
        box(scale, 0, 0, 1280, 28)
            .with_on_draw_bg([](RectangleType r) { draw_menubar(r); })
            .with_debug_name("bk_menubar"));
    text(2, "Offsite Backup", 37, 4, 120, 22, 15, afterhours::Color{38, 48, 48, 255},
         TextAlignment::Left, "bk_menu_app", "AtkinsonMockBold");
    text(3, "File   Edit   Window   Help", 176, 4, 268, 22, 15,
         afterhours::Color{49, 62, 62, 255});
    text(4, "Sat 2:03 PM", 1120, 4, 136, 22, 15, afterhours::Color{28, 39, 39, 255},
         TextAlignment::Right, "bk_menu_time");

    div(context, mk(root.ent(), 10),
        box(scale, 90, 65, 1100, 622)
            .with_on_draw_bg([](RectangleType r) { draw_window(r); })
            .with_debug_name("bk_window"));
    div(context, mk(root.ent(), 11),
        box(scale, 90, 65, 1100, 35)
            .with_on_draw_bg([](RectangleType r) { draw_titlebar(r); })
            .with_debug_name("bk_titlebar"));
    text(12, "Offsite Backup", 536, 71, 208, 24, 16, afterhours::Color{82, 82, 82, 255},
         TextAlignment::Center, "bk_window_title", "AtkinsonMockBold");

    panel(20, 90, 100, 1100, 76, afterhours::Color{255, 255, 255, 255}, "bk_nav");
    panel(21, 90, 175, 1100, 1, line);
    div(context, mk(root.ent(), 22), box(scale, 126, 117, 24, 42)
                                      .with_on_draw_bg([](RectangleType r) { draw_brand_mark(r); }));
    text(23, "offsite", 166, 112, 200, 37, 32, red, TextAlignment::Left,
         "bk_brand", "AtkinsonMockBold");
    text(24, "COMPUTER BACKUP", 166, 148, 210, 22, 15, muted, TextAlignment::Left,
         "bk_brand_sub", "AtkinsonMock");

    if (button(context, mk(root.ent(), 30),
               box(scale, 816, 100, 106, 76)
                   .with_label("Overview")
                   .with_font("AtkinsonMock", pixels(18.f * scale))
                   .with_custom_text_color(red)
                   .with_custom_background(afterhours::Color{0, 0, 0, 0})
                   .with_alignment(TextAlignment::Center)
                   .with_debug_name("bk_tab_overview"))) {
      action_status = "Overview";
    }
    panel(31, 816, 172, 106, 3, red);
    if (button(context, mk(root.ent(), 32),
               box(scale, 944, 100, 98, 76)
                   .with_label("Settings")
                   .with_font("AtkinsonMock", pixels(18.f * scale))
                   .with_custom_text_color(afterhours::Color{126, 126, 126, 255})
                   .with_custom_background(afterhours::Color{0, 0, 0, 0})
                   .with_alignment(TextAlignment::Center)
                   .with_debug_name("bk_tab_settings"))) {
      action_status = "Settings selected";
    }
    if (button(context, mk(root.ent(), 33),
               box(scale, 1056, 100, 116, 76)
                   .with_label("Restore files")
                   .with_font("AtkinsonMock", pixels(18.f * scale))
                   .with_custom_text_color(afterhours::Color{126, 126, 126, 255})
                   .with_custom_background(afterhours::Color{0, 0, 0, 0})
                   .with_alignment(TextAlignment::Center)
                   .with_debug_name("bk_restore"))) {
      action_status = "Restore preview opened";
      detail = Detail::Restore;
      detail_open = true;
    }

    panel(40, 90, 176, 697, 474, afterhours::Color{255, 255, 255, 255}, "bk_main");
    panel(41, 786, 176, 1, 474, afterhours::Color{229, 229, 229, 255});
    panel(42, 787, 176, 403, 474, afterhours::Color{248, 249, 250, 255}, "bk_aside");

    div(context, mk(root.ent(), 50), box(scale, 117, 200, 52, 38)
        .with_on_draw_fg([](RectangleType r) { draw_computer(r); }));
    text(51, "MacBook Pro", 180, 194, 330, 30, 25, ink, TextAlignment::Left, "bk_device", "AtkinsonMockBold");
    text(52, "Personal Backup · 2 TB plan", 180, 225, 380, 26, 18, mid, TextAlignment::Left, "bk_plan_line");
    panel(53, 642, 204, 112, 32, {231, 244, 235, 255}, "bk_health_badge");
    text(54, "Healthy", 668, 207, 78, 26, 18, {44, 113, 75, 255}, TextAlignment::Left, "bk_health");
    div(context, mk(root.ent(), 55), box(scale, 651, 216, 8, 8)
        .with_on_draw_fg([](RectangleType r) { afterhours::draw_circle(static_cast<int>(r.x + r.width / 2), static_cast<int>(r.y + r.height / 2), r.width / 2, {44, 113, 75, 255}); }));
    div(context, mk(root.ent(), 60), box(scale, 126, 260, 132, 132)
        .with_on_draw_fg([value = overall](RectangleType r) { draw_ring(r, value); }).with_debug_name("bk_ring"));
    text(61, fmt::format("{:.0f}%", overall * 100), 140, 299, 104, 40, 34, ink, TextAlignment::Center, "bk_ring_pct");
    text(63, "COMPLETE", 140, 339, 104, 23, 15, mid, TextAlignment::Center, "bk_ring_caption");
    text(64, paused ? "Backup paused" : "Backing up your files", 286, 264, 466, 34, 26, ink, TextAlignment::Left, "bk_progress_title");
    text(65, fmt::format("{:.0f} GB of {:.0f} GB uploaded", total_uploaded_gb(), total_gb()),
         286, 304, 466, 28, 21, ink, TextAlignment::Left, "bk_bytes");
    text(66, fmt::format("{} of 5 folders complete", complete_count()), 286, 335, 300, 26, 18, mid, TextAlignment::Left, "bk_folders_done");
    auto pause_button = button(context, mk(root.ent(), 67), box(scale, 286, 365, 196, 34)
        .with_label(paused ? "Resume backup" : "Pause backup")
        .with_font("AtkinsonMock", pixels(18 * scale)).with_custom_text_color(ink)
        .with_custom_background({235, 239, 242, 255}).with_corner_radius(4 * scale)
        .with_text_inset(32 * scale, 0).with_alignment(TextAlignment::Left)
        .with_on_draw_fg([paused = paused, scale, ink](RectangleType r) {
          if (paused) {
            afterhours::draw_triangle({r.x + 12 * scale, r.y + 9 * scale}, {r.x + 12 * scale, r.y + 25 * scale}, {r.x + 24 * scale, r.y + 17 * scale}, ink);
            return;
          }
          afterhours::draw_rectangle({r.x + 12 * scale, r.y + 9 * scale, 4 * scale, 16 * scale}, ink);
          afterhours::draw_rectangle({r.x + 20 * scale, r.y + 9 * scale, 4 * scale, 16 * scale}, ink);
        }).with_debug_name("bk_pause"));
    pause_button.ent().get<HasLabel>().text_x_offset = 32 * scale;
    if (pause_button) {
      paused = !paused;
      action_status = paused ? "Backup paused" : "Backup resumed";
    }
    panel(70, 122, 410, 629, 1, line);
    const float speed = paused ? 0 : bandwidth * 40;
    const int minutes = speed > 0 ? static_cast<int>(std::ceil((total_gb() - total_uploaded_gb()) * 1000 / speed / 60)) : 0;
    text(72, fmt::format("{:.1f} MB/s simulated", speed), 122, 416, 300, 28, 18, ink, TextAlignment::Left, "bk_speed");
    text(73, speed > 0 ? fmt::format("About {}h {}m remaining", minutes / 60, minutes % 60) : "Waiting to resume", 432, 416, 322, 28, 18, ink, TextAlignment::Right, "bk_eta");
    text(80, "FOLDERS", 122, 452, 124, 25, 16, mid, TextAlignment::Left, "bk_folders_hdr", "AtkinsonMockBold");
    text(81, "Progress", 290, 452, 200, 25, 16, mid);
    text(83, "State", 530, 452, 116, 25, 16, mid);
    text(84, "Size", 664, 452, 90, 25, 16, mid, TextAlignment::Right);
    auto rescan = button(context, mk(root.ent(), 82), box(scale, 644, 365, 110, 34)
        .with_label("Rescan").with_font("AtkinsonMock", pixels(18 * scale))
        .with_custom_text_color(red).with_custom_background({245, 238, 235, 255})
        .with_alignment(TextAlignment::Left).with_text_inset(0, 0)
        .with_on_draw_fg([scale, red](RectangleType r) {
          const float x = r.x + 18 * scale, y = r.y + 17 * scale;
          afterhours::draw_ring_segment(x, y, 6 * scale, 8 * scale, 40, 320, 24, red);
          afterhours::draw_triangle({x + 6 * scale, y - 8 * scale}, {x + 1 * scale, y - 7 * scale}, {x + 6 * scale, y - 2 * scale}, red);
        }).with_debug_name("bk_rescan"));
    rescan.ent().get<HasLabel>().text_x_offset = 38 * scale;
    if (rescan) {
      folder_now.fill(0);
      paused = false;
      action_status = "Rescan started";
    }
    for (size_t i = 0; i < folders.size(); ++i) {
      const float y = 483 + static_cast<float>(i) * 25;
      const bool done = folder_now[i] >= .995f;
      const bool scanning = folder_now[i] + .001f < folders[i].target;
      const std::string state = done ? "Done" : paused ? "Paused" : scanning ? "Scanning" : i == 1 ? "Uploading" : "Queued";
      div(context, mk(root.ent(), 100 + static_cast<int>(i)), box(scale, 122, y + 7, 16, 12)
          .with_on_draw_fg([](RectangleType r) { draw_folder_icon(r); }));
      text(110 + static_cast<int>(i), folders[i].name, 146, y, 132, 24, 18, ink, TextAlignment::Left, "bk_name_" + std::to_string(i));
      div(context, mk(root.ent(), 120 + static_cast<int>(i)), box(scale, 290, y + 10, 158, 7)
          .with_on_draw_fg([value = folder_now[i], done](RectangleType r) { draw_track(r, value, done ? afterhours::Color{72, 141, 101, 255} : afterhours::Color{219, 100, 88, 255}); })
          .with_debug_name("bk_bar_" + std::to_string(i)));
      text(130 + static_cast<int>(i), fmt::format("{:.0f}%", folder_now[i] * 100), 457, y, 58, 24, 17, mid, TextAlignment::Right, "bk_pct_" + std::to_string(i));
      text(150 + static_cast<int>(i), state, 547, y, 105, 24, 17, done ? afterhours::Color{45, 115, 77, 255} : mid, TextAlignment::Left, "bk_state_" + std::to_string(i));
      if (done) div(context, mk(root.ent(), 170 + static_cast<int>(i)), box(scale, 529, y + 6, 12, 12)
          .with_on_draw_fg([](RectangleType r) { afterhours::draw_line_ex({r.x, r.y + r.height / 2}, {r.x + r.width / 3, r.y + r.height}, 2, {45, 115, 77, 255}); afterhours::draw_line_ex({r.x + r.width / 3, r.y + r.height}, {r.x + r.width, r.y}, 2, {45, 115, 77, 255}); }));
      text(140 + static_cast<int>(i), fmt::format("{:.1f} GB", folders[i].size_gb), 660, y, 94, 24, 17, ink, TextAlignment::Right, "bk_size_" + std::to_string(i));
    }
    text(160, "Last completed backup: 2 hours ago", 146, 614, 606, 27, 18, {45, 115, 77, 255}, TextAlignment::Left, "bk_last_label");
    div(context, mk(root.ent(), 161), box(scale, 124, 622, 14, 12)
        .with_on_draw_fg([](RectangleType r) { afterhours::draw_line_ex({r.x, r.y + 5}, {r.x + 5, r.y + 11}, 2, {45, 115, 77, 255}); afterhours::draw_line_ex({r.x + 5, r.y + 11}, {r.x + 14, r.y}, 2, {45, 115, 77, 255}); }));

    text(200, "Backup settings", 815, 194, 347, 32, 25, ink, TextAlignment::Left, "bk_settings_title");
    const auto selector = [&](int id, const std::vector<std::string> &options, size_t &index,
                              float y, const std::string &name, const std::string &status_prefix) {
      const size_t previous = index;
      auto result = dropdown(context, mk(root.ent(), id), options, index,
          box(scale, 986, y, 176, 36)
              .with_font("AtkinsonMock", pixels(18 * scale))
              .with_custom_text_color(ink).with_custom_background({255, 255, 255, 255})
              .with_border({168, 179, 189, 255}, scale).with_alignment(TextAlignment::Left)
              .with_text_inset(10 * scale, 0).with_dropdown_indicators("", "")
              .with_render_layer(10).with_debug_name(name));
      if (index != previous) action_status = status_prefix + options[index];
      const bool expanded = result.ent().get<HasDropdownState>().on;
      for (const auto child_id : result.cmp().children) {
        auto child = UICollectionHolder::getEntityForID(child_id);
        if (!child || !child.asE().has<HasLabel>()) continue;
        auto &trigger = child.asE();
        trigger.addComponentIfMissing<UIComponentDebug>(name + "_trigger").set(name + "_trigger");
        trigger.get<HasLabel>().text_x_offset = 10 * scale;
        trigger.addComponentIfMissing<HasOnDraw>().fg = [scale, mid, expanded](RectangleType r) {
          const float x = r.x + r.width - 21 * scale;
          const float cy = r.y + r.height / 2;
          const float direction = expanded ? -1.f : 1.f;
          afterhours::draw_line_ex({x, cy - 3 * scale * direction}, {x + 5 * scale, cy + 2 * scale * direction}, 2 * scale, mid);
          afterhours::draw_line_ex({x + 5 * scale, cy + 2 * scale * direction}, {x + 10 * scale, cy - 3 * scale * direction}, 2 * scale, mid);
        };
      }
    };
    text(201, "Run backup", 815, 240, 166, 30, 18, ink, TextAlignment::Left, "bk_run_label");
    selector(202, schedules, schedule_idx, 237, "bk_schedule", "Run backup: ");
    text(203, "Keep file versions", 815, 284, 166, 30, 18, ink, TextAlignment::Left, "bk_keep_label");
    selector(204, retentions, retention_idx, 281, "bk_retention", "Keep file versions: ");
    text(205, "Bandwidth limit", 815, 327, 232, 28, 19, ink, TextAlignment::Left, "bk_bandwidth_label");
    text(206, fmt::format("{:.0f}%", bandwidth * 100), 1072, 327, 90, 28, 19, red, TextAlignment::Right, "bk_bandwidth_pct");
    auto bandwidth_slider = slider(context, mk(root.ent(), 207), bandwidth,
        box(scale, 826, 359, 326, 32).with_custom_background({0, 0, 0, 0})
            .with_on_draw_fg([value = bandwidth, scale](RectangleType r) {
              const float width = std::max(0.f, r.width - 6.f);
              const float cy = r.y + r.height / 2;
              const float cx = r.x + width * value;
              afterhours::draw_rectangle({r.x, cy - 3 * scale, width, 6 * scale}, {179, 189, 198, 255});
              afterhours::draw_rectangle({r.x, cy - 3 * scale, width * value, 6 * scale}, {219, 100, 88, 255});
              afterhours::draw_circle(static_cast<int>(cx), static_cast<int>(cy), 11 * scale, {130, 60, 51, 255});
              afterhours::draw_circle(static_cast<int>(cx), static_cast<int>(cy), 9 * scale, {255, 255, 255, 255});
              afterhours::draw_circle(static_cast<int>(cx), static_cast<int>(cy), 3 * scale, {219, 100, 88, 255});
            }).with_debug_name("bk_bandwidth"));
    for (const auto id : bandwidth_slider.cmp().children) {
      auto &child = UICollectionHolder::getEntityForIDEnforce(id);
      if (child.has<HasSliderState>()) child.get<UIComponentDebug>().set("bk_bandwidth_input");
    }
    text(208, "0 MB/s", 815, 391, 130, 24, 16, mid);
    text(209, "40 MB/s", 1032, 391, 130, 24, 16, mid, TextAlignment::Right);
    text(210, fmt::format("Cap: {:.1f} MB/s of 40 MB/s", bandwidth * 40), 815, 415, 347, 26, 18, ink, TextAlignment::Left, "bk_readout");
    const auto check = [&](int id, const std::string &label, float y, bool &value,
                           const std::string &name, const std::string &box_name) {
      auto result = button(context, mk(root.ent(), id), box(scale, 815, y, 347, 36)
          .with_label(label).with_font("AtkinsonMock", pixels(18 * scale))
          .with_custom_text_color(ink).with_alignment(TextAlignment::Left)
          .with_text_inset(34 * scale, 0).with_debug_name(name));
      result.ent().get<HasLabel>().text_x_offset = 34 * scale;
      if (result) {
        value = !value;
        action_status = label + (value ? ": On" : ": Off");
      }
      div(context, mk(root.ent(), id + 1), box(scale, 817, y + 8, 20, 20)
          .with_ignore_pointer_events().with_debug_name(box_name)
          .with_on_draw_fg([value, scale](RectangleType r) {
            afterhours::draw_rectangle(r, value ? afterhours::Color{185, 66, 54, 255} : afterhours::Color{255, 255, 255, 255});
            raylib::DrawRectangleLinesEx(r, 1.5f * scale, {124, 137, 149, 255});
            if (!value) return;
            afterhours::draw_line_ex({r.x + 4 * scale, r.y + 10 * scale}, {r.x + 8 * scale, r.y + 14 * scale}, 2 * scale, {255, 255, 255, 255});
            afterhours::draw_line_ex({r.x + 8 * scale, r.y + 14 * scale}, {r.x + 16 * scale, r.y + 5 * scale}, 2 * scale, {255, 255, 255, 255});
          }));
    };
    check(211, "Pause on battery power", 447, pause_on_battery, "bk_battery_label", "bk_battery_box");
    check(214, "Include external drives", 485, backup_externals, "bk_external_label", "bk_external_box");
    panel(220, 815, 532, 347, 1, line);
    text(221, "ACCOUNT STORAGE", 815, 541, 347, 25, 16, mid, TextAlignment::Left, "bk_account_hdr", "AtkinsonMockBold");
    text(222, "787 GB of 2 TB · 39.4% used", 815, 568, 347, 27, 20, ink, TextAlignment::Left, "bk_plan");
    div(context, mk(root.ent(), 223), box(scale, 815, 601, 347, 8)
        .with_on_draw_fg([](RectangleType r) { draw_storage(r); }).with_debug_name("bk_storage"));
    if (button(context, mk(root.ent(), 224), box(scale, 815, 614, 240, 30)
        .with_label("Manage account  >").with_font("AtkinsonMock", pixels(18 * scale))
        .with_custom_text_color(red).with_alignment(TextAlignment::Left)
        .with_debug_name("bk_manage"))) {
      detail = Detail::Account;
      detail_open = true;
      action_status = "Account preview opened";
    }
    panel(240, 90, 650, 1100, 37, {250, 250, 250, 255}, "bk_footer");
    panel(241, 90, 650, 1100, 1, line);
    div(context, mk(root.ent(), 242), box(scale, 111, 659, 16, 18)
        .with_on_draw_fg([scale](RectangleType r) {
          afterhours::draw_rectangle({r.x + 2 * scale, r.y + 7 * scale, 12 * scale, 11 * scale}, {66, 112, 86, 255});
          raylib::DrawRectangleLinesEx({r.x + 4 * scale, r.y, 8 * scale, 11 * scale}, 2 * scale, {66, 112, 86, 255});
        }));
    text(243, "Encrypted backup preview · no files are uploaded", 138, 656, 540, 27, 16, mid, TextAlignment::Left, "bk_footer_text");
    if (button(context, mk(root.ent(), 244), box(scale, 1144, 653, 34, 31)
        .with_label("?").with_font("AtkinsonMock", pixels(23 * scale))
        .with_custom_text_color(ink).with_custom_background({229, 236, 241, 255})
        .with_corner_radius(5 * scale).with_alignment(TextAlignment::Center).with_debug_name("bk_help"))) {
      detail = Detail::Help;
      detail_open = true;
      action_status = "Backup help opened";
    }
    text(245, action_status, 694, 656, 432, 27, 16, mid, TextAlignment::Right, "bk_action_status");
    std::string detail_title, detail_text;
    if (detail == Detail::Restore) {
      detail_title = "Restore files preview";
      detail_text = "Documents and Projects are complete in this demo. Restoring a file requires a connected backup service.";
    } else if (detail == Detail::Account) {
      detail_title = "Personal Backup plan";
      detail_text = "All backup versions use 787 GB of a 2,000 GB account. The folder table shows this computer's current upload only. Account management is unavailable in this local demo.";
    } else {
      detail_title = "Backup help";
      detail_text = "Pause stops the preview. Rescan replays folder discovery. The bandwidth slider sets the simulated transfer cap; no files leave this application.";
    }
    if (auto dialog = afterhours::modal(context, mk(entity, 900), detail_open,
          afterhours::ModalConfig{}.with_size(pixels(620 * scale), pixels(310 * scale)).with_title(detail_title))) {
      dialog_presentation::style_title(dialog.ent());
      dialog.ent().get<UIComponentDebug>().set("bk_detail_panel");
      div(context, mk(dialog.ent(), 0), ComponentConfig{}
          .with_size({percent(1), pixels(150 * scale)}).with_label(detail_text)
          .with_font("AtkinsonMock", pixels(21 * scale)).with_custom_text_color(ink)
          .with_text_overflow(TextOverflow::Wrap).with_render_layer(1001));
      if (button(context, mk(dialog.ent(), 1), ComponentConfig{}
          .with_size({pixels(140 * scale), pixels(40 * scale)}).with_label("Close")
          .with_font("AtkinsonMock", pixels(20 * scale)).with_custom_text_color(ink)
          .with_custom_background({229, 236, 241, 255}).with_render_layer(1001)
          .with_debug_name("bk_detail_close"))) detail_open = false;
    }
  }
};

REGISTER_EXAMPLE_SCREEN(offsite_backup, "App Mockups",
                        "Backup client: desktop backup window with live controls",
                        OffsiteBackupApp)
