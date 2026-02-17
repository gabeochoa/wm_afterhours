#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
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

  // Configurable UI parameters
  struct UIConfig {
    // Touch target sizes
    static constexpr int WINDOW_CONTROL_SIZE =
        24; // Minimum touch target for window controls
    static constexpr int BUTTON_HEIGHT =
        44; // Standard button height for comfortable touch
    static constexpr int BUTTON_WIDTH = 70; // Standard button width

    // Status indicator
    static constexpr int STATUS_DOT_SIZE =
        10; // Size of online status indicator dot

    // Spacing
    static constexpr int BUTTON_SPACING = 6;    // Space between buttons
    static constexpr int BUTTON_BAR_MARGIN = 8; // Right margin for button bar
  };

  // Placeholder text for input field
  std::string placeholder_text = "Type a message...";

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
    static afterhours::Color placeholder_text() {
      return {160, 160, 160, 255};
    } // Gray placeholder
    static afterhours::Color online_green() {
      return {0, 180, 0, 255};
    } // Online status indicator
  };

  AIMChatDemo() {
    chat_history.push_back({buddy_name, "hey whats up"});
    chat_history.push_back({my_name, "nm u?"});
    chat_history.push_back(
        {buddy_name, "just chillin. did u see that new movie?"});
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = context.theme;
    theme.background = AIMColors::window_bg();
    theme.surface = AIMColors::button_face();
    theme.primary = AIMColors::title_bar();
    theme.secondary = AIMColors::input_bg();
    theme.font = AIMColors::text_default();
    theme.darkfont = AIMColors::text_default();
    theme.accent = AIMColors::button_shadow();
    context.theme = theme;
    context.scaling_mode = ScalingMode::Adaptive;

    // Window dimensions - sized for comfortable chat viewing
    // Children use percent(1.0f) to fill content area; small padding
    // provides the classic Win98 border inset.
    constexpr int W = 500;
    constexpr int H = 600;
    constexpr int PAD = 8;

    // Main window - sharp corners for Windows 98 aesthetic
    auto window = vstack(context, mk(entity, 0),
                         ComponentConfig{}
                             .with_size(ComponentSize{pixels(W), pixels(H)})
                             .with_custom_background(AIMColors::window_bg())
                             .with_padding(Padding{.top = pixels(PAD),
                                                   .left = pixels(PAD),
                                                   .bottom = pixels(PAD),
                                                   .right = pixels(PAD)})
                             .disable_rounded_corners()
                             .with_self_align(SelfAlign::Center)
                             .with_debug_name("aim_window"));

    // Title bar
    auto title_bar =
        hstack(context, mk(window.ent(), 0),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.0f), pixels(28)})
                   .with_custom_background(AIMColors::title_bar())
                   .with_justify_content(JustifyContent::SpaceBetween)
                   .with_align_items(AlignItems::Center)
                   .with_debug_name("title_bar"));

    // Title text - expand to fill remaining space after controls
    div(context, mk(title_bar.ent(), 0),
        ComponentConfig{}
            .with_label(buddy_name + " - Instant Message")
            .with_size(ComponentSize{expand(), pixels(28)})
            .with_custom_text_color(AIMColors::title_text())
            .with_alignment(TextAlignment::Left)
            .with_font(UIComponent::DEFAULT_FONT, pixels(18.0f))
            .with_padding(Padding{.left = pixels(6)})
            .with_skip_tabbing(true));

    // Window controls container - groups buttons on the right
    // Height matches title bar (28px), width accounts for 3 buttons (24px each
    // + 2px margins)
    auto controls_container =
        hstack(context, mk(title_bar.ent(), 1),
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(82), pixels(28)})
                   .with_align_items(AlignItems::Center)
                   .with_justify_content(JustifyContent::FlexEnd)
                   .with_margin(Margin{.right = pixels(2)})
                   .with_debug_name("controls_container"));

    const char *ctrl_labels[] = {"_", "[]", "X"};
    for (int i = 0; i < 3; i++) {
      div(context, mk(controls_container.ent(), i),
          ComponentConfig{}
              .with_label(ctrl_labels[i])
              .with_size(ComponentSize{pixels(UIConfig::WINDOW_CONTROL_SIZE),
                                       pixels(UIConfig::WINDOW_CONTROL_SIZE)})
              .with_custom_background(AIMColors::button_face())
              .with_custom_text_color(AIMColors::text_default())
              .with_font(UIComponent::DEFAULT_FONT, pixels(14.0f))
              .with_margin(Margin{.left = pixels(2)})
              .disable_rounded_corners()
              .with_skip_tabbing(true));
    }

    // Menu bar
    auto menu_bar =
        hstack(context, mk(window.ent(), 1),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.0f), pixels(24)})
                   .with_custom_background(AIMColors::button_face())
                   .with_align_items(AlignItems::Center)
                   .with_debug_name("menu_bar"));

    const char *menus[] = {"File", "Edit", "Insert", "People"};
    for (int i = 0; i < 4; i++) {
      div(context, mk(menu_bar.ent(), i),
          ComponentConfig{}
              .with_label(menus[i])
              .with_size(ComponentSize{pixels(55), pixels(22)})
              .with_custom_text_color(AIMColors::text_default())
              .with_font(UIComponent::DEFAULT_FONT, pixels(18.0f))
              .with_padding(Padding{.left = pixels(6)})
              .with_skip_tabbing(true));
    }

    // Buddy info bar
    auto buddy_bar =
        hstack(context, mk(window.ent(), 2),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.0f), pixels(50)})
                   .with_custom_background(AIMColors::window_bg())
                   .with_align_items(AlignItems::Center)
                   .with_padding(Padding{.left = pixels(PAD)})
                   .with_debug_name("buddy_bar"));

    div(context, mk(buddy_bar.ent(), 0),
        ComponentConfig{}
            .with_label(":-)")
            .with_size(ComponentSize{pixels(40), pixels(40)})
            .with_custom_background(AIMColors::warning_yellow())
            .with_custom_text_color(AIMColors::text_default())
            .with_font(UIComponent::DEFAULT_FONT, pixels(18.0f))
            .with_skip_tabbing(true));

    auto buddy_info =
        vstack(context, mk(buddy_bar.ent(), 1),
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(210), pixels(40)})
                   .with_padding(Padding{.left = pixels(10)}));

    div(context, mk(buddy_info.ent(), 0),
        ComponentConfig{}
            .with_label(buddy_name)
            .with_size(ComponentSize{pixels(190), pixels(22)})
            .with_custom_text_color(AIMColors::buddy_text())
            .with_alignment(TextAlignment::Left)
            .with_font(UIComponent::DEFAULT_FONT, pixels(18.0f))
            .with_skip_tabbing(true));

    // Status row with indicator dot and text
    auto status_row =
        hstack(context, mk(buddy_info.ent(), 1),
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(120), pixels(18)})
                   .with_align_items(AlignItems::Center));

    // Green status indicator dot
    div(context, mk(status_row.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(UIConfig::STATUS_DOT_SIZE),
                                     pixels(UIConfig::STATUS_DOT_SIZE)})
            .with_custom_background(AIMColors::online_green())
            .with_rounded_corners(RoundedCorners().all_round())
            .with_roundness(1.0f) // Fully circular
            .with_margin(Margin{.right = pixels(4)})
            .with_skip_tabbing(true)
            .with_debug_name("status_dot"));

    div(context, mk(status_row.ent(), 1),
        ComponentConfig{}
            .with_label("Available")
            .with_size(ComponentSize{pixels(80), pixels(18)})
            .with_custom_text_color(AIMColors::text_default())
            .with_alignment(TextAlignment::Left)
            .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
            .with_skip_tabbing(true));

    // Chat history container - holds scroll view and scroll indicator side by
    // side
    auto chat_container = hstack(
        context, mk(window.ent(), 3),
        ComponentConfig{}.with_size(ComponentSize{percent(1.0f), pixels(280)}));

    // Chat history area - scrollable messages
    auto chat_area =
        vstack(context, mk(chat_container.ent(), 0),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.0f), pixels(280)})
                   .with_custom_background(AIMColors::chat_bg())
                   .with_align_items(AlignItems::FlexStart)
                   .with_padding(Padding{.left = pixels(6), .top = pixels(4)})
                   .with_overflow(Overflow::Scroll, Axis::Y)
                   .disable_rounded_corners()
                   .with_debug_name("chat_area"));

    // Read scroll state for chat indicator
    float chat_max_scroll = 0.0f;
    float chat_scroll_ratio = 0.0f;
    if (chat_area.ent().has<HasScrollView>()) {
      auto &sv = chat_area.ent().get<HasScrollView>();
      chat_max_scroll = std::max(0.0f, sv.content_size.y - sv.viewport_size.y);
      chat_scroll_ratio =
          chat_max_scroll > 0.0f ? sv.scroll_offset.y / chat_max_scroll : 0.0f;
    }

    int msg_idx = 0;
    for (const auto &[sender, msg] : chat_history) {
      bool is_me = (sender == my_name);
      std::string formatted = sender + ": " + msg;

      // Visual hierarchy: bold sender names via color intensity
      div(context, mk(chat_area.ent(), msg_idx++),
          ComponentConfig{}
              .with_label(formatted)
              .with_size(ComponentSize{percent(1.0f), pixels(28)})
              .with_custom_text_color(is_me ? AIMColors::my_text()
                                            : AIMColors::buddy_text())
              .with_alignment(TextAlignment::Left)
              .with_font(UIComponent::DEFAULT_FONT, pixels(18.0f))
              .with_margin(Margin{.bottom = pixels(4)})
              .with_skip_tabbing(true));
    }

    // Scroll bar track (vertical indicator for chat area)
    constexpr float SCROLL_TRACK_HEIGHT = 260.0f;
    constexpr float SCROLL_TRACK_WIDTH = 12.0f;
    auto scroll_track =
        div(context, mk(chat_container.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(SCROLL_TRACK_WIDTH),
                                         pixels(SCROLL_TRACK_HEIGHT)})
                .with_custom_background(afterhours::Color{200, 200, 200, 255})
                .with_margin(Margin{.left = pixels(2), .top = pixels(10)})
                .disable_rounded_corners()
                .with_skip_tabbing(true)
                .with_debug_name("scroll_track"));

    // Scroll bar thumb - calculate size and position based on content
    float thumb_ratio =
        chat_max_scroll > 0.0f
            ? SCROLL_TRACK_HEIGHT / (SCROLL_TRACK_HEIGHT + chat_max_scroll)
            : 1.0f;
    float thumb_height = std::max(20.0f, SCROLL_TRACK_HEIGHT * thumb_ratio);
    float thumb_offset =
        chat_scroll_ratio * (SCROLL_TRACK_HEIGHT - thumb_height);

    div(context, mk(scroll_track.ent(), 0),
        ComponentConfig{}
            .with_size(
                ComponentSize{pixels(SCROLL_TRACK_WIDTH), pixels(thumb_height)})
            .with_custom_background(afterhours::Color{100, 100, 100, 255})
            .with_absolute_position(pixels(0), pixels(thumb_offset))
            .disable_rounded_corners()
            .with_skip_tabbing(true)
            .with_debug_name("scroll_thumb"));

    // Separator
    div(context, mk(window.ent(), 4),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(3)})
            .with_custom_background(AIMColors::button_shadow())
            .with_margin(Margin{.top = pixels(4), .bottom = pixels(4)})
            .with_skip_tabbing(true)
            .with_debug_name("separator"));

    // Input label
    div(context, mk(window.ent(), 5),
        ComponentConfig{}
            .with_label("Send to: " + buddy_name)
            .with_size(ComponentSize{percent(1.0f), pixels(24)})
            .with_custom_text_color(AIMColors::text_default())
            .with_alignment(TextAlignment::Left)
            .with_font(UIComponent::DEFAULT_FONT, pixels(18.0f))
            .with_padding(Padding{.left = pixels(PAD)})
            .with_skip_tabbing(true));

    // Message input - multiline text area
    // 3 lines at 18px line height = 54px, plus padding
    constexpr float INPUT_LINE_HEIGHT = 18.0f;
    constexpr int INPUT_LINES = 3;
    constexpr int INPUT_HEIGHT =
        static_cast<int>(INPUT_LINE_HEIGHT * INPUT_LINES) + 8;

    // Wrapper div to contain both input and placeholder overlay
    auto input_wrapper = div(
        context, mk(window.ent(), 6),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(INPUT_HEIGHT + 4)})
            .with_debug_name("input_wrapper"));

    auto input_container = div(
        context, mk(input_wrapper.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(INPUT_HEIGHT + 4)})
            .with_custom_background(AIMColors::input_bg())
            .disable_rounded_corners()
            .with_debug_name("input_container"));

    if (afterhours::text_input::text_area(
            context, mk(input_container.ent(), 0), message_input,
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), pixels(INPUT_HEIGHT)})
                .with_custom_background(AIMColors::input_bg())
                .with_custom_text_color(AIMColors::my_text())
                .with_line_height(pixels(INPUT_LINE_HEIGHT))
                .with_max_lines(INPUT_LINES)
                .with_font(UIComponent::DEFAULT_FONT, pixels(18.0f))
                .disable_rounded_corners()
                .with_debug_name("message_input"))) {
    }

    // Placeholder text - render in wrapper, positioned absolutely over the
    // input Since text_area forces a background, we render placeholder on
    // higher render layer
    if (message_input.empty()) {
      div(context, mk(input_wrapper.ent(), 1),
          ComponentConfig{}
              .with_label(placeholder_text)
              .with_size(
                  ComponentSize{percent(1.0f), pixels(INPUT_LINE_HEIGHT)})
              .with_custom_text_color(AIMColors::placeholder_text())
              .with_alignment(TextAlignment::Left)
              .with_font(UIComponent::DEFAULT_FONT, pixels(18.0f))
              .with_absolute_position(pixels(6), pixels(6))
              .with_skip_tabbing(true)
              .with_render_layer(10)); // Render on top of text_area
    }

    // Button bar - padding to keep buttons from window edge
    auto button_bar =
        hstack(context, mk(window.ent(), 7),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.0f), pixels(48)})
                   .with_justify_content(JustifyContent::FlexEnd)
                   .with_align_items(AlignItems::Center)
                   .with_margin(Margin{.top = pixels(6)})
                   .with_debug_name("button_bar"));

    // Warn button - shows confirmation dialog explaining consequences
    if (button(
            context, mk(button_bar.ent(), 0),
            ComponentConfig{}
                .with_label("Warn")
                .with_size(ComponentSize{pixels(UIConfig::BUTTON_WIDTH),
                                         pixels(UIConfig::BUTTON_HEIGHT)})
                .with_custom_background(AIMColors::button_face())
                .with_custom_text_color(AIMColors::text_default())
                .with_border(AIMColors::button_shadow(), 2.0f)
                .with_font(UIComponent::DEFAULT_FONT, pixels(18.0f))
                .with_margin(Margin{.left = pixels(UIConfig::BUTTON_SPACING)})
                .disable_rounded_corners()
                .with_debug_name("warn_btn"))) {
      show_warn_confirm = true;
    }

    // Block button - shows confirmation dialog explaining consequences
    if (button(
            context, mk(button_bar.ent(), 1),
            ComponentConfig{}
                .with_label("Block")
                .with_size(ComponentSize{pixels(UIConfig::BUTTON_WIDTH),
                                         pixels(UIConfig::BUTTON_HEIGHT)})
                .with_custom_background(AIMColors::button_face())
                .with_custom_text_color(AIMColors::text_default())
                .with_border(AIMColors::button_shadow(), 2.0f)
                .with_font(UIComponent::DEFAULT_FONT, pixels(18.0f))
                .with_margin(Margin{.left = pixels(UIConfig::BUTTON_SPACING)})
                .disable_rounded_corners()
                .with_debug_name("block_btn"))) {
      show_block_confirm = true;
    }

    // Send button
    button(context, mk(button_bar.ent(), 2),
           ComponentConfig{}
               .with_label("Send")
               .with_size(ComponentSize{pixels(UIConfig::BUTTON_WIDTH),
                                        pixels(UIConfig::BUTTON_HEIGHT)})
               .with_custom_background(AIMColors::button_face())
               .with_custom_text_color(AIMColors::text_default())
               .with_border(AIMColors::button_shadow(), 2.0f)
               .with_font(UIComponent::DEFAULT_FONT, pixels(18.0f))
               .with_margin(Margin{.left = pixels(UIConfig::BUTTON_SPACING)})
               .disable_rounded_corners());

    // Status bar
    div(context, mk(window.ent(), 8),
        ComponentConfig{}
            .with_label("Direct Connection Established")
            .with_size(ComponentSize{percent(1.0f), pixels(28)})
            .with_custom_background(AIMColors::button_face())
            .with_custom_text_color(AIMColors::text_default())
            .with_alignment(TextAlignment::Left)
            .with_font(UIComponent::DEFAULT_FONT, pixels(18.0f))
            .with_padding(Padding{.left = pixels(PAD)})
            .with_skip_tabbing(true));

    // Confirmation dialog for Warn action
    afterhours::modal::confirm(
        context, mk(entity, MODAL_WARN), show_warn_confirm,
        "Warn " + buddy_name + "?",
        "This will send a warning to " + buddy_name +
            ". If you warn someone too often, you may be penalized.",
        "Send Warning", "Cancel");

    // Confirmation dialog for Block action
    afterhours::modal::confirm(
        context, mk(entity, MODAL_BLOCK), show_block_confirm,
        "Block " + buddy_name + "?",
        "Blocking " + buddy_name +
            " will prevent them from contacting you. You can unblock them "
            "later in your Buddy List settings.",
        "Block User", "Cancel");
  }
};

REGISTER_EXAMPLE_SCREEN(aim_chat, "Text Editing",
                        "AIM-style chat demo with multi-line text input",
                        AIMChatDemo)
