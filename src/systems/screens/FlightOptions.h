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
        .with_on_draw_bg([texture = airspace, scale](RectangleType r) {
          paint(texture, r);
          afterhours::draw_rectangle(r, afterhours::Color{1, 5, 14, 125});
          const auto glow = [=](float cx, float cy, float radius, afterhours::Color inner) {
            afterhours::draw_circle_gradient(
                static_cast<int>(r.x + r.width * cx), static_cast<int>(r.y + r.height * cy),
                radius * scale, inner, afterhours::Color{inner.r, inner.g, inner.b, 0});
          };
          glow(.26f, .34f, 430, afterhours::Color{54, 106, 156, 34});
          glow(.16f, .72f, 320, afterhours::Color{44, 92, 140, 28});
          glow(.84f, .28f, 460, afterhours::Color{42, 82, 138, 30});
          glow(.72f, .12f, 280, afterhours::Color{52, 96, 146, 22});
          glow(.55f, .88f, 380, afterhours::Color{36, 74, 124, 22});
          glow(.66f, .52f, 90, afterhours::Color{90, 140, 190, 24});
          glow(.80f, .66f, 64, afterhours::Color{90, 140, 190, 19});
          glow(.58f, .18f, 70, afterhours::Color{90, 140, 190, 17});
          glow(.92f, .44f, 56, afterhours::Color{90, 140, 190, 17});
          const auto dark = afterhours::Color{0, 2, 8, 0};
          const auto shade = afterhours::Color{0, 2, 8, 110};
          afterhours::draw_rectangle_gradient_v({r.x, r.y, r.width, r.height * .16f}, shade, dark);
          afterhours::draw_rectangle_gradient_v({r.x, r.y + r.height * .84f, r.width, r.height * .16f}, dark, shade);
          afterhours::draw_rectangle_gradient_h({r.x, r.y, r.width * .14f, r.height}, shade, dark);
          afterhours::draw_rectangle_gradient_h({r.x + r.width * .86f, r.y, r.width * .14f, r.height}, dark, shade);
          afterhours::draw_line_ex({r.x + r.width * .06f, r.y}, {r.x + r.width * .22f, r.y + r.height},
                                   1.f, afterhours::Color{140, 190, 225, 58});
          afterhours::draw_line_ex({r.x, r.y + r.height * .18f}, {r.x + r.width * .38f, r.y + r.height * .30f},
                                   1.f, afterhours::Color{140, 190, 225, 46});
          afterhours::draw_line_ex({r.x + r.width * .9f, r.y + r.height * .06f}, {r.x + r.width * .62f, r.y + r.height * .42f},
                                   1.f, afterhours::Color{120, 170, 210, 30});
          afterhours::draw_line_ex({r.x + r.width * .06f, r.y + r.height * .94f}, {r.x + r.width * .5f, r.y + r.height * .58f},
                                   1.f, afterhours::Color{120, 170, 210, 26});
        }));
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
    label(1, "OPTIONS", 136, 48, 380, 40, 32, cyan, "flight_title", true);
    if (!options_open) {
      label(2, "OPTIONS CLOSED", 136, 180, 590, 38, 28, bright);
      label(3, status, 136, 230, 590, 36, 20, cyan, "flight_status");
      if (action(4, "RETURN TO OPTIONS", 136, 280, 260, 38, 22, "flight_reopen")) {
        options_open = true;
        editing = false;
        status.clear();
      }
      return;
    }
    div(context, mk(content.ent()), box(scale, 118, 0, 2, 720)
        .with_custom_background({140, 190, 225, 110}).with_ignore_pointer_events()
        .with_debug_name("flight_navigation"));
    const auto category_y = [](size_t i) {
      return 140.f + static_cast<float>(i) * 30;
    };
    const float glow_y = category_y(active_tab);
    div(context, mk(content.ent()), box(scale, 118, glow_y, 640, 28)
        .with_on_draw_bg([scale](RectangleType glow) {
          afterhours::draw_rectangle_gradient_h(
              glow, afterhours::Color{255, 196, 96, 64}, afterhours::Color{255, 196, 96, 0});
          afterhours::draw_rectangle({glow.x, glow.y + glow.height * .46f, glow.width * .9f, 2.f * scale},
                                     afterhours::Color{255, 224, 150, 150});
          afterhours::draw_circle_gradient(static_cast<int>(glow.x), static_cast<int>(glow.y + glow.height * .5f),
                                           10.f * scale, afterhours::Color{255, 230, 170, 200},
                                           afterhours::Color{255, 230, 170, 0});
        }).with_ignore_pointer_events());
    div(context, mk(content.ent()), box(scale, 118, glow_y + 2, 4, 20)
        .with_custom_background(gold).with_ignore_pointer_events());
    for (size_t i = 0; i < categories.size(); ++i) {
      auto category = action(20 + static_cast<int>(i), categories[i].name,
          136, category_y(i), 240, 28, 18, "flight_category_" + std::to_string(i),
          false, active_tab == i ? bright : cyan);
      category.ent().get<HasLabel>().text_x_offset = 8 * scale;
      if (category) choose_category(i);
      if (focus_category == static_cast<int>(i)) {
        context.set_focus(category.ent().id);
        focus_category = -1;
      }
    }
    auto &rows = categories[active_tab].options;
    label(15, categories[active_tab].name, 400, 140, 400, 24, 16, muted, "flight_subheading");
    for (size_t i = 0; i < rows.size(); ++i) {
      const float y = 168 + static_cast<float>(i) * 36;
      const bool selected = selected_option == i;
      if (selected)
        div(context, mk(content.ent(), 100 + static_cast<int>(i)), box(scale, 400, y, 400, 30)
            .with_custom_background(afterhours::Color{31, 61, 83, 120})
            .with_ignore_pointer_events());
      auto option = action(40 + static_cast<int>(i), "", 400, y, 400, 30, 19,
          "flight_option_" + std::to_string(i), rows[i].unavailable);
      if (rows[i].unavailable) option.ent().removeComponent<HasClickListener>();
      label(110 + static_cast<int>(i), rows[i].name + (rows[i].unavailable ? " (Unavailable)" : ""),
            408, y + 3, 250, 26, 19, rows[i].unavailable ? muted : selected ? gold : cyan);
      auto value_label = label(120 + static_cast<int>(i), rows[i].values.empty() ? "Restore" : rows[i].values[rows[i].value],
            662, y + 3, 126, 26, 19, rows[i].unavailable ? muted : bright, "flight_row_value_" + std::to_string(i));
      value_label.ent().get<HasLabel>().alignment = TextAlignment::Right;
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
    if (active_tab <= 1)
      label(130, "Vibration requires a compatible controller.", 400, 312, 420, 20, 15, muted);
    const float detail_y = 168.f + static_cast<float>(rows.size()) * 36.f + 24.f;
    label(60, editing ? "EDIT PREFERENCE" : "CURRENT SELECTION", 400, detail_y, 260, 22, 15, muted);
    label(70, option.name, 400, detail_y + 22, 200, 26, 18, bright, "flight_edit_name");
    label(71, option.values.empty() ? "Restore" : option.values[option.value], 604, detail_y + 22, 196, 26, 18, gold, "flight_value");
    label(76, option.values.empty() ? "" : "Saved: " + option.values[option.saved], 400, detail_y + 48, 200, 22, 16, cyan, "flight_saved_value");
    label(77, option.value == option.saved ? "Matches saved preference" : "Unapplied change", 604, detail_y + 48, 196, 22, 16,
          option.value == option.saved ? muted : gold, "flight_pending");
    if (editing) {
      if (action(72, "<", 400, detail_y + 74, 40, 30, 20, "flight_previous")) step(-1);
      if (action(73, ">", 448, detail_y + 74, 40, 30, 20, "flight_next")) step(1);
      if (action(74, "APPLY", 496, detail_y + 74, 90, 30, 18, "flight_apply")) save();
      if (action(75, "CANCEL CHANGES", 594, detail_y + 74, 150, 30, 18, "flight_cancel")) {
        restore();
        editing = false;
      }
    } else label(78, "Select a setting to edit. Left / right changes its value.", 400, detail_y + 74, 400, 24, 16, cyan);
    label(80, option.help, 136, 500, 520, 40, 18, cyan, "flight_help");
    if (!status.empty()) label(81, status, 136, 540, 560, 26, 16, gold, "flight_status");
    const auto keycap = [&](int id, const std::string &key, float x, float width) {
      div(context, mk(content.ent(), id), box(scale, x, 620, width, 28).with_label(key)
          .with_font("ArchivoMockBold", pixels(16 * scale)).with_custom_text_color(bright)
          .with_alignment(TextAlignment::Center).with_border({165, 206, 251, 255}, scale)
          .with_corner_radius(3 * scale).with_ignore_pointer_events());
    };
    keycap(90, "Enter", 136, 52);
    keycap(91, "Esc", 272, 44);
    if (button(context, mk(content.ent(), 94), box(scale, 196, 620, 56, 28).with_label("OK")
          .with_font("ArchivoMock", pixels(19 * scale)).with_custom_text_color(bright)
          .with_alignment(TextAlignment::Left).with_text_inset(0, 0)
          .with_click_activation(ClickActivationMode::Release)
          .with_debug_name("flight_ok"))) {
      save();
      editing = false;
      options_open = false;
    }
    if (button(context, mk(content.ent(), 95), box(scale, 324, 620, 60, 28).with_label("BACK")
          .with_font("ArchivoMock", pixels(19 * scale)).with_custom_text_color(bright)
          .with_alignment(TextAlignment::Left).with_text_inset(0, 0)
          .with_click_activation(ClickActivationMode::Release)
          .with_debug_name("flight_back"))) {
      if (editing) editing = false;
      else { restore(); options_open = false; }
    }

  }
};

REGISTER_EXAMPLE_SCREEN(flight_options, "Game Mockups",
                        "Ace Combat style options with editable local preferences",
                        FlightOptionsScreen)
