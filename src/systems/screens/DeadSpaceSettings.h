#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>
#include <afterhours/src/plugins/ui/ui_decorators.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct DeadSpaceSettingsScreen : ScreenSystem<UIContext<InputAction>> {
  size_t selected_initial = 1; // Menu Narration selected
  size_t selected_main = 1;    // Gameplay selected

  // Colors matching Dead Space inspiration - dark sci-fi horror aesthetic
  afterhours::Color bg_black{8, 8, 10, 255};
  afterhours::Color panel_dark{18, 28, 32, 255};
  afterhours::Color panel_border{45, 65, 72, 255};
  afterhours::Color teal_highlight{65, 145, 145, 255};
  afterhours::Color teal_bright{85, 175, 175, 255};
  afterhours::Color text_white{220, 230, 235, 255};
  afterhours::Color text_muted{185, 200, 205, 255};
  afterhours::Color text_dim{155, 170, 175, 255};  // Brighter for 4.5:1 on dark

  std::vector<std::string> initial_settings = {
      "Resume Game",
      "Menu Narration",
      "Voice Language",
      "Subtitles",
      "Select Difficulty",
      "Inverted Camera (Y-Axis)",
      "Show Content Warning",
      "More Settings",
  };

  std::vector<std::string> main_settings = {
      "Controls",
      "Gameplay",
      "Display and Graphics",
      "Audio",
      "Language and Subtitles",
      "Accessibility",
      "Policies and Licenses",
      "Credits",
  };

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    Theme theme;
    theme.font = text_white;
    theme.darkfont = bg_black;
    theme.font_muted = text_muted;
    theme.background = bg_black;
    theme.surface = panel_dark;
    theme.primary = teal_highlight;
    theme.secondary = panel_border;
    theme.accent = teal_bright;
    theme.error = afterhours::Color{140, 45, 45, 255};
    theme.roundness = 0.0f;
    theme.segments = 4;
    context.theme = theme;
    UIStylingDefaults::get().set_default_font("EqProRounded", h720(16.0f));

    int screen_w = Settings::get().get_screen_width();
    int screen_h = Settings::get().get_screen_height();
    auto pxf = [](float v) { return pixels(static_cast<int>(v)); };

    // ========== BACKGROUND ==========
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_w), pixels(screen_h)})
            .with_custom_background(bg_black)
            .with_debug_name("bg"));

    // Calculate responsive layout values
    float content_margin = 30.0f;
    float sidebar_w = 200.0f;
    float sidebar_x = content_margin;
    float sidebar_y = 100.0f;
    float gap = 30.0f;
    float panel_x = sidebar_x + sidebar_w + gap;
    float panel_w = (float)screen_w - panel_x - content_margin;
    float panel_y = 60.0f;
    float panel_h = (float)screen_h - panel_y - 90.0f;

    // Subtle decorative tech lines along the bottom edge of the screen
    for (int i = 0; i < 3; i++) {
      div(context, mk(entity, 5 + i),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(40 + i * 20), pixels(1)})
              .with_absolute_position(content_margin + (float)i * 70.0f,
                              (float)screen_h - 20.0f)
              .with_custom_background(afterhours::Color{30, 50, 55, 120})
              .with_debug_name("techline_" + std::to_string(i)));
    }

    // Title removed - consolidated with panel header "// SETTINGS" to avoid duplication

    // ========== LEFT SIDEBAR: INITIAL SETTINGS ==========

    // Sidebar header
    div(context, mk(entity, 60),
        ComponentConfig{}
            .with_label("PAUSE MENU")
            .with_size(
                ComponentSize{pxf(sidebar_w), pixels(32)})
            .with_absolute_position(sidebar_x, sidebar_y)
            .with_font_size(h720(20.0f))
            .with_custom_text_color(text_white)
            .with_padding(Padding{.left = pixels(8)})
            .with_alignment(TextAlignment::Left));

    // Sidebar items
    for (size_t i = 0; i < initial_settings.size(); i++) {
      float item_y = sidebar_y + 40.0f + (float)i * 38.0f;
      bool is_selected = (i == selected_initial);
      afterhours::Color item_color = is_selected ? text_white : text_muted;
      afterhours::Color item_bg = is_selected ? afterhours::Color{35, 70, 72, 255} : afterhours::Color{0, 0, 0, 0};

      // Item background for selection highlight
      if (is_selected) {
        div(context, mk(entity, 65 + static_cast<int>(i)),
            ComponentConfig{}
                .with_size(ComponentSize{
                    pxf(sidebar_w), pixels(34)})
                .with_absolute_position(sidebar_x, item_y)
                .with_custom_background(item_bg)
                .with_border(teal_highlight, 1.0f)
                .with_debug_name("initial_bg_" + std::to_string(i)));
      }

      // Use consistent font size for all sidebar items (no auto-shrink)
      if (button(context, mk(entity, 70 + static_cast<int>(i)),
                 ComponentConfig{}
                     .with_label(initial_settings[i])
                     .with_size(ComponentSize{
                         pxf(sidebar_w), pixels(34)})
                     .with_absolute_position(sidebar_x, item_y)
                     .with_font_size(h720(17.0f))
                     .with_custom_text_color(item_color)
                     .with_padding(Padding{.left = pixels(8)})
                     .with_alignment(TextAlignment::Left))) {
        selected_initial = i;
      }
    }

    // ========== MAIN PANEL: SETTINGS ==========
    // Panel background with border + corner bracket decorations
    div(context, mk(entity, 100),
        ComponentConfig{}
            .with_720p_size(panel_w, panel_h)
            .with_absolute_position(panel_x, panel_y)
            .with_custom_background(panel_dark)
            .with_border(panel_border, 2.0f)
            .with_debug_name("main_panel"))
        .decorate(with_brackets(context, teal_highlight, 15.0f, 2.0f));

    // Panel header: // SETTINGS
    div(context, mk(entity, 110),
        ComponentConfig{}
            .with_label("// SETTINGS")
            .with_size(ComponentSize{pxf(panel_w - 4),
                                     pixels(44)})
            .with_absolute_position(panel_x + 2.0f, panel_y + 2.0f)
            .with_custom_background(afterhours::Color{35, 55, 60, 255})
            .with_font_size(h720(22.0f))
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center));

    // Settings list items
    float header_h = 50.0f;
    float list_y = panel_y + header_h + 10.0f;
    float list_area_h = panel_h - header_h - 20.0f;
    float item_h = list_area_h / (float)main_settings.size();

    for (size_t i = 0; i < main_settings.size(); i++) {
      float item_y = list_y + (float)i * item_h;
      bool is_selected = (i == selected_main);

      // Item background
      afterhours::Color item_bg =
          is_selected ? afterhours::Color{35, 70, 72, 255} : panel_dark;

      div(context, mk(entity, 120 + static_cast<int>(i) * 2),
          ComponentConfig{}
              .with_720p_size(panel_w - 20, item_h - 4)
              .with_absolute_position(panel_x + 10.0f, item_y)
              .with_custom_background(item_bg)
              .with_border(is_selected ? teal_highlight : panel_border, 1.0f)
              .with_debug_name("item_bg_" + std::to_string(i)));

      // Highlight bar on selected
      if (is_selected) {
        div(context, mk(entity, 180 + static_cast<int>(i)),
            ComponentConfig{}
                .with_size(ComponentSize{pxf(panel_w - 22),
                                         pixels(4)})
                .with_absolute_position(panel_x + 11.0f, item_y + item_h - 8.0f)
                .with_custom_background(teal_highlight)
                .with_debug_name("highlight_" + std::to_string(i)));
      }

      // Item text
      afterhours::Color text_color = is_selected ? text_white : text_muted;

      if (button(context, mk(entity, 121 + static_cast<int>(i) * 2),
                 ComponentConfig{}
                     .with_label(main_settings[i])
                     .with_size(
                         ComponentSize{pxf(panel_w - 40),
                                       pxf(item_h - 8)})
                     .with_absolute_position(panel_x + 25.0f, item_y + 8.0f)
                     .with_font_size(h720(20.0f))
                     .with_custom_text_color(text_color)
                     .with_debug_name("setting_" + std::to_string(i)))) {
        selected_main = i;
      }
    }

    // ========== BOTTOM BUTTON PROMPTS ==========
    float prompt_bar_w = 280.0f;
    float prompt_x = panel_x + (panel_w - prompt_bar_w) / 2.0f;
    float prompt_y = panel_y + panel_h + 10.0f;

    // Enter prompt (smaller, just for SELECT)
    div(context, mk(entity, 200),
        ComponentConfig{}
            .with_label("SELECT")
            .with_size(ComponentSize{pixels(70), pixels(22)})
            .with_absolute_position(prompt_x + 105.0f, prompt_y + 3.0f)
            .with_font_size(h720(19.0f))
            .with_custom_text_color(text_white));

    // Main prompt bar
    div(context, mk(entity, 210),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(280), pixels(35)})
            .with_absolute_position(prompt_x, prompt_y + 30.0f)
            .with_custom_background(panel_dark)
            .with_border(panel_border, 1.0f));

    // Enter key
    div(context, mk(entity, 211),
        ComponentConfig{}
            .with_label("[<-")
            .with_size(ComponentSize{pixels(35), pixels(25)})
            .with_absolute_position(prompt_x + 15.0f, prompt_y + 35.0f)
            .with_custom_background(afterhours::Color{55, 75, 80, 255})
            .with_font_size(h720(19.0f))
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center));

    div(context, mk(entity, 212),
        ComponentConfig{}
            .with_label("SELECT")
            .with_size(ComponentSize{pixels(60), pixels(25)})
            .with_absolute_position(prompt_x + 55.0f, prompt_y + 35.0f)
            .with_font_size(h720(19.0f))
            .with_custom_text_color(text_white));

    // Esc key
    div(context, mk(entity, 213),
        ComponentConfig{}
            .with_label("Esc")
            .with_size(ComponentSize{pixels(35), pixels(25)})
            .with_absolute_position(prompt_x + 135.0f, prompt_y + 35.0f)
            .with_custom_background(afterhours::Color{55, 75, 80, 255})
            .with_font_size(h720(19.0f))
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center));

    div(context, mk(entity, 214),
        ComponentConfig{}
            .with_label("BACK")
            .with_size(ComponentSize{pixels(50), pixels(25)})
            .with_absolute_position(prompt_x + 175.0f, prompt_y + 35.0f)
            .with_font_size(h720(19.0f))
            .with_custom_text_color(text_white));

    // (corner brackets applied via .decorate() on main_panel above)
  }
};

REGISTER_EXAMPLE_SCREEN(deadspace_settings, "Game Mockups",
                        "Sci-fi horror settings menu (Dead Space style)",
                        DeadSpaceSettingsScreen)
