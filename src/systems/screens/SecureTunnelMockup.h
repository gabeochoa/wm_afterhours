#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>
#include <afterhours/src/plugins/ui/text_input/text_input.h>
#include <algorithm>
#include <array>
#include <bitset>
#include <cctype>
#include <cmath>
#include <string>
#include <vector>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct SecureTunnelMockup : ScreenSystem<UIContext<InputAction>> {
  enum struct Link { Off, Dialing, Up };

  struct Server {
    const char *place;
    const char *country;
    const char *tag;
    int ms;
  };

  bool wants_up = false;
  Link link = Link::Off;
  float dialing_for = 0.f;
  size_t server_idx = 1;
  std::string search_query;
  size_t protocol_idx = 0;
  std::string local_status = "Network simulation only";
  bool texture_loaded = false;
  raylib::Texture2D map_texture{};

  static constexpr float DIAL_SECONDS = 1.2f;
  static constexpr std::array<Server, 6> servers{{
      {"Reykjavik", "Iceland", "is-01", 14},
      {"Lisbon", "Portugal", "pt-04", 27},
      {"Montreal", "Canada", "ca-02", 41},
      {"Fortaleza", "Brazil", "br-01", 96},
      {"Wellington", "New Zealand", "nz-03", 178},
      {"Cape Town", "South Africa", "za-01", 204},
  }};
  static constexpr std::array<const char *, 3> protocols{{"WireGuard", "OpenVPN", "IKEv2"}};
  static constexpr std::array<Vector2Type, 6> server_points{{
      {333.f, 70.f}, {349.f, 113.f}, {127.f, 100.f},
      {206.f, 225.f}, {690.f, 293.f}, {420.f, 227.f},
  }};

  ComponentConfig box(float scale, float x, float y, float w, float h) const {
    return ComponentConfig{}
        .with_size({pixels(w * scale), pixels(h * scale)})
        .with_absolute_position(x * scale, y * scale)
        .with_background(Theme::Usage::None);
  }

  void load_texture_if_needed() {
    if (texture_loaded)
      return;
    texture_loaded = true;
    map_texture = raylib::LoadTexture(afterhours::files::get_resource_path(
        "images", "secure_tunnel/world_map.png").string().c_str());
    if (map_texture.id != 0)
      raylib::SetTextureFilter(map_texture, raylib::TEXTURE_FILTER_BILINEAR);
  }

  static void paint(raylib::Texture2D texture, RectangleType r) {
    if (texture.id == 0)
      return;
    raylib::DrawTexturePro(texture,
                           {0, 0, static_cast<float>(texture.width),
                            static_cast<float>(texture.height)},
                           r, {0, 0}, 0.f, afterhours::Color{255, 255, 255, 255});
  }

  static const char *state_word(Link l) {
    if (l == Link::Dialing)
      return "Negotiating";
    if (l == Link::Up)
      return "Tunnel up";
    return "Not connected";
  }

  std::string hero_title() const {
    if (link == Link::Up)
      return "Your connection\nis protected.";
    if (link == Link::Dialing)
      return "Creating a secure\ntunnel.";
    return "Your connection\nis unprotected.";
  }

  std::string connect_label() const {
    if (link == Link::Up)
      return "Disconnect";
    if (link == Link::Dialing)
      return "Connecting...";
    return "Quick connect";
  }

  std::string selected_route() const {
    const auto &s = servers[server_idx];
    return std::string(s.place) + " - " + s.tag;
  }

  static std::string lowercase(std::string value) {
    for (char &c : value)
      c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return value;
  }

  bool server_visible(size_t i) const {
    if (search_query.empty())
      return true;
    const std::string q = lowercase(search_query);
    const std::string place = lowercase(servers[i].place);
    const std::string country = lowercase(servers[i].country);
    return place.find(q) != std::string::npos || country.find(q) != std::string::npos;
  }

  int visible_server_count() const {
    int count = 0;
    for (size_t i = 0; i < servers.size(); ++i)
      if (server_visible(i))
        ++count;
    return count;
  }

  void advance(float dt) {
    if (!wants_up) {
      link = Link::Off;
      dialing_for = 0.f;
      return;
    }
    if (link == Link::Off) {
      link = Link::Dialing;
      dialing_for = 0.f;
    }
    if (link == Link::Dialing) {
      dialing_for += dt;
      if (dialing_for >= DIAL_SECONDS)
        link = Link::Up;
    }
  }

  void toggle_connection() {
    if (wants_up) {
      wants_up = false;
      link = Link::Off;
      dialing_for = 0.f;
      local_status = "Disconnected locally";
      return;
    }
    wants_up = true;
    link = Link::Dialing;
    dialing_for = 0.f;
    local_status = "Simulated tunnel dialing";
  }

  static void draw_map_background(RectangleType r) {
    afterhours::draw_rectangle_gradient_v(r, afterhours::Color{248, 251, 255, 255},
                                          afterhours::Color{231, 237, 246, 255});
    afterhours::draw_circle(static_cast<int>(r.x + r.width * .47f),
                            static_cast<int>(r.y + r.height * .48f),
                            r.width * .52f, afterhours::Color{255, 255, 255, 105});
  }

  static void draw_rail_icon(RectangleType r, int icon, bool active) {
    const auto ink = active ? afterhours::Color{255, 255, 255, 255}
                            : afterhours::Color{152, 168, 197, 255};
    const float cx = r.x + r.width * .5f;
    const float cy = r.y + r.height * .5f;
    if (icon == 0) {
      afterhours::draw_triangle({cx, cy - 13.f}, {cx - 9.f, cy + 10.f},
                                {cx + 9.f, cy + 10.f}, ink);
      afterhours::draw_triangle({cx, cy - 5.f}, {cx - 4.f, cy + 7.f},
                                {cx + 4.f, cy + 7.f}, afterhours::Color{28, 39, 67, 255});
    } else if (icon == 1) {
      afterhours::draw_circle_lines(static_cast<int>(cx), static_cast<int>(cy), 11.f, ink);
      afterhours::draw_circle_lines(static_cast<int>(cx), static_cast<int>(cy), 7.f, ink);
    } else if (icon == 2) {
      afterhours::draw_poly_lines_ex({cx, cy}, 6, 8.f, 30.f, 2.f, ink);
    } else if (icon == 3) {
      afterhours::draw_line_ex({cx - 12.f, cy - 6.f}, {cx + 10.f, cy - 6.f}, 2.f, ink);
      afterhours::draw_line_ex({cx + 5.f, cy - 12.f}, {cx + 12.f, cy - 6.f}, 2.f, ink);
      afterhours::draw_line_ex({cx + 5.f, cy}, {cx + 12.f, cy - 6.f}, 2.f, ink);
      afterhours::draw_line_ex({cx + 12.f, cy + 7.f}, {cx - 10.f, cy + 7.f}, 2.f, ink);
      afterhours::draw_line_ex({cx - 5.f, cy + 1.f}, {cx - 12.f, cy + 7.f}, 2.f, ink);
      afterhours::draw_line_ex({cx - 5.f, cy + 13.f}, {cx - 12.f, cy + 7.f}, 2.f, ink);
    } else if (icon == 4) {
      for (int i = 0; i < 6; ++i)
        afterhours::draw_line_ex({cx - 10.f, cy - 10.f + i * 4.f},
                                 {cx + 10.f, cy - 10.f + i * 4.f}, 1.5f, ink);
    } else {
      afterhours::draw_circle_lines(static_cast<int>(cx), static_cast<int>(cy), 7.f, ink);
      for (int i = 0; i < 8; ++i) {
        const float a = static_cast<float>(i) * 0.7853982f;
        afterhours::draw_line_ex({cx + std::cos(a) * 8.f, cy + std::sin(a) * 8.f},
                                 {cx + std::cos(a) * 12.f, cy + std::sin(a) * 12.f}, 2.f, ink);
      }
    }
  }

  static void draw_flag(RectangleType r, size_t i) {
    afterhours::draw_rectangle_rounded(r, .12f, 4, afterhours::Color{255, 255, 255, 255},
                                       std::bitset<4>().set());
    if (i == 0) {
      afterhours::draw_rectangle({r.x + 2.f, r.y + 2.f, r.width - 4.f, r.height - 4.f},
                                 afterhours::Color{28, 71, 142, 255});
      afterhours::draw_rectangle({r.x + 2.f, r.y + r.height * .43f, r.width - 4.f, 3.f},
                                 afterhours::Color{255, 255, 255, 255});
      afterhours::draw_rectangle({r.x + r.width * .40f, r.y + 2.f, 3.f, r.height - 4.f},
                                 afterhours::Color{255, 255, 255, 255});
      afterhours::draw_rectangle({r.x + 2.f, r.y + r.height * .48f, r.width - 4.f, 2.f},
                                 afterhours::Color{212, 43, 30, 255});
      afterhours::draw_rectangle({r.x + r.width * .45f, r.y + 2.f, 2.f, r.height - 4.f},
                                 afterhours::Color{212, 43, 30, 255});
    } else if (i == 1) {
      afterhours::draw_rectangle({r.x + 2.f, r.y + 2.f, r.width * .38f, r.height - 4.f},
                                 afterhours::Color{0, 102, 56, 255});
      afterhours::draw_rectangle({r.x + r.width * .40f, r.y + 2.f, r.width * .58f, r.height - 4.f},
                                 afterhours::Color{218, 43, 53, 255});
      afterhours::draw_circle(static_cast<int>(r.x + r.width * .47f), static_cast<int>(r.y + r.height * .5f),
                              r.height * .18f, afterhours::Color{255, 218, 92, 255});
    } else if (i == 2) {
      afterhours::draw_rectangle({r.x + 2.f, r.y + 2.f, r.width - 4.f, r.height - 4.f},
                                 afterhours::Color{238, 238, 238, 255});
      afterhours::draw_rectangle({r.x + 2.f, r.y + r.height * .40f, r.width - 4.f, r.height * .20f},
                                 afterhours::Color{221, 35, 50, 255});
      afterhours::draw_rectangle({r.x + r.width * .43f, r.y + 2.f, r.width * .16f, r.height - 4.f},
                                 afterhours::Color{221, 35, 50, 255});
    } else if (i == 3) {
      afterhours::draw_rectangle({r.x + 2.f, r.y + 2.f, r.width - 4.f, r.height - 4.f},
                                 afterhours::Color{23, 154, 75, 255});
      afterhours::draw_poly({r.x + r.width * .5f, r.y + r.height * .5f}, 4,
                            r.height * .34f, 45.f, afterhours::Color{250, 220, 62, 255});
      afterhours::draw_circle(static_cast<int>(r.x + r.width * .5f), static_cast<int>(r.y + r.height * .5f),
                              r.height * .17f, afterhours::Color{44, 86, 170, 255});
    } else if (i == 4) {
      afterhours::draw_rectangle({r.x + 2.f, r.y + 2.f, r.width - 4.f, r.height * .48f},
                                 afterhours::Color{11, 42, 130, 255});
      afterhours::draw_rectangle({r.x + 2.f, r.y + r.height * .5f, r.width - 4.f, r.height * .48f},
                                 afterhours::Color{29, 35, 78, 255});
      afterhours::draw_circle(static_cast<int>(r.x + r.width * .72f), static_cast<int>(r.y + r.height * .33f),
                              r.height * .12f, afterhours::Color{214, 38, 48, 255});
    } else {
      afterhours::draw_rectangle({r.x + 2.f, r.y + 2.f, r.width - 4.f, r.height * .30f},
                                 afterhours::Color{220, 45, 45, 255});
      afterhours::draw_rectangle({r.x + 2.f, r.y + r.height * .35f, r.width - 4.f, r.height * .26f},
                                 afterhours::Color{24, 34, 30, 255});
      afterhours::draw_rectangle({r.x + 2.f, r.y + r.height * .66f, r.width - 4.f, r.height * .30f},
                                 afterhours::Color{35, 125, 62, 255});
      afterhours::draw_triangle({r.x + 2.f, r.y + 2.f}, {r.x + r.width * .48f, r.y + r.height * .5f},
                                {r.x + 2.f, r.y + r.height - 2.f}, afterhours::Color{255, 255, 255, 255});
    }
  }

  static void draw_server_dots(RectangleType r, size_t selected) {
    const float sx = r.width / 740.f;
    const float sy = r.height / 350.f;
    for (size_t i = 0; i < server_points.size(); ++i) {
      const float x = r.x + server_points[i].x * sx;
      const float y = r.y + server_points[i].y * sy;
      if (i == selected) {
        afterhours::draw_circle(static_cast<int>(x), static_cast<int>(y), 25.f * sx,
                                afterhours::Color{103, 137, 237, 42});
        afterhours::draw_circle(static_cast<int>(x), static_cast<int>(y), 15.f * sx,
                                afterhours::Color{102, 133, 233, 255});
        afterhours::draw_circle_lines(static_cast<int>(x), static_cast<int>(y), 15.f * sx,
                                      afterhours::Color{255, 255, 255, 255});
      } else {
        afterhours::draw_circle(static_cast<int>(x), static_cast<int>(y), 7.f * sx,
                                afterhours::Color{92, 128, 176, 255});
        afterhours::draw_circle_lines(static_cast<int>(x), static_cast<int>(y), 7.f * sx,
                                      afterhours::Color{255, 255, 255, 255});
      }
    }
  }

  static void draw_shield(RectangleType r) {
    const auto fill = afterhours::Color{217, 228, 244, 255};
    const auto blue = afterhours::Color{104, 135, 189, 255};
    afterhours::draw_triangle({r.x + r.width * .5f, r.y}, {r.x + r.width, r.y + r.height * .18f},
                              {r.x + r.width * .5f, r.y + r.height}, fill);
    afterhours::draw_triangle({r.x + r.width * .5f, r.y}, {r.x, r.y + r.height * .18f},
                              {r.x + r.width * .5f, r.y + r.height}, fill);
    afterhours::draw_circle_lines(static_cast<int>(r.x + r.width * .5f),
                                  static_cast<int>(r.y + r.height * .50f),
                                  r.width * .18f, blue);
    afterhours::draw_line_ex({r.x + r.width * .5f, r.y + r.height * .35f},
                             {r.x + r.width * .5f, r.y + r.height * .75f}, 2.f, blue);
    afterhours::draw_line_ex({r.x + r.width * .32f, r.y + r.height * .60f},
                             {r.x + r.width * .68f, r.y + r.height * .60f}, 2.f, blue);
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float dt) override {
    advance(dt);
    load_texture_if_needed();

    const float screen_w = context.screen_width > 0.f
                               ? context.screen_width
                               : static_cast<float>(Settings::get().get_screen_width());
    const float screen_h = context.screen_height > 0.f
                               ? context.screen_height
                               : static_cast<float>(Settings::get().get_screen_height());
    const float scale = std::min(screen_w / 1280.f, screen_h / 720.f);

    if (context.pressed(InputAction::WidgetDown))
      server_idx = (server_idx + 1) % servers.size();
    if (context.pressed(InputAction::WidgetUp))
      server_idx = (server_idx + servers.size() - 1) % servers.size();
    if (context.pressed(InputAction::WidgetPress))
      toggle_connection();
    if (context.pressed(InputAction::WidgetRight))
      protocol_idx = (protocol_idx + 1) % protocols.size();
    if (context.pressed(InputAction::WidgetLeft))
      protocol_idx = (protocol_idx + protocols.size() - 1) % protocols.size();

    Theme theme;
    theme.font = {38, 54, 78, 255};
    theme.darkfont = {255, 255, 255, 255};
    theme.font_muted = {120, 131, 154, 255};
    theme.background = {237, 242, 249, 255};
    theme.surface = {255, 255, 255, 255};
    theme.primary = {62, 99, 219, 255};
    theme.secondary = {152, 168, 197, 255};
    theme.accent = {62, 99, 219, 255};
    theme.roundness = .04f;
    theme.segments = 8;
    context.theme = theme;
    context.scaling_mode = ScalingMode::Proportional;
    UIStylingDefaults::get().set_default_font("AtkinsonMock", pixels(18.f * scale));

    const auto text = [&](int id, std::string label, float x, float y, float w, float h,
                          float size, afterhours::Color color,
                          TextAlignment align = TextAlignment::Left,
                          const std::string &debug = "", const char *font = "AtkinsonMock") {
      div(context, mk(entity, id),
          box(scale, x, y, w, std::max(h, size * 1.55f))
              .with_label(label)
              .with_font(font, pixels(size * scale * 1.30f))
              .with_custom_text_color(color)
              .with_alignment(align)
              .with_letter_spacing(-.35f * scale)
              .with_debug_name(debug));
    };
    const auto rect = [&](int id, float x, float y, float w, float h, afterhours::Color fill,
                          const std::string &debug = "") {
      div(context, mk(entity, id),
          box(scale, x, y, w, h)
              .with_custom_background(fill)
              .with_debug_name(debug));
    };

    const auto navy = afterhours::Color{28, 39, 67, 255};
    const auto rail_active = afterhours::Color{52, 70, 106, 255};
    const auto ink = afterhours::Color{38, 54, 78, 255};
    const auto muted = afterhours::Color{120, 131, 154, 255};
    const auto pale = afterhours::Color{237, 241, 255, 255};
    const auto blue = afterhours::Color{62, 99, 219, 255};

    auto root = div(context, mk(entity, 0),
                    box(scale, 0, 0, 1280, 720)
                        .with_custom_background(afterhours::Color{237, 242, 249, 255})
                        .with_debug_name("tn_root"));

    rect(10, 0, 0, 70, 720, navy, "tn_rail");
    for (int i = 0; i < 6; ++i) {
      const float y = i == 5 ? 657.f : 20.f + static_cast<float>(i) * 57.f;
      const bool active = i == 1;
      if (button(context, mk(root.ent(), 20 + i),
                 box(scale, 13, y, 45, 45)
                     .with_label("")
                     .with_custom_background(active ? rail_active : afterhours::Color{0, 0, 0, 0})
                     .with_corner_radius(7.f * scale)
                     .with_on_draw_fg([i, active](RectangleType r) { draw_rail_icon(r, i, active); })
                     .with_debug_name("tn_rail_" + std::to_string(i)))) {
        const char *labels[6] = {"Secure Tunnel", "VPN", "Protection settings",
                                 "Split routing settings", "Diagnostics opened", "Settings opened"};
        local_status = labels[i];
      }
    }

    rect(30, 70, 0, 352, 720, afterhours::Color{255, 255, 255, 255}, "tn_sidebar");
    text(31, "Secure Tunnel", 93, 23, 220, 31, 22, ink, TextAlignment::Left,
         "tn_title", "Archivo@bold");
    rect(32, 93, 66, 7, 7, link == Link::Up ? afterhours::Color{48, 160, 116, 255}
                                             : afterhours::Color{199, 102, 84, 255});
    text(33, state_word(link), 107, 62, 150, 18, 12, muted, TextAlignment::Left,
         "tn_state");
    text(34, hero_title(), 92, 87, 282, 66, 25, ink, TextAlignment::Left,
         "tn_hero", "Archivo@bold");
    if (button(context, mk(root.ent(), 35),
               box(scale, 93, 157, 306, 44)
                   .with_label(connect_label())
                   .with_font("Archivo@bold", pixels(18.f * scale))
                   .with_custom_text_color(afterhours::Color{255, 255, 255, 255})
                   .with_custom_background(link == Link::Up ? afterhours::Color{45, 144, 112, 255}
                                                            : blue)
                   .with_alignment(TextAlignment::Center)
                   .with_corner_radius(5.f * scale)
                   .with_debug_name("tn_connect"))) {
      toggle_connection();
    }
    text(36, std::string(link == Link::Up ? "Connected server - " : "Fastest available server - ") + servers[server_idx].place,
         118, 210, 260, 18, 11, afterhours::Color{137, 145, 162, 255}, TextAlignment::Center,
         "tn_quick_place");

    auto search = text_input(context, mk(root.ent(), 40), search_query,
                             box(scale, 93, 236, 306, 36)
                                 .with_font("AtkinsonMock", pixels(17.f * scale))
                                 .with_placeholder("Search locations")
                                 .with_custom_text_color(afterhours::Color{104, 114, 128, 255})
                                 .with_custom_background(afterhours::Color{247, 248, 250, 255})
                                 .with_border(afterhours::Color{220, 224, 233, 255}, 1.f)
                                 .with_text_inset(34.f * scale, 8.f * scale)
                                 .with_corner_radius(5.f * scale)
                                 .with_debug_name("tn_search"));
    if (search) {
      local_status = search_query.empty() ? "Showing all 6 locations" :
                                            "Search filter applied locally";
    }
    div(context, mk(root.ent(), 41),
        box(scale, 107, 248, 13, 13)
            .with_on_draw_fg([](RectangleType r) {
              afterhours::draw_circle_lines(static_cast<int>(r.x + 5.f), static_cast<int>(r.y + 5.f),
                                            4.f, afterhours::Color{38, 54, 78, 255});
              afterhours::draw_line_ex({r.x + 8.f, r.y + 8.f}, {r.x + 12.f, r.y + 12.f},
                                       1.5f, afterhours::Color{38, 54, 78, 255});
            })
            .with_ignore_pointer_events());
    text(42, "ALL LOCATIONS", 97, 290, 140, 16, 10, afterhours::Color{139, 148, 165, 255},
         TextAlignment::Left, "tn_locations_label", "Archivo");
    text(43, std::to_string(visible_server_count()), 376, 290, 20, 16, 11, muted,
         TextAlignment::Right, "tn_locations_count");

    int visible_row = 0;
    for (size_t i = 0; i < servers.size(); ++i) {
      if (!server_visible(i))
        continue;
      const bool active = i == server_idx;
      const float y = 305.f + static_cast<float>(visible_row) * 46.f;
      ++visible_row;
      if (button(context, mk(root.ent(), 50 + static_cast<int>(i)),
                 box(scale, 93, y, 306, 46)
                     .with_label("")
                     .with_custom_background(active ? pale : afterhours::Color{0, 0, 0, 0})
                     .with_corner_radius(5.f * scale)
                     .with_debug_name("tn_server_" + std::to_string(i)))) {
        server_idx = i;
        local_status = std::string("Selected server - ") + servers[i].place;
      }
      div(context, mk(root.ent(), 80 + static_cast<int>(i)),
          box(scale, 104, y + 13, 22, 15)
              .with_on_draw_fg([i](RectangleType r) { draw_flag(r, i); })
              .with_ignore_pointer_events());
      text(90 + static_cast<int>(i), servers[i].place, 138, y + 8, 134, 18, 13,
           ink, TextAlignment::Left, "tn_server_name_" + std::to_string(i), "Archivo@bold");
      text(100 + static_cast<int>(i), servers[i].country, 138, y + 27, 142, 16, 11,
           afterhours::Color{146, 155, 173, 255}, TextAlignment::Left,
           "tn_server_country_" + std::to_string(i));
      text(110 + static_cast<int>(i), fmt::format("{} ms", servers[i].ms), 305, y + 15, 68, 17, 11,
           afterhours::Color{132, 144, 165, 255}, TextAlignment::Right,
           "tn_server_ms_" + std::to_string(i));
      text(120 + static_cast<int>(i), ">", 383, y + 14, 12, 20, 19,
           afterhours::Color{137, 150, 172, 255}, TextAlignment::Center);
    }
    if (visible_row == 0) {
      text(130, "No locations found", 111, 318, 220, 24, 14,
           afterhours::Color{137, 145, 162, 255}, TextAlignment::Left,
           "tn_no_results", "Archivo@bold");
      text(131, "Try city or country", 111, 344, 220, 18, 11,
           afterhours::Color{146, 155, 173, 255}, TextAlignment::Left,
           "tn_no_results_hint");
    }


    rect(140, 93, 591, 306, 1, afterhours::Color{229, 232, 240, 255}, "tn_protocol_rule");
    text(141, "Protocol", 98, 611, 90, 17, 11, afterhours::Color{139, 148, 166, 255},
         TextAlignment::Left, "tn_protocol_label");
    if (button(context, mk(root.ent(), 142),
               box(scale, 285, 603, 105, 32)
                   .with_label(protocols[protocol_idx])
                   .with_font("AtkinsonMock", pixels(14.f * scale))
                   .with_custom_text_color(afterhours::Color{72, 87, 116, 255})
                   .with_custom_background(afterhours::Color{0, 0, 0, 0})
                   .with_alignment(TextAlignment::Right)
                   .with_debug_name("tn_protocol"))) {
      protocol_idx = (protocol_idx + 1) % protocols.size();
      local_status = std::string("Protocol options - ") + protocols[protocol_idx];
    }

    div(context, mk(root.ent(), 200),
        box(scale, 422, 0, 858, 648)
            .with_on_draw_bg([](RectangleType r) { draw_map_background(r); })
            .with_debug_name("tn_map"));
    text(201, "~ VPN", 449, 34, 80, 20, 14, afterhours::Color{156, 168, 187, 255},
         TextAlignment::Left, "tn_map_label");
    if (button(context, mk(root.ent(), 202),
               box(scale, 1221, 24, 32, 32)
                   .with_label("GO")
                   .with_font("Archivo@bold", pixels(13.f * scale))
                   .with_custom_text_color(afterhours::Color{84, 116, 157, 255})
                   .with_custom_background(afterhours::Color{219, 228, 243, 255})
                   .with_corner_radius(16.f * scale)
                   .with_alignment(TextAlignment::Center)
                   .with_debug_name("tn_account"))) {
      local_status = "Account settings opened";
    }
    div(context, mk(root.ent(), 203),
        box(scale, 437, 141, 825, 390)
            .with_on_draw_fg([texture = map_texture, selected = server_idx](RectangleType r) {
              paint(texture, r);
              draw_server_dots(r, selected);
            })
            .with_debug_name("tn_world_map"));

    const float pin_x = 722.f;
    const float pin_y = 290.f;
    div(context, mk(root.ent(), 210),
        box(scale, pin_x, pin_y, 221, 63)
            .with_custom_background(afterhours::Color{255, 255, 255, 245})
            .with_corner_radius(7.f * scale)
            .with_debug_name("tn_location_card"));
    div(context, mk(root.ent(), 211),
        box(scale, pin_x + 19, pin_y + 22, 24, 16)
            .with_on_draw_fg([idx = server_idx](RectangleType r) { draw_flag(r, idx); })
            .with_ignore_pointer_events());
    text(212, servers[server_idx].place, pin_x + 58, pin_y + 16, 110, 19, 14, ink,
         TextAlignment::Left, "tn_location_name", "Archivo@bold");
    text(213, fmt::format("{} - {} - {} ms", servers[server_idx].country,
                          servers[server_idx].tag, servers[server_idx].ms),
         pin_x + 58, pin_y + 38, 142, 17, 11, afterhours::Color{139, 150, 168, 255},
         TextAlignment::Left, "tn_location_detail");
    rect(214, pin_x + 199, pin_y + 31, 4, 4, blue);

    div(context, mk(root.ent(), 220),
        box(scale, 599, 506, 505, 103)
            .with_custom_background(afterhours::Color{255, 255, 255, 186})
            .with_border(afterhours::Color{255, 255, 255, 220}, 1.f)
            .with_corner_radius(9.f * scale)
            .with_debug_name("tn_connection_card"));
    div(context, mk(root.ent(), 221),
        box(scale, 624, 531, 48, 53)
            .with_on_draw_fg([](RectangleType r) { draw_shield(r); })
            .with_ignore_pointer_events());
    text(222, link == Link::Up ? "You're connected securely." : "Privacy, wherever you go.",
         692, 538, 330, 24, 17, ink, TextAlignment::Left, "tn_card_title", "Archivo@bold");
    text(223, link == Link::Up
                  ? std::string("Traffic is encrypted through ") + servers[server_idx].place + "."
                  : "Connect to keep your internet traffic private.",
         692, 571, 350, 21, 12, afterhours::Color{138, 149, 167, 255}, TextAlignment::Left,
         "tn_card_copy");

    rect(230, 422, 648, 858, 72, afterhours::Color{241, 245, 250, 255}, "tn_bottom");
    rect(231, 422, 648, 858, 1, afterhours::Color{220, 228, 238, 255});
    text(232, "IP address", 451, 672, 80, 14, 10, afterhours::Color{140, 152, 171, 255});
    text(233, link == Link::Up ? "185.23.18.42" : "Not hidden", 451, 691, 130, 18, 12,
         afterhours::Color{99, 114, 139, 255}, TextAlignment::Left, "tn_ip_value");
    text(234, "Connection", 545, 672, 100, 14, 10, afterhours::Color{140, 152, 171, 255});
    text(235, link == Link::Up ? selected_route() : "No route", 545, 691, 165, 18, 12,
         afterhours::Color{99, 114, 139, 255}, TextAlignment::Left, "tn_route_value");
    if (button(context, mk(root.ent(), 236),
               box(scale, 1138, 675, 108, 24)
                   .with_label("View diagnostics")
                   .with_font("AtkinsonMock", pixels(14.f * scale))
                   .with_custom_text_color(afterhours::Color{121, 139, 171, 255})
                   .with_custom_background(afterhours::Color{0, 0, 0, 0})
                   .with_alignment(TextAlignment::Right)
                   .with_debug_name("tn_diagnostics"))) {
      local_status = "Diagnostics opened - simulated network only";
    }
    text(237, local_status, 736, 691, 290, 18, 11, afterhours::Color{121, 139, 171, 255},
         TextAlignment::Left, "tn_local_status");
  }
};

REGISTER_EXAMPLE_SCREEN(secure_tunnel, "App Mockups",
                        "Tunnel client: desktop VPN map and local simulation",
                        SecureTunnelMockup)
