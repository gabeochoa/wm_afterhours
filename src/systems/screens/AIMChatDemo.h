#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/ui/text_input/text_input.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct AIMChatDemo : ScreenSystem<UIContext<InputAction>> {
  // Chat state
  std::string message_input = "";
  std::vector<std::pair<std::string, std::string>> chat_history;
  std::string buddy_name = "SmarterChild";
  std::string my_name = "coolkid2001";

  // AIM color palette
  struct AIMColors {
    static afterhours::Color window_bg() { return {236, 233, 216, 255}; }
    static afterhours::Color title_bar() { return {0, 0, 128, 255}; }
    static afterhours::Color title_text() { return {255, 255, 255, 255}; }
    static afterhours::Color buddy_text() { return {255, 0, 0, 255}; }
    static afterhours::Color my_text() { return {0, 0, 255, 255}; }
    static afterhours::Color chat_bg() { return {255, 255, 255, 255}; }
    static afterhours::Color input_bg() { return {255, 255, 255, 255}; }
    static afterhours::Color button_face() { return {236, 233, 216, 255}; }
    static afterhours::Color button_shadow() { return {128, 128, 128, 255}; }
    static afterhours::Color text_default() { return {0, 0, 0, 255}; }
    static afterhours::Color warning_yellow() { return {255, 255, 0, 255}; }
  };

  AIMChatDemo() {
    chat_history.push_back({buddy_name, "hey whats up"});
    chat_history.push_back({my_name, "nm u?"});
    chat_history.push_back({buddy_name, "just chillin. did u see that new movie?"});
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = context.theme;
    theme.background = AIMColors::window_bg();
    theme.surface = AIMColors::button_face();
    theme.primary = AIMColors::title_bar();
    theme.secondary = AIMColors::input_bg();  // White for text input bg
    theme.font = AIMColors::text_default();
    theme.darkfont = AIMColors::text_default();
    // Set focus ring and accent to a subtle gray instead of bright yellow/green
    theme.accent = AIMColors::button_shadow();
    context.theme = theme;

    // Window dimensions - sized for comfortable chat viewing
    // Note: Children use INNER_W to avoid fractional overflow warnings (W - 3)
    constexpr int W = 500;
    constexpr int H = 600;
    constexpr int INNER_W = 497;  // Account for ~2.2px border rendering
    constexpr int PAD = 6;

    // Main window - sharp corners for Windows 98 aesthetic
    auto window =
        div(context, mk(entity, 0),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(W), pixels(H)})
                .with_custom_background(AIMColors::window_bg())
                .with_flex_direction(FlexDirection::Column)
                .disable_rounded_corners()
                .with_debug_name("aim_window"));

    // Title bar
    auto title_bar =
        div(context, mk(window.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(INNER_W), pixels(28)})
                .with_custom_background(AIMColors::title_bar())
                .with_flex_direction(FlexDirection::Row)
                .with_justify_content(JustifyContent::SpaceBetween)
                .with_align_items(AlignItems::Center)
                .with_debug_name("title_bar"));

    div(context, mk(title_bar.ent(), 0),
        ComponentConfig{}
            .with_label(buddy_name + " - Instant Message")
            .with_size(ComponentSize{pixels(W - 80), pixels(24)})
            .with_custom_text_color(AIMColors::title_text())
            .with_alignment(TextAlignment::Left)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_padding(Padding{.left = pixels(6)})
            .with_skip_tabbing(true)
            .with_debug_name("title_text"));

    // Window controls
    auto controls =
        div(context, mk(title_bar.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(66), pixels(20)})
                .with_flex_direction(FlexDirection::Row)
                .with_align_items(AlignItems::Center)
                .with_debug_name("window_controls"));

    for (int i = 0; i < 3; i++) {
      const char* labels[] = {"_", "□", "×"};
      div(context, mk(controls.ent(), i),
          ComponentConfig{}
              .with_label(labels[i])
              .with_size(ComponentSize{pixels(20), pixels(18)})
              .with_custom_background(AIMColors::button_face())
              .with_custom_text_color(AIMColors::text_default())
              .with_font(UIComponent::DEFAULT_FONT, 14.0f)
              .with_margin(Margin{.left = pixels(2)})
              .disable_rounded_corners()
              .with_skip_tabbing(true)
              .with_debug_name("ctrl_btn"));
    }

    // Menu bar
    auto menu_bar =
        div(context, mk(window.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(INNER_W), pixels(24)})
                .with_custom_background(AIMColors::button_face())
                .with_flex_direction(FlexDirection::Row)
                .with_align_items(AlignItems::Center)
                .with_debug_name("menu_bar"));

    const char* menus[] = {"File", "Edit", "Insert", "People"};
    for (int i = 0; i < 4; i++) {
      div(context, mk(menu_bar.ent(), i),
          ComponentConfig{}
              .with_label(menus[i])
              .with_size(ComponentSize{pixels(55), pixels(22)})
              .with_custom_text_color(AIMColors::text_default())
              .with_font(UIComponent::DEFAULT_FONT, 13.0f)
              .with_padding(Padding{.left = pixels(6)})
              .with_skip_tabbing(true)
              .with_debug_name("menu_item"));
    }

    // Buddy info bar
    auto buddy_bar =
        div(context, mk(window.ent(), 2),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(INNER_W), pixels(50)})
                .with_custom_background(AIMColors::window_bg())
                .with_flex_direction(FlexDirection::Row)
                .with_align_items(AlignItems::Center)
                .with_padding(Padding{.left = pixels(PAD)})
                .with_debug_name("buddy_bar"));

    div(context, mk(buddy_bar.ent(), 0),
        ComponentConfig{}
            .with_label(":-)")
            .with_size(ComponentSize{pixels(40), pixels(40)})
            .with_custom_background(AIMColors::warning_yellow())
            .with_custom_text_color(AIMColors::text_default())
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_skip_tabbing(true)
            .with_debug_name("buddy_icon"));

    auto buddy_info =
        div(context, mk(buddy_bar.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(200), pixels(40)})
                .with_flex_direction(FlexDirection::Column)
                .with_padding(Padding{.left = pixels(10)})
                .with_debug_name("buddy_info"));

    div(context, mk(buddy_info.ent(), 0),
        ComponentConfig{}
            .with_label(buddy_name)
            .with_size(ComponentSize{pixels(180), pixels(20)})
            .with_custom_text_color(AIMColors::buddy_text())
            .with_alignment(TextAlignment::Left)
            .with_font(UIComponent::DEFAULT_FONT, 15.0f)
            .with_skip_tabbing(true)
            .with_debug_name("buddy_name_display"));

    div(context, mk(buddy_info.ent(), 1),
        ComponentConfig{}
            .with_label("Available")
            .with_size(ComponentSize{pixels(100), pixels(18)})
            .with_custom_text_color(AIMColors::text_default())
            .with_alignment(TextAlignment::Left)
            .with_font(UIComponent::DEFAULT_FONT, 13.0f)
            .with_skip_tabbing(true)
            .with_debug_name("buddy_status"));

    // Chat history area - large area for conversation
    auto chat_area =
        div(context, mk(window.ent(), 3),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(INNER_W - PAD * 2), pixels(300)})
                .with_custom_background(AIMColors::chat_bg())
                .with_flex_direction(FlexDirection::Column)
                .with_align_items(AlignItems::FlexStart)
                .with_margin(Margin{.left = pixels(PAD), .right = pixels(PAD)})
                .with_padding(Padding{.left = pixels(4), .top = pixels(2)})
                .disable_rounded_corners()
                .with_debug_name("chat_area"));

    int msg_idx = 0;
    for (const auto& [sender, msg] : chat_history) {
      bool is_me = (sender == my_name);
      std::string formatted = sender + ": " + msg;

      div(context, mk(chat_area.ent(), msg_idx++),
          ComponentConfig{}
              .with_label(formatted)
              .with_size(ComponentSize{pixels(INNER_W - PAD * 4), pixels(22)})
              .with_custom_text_color(is_me ? AIMColors::my_text() : AIMColors::buddy_text())
              .with_alignment(TextAlignment::Left)
              .with_font(UIComponent::DEFAULT_FONT, 14.0f)
              .with_skip_tabbing(true)
              .with_debug_name("chat_msg"));
    }

    // Separator
    div(context, mk(window.ent(), 4),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(INNER_W), pixels(3)})
            .with_custom_background(AIMColors::button_shadow())
            .with_margin(Margin{.top = pixels(4), .bottom = pixels(4)})
            .with_skip_tabbing(true)
            .with_debug_name("separator"));

    // Input label
    div(context, mk(window.ent(), 5),
        ComponentConfig{}
            .with_label("Send to: " + buddy_name)
            .with_size(ComponentSize{pixels(INNER_W), pixels(20)})
            .with_custom_text_color(AIMColors::text_default())
            .with_alignment(TextAlignment::Left)
            .with_font(UIComponent::DEFAULT_FONT, 13.0f)
            .with_padding(Padding{.left = pixels(PAD)})
            .with_skip_tabbing(true)
            .with_debug_name("send_to_label"));

    // Message input - single line (multiline text_area not yet implemented)
    // Framework auto-scales text to fill container height, so size container
    // to match desired font: ~14pt font + 10px text margins = 24px
    constexpr float INPUT_FONT_SIZE = 14.0f;
    constexpr int INPUT_HEIGHT = 24;  // Forces ~14pt font via auto-scaling

    auto input_container =
        div(context, mk(window.ent(), 6),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(INNER_W - PAD * 2), pixels(INPUT_HEIGHT + 4)})
                .with_custom_background(AIMColors::input_bg())
                .with_margin(Margin{.left = pixels(PAD), .right = pixels(PAD)})
                .disable_rounded_corners()
                .with_debug_name("input_container"));

    if (afterhours::text_input::text_input(
            context, mk(input_container.ent(), 0), message_input,
            ComponentConfig{}
                .with_size(ComponentSize{pixels(INNER_W - PAD * 4), pixels(INPUT_HEIGHT)})
                .with_custom_background(AIMColors::input_bg())
                .with_custom_text_color(AIMColors::my_text())
                .with_font(UIComponent::DEFAULT_FONT, INPUT_FONT_SIZE)
                .disable_rounded_corners()
                .with_debug_name("message_input"))) {
    }

    // Button bar - padding to keep buttons from window edge
    auto button_bar =
        div(context, mk(window.ent(), 7),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(INNER_W - PAD * 2), pixels(36)})
                .with_flex_direction(FlexDirection::Row)
                .with_justify_content(JustifyContent::FlexEnd)
                .with_align_items(AlignItems::Center)
                .with_margin(Margin{.top = pixels(6), .left = pixels(PAD)})
                .with_debug_name("button_bar"));

    // All buttons in row - using smaller sizes
    const char* btn_labels[] = {"Warn", "Block", "Send"};
    for (int i = 0; i < 3; i++) {
      div(context, mk(button_bar.ent(), i),
          ComponentConfig{}
              .with_label(btn_labels[i])
              .with_size(ComponentSize{pixels(60), pixels(26)})
              .with_custom_background(AIMColors::button_face())
              .with_custom_text_color(AIMColors::text_default())
              .with_font(UIComponent::DEFAULT_FONT, 13.0f)
              .with_margin(Margin{.left = pixels(4)})
              .disable_rounded_corners()
              .with_skip_tabbing(true)
              .with_debug_name("action_btn"));
    }

    // Status bar
    div(context, mk(window.ent(), 8),
        ComponentConfig{}
            .with_label("Direct Connection Established")
            .with_size(ComponentSize{pixels(INNER_W), pixels(24)})
            .with_custom_background(AIMColors::button_face())
            .with_custom_text_color(AIMColors::text_default())
            .with_alignment(TextAlignment::Left)
            .with_font(UIComponent::DEFAULT_FONT, 13.0f)
            .with_padding(Padding{.left = pixels(PAD)})
            .with_skip_tabbing(true)
            .with_debug_name("status_bar"));
  }
};

REGISTER_EXAMPLE_SCREEN(aim_chat, "Text Editing",
                        "AIM-style chat demo with multi-line text input",
                        AIMChatDemo)
