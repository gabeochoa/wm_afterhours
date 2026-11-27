#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include "KirbyArtwork.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>
#include <afterhours/src/plugins/ui/text_input/text_input.h>
#include <array>
#include <string>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct KirbyOptionsScreen : ScreenSystem<UIContext<InputAction>> {
  enum class Detail { None, Name, Controller, Display, Accessibility, Sound,
                      Online, Data, Messages, Favorites };
  struct Profile {
    std::string name = "Poppy";
    std::array<size_t, 10> preferences{};
    int races = 12;
    int records = 8;
    size_t favorite = 0;
  };
  struct Art {
    const char *name;
    float x, y, w, h;
  };
  struct ArtworkCache : afterhours::BaseComponent {
    raylib::Texture2D texture{};
    ~ArtworkCache() override {
      if (texture.id != 0 && raylib::IsWindowReady()) raylib::UnloadTexture(texture);
    }
  };
  raylib::Texture2D atlas{};
  Profile profile;
  Detail detail = Detail::None;
  size_t selected_tab = 5;
  bool confirm_delete = false, focus_name = false;
  std::string editing_name = "Poppy", status;
  std::array<Art, 25> art{{
      {"desk", 0.f, 0.f, 1280.f, 720.f},
      {"board", 378.f, 140.f, 836.f, 564.f},
      {"notebook", 27.f, 154.f, 350.f, 486.f},
      {"name", 556.f, 198.f, 426.f, 96.f},
      {"delete", 1052.f, 198.f, 132.f, 94.f},
      {"options_label", 916.f, 114.f, 203.f, 46.f},
      {"common_label", 412.f, 315.f, 109.f, 28.f},
      {"message_line", 410.f, 648.f, 811.f, 72.f},
      {"option_0", 515.f, 346.f, 56.f, 56.f},
      {"option_1", 773.f, 346.f, 56.f, 56.f},
      {"option_2", 1031.f, 346.f, 56.f, 56.f},
      {"option_3", 580.f, 506.f, 56.f, 56.f},
      {"option_4", 967.f, 506.f, 56.f, 56.f},
      {"tab_0_off", 396.f, 56.f, 112.f, 88.f},
      {"tab_0_on", 396.f, 40.f, 112.f, 104.f},
      {"tab_1_off", 522.f, 56.f, 112.f, 88.f},
      {"tab_1_on", 522.f, 40.f, 112.f, 104.f},
      {"tab_2_off", 648.f, 56.f, 112.f, 88.f},
      {"tab_2_on", 648.f, 40.f, 112.f, 104.f},
      {"tab_3_off", 774.f, 56.f, 112.f, 88.f},
      {"tab_3_on", 774.f, 40.f, 112.f, 104.f},
      {"tab_4_off", 900.f, 56.f, 112.f, 88.f},
      {"tab_4_on", 900.f, 40.f, 112.f, 104.f},
      {"tab_5_off", 1026.f, 56.f, 112.f, 88.f},
      {"tab_5_on", 1026.f, 40.f, 112.f, 104.f},
  }};
  const afterhours::Color ink{69, 64, 67, 255};
  const afterhours::Color purple{149, 96, 211, 255};
  const afterhours::Color paper{255, 253, 241, 255};

  void load(afterhours::Entity &entity) {
    auto &cache = entity.addComponentIfMissing<ArtworkCache>();
    if (cache.texture.id == 0) {
      cache.texture = raylib::LoadTexture(afterhours::files::get_resource_path(
          "images", "kirby_options/atlas.png").string().c_str());
      if (cache.texture.id != 0)
        raylib::SetTextureFilter(cache.texture, raylib::TEXTURE_FILTER_BILINEAR);
    }
    atlas = cache.texture;
  }
  void open(Detail next) {
    detail = next;
    status.clear();
    confirm_delete = false;
    if (next == Detail::Name) { editing_name = profile.name; focus_name = true; }
  }
  void choose_tab(size_t i) {
    selected_tab = i;
    constexpr std::array<Detail, 6> panels{
        Detail::Online, Detail::Controller, Detail::None, Detail::Messages,
        Detail::Favorites, Detail::None};
    open(panels[i]);
  }
  static void paint(raylib::Texture2D texture, raylib::Rectangle frame, RectangleType r) {
    if (texture.id == 0) return;
    raylib::DrawTexturePro(texture, frame, r, {0, 0}, 0, raylib::WHITE);
  }
  ComponentConfig box(float scale, float x, float y, float w, float h) const {
    return ComponentConfig{}.with_size({pixels(w * scale), pixels(h * scale)})
        .with_absolute_position(x * scale, y * scale)
        .with_background(Theme::Usage::None).with_corner_radius(0);
  }

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context, float) override {
    load(entity);
    const float scale = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    Theme theme;
    theme.font = ink;
    theme.darkfont = {255, 255, 255, 255};
    theme.background = {231, 213, 183, 255};
    theme.surface = paper;
    theme.primary = purple;
    theme.accent = {255, 237, 79, 255};
    theme.corner_radius = 0;
    theme.roundness = 0;
    context.set_theme(theme);
    context.scaling_mode = ScalingMode::Proportional;
    UIStylingDefaults::get().set_grid_snapping(false);
    UIStylingDefaults::get().set_default_font("FredokaMockBold", pixels((28) * scale));
    if (context.pressed(InputAction::MenuBack)) {
      if (confirm_delete) confirm_delete = false;
      else detail = Detail::None;
    }
    if (detail == Detail::None) {
      if (context.pressed(InputAction::WidgetLeft)) choose_tab((selected_tab + 5) % 6);
      if (context.pressed(InputAction::WidgetRight)) choose_tab((selected_tab + 1) % 6);
    }
    div(context, mk(entity), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background({231, 213, 183, 255}).with_corner_radius(0)
        .with_debug_name("kirby_canvas"));
    auto root = div(context, mk(entity), box(scale, 0, 0, 1280, 720)
        .with_absolute_position((context.screen_width - 1280 * scale) / 2,
                                (context.screen_height - 720 * scale) / 2)
        .with_debug_name("kirby_root"));
    const std::array<const char *, 10> help{
        "Enter a new name and customize your controls.", "Edit the name shown on this profile.",
        "Choose your control style and rumble preference.", "Adjust screen brightness and camera distance.",
        "Choose text size and color assistance.", "Set music and sound effects levels.",
        "Choose who can see your player in this local demo.", "Review saved data before choosing whether to delete it.",
        "Messages are unavailable during local play.", "Choose the game mode you want to keep handy."};
    std::string hover_help;

    auto decoration = [&](size_t i) {
      const auto &asset = art[i];
      // The reference shows the notebook as a tilted checklist on the desk.
      // Custom draw callbacks ignore div rotation, so the notebook's texture
      // is drawn with a real -6 degree rotation about its centre instead.
      const bool tilt = (i == 2);
      div(context, mk(root.ent(), 100 + static_cast<int>(i)),
          box(scale, asset.x, asset.y, asset.w, asset.h).with_ignore_pointer_events()
          .with_on_draw_bg([texture = atlas, frame = kirby_artwork_frames[i], tilt](RectangleType r) {
            if (!tilt) { paint(texture, frame, r); return; }
            if (texture.id == 0) return;
            // MarloKart's paint() pattern: pivot on the centre by shifting
            // the dest rect by the origin before DrawTexturePro.
            const raylib::Vector2 center{r.width / 2.f, r.height / 2.f};
            raylib::DrawTexturePro(texture, frame,
                                   {r.x + center.x, r.y + center.y, r.width, r.height},
                                   center, -6.0f, raylib::WHITE);
          }));
    };
    auto label = [&](int id, const std::string &text, float x, float y, float w, float h,
                     float size, afterhours::Color color,
                     const std::string &name = "", const std::string &font = "FredokaMockBold") {
      return div(context, mk(root.ent(), id), box(scale, x, y, w, h).with_label(text)
          .with_font(font, pixels((size * 1.25f) * scale)).with_custom_text_color(color)
          .with_text_overflow(TextOverflow::Ellipsis)
          .with_alignment(TextAlignment::Center).with_ignore_pointer_events()
          .with_debug_name(name));
    };
    auto action = [&](int id, const std::string &text, float x, float y, float w, float h,
                      const std::string &name, bool filled = false, float size = 22,
                      const std::string &hint = "", float radius = 0) {
      auto result = button(context, mk(root.ent(), id), box(scale, x, y, w, h).with_label(text)
          .with_font("FredokaMockBold", pixels((size * 1.25f) * scale))
          .with_custom_text_color(filled ? afterhours::Color{255, 255, 255, 255} : ink)
          .with_custom_background(filled ? purple : afterhours::Color{0, 0, 0, 0})
          .with_corner_radius(filled ? 5 * scale : radius * scale)
          .with_alignment(TextAlignment::Center).with_click_activation(ClickActivationMode::Release)
          .with_debug_name(name));
      if (!hint.empty() && (context.was_hot(result.ent().id) || context.has_focus(result.ent().id)))
        hover_help = hint;
      return result;
    };
    decoration(0);
    // Desk-scene depth: paper-stack edges peeking from under the notebook,
    // a spare pencil on the desk, and soft contact shadows under the tab row.
    // Drawn shapes only (no extracted art), drawn after the desk texture.
    div(context, mk(root.ent()), box(scale, 0, 0, 1280, 720)
        .with_ignore_pointer_events().with_debug_name("kirby_desk_depth")
        .with_on_draw_fg([scale](RectangleType r) {
          const float s = scale;
          const afterhours::Color paper_edge{238, 230, 205, 255};
          const afterhours::Color shadow{104, 78, 48, 70};
          // Paper stack edges under the notebook (left + bottom).
          afterhours::draw_rectangle({r.x + 14 * s, r.y + 176 * s, 44 * s, 440 * s}, paper_edge);
          afterhours::draw_rectangle({r.x + 20 * s, r.y + 168 * s, 44 * s, 440 * s}, paper_edge);
          afterhours::draw_rectangle({r.x + 44 * s, r.y + 632 * s, 300 * s, 9 * s}, paper_edge);
          afterhours::draw_rectangle({r.x + 52 * s, r.y + 638 * s, 292 * s, 8 * s}, paper_edge);
          // Spare pencil lying on the desk, top-left of the notebook.
          afterhours::draw_rectangle({r.x + 56 * s, r.y + 96 * s, 110 * s, 7 * s},
                                     afterhours::Color{211, 104, 51, 255});
          afterhours::draw_triangle({r.x + 166 * s, r.y + 92 * s}, {r.x + 166 * s, r.y + 107 * s},
                                    {r.x + 184 * s, r.y + 99.5f * s},
                                    afterhours::Color{240, 217, 170, 255});
          afterhours::draw_rectangle({r.x + 56 * s, r.y + 96 * s, 12 * s, 7 * s},
                                     afterhours::Color{250, 220, 196, 255});
          // Drop shadow under the tab pills.
          afterhours::draw_rectangle_rounded({r.x + 392 * s, r.y + 64 * s, 744 * s, 88 * s},
                                             .35f, 28, shadow, RoundedCorners().all_round());
        }));
    for (size_t i = 0; i < 6; ++i) decoration(13 + i * 2 + (selected_tab == i ? 1 : 0));
    div(context, mk(root.ent()), box(scale, 386, 148, 836, 564)
        .with_ignore_pointer_events().with_debug_name("kirby_board_shadow")
        .with_on_draw_bg([scale](RectangleType r) {
          afterhours::draw_rectangle_rounded(r, .02f, 12,
                                             afterhours::Color{104, 78, 48, 85},
                                             RoundedCorners().all_round());
        }));
    decoration(1);
    div(context, mk(root.ent()), box(scale, 564, 206, 426, 96)
        .with_ignore_pointer_events().with_debug_name("kirby_name_shadow")
        .with_on_draw_bg([scale](RectangleType r) {
          afterhours::draw_rectangle_rounded(r, .5f, 40,
                                             afterhours::Color{104, 78, 48, 60},
                                             RoundedCorners().all_round());
        }));
    decoration(3);
    decoration(4);
    div(context, mk(root.ent()), box(scale, 35, 162, 350, 486)
        .with_ignore_pointer_events().with_debug_name("kirby_notebook_shadow")
        .with_on_draw_bg([scale](RectangleType r) {
          afterhours::draw_rectangle_rounded(r, .02f, 8,
                                             afterhours::Color{104, 78, 48, 70},
                                             RoundedCorners().all_round());
        }));
    decoration(2);
    // Paper-checklist detail over the tilted notebook: empty checkbox squares
    // in a left column, first one ticked. Positions are notebook-local and
    // rotated about the notebook centre by the same -6 degrees as the
    // notebook texture, so they sit on the printed rows.
    div(context, mk(root.ent()), box(scale, 27, 154, 350, 486)
        .with_ignore_pointer_events().with_debug_name("kirby_checklist")
        .with_on_draw_fg([scale](RectangleType r) {
          const float cos6 = 0.99452f, sin6 = -0.10453f;
          const float cx = r.x + r.width / 2.f, cy = r.y + r.height / 2.f;
          const auto rot = [=](float lx, float ly) {
            const float dx = (lx - r.width / 2.f) * scale;
            const float dy = (ly - r.height / 2.f) * scale;
            return std::pair<float, float>{cx + dx * cos6 - dy * sin6,
                                           cy + dx * sin6 + dy * cos6};
          };
          const afterhours::Color ink{69, 64, 67, 255};
          const afterhours::Color check{60, 140, 90, 255};
          constexpr std::array<float, 4> rows{252.f, 300.f, 348.f, 396.f};
          for (size_t i = 0; i < rows.size(); ++i) {
            auto [px, py] = rot(30.f, rows[i]);
            const float hs = 13.f * scale;
            raylib::DrawRectangleLinesEx({px - hs, py - hs, hs * 2, hs * 2}, 2.f * scale, ink);
            if (i == 0) {
              auto [a, b] = rot(25.f, rows[i] + 2.f);
              auto [c, d] = rot(30.f, rows[i] + 8.f);
              auto [e, f] = rot(38.f, rows[i] - 8.f);
              afterhours::draw_line_ex({a, b}, {c, d}, 3.f * scale, check);
              afterhours::draw_line_ex({c, d}, {e, f}, 3.f * scale, check);
            }
          }
        }));
    constexpr std::array<const char *, 6> tab_names{"Network", "Controller", "Home", "Messages", "Favorites", "Options"};
    // Reference tabs are large unlabeled icon pills; the selected tab is named
    // by the purple banner beneath it instead of a label on the pill.
    for (size_t i = 0; i < 6; ++i) {
      const auto &a = art[13 + i * 2 + (selected_tab == i ? 1 : 0)];
      if (action(10 + static_cast<int>(i), "", a.x, a.y, a.w, a.h,
                 "kirby_tab_" + std::to_string(i))) choose_tab(i);
    }
    {
      const auto &a = art[13 + selected_tab * 2 + 1];
      div(context, mk(root.ent()), box(scale, a.x, 144, 112, 34)
          .with_custom_background(purple).with_corner_radius(5 * scale)
          .with_ignore_pointer_events().with_debug_name("kirby_banner"));
      label(150, tab_names[selected_tab], a.x, 144, 112, 34, 16,
            raylib::WHITE, "kirby_title");
      // White keyline on the selected pill, as the reference lifts it out of
      // the row.
      div(context, mk(root.ent()), box(scale, a.x + 3, a.y + 3, a.w - 6, a.h - 6)
          .with_ignore_pointer_events().with_debug_name("kirby_tab_ring")
          .with_on_draw_fg([scale](RectangleType r) {
            afterhours::draw_rectangle_rounded_lines_ex(r, .35f, 36, 3 * scale, raylib::WHITE);
          }));
    }
    if (action(20, "L", 350, 83, 32, 32, "kirby_previous_tab", true, 17))
      choose_tab((selected_tab + 5) % 6);
    if (action(21, "R", 1152, 83, 32, 32, "kirby_next_tab", true, 17))
      choose_tab((selected_tab + 1) % 6);
    label(23, profile.name, 635, 222, 326, 46, 34, ink, "kirby_profile_name");
    label(24, "Delete Data", 1056, 252, 124, 32, 16, {120, 67, 83, 255}, "kirby_delete_label");
    div(context, mk(root.ent()), box(scale, 414, 304, 130, 28)
        .with_custom_background({112, 102, 84, 255}));
    label(25, "Common", 414, 304, 130, 28, 17, raylib::WHITE, "kirby_common", "FredokaMockBold");
    if (action(26, "", 556, 198, 426, 96, "kirby_name", false, 22, help[1], 48)) open(Detail::Name);
    if (action(27, "", 1052, 198, 132, 94, "kirby_data", false, 22, help[7])) open(Detail::Data);
    if (action(28, "", 212, 269, 106, 56, "kirby_customize", false, 22, help[1])) open(Detail::Name);
    if (detail == Detail::None) {
      constexpr std::array<const char *, 5> names{"Controller", "Display", "Accessibility", "Sound", "Online"};
      constexpr std::array<std::array<float, 4>, 5> cells{{
          {414, 340, 258, 130}, {672, 340, 258, 130}, {930, 340, 258, 130},
          {414, 500, 387, 130}, {801, 500, 387, 130}}};

      for (size_t i = 0; i < 5; ++i) {
        const auto &c = cells[i];
        div(context, mk(root.ent(), 170 + static_cast<int>(i)), box(scale, c[0], c[1], c[2], c[3])
            .with_border({162, 151, 125, 255}, scale).with_corner_radius(6 * scale).with_ignore_pointer_events());
        // Category icon fidelity: the reference draws the glyph itself in
        // purple on the card, not a solid badge, so the atlas badge is
        // replaced with an outline glyph drawn as shapes.
        div(context, mk(root.ent(), 170 + static_cast<int>(i)),
            box(scale, c[0] + (c[2] - 56) / 2.f, c[1] + 16, 56, 56)
                .with_ignore_pointer_events()
                .with_debug_name("kirby_option_icon_" + std::to_string(i))
                .with_on_draw_fg([scale, icon = i, paper_c = paper](RectangleType r) {
                  const float s = scale;
                  const afterhours::Color g{149, 96, 211, 255};
                  const float w = 5.f * s;
                  const auto line = [=](float x, float y, float xx, float yy, float wd = 0.f) {
                    afterhours::draw_line_ex({r.x + x * s, r.y + y * s},
                                             {r.x + xx * s, r.y + yy * s},
                                             wd > 0.f ? wd : w, g);
                  };
                  if (icon == 0) {  // Controller: gamepad outline + d-pad + dots.
                    afterhours::draw_rectangle_rounded(
                        {r.x + 8 * s, r.y + 16 * s, 40 * s, 26 * s}, .45f, 16, g,
                        RoundedCorners().all_round());
                    afterhours::draw_rectangle_rounded(
                        {r.x + 11 * s, r.y + 19 * s, 34 * s, 20 * s}, .45f, 12, paper_c,
                        RoundedCorners().all_round());
                    line(19, 26, 19, 36); line(14, 31, 24, 31);
                    afterhours::draw_circle(static_cast<int>(r.x + 35 * s),
                                            static_cast<int>(r.y + 27 * s), 2.6f * s, g);
                    afterhours::draw_circle(static_cast<int>(r.x + 40 * s),
                                            static_cast<int>(r.y + 32 * s), 2.6f * s, g);
                    return;
                  }
                  if (icon == 1) {  // Display: monitor + stand.
                    raylib::DrawRectangleLinesEx(
                        {r.x + 10 * s, r.y + 13 * s, 36 * s, 26 * s}, 4.f * s, g);
                    line(28, 39, 28, 47); line(20, 47, 36, 47);
                    return;
                  }
                  if (icon == 2) {  // Accessibility: person.
                    afterhours::draw_circle(static_cast<int>(r.x + 28 * s),
                                            static_cast<int>(r.y + 11 * s), 5.5f * s, g);
                    line(28, 17, 28, 33);
                    line(28, 21, 15, 30); line(28, 21, 41, 30);
                    line(28, 33, 18, 49); line(28, 33, 38, 49);
                    return;
                  }
                  if (icon == 3) {  // Sound: speaker + two wave arcs.
                    afterhours::draw_rectangle({r.x + 9 * s, r.y + 19 * s, 9 * s, 18 * s}, g);
                    afterhours::draw_triangle({r.x + 18 * s, r.y + 14 * s},
                                              {r.x + 18 * s, r.y + 42 * s},
                                              {r.x + 29 * s, r.y + 28 * s}, g);
                    line(34, 22, 39, 28); line(39, 28, 34, 34);
                    line(42, 18, 49, 28); line(49, 28, 42, 38);
                    return;
                  }
                  // Online: globe with meridian ellipse.
                  afterhours::draw_circle_lines(static_cast<int>(r.x + 28 * s),
                                                static_cast<int>(r.y + 28 * s), 17 * s, g);
                  line(11, 28, 45, 28);
                  for (int i = 0; i < 12; ++i) {
                    const float a0 = static_cast<float>(i) * 3.14159f / 6.f;
                    const float a1 = static_cast<float>(i + 1) * 3.14159f / 6.f;
                    afterhours::draw_line_ex(
                        {r.x + (28 + std::cos(a0) * 8) * s, r.y + (28 + std::sin(a0) * 17) * s},
                        {r.x + (28 + std::cos(a1) * 8) * s, r.y + (28 + std::sin(a1) * 17) * s},
                        w, g);
                  }
                }));
        label(30 + static_cast<int>(i), names[i], c[0] + 10, c[1] + 75,
              c[2] - 20, 44, 22, ink, "kirby_option_label_" + std::to_string(i));
        if (action(40 + static_cast<int>(i), "", c[0], c[1], c[2], c[3],
                   "kirby_option_" + std::to_string(i), false, 22, help[i + 2], 6))
          open(static_cast<Detail>(static_cast<int>(Detail::Controller) + i));
      }
    } else {
      div(context, mk(root.ent(), 50), box(scale, 414, 307, 774, 324)
          .with_custom_background(paper).with_border({197, 185, 152, 255}, 2 * scale)
          .with_debug_name("kirby_detail"));
      if (action(51, "< Common options", 434, 324, 244, 36, "kirby_detail_back", true, 18))
        detail = Detail::None;
      constexpr std::array<const char *, 10> titles{
          "Common", "Your name", "Controller", "Display", "Accessibility", "Sound",
          "Online", "Save data", "Messages", "Favorites"};
      label(52, titles[static_cast<size_t>(detail)], 440, 371, 705, 45, 29, ink, "kirby_detail_title");
      if (detail == Detail::Name) {
        label(53, "Name", 443, 436, 150, 47, 23, ink);
        auto input = text_input(context, mk(root.ent()), editing_name,
            box(scale, 626, 430, 502, 52).with_font("FredokaMockBold", pixels((28) * scale))
                .with_custom_background(raylib::WHITE).with_custom_text_color(ink)
                .with_border({200, 184, 147, 255}, 2 * scale).with_corner_radius(5 * scale)
                .with_debug_name("kirby_name_input"));
        input.ent().get<afterhours::text_input::HasTextInputState>().max_length = 24;
        if (focus_name) { context.set_focus(input.ent().id); focus_name = false; }
        label(55, "Choose the name your friends will see.", 447, 503, 695, 38, 20, ink, "", "Atkinson");
        if (action(56, "SAVE NAME", 812, 581, 306, 46, "kirby_name_save", true)) {
          profile.name = editing_name.empty() ? "Poppy" : editing_name;
          detail = Detail::None;
          status = "Name saved: " + profile.name;
        }
        if (action(57, "CANCEL", 466, 581, 270, 46, "kirby_name_cancel", true)) detail = Detail::None;
      } else if (detail >= Detail::Controller && detail <= Detail::Online) {
        constexpr std::array<const char *, 10> row_names{
            "Control style", "Rumble", "Screen brightness", "Camera distance", "Text size",
            "Color assistance", "Music volume", "Sound effects", "Connection", "Player visibility"};
        const std::array<std::array<const char *, 3>, 10> values{{
            {"Standard", "Alternate", "Southpaw"}, {"On", "Off", "On"},
            {"75%", "100%", "50%"}, {"Standard", "Near", "Far"},
            {"Normal", "Large", "Normal"}, {"None", "Deuteranopia", "Protanopia"},
            {"100%", "50%", "Off"}, {"100%", "50%", "Off"},
            {"Local play", "Local play", "Local play"}, {"Friends", "Private", "Everyone"}}};
        const size_t offset = (static_cast<size_t>(detail) - static_cast<size_t>(Detail::Controller)) * 2;
        for (size_t r = 0; r < 2; ++r) {
          const size_t index = offset + r;
          label(53 + static_cast<int>(r), row_names[index], 444, 433 + 75 * r, 325, 49, 22, ink);
          const std::string value = values[index][profile.preferences[index]];
          if (index == 8) label(60, value, 820, 433, 325, 49, 22, ink, "kirby_local_connection");
          else if (action(60 + static_cast<int>(r), value, 820, 433 + 75 * r, 325, 49,
                          "kirby_value_" + std::to_string(r), true, 22)) {
            profile.preferences[index] = (profile.preferences[index] + 1) % (index == 1 || index == 4 ? 2 : 3);
            status = std::string(row_names[index]) + ": " + values[index][profile.preferences[index]];
          }
        }
        if (detail == Detail::Online)
          label(62, "Local play only. No network connection is made.", 446, 582, 708, 39, 18, ink, "", "Atkinson");
      } else if (detail == Detail::Data) {
        if (confirm_delete) {
          label(53, "Delete this profile's saved data?", 443, 432, 701, 46, 24, ink);
          label(54, "Your name, records and preferences will be reset.", 443, 483, 701, 45, 20, ink, "", "Atkinson");
          if (action(55, "DELETE DATA", 809, 575, 321, 47, "kirby_delete_confirm", true)) {
            profile = Profile{};
            profile.races = 0;
            profile.records = 0;
            confirm_delete = false;
            status = "Data deleted. Your profile is ready for a new adventure.";
          }
          if (action(56, "CANCEL", 462, 575, 295, 47, "kirby_delete_cancel", true)) confirm_delete = false;
        } else {
          label(53, "Rider: " + profile.name, 447, 427, 694, 42, 22, ink);
          label(54, "Races completed: " + std::to_string(profile.races) +
                    "    Records: " + std::to_string(profile.records), 447, 480, 694, 43, 22, ink, "kirby_saved_data");
          if (action(55, "RESET SAVED DATA", 663, 572, 353, 47, "kirby_delete_request", true)) confirm_delete = true;
        }
      } else if (detail == Detail::Messages) {
        label(53, "No messages", 449, 436, 694, 44, 25, ink);
        label(54, "Messages are unavailable while playing locally.", 444, 500, 705, 44, 20, ink, "", "Atkinson");
      } else if (detail == Detail::Favorites) {
        constexpr std::array<const char *, 3> modes{"Air Ride", "Top Ride", "City Trial"};
        label(53, "Favorite mode", 440, 433, 706, 43, 22, ink);
        if (action(54, modes[profile.favorite], 663, 503, 354, 49, "kirby_favorite", true))
          profile.favorite = (profile.favorite + 1) % modes.size();
      }
    }
    div(context, mk(root.ent()), box(scale, 414, 646, 774, 36)
        .with_label(!status.empty() ? status : !hover_help.empty() ? hover_help : help[static_cast<size_t>(detail)])
        .with_font("FredokaMockBold", pixels(21 * scale)).with_text_overflow(TextOverflow::Wrap).with_custom_text_color(ink)
        .with_alignment(TextAlignment::Left).with_ignore_pointer_events()
        .with_debug_name("kirby_message"));
    div(context, mk(root.ent()), box(scale, 414, 687, 774, 2)
        .with_custom_background(purple).with_ignore_pointer_events()
        .with_debug_name("kirby_footer_rule"));
  }
};

REGISTER_EXAMPLE_SCREEN(kirby_options, "Game Mockups",
                        "Kirby style options board with profile and preferences",
                        KirbyOptionsScreen)
