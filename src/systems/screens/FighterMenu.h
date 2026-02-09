#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>
#include <afterhours/src/plugins/window_manager.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct FighterMenuScreen : ScreenSystem<UIContext<InputAction>> {
  size_t selected_tab = 3;    // Options tab selected
  size_t active_tab = 0;      // Options sub-category tab
  int currency = 25000;

  // ========== LAYOUT CONFIGURATION ==========
  // All spacing and sizing values are configurable here for easy tuning
  struct LayoutConfig {
    // Header area
    float header_height = 145.0f;
    float title_x = 20.0f;
    float title_y = 8.0f;
    float title_font_size = 58.0f;
    float currency_margin_right = 220.0f;
    float currency_y = 22.0f;
    float currency_font_size = 28.0f;

    // Tab bar
    float tab_y = 95.0f;
    float tab_width = 180.0f;
    float tab_height = 44.0f;
    float tab_start_x = 110.0f;
    float tab_font_size = 20.0f;
    float bumper_size = 35.0f;
    float bumper_font_size = 19.0f;
    float tab_underline_height = 4.0f;

    // Left sidebar menu
    float menu_x = 45.0f;
    float menu_y = 180.0f;
    float menu_item_height = 58.0f;
    float menu_icon_width = 56.0f;
    float menu_item_width = 360.0f;
    float menu_icon_font_size = 24.0f;
    float menu_label_font_size = 22.0f;

    // Center area
    float center_panel_y = 200.0f;
    float center_panel_height = 360.0f;
    float character_y_offset = 260.0f;
    float character_font_size = 56.0f;

    // Right info card
    float card_width = 380.0f;
    float card_height = 340.0f;
    float card_margin_right = 50.0f;
    float card_y = 195.0f;
    float card_edge_width = 8.0f;
    float card_icon_size = 150.0f;
    float card_title_font_size = 34.0f;
    float card_desc_font_size = 22.0f;

    // Bottom button prompts
    float prompt_margin_bottom = 60.0f;
    float prompt_start_x = 200.0f;
    float prompt_spacing = 150.0f;
    float prompt_btn_size = 34.0f;
    float prompt_font_size = 20.0f;

    // Shadow configuration
    float shadow_offset_x = 2.0f;
    float shadow_offset_y = 3.0f;
    float shadow_blur = 8.0f;
    int shadow_alpha = 40;
  } layout;

  // Colors matching Cross Tag Battle inspiration - darker, more industrial
  afterhours::Color bg_dark{15, 12, 10, 255};   // Very dark for contrast
  afterhours::Color bg_brown{85, 72, 58, 255};  // Warmer brown background
  afterhours::Color header_black{8, 8, 8, 255}; // Pure black header bar
  afterhours::Color tab_bg_unselected{248, 245, 240, 255}; // Cream white tabs
  afterhours::Color tab_selected{85, 195, 55, 255};        // Bright lime green
  afterhours::Color tab_text_unselected{35, 30, 25, 255};  // Dark text on light
  afterhours::Color tab_border{40, 35, 30, 255};           // Dark border
  afterhours::Color menu_item_bg{10, 10, 10, 255};         // Pure black bars
  afterhours::Color menu_highlight{95, 210, 55, 255};      // Bright lime green
  afterhours::Color text_white{250, 248, 245, 255};
  afterhours::Color text_gray{155, 150, 145, 255};
  afterhours::Color holograph_teal{85, 215, 200, 255}; // Brighter cyan-teal
  afterhours::Color holograph_white{252, 255, 253, 255};
  afterhours::Color gold_text{255, 210, 55, 255}; // Bright gold for P$
  afterhours::Color menu_text_unselected{175, 170, 165,
                                         255}; // Gray text on dark
  afterhours::Color title_yellow{255, 235, 130,
                                 255}; // Yellow for MainMenu title
  afterhours::Color shadow_color{0, 0, 0, 40}; // Soft shadow

  std::vector<std::string> tabs = {"Offline", "Online", "Customize", "Options"};

  // Menu options with icon, label, and description for card
  struct MenuOption {
    std::string icon;
    std::string label;
    std::string description;
  };
  std::vector<MenuOption> menu_options = {
      {"F1", "System Options", "Configure vibration, save data, and autosave settings."},
      {"F2", "Game Options", "Adjust gameplay difficulty and match settings."},
      {"F3", "Display Options", "Modify screen resolution and visual effects."},
      {"F4", "Sound & Language", "Set audio levels and language preferences."},
      {"F5", "Network Options", "Configure online play and connection settings."},
      {"F6", "Button Settings", "Remap controller layout and key bindings."},
  };

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    UIStylingDefaults::get().set_default_font("EqProRounded", h720(18.0f));
    Theme theme;
    theme.font = text_white;
    theme.darkfont = bg_dark;
    theme.font_muted = text_gray;
    theme.background = bg_dark;
    theme.surface = menu_item_bg;
    theme.primary = tab_selected;
    theme.secondary = holograph_teal;
    theme.accent = menu_highlight;
    theme.error = afterhours::Color{200, 60, 60, 255};
    theme.roundness = 0.0f;
    theme.segments = 4;
    context.theme = theme;

    auto *res = afterhours::EntityHelper::get_singleton_cmp<
        afterhours::window_manager::ProvidesCurrentResolution>();
    int screen_w = res ? res->current_resolution.width : 1280;
    int screen_h = res ? res->current_resolution.height : 720;
    auto pxf = [](float v) { return pixels(static_cast<int>(v)); };

    // Use layout config for all positioning
    const auto &L = layout;

    // ========== FULL BACKGROUND (black header + transition) ==========
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_w), pixels(screen_h)})
            .with_custom_background(header_black)
            .with_debug_name("bg_full"));

    // ========== LOWER BROWN AREA (below header bar) ==========
    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_w),
                                     pixels(screen_h - static_cast<int>(L.header_height))})
            .with_absolute_position(0.0f, L.header_height)
            .with_custom_background(bg_brown)
            .with_debug_name("bg_lower"));

    // ========== TITLE: MainMenu (stylized yellow on black) ==========
    div(context, mk(entity, 5),
        ComponentConfig{}
            .with_label("MainMenu")
            .with_size(ComponentSize{pixels(380), pixels(75)})
            .with_absolute_position(L.title_x, L.title_y)
            .with_font("Gaegu-Bold", h720(L.title_font_size))
            .with_custom_text_color(title_yellow));

    // ========== CURRENCY DISPLAY (top right on black header) ==========
    div(context, mk(entity, 10),
        ComponentConfig{}
            .with_label("Points: " + std::to_string(currency))
            .with_size(ComponentSize{pixels(200), pixels(40)})
            .with_absolute_position((float)screen_w - L.currency_margin_right, L.currency_y)
            .with_font("EqProRounded", h720(L.currency_font_size))
            .with_custom_text_color(gold_text)
            .with_alignment(TextAlignment::Right));

    // ========== TAB BAR ==========
    float tab_y = L.tab_y;
    float tab_w = L.tab_width;
    float tab_start_x = L.tab_start_x;

    // L/R bumper indicators
    struct Bumper { const char *label; int id; float x; };
    Bumper bumpers[] = {
        {"L", 20, tab_start_x - 42.0f},
        {"R", 40, tab_start_x + 4 * tab_w + 10.0f},
    };
    for (auto &b : bumpers) {
      div(context, mk(entity, b.id),
          ComponentConfig{}
              .with_label(b.label)
              .with_720p_size(L.bumper_size, L.bumper_size)
              .with_absolute_position(b.x, tab_y + 4.0f)
              .with_custom_background(tab_bg_unselected)
              .with_border(tab_border, 2.0f)
              .with_font("EqProRounded", h720(L.bumper_font_size))
              .with_custom_text_color(tab_text_unselected)
              .with_alignment(TextAlignment::Center)
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.2f)
              .with_soft_shadow(L.shadow_offset_x, L.shadow_offset_y,
                                L.shadow_blur, shadow_color));
    }

    // Tab buttons with shadows on selected tab
    for (size_t i = 0; i < tabs.size(); i++) {
      bool is_selected = (i == selected_tab);
      afterhours::Color bg_color =
          is_selected ? tab_selected : tab_bg_unselected;
      afterhours::Color text_color =
          is_selected ? gold_text : tab_text_unselected;

      auto config = ComponentConfig{}
          .with_label(tabs[i])
          .with_720p_size(tab_w, L.tab_height)
          .with_absolute_position(tab_start_x + (float)i * tab_w, tab_y)
          .with_custom_background(bg_color)
          .with_border(tab_border, 2.0f)
          .with_font("EqProRounded", h720(L.tab_font_size))
          .with_custom_text_color(text_color)
          .with_alignment(TextAlignment::Center)
          .with_debug_name("tab_" + std::to_string(i));

      // Add shadow to selected tab for depth
      if (is_selected) {
        config = config.with_soft_shadow(L.shadow_offset_x, L.shadow_offset_y + 1.0f,
                                         L.shadow_blur + 2.0f,
                                         afterhours::Color{0, 0, 0, 60});
      }

      if (button(context, mk(entity, 30 + static_cast<int>(i)), config)) {
        selected_tab = i;
      }
    }

    // Decorative line under tabs - aligned precisely with tab bar
    div(context, mk(entity, 45),
        ComponentConfig{}
            .with_720p_size(tab_w * 4 + 80, L.tab_underline_height)
            .with_absolute_position(tab_start_x - 42.0f, tab_y + L.tab_height - 1.0f)
            .with_custom_background(tab_selected)
            .with_debug_name("tab_underline"));

    // ========== OPTIONS SUB-CATEGORY TABS ==========
    float menu_x = L.menu_x;
    float menu_y = L.menu_y;
    float menu_item_w = L.menu_item_width;

    std::vector<std::string> option_labels;
    for (const auto &opt : menu_options) {
      option_labels.push_back(opt.label);
    }

    tab_container(context, mk(entity, 100), option_labels, active_tab,
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(menu_item_w + 200)), pixels(44)})
            .with_absolute_position(menu_x, menu_y));

    // ========== CENTER CHARACTER AREA ==========
    float center_area_start = menu_x + menu_item_w + 30.0f;
    float center_area_end = (float)screen_w - L.card_width - L.card_margin_right - 30.0f;
    float center_area_width = center_area_end - center_area_start;
    float character_x = center_area_start + center_area_width / 2.0f - 80.0f;

    // "Offline Mode" indicator - positioned above center panel
    float offline_x = center_area_start + 10.0f;
    div(context, mk(entity, 200),
        ComponentConfig{}
            .with_label("Offline Mode")
            .with_size(ComponentSize{pixels(200), pixels(32)})
            .with_absolute_position(offline_x, 165.0f)
            .with_font("EqProRounded", h720(22.0f))
            .with_custom_text_color(text_white));

    // Background panel with improved styling and shadow
    div(context, mk(entity, 205),
        ComponentConfig{}
            .with_720p_size(center_area_width, L.center_panel_height)
            .with_absolute_position(center_area_start, L.center_panel_y)
            .with_custom_background(afterhours::Color{35, 30, 25, 140})
            .with_border(afterhours::Color{55, 50, 45, 180}, 2.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.08f)
            .with_soft_shadow(L.shadow_offset_x + 1.0f, L.shadow_offset_y + 2.0f,
                              L.shadow_blur + 4.0f, afterhours::Color{0, 0, 0, 35})
            .with_debug_name("center_bg"));

    // Character placeholder with background circle
    div(context, mk(entity, 208),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(140), pixels(140)})
            .with_absolute_position(character_x + 10.0f, L.character_y_offset + 20.0f)
            .with_custom_background(afterhours::Color{50, 45, 40, 150})
            .with_rounded_corners(RoundedCorners())
            .with_roundness(1.0f)
            .with_debug_name("character_bg"));

    div(context, mk(entity, 210),
        ComponentConfig{}
            .with_label("^_^")
            .with_size(ComponentSize{pixels(160), pixels(180)})
            .with_absolute_position(character_x, L.character_y_offset + 10.0f)
            .with_font("EqProRounded", h720(L.character_font_size))
            .with_custom_text_color(text_gray)
            .with_alignment(TextAlignment::Center));

    // Character label
    div(context, mk(entity, 211),
        ComponentConfig{}
            .with_label("Select Character")
            .with_size(ComponentSize{pixels(180), pixels(28)})
            .with_absolute_position(character_x - 10.0f, L.character_y_offset + 180.0f)
            .with_custom_text_color(text_gray)
            .with_alignment(TextAlignment::Center));

    // ========== RIGHT PANEL: Info Card ==========
    float card_w = L.card_width;
    float card_h = L.card_height;
    float card_x = (float)screen_w - card_w - L.card_margin_right;
    float card_y = L.card_y;

    // Get current selected option for dynamic card content
    const auto &current_opt = menu_options[active_tab];

    // Card background with shadow
    div(context, mk(entity, 300),
        ComponentConfig{}
            .with_720p_size(card_w, card_h)
            .with_absolute_position(card_x, card_y)
            .with_custom_background(holograph_white)
            .with_soft_shadow(L.shadow_offset_x + 2.0f, L.shadow_offset_y + 3.0f,
                              L.shadow_blur + 6.0f, afterhours::Color{0, 0, 0, 50})
            .with_debug_name("card_bg"));

    // Teal accent edge
    div(context, mk(entity, 301),
        ComponentConfig{}
            .with_720p_size(L.card_edge_width, card_h)
            .with_absolute_position(card_x + card_w - L.card_edge_width, card_y)
            .with_custom_background(holograph_teal)
            .with_debug_name("card_edge"));

    // Top accent line for visual polish
    div(context, mk(entity, 302),
        ComponentConfig{}
            .with_size(ComponentSize{pxf(card_w - L.card_edge_width),
                                     pixels(4)})
            .with_absolute_position(card_x, card_y)
            .with_custom_background(holograph_teal)
            .with_debug_name("card_top_edge"));

    // Large icon with improved styling
    div(context, mk(entity, 310),
        ComponentConfig{}
            .with_label("@")
            .with_720p_size(L.card_icon_size, L.card_icon_size)
            .with_absolute_position(card_x + 45.0f, card_y + 30.0f)
            .with_custom_background(afterhours::Color{55, 175, 165, 200})
            .with_border(afterhours::Color{45, 155, 145, 255}, 3.0f)
            .with_font("EqProRounded", 90.0f)
            .with_custom_text_color(bg_dark)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.25f)
            .with_soft_shadow(L.shadow_offset_x, L.shadow_offset_y,
                              L.shadow_blur, afterhours::Color{0, 0, 0, 30}));

    // Card title - dynamically updated based on selection
    div(context, mk(entity, 320),
        ComponentConfig{}
            .with_label(current_opt.label)
            .with_size(ComponentSize{pixels(300), pixels(48)})
            .with_absolute_position(card_x + 35.0f, card_y + 195.0f)
            .with_font("Gaegu-Bold", h720(L.card_title_font_size))
            .with_custom_text_color(bg_dark));

    // Description background panel for visual grouping
    div(context, mk(entity, 325),
        ComponentConfig{}
            .with_size(ComponentSize{pxf(card_w - 60),
                                     pixels(100)})
            .with_absolute_position(card_x + 25.0f, card_y + 235.0f)
            .with_custom_background(afterhours::Color{235, 245, 243, 255})
            .with_border(afterhours::Color{180, 210, 205, 255}, 1.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.1f)
            .with_debug_name("card_desc_bg"));

    // Card description - dynamically updated based on selection
    div(context, mk(entity, 326),
        ComponentConfig{}
            .with_label(current_opt.description)
            .with_size(ComponentSize{pxf(card_w - 80),
                                     pixels(80)})
            .with_absolute_position(card_x + 35.0f, card_y + 250.0f)
            .with_font("EqProRounded", h720(20.0f))
            .with_custom_text_color(afterhours::Color{50, 50, 55, 255}));

    // ========== BOTTOM BUTTON PROMPTS ==========
    float prompt_y = (float)screen_h - L.prompt_margin_bottom;
    float hint_spacing = L.prompt_spacing;
    float prompt_x = L.prompt_start_x;
    float btn_size = L.prompt_btn_size;

    // Container background for button prompts
    div(context, mk(entity, 399),
        ComponentConfig{}
            .with_size(ComponentSize{pxf(hint_spacing * 4 + 100),
                                     pixels(48)})
            .with_absolute_position(prompt_x - 10.0f, prompt_y - 6.0f)
            .with_custom_background(afterhours::Color{20, 18, 15, 180})
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.15f)
            .with_debug_name("prompt_bg"));

    // Button prompts - data-driven
    struct Prompt {
      const char *btn; const char *action; int id;
      afterhours::Color bg; float roundness; int label_w;
    };
    Prompt prompts[] = {
        {"+", ": Select", 400, menu_item_bg, 0.15f, 90},
        {"A", ": Confirm", 410, {180, 60, 60, 255}, 1.0f, 100},
        {"B", ": Return", 420, {180, 160, 60, 255}, 1.0f, 90},
    };
    for (size_t pi = 0; pi < 3; pi++) {
      auto &p = prompts[pi];
      float px = prompt_x + (float)pi * hint_spacing;

      auto btn_cfg = ComponentConfig{}
          .with_label(p.btn)
          .with_720p_size(btn_size, btn_size)
          .with_absolute_position(px, prompt_y)
          .with_custom_background(p.bg)
          .with_custom_text_color(text_white)
          .with_alignment(TextAlignment::Center)
          .with_rounded_corners(RoundedCorners())
          .with_roundness(p.roundness);
      if (p.roundness < 0.5f) btn_cfg.with_border(text_gray, 2.0f)
          .with_font("EqProRounded", h720(22.0f));
      div(context, mk(entity, p.id), btn_cfg);

      div(context, mk(entity, p.id + 1),
          ComponentConfig{}
              .with_label(p.action)
              .with_size(ComponentSize{pixels(p.label_w), pixels(36)})
              .with_absolute_position(px + btn_size + 6.0f, prompt_y)
              .with_font("EqProRounded", h720(L.prompt_font_size))
              .with_custom_text_color(text_white));
    }

    // L/R for change entry
    for (int lr = 0; lr < 2; lr++) {
      div(context, mk(entity, 430 + lr),
          ComponentConfig{}
              .with_label(lr == 0 ? "L" : "R")
              .with_720p_size(btn_size - 4, btn_size - 4)
              .with_absolute_position(prompt_x + hint_spacing * 3 + (float)lr * btn_size, prompt_y + 2.0f)
              .with_custom_background(menu_item_bg)
              .with_border(text_gray, 2.0f)
              .with_custom_text_color(text_white)
              .with_alignment(TextAlignment::Center)
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.15f));
    }
    div(context, mk(entity, 432),
        ComponentConfig{}
            .with_label(": Change Entry")
            .with_size(ComponentSize{pixels(130), pixels(36)})
            .with_absolute_position(prompt_x + hint_spacing * 3 + btn_size * 2 + 8.0f, prompt_y)
            .with_font("EqProRounded", h720(L.prompt_font_size))
            .with_custom_text_color(text_white));
  }
};

REGISTER_EXAMPLE_SCREEN(fighter_menu, "Game Mockups",
                        "Anime fighting game menu (Cross Tag style)",
                        FighterMenuScreen)
