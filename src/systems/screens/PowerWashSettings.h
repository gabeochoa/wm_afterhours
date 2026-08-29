#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct PowerWashSettingsScreen : ScreenSystem<UIContext<InputAction>> {
  size_t active_tab = 2;   // VIDEO tab selected
  size_t selected_row = 4; // Anti-Aliasing selected

  // Colors matching PowerWash Simulator - dark blue interface
  afterhours::Color bg_dark{25, 45, 75, 255};
  afterhours::Color panel_blue{35, 65, 105, 255};
  afterhours::Color panel_border{55, 95, 145, 255};
  afterhours::Color header_bar{45, 80, 130, 255};
  afterhours::Color text_white{235, 240, 245, 255};
  afterhours::Color text_cyan{145, 215, 245, 255};
  afterhours::Color text_muted{165, 190, 215, 255};
  afterhours::Color dropdown_bg{55, 90, 140, 255};
  afterhours::Color dropdown_border{75, 120, 175, 255};
  afterhours::Color tab_selected{65, 105, 165, 255};
  afterhours::Color highlight_blue{85, 145, 215, 255};
  // Nav labels sit on highlight_blue; text_muted gave 1.71:1 against it.
  afterhours::Color text_on_blue{15, 20, 30, 255};

  std::vector<std::string> tabs = {"GENERAL", "GAMEPLAY", "VIDEO", "AUDIO",
                                   "CONTROLS"};

  struct SettingRow {
    std::string label;
    std::vector<std::string> options;
    size_t option_idx;
  };

  std::vector<SettingRow> video_settings = {
      {"Screen Resolution", {"1920 x 1080", "2560 x 1440", "3840 x 2160"}, 1},
      {"Window Mode", {"Fullscreen", "Borderless Windowed", "Windowed"}, 1},
      {"Target Framerate", {"30", "60", "120", "Unlimited"}, 3},
      {"Vsync", {"Off", "On"}, 1},
      {"Anti-Aliasing", {"Off", "2x", "4x", "8x"}, 1},
      {"SSAO", {"Off", "2x", "4x"}, 1},
      {"Render Scale", {"Off", "75%", "100%", "125%"}, 0},
      {"Model Quality", {"Low", "Medium", "High", "Ultra"}, 2},
      {"Texture Quality", {"Low", "Medium", "High", "Highest"}, 3},
  };

  std::vector<SettingRow> general_settings = {
      {"Language", {"English", "Spanish", "French", "German"}, 0},
      {"Subtitles", {"Off", "On"}, 1},
      {"Colorblind Mode",
       {"Off", "Deuteranopia", "Protanopia", "Tritanopia"},
       0},
  };

  std::vector<SettingRow> gameplay_settings = {
      {"Difficulty", {"Easy", "Normal", "Hard"}, 1},
      {"Camera Sensitivity", {"Low", "Medium", "High"}, 1},
      {"Invert Y Axis", {"Off", "On"}, 0},
  };

  std::vector<SettingRow> audio_settings = {
      {"Master Volume", {"0%", "25%", "50%", "75%", "100%"}, 4},
      {"Music Volume", {"0%", "25%", "50%", "75%", "100%"}, 3},
      {"SFX Volume", {"0%", "25%", "50%", "75%", "100%"}, 4},
      {"Voice Volume", {"0%", "25%", "50%", "75%", "100%"}, 4},
  };

  std::vector<SettingRow> controls_settings = {
      {"Controller Vibration", {"Off", "On"}, 1},
      {"Button Layout", {"Default", "Alternate A", "Alternate B"}, 0},
  };

  std::vector<SettingRow> &get_current_settings() {
    switch (active_tab) {
    case 0:
      return general_settings;
    case 1:
      return gameplay_settings;
    case 2:
      return video_settings;
    case 3:
      return audio_settings;
    case 4:
      return controls_settings;
    default:
      return video_settings;
    }
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    UIStylingDefaults::get().set_default_font("EqProRounded", pixels(16.0f));
    Theme theme;
    theme.font = text_white;
    theme.darkfont = bg_dark;
    theme.font_muted = text_muted;
    theme.background = bg_dark;
    theme.surface = panel_blue;
    theme.primary = highlight_blue;
    theme.secondary = panel_border;
    theme.accent = text_cyan;
    theme.error = afterhours::Color{180, 80, 80, 255};
    theme.roundness = 0.08f;
    theme.segments = 6;
    // Assigning a whole theme drops the app default from preload.
    theme.text_inset = {5.f, 5.f};
    context.theme = theme;
    context.scaling_mode = ScalingMode::Adaptive;

    // ═══════════════════════════════════════════════════════════════
    // ROOT
    // ═══════════════════════════════════════════════════════════════
    auto root =
        vstack(context, mk(entity),
               ComponentConfig{}
                   .with_size(ComponentSize{screen_pct(1.0f), screen_pct(1.0f)})
                   .with_custom_background(bg_dark)
                   .with_padding(Padding{.top = pixels(20),
                                         .left = pixels(55),
                                         .bottom = pixels(15),
                                         .right = pixels(25)})
                   .with_no_wrap()
                   .with_debug_name("pw_root"));

    // ── Header: X close + SETTINGS title ──
    auto header =
        hstack(context, mk(root.ent()),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.0f), pixels(35)})
                   .with_align_items(AlignItems::Center)
                   .with_no_wrap()
                   .with_debug_name("header"));

    div(context, mk(header.ent()),
        ComponentConfig{}
            .with_label("X")
            .with_size(ComponentSize{pixels(30), pixels(30)})
            .with_font("EqProRounded", pixels(22.0f))
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center));

    div(context, mk(header.ent()),
        ComponentConfig{}
            .with_label("SETTINGS")
            .with_size(ComponentSize{pixels(120), pixels(30)})
            .with_font("EqProRounded", pixels(20.0f))
            .with_custom_text_color(text_white)
            .with_margin(Margin{.left = pixels(10)}));

    // ── Content area: main panel + help panel ──
    auto content =
        hstack(context, mk(root.ent()),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.0f), pixels(380)})
                   .with_align_items(AlignItems::FlexStart)
                   .with_no_wrap()
                   .with_margin(Margin{.top = pixels(5)})
                   .with_debug_name("content"));

    // Main settings panel
    auto main_panel = vstack(context, mk(content.ent()),
                             ComponentConfig{}
                                 .with_720p_size(480, 380)
                                 .with_custom_background(panel_blue)
                                 .with_border(panel_border, 2.0f)
                                 .with_padding(Padding{.top = pixels(8),
                                                       .left = pixels(20),
                                                       .bottom = pixels(8),
                                                       .right = pixels(20)})
                                 .with_no_wrap()
                                 .with_debug_name("main_panel"));

    auto &current_settings = get_current_settings();
    if (selected_row >= current_settings.size()) {
      selected_row = 0;
    }

    for (size_t i = 0; i < current_settings.size(); i++) {
      bool is_selected = (i == selected_row);
      afterhours::Color label_color = is_selected ? text_white : text_on_blue;
      afterhours::Color dd_bg = is_selected ? dropdown_bg : panel_blue;
      afterhours::Color dd_border =
          is_selected ? dropdown_border : panel_border;
      afterhours::Color arrow_col = is_selected ? text_white : text_muted;

      auto row =
          hstack(context, mk(main_panel.ent(), static_cast<int>(i)),
                 ComponentConfig{}
                     .with_size(ComponentSize{percent(1.0f), pixels(36)})
                     .with_align_items(AlignItems::Center)
                     .with_no_wrap()
                     .with_margin(Margin{.top = i > 0 ? pixels(0) : Size{}}));

      if (button(context, mk(row.ent(), 0),
                 ComponentConfig{}
                     .with_label(current_settings[i].label)
                     .with_size(ComponentSize{pixels(170), pixels(36)})
                     .with_font("EqProRounded", pixels(16.0f))
                     .with_custom_background(is_selected ? tab_selected
                                                         : highlight_blue)
                     .with_auto_text_color(false)
                     .with_custom_text_color(label_color))) {
        selected_row = i;
      }

      if (button(context, mk(row.ent(), 1),
                 ComponentConfig{}
                     .with_label("<")
                     .with_size(ComponentSize{pixels(44), pixels(36)})
                     .with_custom_background(dd_bg)
                     .with_border(dd_border, 1.0f)
                     .with_font("EqProRounded", pixels(18.0f))
                     .with_custom_text_color(arrow_col)
                     .with_alignment(TextAlignment::Center)
                     // 170 + 10 + 44 + 170 + 44 = 438 in a 440 row. At 20 the
                     // right arrow ran 8px past the edge on all 18 rows.
                     .with_margin(Margin{.left = pixels(10)}))) {
        selected_row = i;
        auto &setting = current_settings[i];
        setting.option_idx = (setting.option_idx == 0)
                                 ? setting.options.size() - 1
                                 : setting.option_idx - 1;
      }

      div(context, mk(row.ent(), 2),
          ComponentConfig{}
              .with_label(
                  current_settings[i].options[current_settings[i].option_idx])
              .with_size(ComponentSize{pixels(170), pixels(36)})
              .with_custom_background(dd_bg)
              .with_custom_text_color(text_white)
              .with_alignment(TextAlignment::Center));

      if (button(context, mk(row.ent(), 3),
                 ComponentConfig{}
                     .with_label(">")
                     .with_size(ComponentSize{pixels(44), pixels(36)})
                     .with_custom_background(dd_bg)
                     .with_border(dd_border, 1.0f)
                     .with_font("EqProRounded", pixels(18.0f))
                     .with_custom_text_color(arrow_col)
                     .with_alignment(TextAlignment::Center))) {
        selected_row = i;
        auto &setting = current_settings[i];
        setting.option_idx = (setting.option_idx + 1) % setting.options.size();
      }
    }

    // Spacer pushes footer to bottom
    div(context, mk(main_panel.ent()),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), expand()})
            .with_skip_tabbing(true));

    // Footer buttons inside panel
    auto footer =
        hstack(context, mk(main_panel.ent()),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.0f), pixels(36)})
                   .with_justify_content(JustifyContent::FlexEnd)
                   .with_align_items(AlignItems::Center)
                   .with_no_wrap()
                   .with_debug_name("footer"));

    button(context, mk(footer.ent()),
           ComponentConfig{}
               .with_label("OK")
               .with_size(ComponentSize{pixels(80), pixels(36)})
               .with_custom_background(panel_border)
               .with_border(text_cyan, 1.0f)
               .with_font("EqProRounded", pixels(18.0f))
               .with_custom_text_color(text_white)
               .with_alignment(TextAlignment::Center)
               .with_debug_name("btn_ok"));

    button(context, mk(footer.ent()),
           ComponentConfig{}
               .with_label("Cancel")
               .with_size(ComponentSize{pixels(80), pixels(36)})
               .with_custom_background(panel_blue)
               .with_border(panel_border, 1.0f)
               .with_font("EqProRounded", pixels(18.0f))
               .with_custom_text_color(text_white)
               .with_alignment(TextAlignment::Center)
               .with_margin(Margin{.left = pixels(10)})
               .with_debug_name("btn_cancel"));

    button(context, mk(footer.ent()),
           ComponentConfig{}
               .with_label("Apply")
               .with_size(ComponentSize{pixels(80), pixels(36)})
               .with_custom_background(panel_blue)
               .with_border(panel_border, 1.0f)
               .with_font("EqProRounded", pixels(18.0f))
               .with_custom_text_color(text_white)
               .with_alignment(TextAlignment::Center)
               .with_margin(Margin{.left = pixels(10)})
               .with_debug_name("btn_apply"));

    // Help panel (right of main)
    auto help_panel = vstack(context, mk(content.ent()),
                             ComponentConfig{}
                                 .with_720p_size(220, 140)
                                 .with_custom_background(panel_blue)
                                 .with_border(panel_border, 2.0f)
                                 .with_padding(Padding{.top = pixels(12),
                                                       .left = pixels(12),
                                                       .bottom = pixels(12),
                                                       .right = pixels(12)})
                                 .with_margin(Margin{.left = pixels(15)})
                                 .with_no_wrap()
                                 .with_debug_name("help_panel"));

    std::string setting_name = current_settings[selected_row].label;
    std::string help_title = "Help: " + setting_name;
    std::string help_line1 = "Adjust the " + setting_name;
    std::string help_line2 = "setting to your preference.";
    std::string current_val =
        "Current: " + current_settings[selected_row]
                          .options[current_settings[selected_row].option_idx];

    div(context, mk(help_panel.ent()),
        ComponentConfig{}
            .with_label(help_title)
            .with_size(ComponentSize{percent(1.0f), pixels(28)})
            .with_font("EqProRounded", pixels(17.0f))
            .with_custom_text_color(text_cyan));

    div(context, mk(help_panel.ent()),
        ComponentConfig{}
            .with_label(help_line1)
            .with_size(ComponentSize{percent(1.0f), pixels(25)})
            .with_font("EqProRounded", pixels(14.0f))
            .with_custom_text_color(text_white)
            .with_text_overflow(TextOverflow::Ellipsis)
            .with_margin(Margin{.top = pixels(5)}));

    div(context, mk(help_panel.ent()),
        ComponentConfig{}
            .with_label(help_line2)
            .with_size(ComponentSize{percent(1.0f), pixels(25)})
            .with_font("EqProRounded", pixels(14.0f))
            .with_custom_text_color(text_white)
            .with_text_overflow(TextOverflow::Ellipsis));

    div(context, mk(help_panel.ent()),
        ComponentConfig{}
            .with_label(current_val)
            .with_size(ComponentSize{percent(1.0f), pixels(25)})
            .with_font("EqProRounded", pixels(14.0f))
            .with_custom_text_color(text_muted)
            .with_margin(Margin{.top = pixels(10)}));

    // ── Tab bar ──
    tab_container(context, mk(root.ent()), tabs, active_tab,
                  ComponentConfig{}
                      .with_size(ComponentSize{percent(1.0f), pixels(44)})
                      .with_margin(Margin{.top = pixels(15)}));

    // ── Bottom button prompts ──
    afterhours::Color prompt_bg{55, 75, 95, 255};
    auto prompts =
        hstack(context, mk(root.ent()),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.0f), pixels(28)})
                   .with_align_items(AlignItems::Center)
                   .with_no_wrap()
                   .with_margin(Margin{.top = pixels(16)})
                   .with_debug_name("prompts"));

    struct Prompt {
      const char *icon;
      const char *label;
    };
    Prompt prompt_data[] = {{"X", "Close"}, {"O", "Reset"}, {"[]", "Select"}};
    for (int pi = 0; pi < 3; pi++) {
      // Spacer between prompt groups
      if (pi > 0) {
        div(context, mk(prompts.ent(), pi * 3),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(20), pixels(1)})
                .with_skip_tabbing(true));
      }
      div(context, mk(prompts.ent(), pi * 3 + 1),
          ComponentConfig{}
              .with_label(prompt_data[pi].icon)
              .with_size(ComponentSize{pixels(28), pixels(28)})
              .with_custom_background(prompt_bg)
              .with_custom_text_color(text_white)
              .with_alignment(TextAlignment::Center));
      div(context, mk(prompts.ent(), pi * 3 + 2),
          ComponentConfig{}
              .with_label(prompt_data[pi].label)
              .with_size(ComponentSize{pixels(55), pixels(25)})
              .with_font("EqProRounded", pixels(16.0f))
              .with_custom_text_color(text_white)
              .with_margin(Margin{.left = pixels(7)}));
    }
  }
};

REGISTER_EXAMPLE_SCREEN(powerwash_settings, "Game Mockups",
                        "Simulation game settings (PowerWash style)",
                        PowerWashSettingsScreen)
