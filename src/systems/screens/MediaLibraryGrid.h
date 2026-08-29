#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// A paged poster wall. Only the current page is built, so the readout at the
// bottom reports how many of the 24 entries actually exist as widgets right
// now, which is the part a static mock cannot show you.
struct MediaLibraryGrid : ScreenSystem<UIContext<InputAction>> {
  using Rect = afterhours::texture_manager::Rectangle;

  struct Item {
    const char *title;
    const char *kind;
    int year;
    int mins;
    int art;   // index into ART
    int stars; // 1..5
  };

  static constexpr int COLS = 3;
  static constexpr int ROWS = 2;
  static constexpr int PER_PAGE = COLS * ROWS;
  static constexpr float TILE_W = 240.f;
  static constexpr float TILE_H = 212.f;
  static constexpr float GAP = 16.f;
  static constexpr float GRID_X = 48.f;
  static constexpr float GRID_Y = 88.f;

  // Frames into sheet_white2x, so icon_row gets one real sheet and distinct
  // rects rather than the same icon repeated.
  static constexpr Rect ART[6] = {
      {0, 1900, 100, 100},   // video
      {200, 1400, 100, 100}, // musicOn
      {100, 400, 100, 100},  // trophy
      {200, 1600, 100, 100}, // multiplayer
      {100, 1200, 100, 100}, // singleplayer
      {300, 1600, 100, 100}, // home
  };
  static constexpr Rect BADGE_STAR{100, 1000, 100, 100};
  static constexpr Rect BADGE_CHECK{400, 1400, 100, 100};
  static constexpr Rect BADGE_LOCK{300, 500, 100, 100};

  static constexpr Item ITEMS[24] = {
      {"Harbour Lights", "Feature", 2019, 118, 0, 4},
      {"The Long Ascent", "Feature", 2021, 141, 0, 5},
      {"Salt and Iron", "Series", 2020, 46, 3, 3},
      {"Cold Orbit", "Feature", 2018, 96, 4, 4},
      {"Field Recordings", "Album", 2022, 52, 1, 5},
      {"Nine Winters", "Series", 2017, 51, 3, 2},
      {"Paper Kingdoms", "Feature", 2023, 107, 0, 4},
      {"Static Bloom", "Album", 2021, 38, 1, 3},
      {"The Quiet Wire", "Series", 2022, 44, 3, 5},
      {"Ledger of Hours", "Feature", 2016, 133, 4, 3},
      {"Brass Season", "Album", 2020, 61, 1, 4},
      {"Understory", "Series", 2024, 39, 3, 4},
      {"Ember Coast", "Feature", 2015, 89, 0, 2},
      {"Signal Garden", "Album", 2023, 47, 1, 5},
      {"Tin Almanac", "Series", 2019, 55, 3, 3},
      {"Northbound", "Feature", 2022, 124, 4, 5},
      {"Glass Meridian", "Feature", 2020, 101, 0, 3},
      {"Low Tide Choir", "Album", 2018, 44, 1, 4},
      {"The Fifth Room", "Series", 2021, 48, 3, 4},
      {"Ash and Amber", "Feature", 2024, 112, 2, 5},
      {"Quarry Songs", "Album", 2017, 57, 1, 2},
      {"Every Lit Window", "Series", 2023, 42, 5, 4},
      {"Meridian Fault", "Feature", 2019, 95, 4, 3},
      {"Slow Cartography", "Album", 2025, 63, 1, 5},
  };

  bool loaded = false;
  raylib::Texture2D sheet{};
  size_t page = 0;
  std::vector<std::string> page_labels = {"1", "2", "3", "4"};
  int selected = 0;
  afterhours::EntityID tile_id[PER_PAGE] = {-1, -1, -1, -1, -1, -1};

  void load() {
    if (loaded)
      return;
    loaded = true;
    const std::string p =
        afterhours::files::get_resource_path(
            "kenney/kenney_game-icons/Spritesheet/", "sheet_white2x.png")
            .string();
    sheet = raylib::LoadTexture(p.c_str());
  }

