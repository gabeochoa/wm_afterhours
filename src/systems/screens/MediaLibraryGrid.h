#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>
#include <afterhours/src/plugins/ui/text_input/text_input.h>
#include <algorithm>
#include <array>
#include <cctype>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct MediaLibraryGrid : ScreenSystem<UIContext<InputAction>> {
  struct Item {
    const char *title;
    const char *kind;
    int year;
    int mins;
    int art;   // Original library icon category.
    int stars; // 1..5
    const char *blurb;
    const char *credit;
  };

  static constexpr Item ITEMS[24] = {
      {"Harbour Lights", "Feature", 2019, 118, 0, 4,
       "A lighthouse keeper counts the ships home.", "dir. Ana Weir"},
      {"The Long Ascent", "Feature", 2021, 141, 0, 5,
       "Two climbers, one rope, and a closing season.", "dir. Piet Oskarsson"},
      {"Salt and Iron", "Series", 2020, 46, 3, 3,
       "A shipyard town votes on its own closure.", "6 episodes"},
      {"Cold Orbit", "Feature", 2018, 96, 4, 4,
       "The relay station stops answering.", "dir. Mira Halvorsen"},
      {"Field Recordings", "Album", 2022, 52, 1, 5,
       "Tape loops gathered across four winters.", "The Quiet Hours"},
      {"Nine Winters", "Series", 2017, 51, 3, 2,
       "A family ledger, read one year at a time.", "9 episodes"},
      {"Paper Kingdoms", "Feature", 2023, 107, 0, 4,
       "A mapmaker invents a country and sells it.", "dir. Owen Cassidy"},
      {"Static Bloom", "Album", 2021, 38, 1, 3,
       "Guitar through a broken spring reverb.", "Lantern Field"},
      {"The Quiet Wire", "Series", 2022, 44, 3, 5,
       "A switchboard operator hears too much.", "8 episodes"},
      {"Ledger of Hours", "Feature", 2016, 133, 4, 3,
       "One clerk, forty years, the same desk.", "dir. Tomas Rey"},
      {"Brass Season", "Album", 2020, 61, 1, 4,
       "A marching band recorded in an empty pool.", "Northsound Nine"},
      {"Understory", "Series", 2024, 39, 3, 4,
       "What grows back after the loggers leave.", "5 episodes"},
      {"Ember Coast", "Feature", 2015, 89, 0, 2,
       "A wildfire season told from the water.", "dir. Lise Marchand"},
      {"Signal Garden", "Album", 2023, 47, 1, 5,
       "Modular synths tuned to birdsong.", "Ivy Constant"},
      {"Tin Almanac", "Series", 2019, 55, 3, 3,
       "Twelve months in a hardware shop.", "12 episodes"},
      {"Northbound", "Feature", 2022, 124, 4, 5,
       "A night train, a stolen case, no plan.", "dir. Ruth Okonkwo"},
      {"Glass Meridian", "Feature", 2020, 101, 0, 3,
       "The greenhouse outlives everyone in it.", "dir. Sander Vos"},
      {"Low Tide Choir", "Album", 2018, 44, 1, 4,
       "Voices recorded on the flats at dawn.", "Harbourmouth"},
      {"The Fifth Room", "Series", 2021, 48, 3, 4,
       "Four doors are mapped. The fifth is not.", "7 episodes"},
      {"Ash and Amber", "Feature", 2024, 112, 2, 5,
       "A glassblower and the fire she keeps.", "dir. Nadia Ferro"},
      {"Quarry Songs", "Album", 2017, 57, 1, 2,
       "Percussion cut from stone and echo.", "Deepcut Ensemble"},
      {"Every Lit Window", "Series", 2023, 42, 5, 4,
       "One building, one night, thirty stories.", "10 episodes"},
      {"Meridian Fault", "Feature", 2019, 95, 4, 3,
       "The survey crew disagrees about the line.", "dir. Cole Adeyemi"},
      {"Slow Cartography", "Album", 2025, 63, 1, 5,
       "Ambient pieces named after old roads.", "Fen and Ridge"},
  };

  enum class Section { Library, Movies, Television, Music, Watchlist };
  enum class View { Recommended, Library, Collections };
  enum class Overlay { None, Player, Server, Settings, Profile };
  Section section = Section::Library;
  View view = View::Recommended;
  Overlay overlay = Overlay::None;
  int page = 0, selected = 0, media_filter = 0, sort = 0;
  bool list_view = false, playing = false, loaded = false;
  float progress = 0, scale = 1;
  int playing_item = 0;
  std::string query, previous_query;
  std::array<bool, 24> watchlist{};
  std::array<raylib::Texture2D, 7> art{};
  std::array<afterhours::EntityID, 6> tile_ids{{-1, -1, -1, -1, -1, -1}};
  const afterhours::Color ink{232, 232, 232, 255}, muted{149, 151, 156, 255};
  const afterhours::Color gold{227, 170, 42, 255}, transparent{0, 0, 0, 0};

  void load() {
    if (loaded) return;
    for (size_t i = 0; i < art.size(); ++i) {
      const std::string name = i == 6 ? "background" : "poster_" + std::to_string(i);
      art[i] = raylib::LoadTexture(afterhours::files::get_resource_path("images",
          "media_library/" + name + ".png").string().c_str());
      raylib::SetTextureFilter(art[i], raylib::TEXTURE_FILTER_BILINEAR);
    }
    loaded = true;
  }
  static std::string lower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });
    return s;
  }
  static std::string upper(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char ch) { return static_cast<char>(std::toupper(ch)); });
    return s;
  }
  std::vector<int> matching() const {
    std::vector<int> ids;
    for (int i = 0; i < 24; ++i) {
      const auto &item = ITEMS[i];
      if (section == Section::Movies && item.kind[0] != 'F') continue;
      if (section == Section::Television && item.kind[0] != 'S') continue;
      if (section == Section::Music && item.kind[0] != 'A') continue;
      if (section == Section::Watchlist && !watchlist[i]) continue;
      if (media_filter > 0 && item.kind[0] != std::array<char, 4>{' ', 'F', 'S', 'A'}[media_filter]) continue;
      const auto haystack = lower(fmt::format("{} {} {} {} {}", item.title, item.kind, item.year, item.blurb, item.credit));
      if (!query.empty() && haystack.find(lower(query)) == std::string::npos) continue;
      ids.push_back(i);
    }
    if (sort != 0 || view == View::Library)
      std::stable_sort(ids.begin(), ids.end(), [this](int a, int b) {
        if (sort == 1) return ITEMS[a].year > ITEMS[b].year;
        if (sort == 2) return ITEMS[a].stars > ITEMS[b].stars;
        return std::string(ITEMS[a].title) < ITEMS[b].title;
      });
    return ids;
  }
  ComponentConfig box(float x, float y, float w, float h) const {
    return ComponentConfig{}.with_size({pixels(w * scale), pixels(h * scale)})
        .with_absolute_position(x * scale, y * scale).with_background(Theme::Usage::None)
        .with_corner_radius(0);
  }
  ElementResult label(UIContext<InputAction> &c, afterhours::Entity &p, int id,
                      const std::string &text, float x, float y, float w, float h,
                      float size, afterhours::Color color, const std::string &name = "",
                      bool strong = false) {
    return div(c, mk(p, id), box(x, y, w, h).with_label(text)
        .with_font(strong ? "ArchivoMockBold" : "AtkinsonMock", h720(size * (size < 18 ? 1.4f : 1.25f))).with_letter_spacing(-.7f * scale)
        .with_custom_text_color(color).with_alignment(TextAlignment::Left)
        .with_ignore_pointer_events().with_debug_name(name));
  }
  ElementResult action(UIContext<InputAction> &c, afterhours::Entity &p, int id,
                       const std::string &text, float x, float y, float w, float h,
                       const std::string &name, bool active = false, bool filled = false) {
    const bool modal_action = name.starts_with("ml_overlay_");
    return button(c, mk(p, id), box(x, y, w, h).with_label(text)
        .with_font("AtkinsonMock", h720(16.25f)).with_letter_spacing(-.7f * scale).with_alignment(TextAlignment::Center)
        .with_custom_text_color(filled ? afterhours::Color{35, 37, 37, 255} : active ? gold : muted)
        .with_custom_background(filled ? gold : active ? afterhours::Color{57, 55, 49, 255} : transparent)
        .with_corner_radius(4 * scale).with_disabled(overlay != Overlay::None && !modal_action)
        .with_click_activation(ClickActivationMode::Release).with_debug_name(name));
  }
  void image(UIContext<InputAction> &c, afterhours::Entity &p, int id, int i,
             float x, float y, float w, float h) {
    div(c, mk(p, id), box(x, y, w, h).with_ignore_pointer_events()
        .with_on_draw_bg([texture = art[i]](RectangleType r) {
          raylib::DrawTexturePro(texture, {0, 0, static_cast<float>(texture.width),
              static_cast<float>(texture.height)}, r, {0, 0}, 0, raylib::WHITE);
        }));
  }
  void poster(UIContext<InputAction> &c, afterhours::Entity &p, int id, int item,
              float x, float y, float w, float h, bool active) {
    auto frame = div(c, mk(p, id), box(x, y, w, h).with_custom_background({40, 43, 46, 255})
        .with_border(active ? gold : transparent, active ? 3 * scale : 0)
        .with_ignore_pointer_events().with_debug_name("ml_poster_" + std::to_string(item)));
    const float unit = std::min(w / 200, h / 260), ox = (w - unit * 200) / 2;
    image(c, frame.ent(), 0, item % 6, ox, 0, unit * 200, unit * 260);
    const bool dark = item % 6 == 1 || item % 6 == 4 || item % 6 == 5;
    const float ty = item % 6 == 4 ? 33 : 212;
    if (unit >= .5f) div(c, mk(frame.ent(), 1), box(ox + 8 * unit, ty * unit, 184 * unit, 25 * unit)
        .with_label(upper(ITEMS[item].title)).with_font("Garamond", h720((std::string(ITEMS[item].title).size() > 15 ? 13 : 17) * unit * 1.25f))
        .with_custom_text_color(dark ? afterhours::Color{40, 56, 60, 255} : afterhours::Color{246, 236, 219, 255})
        .with_alignment(TextAlignment::Center).with_ignore_pointer_events());
    if (unit >= .9f) div(c, mk(frame.ent(), 2), box(ox + 10 * unit, (ty + 29) * unit, 180 * unit, 10 * unit)
        .with_label(ITEMS[item].kind[0] == 'A' ? upper(ITEMS[item].credit) : "A FILM TO GET LOST IN")
        .with_font("AtkinsonMock", h720(5.5f * unit * 1.25f))
        .with_custom_text_color(dark ? muted : afterhours::Color{182, 191, 194, 255})
        .with_alignment(TextAlignment::Center).with_ignore_pointer_events());
  }

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &c, float dt) override {
    load(); scale = c.screen_height / 720.f;
    Theme theme; theme.font = ink; theme.darkfont = ink; theme.background = {32, 33, 35, 255};
    theme.surface = {40, 43, 46, 255}; theme.primary = gold; theme.accent = gold; theme.corner_radius = 0; theme.roundness = 0;
    c.set_theme(theme); c.scaling_mode = ScalingMode::Proportional;
    UIStylingDefaults::get().set_default_font("AtkinsonMock", h720(17.5f));
    if (c.pressed(InputAction::MenuBack)) { overlay = Overlay::None; playing = false; }
    if (playing && overlay == Overlay::Player) progress = std::min(progress + std::min(dt, .1f), ITEMS[playing_item].mins * 60.f);
    auto root = div(c, mk(entity), box(0, 0, 1280, 720).with_debug_name("ml_bg"));
    auto &p = root.ent(); image(c, p, 900, 6, 0, 0, 1280, 720);
    div(c, mk(p, 0), box(0, 0, 1280, 66).with_custom_background({23, 24, 25, 245}));
    label(c, p, 1, "media", 25, 13, 125, 45, 29, ink, "ml_brand", true);
    label(c, p, 2, ">", 100, 9, 43, 49, 37, gold);
    auto input = text_input(c, mk(p, 3), query, box(233, 17, 362, 32)
        .with_custom_background({50, 51, 53, 255}).with_custom_text_color(ink)
        .with_font("AtkinsonMock", h720(15)).with_corner_radius(4 * scale)
        .with_disabled(overlay != Overlay::None).with_debug_name("ml_search"));
    if (query.empty() && overlay == Overlay::None)
      div(c, mk(p, 4), box(263, 23, 314, 24).with_label("Search your library")
          .with_font("AtkinsonMock", h720(15)).with_custom_text_color(muted)
          .with_render_layer(10).with_ignore_pointer_events());
    if (query != previous_query) { previous_query = query; page = 0; }
    if (action(c, p, 5, "Home", 1010, 20, 54, 28, "ml_home")) { section = Section::Library; view = View::Recommended; media_filter = 0; page = 0; }
    if (action(c, p, 6, "Server", 1070, 20, 57, 28, "ml_server")) overlay = Overlay::Server;
    if (action(c, p, 7, "Settings", 1133, 20, 68, 28, "ml_settings")) overlay = Overlay::Settings;
    if (action(c, p, 8, "G", 1220, 18, 31, 31, "ml_profile", true)) overlay = Overlay::Profile;
    div(c, mk(p, 10), box(0, 66, 202, 654).with_custom_background({25, 26, 28, 102}));
    label(c, p, 11, "YOUR MEDIA", 28, 92, 152, 19, 9, {117, 119, 123, 255}, "", true);
    const std::array<const char *, 5> sections{"Library", "Movies", "TV Shows", "Music", "Watchlist"};
    for (int i = 0; i < 5; ++i) {
      const bool active = static_cast<int>(section) == i;
      if (action(c, p, 20 + i, sections[i], 12, 123 + i * 44, 178, 40, "ml_nav_" + std::to_string(i), active)) {
        section = static_cast<Section>(i); page = 0; media_filter = 0;
        if (view == View::Collections) view = View::Recommended;
      }
      if (active) div(c, mk(p, 30 + i), box(12, 125 + i * 44, 3, 36).with_custom_background(gold));
    }
    div(c, mk(p, 40), box(27, 363, 148, 1).with_custom_background({65, 66, 70, 255}));
    label(c, p, 41, "SERVER", 28, 386, 152, 19, 9, {117, 119, 123, 255}, "", true);
    label(c, p, 42, "Home server", 29, 419, 160, 22, 12, muted);
    label(c, p, 43, "Local demo library", 33, 446, 152, 18, 10, {98, 102, 107, 255});
    auto ids = matching();
    const int total = static_cast<int>(ids.size()), page_count = std::max(1, (total + 5) / 6);
    page = std::clamp(page, 0, page_count - 1);
    if (!ids.empty() && std::find(ids.begin(), ids.end(), selected) == ids.end()) selected = ids[0];
    label(c, p, 50, sections[static_cast<int>(section)], 228, 89, 500, 40, 26, ink, "ml_title");
    label(c, p, 51, std::to_string(total), 348, 99, 65, 22, 12, {119, 119, 119, 255});
    const std::array<const char *, 3> views{"Recommended", "Library", "Collections"};
    for (int i = 0; i < 3; ++i) {
      if (action(c, p, 60 + i, views[i], 228 + i * 126, 136, 110, 37, "ml_view_" + std::to_string(i), false)) { view = static_cast<View>(i); page = 0; }
      if (static_cast<int>(view) == i) div(c, mk(p, 65 + i), box(230 + i * 126, 173, 106, 2).with_custom_background(gold));
    }
    const std::array<const char *, 4> kinds{"All media", "Features", "Series", "Albums"};
    const std::array<const char *, 3> sorts{"By title", "Newest first", "Highest rated"};
    if (action(c, p, 70, kinds[media_filter], 222, 181, 111, 30, "ml_filter")) { media_filter = (media_filter + 1) % 4; page = 0; }
    if (action(c, p, 71, sorts[sort], 344, 181, 119, 30, "ml_sort")) { sort = (sort + 1) % 3; page = 0; }
    if (action(c, p, 72, "Grid", 816, 181, 42, 30, "ml_grid", !list_view)) list_view = false;
    if (action(c, p, 73, "List", 866, 181, 42, 30, "ml_list", list_view)) list_view = true;
    tile_ids.fill(-1);
    if (view == View::Collections) {
      for (int i = 0; i < 3; ++i) {
        const float x = 228 + i * 234;
        poster(c, p, 80 + i, i == 0 ? 0 : i == 1 ? 2 : 4, x, 232, 211, 260, false);
        if (action(c, p, 90 + i, kinds[i + 1], x, 509, 211, 42, "ml_collection_" + std::to_string(i))) {
          section = Section::Library; media_filter = i + 1; view = View::Recommended; page = 0;
        }
      }
    } else {
      for (int slot = 0; slot < 6 && page * 6 + slot < total; ++slot) {
        const int idx = ids[page * 6 + slot]; const auto &item = ITEMS[idx];
        const float x = list_view ? 228 : 228 + (slot % 3) * 234;
        const float y = list_view ? 215 + slot * 69 : 215 + (slot / 3) * 231;
        const float w = list_view ? 682 : 215, h = list_view ? 64 : 219;
        auto tile = button(c, mk(p, 100 + slot), box(x, y, w, h)
            .with_padding(Padding{.top = pixels(0), .left = pixels(0), .bottom = pixels(0), .right = pixels(0)})
            .with_custom_background(list_view && selected == idx ? afterhours::Color{57, 55, 49, 255} : transparent)
            .with_disabled(overlay != Overlay::None).with_click_activation(ClickActivationMode::Release)
            .with_debug_name("ml_tile_" + std::to_string(slot)));
        tile.cmp().set_desired_padding(pixels(0), Axis::X).set_desired_padding(pixels(0), Axis::Y);
        tile_ids[slot] = tile.ent().id;
        if (tile) selected = idx;
        poster(c, tile.ent(), 0, idx, 0, 0, list_view ? 51 : 215, list_view ? 64 : 169, selected == idx);
        label(c, tile.ent(), 1, item.title, list_view ? 69 : 0, list_view ? 5 : 179,
              list_view ? 405 : 215, 23, list_view ? 16 : 12, ink, "ml_name_" + std::to_string(slot));
        label(c, tile.ent(), 2, fmt::format("{}", item.year), list_view ? 69 : 0, list_view ? 35 : 203,
              97, 16, 10, muted, "ml_kind_" + std::to_string(slot));
        label(c, tile.ent(), 3, item.kind, list_view ? 438 : 166, list_view ? 24 : 203,
              67, 16, 10, muted);
        if (list_view) label(c, tile.ent(), 4, fmt::format("{} min", item.mins), 550, 24, 115, 21, 12, muted);
      }
      if (ids.empty()) label(c, p, 115, section == Section::Watchlist ? "Your watchlist is empty. Add a title with +." : "No titles match your search and filters.",
          250, 338, 645, 60, 20, muted, "ml_empty");
      label(c, p, 120, total == 0 ? "Showing 0 of 0" : fmt::format("Showing {}-{} of {}", page * 6 + 1, std::min(page * 6 + 6, total), total),
            228, 681, 435, 20, 10, {118, 123, 129, 255}, "ml_readout");
      for (int i = 0; i < page_count; ++i)
        if (action(c, p, 130 + i, std::to_string(i + 1), 792 + i * 30, 674, 26, 26,
                   "ml_page_" + std::to_string(i), page == i, page == i)) { page = i; selected = ids[i * 6]; }
    }
    int shown = selected;
    for (int slot = 0; slot < 6 && page * 6 + slot < total; ++slot)
      if (tile_ids[slot] >= 0 && c.was_hot(tile_ids[slot])) shown = ids[page * 6 + slot];
    if (!ids.empty()) build_detail(c, p, shown);
    if (overlay != Overlay::None) build_overlay(c, p);
  }

  void build_detail(UIContext<InputAction> &c, afterhours::Entity &p, int shown) {
    const auto &item = ITEMS[shown];
    div(c, mk(p, 300), box(937, 66, 343, 654).with_custom_background({20, 23, 25, 85}).with_debug_name("ml_detail"));
    poster(c, p, 301, shown, 1009, 95, 198, 258, false);
    label(c, p, 302, item.title, 965, 377, 287, 43, 24, ink, "ml_detail_title");
    label(c, p, 303, fmt::format("{}  -  {} min  -  {}", item.year, item.mins, item.kind),
          965, 419, 287, 22, 10, muted, "ml_detail_meta");
    // Each star is native geometry, so the rating does not depend on a symbol font.
    div(c, mk(p, 304), box(965, 454, 136, 22).with_ignore_pointer_events()
        .with_on_draw_fg([stars = item.stars](RectangleType r) {
          const float s = r.height / 22;
          for (int i = 0; i < 5; ++i) {
            const raylib::Vector2 center{r.x + (10 + i * 25) * s, r.y + 11 * s};
            const auto color = i < stars ? raylib::Color{229, 174, 51, 255} : raylib::Color{87, 89, 91, 255};
            for (int point = 0; point < 10; ++point) {
              const float a = -1.57079633f + point * .62831853f, b = a + .62831853f;
              const float ra = (point % 2 ? 4 : 10) * s, rb = (point % 2 ? 10 : 4) * s;
              raylib::DrawTriangle(center, {center.x + std::cos(b) * rb, center.y + std::sin(b) * rb},
                  {center.x + std::cos(a) * ra, center.y + std::sin(a) * ra}, color);
            }
          }
        }).with_debug_name("ml_detail_stars"));
    div(c, mk(p, 305), box(965, 480, 287, 41).with_label(item.blurb)
        .with_font("AtkinsonMock", h720(16.25f)).with_letter_spacing(-.7f * scale).with_custom_text_color({180, 181, 183, 255})
        .with_text_overflow(TextOverflow::Wrap).with_ignore_pointer_events().with_debug_name("ml_detail_blurb"));
    label(c, p, 306, item.credit, 965, 514, 287, 23, 11, {116, 122, 128, 255}, "ml_detail_credit");
    if (action(c, p, 307, "Play", 965, 549, 140, 36, "ml_play", false, true)) {
      playing_item = shown; progress = 0; playing = true; overlay = Overlay::Player;
    }
    if (action(c, p, 308, watchlist[shown] ? "-" : "+", 1115, 549, 41, 36, "ml_queue", watchlist[shown], watchlist[shown])) watchlist[shown] = !watchlist[shown];
    if (watchlist[shown]) label(c, p, 309, "In your watchlist", 965, 594, 249, 23, 11, gold, "ml_saved");
  }

  void build_overlay(UIContext<InputAction> &c, afterhours::Entity &p) {
    div(c, mk(p, 700), box(0, 0, 1280, 720).with_custom_background({0, 0, 0, 185}).with_overlay(5));
    auto panel = div(c, mk(p, 701), box(300, 135, 680, 450).with_custom_background({28, 30, 32, 255})
        .with_border({74, 77, 81, 255}, scale).with_corner_radius(8 * scale).with_overlay(6).with_debug_name("ml_overlay"));
    auto &q = panel.ent();
    const std::string heading = overlay == Overlay::Player ? ITEMS[playing_item].title :
        overlay == Overlay::Server ? "Home server" : overlay == Overlay::Settings ? "Library settings" : "Guest profile";
    label(c, q, 0, heading, 30, 24, 570, 49, 28, ink, "ml_overlay_title");
    if (action(c, q, 1, "Close", 584, 29, 67, 32, "ml_overlay_close")) { overlay = Overlay::None; playing = false; }
    if (overlay == Overlay::Player) {
      poster(c, q, 10, playing_item, 38, 94, 168, 218, false);
      label(c, q, 11, "LOCAL PLAYBACK PREVIEW", 239, 108, 405, 31, 16, gold);
      div(c, mk(q, 12), box(239, 152, 400, 91).with_label("No media file is attached. These controls simulate playback locally so you can review the player interaction.")
          .with_font("AtkinsonMock", h720(18.75f)).with_custom_text_color(muted).with_text_overflow(TextOverflow::Wrap));
      label(c, q, 13, playing ? "Playing preview" : "Preview paused", 239, 260, 400, 32, 19, ink, "ml_playback_state");
      const float length = ITEMS[playing_item].mins * 60.f;
      div(c, mk(q, 14), box(38, 343, 604, 8).with_custom_background({66, 68, 71, 255})
          .with_on_draw_fg([fraction = progress / length](RectangleType r) {
            r.width *= fraction; afterhours::draw_rectangle(r, {227, 170, 42, 255});
          }).with_debug_name("ml_progress"));
      label(c, q, 15, fmt::format("{}:{:02} / {}:00", static_cast<int>(progress) / 60, static_cast<int>(progress) % 60, ITEMS[playing_item].mins),
            38, 357, 312, 23, 12, muted, "ml_playback_time");
      if (action(c, q, 16, "-10 sec", 239, 387, 100, 35, "ml_overlay_rewind")) progress = std::max(0.f, progress - 10);
      if (action(c, q, 17, playing ? "Pause" : "Resume", 352, 387, 140, 35, "ml_overlay_pause", false, true)) playing = !playing;
      if (action(c, q, 18, "+10 sec", 505, 387, 100, 35, "ml_overlay_forward")) progress = std::min(length, progress + 10);
    } else if (overlay == Overlay::Server) {
      label(c, q, 20, "24 titles available in this local demo", 32, 106, 610, 38, 23, ink);
      label(c, q, 21, "No remote server is connected.", 32, 155, 610, 32, 17, muted);
      label(c, q, 22, "All metadata and poster illustrations are bundled locally.", 32, 199, 610, 32, 17, muted);
    } else if (overlay == Overlay::Settings) {
      label(c, q, 30, "Library layout", 32, 113, 286, 35, 21, ink);
      if (action(c, q, 31, list_view ? "List" : "Grid", 349, 110, 291, 40, "ml_overlay_layout", true)) list_view = !list_view;
      label(c, q, 32, "Sort order", 32, 180, 286, 35, 21, ink);
      const std::array<const char *, 3> sorts{"By title", "Newest first", "Highest rated"};
      if (action(c, q, 33, sorts[sort], 349, 177, 291, 40, "ml_overlay_sort", true)) { sort = (sort + 1) % 3; page = 0; }
      label(c, q, 34, "Preferences are kept while this screen is open.", 32, 271, 610, 35, 16, muted);
    } else if (overlay == Overlay::Profile) {
      const int count = static_cast<int>(std::count(watchlist.begin(), watchlist.end(), true));
      label(c, q, 40, "Local guest", 32, 110, 610, 45, 28, ink);
      label(c, q, 41, fmt::format("{} titles saved to your watchlist", count), 32, 172, 610, 32, 18, gold);
      label(c, q, 42, "No account or network connection is required.", 32, 225, 610, 32, 17, muted);
      if (action(c, q, 43, "Open watchlist", 32, 318, 224, 42, "ml_overlay_watchlist", false, true)) {
        section = Section::Watchlist; media_filter = 0; page = 0; overlay = Overlay::None;
      }
    }
  }
};

REGISTER_EXAMPLE_SCREEN(media_library, "App Mockups", "Paged poster wall, only the current page is built", MediaLibraryGrid)
