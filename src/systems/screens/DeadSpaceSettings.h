#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct DeadSpaceSettingsScreen : ScreenSystem<UIContext<InputAction>> {
  size_t selected_initial = 1; // Continue selected
  size_t selected_main = 1;    // Gameplay selected

  // Colors matching Dead Space inspiration - dark sci-fi horror aesthetic
  afterhours::Color bg_black{8, 8, 10, 255};
  afterhours::Color panel_dark{18, 28, 32, 255};
  afterhours::Color panel_border{45, 65, 72, 255};
  afterhours::Color teal_highlight{65, 145, 145, 255};
  afterhours::Color teal_bright{85, 175, 175, 255};
  afterhours::Color text_white{195, 205, 210, 255};
  afterhours::Color text_muted{95, 110, 115, 255};
  afterhours::Color text_dim{55, 65, 70, 255};

  std::vector<std::string> initial_settings = {
      "Continue",
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

    int screen_w = Settings::get().get_screen_width();
    int screen_h = Settings::get().get_screen_height();

    // ========== BACKGROUND ==========
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_w), pixels(screen_h)})
            .with_custom_background(bg_black)
            .with_debug_name("bg"));

    // Subtle vertical scan lines effect
    for (int i = 0; i < 30; i++) {
      if (i % 3 == 0) {
        div(context, mk(entity, 5 + i),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(1), pixels(screen_h)})
                .with_absolute_position()
                .with_translate((float)screen_w - 120.0f + (float)i * 4.0f,
                                0.0f)
                .with_custom_background(afterhours::Color{15, 20, 22, 180})
                .with_debug_name("scanline_" + std::to_string(i)));
      }
    }

    // ========== TITLE: MORE SETTINGS ==========
    div(context, mk(entity, 50),
        ComponentConfig{}
            .with_label("MORE SETTINGS")
            .with_size(ComponentSize{pixels(280), pixels(40)})
            .with_absolute_position()
            .with_translate(350.0f, 55.0f)
            .with_font("EqProRounded", 26.0f)
            .with_custom_text_color(teal_bright)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("title"));

    // ========== LEFT SIDEBAR: INITIAL SETTINGS ==========
    float sidebar_x = 35.0f;
    float sidebar_y = 150.0f;
    float sidebar_w = 230.0f;

    // Sidebar header
    div(context, mk(entity, 60),
        ComponentConfig{}
            .with_label("INITIAL SETTINGS")
            .with_size(
                ComponentSize{pixels(static_cast<int>(sidebar_w)), pixels(28)})
            .with_absolute_position()
            .with_translate(sidebar_x, sidebar_y)
            .with_font("EqProRounded", 19.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("sidebar_header"));

    // Sidebar items
    for (size_t i = 0; i < initial_settings.size(); i++) {
      float item_y = sidebar_y + 35.0f + (float)i * 32.0f;
      bool is_selected = (i == selected_initial);
      afterhours::Color item_color = is_selected ? text_white : text_dim;

      if (button(context, mk(entity, 70 + static_cast<int>(i)),
                 ComponentConfig{}
                     .with_label(initial_settings[i])
                     .with_size(ComponentSize{
                         pixels(static_cast<int>(sidebar_w)), pixels(28)})
                     .with_absolute_position()
                     .with_translate(sidebar_x, item_y)
                     .with_font("EqProRounded", 19.0f)
                     .with_custom_text_color(item_color)
                     .with_debug_name("initial_" + std::to_string(i)))) {
        selected_initial = i;
      }
    }

    // ========== MAIN PANEL: SETTINGS ==========
    float panel_x = 300.0f;
    float panel_y = 100.0f;
    float panel_w = 520.0f;
    float panel_h = 450.0f;

    // Panel background with border
    div(context, mk(entity, 100),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(panel_w)),
                                     pixels(static_cast<int>(panel_h))})
            .with_absolute_position()
            .with_translate(panel_x, panel_y)
            .with_custom_background(panel_dark)
            .with_border(panel_border, 2.0f)
            .with_debug_name("main_panel"));

    // Panel header: // SETTINGS
    div(context, mk(entity, 110),
        ComponentConfig{}
            .with_label("// SETTINGS")
            .with_size(ComponentSize{pixels(static_cast<int>(panel_w - 4)),
                                     pixels(38)})
            .with_absolute_position()
            .with_translate(panel_x + 2.0f, panel_y + 2.0f)
            .with_custom_background(afterhours::Color{35, 55, 60, 255})
            .with_font("EqProRounded", 20.0f)
            .with_custom_text_color(teal_bright)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("panel_header"));

    // Settings list items
    float list_y = panel_y + 55.0f;
    float item_h = 48.0f;

    for (size_t i = 0; i < main_settings.size(); i++) {
      float item_y = list_y + (float)i * item_h;
      bool is_selected = (i == selected_main);

      // Item background
      afterhours::Color item_bg =
          is_selected ? afterhours::Color{35, 70, 72, 255} : panel_dark;

      div(context, mk(entity, 120 + static_cast<int>(i) * 2),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(static_cast<int>(panel_w - 20)),
                                       pixels(static_cast<int>(item_h - 4))})
              .with_absolute_position()
              .with_translate(panel_x + 10.0f, item_y)
              .with_custom_background(item_bg)
              .with_border(panel_border, 1.0f)
              .with_debug_name("item_bg_" + std::to_string(i)));

      // Highlight bar on selected
      if (is_selected) {
        div(context, mk(entity, 180 + static_cast<int>(i)),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(static_cast<int>(panel_w - 22)),
                                         pixels(4)})
                .with_absolute_position()
                .with_translate(panel_x + 11.0f, item_y + item_h - 8.0f)
                .with_custom_background(teal_highlight)
                .with_debug_name("highlight_" + std::to_string(i)));
      }

      // Item text
      afterhours::Color text_color = is_selected ? text_white : text_muted;

      if (button(context, mk(entity, 121 + static_cast<int>(i) * 2),
                 ComponentConfig{}
                     .with_label(main_settings[i])
                     .with_size(
                         ComponentSize{pixels(static_cast<int>(panel_w - 40)),
                                       pixels(static_cast<int>(item_h - 8))})
                     .with_absolute_position()
                     .with_translate(panel_x + 25.0f, item_y + 8.0f)
                     .with_font("EqProRounded", 19.0f)
                     .with_custom_text_color(text_color)
                     .with_debug_name("setting_" + std::to_string(i)))) {
        selected_main = i;
      }
    }

    // ========== BOTTOM BUTTON PROMPTS ==========
    float prompt_y = panel_y + panel_h + 20.0f;
    float prompt_x = panel_x + 150.0f;

    // Enter prompt (smaller, just for SELECT)
    div(context, mk(entity, 200),
        ComponentConfig{}
            .with_label("SELECT")
            .with_size(ComponentSize{pixels(70), pixels(22)})
            .with_absolute_position()
            .with_translate(prompt_x + 105.0f, prompt_y + 3.0f)
            .with_font("EqProRounded", 19.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("select_small"));

    // Main prompt bar
    div(context, mk(entity, 210),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(280), pixels(35)})
            .with_absolute_position()
            .with_translate(prompt_x, prompt_y + 30.0f)
            .with_custom_background(panel_dark)
            .with_border(panel_border, 1.0f)
            .with_debug_name("prompt_bar"));

    // Enter key
    div(context, mk(entity, 211),
        ComponentConfig{}
            .with_label("[<-")
            .with_size(ComponentSize{pixels(35), pixels(25)})
            .with_absolute_position()
            .with_translate(prompt_x + 15.0f, prompt_y + 35.0f)
            .with_custom_background(afterhours::Color{55, 75, 80, 255})
            .with_font("EqProRounded", 19.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("enter_key"));

    div(context, mk(entity, 212),
        ComponentConfig{}
            .with_label("SELECT")
            .with_size(ComponentSize{pixels(60), pixels(25)})
            .with_absolute_position()
            .with_translate(prompt_x + 55.0f, prompt_y + 35.0f)
            .with_font("EqProRounded", 19.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("select_label"));

    // Esc key
    div(context, mk(entity, 213),
        ComponentConfig{}
            .with_label("Esc")
            .with_size(ComponentSize{pixels(35), pixels(25)})
            .with_absolute_position()
            .with_translate(prompt_x + 135.0f, prompt_y + 35.0f)
            .with_custom_background(afterhours::Color{55, 75, 80, 255})
            .with_font("EqProRounded", 19.0f)
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("esc_key"));

    div(context, mk(entity, 214),
        ComponentConfig{}
            .with_label("BACK")
            .with_size(ComponentSize{pixels(50), pixels(25)})
            .with_absolute_position()
            .with_translate(prompt_x + 175.0f, prompt_y + 35.0f)
            .with_font("EqProRounded", 19.0f)
            .with_custom_text_color(text_white)
            .with_debug_name("back_label"));

    // ========== DECORATIVE TECH ELEMENTS ==========
    // Corner brackets on main panel
    int bracket_size = 15;
    afterhours::Color bracket_color = teal_highlight;

    // Top-left bracket
    div(context, mk(entity, 300),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(bracket_size), pixels(2)})
            .with_absolute_position()
            .with_translate(panel_x - 2.0f, panel_y - 2.0f)
            .with_custom_background(bracket_color)
            .with_debug_name("bracket_tl_h"));
    div(context, mk(entity, 301),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(2), pixels(bracket_size)})
            .with_absolute_position()
            .with_translate(panel_x - 2.0f, panel_y - 2.0f)
            .with_custom_background(bracket_color)
            .with_debug_name("bracket_tl_v"));

    // Top-right bracket
    div(context, mk(entity, 302),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(bracket_size), pixels(2)})
            .with_absolute_position()
            .with_translate(panel_x + panel_w - bracket_size + 2.0f,
                            panel_y - 2.0f)
            .with_custom_background(bracket_color)
            .with_debug_name("bracket_tr_h"));
    div(context, mk(entity, 303),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(2), pixels(bracket_size)})
            .with_absolute_position()
            .with_translate(panel_x + panel_w, panel_y - 2.0f)
            .with_custom_background(bracket_color)
            .with_debug_name("bracket_tr_v"));

    // Bottom-left bracket
    div(context, mk(entity, 304),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(bracket_size), pixels(2)})
            .with_absolute_position()
            .with_translate(panel_x - 2.0f, panel_y + panel_h)
            .with_custom_background(bracket_color)
            .with_debug_name("bracket_bl_h"));
    div(context, mk(entity, 305),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(2), pixels(bracket_size)})
            .with_absolute_position()
            .with_translate(panel_x - 2.0f,
                            panel_y + panel_h - bracket_size + 2.0f)
            .with_custom_background(bracket_color)
            .with_debug_name("bracket_bl_v"));

    // Bottom-right bracket
    div(context, mk(entity, 306),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(bracket_size), pixels(2)})
            .with_absolute_position()
            .with_translate(panel_x + panel_w - bracket_size + 2.0f,
                            panel_y + panel_h)
            .with_custom_background(bracket_color)
            .with_debug_name("bracket_br_h"));
    div(context, mk(entity, 307),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(2), pixels(bracket_size)})
            .with_absolute_position()
            .with_translate(panel_x + panel_w,
                            panel_y + panel_h - bracket_size + 2.0f)
            .with_custom_background(bracket_color)
            .with_debug_name("bracket_br_v"));
  }
};

REGISTER_EXAMPLE_SCREEN(deadspace_settings, "Game Mockups",
                        "Sci-fi horror settings menu (Dead Space style)",
                        DeadSpaceSettingsScreen)
