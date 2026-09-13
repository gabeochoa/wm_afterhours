#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include "DialogPresentation.h"
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

  bool show_details = false;
  std::string details_title;
  std::string details_body;
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
      {324.90f, 50.33f}, {351.21f, 111.71f}, {218.77f, 95.34f},
      {290.80f, 214.18f}, {729.27f, 304.84f}, {407.86f, 287.05f},
  }};

  ComponentConfig box(float scale, float x, float y, float w, float h) const {
    return ComponentConfig{}
        .with_size({pixels(w * scale), pixels(h * scale)})
        .with_absolute_position(x * scale, y * scale)
        .with_background(Theme::Usage::None).with_corner_radius(0);
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
    return "Disconnected";
  }

  std::string hero_title() const {
    if (link == Link::Up)
      return "Demo connection\nis active.";
    if (link == Link::Dialing)
      return "Simulating a\nconnection.";
    return "Your connection\nis unprotected.";
  }

  std::string connect_label() const {
    if (link == Link::Up)
      return "Disconnect";
    if (link == Link::Dialing)
      return "Connecting...";
    return "Connect selected server";
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

  }

  static void draw_rail_icon(RectangleType r, int icon, bool active) {
    const auto ink = active ? afterhours::Color{255, 255, 255, 255}
                            : afterhours::Color{152, 168, 197, 255};
    const float cx = r.x + r.width * .5f;
    const float cy = r.y + r.height * .5f;
    if (icon == 0) {
      afterhours::draw_rectangle({cx - 10, cy - 10, 20, 5}, ink);
      afterhours::draw_rectangle({cx - 3, cy - 5, 6, 18}, ink);
    } else if (icon == 1) {
      afterhours::draw_circle_lines(static_cast<int>(cx), static_cast<int>(cy), 11.f, ink);
      afterhours::draw_line_ex({cx - 11, cy}, {cx + 11, cy}, 2, ink);
      afterhours::draw_line_ex({cx, cy - 11}, {cx, cy + 11}, 2, ink);
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
      const auto red = afterhours::Color{215, 41, 58, 255};
      afterhours::draw_rectangle({r.x, r.y, r.width * .24f, r.height}, red);
      afterhours::draw_rectangle({r.x + r.width * .76f, r.y, r.width * .24f, r.height}, red);
      afterhours::draw_poly({r.x + r.width * .5f, r.y + r.height * .48f}, 5, r.height * .29f, -90, red);
      afterhours::draw_line_ex({r.x + r.width * .5f, r.y + r.height * .5f},
          {r.x + r.width * .5f, r.y + r.height * .86f}, r.width * .06f, red);
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
      const float radius = (i == selected ? 12.f : 6.f) * sx;
      if (i == selected)
        afterhours::draw_circle(static_cast<int>(x), static_cast<int>(y), 22 * sx,
            afterhours::Color{103, 137, 237, 42});
      afterhours::draw_circle(static_cast<int>(x), static_cast<int>(y), radius + 2 * sx,
          afterhours::Color{255, 255, 255, 255});
      afterhours::draw_circle(static_cast<int>(x), static_cast<int>(y), radius,
          afterhours::Color{62, 99, 219, 255});
    }
  }

  static void draw_shield(RectangleType r) {
    const auto ink = afterhours::Color{78, 108, 158, 255};
    const std::array<Vector2Type, 6> points{{
        {r.x + r.width * .5f, r.y}, {r.x + r.width, r.y + r.height * .18f},
        {r.x + r.width * .88f, r.y + r.height * .66f},
        {r.x + r.width * .5f, r.y + r.height},
        {r.x + r.width * .12f, r.y + r.height * .66f}, {r.x, r.y + r.height * .18f}}};
    for (size_t i = 0; i < points.size(); ++i)
      afterhours::draw_line_ex(points[i], points[(i + 1) % points.size()], r.width * .055f, ink);
    afterhours::draw_line_ex({r.x + r.width * .32f, r.y + r.height * .48f},
        {r.x + r.width * .47f, r.y + r.height * .62f}, r.width * .06f, ink);
    afterhours::draw_line_ex({r.x + r.width * .47f, r.y + r.height * .62f},
        {r.x + r.width * .72f, r.y + r.height * .34f}, r.width * .06f, ink);
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

    Theme theme;
    theme.font = {38, 54, 78, 255};
    theme.darkfont = {255, 255, 255, 255};
    theme.font_muted = {85, 102, 126, 255};
    theme.background = {237, 242, 249, 255};
    theme.surface = {255, 255, 255, 255};
    theme.primary = {62, 99, 219, 255};
    theme.secondary = {152, 168, 197, 255};
    theme.accent = {62, 99, 219, 255};
    theme.roundness = .04f;
    theme.segments = 8;
    context.theme = theme;
    context.scaling_mode = ScalingMode::Proportional;
    UIStylingDefaults::get().set_grid_snapping(false);
    UIStylingDefaults::get().set_default_font("AtkinsonMock", pixels(18.f * scale));

    auto root = div(context, mk(entity, 0),
                    box(scale, 0, 0, 1280, 720)
                        .with_absolute_position((screen_w - 1280 * scale) / 2, (screen_h - 720 * scale) / 2)
                        .with_custom_background(afterhours::Color{237, 242, 249, 255})
                        .with_debug_name("tn_root"));

    const auto text = [&](int id, std::string label, float x, float y, float w, float h,
                          float size, afterhours::Color color,
                          TextAlignment align = TextAlignment::Left,
                          const std::string &debug = "", const char *font = "AtkinsonMock") {
      div(context, mk(root.ent(), id),
          box(scale, x, y, w, h)
              .with_label(label)
              .with_font(font, pixels(size * scale))
              .with_custom_text_color(color)
              .with_alignment(align)
              .with_letter_spacing(0).with_ignore_pointer_events()
              .with_debug_name(debug));
    };
    const auto rect = [&](int id, float x, float y, float w, float h, afterhours::Color fill,
                          const std::string &debug = "") {
      div(context, mk(root.ent(), id),
          box(scale, x, y, w, h)
              .with_custom_background(fill)
              .with_debug_name(debug));
    };

    const auto navy = afterhours::Color{28, 39, 67, 255};
    const auto rail_active = afterhours::Color{52, 70, 106, 255};
    const auto ink = afterhours::Color{38, 54, 78, 255};
    const auto muted = afterhours::Color{85, 102, 126, 255};
    const auto pale = afterhours::Color{237, 241, 255, 255};
    const auto blue = afterhours::Color{62, 99, 219, 255};

    rect(10, 0, 0, 70, 720, navy, "tn_rail");
    for (int i = 0; i < 6; ++i) {
      const float y = i == 5 ? 638.f : 12.f + static_cast<float>(i) * 80.f;
      const bool active = i == 1;
      if (button(context, mk(root.ent(), 20 + i),
                 box(scale, 13, y, 45, 45)
                     .with_label("")
                     .with_custom_background(active ? rail_active : afterhours::Color{0, 0, 0, 0})
                     .with_corner_radius(7.f * scale)
                     .with_on_draw_fg([i, active](RectangleType r) { draw_rail_icon(r, i, active); })
                     .with_debug_name("tn_rail_" + std::to_string(i)))) {
        const char *labels[6] = {"Secure Tunnel", "VPN", "Protection: unavailable in demo",
                                 "Routing: unavailable in demo", "Diagnostics selected", "Settings: local demo only"};
        local_status = labels[i];
      }
      const char *names[] = {"Tunnel", "VPN", "Protect", "Routes", "Logs", "Setup"};
      text(700 + i, names[i], 4, y + 47, 62, 22, 13, afterhours::Color{217, 227, 244, 255}, TextAlignment::Center);
    }

    rect(30, 70, 0, 352, 720, afterhours::Color{255, 255, 255, 255}, "tn_sidebar");
    text(31, "Secure Tunnel", 93, 23, 290, 36, 27, ink, TextAlignment::Left,
         "tn_title", "AtkinsonMockBold");
    rect(32, 93, 69, 10, 10, link == Link::Up ? afterhours::Color{48, 160, 116, 255}
                                             : afterhours::Color{199, 102, 84, 255});
    text(33, state_word(link), 112, 63, 168, 24, 17, ink, TextAlignment::Left,
         "tn_state");
    text(738, "Demo mode", 299, 63, 100, 24, 14, blue, TextAlignment::Right, "tn_demo");
    text(34, hero_title(), 93, 90, 306, 64, 25, ink, TextAlignment::Left,
         "tn_hero", "AtkinsonMockBold");
    if (button(context, mk(root.ent(), 35),
               box(scale, 93, 157, 306, 44)
                   .with_label(connect_label())
                   .with_font("AtkinsonMockBold", pixels(18.f * scale))
                   .with_custom_text_color(afterhours::Color{255, 255, 255, 255})
                   .with_custom_background(link == Link::Up ? afterhours::Color{45, 144, 112, 255}
                                                            : blue)
                   .with_alignment(TextAlignment::Center)
                   .with_corner_radius(5.f * scale)
                   .with_debug_name("tn_connect"))) {
      toggle_connection();
    }
    text(36, std::string("Connect to ") + servers[server_idx].place,
         93, 205, 306, 22, 16, muted, TextAlignment::Center, "tn_quick_place");
    auto search = text_input(context, mk(root.ent(), 40), search_query,
                             box(scale, 93, 236, 306, 38)
                                 .with_font("AtkinsonMock", pixels(17.f * scale))
                                 .with_placeholder("Search locations")
                                 .with_custom_text_color(afterhours::Color{104, 114, 128, 255})
                                 .with_custom_background(afterhours::Color{247, 248, 250, 255})
                                 .with_border(afterhours::Color{136, 153, 178, 255}, 1.f)
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
    text(42, "LOCATIONS", 97, 281, 150, 24, 15, muted,
         TextAlignment::Left, "tn_locations_label", "AtkinsonMock");
    text(43, std::to_string(visible_server_count()) + " locations", 280, 281, 116, 24, 14, muted,
         TextAlignment::Right, "tn_locations_count");

    int visible_row = 0;
    for (size_t i = 0; i < servers.size(); ++i) {
      if (!server_visible(i))
        continue;
      const bool active = i == server_idx;
      const float y = 310.f + static_cast<float>(visible_row) * 46.f;
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
          box(scale, 103, y + 13, 28, 19)
              .with_on_draw_fg([i](RectangleType r) { draw_flag(r, i); })
              .with_ignore_pointer_events());
      text(90 + static_cast<int>(i), servers[i].place, 138, y + 3, 156, 22, 19,
           ink, TextAlignment::Left, "tn_server_name_" + std::to_string(i), "AtkinsonMockBold");
      text(100 + static_cast<int>(i), servers[i].country, 138, y + 25, 156, 20, 18,
           afterhours::Color{85, 102, 126, 255}, TextAlignment::Left,
           "tn_server_country_" + std::to_string(i));
      text(110 + static_cast<int>(i), fmt::format("{} ms", servers[i].ms), 299, y + 24, 74, 20, 18,
           afterhours::Color{85, 102, 126, 255}, TextAlignment::Right,
           "tn_server_ms_" + std::to_string(i));
      text(120 + static_cast<int>(i), active ? "Selected" : ">", 307, y + 3, 82, 20, 14,
           active ? blue : muted, TextAlignment::Right);
    }
    if (visible_row == 0) {
      text(130, "No locations found", 111, 318, 220, 24, 14,
           afterhours::Color{137, 145, 162, 255}, TextAlignment::Left,
           "tn_no_results", "AtkinsonMockBold");
      text(131, "Try city or country", 111, 344, 220, 18, 11,
           afterhours::Color{85, 102, 126, 255}, TextAlignment::Left,
           "tn_no_results_hint");
    }


    rect(140, 93, 591, 306, 1, afterhours::Color{229, 232, 240, 255}, "tn_protocol_rule");
    text(141, "Change protocol", 98, 611, 144, 24, 16, muted,
         TextAlignment::Left, "tn_protocol_label");
    if (button(context, mk(root.ent(), 142),
               box(scale, 242, 603, 157, 38)
                   .with_label(std::string(protocols[protocol_idx]) + "  >")
                   .with_font("AtkinsonMock", pixels(17.f * scale))
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
    text(201, "VPN locations", 449, 27, 240, 36, 26, ink,
         TextAlignment::Left, "tn_map_label");
    if (button(context, mk(root.ent(), 202),
               box(scale, 1119, 24, 130, 38)
                   .with_label("Account")
                   .with_font("AtkinsonMockBold", pixels(17.f * scale))
                   .with_custom_text_color(afterhours::Color{84, 116, 157, 255})
                   .with_custom_background(afterhours::Color{219, 228, 243, 255})
                   .with_corner_radius(16.f * scale)
                   .with_alignment(TextAlignment::Center)
                   .with_debug_name("tn_account"))) {
      show_details = true;
      details_title = "Demo account";
      details_body = "No account is signed in. This local preview does not contact a VPN provider.";
    }
    div(context, mk(root.ent(), 203),
        box(scale, 437, 141, 825, 390)
            .with_on_draw_fg([texture = map_texture, selected = server_idx](RectangleType r) {
              paint(texture, r);
              draw_server_dots(r, selected);
            })
            .with_debug_name("tn_world_map"));

    for (size_t i = 0; i < servers.size(); ++i) {
      if (i == server_idx) continue;
      const float x = 437 + server_points[i].x * 825 / 740;
      const float y = 141 + server_points[i].y * 390 / 350;
      text(750 + static_cast<int>(i), servers[i].place, std::clamp(x - 64, 445.f, 1120.f),
           y + 10, 128, 22, 14, ink, TextAlignment::Center);
    }
    const float marker_x = 437 + server_points[server_idx].x * 825 / 740;
    const float marker_y = 141 + server_points[server_idx].y * 390 / 350;
    const float pin_x = std::clamp(marker_x - 144, 449.f, 952.f);
    const float pin_y = std::clamp(marker_y + 28, 105.f, 432.f);
    div(context, mk(root.ent(), 209), box(scale, 0, 0, 1280, 720)
        .with_ignore_pointer_events().with_on_draw_fg([marker_x, marker_y, pin_x, pin_y, scale, blue](RectangleType r) {
          afterhours::draw_line_ex({r.x + marker_x * scale, r.y + marker_y * scale},
              {r.x + std::clamp(marker_x, pin_x + 12, pin_x + 276) * scale, r.y + pin_y * scale},
              2 * scale, blue);
        }));
    div(context, mk(root.ent(), 210), box(scale, pin_x, pin_y, 288, 70)
        .with_custom_background(afterhours::Color{255, 255, 255, 250})
        .with_corner_radius(7 * scale).with_debug_name("tn_location_card"));
    div(context, mk(root.ent(), 211), box(scale, pin_x + 14, pin_y + 13, 24, 16)
        .with_on_draw_fg([idx = server_idx](RectangleType r) { draw_flag(r, idx); })
        .with_ignore_pointer_events());
    text(212, servers[server_idx].place, pin_x + 46, pin_y + 6, 222, 28, 20, ink,
         TextAlignment::Left, "tn_location_name", "AtkinsonMockBold");
    text(213, fmt::format("{} / {} / {} ms", servers[server_idx].country,
         servers[server_idx].tag, servers[server_idx].ms), pin_x + 14, pin_y + 39, 260, 22,
         17, muted, TextAlignment::Left, "tn_location_detail");

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
    text(222, link == Link::Up ? "Demo connection active" : "Privacy, wherever you go.",
         692, 522, 390, 30, 23, ink, TextAlignment::Left, "tn_card_title", "AtkinsonMockBold");
    text(223, "Network simulation only. Your IP stays visible.", 692, 556, 390, 24, 16,
         muted, TextAlignment::Left, "tn_card_copy");
    text(224, "No real traffic is encrypted by this demo.", 692, 582, 390, 22, 15,
         muted, TextAlignment::Left, "tn_card_scope");

    rect(230, 422, 648, 858, 72, afterhours::Color{241, 245, 250, 255}, "tn_bottom");
    rect(231, 422, 648, 858, 1, afterhours::Color{220, 228, 238, 255});
    text(232, "IP address", 451, 657, 124, 22, 14, muted);
    text(233, "Not hidden", 451, 685, 144, 24, 18, afterhours::Color{165, 67, 49, 255},
         TextAlignment::Left, "tn_ip_value", "AtkinsonMockBold");
    text(234, "Demo connection", 625, 657, 300, 22, 14, muted);
    text(235, link == Link::Up ? selected_route() : "Not connected", 625, 685, 300, 24,
         18, ink, TextAlignment::Left, "tn_route_value");
    if (button(context, mk(root.ent(), 236), box(scale, 1022, 670, 226, 36)
        .with_label("View diagnostics  >").with_font("AtkinsonMock", pixels(18 * scale))
        .with_custom_text_color(blue).with_custom_background(afterhours::Color{0, 0, 0, 0})
        .with_debug_name("tn_diagnostics"))) {
      show_details = true;
      details_title = "Connection diagnostics";
      details_body = "Demo state: " + std::string(state_word(link)) + "\nSelected route: " + selected_route() +
          "\nProtocol: " + protocols[protocol_idx] + "\nNetwork simulation only. No traffic is routed.";
    }
    text(739, "Lowest latency: Reykjavik / 14 ms", 93, 649, 306, 22, 14, muted);
    text(237, local_status, 93, 675, 306, 38, 14, muted, TextAlignment::Left, "tn_local_status");
    auto details = afterhours::modal::info(context, mk(entity, 790), show_details,
        details_title, details_body, "Close");
    if (details) {
      const std::array<float, 1> widths{120};
      dialog_presentation::style(context, details.ent(), 620, 144, widths);
      details.ent().addComponentIfMissing<afterhours::HasColor>(navy).set(navy);
    }

  }
};

REGISTER_EXAMPLE_SCREEN(secure_tunnel, "App Mockups",
                        "Tunnel client: desktop VPN map and local simulation",
                        SecureTunnelMockup)
