#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>
#include <afterhours/src/plugins/ui/text_input/text_input.h>
#include <array>
#include <cctype>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct AIMChatDemo : ScreenSystem<UIContext<InputAction>> {
  struct Message { std::string sender, text; bool bold = false; int color = 0; };
  struct Buddy {
    std::string name;
    std::vector<Message> messages;
    std::string draft;
    bool blocked = false;
    int warning = 0;
  };
  enum class Dialog { None, Warn, Block, Info, Help, Fonts, Clear, File, Edit, Insert, People, Start };
  std::array<Buddy, 3> buddies{{
      {"SmarterChild", {{"SmarterChild", "hey whats up"}, {"coolkid2001", "nm u?"},
                       {"SmarterChild", "just chillin. did u see that new movie?"}}},
      {"sk8ergirl88", {}}, {"xXguitarguyXx", {}}}};
  size_t selected = 0;
  std::string my_name = "coolkid2001", message_input;
  bool chat_open = true, buddy_open = true, maximized = false, away = false;
  bool setup = false, bold = false, replace_composer = false, scroll_bottom = false;
  int color = 0, text_size = 15;
  Dialog dialog = Dialog::None;
  bool loaded = false;
  float scale = 1;
  std::array<raylib::Texture2D, 4> art{};
  const afterhours::Color ink{17, 17, 17, 255}, face{212, 208, 200, 255};
  const afterhours::Color white{255, 255, 255, 255}, blue{0, 0, 128, 255};
  const std::array<afterhours::Color, 3> message_colors{{{0, 0, 187, 255}, {180, 20, 20, 255}, {0, 110, 65, 255}}};

  void load() {
    if (loaded) return;
    const std::array<const char *, 4> names{"runner", "smiley", "computer", "folder"};
    for (size_t i = 0; i < art.size(); ++i) {
      art[i] = raylib::LoadTexture(afterhours::files::get_resource_path("images",
          std::string("aim_chat/") + names[i] + ".png").string().c_str());
      raylib::SetTextureFilter(art[i], raylib::TEXTURE_FILTER_BILINEAR);
    }
    loaded = true;
  }
  ComponentConfig box(float x, float y, float w, float h) const {
    return ComponentConfig{}.with_size({pixels(w * scale), pixels(h * scale)})
        .with_absolute_position(x * scale, y * scale).with_background(Theme::Usage::None)
        .with_corner_radius(0);
  }
  static void bevel(RectangleType r, bool inset, float s) {
    afterhours::Color light{255, 255, 255, 255}, dark{112, 112, 112, 255};
    if (inset) std::swap(light, dark);
    afterhours::draw_rectangle({r.x, r.y, r.width, s}, light);
    afterhours::draw_rectangle({r.x, r.y, s, r.height}, light);
    afterhours::draw_rectangle({r.x, r.y + r.height - s, r.width, s}, dark);
    afterhours::draw_rectangle({r.x + r.width - s, r.y, s, r.height}, dark);
  }
  ElementResult label(UIContext<InputAction> &c, afterhours::Entity &p, int id,
                      const std::string &text, float x, float y, float w, float h,
                      float size, afterhours::Color color, const std::string &name = "",
                      bool strong = false, int layer = 0) {
    return div(c, mk(p, id), box(x, y, w, h).with_label(text)
        .with_font("AtkinsonMock", h720(size * 1.25f))
        .with_text_stroke(color, strong ? .25f * scale : 0).with_letter_spacing(-.5f * scale)
        .with_render_layer(layer)
        .with_custom_text_color(color).with_alignment(TextAlignment::Left)
        .with_ignore_pointer_events().with_debug_name(name));
  }
  ElementResult action(UIContext<InputAction> &c, afterhours::Entity &p, int id,
                       const std::string &text, float x, float y, float w, float h,
                       const std::string &name, bool down = false, bool disabled = false) {
    const bool modal_action = name.starts_with("aim_dialog_") || name.starts_with("aim_menu_action_");
    return button(c, mk(p, id), box(x, y, w, h).with_label(text)
        .with_font("AtkinsonMock", h720(15)).with_letter_spacing(-.5f * scale).with_alignment(TextAlignment::Center)
        .with_custom_text_color(ink).with_custom_background(down ? white : face)
        .with_on_draw_fg([down, s = scale](RectangleType r) { bevel(r, down, s); })
        .with_disabled(disabled || (dialog != Dialog::None && !modal_action)).with_click_activation(ClickActivationMode::Release)
        .with_debug_name(name));
  }
  void image(UIContext<InputAction> &c, afterhours::Entity &p, int id, int i,
             float x, float y, float w, float h) {
    div(c, mk(p, id), box(x, y, w, h).with_ignore_pointer_events()
        .with_on_draw_bg([texture = art[i]](RectangleType r) {
          raylib::DrawTexturePro(texture, {0, 0, static_cast<float>(texture.width),
              static_cast<float>(texture.height)}, r, {0, 0}, 0, raylib::WHITE);
        }));
  }
  void title(UIContext<InputAction> &c, afterhours::Entity &p, const std::string &text,
             float w, bool conversation) {
    div(c, mk(p, 900), box(5, 5, w - 10, 25)
        .with_on_draw_bg([](RectangleType r) {
          raylib::DrawRectangleGradientH(static_cast<int>(r.x), static_cast<int>(r.y),
              static_cast<int>(r.width), static_cast<int>(r.height), {0, 0, 128, 255}, {16, 132, 207, 255});
        }).with_ignore_pointer_events());
    label(c, p, 901, text, 10, 7, w - 90, 21, 13, white, "", true);
    const std::string prefix = conversation ? "aim_chat_" : "aim_buddy_";
    if (action(c, p, 902, "_", w - 68, 8, 19, 19, prefix + "minimize")) {
      if (conversation) chat_open = false; else buddy_open = false;
    }
    if (action(c, p, 903, "[]", w - 47, 8, 19, 19, prefix + "maximize", false, !conversation))
      maximized = !maximized;
    if (action(c, p, 904, "X", w - 26, 8, 19, 19, prefix + "close")) {
      if (conversation) chat_open = false; else buddy_open = false;
    }
  }
  void sync_composer(afterhours::Entity &e) {
    auto &st = e.get<afterhours::text_input::HasTextAreaState>();
    st.storage.data = message_input;
    st.cursor_position = message_input.size();
    st.clear_selection();
    st.rebuild_line_index();
  }
  void send_message(afterhours::Entity &e) {
    if (buddies[selected].blocked || dialog != Dialog::None) return;
    while (!message_input.empty() && std::isspace(static_cast<unsigned char>(message_input.back())))
      message_input.pop_back();
    if (message_input.empty()) return;
    buddies[selected].messages.push_back({my_name, message_input, bold, color});
    message_input.clear(); buddies[selected].draft.clear();
    sync_composer(e); scroll_bottom = true;
  }
  void choose(size_t i) {
    buddies[selected].draft = message_input;
    selected = i; message_input = buddies[i].draft;
    replace_composer = true; chat_open = true; scroll_bottom = true;
  }

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &c, float) override {
    load(); scale = c.screen_height / 720.f;
    Theme theme; theme.font = ink; theme.darkfont = ink; theme.background = face;
    theme.surface = face; theme.secondary = white; theme.primary = blue; theme.accent = blue; theme.corner_radius = 0; theme.roundness = 0;
    c.set_theme(theme); c.scaling_mode = ScalingMode::Proportional;
    UIStylingDefaults::get().set_default_font("AtkinsonMock", h720(16.25f));
    if (c.pressed(InputAction::MenuBack)) dialog = Dialog::None;
    auto root = div(c, mk(entity), box(0, 0, 1280, 720).with_custom_background({0, 130, 128, 255}).with_debug_name("aim_desktop"));
    image(c, root.ent(), 0, 2, 25, 16, 65, 47);
    label(c, root.ent(), 1, "My Computer", 19, 65, 92, 22, 12, white);
    image(c, root.ent(), 2, 3, 25, 109, 65, 47);
    label(c, root.ent(), 3, "My Documents", 16, 157, 101, 22, 12, white);
    image(c, root.ent(), 4, 0, 33, 207, 50, 56);
    label(c, root.ent(), 5, "AOL Instant", 25, 267, 100, 19, 12, white);
    label(c, root.ent(), 6, "Messenger", 29, 284, 90, 19, 12, white);
    if (buddy_open) build_buddy(c, root.ent());
    if (chat_open) build_chat(c, root.ent());
    div(c, mk(root.ent(), 500), box(0, 687, 1280, 33).with_custom_background({195, 195, 195, 255})
        .with_on_draw_fg([s = scale](RectangleType r) { bevel(r, false, s); }));
    if (action(c, root.ent(), 501, "Start", 4, 691, 78, 26, "aim_start")) dialog = Dialog::Start;
    if (action(c, root.ent(), 502, buddies[selected].name + " - Instant Message", 88, 691, 253, 25, "aim_restore_chat", chat_open)) chat_open = true;
    if (action(c, root.ent(), 503, "Buddy List", 347, 691, 190, 25, "aim_restore_buddies", buddy_open)) buddy_open = true;
    label(c, root.ent(), 504, "2:03 PM", 1190, 694, 83, 21, 12, ink);
    if (dialog != Dialog::None) build_dialog(c, root.ent());
  }

  void build_buddy(UIContext<InputAction> &c, afterhours::Entity &root) {
    auto panel = div(c, mk(root, 100), box(150, 58, 297, 566).with_custom_background(face)
        .with_on_draw_fg([s = scale](RectangleType r) { bevel(r, false, s); }).with_debug_name("aim_buddy_window"));
    auto &p = panel.ent(); title(c, p, "AOL Instant Messenger", 297, false);
    if (action(c, p, 0, "My AIM", 10, 33, 61, 23, "aim_my_menu")) dialog = Dialog::Fonts;
    if (action(c, p, 1, "People", 75, 33, 60, 23, "aim_people_menu")) dialog = Dialog::People;
    if (action(c, p, 2, "Help", 141, 33, 45, 23, "aim_help")) dialog = Dialog::Help;
    div(c, mk(p, 3), box(5, 56, 287, 90).with_custom_background({226, 220, 202, 255}));
    image(c, p, 4, 0, 21, 65, 68, 76);
    label(c, p, 5, "AOL Instant", 94, 70, 190, 28, 20, {32, 41, 127, 255}, "", true);
    label(c, p, 6, "Messenger", 94, 93, 190, 26, 20, {32, 41, 127, 255}, "", true);
    label(c, p, 7, my_name, 94, 122, 180, 20, 12, ink);
    if (action(c, p, 8, "Buddy List", 10, 158, 92, 25, "aim_list", !setup)) setup = false;
    if (action(c, p, 9, "List Setup", 104, 158, 93, 25, "aim_setup", setup)) setup = true;
    div(c, mk(p, 10), box(10, 183, 277, 214).with_custom_background(white)
        .with_on_draw_fg([s = scale](RectangleType r) { bevel(r, true, s); }));
    label(c, p, 11, setup ? "Select a buddy to manage" : "v Buddies (3/8)", 20, 193, 252, 24, 13, ink);
    for (size_t i = 0; i < buddies.size(); ++i) {
      const bool picked = selected == i;
      if (button(c, mk(p, 20 + static_cast<int>(i)), box(29, 223 + i * 25, 245, 23)
          .with_label(buddies[i].name + (buddies[i].blocked ? " (blocked)" : ""))
          .with_font("AtkinsonMock", h720(16.25f)).with_custom_background(picked ? blue : white)
          .with_custom_text_color(picked ? white : ink).with_alignment(TextAlignment::Left)
          .with_disabled(dialog != Dialog::None)
          .with_click_activation(ClickActivationMode::Release).with_debug_name("aim_buddy_" + std::to_string(i)))) choose(i);
    }
    if (setup) {
      if (action(c, p, 30, buddies[selected].blocked ? "Unblock buddy" : "Block buddy", 28, 318, 245, 28, "aim_setup_block")) dialog = Dialog::Block;
      if (action(c, p, 31, "Buddy information", 28, 354, 245, 28, "aim_setup_info")) dialog = Dialog::Info;
    } else {
      label(c, p, 32, "> Family (0/3)", 20, 309, 253, 24, 13, ink);
      label(c, p, 33, "> Co-Workers (0/4)", 20, 334, 253, 24, 13, ink);
      label(c, p, 34, "> Offline (12)", 20, 359, 253, 24, 13, ink);
    }
    const std::array<const char *, 4> buttons{"IM", "Chat", "Info", "Setup"};
    for (int i = 0; i < 4; ++i)
      if (action(c, p, 40 + i, buttons[i], 18 + i * 67, 413, 59, 40, "aim_buddy_action_" + std::to_string(i))) {
        if (i == 0 || i == 1) chat_open = true;
        else if (i == 2) dialog = Dialog::Info;
        else setup = !setup;
      }
    if (action(c, p, 50, away ? "Away - be right back" : "I'm Available", 12, 465, 273, 30, "aim_availability", true)) away = !away;
    div(c, mk(p, 51), box(12, 506, 273, 47).with_custom_background({23, 66, 162, 255}));
    label(c, p, 52, "AOL", 24, 510, 72, 38, 30, white, "", true);
    label(c, p, 53, "You've got a friend.", 112, 519, 164, 22, 12, white);
  }

  void build_chat(UIContext<InputAction> &c, afterhours::Entity &root) {
    const float x = maximized ? 225 : 493, y = maximized ? 30 : 90;
    const float w = maximized ? 1020 : 636, h = maximized ? 630 : 551;
    auto window = div(c, mk(root, 200), box(x, y, w, h).with_custom_background(face)
        .with_on_draw_fg([s = scale](RectangleType r) { bevel(r, false, s); }).with_debug_name("aim_window"));
    auto &p = window.ent(); auto &buddy = buddies[selected];
    title(c, p, buddy.name + " - Instant Message", w, true);
    const std::array<const char *, 4> menus{"File", "Edit", "Insert", "People"};
    const std::array<Dialog, 4> dialogs{Dialog::File, Dialog::Edit, Dialog::Insert, Dialog::People};
    for (int i = 0; i < 4; ++i)
      if (action(c, p, i, menus[i], 11 + i * 48, 32, 45, 23, "aim_menu_" + std::to_string(i))) dialog = dialogs[i];
    image(c, p, 10, 1, 15, 66, 44, 44);
    label(c, p, 11, buddy.name, 72, 68, 235, 25, 14, {201, 0, 0, 255}, "", true);
    label(c, p, 12, buddy.blocked ? "Blocked" : "Available", 82, 94, 186, 20, 11, ink, "aim_buddy_status");
    div(c, mk(p, 13), box(73, 101, 7, 7).with_custom_background(buddy.blocked ? afterhours::Color{160, 160, 160, 255} : afterhours::Color{0, 140, 0, 255}).with_roundness(1));
    label(c, p, 14, fmt::format("Warning level: {}%", buddy.warning), w - 142, 84, 130, 20, 11, ink, "aim_warning_level");
    auto history = vstack(c, mk(p, 20), box(12, 121, w - 24, h - 336)
        .with_custom_background(white).with_padding(Padding{.top = pixels(9 * scale), .left = pixels(8 * scale),
              .bottom = pixels(8 * scale), .right = pixels(8 * scale)})
        .with_on_draw_fg([s = scale](RectangleType r) { bevel(r, true, s); })
        .with_overflow(dialog == Dialog::None ? Overflow::Scroll : Overflow::Hidden, Axis::Y).with_debug_name("chat_area"));
    if (history.ent().has<HasScrollView>()) {
      auto &scroll = history.ent().get<HasScrollView>();
      scroll.vertical_enabled = dialog == Dialog::None;
      scroll.horizontal_enabled = false;
    }
    int index = 0;
    for (const auto &message : buddy.messages) {
      const auto name_color = message.sender == my_name ? message_colors[message.color] : afterhours::Color{201, 0, 0, 255};
      div(c, mk(history.ent(), index++), ComponentConfig{}
          .with_size({percent(1), Size{Dim::Text, 0, 1}})
          .with_styled_label({{message.sender + ": ", name_color}, {message.text, ink}})
          .with_font(message.bold ? "ArchivoMockBold" : "AtkinsonMock", h720(18.75f))
          .with_letter_spacing(-.5f * scale)
          .with_text_overflow(TextOverflow::Wrap).with_alignment(TextAlignment::Left)
          .with_margin(Margin{.bottom = pixels(8 * scale)}).with_background(Theme::Usage::None)
          .with_ignore_pointer_events());
    }
    if (selected == 0) {
      auto pasted = div(c, mk(history.ent(), 2000), ComponentConfig{}
          .with_size({percent(1), pixels(25 * scale)}).with_custom_background({244, 243, 235, 255})
          .with_overflow(dialog == Dialog::None ? Overflow::Scroll : Overflow::Hidden, Axis::X).with_debug_name("pasted_log"));
      if (pasted.ent().has<HasScrollView>()) {
        auto &scroll = pasted.ent().get<HasScrollView>();
        scroll.horizontal_enabled = dialog == Dialog::None;
        scroll.vertical_enabled = false;
      }
      label(c, pasted.ent(), 0, "14:02:11 GET /index.html 200 | 14:02:12 GET /style.css 200 | 14:02:14 POST /api/send 500",
            4, 2, 900, 22, 11, ink, "pasted_log_line");
    }
    div(c, mk(history.ent(), 2001), ComponentConfig{}.with_size({percent(1), pixels(32 * scale)})
        .with_label(buddy.messages.empty() ? "Local conversation. Messages stay in this demo." : "Last message received at 2:02 PM.")
        .with_font("AtkinsonMock", h720(13.75f)).with_custom_text_color({136, 136, 136, 255})
        .with_alignment(TextAlignment::Left).with_background(Theme::Usage::None).with_ignore_pointer_events());
    if (scroll_bottom && history.ent().has<HasScrollView>()) {
      auto &scroll = history.ent().get<HasScrollView>();
      scroll.scroll_offset.y = std::max(0.f, scroll.content_size.y - scroll.viewport_or_zero().y);
      scroll_bottom = false;
    }
    const float format_y = h - 215;
    if (action(c, p, 30, "A", 14, format_y + 4, 23, 23, "aim_font_size")) {
      text_size = text_size == 18 ? 13 : text_size + 1;
    }
    if (action(c, p, 31, "B", 40, format_y + 4, 23, 23, "aim_bold", bold)) bold = !bold;
    if (action(c, p, 32, "I", 66, format_y + 4, 23, 23, "aim_italic")) dialog = Dialog::Fonts;
    if (action(c, p, 33, "U", 92, format_y + 4, 23, 23, "aim_underline")) dialog = Dialog::Fonts;
    if (action(c, p, 34, "A", 133, format_y + 4, 23, 23, "aim_color", color != 0)) color = (color + 1) % 3;
    if (action(c, p, 35, ":)", 159, format_y + 4, 23, 23, "aim_smiley")) {
      message_input += " :-)"; replace_composer = true;
    }
    if (action(c, p, 36, "/", 185, format_y + 4, 23, 23, "aim_link")) dialog = Dialog::Insert;
    label(c, p, 37, "Send to: " + buddy.name, 14, h - 184, w - 30, 20, 11, ink);
    const std::string replacement = replace_composer ? message_input : std::string{};
    auto composer = afterhours::text_input::text_area(c, mk(p, 40), message_input,
        box(12, h - 164, w - 24, 79).with_custom_background(white)
        .with_custom_text_color(message_colors[color]).with_border({128, 128, 128, 255}, scale)
        .with_font(bold ? "ArchivoMockBold" : "AtkinsonMock", h720(text_size * 1.25f))
        .with_line_height(pixels(22 * scale)).with_max_lines(3).with_submit_on_enter()
        .with_disabled(buddy.blocked || dialog != Dialog::None).with_debug_name("message_input"));
    if (replace_composer) { message_input = replacement; sync_composer(composer.ent()); replace_composer = false; }
    composer.ent().addComponentIfMissing<afterhours::text_input::HasTextInputListener>();
    composer.ent().get<afterhours::text_input::HasTextInputListener>().on_submit =
        [this](afterhours::Entity &e) { send_message(e); };
    buddies[selected].draft = message_input;
    if (message_input.empty() && dialog == Dialog::None)
      label(c, p, 41, buddy.blocked ? "Unblock this buddy to send messages." : "Type a message...",
            21, h - 158, w - 44, 27, 15, {160, 160, 160, 255}, "aim_placeholder", false, 10);
    if (action(c, p, 50, "Warn", w - 245, h - 79, 70, 39, "warn_btn")) dialog = Dialog::Warn;
    if (action(c, p, 51, buddy.blocked ? "Unblock" : "Block", w - 167, h - 79, 70, 39, "block_btn")) dialog = Dialog::Block;
    if (action(c, p, 52, "Send", w - 89, h - 79, 70, 39, "aim_send", false,
               buddy.blocked || dialog != Dialog::None)) send_message(composer.ent());
    label(c, p, 53, "Direct Connection Established (local demo)", 12, h - 31, w - 30, 22, 11, ink, "aim_connection");
  }

  void build_dialog(UIContext<InputAction> &c, afterhours::Entity &root) {
    auto &buddy = buddies[selected];
    const bool menu = dialog == Dialog::File || dialog == Dialog::Edit || dialog == Dialog::Insert || dialog == Dialog::People || dialog == Dialog::Start;
    std::string heading, body;
    switch (dialog) {
    case Dialog::Warn: heading = "Warn " + buddy.name + "?"; body = "Raise this buddy's warning level by 20%. This changes only the local demo; no warning is sent over a network."; break;
    case Dialog::Block: heading = (buddy.blocked ? "Unblock " : "Block ") + buddy.name + "?"; body = buddy.blocked ? "Allow composing messages to this buddy again. Your conversation history is preserved." : "Block this buddy in the local Buddy List. Sending is disabled until you unblock them. Conversation history is preserved."; break;
    case Dialog::Info: heading = buddy.name + " - Buddy Info"; body = fmt::format("Status: {}\nWarning level: {}%\n{} messages in this local conversation.", buddy.blocked ? "Blocked" : "Available", buddy.warning, buddy.messages.size()); break;
    case Dialog::Help: heading = "AOL Instant Messenger - Help"; body = "This is a local AIM-style demo. Enter sends; Shift+Enter inserts a new line. Select buddies to keep separate conversations. Warn and Block affect this demo only."; break;
    case Dialog::Fonts: heading = "Message formatting"; body = "Use A to change composing size, B for bold, and the second A to cycle blue, red, and green sender colors. Italic and underline are unavailable with the bundled fonts."; break;
    case Dialog::Clear: heading = "Clear conversation?"; body = "Remove the displayed messages from this local conversation. Your unsent draft will be kept."; break;
    case Dialog::File: heading = "File"; break;
    case Dialog::Edit: heading = "Edit"; break;
    case Dialog::Insert: heading = "Insert"; break;
    case Dialog::People: heading = "People"; break;
    case Dialog::Start: heading = "Start"; break;
    default: break;
    }
    div(c, mk(root, 700), box(0, 0, 1280, 687).with_custom_background({0, 0, 0, 65}).with_overlay(10));
    auto panel = div(c, mk(root, 701), box(405, 232, 470, menu ? 242 : 256)
        .with_custom_background(face).with_on_draw_fg([s = scale](RectangleType r) { bevel(r, false, s); })
        .with_overlay(11).with_debug_name("aim_dialog"));
    auto &p = panel.ent();
    div(c, mk(p, 0), box(5, 5, 460, 27).with_custom_background(blue));
    label(c, p, 1, heading, 13, 8, 429, 24, 14, white, "aim_dialog_title", true);
    if (menu) {
      std::array<std::string, 3> options;
      if (dialog == Dialog::File) options = {"Clear conversation...", "Close conversation", "Cancel"};
      if (dialog == Dialog::Edit) options = {"Message formatting...", "Clear unsent draft", "Cancel"};
      if (dialog == Dialog::Insert) options = {"Insert smiley :-)", "Insert https://example.com", "Cancel"};
      if (dialog == Dialog::People) options = {"Buddy information...", buddy.blocked ? "Unblock buddy..." : "Block buddy...", "Cancel"};
      if (dialog == Dialog::Start) options = {"Open Buddy List", "Open Instant Message", "Cancel"};
      for (int i = 0; i < 3; ++i)
        if (action(c, p, 10 + i, options[i], 25, 52 + i * 53, 420, 39, "aim_menu_action_" + std::to_string(i))) {
          const Dialog old = dialog; dialog = Dialog::None;
          if (i == 2) continue;
          if (old == Dialog::File) { if (i == 0) dialog = Dialog::Clear; else chat_open = false; }
          if (old == Dialog::Edit) { if (i == 0) dialog = Dialog::Fonts; else { message_input.clear(); replace_composer = true; } }
          if (old == Dialog::Insert) { message_input += i == 0 ? " :-)" : " https://example.com"; replace_composer = true; }
          if (old == Dialog::People) dialog = i == 0 ? Dialog::Info : Dialog::Block;
          if (old == Dialog::Start) { if (i == 0) buddy_open = true; else chat_open = true; }
        }
      return;
    }
    div(c, mk(p, 2), box(25, 52, 420, 126).with_label(body).with_font("AtkinsonMock", h720(17.5f))
        .with_text_overflow(TextOverflow::Wrap).with_custom_text_color(ink).with_ignore_pointer_events());
    const bool confirmation = dialog == Dialog::Warn || dialog == Dialog::Block || dialog == Dialog::Clear;
    std::string primary = "OK";
    if (dialog == Dialog::Warn) primary = "Send Warning";
    if (dialog == Dialog::Block) primary = buddy.blocked ? "Unblock User" : "Block User";
    if (dialog == Dialog::Clear) primary = "Clear messages";
    if (action(c, p, 3, primary, confirmation ? 156 : 307, 201, 138, 34, "aim_dialog_confirm")) {
      if (dialog == Dialog::Warn) buddy.warning = std::min(100, buddy.warning + 20);
      if (dialog == Dialog::Block) buddy.blocked = !buddy.blocked;
      if (dialog == Dialog::Clear) buddy.messages.clear();
      dialog = Dialog::None;
    }
    if (confirmation && action(c, p, 4, "Cancel", 307, 201, 138, 34, "aim_dialog_cancel")) dialog = Dialog::None;
  }
};

REGISTER_EXAMPLE_SCREEN(aim_chat, "App Mockups", "AIM-style chat demo with multi-line text input", AIMChatDemo)