  static afterhours::Color kind_plate(const char *kind) {
    if (kind[0] == 'S')
      return afterhours::Color{52, 66, 96, 255};
    if (kind[0] == 'A')
      return afterhours::Color{74, 52, 88, 255};
    return afterhours::Color{44, 72, 76, 255};
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    load();

    context.theme = afterhours::ui::theme_presets::midnight();
    context.scaling_mode = ScalingMode::Adaptive;

    const auto ink = afterhours::Color{234, 238, 246, 255};
    const auto muted = afterhours::Color{160, 170, 192, 255};
    const auto panel = afterhours::Color{30, 34, 46, 255};
    const auto tile_bg = afterhours::Color{40, 45, 60, 255};
    const auto tile_pick = afterhours::Color{62, 82, 118, 255};

    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(1280), pixels(720)})
            .with_absolute_position(0.f, 0.f)
            .with_custom_background(afterhours::Color{18, 20, 28, 255})
            .with_debug_name("ml_bg"));

    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_label("Media Library")
            .with_size(ComponentSize{pixels(600), pixels(34)})
            .with_absolute_position(GRID_X, 32.f)
            .with_alignment(TextAlignment::Left)
            .with_font_size(26.f)
            .with_custom_text_color(ink)
            .with_debug_name("ml_title"));

    const int first = (int)page * PER_PAGE;

    for (int slot = 0; slot < PER_PAGE; slot++) {
      const int idx = first + slot;
      if (idx >= 24)
        break;
      const Item &it = ITEMS[idx];
      const float tx = GRID_X + (float)(slot % COLS) * (TILE_W + GAP);
      const float ty = GRID_Y + (float)(slot / COLS) * (TILE_H + GAP);
      const bool picked = idx == selected;

      auto tile = button(context, mk(entity, 100 + slot),
                         ComponentConfig{}
                             .with_size(ComponentSize{pixels(TILE_W),
                                                      pixels(TILE_H)})
                             .with_absolute_position(tx, ty)
                             .with_custom_background(picked ? tile_pick
                                                            : tile_bg)
                             .with_padding(Padding{.top = pixels(0),
                                                   .left = pixels(0),
                                                   .bottom = pixels(0),
                                                   .right = pixels(0)})
                             .with_roundness(0.05f)
                             .with_debug_name(fmt::format("ml_tile_{}", slot)));
      tile_id[slot] = tile.ent().id;
      if (tile)
        selected = idx;

      // Poster plate plus its art. Children ignore the pointer so the tile
      // keeps the hit for hover.
      div(context, mk(tile.ent(), 0),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(TILE_W), pixels(116)})
              .with_absolute_position(0.f, 0.f)
              .with_custom_background(kind_plate(it.kind))
              .with_ignore_pointer_events(true)
              .with_debug_name(fmt::format("ml_plate_{}", slot)));

      sprite(context, mk(tile.ent(), 1), sheet, ART[it.art],
             ComponentConfig{}
                 .with_size(ComponentSize{pixels(60), pixels(60)})
                 .with_absolute_position(TILE_W / 2.f - 30.f, 28.f)
                 .with_ignore_pointer_events(true)
                 .with_debug_name(fmt::format("ml_art_{}", slot)));

      div(context, mk(tile.ent(), 2),
          ComponentConfig{}
              .with_label(it.title)
              .with_size(ComponentSize{pixels(216), pixels(26)})
              .with_absolute_position(12.f, 124.f)
              .with_alignment(TextAlignment::Left)
              .with_font_size(17.f)
              .with_custom_text_color(ink)
              .with_text_overflow(TextOverflow::Ellipsis)
              .with_ignore_pointer_events(true)
              .with_debug_name(fmt::format("ml_name_{}", slot)));

      div(context, mk(tile.ent(), 3),
          ComponentConfig{}
              .with_label(fmt::format("{}  {}", it.kind, it.year))
              .with_size(ComponentSize{pixels(140), pixels(22)})
              .with_absolute_position(12.f, 150.f)
              .with_alignment(TextAlignment::Left)
              .with_font_size(16.f)
              .with_custom_text_color(muted)
              .with_ignore_pointer_events(true)
              .with_debug_name(fmt::format("ml_kind_{}", slot)));

      // Runtime is a number, so it lines up on the right edge of the tile.
      div(context, mk(tile.ent(), 4),
          ComponentConfig{}
              .with_label(fmt::format("{} min", it.mins))
              .with_size(ComponentSize{pixels(76), pixels(22)})
              .with_absolute_position(152.f, 150.f)
              .with_alignment(TextAlignment::Right)
              .with_font_size(16.f)
              .with_custom_text_color(muted)
              .with_ignore_pointer_events(true)
              .with_debug_name(fmt::format("ml_mins_{}", slot)));

      std::vector<Rect> badges{BADGE_STAR};
      if (it.stars >= 4)
        badges.push_back(BADGE_CHECK);
      if (it.kind[0] == 'S')
        badges.push_back(BADGE_LOCK);
      auto badge_slot =
          div(context, mk(tile.ent(), 5),
              ComponentConfig{}
                  .with_size(ComponentSize{pixels(216), pixels(28)})
                  .with_absolute_position(12.f, 178.f)
                  .with_ignore_pointer_events(true)
                  .with_debug_name(fmt::format("ml_badgeslot_{}", slot)));
      icon_row(context, mk(badge_slot.ent(), 0), sheet, badges, 0.22f,
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.f), pixels(28)})
                   .with_ignore_pointer_events(true)
                   .with_debug_name(fmt::format("ml_badges_{}", slot)));
    }

    pagination(context, mk(entity, 2), page_labels, page,
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(752), pixels(44)})
                   .with_absolute_position(GRID_X, 548.f)
                   .with_custom_background(tile_bg)
                   .with_auto_text_color(false)
                   .with_custom_text_color(ink)
                   .with_font_size(17.f)
                   .with_debug_name("ml_pages"));

    // ---- detail panel -----------------------------------------------------
    const int hovered = [&] {
      for (int s = 0; s < PER_PAGE; s++)
        if (tile_id[s] >= 0 && context.was_hot(tile_id[s]))
          return first + s;
      return -1;
    }();
    const int shown = hovered >= 0 && hovered < 24 ? hovered : selected;
    const Item &d = ITEMS[shown];

    div(context, mk(entity, 3),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(408), pixels(508)})
            .with_absolute_position(824.f, GRID_Y)
            .with_custom_background(panel)
            .with_roundness(0.04f)
            .with_debug_name("ml_detail"));

    div(context, mk(entity, 4),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(408), pixels(180)})
            .with_absolute_position(824.f, GRID_Y)
            .with_custom_background(kind_plate(d.kind))
            .with_roundness(0.04f)
            .with_debug_name("ml_detail_plate"));

    sprite(context, mk(entity, 5), sheet, ART[d.art],
           ComponentConfig{}
               .with_size(ComponentSize{pixels(88), pixels(88)})
               .with_absolute_position(824.f + 160.f, GRID_Y + 46.f)
               .with_debug_name("ml_detail_art"));

    div(context, mk(entity, 6),
        ComponentConfig{}
            .with_label(d.title)
            .with_size(ComponentSize{pixels(360), pixels(34)})
            .with_absolute_position(848.f, GRID_Y + 200.f)
            .with_alignment(TextAlignment::Left)
            .with_font_size(24.f)
            .with_custom_text_color(ink)
            .with_debug_name("ml_detail_title"));

    div(context, mk(entity, 7),
        ComponentConfig{}
            .with_label(fmt::format("{}   {}   {} min", d.kind, d.year, d.mins))
            .with_size(ComponentSize{pixels(360), pixels(26)})
            .with_absolute_position(848.f, GRID_Y + 238.f)
            .with_alignment(TextAlignment::Left)
            .with_font_size(17.f)
            .with_custom_text_color(muted)
            .with_debug_name("ml_detail_meta"));

    std::vector<Rect> rating(static_cast<size_t>(d.stars), BADGE_STAR);
    auto star_slot = div(context, mk(entity, 8),
                         ComponentConfig{}
                             .with_size(ComponentSize{pixels(360), pixels(32)})
                             .with_absolute_position(848.f, GRID_Y + 272.f)
                             .with_debug_name("ml_starslot"));
    icon_row(context, mk(star_slot.ent(), 0), sheet, rating, 0.26f,
             ComponentConfig{}
                 .with_size(ComponentSize{percent(1.f), pixels(32)})
                 .with_debug_name("ml_detail_stars"));

    div(context, mk(entity, 9),
        ComponentConfig{}
            .with_label(hovered >= 0 ? "showing the tile under the cursor"
                                     : "showing the selected tile")
            .with_size(ComponentSize{pixels(360), pixels(26)})
            .with_absolute_position(848.f, GRID_Y + 320.f)
            .with_alignment(TextAlignment::Left)
            .with_font_size(16.f)
            .with_custom_text_color(muted)
            .with_debug_name("ml_detail_src"));

    // ---- readout ----------------------------------------------------------
    int built = 0;
    for (int s = 0; s < PER_PAGE; s++)
      if (first + s < 24)
        built++;

    div(context, mk(entity, 10),
        ComponentConfig{}
            .with_label(fmt::format(
                "page {} of {}   items {}-{} of 24   tiles built {}   "
                "hovered {}   selected {}",
                page + 1, page_labels.size(), first + 1, first + built, built,
                hovered < 0 ? std::string("none")
                            : std::string(ITEMS[hovered].title),
                ITEMS[selected].title))
            .with_size(ComponentSize{pixels(1180), pixels(26)})
            .with_absolute_position(GRID_X, 612.f)
            .with_alignment(TextAlignment::Left)
            .with_font_size(16.f)
            .with_custom_text_color(muted)
            .with_debug_name("ml_readout"));

    div(context, mk(entity, 11),
        ComponentConfig{}
            .with_label("18 of the 24 entries have no widgets at all right now; "
                        "page through to build them")
            .with_size(ComponentSize{pixels(1180), pixels(26)})
            .with_absolute_position(GRID_X, 642.f)
            .with_alignment(TextAlignment::Left)
            .with_font_size(16.f)
            .with_custom_text_color(muted)
            .with_debug_name("ml_hint"));
  }
};

REGISTER_EXAMPLE_SCREEN(media_library, "App Mockups",
                        "Paged poster wall, only the current page is built",
                        MediaLibraryGrid)
