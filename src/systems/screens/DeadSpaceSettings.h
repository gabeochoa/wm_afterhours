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
  size_t active_tab = 0;       // Settings category tab

  // Colors matching Dead Space inspiration - dark sci-fi horror aesthetic
  afterhours::Color bg_black{8, 8, 10, 255};
  afterhours::Color panel_dark{18, 28, 32, 255};
  afterhours::Color panel_border{45, 65, 72, 255};
  afterhours::Color teal_highlight{65, 145, 145, 255};
  afterhours::Color teal_bright{100, 195, 195, 255};
  afterhours::Color text_white{220, 230, 235, 255};
  afterhours::Color text_muted{185, 200, 205, 255};
  afterhours::Color text_dim{155, 170, 175, 255};

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
    context.scaling_mode = ScalingMode::Adaptive;
    UIStylingDefaults::get().set_default_font("EqProRounded", pixels(16.0f));

    int screen_w = Settings::get().get_screen_width();
    int screen_h = Settings::get().get_screen_height();

    // ═══════════════════════════════════════════════════════════════
    // ROOT - full screen
    // ═══════════════════════════════════════════════════════════════
    auto root = vstack(
        context, mk(entity),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(1.0f), screen_pct(1.0f)})
            .with_custom_background(bg_black)
            .with_no_wrap()
            .with_debug_name("ds_root"));

    // Decorative tech lines (screen-edge decoration, kept absolute)
    for (int i = 0; i < 3; i++) {
      div(context, mk(entity, 5 + i),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(40 + i * 20), pixels(1)})
              .with_absolute_position(30.0f + (float)i * 70.0f,
                                      (float)screen_h - 20.0f)
              .with_custom_background(afterhours::Color{30, 50, 55, 120})
              .with_debug_name("techline_" + std::to_string(i)));
    }

    // ═══════════════════════════════════════════════════════════════
    // MAIN CONTENT AREA (sidebar + panel)
    // ═══════════════════════════════════════════════════════════════
    auto main_area = hstack(
        context, mk(root.ent()),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), expand()})
            .with_no_wrap()
            .with_padding(Padding{.top = pixels(90), .left = pixels(30),
                                  .right = pixels(30), .bottom = pixels(10)})
            .with_align_items(AlignItems::FlexStart)
            .with_debug_name("main_area"));

    // ── LEFT SIDEBAR ──
    auto sidebar = vstack(
        context, mk(main_area.ent()),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(200), percent(1.0f)})
            .with_no_wrap()
            .with_padding(Padding{.top = pixels(10)})
            .with_debug_name("sidebar"));

    // Sidebar header
    div(context, mk(sidebar.ent()),
        ComponentConfig{}
            .with_label("PAUSE MENU")
            .with_size(ComponentSize{percent(1.0f), pixels(32)})
            .with_font_size(pixels(20.0f))
            .with_custom_text_color(text_white)
            .with_padding(Padding{.left = pixels(8)})
            .with_alignment(TextAlignment::Left));

    // Separator
    div(context, mk(sidebar.ent()),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(1)})
            .with_custom_background(panel_border)
            .with_margin(Margin{.top = pixels(3), .bottom = pixels(5),
                                .left = pixels(8), .right = pixels(8)})
            .with_debug_name("sep_sidebar"));

    // Sidebar items
    for (size_t i = 0; i < initial_settings.size(); i++) {
      bool is_selected = (i == selected_initial);
      afterhours::Color item_color = is_selected ? text_white : text_muted;
      afterhours::Color item_bg =
          is_selected ? afterhours::Color{35, 70, 72, 255}
                      : afterhours::Color{0, 0, 0, 0};
      afterhours::Color item_border_color =
          is_selected ? teal_highlight : afterhours::Color{0, 0, 0, 0};

      if (button(context, mk(sidebar.ent(), static_cast<int>(i)),
                 ComponentConfig{}
                     .with_label(initial_settings[i])
                     .with_size(ComponentSize{percent(1.0f), pixels(34)})
                     .with_custom_background(item_bg)
                     .with_border(item_border_color, is_selected ? 1.0f : 0.0f)
                     .with_font_size(pixels(17.0f))
                     .with_custom_text_color(item_color)
                     .with_padding(Padding{.left = pixels(8)})
                     .with_alignment(TextAlignment::Left))) {
        selected_initial = i;
      }
    }

    // ── GAP ──
    div(context, mk(main_area.ent()),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(30), pixels(1)})
            .with_skip_tabbing(true));

    // ── MAIN PANEL ──
    auto panel = vstack(
        context, mk(main_area.ent()),
        ComponentConfig{}
            .with_size(ComponentSize{expand(), percent(1.0f)})
            .with_custom_background(panel_dark)
            .with_border(panel_border, 2.0f)
            .with_no_wrap()
            .with_debug_name("main_panel"));
    panel.decorate(with_brackets(context, teal_highlight, 15.0f, 2.0f));

    // Panel header: // SETTINGS
    div(context, mk(panel.ent()),
        ComponentConfig{}
            .with_label("// SETTINGS")
            .with_size(ComponentSize{percent(1.0f), pixels(44)})
            .with_custom_background(afterhours::Color{35, 55, 60, 255})
            .with_font_size(pixels(22.0f))
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center));

    // Tab row
    auto tab_row = hstack(
        context, mk(panel.ent()),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(32)})
            .with_no_wrap()
            .with_padding(Padding{.left = pixels(10), .right = pixels(10)})
            .with_margin(Margin{.top = pixels(8)})
            .with_debug_name("tab_row"));

    for (size_t ti = 0; ti < main_settings.size(); ti++) {
      bool is_active = (ti == active_tab);
      afterhours::Color tab_bg =
          is_active ? teal_highlight : afterhours::Color{25, 45, 50, 255};
      afterhours::Color tab_text = is_active ? text_white : text_dim;

      if (button(context, mk(tab_row.ent(), static_cast<int>(ti)),
                 ComponentConfig{}
                     .with_label(main_settings[ti])
                     .with_size(ComponentSize{expand(), percent(1.0f)})
                     .with_custom_background(tab_bg)
                     .with_custom_text_color(tab_text)
                     .with_font_size(pixels(13.0f))
                     .with_alignment(TextAlignment::Center)
                     .with_margin(Margin{.right = pixels(2)})
                     .with_debug_name("tab_" + std::to_string(ti)))) {
        active_tab = ti;
      }
    }

    // Tab content
    auto tab_content = vstack(
        context, mk(panel.ent()),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), expand()})
            .with_no_wrap()
            .with_padding(Padding{.top = pixels(16), .left = pixels(25),
                                  .right = pixels(25)})
            .with_debug_name("tab_content"));

    std::string tab_title = main_settings[active_tab];
    std::string tab_desc = tab_title + " options will be displayed here.";

    div(context, mk(tab_content.ent()),
        ComponentConfig{}
            .with_label(tab_title)
            .with_size(ComponentSize{percent(1.0f), pixels(40)})
            .with_font_size(pixels(22.0f))
            .with_custom_text_color(teal_bright));

    div(context, mk(tab_content.ent()),
        ComponentConfig{}
            .with_label(tab_desc)
            .with_size(ComponentSize{percent(1.0f), pixels(30)})
            .with_font_size(pixels(17.0f))
            .with_custom_text_color(text_muted)
            .with_margin(Margin{.top = pixels(5)}));

    // Footer: OK / Cancel / Apply (inside panel, pushed to bottom)
    // Spacer
    div(context, mk(panel.ent()),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), expand()})
            .with_skip_tabbing(true));

    auto footer = hstack(
        context, mk(panel.ent()),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(50)})
            .with_justify_content(JustifyContent::FlexEnd)
            .with_align_items(AlignItems::Center)
            .with_no_wrap()
            .with_padding(Padding{.right = pixels(20)})
            .with_debug_name("footer"));

    button(context, mk(footer.ent()),
           ComponentConfig{}
               .with_label("OK")
               .with_size(ComponentSize{pixels(80), pixels(36)})
               .with_custom_background(teal_highlight)
               .with_border(teal_bright, 1.0f)
               .with_font_size(pixels(18.0f))
               .with_custom_text_color(text_white)
               .with_alignment(TextAlignment::Center)
               .with_debug_name("btn_ok"));

    button(context, mk(footer.ent()),
           ComponentConfig{}
               .with_label("Cancel")
               .with_size(ComponentSize{pixels(80), pixels(36)})
               .with_custom_background(panel_dark)
               .with_border(panel_border, 1.0f)
               .with_font_size(pixels(18.0f))
               .with_custom_text_color(text_white)
               .with_alignment(TextAlignment::Center)
               .with_margin(Margin{.left = pixels(10)})
               .with_debug_name("btn_cancel"));

    button(context, mk(footer.ent()),
           ComponentConfig{}
               .with_label("Apply")
               .with_size(ComponentSize{pixels(80), pixels(36)})
               .with_custom_background(panel_dark)
               .with_border(panel_border, 1.0f)
               .with_font_size(pixels(18.0f))
               .with_custom_text_color(text_white)
               .with_alignment(TextAlignment::Center)
               .with_margin(Margin{.left = pixels(10)})
               .with_debug_name("btn_apply"));

    // ═══════════════════════════════════════════════════════════════
    // BOTTOM PROMPT BAR
    // ═══════════════════════════════════════════════════════════════
    auto prompt_area = hstack(
        context, mk(root.ent()),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(70)})
            .with_justify_content(JustifyContent::Center)
            .with_align_items(AlignItems::FlexStart)
            .with_no_wrap()
            .with_debug_name("prompt_area"));

    auto prompt_col = vstack(
        context, mk(prompt_area.ent()),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(280), percent(1.0f)})
            .with_align_items(AlignItems::Center)
            .with_no_wrap());

    // SELECT label
    div(context, mk(prompt_col.ent()),
        ComponentConfig{}
            .with_label("SELECT")
            .with_size(ComponentSize{pixels(70), pixels(22)})
            .with_font_size(pixels(19.0f))
            .with_custom_text_color(text_white));

    // Prompt bar background
    auto prompt_bar = hstack(
        context, mk(prompt_col.ent()),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(280), pixels(35)})
            .with_custom_background(panel_dark)
            .with_border(panel_border, 1.0f)
            .with_align_items(AlignItems::Center)
            .with_no_wrap()
            .with_padding(Padding{.left = pixels(15)})
            .with_margin(Margin{.top = pixels(5)}));

    // [<- SELECT
    div(context, mk(prompt_bar.ent()),
        ComponentConfig{}
            .with_label("[<-")
            .with_size(ComponentSize{pixels(35), pixels(25)})
            .with_custom_background(afterhours::Color{55, 75, 80, 255})
            .with_font_size(pixels(19.0f))
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center));

    div(context, mk(prompt_bar.ent()),
        ComponentConfig{}
            .with_label("SELECT")
            .with_size(ComponentSize{pixels(60), pixels(25)})
            .with_font_size(pixels(19.0f))
            .with_custom_text_color(text_white)
            .with_margin(Margin{.left = pixels(5)}));

    // Esc BACK
    div(context, mk(prompt_bar.ent()),
        ComponentConfig{}
            .with_label("Esc")
            .with_size(ComponentSize{pixels(35), pixels(25)})
            .with_custom_background(afterhours::Color{55, 75, 80, 255})
            .with_font_size(pixels(19.0f))
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_margin(Margin{.left = pixels(20)}));

    div(context, mk(prompt_bar.ent()),
        ComponentConfig{}
            .with_label("BACK")
            .with_size(ComponentSize{pixels(50), pixels(25)})
            .with_font_size(pixels(19.0f))
            .with_custom_text_color(text_white)
            .with_margin(Margin{.left = pixels(5)}));
  }
};

REGISTER_EXAMPLE_SCREEN(deadspace_settings, "Game Mockups",
                        "Sci-fi horror settings menu (Dead Space style)",
                        DeadSpaceSettingsScreen)
