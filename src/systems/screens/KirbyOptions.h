#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct KirbyOptionsScreen : ScreenSystem<UIContext<InputAction>> {
  size_t selected_tab = 5;
  size_t active_tab = 0;
  bool show_delete_confirm = false;

  // Colors matching Kirby Air Ride inspiration - bright Nintendo aesthetic
  afterhours::Color bg_cream{245, 240, 230, 255};
  afterhours::Color panel_white{255, 255, 255, 255};
  afterhours::Color tab_purple{105, 75, 165, 255};
  afterhours::Color tab_purple_dark{85, 55, 135, 255};
  afterhours::Color btn_yellow{255, 205, 45, 255};
  afterhours::Color btn_yellow_dark{225, 175, 25, 255};
  afterhours::Color icon_red{215, 75, 75, 255};
  afterhours::Color icon_blue{75, 135, 200, 255};
  afterhours::Color icon_green{95, 175, 95, 255};
  afterhours::Color icon_purple{145, 95, 185, 255};
  afterhours::Color text_dark{45, 45, 55, 255};
  afterhours::Color text_black{0, 0, 0, 255};
  afterhours::Color text_muted{120, 115, 125, 255};
  afterhours::Color border_gray{195, 190, 185, 255};

  std::vector<std::tuple<std::string, afterhours::Color>> tabs = {
      {"WiFi", icon_blue},    {"Control", icon_red},
      {"Home", icon_green},   {"Mail", icon_purple},
      {"Star", btn_yellow},   {"Gear", tab_purple},
  };

  std::vector<std::tuple<std::string, std::string, afterhours::Color>> options =
      {
          {"Controls", "Controls", icon_green},
          {"Display", "Display", icon_blue},
          {"Accessibility", "Accessibility", icon_purple},
          {"Sound", "Sound", icon_blue},
          {"Network", "Network", icon_green},
      };

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    UIStylingDefaults::get().set_default_font("Gaegu-Bold", h720(20.0f));
    Theme theme;
    theme.font = text_dark;
    theme.darkfont = panel_white;
    theme.font_muted = text_muted;
    theme.background = bg_cream;
    theme.surface = panel_white;
    theme.primary = tab_purple;
    theme.secondary = icon_blue;
    theme.accent = btn_yellow;
    theme.error = icon_red;
    theme.roundness = 0.15f;
    theme.segments = 12;
    context.theme = theme;

    // ═══════════════════════════════════════════════════════════════
    // ROOT
    // ═══════════════════════════════════════════════════════════════
    auto root = vstack(
        context, mk(entity),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(1.0f), screen_pct(1.0f)})
            .with_custom_background(bg_cream)
            .with_padding(Padding{.top = h720(25), .left = w1280(65),
                                  .bottom = h720(20), .right = w1280(50)})
            .with_no_wrap()
            .with_debug_name("kirby_root"));

    // ── Top Tab Bar: L + tab icons + R ──
    auto tab_bar = hstack(
        context, mk(root.ent()),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), h720(70)})
            .with_align_items(AlignItems::Center)
            .with_no_wrap()
            .with_margin(Margin{.left = w1280(135)})
            .with_debug_name("tab_bar"));

    // L bumper
    div(context, mk(tab_bar.ent()),
        ComponentConfig{}
            .with_label("L")
            .with_size(ComponentSize{w1280(36), h720(36)})
            .with_custom_background(tab_purple)
            .with_border(tab_purple_dark, 2.0f)
            .with_custom_text_color(panel_white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.35f)
            .with_soft_shadow(1.0f, 2.0f, 4.0f,
                              afterhours::Color{0, 0, 0, 30})
            .with_margin(Margin{.right = w1280(10)}));

    // Tab icons
    for (size_t i = 0; i < tabs.size(); i++) {
      bool is_selected = (i == selected_tab);
      auto &[icon, color] = tabs[i];
      afterhours::Color tab_bg = is_selected ? tab_purple : color;
      float scale = is_selected ? 1.15f : 1.0f;
      int sz = static_cast<int>(58.0f * scale);

      if (button(context, mk(tab_bar.ent(), 1 + static_cast<int>(i)),
                 ComponentConfig{}
                     .with_label(icon)
                     .with_size(ComponentSize{w1280(static_cast<float>(sz)),
                                              h720(static_cast<float>(sz))})
                     .with_custom_background(tab_bg)
                     .with_border(is_selected ? tab_purple_dark
                                              : afterhours::Color{0, 0, 0, 0},
                                  is_selected ? 4.0f : 0.0f)
                     .with_font("Gaegu-Bold", h720(18.0f * scale))
                     .with_custom_text_color(text_black)
                     .with_alignment(TextAlignment::Center)
                     .with_rounded_corners(RoundedCorners())
                     .with_roundness(0.25f)
                     .with_soft_shadow(2.0f, 3.0f, 8.0f,
                                       afterhours::Color{0, 0, 0, 40})
                     .with_margin(Margin{.left = w1280(5),
                                         .right = w1280(5)})
                     .with_debug_name("tab_" + std::to_string(i)))) {
        selected_tab = i;
      }
    }

    // R bumper
    div(context, mk(tab_bar.ent()),
        ComponentConfig{}
            .with_label("R")
            .with_size(ComponentSize{w1280(36), h720(36)})
            .with_custom_background(tab_purple)
            .with_border(tab_purple_dark, 2.0f)
            .with_custom_text_color(panel_white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.35f)
            .with_soft_shadow(1.0f, 2.0f, 4.0f,
                              afterhours::Color{0, 0, 0, 30})
            .with_margin(Margin{.left = w1280(10)}));

    // ── "Options" label ──
    div(context, mk(root.ent()),
        ComponentConfig{}
            .with_label("Options")
            .with_size(ComponentSize{w1280(100), h720(28)})
            .with_custom_background(tab_purple)
            .with_font("Gaegu-Bold", h720(19.0f))
            .with_custom_text_color(panel_white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b0011))
            .with_roundness(0.4f)
            .with_margin(Margin{.left = w1280(530)}));

    // ── Main content area: tools sidebar + panel ──
    auto body = hstack(
        context, mk(root.ent()),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), h720(430)})
            .with_align_items(AlignItems::FlexStart)
            .with_no_wrap()
            .with_margin(Margin{.top = h720(5)})
            .with_debug_name("body"));

    // Tool icons sidebar
    auto tools = vstack(
        context, mk(body.ent()),
        ComponentConfig{}
            .with_size(ComponentSize{w1280(55), h720(140)})
            .with_no_wrap()
            .with_margin(Margin{.top = h720(30)})
            .with_debug_name("tools"));

    std::vector<std::tuple<std::string, afterhours::Color>> tool_icons = {
        {"Edit", text_black},
        {"Erase", text_dark},
    };
    for (size_t i = 0; i < tool_icons.size(); i++) {
      auto &[icon, color] = tool_icons[i];
      div(context, mk(tools.ent(), static_cast<int>(i)),
          ComponentConfig{}
              .with_label(icon)
              .with_size(ComponentSize{w1280(52), h720(52)})
              .with_custom_background(panel_white)
              .with_border(border_gray, 2.0f)
              .with_font("Gaegu-Bold", h720(18.0f))
              .with_custom_text_color(color)
              .with_alignment(TextAlignment::Center)
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.2f)
              .with_margin(i > 0 ? Margin{.top = h720(8)} : Margin{})
              .with_debug_name("tool_" + std::to_string(i)));
    }

    // Main panel
    auto panel = vstack(
        context, mk(body.ent()),
        ComponentConfig{}
            .with_size(ComponentSize{expand(), percent(1.0f)})
            .with_custom_background(panel_white)
            .with_border(border_gray, 3.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.08f)
            .with_soft_shadow(3.0f, 5.0f, 15.0f,
                              afterhours::Color{0, 0, 0, 35})
            .with_padding(Padding{.top = h720(20), .left = w1280(25),
                                  .bottom = h720(15), .right = w1280(25)})
            .with_no_wrap()
            .with_margin(Margin{.left = w1280(10)})
            .with_debug_name("main_panel"));

    // Name button row
    auto name_row = hstack(
        context, mk(panel.ent()),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), h720(58)})
            .with_align_items(AlignItems::Center)
            .with_no_wrap()
            .with_margin(Margin{.left = w1280(155)})
            .with_debug_name("name_row"));

    button(context, mk(name_row.ent()),
           ComponentConfig{}
               .with_size(ComponentSize{w1280(260), h720(58)})
               .with_custom_background(btn_yellow)
               .with_border(btn_yellow_dark, 4.0f)
               .with_rounded_corners(RoundedCorners())
               .with_roundness(0.5f)
               .with_soft_shadow(2.0f, 3.0f, 8.0f,
                                 afterhours::Color{0, 0, 0, 40})
               .with_debug_name("name_btn"));

    // Avatar + Name overlaid on button using translate
    div(context, mk(name_row.ent()),
        ComponentConfig{}
            .with_label("@")
            .with_size(ComponentSize{w1280(42), h720(42)})
            .with_custom_background(icon_blue)
            .with_font("Gaegu-Bold", h720(24.0f))
            .with_custom_text_color(text_black)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(1.0f)
            .with_translate(w1280(-248), Size{}));

    div(context, mk(name_row.ent()),
        ComponentConfig{}
            .with_label("Name")
            .with_size(ComponentSize{w1280(120), h720(40)})
            .with_font("Gaegu-Bold", h720(28.0f))
            .with_custom_text_color(text_dark)
            .with_translate(w1280(-240), Size{}));

    // "Common" label
    div(context, mk(panel.ent()),
        ComponentConfig{}
            .with_label("Common")
            .with_size(ComponentSize{w1280(100), h720(28)})
            .with_custom_text_color(text_dark)
            .with_margin(Margin{.top = h720(15)}));

    // Options tabs
    std::vector<std::string> option_labels;
    for (auto &[icon, label, color] : options) {
      option_labels.push_back(label);
    }

    auto opts_tabs = hstack(
        context, mk(panel.ent()),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), h720(36)})
            .with_no_wrap()
            .with_margin(Margin{.top = h720(8)})
            .with_debug_name("opts_tabs"));

    for (size_t ti = 0; ti < option_labels.size(); ti++) {
      bool is_active = (ti == active_tab);
      afterhours::Color opt_tab_bg =
          is_active ? tab_purple : afterhours::Color{230, 226, 220, 255};
      afterhours::Color opt_tab_text = is_active ? panel_white : text_dark;

      if (button(context, mk(opts_tabs.ent(), static_cast<int>(ti)),
                 ComponentConfig{}
                     .with_label(option_labels[ti])
                     .with_size(ComponentSize{expand(1.0f), h720(36)})
                     .with_custom_background(opt_tab_bg)
                     .with_custom_text_color(opt_tab_text)
                     .with_font("Gaegu-Bold", h720(16.0f))
                     .with_alignment(TextAlignment::Center)
                     .with_border(is_active ? tab_purple_dark : border_gray,
                                  is_active ? 2.0f : 1.0f)
                     .with_rounded_corners(std::bitset<4>(0b1100))
                     .with_roundness(0.2f)
                     .with_debug_name("opt_tab_" + std::to_string(ti)))) {
        active_tab = ti;
      }
    }

    // Tab content
    auto &[tab_icon, tab_label, tab_color] = options[active_tab];
    std::string tab_desc = tab_label + " options will be displayed here.";

    div(context, mk(panel.ent()),
        ComponentConfig{}
            .with_label(tab_label)
            .with_size(ComponentSize{percent(1.0f), h720(40)})
            .with_font("Gaegu-Bold", h720(28.0f))
            .with_custom_text_color(tab_color)
            .with_margin(Margin{.top = h720(15), .left = w1280(15)}));

    div(context, mk(panel.ent()),
        ComponentConfig{}
            .with_label(tab_desc)
            .with_size(ComponentSize{percent(1.0f), h720(30)})
            .with_font("Gaegu-Bold", h720(20.0f))
            .with_custom_text_color(text_muted)
            .with_margin(Margin{.top = h720(10), .left = w1280(15)}));

    // Spacer
    div(context, mk(panel.ent()),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), expand()})
            .with_skip_tabbing(true));

    // Bottom row: delete/data + description
    auto panel_bottom = hstack(
        context, mk(panel.ent()),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), h720(50)})
            .with_align_items(AlignItems::Center)
            .with_no_wrap()
            .with_debug_name("panel_bottom"));

    // Description
    div(context, mk(panel_bottom.ent()),
        ComponentConfig{}
            .with_label("Enter a new name and customize your controls.")
            .with_size(ComponentSize{expand(), h720(46)})
            .with_custom_background(afterhours::Color{240, 235, 230, 255})
            .with_border(border_gray, 1.0f)
            .with_font("Gaegu-Bold", h720(26.0f))
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.3f));

    // Data button
    if (!show_delete_confirm) {
      if (button(context, mk(panel_bottom.ent()),
                 ComponentConfig{}
                     .with_label("Data...")
                     .with_size(ComponentSize{w1280(80), h720(24)})
                     .with_custom_background(
                         afterhours::Color{230, 228, 225, 255})
                     .with_border(afterhours::Color{210, 208, 205, 255}, 1.0f)
                     .with_font("Gaegu-Bold", h720(14.0f))
                     .with_custom_text_color(text_muted)
                     .with_rounded_corners(RoundedCorners())
                     .with_roundness(0.25f)
                     .with_margin(Margin{.left = w1280(10)})
                     .with_debug_name("data_menu"))) {
        show_delete_confirm = true;
      }
    } else {
      if (button(context, mk(panel_bottom.ent()),
                 ComponentConfig{}
                     .with_label("Cancel")
                     .with_size(ComponentSize{w1280(70), h720(24)})
                     .with_custom_background(
                         afterhours::Color{210, 208, 205, 255})
                     .with_border(border_gray, 1.0f)
                     .with_font("Gaegu-Bold", h720(14.0f))
                     .with_custom_text_color(text_dark)
                     .with_rounded_corners(RoundedCorners())
                     .with_roundness(0.25f)
                     .with_margin(Margin{.left = w1280(10)})
                     .with_debug_name("cancel_delete"))) {
        show_delete_confirm = false;
      }
      button(context, mk(panel_bottom.ent()),
             ComponentConfig{}
                 .with_label("Delete")
                 .with_size(ComponentSize{w1280(70), h720(24)})
                 .with_custom_background(
                     afterhours::Color{180, 120, 110, 255})
                 .with_font("Gaegu-Bold", h720(14.0f))
                 .with_custom_text_color(panel_white)
                 .with_rounded_corners(RoundedCorners())
                 .with_roundness(0.25f)
                 .with_margin(Margin{.left = w1280(5)}));
    }
  }
};

REGISTER_EXAMPLE_SCREEN(kirby_options, "Game Mockups",
                        "Colorful Nintendo options menu (Kirby style)",
                        KirbyOptionsScreen)
