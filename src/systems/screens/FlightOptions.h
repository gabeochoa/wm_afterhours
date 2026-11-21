#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include "../SettingsPage.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>
#include <string>
#include <vector>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Settings data lives in a settings_kit::Page (tabs of stepper rows with
// saved snapshots); the kit renders the category column and the row stack.
// This screen keeps only its bespoke chrome: artwork, preview pane, help
// text, footer, and the modal select -> edit -> apply/cancel flow.
// Gap in the vendored widget, reported rather than forked: setting_row has
// no action/button control, so the KEYBOARD tab's "Restore Defaults" command
// is drawn here as a plain button after the kit's rows, in the next row slot.
struct FlightOptionsScreen : ScreenSystem<UIContext<InputAction>> {
  settings_kit::Page page{{
      {"FLIGHT SYSTEM", {settings_kit::stepper_row("Control Type", {"Standard", "Expert"}, 0, "Choose between standard or expert control schemes."),
                          settings_kit::stepper_row("High-G Turn Settings", {"Standard", "Hold", "Toggle"}, 0, "Choose how the high-G turn command is activated."),
                          settings_kit::stepper_row("Reduced Collision Damage", {"Off", "On"}, 0, "Reduce damage from collisions with terrain and aircraft."),
                          settings_kit::unavailable_row("Vibration (Unavailable)", "Vibration requires a compatible controller.")}},
      {"CONTROLS", {settings_kit::stepper_row("Controller Preset", {"Type A", "Type B", "Type C"}, 0, "Choose the controller button layout."),
                     settings_kit::stepper_row("Button Assignment", {"Default", "Southpaw"}, 0, "Swap the movement and camera stick assignments."),
                     settings_kit::unavailable_row("Vibration (Unavailable)", "Vibration requires a compatible controller.")}},
      {"KEYBOARD", {settings_kit::stepper_row("Keyboard Bindings", {"WASD", "Arrow keys"}, 0, "Choose the keys used for pitch and roll.")}},
      {"MOUSE", {settings_kit::stepper_row("Sensitivity", {"Low", "Medium", "High"}, 1, "Adjust how quickly mouse movement changes the view."),
                  settings_kit::stepper_row("Invert Y-Axis", {"Off", "On"}, 0, "Reverse the vertical mouse axis.")}},
      {"FLIGHT STICK", {settings_kit::stepper_row("Axis Assignment", {"Standard", "Reversed"}, 0, "Choose the preferred pitch axis for a flight stick."),
                         settings_kit::stepper_row("Dead Zone", {"5%", "10%", "15%"}, 1, "Ignore small movements around the center of the stick.")}},
      {"DISPLAY", {settings_kit::stepper_row("HUD Display", {"On", "Off"}, 0, "Show or hide flight instruments and targeting information."),
                    settings_kit::stepper_row("Brightness", {"40%", "50%", "60%", "70%"}, 1, "Set the preferred display brightness.")}},
      {"GRAPHICS", {settings_kit::stepper_row("Resolution", {"1280 x 720", "1920 x 1080", "2560 x 1440"}, 1, "Choose the preferred game rendering resolution."),
                     settings_kit::stepper_row("Texture Quality", {"Low", "Medium", "High"}, 2, "Choose texture detail. Higher quality uses more memory."),
                     settings_kit::stepper_row("Frame Rate", {"30 FPS", "60 FPS", "120 FPS", "Unlimited"}, 1, "Choose the preferred frame rate limit.")}},
      {"SOUND", {settings_kit::stepper_row("Master Volume", {"0%", "25%", "50%", "75%", "100%"}, 4, "Set the overall audio level."),
                  settings_kit::stepper_row("Music", {"0%", "25%", "50%", "75%", "100%"}, 3, "Set the background music level."),
                  settings_kit::stepper_row("Voice", {"0%", "25%", "50%", "75%", "100%"}, 4, "Set the radio and dialogue level.")}},
      {"LANGUAGE", {settings_kit::stepper_row("Voice Language", {"English", "Japanese"}, 0, "Choose the preferred voice language."),
                     settings_kit::stepper_row("Text Language", {"English", "Spanish", "French"}, 0, "Choose the preferred text language.")}},
  }};
  bool editing = false, options_open = true, loaded = false;
  int focus_category = -1, focus_option = -1;
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
    page.apply();
    page.status = "Settings applied";
  }
  void restore() {
    page.revert();
    page.status = "Unapplied changes discarded";
  }
  void choose_category(size_t i) {
    page.select_tab(i);
    editing = false;
    page.status.clear();
  }
  void step(int direction) {
    if (page.step_selected(direction)) page.status = "Changes pending";
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
      if (!options_open) { options_open = true; page.status.clear(); }
      else if (editing) { editing = false; focus_category = static_cast<int>(page.active_tab); }
      else { restore(); options_open = false; }
    }
    if (options_open) {
      int direction = context.pressed(InputAction::WidgetDown) ? 1 :
                      context.pressed(InputAction::WidgetUp) ? -1 : 0;
      if (direction && !editing) {
        choose_category((page.active_tab + page.tabs.size() + direction) % page.tabs.size());
        focus_category = static_cast<int>(page.active_tab);
      } else if (direction && editing) {
        page.move_selection(direction);
        focus_option = static_cast<int>(page.selected_row);
      }
      if (editing) {
        if (context.pressed(InputAction::WidgetRight)) step(1);
        if (context.pressed(InputAction::WidgetLeft)) step(-1);
      } else if (context.pressed(InputAction::WidgetRight)) {
        if (page.selected().unavailable) page.selected_row = 0;
        editing = true;
        focus_option = static_cast<int>(page.selected_row);
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
    label(1, "OPTIONS", 92, 55, 380, 48, 40, bright, "flight_title", true);
    label(6, "FLIGHT CONFIGURATION", 94, 105, 560, 28, 20, cyan);
    if (!options_open) {
      div(context, mk(content.ent()), box(scale, 76, 196, 660, 224)
          .with_custom_background({3, 18, 34, 235}));
      label(2, "OPTIONS CLOSED", 100, 220, 590, 42, 30, bright);
      label(3, page.status, 100, 270, 590, 44, 23, cyan, "flight_status");
      if (action(4, "RETURN TO OPTIONS", 100, 344, 340, 44, 25, "flight_reopen")) {
        options_open = true;
        editing = false;
        page.status.clear();
      }
      return;
    }
    div(context, mk(content.ent()), box(scale, 76, 136, 272, 426)
        .with_custom_background({3, 18, 34, 225}).with_debug_name("flight_navigation"));
    div(context, mk(content.ent()), box(scale, 384, 136, 456, 426)
        .with_custom_background({3, 18, 34, 235}).with_debug_name("flight_settings"));
    div(context, mk(content.ent()), box(scale, 868, 136, 324, 426)
        .with_custom_background({5, 24, 44, 235}).with_border({78, 124, 151, 255}, scale)
        .with_debug_name("flight_preview"));
    label(12, "INPUT", 102, 139, 224, 23, 16, muted);

    settings_kit::Style style;
    style.font_name = "ArchivoMock";
    style.tab_debug_prefix = "flight_category_";
    style.row_debug_prefix = "flight_option_";
    style.tab_alignment = TextAlignment::Left;
    style.tab_width = 252 * scale;
    style.tab_height = 34 * scale;
    style.tab_gap = 4 * scale;
    style.row_height = 42 * scale;
    style.row_spacing = 6 * scale;
    style.font_size = 22 * scale;
    style.focus_tab = focus_category;
    style.focus_row = focus_option;
    style.row_bg = {12, 35, 53, 170};
    style.row_bg_selected = {31, 61, 83, 210};
    style.tab_bg = {0, 0, 0, 0};
    style.tab_bg_selected = {109, 93, 53, 105};
    style.label_color = cyan;
    style.label_color_selected = gold;
    style.tab_text = cyan;
    style.tab_text_selected = bright;

    auto tabs_result = settings_kit::render_tabs(
        context, mk(content.ent(), 500), page, box(scale, 86, 164, 252, 386), style, true);
    if (tabs_result.tab_changed) { editing = false; page.status.clear(); }
    label(15, page.tabs[page.active_tab].name, 404, 153, 416, 38, 27, bright, "flight_subheading", true);
    const float rows_h = static_cast<float>(page.tabs[page.active_tab].rows.size()) * 48.f;
    auto rows_result = settings_kit::render_rows(
        context, mk(content.ent(), 501), page, box(scale, 396, 208, 432, rows_h), style);
    focus_category = -1;
    focus_option = -1;
    if (rows_result.row_selected) {
      editing = !page.selected().unavailable;
      if (editing) page.status.clear();
    }
    if (rows_result.row_changed) {
      editing = true;
      page.status = "Changes pending";
    }
    // Command row the kit cannot express (see header note): same slot the
    // hand-drawn screen gave its second KEYBOARD row.
    if (page.active_tab == 2) {
      auto restore_btn = action(41, "Restore Defaults", 396, 256, 432, 42, 22, "flight_option_1");
      restore_btn.ent().get<HasLabel>().text_x_offset = 12 * scale;
      label(121, "Restore", 688, 259, 126, 36, 22, bright, "flight_row_value_1")
          .ent().get<HasLabel>().alignment = TextAlignment::Right;
      if (restore_btn) {
        page.tabs[2].rows[0].index = 0;
        page.selected_row = 0;
        editing = false;
        page.status = "Keyboard bindings restored to WASD";
      }
    }
    const auto &option = page.selected();
    if (page.active_tab <= 1)
      label(130, "Vibration requires a compatible controller.", 404, 410, 416, 30, 20, muted);
    label(80, option.help, 404, 451, 416, 80, 23, cyan, "flight_help");
    label(60, editing ? "EDIT PREFERENCE" : "CURRENT SELECTION", 888, 153, 284, 28, 18, muted);
    label(70, option.label, 888, 197, 284, 68, 28, bright, "flight_edit_name", true);
    label(71, option.current_text(), 934, 284, 204, 44, 29, gold, "flight_value", true);
    label(76, "Saved: " + option.saved_text(), 888, 350, 284, 32, 23, cyan, "flight_saved_value");
    label(77, option.dirty() ? "Unapplied change" : "Matches saved preference", 888, 389, 284, 30, 22,
          option.dirty() ? gold : muted, "flight_pending");
    if (editing) {
      if (action(72, "<", 890, 284, 32, 44, 32, "flight_previous")) step(-1);
      if (action(73, ">", 1150, 284, 32, 44, 32, "flight_next")) step(1);
      if (action(74, "APPLY", 888, 447, 122, 44, 23, "flight_apply")) save();
      if (action(75, "CANCEL CHANGES", 1026, 447, 146, 44, 22, "flight_cancel")) {
        restore();
        editing = false;
      }
    } else label(78, "Select a setting to edit.\nLeft / right changes its value.", 888, 448, 284, 60, 22, cyan);
    settings_kit::render_status(context, mk(content.ent(), 81), page,
        box(scale, 404, 568, 788, 28).with_font("ArchivoMock", pixels(21 * scale))
            .with_custom_text_color(gold).with_alignment(TextAlignment::Left)
            .with_debug_name("flight_status"));
    div(context, mk(content.ent(), 82), box(scale, 76, 606, 1116, 98)
        .with_custom_background({3, 18, 34, 240}).with_debug_name("flight_footer"));
    const auto keycap = [&](int id, const std::string &key, float x, float width) {
      div(context, mk(content.ent(), id), box(scale, x, 630, width, 34).with_label(key)
          .with_font("ArchivoMockBold", pixels(22 * scale)).with_custom_text_color(bright)
          .with_alignment(TextAlignment::Center).with_border({165, 206, 251, 255}, scale)
          .with_corner_radius(3 * scale).with_ignore_pointer_events());
    };
    keycap(90, "Enter", 100, 64);
    keycap(91, "Esc", 306, 52);
    label(92, "SELECT", 176, 630, 108, 34, 24, cyan);
    label(93, "BACK", 370, 630, 92, 34, 24, cyan);
    label(96, "Enter activates the focused control.", 100, 670, 600, 24, 18, muted);
    if (action(94, "APPLY & CLOSE", 770, 630, 208, 44, 24, "flight_ok")) {
      save();
      editing = false;
      options_open = false;
    }
    if (action(95, "BACK", 1026, 630, 146, 44, 24, "flight_back")) {
      if (editing) editing = false;
      else { restore(); options_open = false; }
    }
  }
};

REGISTER_EXAMPLE_SCREEN(flight_options, "Game Mockups",
                        "Ace Combat style options with editable local preferences",
                        FlightOptionsScreen)
