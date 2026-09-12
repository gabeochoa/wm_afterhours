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
  raylib::Texture2D airspace{}, selection{};
  const afterhours::Color cyan{155, 223, 255, 255};
  const afterhours::Color bright{228, 249, 255, 255};
  const afterhours::Color gold{255, 244, 192, 255};
  const afterhours::Color muted{101, 157, 185, 255};

  void load() {
    if (loaded) return;
    airspace = raylib::LoadTexture(afterhours::files::get_resource_path(
        "images", "flight_options/airspace.png").string().c_str());
    selection = raylib::LoadTexture(afterhours::files::get_resource_path(
        "images", "flight_options/selection.png").string().c_str());
    raylib::SetTextureFilter(airspace, raylib::TEXTURE_FILTER_BILINEAR);
    raylib::SetTextureFilter(selection, raylib::TEXTURE_FILTER_BILINEAR);
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
    const float scale = context.screen_height / 720.f;
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
    UIStylingDefaults::get().set_default_font("Archivo", h720(20));

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

    auto root = div(context, mk(entity, 0), box(scale, 0, 0, 1280, 720)
        .with_debug_name("flight_root")
        .with_on_draw_bg([texture = airspace](RectangleType r) { paint(texture, r); }));
    auto label = [&](int id, const std::string &text, float x, float y, float w, float h,
                     float size, afterhours::Color color, const std::string &name = "") {
      return div(context, mk(root.ent(), id), box(scale, x, y, w, h).with_label(text)
          .with_font("Archivo", h720(size * 1.25f)).with_custom_text_color(color)
          .with_alignment(TextAlignment::Left).with_text_inset(0, 0)
          .with_text_shadow({48, 117, 176, 200}, 0, scale)
          .with_ignore_pointer_events().with_debug_name(name));
    };
    auto action = [&](int id, const std::string &text, float x, float y, float w, float h,
                      float size, const std::string &name, bool disabled = false,
                      afterhours::Color color = afterhours::Color{155, 223, 255, 255}) {
      return button(context, mk(root.ent(), id), box(scale, x, y, w, h).with_label(text)
          .with_font("Archivo", h720(size * 1.25f)).with_custom_text_color(color)
          .with_alignment(TextAlignment::Left).with_text_inset(0, 0)
          .with_disabled(disabled).with_click_activation(ClickActivationMode::Release)
          .with_text_shadow({48, 117, 176, 200}, 0, scale)
          .with_debug_name(name));
    };
    label(1, "OPTIONS", 135, 62, 260, 39, 27, cyan, "flight_title");

    if (!options_open) {
      label(2, "OPTIONS CLOSED", 153, 230, 370, 42, 25, bright);
      label(3, status, 153, 278, 540, 34, 18, cyan, "flight_status");
      if (action(4, "RETURN TO OPTIONS", 153, 342, 270, 34, 20, "flight_reopen")) {
        options_open = true;
        editing = false;
        status.clear();
      }
      return;
    }

    div(context, mk(root.ent(), 5), box(scale, 108, 123 + active_tab * 27.f, 360, 67)
        .with_ignore_pointer_events().with_on_draw_bg([texture = selection](RectangleType r) { paint(texture, r); }));
    for (size_t i = 0; i < categories.size(); ++i) {
      auto category = action(20 + static_cast<int>(i), categories[i].name,
          153, 143 + i * 27.f, 165, 27, 20, "flight_category_" + std::to_string(i),
          false, active_tab == i ? bright : cyan);
      if (category) choose_category(i);
      if (focus_category == static_cast<int>(i)) {
        context.set_focus(category.ent().id);
        focus_category = -1;
      }
    }
    auto &rows = categories[active_tab].options;
    for (size_t i = 0; i < rows.size(); ++i) {
      auto option = action(40 + static_cast<int>(i),
          rows[i].name + (rows[i].unavailable ? " (Unavailable)" : ""), 333,
          226 + i * 22.f, 230, 17, 13, "flight_option_" + std::to_string(i),
          rows[i].unavailable, rows[i].unavailable ? muted : editing && selected_option == i ? gold : cyan);
      if (option) {
        selected_option = i;
        if (rows[i].values.empty()) {
          rows[0].value = 0;
          editing = false;
          status = "Keyboard bindings restored to WASD";
        } else { editing = true; status.clear(); }
      }
      if (focus_option == static_cast<int>(i)) {
        context.set_focus(option.ent().id);
        focus_option = -1;
      }
    }
    if (editing) {
      auto &option = rows[selected_option];
      label(70, option.name, 333, 342, 430, 28, 20, bright, "flight_edit_name");
      label(71, option.values[option.value], 370, 386, 260, 32, 22, gold, "flight_value");
      if (action(72, "<", 333, 386, 29, 32, 22, "flight_previous")) step(-1);
      if (action(73, ">", 640, 386, 29, 32, 22, "flight_next")) step(1);
      if (action(74, "APPLY", 333, 445, 91, 30, 17, "flight_apply")) save();
      if (action(75, "CANCEL CHANGES", 451, 445, 180, 30, 17, "flight_cancel")) {
        restore();
        editing = false;
      }
    }
    label(80, editing ? rows[selected_option].help :
          active_tab == 0 ? "Select the in-flight system." : "Configure " + categories[active_tab].name + " settings.",
          153, 547, 900, 30, 18, cyan, "flight_help");
    if (!status.empty()) label(81, status, 153, 587, 720, 28, 16, gold, "flight_status");
    auto keycap = [&](int id, const std::string &key, float x) {
      div(context, mk(root.ent(), id), box(scale, x, 659, 24, 26).with_label(key)
          .with_font("Archivo", h720(11 * 1.25f)).with_custom_text_color(cyan)
          .with_alignment(TextAlignment::Center).with_border({165, 206, 251, 255}, scale)
          .with_corner_radius(3 * scale).with_ignore_pointer_events());
    };
    keycap(90, "Enter", 149);
    keycap(91, "Esc", 221);
    label(92, "OK", 184, 655, 32, 34, 19, cyan);
    label(93, "BACK", 255, 655, 65, 34, 19, cyan);
    if (action(94, "", 149, 655, 64, 34, 19, "flight_ok")) {
      save();
      editing = false;
      options_open = false;
    }
    if (action(95, "", 221, 655, 100, 34, 19, "flight_back")) {
      if (editing) editing = false;
      else { restore(); options_open = false; }
    }
  }
};

REGISTER_EXAMPLE_SCREEN(flight_options, "Game Mockups",
                        "Ace Combat style options with editable local preferences",
                        FlightOptionsScreen)
