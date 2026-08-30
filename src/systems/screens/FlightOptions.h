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
  size_t active_tab = 0;
  size_t selected_option = 0;

  // Colors matching Ace Combat inspiration - dark cyber/military aesthetic
  afterhours::Color bg_dark{8, 12, 22, 255};
  afterhours::Color text_cyan{85, 175, 225, 255};
  afterhours::Color text_bright{165, 215, 245, 255};
  afterhours::Color text_muted{110, 135, 165, 255};
  afterhours::Color highlight_line{65, 140, 195, 255};
  afterhours::Color connector_line{110, 180, 225, 255};
  afterhours::Color grid_color{15, 25, 40, 120};

  std::vector<std::string> categories = {
      "FLIGHT SYS", "CONTROLS", "KEYBOARD", "MOUSE",    "FLT STICK",
      "DISPLAY",    "GRAPHICS", "SOUND",    "LANGUAGE",
  };

  std::vector<std::string> suboptions = {
      "Control Type",
      "High-G Turn Settings",
      "Reduced Collision Damage",
      "Vibration",
  };

  std::vector<std::string> suboption_help = {
      "Choose between standard or expert control schemes.",
      "Configure high-G maneuver sensitivity.",
      "Toggle reduced damage from collisions.",
      "Vibration feedback requires a compatible controller.",
  };

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
    UIStylingDefaults::get().set_default_font("EqProRounded", pixels(18.0f));
    Theme theme;
    theme.font = afterhours::Color{255, 255, 255, 255};
    theme.darkfont = afterhours::Color{10, 15, 25, 255};
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
    context.scaling_mode = ScalingMode::Adaptive;

    // ═══════════════════════════════════════════════════════════════
    // ROOT
    // ═══════════════════════════════════════════════════════════════
    auto root = vstack(
        context, mk(entity),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(1.0f), screen_pct(1.0f)})
            .with_custom_background(bg_dark)
            .with_padding(Padding{
                .top = pixels(20), .left = pixels(135), .right = pixels(40)})
            .with_no_wrap()
            .with_debug_name("flight_root"));

    // ── Title ──
    div(context, mk(root.ent()),
        ComponentConfig{}
            .with_label("OPTIONS")
            .with_size(ComponentSize{pixels(200), pixels(50)})
            .with_font("EqProRounded", pixels(36.0f))
            .with_custom_text_color(text_cyan));

    // ── Tab bar ──
    // 9 categories in one row: smaller font + no-wrap so all labels fit on one
    // row (content-fit tab sizing alone can't help when the summed label width
    // at the default font exceeds the bar).
    tab_container(context, mk(root.ent()), categories, active_tab,
                  ComponentConfig{}
                      // 0.99: grid snapping rounds each of the nine tabs up,
                      // and at full width they summed to 1105 against 1104.
                      .with_size(ComponentSize{percent(0.99f), pixels(32)})
                      // 14, not 15: at 15 the nine content-fit tabs summed
                      // to 1105 against a 1104 bar.
                      .with_font(UIComponent::DEFAULT_FONT, pixels(14.0f))
                      .with_no_wrap()
                      .with_margin(Margin{.top = pixels(-5)}));

    // A decorative rule used to sit here and read as a second, broken underline.

    // ── Content area: connector + sub-options ──
    std::string sub_header = categories[active_tab] + " SETTINGS";

    auto content =
        vstack(context, mk(root.ent()),
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(840), pixels(350)})
                   .with_no_wrap()
                   .with_margin(Margin{.top = pixels(5), .left = pixels(265)})
                   .with_debug_name("content"));

    div(context, mk(content.ent()),
        ComponentConfig{}
            .with_label(sub_header)
            .with_size(ComponentSize{pixels(300), pixels(28)})
            .with_font("EqProRounded", pixels(16.0f))
            .with_custom_text_color(text_cyan));

    bool vibration_unavailable = true;
    afterhours::Color disabled_color{80, 95, 115, 128};

    for (size_t i = 0; i < suboptions.size(); i++) {
      bool is_selected = (i == selected_option);
      bool is_vibration = (i == 3);
      bool is_disabled = is_vibration && vibration_unavailable;
      // Only the selected row is filled. Every row used to take the primary
      // blue while the unselected ones kept text_muted, which was picked
      // against the page background: 1.02:1, text the colour of its own fill.
      afterhours::Color opt_color;
      if (is_disabled) {
        opt_color = disabled_color;
      } else {
        opt_color = is_selected ? theme.darkfont : text_muted;
      }

      std::string label = suboptions[i];
      if (is_disabled) {
        label += "  (Unavailable)";
      }

      if (button(context, mk(content.ent(), 1 + static_cast<int>(i)),
                 ComponentConfig{}
                     .with_label(label)
                     .with_size(ComponentSize{pixels(300), pixels(32)})
                     .with_font("EqProRounded", pixels(20.0f))
                     .with_custom_background(is_selected ? highlight_line
                                                         : bg_dark)
                     .with_auto_text_color(false)
                     .with_custom_text_color(opt_color)
                     .with_disabled(is_disabled)
                     .with_opacity(is_disabled ? 0.5f : 1.0f)
                     .with_margin(Margin{.top = pixels(4)})
                     .with_debug_name("opt_" + std::to_string(i)))) {
        if (!is_disabled) {
          selected_option = i;
        }
      }
    }

    // Vibration tooltip
    if (vibration_unavailable) {
      div(context, mk(content.ent()),
          ComponentConfig{}
              .with_label("Requires compatible controller")
              .with_size(ComponentSize{pixels(280), pixels(20)})
              .with_font("EqProRounded", pixels(14.0f))
              .with_custom_text_color(text_muted)
              .with_margin(Margin{.left = pixels(18)}));
    }

    // ── Help text ──
    std::string help_text;
    if (active_tab == 0 && selected_option < suboption_help.size()) {
      help_text = suboption_help[selected_option];
    } else if (active_tab < category_help.size()) {
      help_text = category_help[active_tab];
    } else {
      help_text = "Use arrow keys to browse categories and options.";
    }

    div(context, mk(root.ent()),
        ComponentConfig{}
            .with_label(help_text)
            .with_size(ComponentSize{pixels(500), pixels(28)})
            .with_font("EqProRounded", pixels(18.0f))
            .with_custom_text_color(text_bright)
            .with_margin(Margin{.left = pixels(35)}));

    auto footer_btns =
        hstack(context, mk(root.ent()),
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(740), pixels(36)})
                   .with_align_items(AlignItems::Center)
                   .with_no_wrap()
                   .with_margin(Margin{.top = pixels(5), .left = pixels(365)})
                   .with_debug_name("footer_btns"));

    button(context, mk(footer_btns.ent()),
           ComponentConfig{}
               .with_label("OK")
               .with_size(ComponentSize{pixels(80), pixels(36)})
               .with_custom_background(highlight_line)
               .with_border(text_muted, 1.0f)
               .with_font("EqProRounded", pixels(18.0f))
               // Dark on the blue fill: text_bright gives 2.37:1 against it.
               .with_custom_text_color(theme.darkfont)
               .with_alignment(TextAlignment::Center)
               .with_debug_name("btn_ok"));

    button(context, mk(footer_btns.ent()),
           ComponentConfig{}
               .with_label("Cancel")
               .with_size(ComponentSize{pixels(80), pixels(36)})
               .with_custom_background(afterhours::Color{35, 50, 70, 255})
               .with_border(text_muted, 1.0f)
               .with_font("EqProRounded", pixels(18.0f))
               .with_custom_text_color(text_bright)
               .with_alignment(TextAlignment::Center)
               .with_margin(Margin{.left = pixels(10)})
               .with_debug_name("btn_cancel"));

    button(context, mk(footer_btns.ent()),
           ComponentConfig{}
               .with_label("Apply")
               .with_size(ComponentSize{pixels(80), pixels(36)})
               .with_custom_background(afterhours::Color{35, 50, 70, 255})
               .with_border(text_muted, 1.0f)
               .with_font("EqProRounded", pixels(18.0f))
               .with_custom_text_color(text_bright)
               .with_alignment(TextAlignment::Center)
               .with_margin(Margin{.left = pixels(10)})
               .with_debug_name("btn_apply"));

    auto prompts =
        hstack(context, mk(root.ent()),
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(1080), pixels(28)})
                   .with_align_items(AlignItems::Center)
                   .with_no_wrap()
                   .with_margin(Margin{.top = pixels(5), .left = pixels(25)})
                   .with_debug_name("prompts"));

    afterhours::Color key_bg{35, 50, 70, 255};
    div(context, mk(prompts.ent()),
        ComponentConfig{}
            .with_label("Enter")
            .with_size(ComponentSize{pixels(56), pixels(28)})
            .with_custom_background(key_bg)
            .with_border(text_muted, 1.0f)
            .with_custom_text_color(text_bright)
            .with_alignment(TextAlignment::Center));
    div(context, mk(prompts.ent()),
        ComponentConfig{}
            .with_label("OK")
            .with_size(ComponentSize{pixels(36), pixels(28)})
            .with_custom_text_color(text_bright)
            .with_margin(Margin{.left = pixels(8)}));
    div(context, mk(prompts.ent()),
        ComponentConfig{}
            .with_label("Esc")
            .with_size(ComponentSize{pixels(48), pixels(28)})
            .with_custom_background(key_bg)
            .with_border(text_muted, 1.0f)
            .with_custom_text_color(text_bright)
            .with_alignment(TextAlignment::Center)
            .with_margin(Margin{.left = pixels(15)}));
    div(context, mk(prompts.ent()),
        ComponentConfig{}
            .with_label("BACK")
            .with_size(ComponentSize{pixels(56), pixels(28)})
            .with_custom_text_color(text_bright)
            .with_margin(Margin{.left = pixels(8)}));
  }
};

REGISTER_EXAMPLE_SCREEN(flight_options, "Game Mockups",
                        "Flight sim options menu (Ace Combat style)",
                        FlightOptionsScreen)
