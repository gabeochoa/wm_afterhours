#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
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
    raylib::Texture2D texture{};
  };
  Profile profile;
  Detail detail = Detail::None;
  size_t selected_tab = 5;
  bool confirm_delete = false, loaded = false, focus_name = false;
  std::string editing_name = "Poppy", status;
  std::array<Art, 25> art{{
      {"desk", 0.f, 0.f, 1280.f, 720.f},
      {"board", 378.f, 140.f, 836.f, 564.f},
      {"notebook", 12.f, 154.f, 350.f, 486.f},
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

  void load() {
    if (loaded) return;
    for (auto &asset : art) {
      asset.texture = raylib::LoadTexture(afterhours::files::get_resource_path(
          "images", std::string("kirby_options/") + asset.name + ".png").string().c_str());
      raylib::SetTextureFilter(asset.texture, raylib::TEXTURE_FILTER_BILINEAR);
    }
    loaded = true;
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
  static void paint(raylib::Texture2D texture, RectangleType r) {
    raylib::DrawTexturePro(texture, {0, 0, static_cast<float>(texture.width),
        static_cast<float>(texture.height)}, r, {0, 0}, 0, raylib::WHITE);
  }
  ComponentConfig box(float scale, float x, float y, float w, float h) const {
    return ComponentConfig{}.with_size({pixels(w * scale), pixels(h * scale)})
        .with_absolute_position(x * scale, y * scale)
        .with_background(Theme::Usage::None).with_corner_radius(0);
  }

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context, float) override {
    load();
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
    div(context, mk(entity, 1), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background({231, 213, 183, 255}).with_corner_radius(0)
        .with_debug_name("kirby_canvas"));
    auto root = div(context, mk(entity, 0), box(scale, 0, 0, 1280, 720)
        .with_absolute_position((context.screen_width - 1280 * scale) / 2,
                                (context.screen_height - 720 * scale) / 2)
        .with_debug_name("kirby_root"));
    const std::array<const char *, 10> help{
        "Choose a category to customize your rider.", "Edit the name shown on this profile.",
        "Choose your control style and rumble preference.", "Adjust screen brightness and camera distance.",
        "Choose text size and color assistance.", "Set music and sound effects levels.",
        "Choose who can see your player in this local demo.", "Review saved data before choosing whether to delete it.",
        "Messages are unavailable during local play.", "Choose the game mode you want to keep handy."};
    std::string hover_help;

    auto decoration = [&](size_t i) {
      const auto &asset = art[i];
      div(context, mk(root.ent(), 100 + static_cast<int>(i)),
          box(scale, asset.x, asset.y, asset.w, asset.h).with_ignore_pointer_events()
          .with_on_draw_bg([texture = asset.texture](RectangleType r) { paint(texture, r); }));
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
    for (size_t i = 0; i < 6; ++i) decoration(13 + i * 2 + (selected_tab == i ? 1 : 0));
    decoration(1);
    decoration(3);
    decoration(4);
    decoration(2);
    constexpr std::array<const char *, 6> tab_names{"Network", "Controller", "Home", "Messages", "Favorites", "Options"};
    for (size_t i = 0; i < 6; ++i) {
      const auto &a = art[13 + i * 2 + (selected_tab == i ? 1 : 0)];
      label(150 + static_cast<int>(i), tab_names[i], a.x + 3, 114, 106, 25,
            12.5f, raylib::WHITE, "kirby_tab_label_" + std::to_string(i));
      if (action(10 + static_cast<int>(i), "", a.x, a.y, a.w, a.h,
                 "kirby_tab_" + std::to_string(i))) choose_tab(i);
    }
    if (action(20, "L", 350, 83, 32, 32, "kirby_previous_tab", true, 17))
      choose_tab((selected_tab + 5) % 6);
    if (action(21, "R", 1152, 83, 32, 32, "kirby_next_tab", true, 17))
      choose_tab((selected_tab + 1) % 6);
    div(context, mk(root.ent(), 160), box(scale, 940, 144, 198, 34)
        .with_custom_background(purple).with_debug_name("kirby_banner"));
    label(22, tab_names[selected_tab], 940, 145, 198, 32, 19,
          raylib::WHITE, "kirby_title");
    label(23, profile.name, 635, 207, 326, 46, 34, ink, "kirby_profile_name");
    label(161, "Edit name", 635, 253, 326, 28, 16, ink, "kirby_edit_name", "FredokaMockBold");
    label(24, "Delete Data", 1056, 252, 124, 32, 16, {120, 67, 83, 255}, "kirby_delete_label");
    div(context, mk(root.ent(), 162), box(scale, 414, 308, 130, 28)
        .with_custom_background({112, 102, 84, 255}));
    label(25, "Common", 414, 308, 130, 28, 17, raylib::WHITE, "kirby_common", "FredokaMockBold");
    if (action(26, "", 556, 198, 426, 96, "kirby_name", false, 22, help[1], 48)) open(Detail::Name);
    if (action(27, "", 1052, 198, 132, 94, "kirby_data", false, 22, help[7])) open(Detail::Data);
    if (action(28, "", 212, 269, 106, 56, "kirby_customize", false, 22, help[1])) open(Detail::Name);
    label(163, "Profile customization", 39, 332, 292, 31, 15, ink, "kirby_profile_caption", "FredokaMockBold");
    if (detail == Detail::None) {
      constexpr std::array<const char *, 5> names{"Controller", "Display", "Accessibility", "Sound", "Online"};
      constexpr std::array<std::array<float, 4>, 5> cells{{
          {414, 340, 258, 130}, {672, 340, 258, 130}, {930, 340, 258, 130},
          {414, 500, 387, 130}, {801, 500, 387, 130}}};
      label(164, "Controls & visibility", 572, 310, 578, 25, 15, ink, "", "FredokaMockBold");
      label(165, "Sound & connection", 414, 473, 774, 25, 15, ink, "", "FredokaMockBold");
      for (size_t i = 0; i < 5; ++i) {
        const auto &c = cells[i];
        div(context, mk(root.ent(), 170 + static_cast<int>(i)), box(scale, c[0], c[1], c[2], c[3])
            .with_border({162, 151, 125, 255}, scale).with_ignore_pointer_events());
        decoration(8 + i);
        label(30 + static_cast<int>(i), names[i], c[0] + 10, c[1] + 75,
              c[2] - 20, 44, 22, ink, "kirby_option_label_" + std::to_string(i));
        if (action(40 + static_cast<int>(i), "", c[0], c[1], c[2], c[3],
                   "kirby_option_" + std::to_string(i), false, 22, help[i + 2]))
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
        auto input = text_input(context, mk(root.ent(), 54), editing_name,
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
    div(context, mk(root.ent(), 80), box(scale, 414, 646, 774, 36)
        .with_label(!status.empty() ? status : !hover_help.empty() ? hover_help : help[static_cast<size_t>(detail)])
        .with_font("FredokaMockBold", pixels(21 * scale)).with_text_overflow(TextOverflow::Wrap).with_custom_text_color(ink)
        .with_alignment(TextAlignment::Left).with_ignore_pointer_events()
        .with_debug_name("kirby_message"));
    div(context, mk(root.ent(), 81), box(scale, 414, 687, 774, 2)
        .with_custom_background(purple).with_ignore_pointer_events()
        .with_debug_name("kirby_footer_rule"));
  }
};

REGISTER_EXAMPLE_SCREEN(kirby_options, "Game Mockups",
                        "Kirby style options board with profile and preferences",
                        KirbyOptionsScreen)
