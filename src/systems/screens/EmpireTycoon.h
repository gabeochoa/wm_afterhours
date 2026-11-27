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
  std::string chat_status = "GlobalChat: New area unlocked!";

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
      {"Expand", 35},
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

  std::array<std::string, 5> tabs = {"Rides", "Food Stalls", "Upgrades",
                                     "Blueprint", "Finance"};
  std::array<std::string, 3> tools = {"Shop", "Settings", "Leaderboards"};

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

  static std::string format_rate(int rate) {
    std::string s = std::to_string(rate);
    std::string result;
    int count = 0;
    for (int i = static_cast<int>(s.size()) - 1; i >= 0; --i) {
      if (count > 0 && count % 3 == 0)
        result = "," + result;
      result = s[static_cast<size_t>(i)] + result;
      ++count;
    }
    return result;
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
    // Square stage: the square ref (1024x1024) fits a centered 720x720
    // stage; park art shows in the side margins on wide windows.
    const float scale = std::min(screen_w / 720.f, screen_h / 720.f);

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

    auto content = div(context, mk(root.ent()), ComponentConfig{}
        .with_size({pixels(720 * scale), pixels(720 * scale)})
        .with_absolute_position((screen_w - 720 * scale) / 2, (screen_h - 720 * scale) / 2)
        .with_background(Theme::Usage::None).with_corner_radius(0));

    auto icon = [&](int id, size_t index, float x, float y, float w, float h) {
      const auto texture = icons[index];
      sprite(context, mk(content.ent(), id), texture,
             {0, 0, (float)texture.width, (float)texture.height},
             box(scale, x, y, w, h).with_ignore_pointer_events());
    };

    div(context, mk(content.ent()),
        box(scale, 16, 12, 300, 60)
            .with_label("DREAM")
            .with_font("FredokaMockBold", pixels(56.f * scale))
            .with_custom_text_color(white)
            .with_text_stroke(stroke, 2.4f)
            .with_text_shadow(afterhours::Color{101, 185, 229, 255}, 0.f,
                              6.f * scale)
            .with_letter_spacing(-2.f * scale)
            .with_debug_name("empire_title_dream"));
    div(context, mk(content.ent()),
        box(scale, 16, 76, 270, 34)
            .with_label("INCORPORATED")
            .with_font("FredokaMockBold", pixels(31.f * scale))
            .with_custom_text_color(white)
            .with_text_stroke(stroke, 1.9f)
            .with_text_shadow(afterhours::Color{101, 185, 229, 255}, 0.f,
                              4.f * scale)
            .with_letter_spacing(-1.3f * scale)
            .with_debug_name("empire_title_incorporated"));

    div(context, mk(content.ent()),
        box(scale, 392, 16, 312, 60)
            .with_custom_background(afterhours::Color{128, 211, 246, 245})
            .with_border(stroke, 3.f)
            .with_corner_radius(30.f * scale)
            .with_on_draw_fg([=, this](RectangleType r) {
              outline(r, 30.f * scale, 3.f * scale, stroke);
            })
            .with_debug_name("empire_cash_pill"));
    div(context, mk(content.ent()),
        box(scale, 400, 22, 48, 48)
            .with_on_draw_fg([](RectangleType r) { draw_coin(r); })
            .with_ignore_pointer_events()
            .with_debug_name("empire_coin"));
    div(context, mk(content.ent()), box(scale, 406, 27, 36, 36)
        .with_label("$").with_font("FredokaMockBold", pixels(28 * scale))
        .with_custom_text_color(ink).with_alignment(TextAlignment::Center).with_ignore_pointer_events());

    div(context, mk(content.ent()),
        box(scale, 452, 24, 240, 44)
            .with_label(format_money(cash))
            .with_font("FredokaMockBold", pixels(32.f * scale))
            .with_custom_text_color(ink)
            .with_alignment(TextAlignment::Center)
            .with_letter_spacing(-1.f * scale)
            .with_debug_name("empire_cash"));

    struct Gauge {
      const char *label;
      float pct;
      afterhours::Color fill;
      float x;
    };
    std::array<Gauge, 2> gauges = {
        {{"Happiness", happiness_pct, afterhours::Color{83, 168, 87, 255},
          414.f},
         {"Resources", resources_pct, afterhours::Color{31, 129, 180, 255},
          566.f}}};
    for (size_t i = 0; i < gauges.size(); ++i) {
      const auto &g = gauges[i];
      div(context, mk(content.ent(), 30 + static_cast<int>(i) * 10),
          box(scale, g.x, 86, 128, 54)
              .with_on_draw_fg([pct = g.pct, fill = g.fill, resource = i == 1](RectangleType r) {
                draw_gauge(r, pct, fill, resource);
              })
              .with_ignore_pointer_events()
              .with_debug_name("empire_gauge_" + std::to_string(i)));
      div(context, mk(content.ent(), 31 + static_cast<int>(i) * 10),
          box(scale, g.x - 16, 142, 160, 26)
              .with_label(g.label + std::string(" ") +
                          std::to_string(static_cast<int>(std::lround(g.pct * 100.f))) + "%")
              .with_custom_background(afterhours::Color{184, 241, 255, 230})
              .with_border(stroke, 2.f)
              .with_corner_radius(13.f * scale)
              .with_font("FredokaMockBold", pixels(17.f * scale))
              .with_custom_text_color(ink)
              .with_alignment(TextAlignment::Center)
              .with_debug_name("empire_gauge_label_" + std::to_string(i)));
      for (int endpoint = 0; endpoint < 2; ++endpoint)
        div(context, mk(content.ent(), 32 + static_cast<int>(i) * 10 + endpoint),
            box(scale, g.x - 6 + endpoint * 108.f, 124, 26, 18)
                .with_label(endpoint == 0 ? "0" : "100").with_font("FredokaMockBold", pixels(13 * scale))
                .with_custom_text_color(ink).with_alignment(TextAlignment::Center).with_ignore_pointer_events());
    }

    div(context, mk(content.ent()),
        box(scale, 16, 172, 104, 352)
            .with_custom_background(afterhours::Color{191, 238, 255, 245})
            .with_border(stroke, 3.f)
            .with_corner_radius(20.f * scale)
            .with_on_draw_fg([=, this](RectangleType r) {
              outline(r, 20.f * scale, 3.f * scale, stroke);
            })
            .with_debug_name("empire_tabs_panel"));

    const std::array<afterhours::Color, 5> tab_colors = {{{114, 201, 237, 255},
                                                          {161, 216, 91, 255},
                                                          {233, 151, 194, 255},
                                                          {159, 213, 243, 255},
                                                          {255, 195, 85, 255}}};
    const std::array<int, 5> tab_icons = {1, 2, 3, -1, 4};
    for (size_t i = 0; i < tabs.size(); ++i) {
      const float y = 182.f + static_cast<float>(i) * 68.f;
      const bool active = i == selected_tab;
      if (button(context, mk(content.ent(), 100 + static_cast<int>(i)),
                 box(scale, 24, y, 88, 60)
                     .with_label("")
                     .with_custom_background(tab_colors[i])
                     .with_border(active ? afterhours::Color{255, 248, 181, 255}
                                         : afterhours::Color{72, 107, 131, 255},
                                  active ? 4.f : 2.f)
                     .with_corner_radius(4.f * scale)
                     .with_font("FredokaMockBold", pixels(15.f * scale))
                     .with_custom_text_color(ink)
                     .with_alignment(TextAlignment::Center)
                     .with_on_draw_fg([=, this](RectangleType r) {
                       outline(r, 4.f * scale, 2.f * scale, stroke);
                       if (active) outline({r.x + 5 * scale, r.y + 5 * scale, r.width - 10 * scale, r.height - 10 * scale},
                                           4 * scale, 3 * scale, {255, 253, 220, 255});
                     })
                     .with_debug_name("empire_tab_" + std::to_string(i)))) {
        select_tab(i);
      }

      if (tab_icons[i] >= 0)
        icon(710 + static_cast<int>(i), static_cast<size_t>(tab_icons[i]), 40, y + 4, 56, 36);
      div(context, mk(content.ent(), 720 + static_cast<int>(i)),
          box(scale, 24, y + 40, 88, 20)
              .with_label(tabs[i])
              .with_font("FredokaMockBold", pixels(15.f * scale))
              .with_custom_text_color(ink)
              .with_ignore_pointer_events()
              .with_alignment(TextAlignment::Center));
      if (i == 2) {
        div(context, mk(content.ent(), 130),
            box(scale, 98, y + 2, 24, 24)
                .with_label("!")
                .with_custom_background(afterhours::Color{242, 108, 105, 255})
                .with_border(afterhours::Color{118, 62, 64, 255}, 2.f)
                .with_corner_radius(12.f * scale)
                .with_font("FredokaMockBold", pixels(20.f * scale))
                .with_custom_text_color(white)
                .with_alignment(TextAlignment::Center)
                .with_debug_name("empire_upgrade_badge"));
      }
    }

    div(context, mk(content.ent()),
        box(scale, 134, 180, 570, 320)
            .with_on_draw_bg([](RectangleType r) { draw_panel(r); })
            .with_debug_name("empire_dashboard"));

    auto heading = [&](int id, float x, const char *label) {
      div(context, mk(content.ent(), id),
          box(scale, x, 198, 262, 30)
              .with_label(label)
              .with_font("FredokaMockBold", pixels(24.f * scale))
              .with_custom_text_color(ink)
              .with_alignment(TextAlignment::Center));
    };
    heading(210, 150, "Production Overview");
    heading(211, 432, "Current Projects");

    div(context, mk(content.ent()),
        box(scale, 150, 236, 262, 150)
            .with_custom_background(card_bg)
            .with_border(afterhours::Color{93, 153, 184, 255}, 2.f)
            .with_corner_radius(14.f * scale)
            .with_on_draw_fg([=, this](RectangleType r) {
              outline(r, 14.f * scale, 2.f * scale, panel_edge);
            })
            .with_debug_name("empire_production_list"));
    for (size_t i = 0; i < production.size(); ++i) {
      const float y = 252.f + static_cast<float>(i) * 42.f;
      div(context, mk(content.ent(), 221 + static_cast<int>(i) * 3),
          box(scale, 162, y, 208, 36)
              .with_label(production[i].name + ": " +
                          format_rate(production[i].rate) + "/min")
              .with_font("FredokaMockBold", pixels(19.f * scale))
              .with_custom_text_color(ink)
              .with_alignment(TextAlignment::Left)
              .with_debug_name("empire_prod_" + std::to_string(i)));
      icon(223 + static_cast<int>(i) * 3, 8, 376, y + 5, 26, 26);
    }

    div(context, mk(content.ent(), 250),
        box(scale, 432, 236, 262, 150)
            .with_custom_background(card_bg)
            .with_border(afterhours::Color{93, 153, 184, 255}, 2.f)
            .with_corner_radius(14.f * scale)
            .with_on_draw_fg([=, this](RectangleType r) {
              outline(r, 14.f * scale, 2.f * scale, panel_edge);
            })
            .with_debug_name("empire_project_list"));
    for (size_t i = 0; i < projects.size(); ++i) {
      const auto &p = projects[i];
      const float y = 246.f + static_cast<float>(i) * 48.f;
      if (button(context, mk(content.ent(), 260 + static_cast<int>(i) * 6),
          box(scale, 442, y, 242, 44)
              .with_border(afterhours::Color{116, 164, 191, 255}, 1.5f)
              .with_corner_radius(8.f * scale)
              .with_custom_background(i == selected_project ? afterhours::Color{222, 240, 250, 255} : card_bg)
              .with_debug_name("empire_project_" + std::to_string(i)))) {
        selected_project = i;
        chat_status = p.name + " selected";
      }
      div(context, mk(content.ent(), 261 + static_cast<int>(i) * 6),
          box(scale, 450, y + 8, 226, 28)
              .with_label((i == selected_project ? "> " : "") + p.name +
                          " - " + std::to_string(p.progress) + "%")
              .with_ignore_pointer_events()
              .with_font("FredokaMockBold", pixels(17.f * scale))
              .with_custom_text_color(ink)
              .with_debug_name("empire_project_title_" + std::to_string(i)));
    }

    const std::array<std::string, 3> actions{"Speed Up", "Prioritize", "New Project"};
    for (size_t i = 0; i < actions.size(); ++i) {
      const float x = 146.f + static_cast<float>(i) * 184.f;
      if (button(context, mk(content.ent(), 300 + static_cast<int>(i)),
                 box(scale, x, 512, 176, 44)
                     .with_label(actions[i])
                     .with_tooltip(i == 0 ? "Free boost: +$25,000; +400 items/min per line; +5% work per project; +2 happiness; +4 resources; +5 park development."
                                          : i == 1 ? "Spend $5,000 to add 10 percentage points of work to the selected project."
                                                   : "Spend $12,000 to start Sky Garden, or add 5 percentage points after it has started.")
                     .with_custom_background(yellow)
                     .with_disabled((i == 1 && (cash < 5000 || projects[selected_project].progress >= 100)) ||
                                    (i == 2 && (cash < 12000 || (new_project_started && projects.back().progress >= 100))))
                     .with_border(afterhours::Color{70, 89, 112, 255}, 3.f)
                     .with_corner_radius(12.f * scale)
                     .with_font("FredokaMockBold", pixels(20.f * scale))
                     .with_custom_text_color(ink)
                     .with_alignment(TextAlignment::Center)
                     .with_on_draw_fg([=, this](RectangleType r) {
                       outline(r, 12.f * scale, 3.f * scale, stroke);
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

    div(context, mk(content.ent(), 400), box(scale, 16, 600, 270, 96)
        .with_custom_background(card_bg).with_corner_radius(12 * scale).with_debug_name("empire_chat"));
    div(context, mk(content.ent(), 401), box(scale, 28, 608, 246, 26)
        .with_label(chat_status).with_font("FredokaMockBold", pixels(16 * scale))
        .with_custom_text_color(ink).with_text_overflow(TextOverflow::Ellipsis).with_debug_name("empire_chat_status"));
    div(context, mk(content.ent(), 402), box(scale, 28, 660, 246, 26)
        .with_label("DevTeam: Update v2.0 is live").with_font("FredokaMockBold", pixels(16 * scale)).with_custom_text_color(ink));
    div(context, mk(content.ent()), box(scale, 296, 602, 194, 28)
        .with_label("Next Milestone")
        .with_font("FredokaMockBold", pixels(20 * scale)).with_custom_text_color(ink)
        .with_alignment(TextAlignment::Center).with_debug_name("empire_milestone_title"));
    div(context, mk(content.ent()), box(scale, 310, 642, 166, 16)
        .with_custom_background({76, 95, 122, 255}).with_corner_radius(8 * scale).with_debug_name("empire_milestone_bar"));
    div(context, mk(content.ent()), box(scale, 312, 644, 162 * milestone_pct, 12)
        .with_custom_background(yellow).with_corner_radius(6 * scale).with_debug_name("empire_milestone_fill"));

    const std::array<float, 3> tool_positions{500, 568, 638};
    const std::array<float, 3> caption_widths{46, 58, 82};
    for (size_t i = 0; i < tools.size(); ++i) {
      const float x = tool_positions[i];
      const float caption_width = caption_widths[i];
      if (button(
              context, mk(content.ent(), 500 + static_cast<int>(i)),
              box(scale, x, 606, 40, 40)
                  .with_label("")
                  .with_custom_background(afterhours::Color{245, 194, 95, 255})
                  .with_border(stroke, 2.f)
                  .with_corner_radius(20.f * scale)
                  .with_font("FredokaMockBold", pixels(26.f * scale))
                  .with_custom_text_color(white)
                  .with_text_stroke(stroke, 1.f)
                  .with_alignment(TextAlignment::Center)
                  .with_debug_name("empire_tool_" + std::to_string(i)))) {
        chat_status = tools[i] + " opened";
      }
      icon(730 + static_cast<int>(i), i + 5, x + 5, 611, 30, 30);

      div(context, mk(content.ent(), 510 + static_cast<int>(i)),
          box(scale, x + (40 - caption_width) / 2, 652, caption_width, 20)
              .with_label(tools[i])
              .with_font("FredokaMockBold", pixels(12.f * scale))
              .with_custom_text_color(ink)
              .with_custom_background({244, 252, 255, 230})
              .with_corner_radius(4 * scale)
              .with_ignore_pointer_events()
              .with_alignment(TextAlignment::Center));
    }
  }
};

REGISTER_EXAMPLE_SCREEN(empire_tycoon, "Game Mockups",
                        "Bright cartoon tycoon game", EmpireTycoonScreen)
