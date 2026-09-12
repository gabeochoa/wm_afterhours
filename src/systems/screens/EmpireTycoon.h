#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <string>
#include <vector>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct EmpireTycoonScreen : ScreenSystem<UIContext<InputAction>> {
  int64_t cash = 1250980;
  size_t selected_tab = 0;
  float happiness_pct = 0.85f;
  float resources_pct = 0.60f;
  float milestone_pct = 0.65f;
  bool texture_loaded = false;
  bool new_project_started = false;
  raylib::Texture2D park_texture{};
  std::array<raylib::Texture2D, 9> icons{};
  std::string chat_status = "GlobalChat: New area unlocked!";

  struct Production {
    std::string name;
    int rate;
  };

  struct Project {
    std::string name;
    int progress;
    std::string note;
  };

  std::vector<Production> production = {
      {"Toys", 5200}, {"Snacks", 3800}, {"Gadgets", 3100}};
  std::vector<Project> projects = {
      {"New Rollercoaster", 80, "Expand the fun"},
      {"Develop Robot Mascot", 10, "3,100/min"},
  };

  const afterhours::Color white{255, 253, 244, 255};
  const afterhours::Color ink{49, 69, 97, 255};
  const afterhours::Color stroke{48, 72, 103, 255};
  const afterhours::Color panel_blue{105, 196, 237, 255};
  const afterhours::Color panel_edge{52, 83, 116, 255};
  const afterhours::Color card_bg{244, 252, 255, 255};
  const afterhours::Color yellow{255, 220, 78, 255};
  const afterhours::Color yellow_dark{244, 166, 46, 255};
  const afterhours::Color green{118, 179, 211, 255};

  std::array<std::string, 4> tabs = {"Rides", "Food Stalls", "Upgrades",
                                     "Finance"};
  std::array<std::string, 3> tools = {"Shop", "Settings", "Leaderboard"};

  void load_texture_if_needed() {
    if (texture_loaded)
      return;
    texture_loaded = true;
    park_texture = raylib::LoadTexture(
        afterhours::files::get_resource_path("images", "empire_tycoon/park.png")
            .string()
            .c_str());
    const std::array<const char *, 9> names{
        "coin_star", "rides",    "food",        "upgrades", "finance",
        "shop",      "settings", "leaderboard", "arrow"};
    for (size_t i = 0; i < names.size(); ++i) {
      icons[i] = raylib::LoadTexture(
          afterhours::files::get_resource_path(
              "images", std::string("empire_tycoon/") + names[i] + ".png")
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

  ComponentConfig box(float scale, float x, float y, float w, float h) const {
    return ComponentConfig{}
        .with_size({pixels(w * scale), pixels(h * scale)})
        .with_absolute_position(x * scale, y * scale)
        .with_background(Theme::Usage::None);
  }

  std::string format_money(int64_t amt) const {
    std::string s = std::to_string(amt);
    std::string result;
    int count = 0;
    for (int i = static_cast<int>(s.size()) - 1; i >= 0; --i) {
      if (count > 0 && count % 3 == 0)
        result = "," + result;
      result = s[static_cast<size_t>(i)] + result;
      ++count;
    }
    return "$" + result;
  }

  static void draw_coin(RectangleType r) {
    const float cx = r.x + r.width * .5f;
    const float cy = r.y + r.height * .5f;
    afterhours::draw_circle(static_cast<int>(cx), static_cast<int>(cy),
                            r.width * .48f,
                            afterhours::Color{171, 116, 44, 255});
    afterhours::draw_circle(static_cast<int>(cx), static_cast<int>(cy),
                            r.width * .41f,
                            afterhours::Color{255, 206, 86, 255});
    afterhours::draw_poly({cx, cy - r.height * .03f}, 5, r.width * .25f, -18.f,
                          afterhours::Color{255, 232, 131, 255});
  }

  static void draw_gauge(RectangleType r, float pct, afterhours::Color fill) {
    const float cx = r.x + r.width * .5f;
    const float cy = r.y + r.height * .93f;
    const float radius = r.width * .44f;
    afterhours::draw_circle_sector({cx, cy}, radius, 180.f, 360.f, 48,
                                   afterhours::Color{190, 228, 245, 190});
    afterhours::draw_circle_sector({cx, cy}, radius, 180.f, 180.f + 180.f * pct,
                                   48, fill);
    afterhours::draw_circle_sector_lines({cx, cy}, radius, 180.f, 360.f, 48,
                                         afterhours::Color{52, 77, 112, 255});
    afterhours::draw_circle_sector({cx, cy}, radius * .86f, 180.f, 360.f, 48,
                                   afterhours::Color{140, 189, 216, 255});
    float face = radius * .27f;
    float fy = cy - face;
    afterhours::draw_circle(static_cast<int>(cx), static_cast<int>(fy), face,
                            fill);
    afterhours::draw_circle_lines(static_cast<int>(cx), static_cast<int>(fy),
                                  face, afterhours::Color{52, 77, 112, 255});
    afterhours::draw_circle(static_cast<int>(cx - face * .35f),
                            static_cast<int>(fy - face * .2f), face * .09f,
                            afterhours::Color{52, 77, 112, 255});
    afterhours::draw_circle(static_cast<int>(cx + face * .35f),
                            static_cast<int>(fy - face * .2f), face * .09f,
                            afterhours::Color{52, 77, 112, 255});
    afterhours::draw_circle_sector_lines({cx, fy}, face * .55f, 30.f, 150.f, 14,
                                         afterhours::Color{52, 77, 112, 255});
    for (int i = 0; i <= 8; ++i) {
      const float a =
          (180.f + 180.f * static_cast<float>(i) / 8.f) * 3.14159f / 180.f;
      const float x1 = cx + std::cos(a) * radius * .82f;
      const float y1 = cy + std::sin(a) * radius * .82f;
      const float x2 = cx + std::cos(a) * radius;
      const float y2 = cy + std::sin(a) * radius;
      afterhours::draw_line_ex({x1, y1}, {x2, y2}, 2.f,
                               afterhours::Color{52, 77, 112, 185});
    }
  }

  static void outline(RectangleType r, float radius, float thickness,
                      afterhours::Color color) {
    afterhours::draw_rectangle_rounded_lines_ex(
        r, 2.f * radius / std::min(r.width, r.height), 16, thickness, color);
  }

  static void draw_panel(RectangleType r) {
    afterhours::draw_rectangle_rounded(r, .23f, 16,
                                       afterhours::Color{104, 197, 238, 255},
                                       std::bitset<4>().set());
    afterhours::draw_rectangle_rounded_lines_ex(
        r, .23f, 16, 3.f, afterhours::Color{52, 83, 116, 255});
    afterhours::draw_rectangle_rounded_lines_ex(
        {r.x + 7.f, r.y + 7.f, r.width - 14.f, r.height - 14.f}, .20f, 16, 3.f,
        afterhours::Color{200, 245, 255, 220});
  }

  void speed_up() {
    cash += 25000;
    happiness_pct = std::min(1.0f, happiness_pct + 0.02f);
    resources_pct = std::min(1.0f, resources_pct + 0.04f);
    milestone_pct = std::min(1.0f, milestone_pct + 0.05f);
    for (auto &item : production)
      item.rate += 400;
    for (auto &project : projects)
      project.progress = std::min(100, project.progress + 5);
    chat_status = "GlobalChat: Production boosted!";
  }

  void prioritize() {
    if (!projects.empty())
      projects.front().progress = std::min(100, projects.front().progress + 10);
    cash = std::max<int64_t>(0, cash - 5000);
    chat_status = "New Rollercoaster prioritized";
  }

  void new_project() {
    if (!new_project_started) {
      new_project_started = true;
      projects.back() = {"Sky Garden", 5, "New project started"};
    } else {
      projects.back().progress = std::min(100, projects.back().progress + 5);
    }
    cash = std::max<int64_t>(0, cash - 12000);
    chat_status = "New project started";
  }

  void select_tab(size_t index) {
    selected_tab = index % tabs.size();
    chat_status = tabs[selected_tab] + " department selected";
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
      select_tab(selected_tab + 1);
    if (context.pressed(InputAction::WidgetUp))
      select_tab(selected_tab + tabs.size() - 1);

    Theme theme;
    theme.font = ink;
    theme.darkfont = white;
    theme.font_muted = afterhours::Color{82, 115, 135, 255};
    theme.background = afterhours::Color{185, 226, 242, 255};
    theme.surface = card_bg;
    theme.primary = panel_blue;
    theme.secondary = panel_edge;
    theme.accent = yellow;
    theme.roundness = .14f;
    theme.segments = 14;
    context.set_theme(theme);
    context.scaling_mode = ScalingMode::Proportional;
    UIStylingDefaults::get().set_default_font("FredokaMockBold", h720(21.f));

    auto root =
        div(context, mk(entity, 0),
            box(scale, 0, 0, 1280, 720)
                .with_on_draw_bg([texture = park_texture](RectangleType r) {
                  afterhours::draw_rectangle(
                      r, afterhours::Color{185, 226, 242, 255});
                  paint(texture, r);
                })
                .with_debug_name("empire_root"));

    auto icon = [&](int id, size_t index, float x, float y, float w, float h) {
      const auto texture = icons[index];
      sprite(context, mk(root.ent(), id), texture,
             {0, 0, (float)texture.width, (float)texture.height},
             box(scale, x, y, w, h).with_ignore_pointer_events());
    };

    div(context, mk(root.ent(), 10),
        box(scale, 88, 23, 330, 105)
            .with_label("DREAM")
            .with_font("FredokaMockBold", h720(100.f))
            .with_custom_text_color(white)
            .with_text_stroke(stroke, 2.4f)
            .with_text_shadow(afterhours::Color{101, 185, 229, 255}, 0.f,
                              7.f * scale)
            .with_letter_spacing(-3.f * scale)
            .with_debug_name("empire_title_dream"));
    div(context, mk(root.ent(), 11),
        box(scale, 89, 113, 320, 56)
            .with_label("INCORPORATED")
            .with_font("FredokaMockBold", h720(49.f))
            .with_custom_text_color(white)
            .with_text_stroke(stroke, 1.9f)
            .with_text_shadow(afterhours::Color{101, 185, 229, 255}, 0.f,
                              6.f * scale)
            .with_letter_spacing(-2.f * scale)
            .with_debug_name("empire_title_incorporated"));

    div(context, mk(root.ent(), 20),
        box(scale, 777, 33, 402, 78)
            .with_custom_background(afterhours::Color{128, 211, 246, 245})
            .with_border(stroke, 3.f)
            .with_corner_radius(39.f * scale)
            .with_on_draw_fg([=, this](RectangleType r) {
              outline(r, 39.f * scale, 3.f * scale, stroke);
            })
            .with_debug_name("empire_cash_pill"));
    div(context, mk(root.ent(), 21),
        box(scale, 788, 40, 65, 65)
            .with_on_draw_fg([](RectangleType r) { draw_coin(r); })
            .with_ignore_pointer_events()
            .with_debug_name("empire_coin"));
    icon(700, 0, 794, 45, 49, 49);

    div(context, mk(root.ent(), 22),
        box(scale, 858, 43, 285, 62)
            .with_label(format_money(cash))
            .with_font("FredokaMockBold", h720(56.f))
            .with_custom_text_color(white)
            .with_text_stroke(stroke, 1.9f)
            .with_alignment(TextAlignment::Center)
            .with_letter_spacing(-1.3f * scale)
            .with_debug_name("empire_cash"));

    struct Gauge {
      const char *label;
      const char *icon;
      float pct;
      afterhours::Color fill;
      float x;
    };
    std::array<Gauge, 2> gauges = {
        {{"Happiness", "", happiness_pct, afterhours::Color{164, 217, 110, 255},
          780.f},
         {"Resources", "", resources_pct, afterhours::Color{117, 210, 238, 255},
          1012.f}}};
    for (size_t i = 0; i < gauges.size(); ++i) {
      const auto &g = gauges[i];
      div(context, mk(root.ent(), 30 + static_cast<int>(i) * 10),
          box(scale, g.x, 135, 143, 62)
              .with_on_draw_fg([pct = g.pct, fill = g.fill](RectangleType r) {
                draw_gauge(r, pct, fill);
              })
              .with_ignore_pointer_events()
              .with_debug_name("empire_gauge_" + std::to_string(i)));
      div(context, mk(root.ent(), 31 + static_cast<int>(i) * 10),
          box(scale, g.x - 34, 209, 186, 34)
              .with_label(g.label + std::string(" ") +
                          std::to_string(static_cast<int>(g.pct * 100.f)) + "%")
              .with_custom_background(afterhours::Color{184, 241, 255, 230})
              .with_border(stroke, 2.f)
              .with_corner_radius(17.f * scale)
              .with_font("FredokaMockBold", h720(16.f))
              .with_custom_text_color(white)
              .with_text_stroke(stroke, 1.f)
              .with_alignment(TextAlignment::Center)
              .with_debug_name("empire_gauge_label_" + std::to_string(i)));
    }

    div(context, mk(root.ent(), 90),
        box(scale, 86, 246, 138, 333)
            .with_custom_background(afterhours::Color{191, 238, 255, 245})
            .with_border(stroke, 3.f)
            .with_corner_radius(24.f * scale)
            .with_on_draw_fg([=, this](RectangleType r) {
              outline(r, 24.f * scale, 3.f * scale, stroke);
            })
            .with_debug_name("empire_tabs_panel"));

    const std::array<afterhours::Color, 4> tab_colors = {{{114, 201, 237, 255},
                                                          {161, 216, 91, 255},
                                                          {233, 151, 194, 255},
                                                          {255, 195, 85, 255}}};
    for (size_t i = 0; i < tabs.size(); ++i) {
      const float y = 259.f + static_cast<float>(i) * 82.f;
      const bool active = i == selected_tab;
      if (button(context, mk(root.ent(), 100 + static_cast<int>(i)),
                 box(scale, 98, y, 112, 68)
                     .with_label("")
                     .with_custom_background(tab_colors[i])
                     .with_border(active ? afterhours::Color{255, 248, 181, 255}
                                         : afterhours::Color{72, 107, 131, 255},
                                  active ? 4.f : 2.f)
                     .with_corner_radius(11.f * scale)
                     .with_font("FredokaMockBold", h720(18.f))
                     .with_custom_text_color(white)
                     .with_text_stroke(stroke, 1.3f)
                     .with_alignment(TextAlignment::Center)
                     .with_on_draw_fg([=, this](RectangleType r) {
                       outline(r, 11.f * scale, 2.f * scale, stroke);
                     })
                     .with_debug_name("empire_tab_" + std::to_string(i)))) {
        select_tab(i);
      }

      icon(710 + static_cast<int>(i), i + 1, 132, y + 7, 42, 38);
      div(context, mk(root.ent(), 720 + static_cast<int>(i)),
          box(scale, 98, y + 42, 112, 28)
              .with_label(tabs[i])
              .with_font("FredokaMockBold", h720(21.f))
              .with_custom_text_color(white)
              .with_text_stroke(stroke, .7f * scale)
              .with_alignment(TextAlignment::Center));
      if (i == 2) {
        div(context, mk(root.ent(), 130),
            box(scale, 196, 411, 28, 28)
                .with_label("!")
                .with_custom_background(afterhours::Color{242, 108, 105, 255})
                .with_border(afterhours::Color{118, 62, 64, 255}, 2.f)
                .with_corner_radius(14.f * scale)
                .with_font("FredokaMockBold", h720(24.f))
                .with_custom_text_color(white)
                .with_alignment(TextAlignment::Center)
                .with_debug_name("empire_upgrade_badge"));
      }
    }

    div(context, mk(root.ent(), 200),
        box(scale, 254, 257, 942, 342)
            .with_on_draw_bg([](RectangleType r) { draw_panel(r); })
            .with_debug_name("empire_dashboard"));

    auto heading = [&](int id, float x, const char *label) {
      div(context, mk(root.ent(), id),
          box(scale, x, 287, 330, 35)
              .with_label(label)
              .with_font("FredokaMockBold", h720(27.f))
              .with_custom_text_color(white)
              .with_text_stroke(stroke, 1.f)
              .with_alignment(TextAlignment::Center));
    };
    heading(210, 294, "Production Overview");
    heading(211, 820, "Current Projects");

    div(context, mk(root.ent(), 220),
        box(scale, 282, 330, 430, 170)
            .with_custom_background(card_bg)
            .with_border(afterhours::Color{93, 153, 184, 255}, 2.f)
            .with_corner_radius(16.f * scale)
            .with_on_draw_fg([=, this](RectangleType r) {
              outline(r, 16.f * scale, 2.f * scale, panel_edge);
            })
            .with_debug_name("empire_production_list"));
    for (size_t i = 0; i < production.size(); ++i) {
      const float y = 349.f + static_cast<float>(i) * 43.f;
      div(context, mk(root.ent(), 221 + static_cast<int>(i) * 3),
          box(scale, 301, y, 332, 38)
              .with_label(production[i].name + ": " +
                          std::to_string(production[i].rate / 1000) + "," +
                          std::to_string((production[i].rate % 1000) / 100) +
                          "00/min")
              .with_font("FredokaMockBold", h720(21.f))
              .with_custom_text_color(ink)
              .with_alignment(TextAlignment::Left)
              .with_debug_name("empire_prod_" + std::to_string(i)));
      icon(222 + static_cast<int>(i) * 3, 8, 670, y + 3, 21, 24);
      div(context, mk(root.ent(), 223 + static_cast<int>(i) * 3),
          box(scale, 301, y + 35, 390, 2)
              .with_custom_background(afterhours::Color{220, 238, 245, 255}));
    }
    div(context, mk(root.ent(), 240),
        box(scale, 408, 481, 190, 19)
            .with_label("All systems running smoothly")
            .with_font("FredokaMockBold", h720(12.f))
            .with_custom_text_color(afterhours::Color{121, 147, 156, 255})
            .with_alignment(TextAlignment::Center));

    div(context, mk(root.ent(), 250),
        box(scale, 738, 330, 430, 170)
            .with_custom_background(card_bg)
            .with_border(afterhours::Color{93, 153, 184, 255}, 2.f)
            .with_corner_radius(16.f * scale)
            .with_on_draw_fg([=, this](RectangleType r) {
              outline(r, 16.f * scale, 2.f * scale, panel_edge);
            })
            .with_debug_name("empire_project_list"));
    for (size_t i = 0; i < projects.size(); ++i) {
      const auto &p = projects[i];
      const float y = 340.f + static_cast<float>(i) * 78.f;
      div(context, mk(root.ent(), 260 + static_cast<int>(i) * 6),
          box(scale, 751, y, 405, 70)
              .with_border(afterhours::Color{116, 164, 191, 255}, 1.5f)
              .with_corner_radius(11.f * scale)
              .with_debug_name("empire_project_" + std::to_string(i)));
      div(context, mk(root.ent(), 261 + static_cast<int>(i) * 6),
          box(scale, 760, y + 5, 330, 29)
              .with_label(p.name)
              .with_custom_background(afterhours::Color{162, 223, 247, 255})
              .with_font("FredokaMockBold", h720(19.f))
              .with_custom_text_color(white)
              .with_text_stroke(stroke, 1.3f)
              .with_debug_name("empire_project_title_" + std::to_string(i)));
      div(context, mk(root.ent(), 800 + static_cast<int>(i)),
          box(scale, 1090, y + 5, 58, 29)
              .with_label(std::to_string(p.progress) + "%")
              .with_font("FredokaMockBold", h720(19.f))
              .with_custom_text_color(white)
              .with_text_stroke(stroke, 1.f)
              .with_alignment(TextAlignment::Right));
      div(context, mk(root.ent(), 262 + static_cast<int>(i) * 6),
          box(scale, 760, y + 31, 390, 5)
              .with_custom_background(afterhours::Color{219, 241, 250, 255}));
      div(context, mk(root.ent(), 263 + static_cast<int>(i) * 6),
          box(scale, 760, y + 31,
              390.f * static_cast<float>(p.progress) / 100.f, 5)
              .with_custom_background(green)
              .with_debug_name("empire_project_progress_" + std::to_string(i)));
      div(context, mk(root.ent(), 264 + static_cast<int>(i) * 6),
          box(scale, 761, y + 39, 330, 24)
              .with_label(p.note)
              .with_font("FredokaMockBold", h720(17.f))
              .with_custom_text_color(ink)
              .with_alignment(TextAlignment::Left));
      icon(265 + static_cast<int>(i) * 6, 8, 1120, y + 40, 21, 24);
    }

    const std::array<const char *, 3> actions = {
        {"Speed Up", "Prioritize", "New Project"}};
    for (size_t i = 0; i < actions.size(); ++i) {
      const float x = 282.f + static_cast<float>(i) * 304.f;
      if (button(context, mk(root.ent(), 300 + static_cast<int>(i)),
                 box(scale, x, 517, 278, 61)
                     .with_label(actions[i])
                     .with_custom_background(yellow)
                     .with_border(afterhours::Color{70, 89, 112, 255}, 3.f)
                     .with_corner_radius(15.f * scale)
                     .with_font("FredokaMockBold", h720(29.f))
                     .with_custom_text_color(white)
                     .with_text_stroke(stroke, 1.f)
                     .with_alignment(TextAlignment::Center)
                     .with_on_draw_fg([=, this](RectangleType r) {
                       outline(r, 15.f * scale, 3.f * scale, stroke);
                     })
                     .with_debug_name("empire_action_" + std::to_string(i)))) {
        if (i == 0)
          speed_up();
        else if (i == 1)
          prioritize();
        else
          new_project();
      }
    }

    div(context, mk(root.ent(), 400),
        box(scale, 91, 616, 250, 75)
            .with_custom_background(card_bg)
            .with_border(stroke, 2.f)
            .with_corner_radius(16.f * scale)
            .with_on_draw_fg([=, this](RectangleType r) {
              outline(r, 16.f * scale, 2.f * scale, stroke);
            })
            .with_debug_name("empire_chat"));
    div(context, mk(root.ent(), 401),
        box(scale, 110, 631, 210, 24)
            .with_label(chat_status)
            .with_font("Archivo@bold", h720(17.f))
            .with_custom_text_color(ink)
            .with_alignment(TextAlignment::Left)
            .with_debug_name("empire_chat_status"));
    div(context, mk(root.ent(), 402),
        box(scale, 110, 659, 210, 24)
            .with_label("DevTeam: Update v2.0 is live")
            .with_font("Archivo@bold", h720(17.f))
            .with_custom_text_color(ink)
            .with_alignment(TextAlignment::Left));

    div(context, mk(root.ent(), 410),
        box(scale, 503, 625, 305, 28)
            .with_label("Next Milestone")
            .with_font("FredokaMockBold", h720(25.f))
            .with_custom_text_color(white)
            .with_text_stroke(stroke, 1.f)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("empire_milestone_title"));
    div(context, mk(root.ent(), 411),
        box(scale, 498, 658, 316, 36)
            .with_custom_background(afterhours::Color{96, 88, 131, 255})
            .with_border(stroke, 3.f)
            .with_corner_radius(18.f * scale)
            .with_on_draw_fg([=, this](RectangleType r) {
              outline(r, 18.f * scale, 3.f * scale, stroke);
            })
            .with_debug_name("empire_milestone_bar"));
    div(context, mk(root.ent(), 412),
        box(scale, 506, 665, 300.f * milestone_pct, 22)
            .with_custom_background(yellow)
            .with_corner_radius(11.f * scale)
            .with_debug_name("empire_milestone_fill"));
    div(context, mk(root.ent(), 413),
        box(scale, 620, 665, 70, 22)
            .with_label(
                std::to_string(static_cast<int>(milestone_pct * 100.f)) + "%")
            .with_font("FredokaMockBold", h720(15.f))
            .with_custom_text_color(ink)
            .with_alignment(TextAlignment::Center));

    for (size_t i = 0; i < tools.size(); ++i) {
      const float x = 950.f + static_cast<float>(i) * 92.f;
      if (button(
              context, mk(root.ent(), 500 + static_cast<int>(i)),
              box(scale, x, 620, 49, 49)
                  .with_label("")
                  .with_custom_background(afterhours::Color{245, 194, 95, 255})
                  .with_border(stroke, 2.f)
                  .with_corner_radius(24.5f * scale)
                  .with_font("FredokaMockBold", h720(26.f))
                  .with_custom_text_color(white)
                  .with_text_stroke(stroke, 1.f)
                  .with_alignment(TextAlignment::Center)
                  .with_debug_name("empire_tool_" + std::to_string(i)))) {
        chat_status = tools[i] + " opened";
      }
      icon(730 + static_cast<int>(i), i + 5, x + 6, 626, 37, 37);

      div(context, mk(root.ent(), 510 + static_cast<int>(i)),
          box(scale, x - 20, 676, 90, 24)
              .with_label(tools[i])
              .with_font("FredokaMockBold", h720(15.f))
              .with_custom_text_color(white)
              .with_text_stroke(stroke, 1.3f)
              .with_alignment(TextAlignment::Center));
    }
  }
};

REGISTER_EXAMPLE_SCREEN(empire_tycoon, "Game Mockups",
                        "Bright cartoon tycoon game", EmpireTycoonScreen)
