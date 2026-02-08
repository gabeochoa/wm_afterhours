#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct FlightOptionsScreen : ScreenSystem<UIContext<InputAction>> {
  size_t selected_category = 0;
  size_t selected_option = 0;

  // Colors matching Ace Combat inspiration - dark cyber/military aesthetic
  afterhours::Color bg_dark{8, 12, 22, 255};
  afterhours::Color text_cyan{85, 175, 225, 255};
  afterhours::Color text_bright{165, 215, 245, 255};
  afterhours::Color text_muted{110, 135, 165, 255};  // Brightened for WCAG AA contrast
  afterhours::Color highlight_line{65, 140, 195,
                                   255}; // Brightened for better visibility
  afterhours::Color connector_line{110, 180, 225,
                                   255}; // High-visibility tree connector
  afterhours::Color grid_color{15, 25, 40, 120};  // Made more subtle

  std::vector<std::string> categories = {
      "FLIGHT SYSTEM", "CONTROLS", "KEYBOARD", "MOUSE",    "FLIGHT STICK",
      "DISPLAY",       "GRAPHICS", "SOUND",    "LANGUAGE",
  };

  std::vector<std::string> suboptions = {
      "Control Type",
      "High-G Turn Settings",
      "Reduced Collision Damage",
      "Vibration",
  };

  // Context-aware help text for each suboption
  std::vector<std::string> suboption_help = {
      "Choose between standard or expert control schemes.",
      "Configure high-G maneuver sensitivity.",
      "Toggle reduced damage from collisions.",
      "Vibration feedback requires a compatible controller.",
  };

  // Help text for each category (when no suboption is focused)
  std::vector<std::string> category_help = {
      "Configure flight control behavior and aircraft handling.",
      "Adjust control mappings and input settings.",
      "Customize keyboard input bindings.",
      "Configure mouse sensitivity and behavior.",
      "Set up flight stick axes and buttons.",
      "Adjust HUD and display preferences.",
      "Configure graphics quality and performance.",
      "Adjust audio levels and settings.",
      "Select game language and localization.",
  };

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    UIStylingDefaults::get().set_default_font("EqProRounded", h720(18.0f));
    Theme theme;
    // Use pure white/black for auto_text_color to achieve WCAG AA contrast
    theme.font =
        afterhours::Color{255, 255, 255, 255}; // Pure white for dark bgs
    theme.darkfont =
        afterhours::Color{10, 15, 25, 255}; // Near-black for light bgs
    theme.font_muted = text_muted;
    theme.background = bg_dark;
    theme.surface = afterhours::Color{12, 18, 30, 255};
    theme.primary = highlight_line;
    theme.secondary = text_muted;
    theme.accent = text_cyan;
    theme.error = afterhours::Color{180, 50, 50, 255};
    theme.roundness = 0.0f;
    theme.segments = 4;
    context.theme = theme;

    // Note: We only need screen_h for positioning elements relative to screen bottom
    int screen_h = Settings::get().get_screen_height();

    // ========== BACKGROUND ==========
    // Use screen_pct(1.0f) to cover full screen regardless of Settings resolution
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(1.0f), screen_pct(1.0f)})
            .with_custom_background(bg_dark)
            .with_debug_name("bg"));

    // Note: Grid lines removed to reduce visual clutter - clean background only

    // ========== DECORATIVE HUD LINES ==========
    // Position decorative elements adjacent to menu for visual connection
    float menu_x = 185.0f;
    float menu_y = 150.0f;
    float line_origin_x = menu_x - 20.0f;  // Position line close to menu items
    float line_origin_y = menu_y - 15.0f;  // Align with menu start

    // Top angled line connecting to title area
    div(context, mk(entity, 60),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(80), pixels(2)})
            .with_absolute_position(line_origin_x - 80.0f, line_origin_y)
            .with_custom_background(highlight_line)
            .with_debug_name("line_top"));

    // Connecting vertical line running alongside menu categories
    float vertical_line_height = (float)categories.size() * 36.0f + 20.0f;
    div(context, mk(entity, 61),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(2), pixels((int)vertical_line_height)})
            .with_absolute_position(line_origin_x, line_origin_y)
            .with_custom_background(highlight_line)
            .with_debug_name("line_vert"));

    // Glow dot at intersection - positioned at line corner
    div(context, mk(entity, 62),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(6), pixels(6)})
            .with_absolute_position(line_origin_x - 2.0f, line_origin_y - 2.0f)
            .with_custom_background(afterhours::Color{255, 200, 80, 255})
            .with_rounded_corners(RoundedCorners())
            .with_roundness(1.0f)
            .with_debug_name("glow_dot"));

    // Horizontal tick marks aligned with each menu category
    for (size_t i = 0; i < categories.size(); i++) {
      float tick_y = menu_y + (float)i * 36.0f + 10.0f;  // Center tick with menu item
      div(context, mk(entity, 70 + static_cast<int>(i)),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(10), pixels(2)})
              .with_absolute_position(line_origin_x + 2.0f, tick_y)  // Tick extends from line toward menu
              .with_custom_background(i == selected_category ? text_cyan
                                                             : text_muted)
              .with_debug_name("tick_" + std::to_string(i)));
    }

    // ========== TITLE: OPTIONS ==========
    div(context, mk(entity, 100),
        ComponentConfig{}
            .with_label("OPTIONS")
            .with_size(ComponentSize{pixels(200), pixels(50)})
            .with_absolute_position(135.0f, 60.0f)
            .with_font("EqProRounded", h720(36.0f))
            .with_custom_text_color(text_cyan)
            .with_debug_name("title"));

    // ========== MENU CATEGORIES ==========
    // menu_x and menu_y already defined above with decorative elements

    for (size_t i = 0; i < categories.size(); i++) {
      bool selected = (i == selected_category);
      afterhours::Color item_color = selected ? text_bright : text_cyan;

      if (button(context, mk(entity, 110 + static_cast<int>(i)),
                 ComponentConfig{}
                     .with_label(categories[i])
                     .with_size(ComponentSize{pixels(200), pixels(32)})
                     .with_absolute_position(menu_x, menu_y + (float)i * 36.0f)
                     .with_font("EqProRounded", h720(20.0f))
                     .with_custom_text_color(item_color)
                     .with_alignment(TextAlignment::Left)
                     .with_padding(Padding{.left = pixels(8)})
                     .with_debug_name("cat_" + std::to_string(i)))) {
        selected_category = i;
      }

      // Selection indicator bar
      if (selected) {
        div(context, mk(entity, 150 + static_cast<int>(i)),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(4), pixels(26)})
                .with_absolute_position(menu_x - 18.0f,
                                menu_y + (float)i * 36.0f + 3.0f)
                .with_custom_background(text_cyan)
                .with_debug_name("select_bar_" + std::to_string(i)));
      }
    }

    // ========== SUB-OPTIONS (right side) ==========
    float sub_x = 400.0f;
    float sub_y = 250.0f;

    // ========== VISUAL CONNECTION LINE (menu to submenu) ==========
    // Vertical connector spanning all suboptions
    float connector_x = 385.0f;
    float connector_top = sub_y + 10.0f;
    float connector_bottom = sub_y + ((float)suboptions.size() - 1) * 36.0f + 10.0f;
    div(context, mk(entity, 160),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(3), pixels((int)(connector_bottom - connector_top))})
            .with_absolute_position(connector_x, connector_top)
            .with_custom_background(connector_line)
            .with_debug_name("connector_line"));

    // Horizontal branch lines from connector to each suboption
    for (size_t i = 0; i < suboptions.size(); i++) {
      float branch_y = sub_y + (float)i * 36.0f + 10.0f;
      div(context, mk(entity, 170 + static_cast<int>(i)),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(12), pixels(2)})
              .with_absolute_position(connector_x + 3.0f, branch_y)
              .with_custom_background(connector_line)
              .with_debug_name("branch_" + std::to_string(i)));
    }

    // Sub-option header showing which category's settings are displayed
    std::string sub_header = categories[selected_category] + " SETTINGS";
    div(context, mk(entity, 195),
        ComponentConfig{}
            .with_label(sub_header)
            .with_size(ComponentSize{pixels(300), pixels(28)})
            .with_absolute_position(sub_x, sub_y - 36.0f)
            .with_font("EqProRounded", h720(16.0f))
            .with_custom_text_color(text_cyan)
            .with_debug_name("sub_header"));

    // Sub-option items
    bool vibration_unavailable = true;
    afterhours::Color disabled_color{70, 85, 105, 255};  // Dimmer than text_muted

    for (size_t i = 0; i < suboptions.size(); i++) {
      bool is_selected = (i == selected_option);
      bool is_vibration = (i == 3);
      bool is_disabled = is_vibration && vibration_unavailable;
      afterhours::Color opt_color;
      if (is_disabled) {
        opt_color = disabled_color;
      } else {
        opt_color = is_selected ? text_bright : text_muted;
      }

      // Append "(Unavailable)" to disabled items so the state is clear inline
      std::string label = suboptions[i];
      if (is_disabled) {
        label += "  (Unavailable)";
      }

      if (button(context, mk(entity, 200 + static_cast<int>(i)),
                 ComponentConfig{}
                     .with_label(label)
                     .with_size(ComponentSize{pixels(300), pixels(32)})
                     .with_absolute_position(sub_x, sub_y + (float)i * 36.0f)
                     .with_font("EqProRounded", h720(20.0f))
                     .with_custom_text_color(opt_color)
                     .with_disabled(is_disabled)
                     .with_debug_name("opt_" + std::to_string(i)))) {
        if (!is_disabled) {
          selected_option = i;
        }
      }
    }

    // Tooltip for disabled Vibration option
    if (vibration_unavailable) {
      // Small indicator dot to draw attention to the tooltip
      div(context, mk(entity, 249),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(6), pixels(6)})
              .with_absolute_position(sub_x + 8.0f, sub_y + 3 * 36.0f + 28.0f)
              .with_custom_background(afterhours::Color{180, 140, 50, 255})
              .with_rounded_corners(RoundedCorners())
              .with_roundness(1.0f)
              .with_debug_name("vibration_warn_dot"));

      div(context, mk(entity, 250),
          ComponentConfig{}
              .with_label("Requires compatible controller")
              .with_size(ComponentSize{pixels(280), pixels(20)})
              .with_absolute_position(sub_x + 18.0f, sub_y + 3 * 36.0f + 24.0f)
              .with_font("EqProRounded", h720(14.0f))
              .with_custom_text_color(text_muted)
              .with_debug_name("vibration_tooltip"));
    }

    // ========== HELP TEXT ==========
    // Generate context-aware help text based on current selection
    std::string help_text;
    if (selected_category == 0 && selected_option < suboption_help.size()) {
      // When on Flight System category, show suboption-specific help
      help_text = suboption_help[selected_option];
    } else if (selected_category < category_help.size()) {
      // Show category-specific help for other categories
      help_text = category_help[selected_category];
    } else {
      help_text = "Use arrow keys to browse categories and options.";
    }

    div(context, mk(entity, 300),
        ComponentConfig{}
            .with_label(help_text)
            .with_size(ComponentSize{pixels(500), pixels(36)})
            .with_absolute_position(170.0f, (float)screen_h - 180.0f)
            .with_font("EqProRounded", h720(20.0f))
            .with_custom_text_color(text_bright)
            .with_debug_name("help"));

    // ========== BOTTOM BUTTON PROMPTS ==========
    float btn_y = (float)screen_h - 60.0f;

    // Enter key box
    div(context, mk(entity, 400),
        ComponentConfig{}
            .with_label("Enter")
            .with_size(ComponentSize{pixels(56), pixels(36)})
            .with_absolute_position(160.0f, btn_y)
            .with_custom_background(afterhours::Color{35, 50, 70, 255})
            .with_border(text_muted, 1.0f)
            .with_custom_text_color(text_bright)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("enter_key"));

    div(context, mk(entity, 401),
        ComponentConfig{}
            .with_label("OK")
            .with_size(ComponentSize{pixels(36), pixels(36)})
            .with_absolute_position(224.0f, btn_y)
            .with_custom_text_color(text_bright)
            .with_debug_name("ok_label"));

    // Esc key box
    div(context, mk(entity, 402),
        ComponentConfig{}
            .with_label("Esc")
            .with_size(ComponentSize{pixels(48), pixels(36)})
            .with_absolute_position(275.0f, btn_y)
            .with_custom_background(afterhours::Color{35, 50, 70, 255})
            .with_border(text_muted, 1.0f)
            .with_custom_text_color(text_bright)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("esc_key"));

    div(context, mk(entity, 403),
        ComponentConfig{}
            .with_label("BACK")
            .with_size(ComponentSize{pixels(56), pixels(36)})
            .with_absolute_position(331.0f, btn_y)
            .with_custom_text_color(text_bright)
            .with_debug_name("back_label"));
  }
};

REGISTER_EXAMPLE_SCREEN(flight_options, "Game Mockups",
                        "Flight sim options menu (Ace Combat style)",
                        FlightOptionsScreen)
