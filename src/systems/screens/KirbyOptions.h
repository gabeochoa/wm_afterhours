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
      {"WiFi", icon_blue},   {"Control", icon_red}, {"Home", icon_green},
      {"Mail", icon_purple}, {"Star", btn_yellow},  {"Gear", tab_purple},
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
    UIStylingDefaults::get().set_default_font("Gaegu-Bold", pixels(20.0f));
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
    context.scaling_mode = ScalingMode::Adaptive;

    // ═══════════════════════════════════════════════════════════════
    // ROOT
    // ═══════════════════════════════════════════════════════════════
    auto root =
        vstack(context, mk(entity),
               ComponentConfig{}
                   .with_size(ComponentSize{screen_pct(1.0f), screen_pct(1.0f)})
                   .with_custom_background(bg_cream)
                   .with_padding(Padding{.top = pixels(25),
                                         .left = pixels(65),
                                         .bottom = pixels(20),
                                         .right = pixels(50)})
                   .with_no_wrap()
                   .with_debug_name("kirby_root"));

    // ── Top Tab Bar: L + tab icons + R ──
    auto tab_bar =
        hstack(context, mk(root.ent()),
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(1030), pixels(70)})
                   .with_align_items(AlignItems::Center)
                   .with_no_wrap()
                   .with_margin(Margin{.left = pixels(135)})
                   .with_debug_name("tab_bar"));

    // L bumper
    div(context, mk(tab_bar.ent()),
        ComponentConfig{}
            .with_label("L")
            .with_size(ComponentSize{pixels(36), pixels(36)})
            .with_custom_background(tab_purple)
            .with_border(tab_purple_dark, 2.0f)
            .with_custom_text_color(panel_white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.35f)
            .with_soft_shadow(1.0f, 2.0f, 4.0f, afterhours::Color{0, 0, 0, 30})
            .with_margin(Margin{.right = pixels(10)}));

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
                     .with_size(ComponentSize{pixels(static_cast<float>(sz)),
                                              pixels(static_cast<float>(sz))})
                     .with_custom_background(tab_bg)
                     .with_border(is_selected ? tab_purple_dark
                                              : afterhours::Color{0, 0, 0, 0},
                                  is_selected ? 4.0f : 0.0f)
                     .with_font("Gaegu-Bold", pixels(18.0f * scale))
                     .with_custom_text_color(text_black)
                     .with_alignment(TextAlignment::Center)
                     .with_text_overflow(TextOverflow::Ellipsis)
                     .with_rounded_corners(RoundedCorners())
                     .with_roundness(0.25f)
                     .with_soft_shadow(2.0f, 3.0f, 8.0f,
                                       afterhours::Color{0, 0, 0, 40})
                     .with_margin(Margin{.left = pixels(5), .right = pixels(5)})
                     .with_debug_name("tab_" + std::to_string(i)))) {
        selected_tab = i;
      }
    }

    // R bumper
    div(context, mk(tab_bar.ent()),
        ComponentConfig{}
            .with_label("R")
            .with_size(ComponentSize{pixels(36), pixels(36)})
            .with_custom_background(tab_purple)
            .with_border(tab_purple_dark, 2.0f)
            .with_custom_text_color(panel_white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.35f)
            .with_soft_shadow(1.0f, 2.0f, 4.0f, afterhours::Color{0, 0, 0, 30})
            .with_margin(Margin{.left = pixels(10)}));

    // ── "Options" label ──
    div(context, mk(root.ent()),
        ComponentConfig{}
            .with_label("Options")
            .with_size(ComponentSize{pixels(100), pixels(28)})
            .with_custom_background(tab_purple)
            .with_font("Gaegu-Bold", pixels(19.0f))
            .with_custom_text_color(panel_white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(std::bitset<4>(0b0011))
            .with_roundness(0.4f)
            .with_translate(pixels(530), pixels(0)));

    // ── Main content area: tools sidebar + panel ──
    auto body = hstack(context, mk(root.ent()),
                       ComponentConfig{}
                           .with_size(ComponentSize{percent(1.0f), pixels(540)})
                           .with_align_items(AlignItems::FlexStart)
                           .with_no_wrap()
                           .with_margin(Margin{.top = pixels(5)})
                           .with_debug_name("body"));

    // Tool icons sidebar
    auto tools = vstack(context, mk(body.ent()),
                        ComponentConfig{}
                            .with_size(ComponentSize{pixels(55), pixels(140)})
                            .with_no_wrap()
                            .with_margin(Margin{.top = pixels(30),
                                                .right = pixels(14)})
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
              .with_size(ComponentSize{pixels(52), pixels(52)})
              .with_custom_background(panel_white)
              .with_border(border_gray, 2.0f)
              .with_font("Gaegu-Bold", pixels(18.0f))
              .with_custom_text_color(color)
              .with_alignment(TextAlignment::Center)
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.2f)
              .with_margin(i > 0 ? Margin{.top = pixels(8)} : Margin{})
              .with_debug_name("tool_" + std::to_string(i)));
    }

    // Main panel
    auto panel = vstack(
        context, mk(body.ent()),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(1050), percent(1.0f)})
            .with_custom_background(panel_white)
            .with_border(border_gray, 3.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.08f)
            .with_soft_shadow(3.0f, 5.0f, 15.0f, afterhours::Color{0, 0, 0, 35})
            .with_padding(Padding{.top = pixels(20),
                                  .left = pixels(25),
                                  .bottom = pixels(15),
                                  .right = pixels(25)})
            .with_no_wrap()
            .with_debug_name("main_panel"));

    // Name button row
    auto name_row = hstack(context, mk(panel.ent()),
                           ComponentConfig{}
                               .with_size(ComponentSize{children(), pixels(58)})
                               .with_align_items(AlignItems::Center)
                               .with_no_wrap()
                               .with_margin(Margin{.left = pixels(155)})
                               .with_debug_name("name_row"));

    button(
        context, mk(name_row.ent()),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(260), pixels(58)})
            .with_custom_background(btn_yellow)
            .with_border(btn_yellow_dark, 4.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.5f)
            .with_soft_shadow(2.0f, 3.0f, 8.0f, afterhours::Color{0, 0, 0, 40})
            .with_debug_name("name_btn"));

    // Avatar + Name overlaid on button using translate
    div(context, mk(name_row.ent()),
        ComponentConfig{}
            .with_label("@")
            .with_size(ComponentSize{pixels(42), pixels(42)})
            .with_custom_background(icon_blue)
            .with_font("Gaegu-Bold", pixels(24.0f))
            .with_custom_text_color(text_black)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(1.0f)
            .with_translate(pixels(-248), pixels(0)));

    div(context, mk(name_row.ent()),
        ComponentConfig{}
            .with_label("Name")
            .with_size(ComponentSize{pixels(120), pixels(40)})
            .with_font("Gaegu-Bold", pixels(28.0f))
            .with_custom_text_color(text_dark)
            .with_translate(pixels(-240), pixels(0)));

    // "Common" label
    div(context, mk(panel.ent()),
        ComponentConfig{}
            .with_label("Common")
            .with_size(ComponentSize{pixels(100), pixels(28)})
            .with_custom_text_color(text_dark)
            .with_margin(Margin{.top = pixels(15)}));

    // Options tabs
    std::vector<std::string> option_labels;
    for (auto &[icon, label, color] : options) {
      option_labels.push_back(label);
    }

    auto opts_tabs =
        hstack(context, mk(panel.ent()),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.0f), pixels(36)})
                   .with_no_wrap()
                   .with_margin(Margin{.top = pixels(8)})
                   .with_debug_name("opts_tabs"));

    for (size_t ti = 0; ti < option_labels.size(); ti++) {
      bool is_active = (ti == active_tab);
      afterhours::Color opt_tab_bg =
          is_active ? tab_purple : afterhours::Color{230, 226, 220, 255};
      afterhours::Color opt_tab_text = is_active ? panel_white : text_dark;

      if (button(context, mk(opts_tabs.ent(), static_cast<int>(ti)),
                 ComponentConfig{}
                     .with_label(option_labels[ti])
                     .with_size(ComponentSize{expand(1.0f), pixels(36)})
                     .with_custom_background(opt_tab_bg)
                     .with_custom_text_color(opt_tab_text)
                     .with_font("Gaegu-Bold", pixels(16.0f))
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
            .with_size(ComponentSize{percent(1.0f), pixels(40)})
            .with_font("Gaegu-Bold", pixels(28.0f))
            .with_custom_text_color(tab_color)
            .with_margin(Margin{.top = pixels(15)})
            .with_translate(pixels(15), pixels(0)));

    div(context, mk(panel.ent()),
        ComponentConfig{}
            .with_label(tab_desc)
            .with_size(ComponentSize{percent(1.0f), pixels(30)})
            .with_font("Gaegu-Bold", pixels(20.0f))
            .with_custom_text_color(text_muted)
            .with_margin(Margin{.top = pixels(10)})
            .with_translate(pixels(15), pixels(0)));

    // Bottom row: delete/data + description
    auto panel_bottom =
        hstack(context, mk(panel.ent()),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.0f), pixels(50)})
                   .with_align_items(AlignItems::Center)
                   .with_no_wrap()
                   .with_debug_name("panel_bottom"));

    // Description
    div(context, mk(panel_bottom.ent()),
        ComponentConfig{}
            .with_label("Enter a new name and customize your controls.")
            .with_size(ComponentSize{expand(), pixels(46)})
            .with_custom_background(afterhours::Color{240, 235, 230, 255})
            .with_border(border_gray, 1.0f)
            .with_font("Gaegu-Bold", pixels(26.0f))
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.3f));

    // Data button
    if (!show_delete_confirm) {
      if (button(
              context, mk(panel_bottom.ent()),
              ComponentConfig{}
                  .with_label("Data...")
                  .with_size(ComponentSize{pixels(80), pixels(24)})
                  .with_custom_background(afterhours::Color{230, 228, 225, 255})
                  .with_border(afterhours::Color{210, 208, 205, 255}, 1.0f)
                  .with_font("Gaegu-Bold", pixels(14.0f))
                  .with_custom_text_color(text_muted)
                  .with_rounded_corners(RoundedCorners())
                  .with_roundness(0.25f)
                  .with_debug_name("data_menu"))) {
        show_delete_confirm = true;
      }
    } else {
      if (button(
              context, mk(panel_bottom.ent()),
              ComponentConfig{}
                  .with_label("Cancel")
                  .with_size(ComponentSize{pixels(70), pixels(24)})
                  .with_custom_background(afterhours::Color{210, 208, 205, 255})
                  .with_border(border_gray, 1.0f)
                  .with_font("Gaegu-Bold", pixels(14.0f))
                  .with_custom_text_color(text_dark)
                  .with_rounded_corners(RoundedCorners())
                  .with_roundness(0.25f)
                  .with_margin(Margin{.left = pixels(10)})
                  .with_debug_name("cancel_delete"))) {
        show_delete_confirm = false;
      }
      button(context, mk(panel_bottom.ent()),
             ComponentConfig{}
                 .with_label("Delete")
                 .with_size(ComponentSize{pixels(70), pixels(24)})
                 .with_custom_background(afterhours::Color{180, 120, 110, 255})
                 .with_font("Gaegu-Bold", pixels(14.0f))
                 .with_custom_text_color(panel_white)
                 .with_rounded_corners(RoundedCorners())
                 .with_roundness(0.25f)
                 .with_margin(Margin{.left = pixels(5)}));
    }
  }
};

REGISTER_EXAMPLE_SCREEN(kirby_options, "Game Mockups",
                        "Colorful Nintendo options menu (Kirby style)",
                        KirbyOptionsScreen)
