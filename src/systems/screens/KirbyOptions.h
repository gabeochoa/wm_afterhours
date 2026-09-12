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
    std::string name = "Name";
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
  std::string editing_name = "Name", status;
  std::array<Art, 25> art{{
      {"desk", 0.f, 0.f, 1280.f, 720.f},
      {"board", 356.f, 99.f, 879.f, 589.f},
      {"notebook", 0.f, 113.f, 444.f, 566.f},
      {"name", 569.f, 163.f, 443.f, 136.f},
      {"delete", 1066.f, 156.f, 114.f, 107.f},
      {"options_label", 916.f, 114.f, 203.f, 46.f},
      {"common_label", 412.f, 315.f, 109.f, 28.f},
      {"message_line", 410.f, 648.f, 811.f, 72.f},
      {"option_0", 503.f, 362.f, 81.f, 81.f},
      {"option_1", 755.f, 353.f, 81.f, 81.f},
      {"option_2", 1007.f, 344.f, 81.f, 81.f},
      {"option_3", 527.f, 538.f, 81.f, 81.f},
      {"option_4", 903.f, 525.f, 81.f, 81.f},
      {"tab_0_off", 384.f, 58.f, 134.f, 92.f},
      {"tab_0_on", 380.f, 38.f, 142.f, 123.f},
      {"tab_1_off", 509.f, 54.f, 134.f, 92.f},
      {"tab_1_on", 505.f, 33.f, 142.f, 123.f},
      {"tab_2_off", 634.f, 49.f, 134.f, 92.f},
      {"tab_2_on", 630.f, 29.f, 142.f, 123.f},
      {"tab_3_off", 759.f, 45.f, 134.f, 92.f},
      {"tab_3_on", 755.f, 25.f, 142.f, 123.f},
      {"tab_4_off", 884.f, 40.f, 134.f, 92.f},
      {"tab_4_on", 879.f, 20.f, 142.f, 123.f},
      {"tab_5_off", 1009.f, 36.f, 134.f, 92.f},
      {"tab_5_on", 1004.f, 16.f, 142.f, 123.f},
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
    const float scale = context.screen_height / 720.f;
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
    UIStylingDefaults::get().set_default_font("FredokaMockBold", h720(28));
    if (context.pressed(InputAction::MenuBack)) {
      if (confirm_delete) confirm_delete = false;
      else detail = Detail::None;
    }
    if (detail == Detail::None) {
      if (context.pressed(InputAction::WidgetLeft)) choose_tab((selected_tab + 5) % 6);
      if (context.pressed(InputAction::WidgetRight)) choose_tab((selected_tab + 1) % 6);
    }
    auto root = div(context, mk(entity, 0), box(scale, 0, 0, 1280, 720)
        .with_debug_name("kirby_root"));
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
          .with_font(font, h720(size * 1.25f)).with_custom_text_color(color)
          .with_text_overflow(TextOverflow::Ellipsis)
          .with_alignment(TextAlignment::Center).with_ignore_pointer_events()
          .with_debug_name(name));
    };
    auto action = [&](int id, const std::string &text, float x, float y, float w, float h,
                      const std::string &name, bool filled = false, float size = 22) {
      return button(context, mk(root.ent(), id), box(scale, x, y, w, h).with_label(text)
          .with_font("FredokaMockBold", h720(size * 1.25f))
          .with_custom_text_color(filled ? afterhours::Color{255, 255, 255, 255} : ink)
          .with_custom_background(filled ? purple : afterhours::Color{0, 0, 0, 0})
          .with_corner_radius(filled ? 5 * scale : 0)
          .with_alignment(TextAlignment::Center).with_click_activation(ClickActivationMode::Release)
          .with_debug_name(name));
    };
    decoration(0);
    for (size_t i = 0; i < 6; ++i) decoration(13 + i * 2 + (selected_tab == i ? 1 : 0));
    decoration(1);
    decoration(3);
    decoration(4);
    decoration(5);
    decoration(6);
    decoration(7);
    decoration(2);
    for (size_t i = 0; i < 6; ++i) {
      const auto &a = art[13 + i * 2 + (selected_tab == i ? 1 : 0)];
      if (action(10 + static_cast<int>(i), "", a.x + 8, a.y + 8,
                 selected_tab == i ? 126 : 118, selected_tab == i ? 107 : 76,
                 "kirby_tab_" + std::to_string(i))) choose_tab(i);
    }
    if (action(20, "L", 369, 81, 24, 31, "kirby_previous_tab", true, 20))
      choose_tab((selected_tab + 5) % 6);
    if (action(21, "R", 1138, 54, 24, 31, "kirby_next_tab", true, 20))
      choose_tab((selected_tab + 1) % 6);
    label(22, "Options", 919, 117, 199, 40, 23, raylib::WHITE, "kirby_title");
    label(23, profile.name, 690, 201, 236, 64, 38, ink, "kirby_profile_name");
    label(24, "Delete Data", 1073, 218, 100, 31, 14, ink);
    label(25, "Common", 414, 316, 106, 28, 18, raylib::WHITE, "", "Atkinson");
    if (action(26, "", 578, 172, 426, 120, "kirby_name")) open(Detail::Name);
    if (action(27, "", 1070, 161, 105, 99, "kirby_data")) open(Detail::Data);
    if (action(28, "", 263, 265, 115, 69, "kirby_customize")) open(Detail::Name);
    if (detail == Detail::None) {
      constexpr std::array<const char *, 5> names{"Controller", "Display", "Accessibility", "Sound", "Online"};
      constexpr std::array<std::array<float, 4>, 5> cells{{
          {416, 333, 251, 170}, {668, 324, 251, 170}, {920, 315, 252, 170},
          {423, 516, 376, 121}, {805, 504, 376, 121}}};
      constexpr std::array<std::array<float, 4>, 5> labels{{
          {417, 442, 256, 46}, {669, 433, 256, 46}, {920, 424, 258, 46},
          {577, 554, 157, 48}, {954, 539, 166, 48}}};
      for (size_t i = 0; i < 5; ++i) {
        decoration(8 + i);
        const auto &c = cells[i];
        const auto &l = labels[i];
        label(30 + static_cast<int>(i), names[i], l[0], l[1], l[2], l[3], 24.5f, ink);
        if (action(40 + static_cast<int>(i), "", c[0], c[1], c[2], c[3],
                   "kirby_option_" + std::to_string(i)))
          open(static_cast<Detail>(static_cast<int>(Detail::Controller) + i));
      }
    } else {
      div(context, mk(root.ent(), 50), box(scale, 417, 311, 772, 343)
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
            box(scale, 626, 430, 502, 52).with_font("FredokaMockBold", h720(28))
                .with_custom_background(raylib::WHITE).with_custom_text_color(ink)
                .with_border({200, 184, 147, 255}, 2 * scale).with_corner_radius(5 * scale)
                .with_debug_name("kirby_name_input"));
        input.ent().get<afterhours::text_input::HasTextInputState>().max_length = 24;
        if (focus_name) { context.set_focus(input.ent().id); focus_name = false; }
        label(55, "Choose the name your friends will see.", 447, 503, 695, 38, 20, ink, "", "Atkinson");
        if (action(56, "SAVE NAME", 812, 581, 306, 46, "kirby_name_save", true)) {
          profile.name = editing_name.empty() ? "Name" : editing_name;
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
    div(context, mk(root.ent(), 80), box(scale, 416, 665, 804, 41)
        .with_label(status.empty() ? "Enter a new name and customize your controls." : status)
        .with_font("Atkinson", h720(31.25f)).with_custom_text_color(ink)
        .with_alignment(TextAlignment::Left).with_ignore_pointer_events()
        .with_debug_name("kirby_message"));
  }
};

REGISTER_EXAMPLE_SCREEN(kirby_options, "Game Mockups",
                        "Kirby style options board with profile and preferences",
                        KirbyOptionsScreen)
