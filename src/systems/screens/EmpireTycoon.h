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
  size_t selected_project = 0;
  bool milestone_rewarded = false;
  float happiness_pct = 0.85f;
  float resources_pct = 0.60f;
  float milestone_pct = 0.65f;
  bool texture_loaded = false;
  bool new_project_started = false;
  raylib::Texture2D park_texture{};
  std::array<raylib::Texture2D, 9> icons{};
  std::string chat_status = "New area unlocked!";

  struct Production {
    std::string name;
    int rate;
  };

  struct Project {
    std::string name;
    int progress;
  };

  std::vector<Production> production = {
      {"Toys", 5200}, {"Snacks", 3800}, {"Gadgets", 3100}};
  std::vector<Project> projects = {
      {"New Rollercoaster", 80},
      {"Develop Robot Mascot", 10},
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
  }

  static void draw_gauge(RectangleType r, float pct, afterhours::Color fill, bool resource) {
    const float cx = r.x + r.width * .5f;
    const float cy = r.y + r.height * .93f;
    const float radius = r.width * .44f;
    afterhours::draw_circle_sector({cx, cy}, radius, 180.f, 360.f, 48,
                                   afterhours::Color{74, 118, 143, 255});
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
    const afterhours::Color symbol{42, 66, 83, 255};
    if (resource) {
      for (int tooth = 0; tooth < 8; ++tooth) {
        const float angle = static_cast<float>(tooth) * 45.f;
        raylib::DrawRectanglePro({cx, fy, face * .43f, face * 1.45f},
                                 {face * .215f, face * 1.1f}, angle, symbol);
      }
      afterhours::draw_circle(static_cast<int>(cx), static_cast<int>(fy), face * .72f, symbol);
      afterhours::draw_circle(static_cast<int>(cx), static_cast<int>(fy), face * .32f, fill);
    } else {
      afterhours::draw_circle(static_cast<int>(cx - face * .35f),
                              static_cast<int>(fy - face * .2f), face * .09f, symbol);
      afterhours::draw_circle(static_cast<int>(cx + face * .35f),
                              static_cast<int>(fy - face * .2f), face * .09f, symbol);
      for (int part = 0; part < 12; ++part) {
        const float a = (30.f + part * 10.f) * 3.14159f / 180.f;
        const float b = a + 10.f * 3.14159f / 180.f;
        afterhours::draw_line_ex({cx + std::cos(a) * face * .55f, fy + std::sin(a) * face * .55f},
                                 {cx + std::cos(b) * face * .55f, fy + std::sin(b) * face * .55f},
                                 std::max(1.f, face * .1f), symbol);
      }
    }
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
    milestone_pct = static_cast<float>(std::min(100, static_cast<int>(std::lround(milestone_pct * 100)) + 5)) / 100.f;
    for (auto &item : production)
      item.rate += 400;
    for (auto &project : projects)
      project.progress = std::min(100, project.progress + 5);
    chat_status = "Production boosted!";
    if (!milestone_rewarded && milestone_pct >= 1.f) {
      milestone_rewarded = true;
      cash += 50000;
      chat_status = "Milestone complete! +$50,000 reward";
    }
  }

  void prioritize() {
    if (projects.empty() || cash < 5000) return;
    auto &project = projects[selected_project];
    if (project.progress >= 100) return;
    project.progress = std::min(100, project.progress + 10);
    cash -= 5000;
    chat_status = project.name + " prioritized";
  }

  void new_project() {
    if (cash < 12000) return;
    if (new_project_started && projects.back().progress >= 100) return;
    cash -= 12000;
    if (!new_project_started) {
      new_project_started = true;
      projects.back() = {"Sky Garden", 5};
      chat_status = "New project started";
    } else {
      projects.back().progress = std::min(100, projects.back().progress + 5);
      chat_status = "Sky Garden expanded";
    }
    selected_project = projects.size() - 1;
  }

  void select_tab(size_t index) {
    selected_tab = index % tabs.size();
    chat_status = tabs[selected_tab] + " department selected";
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    UIStylingDefaults::get().default_font_name = "FredokaMockBold";
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

    auto root =
        div(context, mk(entity, 0),
            ComponentConfig{}.with_size({pixels(screen_w), pixels(screen_h)}).with_corner_radius(0)
                .with_on_draw_bg([texture = park_texture](RectangleType r) {
                  afterhours::draw_rectangle(
                      r, afterhours::Color{185, 226, 242, 255});
                  paint(texture, r);
                })
                .with_debug_name("empire_root"));

    auto content = div(context, mk(root.ent(), 1), ComponentConfig{}
        .with_size({pixels(1280 * scale), pixels(720 * scale)})
        .with_absolute_position((screen_w - 1280 * scale) / 2, (screen_h - 720 * scale) / 2)
        .with_background(Theme::Usage::None).with_corner_radius(0));

    auto icon = [&](int id, size_t index, float x, float y, float w, float h) {
      const auto texture = icons[index];
      sprite(context, mk(content.ent(), id), texture,
             {0, 0, (float)texture.width, (float)texture.height},
             box(scale, x, y, w, h).with_ignore_pointer_events());
    };

    div(context, mk(content.ent(), 10),
        box(scale, 88, 23, 330, 105)
            .with_label("DREAM")
            .with_font("FredokaMockBold", pixels(100.f * scale))
            .with_custom_text_color(white)
            .with_text_stroke(stroke, 2.4f)
            .with_text_shadow(afterhours::Color{101, 185, 229, 255}, 0.f,
                              7.f * scale)
            .with_letter_spacing(-3.f * scale)
            .with_debug_name("empire_title_dream"));
    div(context, mk(content.ent(), 11),
        box(scale, 89, 113, 320, 56)
            .with_label("INCORPORATED")
            .with_font("FredokaMockBold", pixels(49.f * scale))
            .with_custom_text_color(white)
            .with_text_stroke(stroke, 1.9f)
            .with_text_shadow(afterhours::Color{101, 185, 229, 255}, 0.f,
                              6.f * scale)
            .with_letter_spacing(-2.f * scale)
            .with_debug_name("empire_title_incorporated"));

    div(context, mk(content.ent(), 20),
        box(scale, 777, 33, 402, 78)
            .with_custom_background(afterhours::Color{128, 211, 246, 245})
            .with_border(stroke, 3.f)
            .with_corner_radius(39.f * scale)
            .with_on_draw_fg([=, this](RectangleType r) {
              outline(r, 39.f * scale, 3.f * scale, stroke);
            })
            .with_debug_name("empire_cash_pill"));
    div(context, mk(content.ent(), 21),
        box(scale, 788, 40, 65, 65)
            .with_on_draw_fg([](RectangleType r) { draw_coin(r); })
            .with_ignore_pointer_events()
            .with_debug_name("empire_coin"));
    div(context, mk(content.ent(), 700), box(scale, 794, 45, 49, 49)
        .with_label("$").with_font("FredokaMockBold", pixels(38 * scale))
        .with_custom_text_color(ink).with_alignment(TextAlignment::Center).with_ignore_pointer_events());

    div(context, mk(content.ent(), 22),
        box(scale, 858, 43, 285, 62)
            .with_label(format_money(cash))
            .with_font("FredokaMockBold", pixels(46.f * scale))
            .with_custom_text_color(ink)
            .with_alignment(TextAlignment::Center)
            .with_letter_spacing(-1.3f * scale)
            .with_debug_name("empire_cash"));

    struct Gauge {
      const char *label;
      float pct;
      afterhours::Color fill;
      float x;
    };
    std::array<Gauge, 2> gauges = {
        {{"Happiness", happiness_pct, afterhours::Color{83, 168, 87, 255},
          780.f},
         {"Resources", resources_pct, afterhours::Color{31, 129, 180, 255},
          1012.f}}};
    for (size_t i = 0; i < gauges.size(); ++i) {
      const auto &g = gauges[i];
      div(context, mk(content.ent(), 30 + static_cast<int>(i) * 10),
          box(scale, g.x, 135, 143, 62)
              .with_on_draw_fg([pct = g.pct, fill = g.fill, resource = i == 1](RectangleType r) {
                draw_gauge(r, pct, fill, resource);
              })
              .with_ignore_pointer_events()
              .with_debug_name("empire_gauge_" + std::to_string(i)));
      div(context, mk(content.ent(), 31 + static_cast<int>(i) * 10),
          box(scale, g.x - 34, 209, 186, 34)
              .with_label(g.label + std::string(" ") +
                          std::to_string(static_cast<int>(std::lround(g.pct * 100.f))) + "%")
              .with_custom_background(afterhours::Color{184, 241, 255, 230})
              .with_border(stroke, 2.f)
              .with_corner_radius(17.f * scale)
              .with_font("FredokaMockBold", pixels(21.f * scale))
              .with_custom_text_color(ink)
              .with_alignment(TextAlignment::Center)
              .with_debug_name("empire_gauge_label_" + std::to_string(i)));
      for (int endpoint = 0; endpoint < 2; ++endpoint)
        div(context, mk(content.ent(), 32 + static_cast<int>(i) * 10 + endpoint),
            box(scale, g.x - 8 + endpoint * 124.f, 188, 38, 23)
                .with_label(endpoint == 0 ? "0" : "100").with_font("FredokaMockBold", pixels(17 * scale))
                .with_custom_text_color(ink).with_alignment(TextAlignment::Center).with_ignore_pointer_events());
    }

    div(context, mk(content.ent(), 90),
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
      if (button(context, mk(content.ent(), 100 + static_cast<int>(i)),
                 box(scale, 98, y, 112, 68)
                     .with_label("")
                     .with_custom_background(tab_colors[i])
                     .with_border(active ? afterhours::Color{255, 248, 181, 255}
                                         : afterhours::Color{72, 107, 131, 255},
                                  active ? 4.f : 2.f)
                     .with_corner_radius(11.f * scale)
                     .with_font("FredokaMockBold", pixels(18.f * scale))
                     .with_custom_text_color(white)
                     .with_text_stroke(stroke, 1.3f)
                     .with_alignment(TextAlignment::Center)
                     .with_on_draw_fg([=, this](RectangleType r) {
                       outline(r, 11.f * scale, 2.f * scale, stroke);
                       if (active) outline({r.x + 5 * scale, r.y + 5 * scale, r.width - 10 * scale, r.height - 10 * scale},
                                           7 * scale, 3 * scale, {255, 253, 220, 255});
                     })
                     .with_debug_name("empire_tab_" + std::to_string(i)))) {
        select_tab(i);
      }

      icon(710 + static_cast<int>(i), i + 1, 128, y + 3, 52, 42);
      div(context, mk(content.ent(), 720 + static_cast<int>(i)),
          box(scale, 98, y + 42, 112, 28)
              .with_label(tabs[i])
              .with_font("FredokaMockBold", pixels(20.f * scale))
              .with_custom_text_color(ink)
              .with_ignore_pointer_events()
              .with_alignment(TextAlignment::Center));
      if (i == 2) {
        div(context, mk(content.ent(), 130),
            box(scale, 182, y + 5, 22, 22)
                .with_label("!")
                .with_custom_background(afterhours::Color{242, 108, 105, 255})
                .with_border(afterhours::Color{118, 62, 64, 255}, 2.f)
                .with_corner_radius(11.f * scale)
                .with_font("FredokaMockBold", pixels(24.f * scale))
                .with_custom_text_color(white)
                .with_alignment(TextAlignment::Center)
                .with_debug_name("empire_upgrade_badge"));
      }
    }

    div(context, mk(content.ent(), 200),
        box(scale, 254, 257, 942, 342)
            .with_on_draw_bg([](RectangleType r) { draw_panel(r); })
            .with_debug_name("empire_dashboard"));

    auto heading = [&](int id, float x, const char *label) {
      div(context, mk(content.ent(), id),
          box(scale, x, 287, 330, 35)
              .with_label(label)
              .with_font("FredokaMockBold", pixels(28.f * scale))
              .with_custom_text_color(ink)
              .with_alignment(TextAlignment::Center));
    };
    heading(210, 294, "Production Overview");
    heading(211, 820, "Current Projects");

    div(context, mk(content.ent(), 220),
        box(scale, 282, 330, 430, 170)
            .with_custom_background(card_bg)
            .with_border(afterhours::Color{93, 153, 184, 255}, 2.f)
            .with_corner_radius(16.f * scale)
            .with_on_draw_fg([=, this](RectangleType r) {
              outline(r, 16.f * scale, 2.f * scale, panel_edge);
            })
            .with_debug_name("empire_production_list"));
    div(context, mk(content.ent(), 241), box(scale, 301, 331, 390, 21)
        .with_label("items/min / change from starting rate").with_font("FredokaMockBold", pixels(16 * scale))
        .with_custom_text_color(ink).with_ignore_pointer_events());
    for (size_t i = 0; i < production.size(); ++i) {
      const float y = 348.f + static_cast<float>(i) * 40.f;
      div(context, mk(content.ent(), 221 + static_cast<int>(i) * 3),
          box(scale, 301, y, 346, 38)
              .with_label(production[i].name + ": " +
                          std::to_string(production[i].rate / 1000) + "," +
                          std::to_string((production[i].rate % 1000) / 100) +
                          "00 items/min")
              .with_font("FredokaMockBold", pixels(21.f * scale))
              .with_custom_text_color(ink)
              .with_alignment(TextAlignment::Left)
              .with_debug_name("empire_prod_" + std::to_string(i)));
      const std::array<int, 3> starting_rates{5200, 3800, 3100};
      div(context, mk(content.ent(), 222 + static_cast<int>(i) * 3), box(scale, 650, y + 3, 52, 28)
          .with_label("+" + std::to_string(production[i].rate - starting_rates[i]))
          .with_font("FredokaMockBold", pixels(19 * scale)).with_custom_text_color({37, 118, 69, 255})
          .with_alignment(TextAlignment::Right).with_ignore_pointer_events());
      div(context, mk(content.ent(), 223 + static_cast<int>(i) * 3),
          box(scale, 301, y + 35, 390, 2)
              .with_custom_background(afterhours::Color{220, 238, 245, 255}));
    }
    div(context, mk(content.ent(), 240),
        box(scale, 301, 473, 390, 24)
            .with_label("All systems running smoothly")
            .with_font("FredokaMockBold", pixels(19.f * scale))
            .with_custom_text_color(afterhours::Color{63, 105, 119, 255})
            .with_alignment(TextAlignment::Center));

    div(context, mk(content.ent(), 250),
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
      if (button(context, mk(content.ent(), 260 + static_cast<int>(i) * 6),
          box(scale, 751, y, 405, 70)
              .with_border(afterhours::Color{116, 164, 191, 255}, 1.5f)
              .with_corner_radius(11.f * scale)
              .with_custom_background(i == selected_project ? afterhours::Color{222, 240, 250, 255} : card_bg)
              .with_debug_name("empire_project_" + std::to_string(i)))) {
        selected_project = i;
        chat_status = p.name + " selected";
      }
      div(context, mk(content.ent(), 261 + static_cast<int>(i) * 6),
          box(scale, 760, y + 5, 330, 29)
              .with_label((i == selected_project ? "> " : "") + p.name)
              .with_ignore_pointer_events()
              .with_font("FredokaMockBold", pixels(20.f * scale))
              .with_custom_text_color(ink)
              .with_debug_name("empire_project_title_" + std::to_string(i)));
      div(context, mk(content.ent(), 800 + static_cast<int>(i)),
          box(scale, 1090, y + 5, 58, 29)
              .with_label(std::to_string(p.progress) + "%")
              .with_font("FredokaMockBold", pixels(20.f * scale))
              .with_custom_text_color(ink)
              .with_ignore_pointer_events()
              .with_alignment(TextAlignment::Right));
      div(context, mk(content.ent(), 262 + static_cast<int>(i) * 6),
          box(scale, 760, y + 31, 390, 5)
              .with_custom_background(afterhours::Color{219, 241, 250, 255}));
      div(context, mk(content.ent(), 263 + static_cast<int>(i) * 6),
          box(scale, 760, y + 31,
              390.f * static_cast<float>(p.progress) / 100.f, 5)
              .with_custom_background(green)
              .with_debug_name("empire_project_progress_" + std::to_string(i)));
      div(context, mk(content.ent(), 264 + static_cast<int>(i) * 6),
          box(scale, 761, y + 39, 330, 24)
              .with_label(std::to_string(100 - p.progress) + "% of work remaining")
              .with_ignore_pointer_events()
              .with_font("FredokaMockBold", pixels(17.f * scale))
              .with_custom_text_color(ink)
              .with_alignment(TextAlignment::Left));

    }

    const std::array<std::string, 3> actions{"Speed Up / +5% work", "Prioritize selected", new_project_started ? "Expand garden" : "New Project"};
    const std::array<std::string, 3> action_notes{"Free / +$25,000 cash", "$5,000 / +10% selected work",
        new_project_started ? "$12,000 / +5% garden work" : "$12,000 / start Sky Garden"};
    for (size_t i = 0; i < actions.size(); ++i) {
      const float x = 282.f + static_cast<float>(i) * 304.f;
      if (button(context, mk(content.ent(), 300 + static_cast<int>(i)),
                 box(scale, x, 517, 278, 44)
                     .with_label(actions[i])
                     .with_tooltip(i == 0 ? "Free boost: +$25,000; +400 items/min per line; +5% work per project; +2 happiness; +4 resources; +5 park development."
                                          : i == 1 ? "Spend $5,000 to add 10 percentage points of work to the selected project."
                                                   : "Spend $12,000 to start Sky Garden, or add 5 percentage points after it has started.")
                     .with_custom_background(i == 2 ? yellow : afterhours::Color{215, 243, 253, 255})
                     .with_disabled((i == 1 && (cash < 5000 || projects[selected_project].progress >= 100)) ||
                                    (i == 2 && (cash < 12000 || (new_project_started && projects.back().progress >= 100))))
                     .with_border(afterhours::Color{70, 89, 112, 255}, 3.f)
                     .with_corner_radius(15.f * scale)
                     .with_font("FredokaMockBold", pixels(23.f * scale))
                     .with_custom_text_color(ink)
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
      div(context, mk(content.ent(), 320 + static_cast<int>(i)), box(scale, x, 565, 278, 26)
          .with_label(action_notes[i]).with_font("FredokaMockBold", pixels(19 * scale))
          .with_custom_text_color(ink).with_alignment(TextAlignment::Center).with_ignore_pointer_events());
    }

    div(context, mk(content.ent(), 400), box(scale, 68, 616, 388, 80)
        .with_custom_background(card_bg).with_corner_radius(12 * scale).with_debug_name("empire_chat"));
    div(context, mk(content.ent(), 401), box(scale, 184, 625, 256, 28)
        .with_label(chat_status).with_font("FredokaMockBold", pixels(20 * scale))
        .with_custom_text_color(ink).with_text_overflow(TextOverflow::Ellipsis).with_debug_name("empire_chat_status"));
    div(context, mk(content.ent(), 402), box(scale, 184, 660, 256, 26)
        .with_label("Update v2.0 is live").with_font("FredokaMockBold", pixels(20 * scale)).with_custom_text_color(ink));
    div(context, mk(content.ent(), 403), box(scale, 84, 625, 88, 28)
        .with_label("GLOBAL").with_font("FredokaMockBold", pixels(18 * scale)).with_custom_text_color({48, 107, 141, 255}));
    div(context, mk(content.ent(), 404), box(scale, 84, 660, 88, 26)
        .with_label("DEVTEAM").with_font("FredokaMockBold", pixels(18 * scale)).with_custom_text_color({48, 107, 141, 255}));
    div(context, mk(content.ent(), 410), box(scale, 474, 617, 386, 32)
        .with_label("Park development " + std::to_string(static_cast<int>(std::lround(milestone_pct * 100))) + "%")
        .with_font("FredokaMockBold", pixels(23 * scale)).with_custom_text_color(ink)
        .with_alignment(TextAlignment::Center).with_debug_name("empire_milestone_title"));
    div(context, mk(content.ent(), 414), box(scale, 474, 650, 386, 28)
        .with_label(milestone_rewarded ? "$50,000 reward collected" : "Goal: 100% / Reward: $50,000")
        .with_font("FredokaMockBold", pixels(21 * scale)).with_custom_text_color(ink)
        .with_alignment(TextAlignment::Center));
    div(context, mk(content.ent(), 411), box(scale, 490, 682, 354, 14)
        .with_custom_background({76, 95, 122, 255}).with_corner_radius(7 * scale).with_debug_name("empire_milestone_bar"));
    div(context, mk(content.ent(), 412), box(scale, 492, 684, 350 * milestone_pct, 10)
        .with_custom_background(yellow).with_corner_radius(5 * scale).with_debug_name("empire_milestone_fill"));

    const std::array<float, 3> tool_positions{920, 1026, 1140};
    const std::array<float, 3> caption_widths{80, 96, 124};
    for (size_t i = 0; i < tools.size(); ++i) {
      const float x = tool_positions[i];
      const float caption_width = caption_widths[i];
      if (button(
              context, mk(content.ent(), 500 + static_cast<int>(i)),
              box(scale, x, 620, 49, 49)
                  .with_label("")
                  .with_custom_background(afterhours::Color{245, 194, 95, 255})
                  .with_border(stroke, 2.f)
                  .with_corner_radius(24.5f * scale)
                  .with_font("FredokaMockBold", pixels(26.f * scale))
                  .with_custom_text_color(white)
                  .with_text_stroke(stroke, 1.f)
                  .with_alignment(TextAlignment::Center)
                  .with_debug_name("empire_tool_" + std::to_string(i)))) {
        chat_status = tools[i] + " opened";
      }
      icon(730 + static_cast<int>(i), i + 5, x + 6, 626, 37, 37);

      div(context, mk(content.ent(), 510 + static_cast<int>(i)),
          box(scale, x + (49 - caption_width) / 2, 676, caption_width, 24)
              .with_label(tools[i])
              .with_font("FredokaMockBold", pixels(21.f * scale))
              .with_custom_text_color(ink)
              .with_custom_background({244, 252, 255, 230})
              .with_corner_radius(6 * scale)
              .with_ignore_pointer_events()
              .with_alignment(TextAlignment::Center));
    }
  }
};

REGISTER_EXAMPLE_SCREEN(empire_tycoon, "Game Mockups",
                        "Bright cartoon tycoon game", EmpireTycoonScreen)
