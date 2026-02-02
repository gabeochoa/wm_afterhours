#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/modal.h>
#include <afterhours/src/plugins/ui/text_input/text_input.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct AIMChatDemo : ScreenSystem<UIContext<InputAction>> {
  // Chat state
  std::string message_input = "";
  std::vector<std::pair<std::string, std::string>> chat_history;
  std::string buddy_name = "SmarterChild";
  std::string my_name = "coolkid2001";

  // Confirmation dialog states
  bool show_warn_confirm = false;
  bool show_block_confirm = false;

  // Modal entity IDs
  static constexpr int MODAL_WARN = 100;
  static constexpr int MODAL_BLOCK = 101;

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

    // Window dimensions - sized for comfortable chat viewing with safe area margins
    // Note: Children use INNER_W to avoid fractional overflow warnings (W - 3)
    constexpr int W = 500;
    constexpr int H = 600;
    constexpr int INNER_W = 485;  // Account for safe area margin
    constexpr int PAD = 10;  // Increased padding for safe areas
    constexpr int SAFE_MARGIN = 8;  // Safe area margin at screen edges

    // Main window - sharp corners for Windows 98 aesthetic with safe area margin
    // Centered on screen to utilize available space
    auto window =
        div(context, mk(entity, 0),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(W), pixels(H)})
                .with_custom_background(AIMColors::window_bg())
                .with_flex_direction(FlexDirection::Column)
                .with_padding(Padding{.left = pixels(SAFE_MARGIN), .right = pixels(SAFE_MARGIN),
                                      .top = pixels(SAFE_MARGIN), .bottom = pixels(SAFE_MARGIN)})
                .disable_rounded_corners()
                .with_self_align(SelfAlign::Center)
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
            .with_size(ComponentSize{pixels(INNER_W - 80), pixels(28)})  // Leave room for controls
            .with_custom_text_color(AIMColors::title_text())
            .with_alignment(TextAlignment::Left)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_padding(Padding{.left = pixels(6)})
            .with_skip_tabbing(true)
            .with_debug_name("title_text"));

    // Window controls container - groups buttons on the right
    auto controls_container =
        div(context, mk(title_bar.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(72), pixels(24)})
                .with_flex_direction(FlexDirection::Row)
                .with_align_items(AlignItems::Center)
                .with_justify_content(JustifyContent::FlexEnd)
                .with_margin(Margin{.right = pixels(2)})
                .with_debug_name("controls_container"));

    const char* ctrl_labels[] = {"_", "[]", "X"};
    for (int i = 0; i < 3; i++) {
      div(context, mk(controls_container.ent(), i),
          ComponentConfig{}
              .with_label(ctrl_labels[i])
              .with_size(ComponentSize{pixels(20), pixels(20)})
              .with_custom_background(AIMColors::button_face())
              .with_custom_text_color(AIMColors::text_default())
              .with_font(UIComponent::DEFAULT_FONT, 12.0f)
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
              .with_font(UIComponent::DEFAULT_FONT, 18.0f)
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
            .with_debug_name("buddy_icon_available"));

    auto buddy_info =
        div(context, mk(buddy_bar.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(210), pixels(40)})
                .with_flex_direction(FlexDirection::Column)
                .with_padding(Padding{.left = pixels(10)})
                .with_debug_name("buddy_info"));

    div(context, mk(buddy_info.ent(), 0),
        ComponentConfig{}
            .with_label(buddy_name)
            .with_size(ComponentSize{pixels(190), pixels(22)})
            .with_custom_text_color(AIMColors::buddy_text())
            .with_alignment(TextAlignment::Left)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_skip_tabbing(true)
            .with_debug_name("buddy_name_display"));

    div(context, mk(buddy_info.ent(), 1),
        ComponentConfig{}
            .with_label("Available")
            .with_size(ComponentSize{pixels(90), pixels(18)})
            .with_custom_text_color(AIMColors::text_default())
            .with_alignment(TextAlignment::Left)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_skip_tabbing(true)
            .with_debug_name("buddy_status"));

    // Chat history container - holds scroll view and scroll indicator side by side
    auto chat_container =
        div(context, mk(window.ent(), 3),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(INNER_W - PAD * 2), pixels(280)})
                .with_flex_direction(FlexDirection::Row)
                .with_margin(Margin{.left = pixels(PAD), .right = pixels(PAD)})
                .with_debug_name("chat_container"));

    // Chat history area - uses scroll_view for scrollable messages
    auto chat_area =
        scroll_view(context, mk(chat_container.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(INNER_W - PAD * 2 - 14), pixels(280)})
                .with_custom_background(AIMColors::chat_bg())
                .with_flex_direction(FlexDirection::Column)
                .with_align_items(AlignItems::FlexStart)
                .with_padding(Padding{.left = pixels(6), .top = pixels(4)})
                .with_clip_children(true)
                .disable_rounded_corners()
                .with_debug_name("chat_area"));

    // Configure vertical scrolling for chat
    float chat_max_scroll = 0.0f;
    float chat_scroll_ratio = 0.0f;
    if (chat_area.ent().has<HasScrollView>()) {
      auto &sv = chat_area.ent().get<HasScrollView>();
      sv.vertical_enabled = true;
      sv.horizontal_enabled = false;
      chat_max_scroll = std::max(0.0f, sv.content_size.y - sv.viewport_size.y);
      chat_scroll_ratio = chat_max_scroll > 0.0f ? sv.scroll_offset.y / chat_max_scroll : 0.0f;
    }

    int msg_idx = 0;
    for (const auto& [sender, msg] : chat_history) {
      bool is_me = (sender == my_name);
      std::string formatted = sender + ": " + msg;

      // Visual hierarchy: bold sender names via color intensity
      div(context, mk(chat_area.ent(), msg_idx++),
          ComponentConfig{}
              .with_label(formatted)
              .with_size(ComponentSize{pixels(INNER_W - PAD * 4 - 14), pixels(28)})
              .with_custom_text_color(is_me ? AIMColors::my_text() : AIMColors::buddy_text())
              .with_alignment(TextAlignment::Left)
              .with_font(UIComponent::DEFAULT_FONT, 18.0f)
              .with_margin(Margin{.bottom = pixels(4)})
              .with_skip_tabbing(true)
              .with_debug_name("chat_msg"));
    }

    // Scroll bar track (vertical indicator for chat area)
    constexpr float SCROLL_TRACK_HEIGHT = 260.0f;
    constexpr float SCROLL_TRACK_WIDTH = 12.0f;
    auto scroll_track = div(
        context, mk(chat_container.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(SCROLL_TRACK_WIDTH), pixels(SCROLL_TRACK_HEIGHT)})
            .with_custom_background(afterhours::Color{200, 200, 200, 255})
            .with_margin(Margin{.left = pixels(2), .top = pixels(10)})
            .disable_rounded_corners()
            .with_skip_tabbing(true)
            .with_debug_name("scroll_track"));

    // Scroll bar thumb - calculate size and position based on content
    float thumb_ratio = chat_max_scroll > 0.0f ? SCROLL_TRACK_HEIGHT / (SCROLL_TRACK_HEIGHT + chat_max_scroll) : 1.0f;
    float thumb_height = std::max(20.0f, SCROLL_TRACK_HEIGHT * thumb_ratio);
    float thumb_offset = chat_scroll_ratio * (SCROLL_TRACK_HEIGHT - thumb_height);

    div(context, mk(scroll_track.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(SCROLL_TRACK_WIDTH), pixels(thumb_height)})
            .with_custom_background(afterhours::Color{100, 100, 100, 255})
            .with_absolute_position()
            .with_translate(pixels(0), pixels(thumb_offset))
            .disable_rounded_corners()
            .with_skip_tabbing(true)
            .with_debug_name("scroll_thumb"));

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
            .with_size(ComponentSize{pixels(INNER_W), pixels(24)})
            .with_custom_text_color(AIMColors::text_default())
            .with_alignment(TextAlignment::Left)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_padding(Padding{.left = pixels(PAD)})
            .with_skip_tabbing(true)
            .with_debug_name("send_to_label"));

    // Message input - multiline text area
    // 3 lines at 18px line height = 54px, plus padding
    constexpr float INPUT_LINE_HEIGHT = 18.0f;
    constexpr int INPUT_LINES = 3;
    constexpr int INPUT_HEIGHT = static_cast<int>(INPUT_LINE_HEIGHT * INPUT_LINES) + 8;

    auto input_container =
        div(context, mk(window.ent(), 6),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(INNER_W - PAD * 2), pixels(INPUT_HEIGHT + 4)})
                .with_custom_background(AIMColors::input_bg())
                .with_margin(Margin{.left = pixels(PAD), .right = pixels(PAD)})
                .disable_rounded_corners()
                .with_debug_name("input_container"));

    if (afterhours::text_input::text_area(
            context, mk(input_container.ent(), 0), message_input,
            ComponentConfig{}
                .with_size(ComponentSize{pixels(INNER_W - PAD * 4), pixels(INPUT_HEIGHT)})
                .with_custom_background(AIMColors::input_bg())
                .with_custom_text_color(AIMColors::my_text())
                .with_line_height(pixels(INPUT_LINE_HEIGHT))
                .with_max_lines(INPUT_LINES)
                .with_font(UIComponent::DEFAULT_FONT, 18.0f)
                .disable_rounded_corners()
                .with_debug_name("message_input"))) {
    }

    // Button bar - padding to keep buttons from window edge
    auto button_bar =
        div(context, mk(window.ent(), 7),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(INNER_W - PAD * 2), pixels(48)})
                .with_flex_direction(FlexDirection::Row)
                .with_justify_content(JustifyContent::FlexEnd)
                .with_align_items(AlignItems::Center)
                .with_margin(Margin{.top = pixels(6), .left = pixels(PAD)})
                .with_debug_name("button_bar"));

    // Warn button - shows confirmation dialog explaining consequences
    if (button(context, mk(button_bar.ent(), 0),
               ComponentConfig{}
                   .with_label("Warn")
                   .with_size(ComponentSize{pixels(70), pixels(44)})
                   .with_custom_background(AIMColors::button_face())
                   .with_custom_text_color(AIMColors::text_default())
                   .with_border(AIMColors::button_shadow(), 2.0f)
                   .with_font(UIComponent::DEFAULT_FONT, 18.0f)
                   .with_margin(Margin{.left = pixels(6)})
                   .disable_rounded_corners()
                   .with_debug_name("warn_btn"))) {
      show_warn_confirm = true;
    }

    // Block button - shows confirmation dialog explaining consequences
    if (button(context, mk(button_bar.ent(), 1),
               ComponentConfig{}
                   .with_label("Block")
                   .with_size(ComponentSize{pixels(70), pixels(44)})
                   .with_custom_background(AIMColors::button_face())
                   .with_custom_text_color(AIMColors::text_default())
                   .with_border(AIMColors::button_shadow(), 2.0f)
                   .with_font(UIComponent::DEFAULT_FONT, 18.0f)
                   .with_margin(Margin{.left = pixels(6)})
                   .disable_rounded_corners()
                   .with_debug_name("block_btn"))) {
      show_block_confirm = true;
    }

    // Send button
    button(context, mk(button_bar.ent(), 2),
           ComponentConfig{}
               .with_label("Send")
               .with_size(ComponentSize{pixels(70), pixels(44)})
               .with_custom_background(AIMColors::button_face())
               .with_custom_text_color(AIMColors::text_default())
               .with_border(AIMColors::button_shadow(), 2.0f)
               .with_font(UIComponent::DEFAULT_FONT, 18.0f)
               .with_margin(Margin{.left = pixels(6)})
               .disable_rounded_corners()
               .with_debug_name("send_btn"));

    // Status bar
    div(context, mk(window.ent(), 8),
        ComponentConfig{}
            .with_label("Direct Connection Established")
            .with_size(ComponentSize{pixels(INNER_W), pixels(28)})
            .with_custom_background(AIMColors::button_face())
            .with_custom_text_color(AIMColors::text_default())
            .with_alignment(TextAlignment::Left)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_padding(Padding{.left = pixels(PAD)})
            .with_skip_tabbing(true)
            .with_debug_name("status_bar"));

    // Confirmation dialog for Warn action
    afterhours::modal::confirm(
        context, mk(entity, MODAL_WARN), show_warn_confirm,
        "Warn " + buddy_name + "?",
        "This will send a warning to " + buddy_name + ". If you warn someone too often, you may be penalized.",
        "Send Warning", "Cancel");

    // Confirmation dialog for Block action
    afterhours::modal::confirm(
        context, mk(entity, MODAL_BLOCK), show_block_confirm,
        "Block " + buddy_name + "?",
        "Blocking " + buddy_name + " will prevent them from contacting you. You can unblock them later in your Buddy List settings.",
        "Block User", "Cancel");
  }
};

REGISTER_EXAMPLE_SCREEN(aim_chat, "Text Editing",
                        "AIM-style chat demo with multi-line text input",
                        AIMChatDemo)
