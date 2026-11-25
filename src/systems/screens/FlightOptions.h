#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>
#include <array>
#include <string>
#include <vector>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct FlightOptionsScreen : ScreenSystem<UIContext<InputAction>> {
  struct Option {
    std::string name;
    std::vector<std::string> values;
    std::string help;
    size_t value = 0;
    size_t saved = 0;
    bool unavailable = false;
  };
  struct Category { std::string name; std::vector<Option> options; };
  std::array<Category, 9> categories{{
      {"FLIGHT SYSTEM", {
          {"Control Type", {"Standard", "Expert"}, "Choose between standard or expert control schemes."},
          {"High-G Turn Settings", {"Standard", "Hold", "Toggle"}, "Choose how the high-G turn command is activated."},
          {"Reduced Collision Damage", {"Off", "On"}, "Reduce damage from collisions with terrain and aircraft."},
          {"Vibration", {"Unavailable"}, "Vibration requires a compatible controller.", 0, 0, true}}},
      {"CONTROLS", {
          {"Controller Preset", {"Type A", "Type B", "Type C"}, "Choose the controller button layout."},
          {"Button Assignment", {"Default", "Southpaw"}, "Swap the movement and camera stick assignments."},
          {"Vibration", {"Unavailable"}, "Vibration requires a compatible controller.", 0, 0, true}}},
      {"KEYBOARD", {
          {"Keyboard Bindings", {"WASD", "Arrow keys"}, "Choose the keys used for pitch and roll."},
          {"Restore Defaults", {}, "Restore the default keyboard bindings."}}},
      {"MOUSE", {
          {"Sensitivity", {"Low", "Medium", "High"}, "Adjust how quickly mouse movement changes the view.", 1, 1},
          {"Invert Y-Axis", {"Off", "On"}, "Reverse the vertical mouse axis."}}},
      {"FLIGHT STICK", {
          {"Axis Assignment", {"Standard", "Reversed"}, "Choose the preferred pitch axis for a flight stick."},
          {"Dead Zone", {"5%", "10%", "15%"}, "Ignore small movements around the center of the stick.", 1, 1}}},
      {"DISPLAY", {
          {"HUD Display", {"On", "Off"}, "Show or hide flight instruments and targeting information."},
          {"Brightness", {"40%", "50%", "60%", "70%"}, "Set the preferred display brightness.", 1, 1}}},
      {"GRAPHICS", {
          {"Resolution", {"1280 x 720", "1920 x 1080", "2560 x 1440"}, "Choose the preferred game rendering resolution.", 1, 1},
          {"Texture Quality", {"Low", "Medium", "High"}, "Choose texture detail. Higher quality uses more memory.", 2, 2},
          {"Frame Rate", {"30 FPS", "60 FPS", "120 FPS", "Unlimited"}, "Choose the preferred frame rate limit.", 1, 1}}},
      {"SOUND", {
          {"Master Volume", {"0%", "25%", "50%", "75%", "100%"}, "Set the overall audio level.", 4, 4},
          {"Music", {"0%", "25%", "50%", "75%", "100%"}, "Set the background music level.", 3, 3},
          {"Voice", {"0%", "25%", "50%", "75%", "100%"}, "Set the radio and dialogue level.", 4, 4}}},
      {"LANGUAGE", {
          {"Voice Language", {"English", "Japanese"}, "Choose the preferred voice language."},
          {"Text Language", {"English", "Spanish", "French"}, "Choose the preferred text language."}}},
  }};
  size_t active_tab = 0, selected_option = 0;
  bool editing = false, options_open = true, loaded = false;
  int focus_category = -1, focus_option = -1;
  std::string status;
  raylib::Texture2D airspace{};
  const afterhours::Color cyan{155, 223, 255, 255};
  const afterhours::Color bright{228, 249, 255, 255};
  const afterhours::Color gold{255, 244, 192, 255};
  const afterhours::Color muted{154, 184, 205, 255};

  void load() {
    if (loaded) return;
    airspace = raylib::LoadTexture(afterhours::files::get_resource_path(
        "images", "flight_options/airspace.png").string().c_str());
    raylib::SetTextureFilter(airspace, raylib::TEXTURE_FILTER_BILINEAR);
    loaded = true;
  }
  static void paint(raylib::Texture2D texture, RectangleType r) {
    raylib::DrawTexturePro(texture, {0, 0, static_cast<float>(texture.width),
        static_cast<float>(texture.height)}, r, {0, 0}, 0, raylib::WHITE);
  }
  void save() {
    for (auto &category : categories)
      for (auto &option : category.options) option.saved = option.value;
    status = "Settings applied";
  }
  void restore() {
    for (auto &category : categories)
      for (auto &option : category.options) option.value = option.saved;
    status = "Unapplied changes discarded";
  }
  void choose_category(size_t i) {
    active_tab = i;
    selected_option = 0;
    editing = false;
    status.clear();
  }
  void step(int direction) {
    auto &option = categories[active_tab].options[selected_option];
    if (option.unavailable || option.values.empty()) return;
    option.value = (option.value + option.values.size() + direction) % option.values.size();
    status = "Changes pending";
  }
  ComponentConfig box(float scale, float x, float y, float w, float h) const {
    return ComponentConfig{}.with_size({pixels(w * scale), pixels(h * scale)})
        .with_absolute_position(x * scale, y * scale)
        .with_background(Theme::Usage::None).with_corner_radius(0);
  }

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context, float) override {
    load();
    const float scale = std::min(context.screen_height / 720.f, context.screen_width / 1280.f);
    Theme theme;
    theme.font = cyan;
    theme.darkfont = bright;
    theme.font_muted = muted;
    theme.background = {2, 9, 19, 255};
    theme.surface = {10, 35, 73, 255};
    theme.primary = {25, 67, 106, 255};
    theme.accent = gold;
    theme.corner_radius = 0;
    theme.roundness = 0;
    context.set_theme(theme);
    context.scaling_mode = ScalingMode::Proportional;

    if (context.pressed(InputAction::MenuBack)) {
      if (!options_open) { options_open = true; status.clear(); }
      else if (editing) { editing = false; focus_category = static_cast<int>(active_tab); }
      else { restore(); options_open = false; }
    }
    if (options_open) {
      int direction = context.pressed(InputAction::WidgetDown) ? 1 :
                      context.pressed(InputAction::WidgetUp) ? -1 : 0;
      if (direction && !editing) {
        choose_category((active_tab + categories.size() + direction) % categories.size());
        focus_category = static_cast<int>(active_tab);
      } else if (direction && editing) {
        const auto &rows = categories[active_tab].options;
        do { selected_option = (selected_option + rows.size() + direction) % rows.size(); }
        while (rows[selected_option].unavailable || rows[selected_option].values.empty());
        focus_option = static_cast<int>(selected_option);
      }
      if (editing) {
        if (context.pressed(InputAction::WidgetRight)) step(1);
        if (context.pressed(InputAction::WidgetLeft)) step(-1);
      } else if (context.pressed(InputAction::WidgetRight)) {
        const auto &rows = categories[active_tab].options;
        if (rows[selected_option].unavailable || rows[selected_option].values.empty())
          selected_option = 0;
        editing = true;
        focus_option = static_cast<int>(selected_option);
      }
    }

    auto root = div(context, mk(entity, 0), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)}).with_corner_radius(0)
        .with_debug_name("flight_root")
        .with_on_draw_bg([texture = airspace](RectangleType r) { paint(texture, r); }));
    auto content = div(context, mk(root.ent()), ComponentConfig{}
        .with_size({pixels(1280 * scale), pixels(720 * scale)})
        .with_absolute_position((context.screen_width - 1280 * scale) / 2,
                                (context.screen_height - 720 * scale) / 2)
        .with_background(Theme::Usage::None).with_corner_radius(0));
    const auto label = [&](int id, const std::string &text, float x, float y, float w, float h,
                           float size, afterhours::Color color, const std::string &name = "", bool bold = false) {
      return div(context, mk(content.ent(), id), box(scale, x, y, w, h).with_label(text)
          .with_font(bold ? "ArchivoMockBold" : "ArchivoMock", pixels(size * scale))
          .with_custom_text_color(color).with_alignment(TextAlignment::Left).with_text_inset(0, 0)
          .with_text_overflow(TextOverflow::Wrap).with_ignore_pointer_events().with_debug_name(name));
    };
    const auto action = [&](int id, const std::string &text, float x, float y, float w, float h,
                            float size, const std::string &name, bool disabled = false,
                            afterhours::Color color = afterhours::Color{155, 223, 255, 255}) {
      return button(context, mk(content.ent(), id), box(scale, x, y, w, h).with_label(text)
          .with_font("ArchivoMock", pixels(size * scale)).with_custom_text_color(color)
          .with_alignment(TextAlignment::Left).with_text_inset(0, 0)
          .with_disabled(disabled).with_click_activation(ClickActivationMode::Release).with_debug_name(name));
    };
    label(1, "OPTIONS", 138, 62, 380, 48, 38, bright, "flight_title", true);
    if (!options_open) {
      label(2, "OPTIONS CLOSED", 138, 220, 590, 42, 30, bright);
      label(3, status, 138, 270, 590, 44, 23, cyan, "flight_status");
      if (action(4, "RETURN TO OPTIONS", 138, 344, 340, 44, 25, "flight_reopen")) {
        options_open = true;
        editing = false;
        status.clear();
      }
      return;
    }
    const auto category_y = [](size_t i) { return 176.f + static_cast<float>(i) * 38.f; };
    // glowing blue highlight band + orange flare on active category
    div(context, mk(content.ent()), box(scale, 130, category_y(active_tab), 360, 36)
        .with_ignore_pointer_events().with_on_draw_bg([](RectangleType r) {
          afterhours::draw_rectangle_gradient_h(r, {28, 110, 190, 175}, {28, 110, 190, 0});
        }));
    div(context, mk(content.ent()), box(scale, 122, category_y(active_tab) + 10, 16, 16)
        .with_ignore_pointer_events().with_on_draw_fg([](RectangleType r) {
          const float cx = r.x + r.width / 2, cy = r.y + r.height / 2;
          afterhours::draw_poly({cx, cy}, 4, r.width * .42f, 0, {255, 170, 60, 255});
        }));
    for (size_t i = 0; i < categories.size(); ++i) {
      auto category = action(20 + static_cast<int>(i), categories[i].name,
          138, category_y(i), 310, 36, 23, "flight_category_" + std::to_string(i),
          false, active_tab == i ? bright : cyan);
      category.ent().get<HasLabel>().text_x_offset = 16 * scale;
      if (category) choose_category(i);
      if (focus_category == static_cast<int>(i)) {
        context.set_focus(category.ent().id);
        focus_category = -1;
      }
    }
    // floating faint preview column of the active category's sub-items
    auto &rows = categories[active_tab].options;
    for (size_t i = 0; i < rows.size(); ++i) {
      const float y = 258 + static_cast<float>(i) * 26;
      const bool selected = selected_option == i;
      auto option = button(context, mk(content.ent(), 40 + static_cast<int>(i)),
          box(scale, 360, y, 270, 24).with_label(rows[i].name)
              .with_font("ArchivoMock", pixels(17 * scale))
              .with_custom_text_color(selected && editing ? gold : afterhours::Color{165, 215, 245, 255})
              .with_alignment(TextAlignment::Left).with_text_inset(0, 0)
              .with_click_activation(ClickActivationMode::Release)
              .with_debug_name("flight_option_" + std::to_string(i)));
      option.ent().get<HasLabel>().text_x_offset = 8 * scale;
      if (option && !rows[i].unavailable) {
        selected_option = i;
        if (rows[i].values.empty()) {
          rows[0].value = 0;
          selected_option = 0;
          editing = false;
          status = "Keyboard bindings restored to WASD";
        } else { editing = true; status.clear(); }
      }
      if (focus_option == static_cast<int>(i)) {
        context.set_focus(option.ent().id);
        focus_option = -1;
      }
    }
    const auto &option = rows[selected_option];
    // editor (only while editing): floating value + prev/next + apply/cancel, no boxes
    if (editing) {
      label(70, option.name, 668, 252, 360, 40, 27, bright, "flight_edit_name", true);
      auto value_label = label(71, option.values.empty() ? "Restore" : option.values[option.value], 712, 302, 220, 44, 28, gold, "flight_value", true);
      value_label.ent().get<HasLabel>().alignment = TextAlignment::Center;
      if (action(72, "<", 668, 302, 36, 44, 30, "flight_previous")) step(-1);
      if (action(73, ">", 940, 302, 36, 44, 30, "flight_next")) step(1);
      if (action(74, "APPLY", 668, 366, 120, 40, 22, "flight_apply")) save();
      if (action(75, "CANCEL CHANGES", 806, 366, 180, 40, 21, "flight_cancel")) {
        restore();
        editing = false;
      }
    }
    // bottom-left description + status
    std::string desc = "Select the in-flight system.";
    if (active_tab == 1) desc = "Select the controls.";
    else if (active_tab == 2) desc = "Select the keyboard settings.";
    else if (active_tab == 3) desc = "Select the mouse settings.";
    else if (active_tab == 4) desc = "Select the flight stick settings.";
    else if (active_tab == 5) desc = "Select the display settings.";
    else if (active_tab == 6) desc = "Select the graphics settings.";
    else if (active_tab == 7) desc = "Select the sound settings.";
    else if (active_tab == 8) desc = "Select the language settings.";
    label(80, editing ? option.help : desc, 150, 578, 560, 34, 21, cyan, "flight_help");
    if (!status.empty()) label(81, status, 150, 548, 560, 28, 20, gold, "flight_status");
    // minimal bottom-left prompts: [Enter] OK  [Esc] BACK
    if (action(94, "[Enter] OK", 150, 640, 150, 36, 20, "flight_ok")) {
      save();
      editing = false;
      options_open = false;
    }
    if (action(95, "[Esc] BACK", 318, 640, 140, 36, 20, "flight_back")) {
      if (editing) editing = false;
      else { restore(); options_open = false; }
    }
  }
};

REGISTER_EXAMPLE_SCREEN(flight_options, "Game Mockups",
                        "Ace Combat style options with editable local preferences",
                        FlightOptionsScreen)
