#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>
#include <afterhours/src/plugins/modal.h>
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
  std::array<Buddy, 4> buddies{{
      {"SmarterChild", {{"SmarterChild", "hey whats up"}, {"coolkid2001", "nm u?"},
                       {"SmarterChild", "just chillin. did u see that new movie?"}}},
      {"sk8ergirl88", {}}, {"xXguitarguyXx", {}}, {"Friends room", {}}}};
  size_t selected = 0, selected_buddy = 0;
  std::string my_name = "coolkid2001", message_input;
  bool chat_open = true, buddy_open = true, maximized = false, away = false;
  bool setup = false, bold = false, replace_composer = false, scroll_bottom = false;
  bool show_log = false;
  std::array<bool, 4> groups_open{true, false, false, false};
  enum class Glyph { None, Minimize, Maximize, Close, Down, Right, Color, Link };
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
      raylib::SetTextureFilter(art[i], i >= 2 ? raylib::TEXTURE_FILTER_POINT : raylib::TEXTURE_FILTER_BILINEAR);
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
  static void draw_glyph(RectangleType r, Glyph glyph, float s, afterhours::Color color) {
    const float x = r.x + r.width / 2, y = r.y + r.height / 2;
    const auto line = [&](float ax, float ay, float bx, float by) {
      raylib::DrawLineEx({x + ax * s, y + ay * s}, {x + bx * s, y + by * s}, s, color);
    };
    switch (glyph) {
    case Glyph::Minimize: line(-4, 4, 4, 4); line(-4, 3, 4, 3); return;
    case Glyph::Maximize:
      raylib::DrawRectangleLinesEx({x - 4 * s, y - 4 * s, 9 * s, 9 * s}, s, color);
      line(-4, -2, 5, -2); return;
    case Glyph::Close: line(-4, -4, 4, 4); line(-4, 4, 4, -4); return;
    case Glyph::Down:
      raylib::DrawTriangle({x - 4 * s, y - 2 * s}, {x, y + 3 * s}, {x + 4 * s, y - 2 * s}, color); return;
    case Glyph::Right:
      raylib::DrawTriangle({x - 2 * s, y - 4 * s}, {x - 2 * s, y + 4 * s}, {x + 3 * s, y}, color); return;
    case Glyph::Color:
      afterhours::draw_rectangle({r.x + 4 * s, r.y + r.height - 4 * s, r.width - 8 * s, 2 * s}, color); return;
    case Glyph::Link:
      raylib::DrawRectangleLinesEx({x - 7 * s, y - 2 * s, 9 * s, 6 * s}, s, color);
      raylib::DrawRectangleLinesEx({x - 1 * s, y - 5 * s, 9 * s, 6 * s}, s, color);
      line(-2, 1, 3, -2); return;
    case Glyph::None: return;
    }
  }
  ElementResult action(UIContext<InputAction> &c, afterhours::Entity &p, int id,
                       const std::string &text, float x, float y, float w, float h,
                       const std::string &name, bool down = false, bool disabled = false,
                       bool flat = false, Glyph glyph = Glyph::None) {
    const bool modal_action = name.starts_with("aim_dialog_") || name.starts_with("aim_menu_action_");
    return button(c, mk(p, id), box(x, y, w, h).with_label(text)
        .with_font("AtkinsonMock", h720(16.25f)).with_letter_spacing(-.5f * scale).with_alignment(TextAlignment::Center)
        .with_custom_text_color(ink).with_custom_background(down ? white : face)
        .with_on_draw_fg([down, flat, glyph, s = scale, color = glyph == Glyph::Color ? message_colors[color] : ink](RectangleType r) {
          if (!flat || down) bevel(r, down, s);
          draw_glyph(r, glyph, s, color);
        })
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
        .with_on_draw_bg([active = conversation || !chat_open](RectangleType r) {
          raylib::DrawRectangleGradientH(static_cast<int>(r.x), static_cast<int>(r.y),
              static_cast<int>(r.width), static_cast<int>(r.height), active ? raylib::Color{0, 0, 128, 255} : raylib::Color{100, 105, 112, 255},
              active ? raylib::Color{16, 132, 207, 255} : raylib::Color{160, 166, 174, 255});
        }).with_ignore_pointer_events());
    label(c, p, 901, text, 10, 7, w - 90, 21, 13, white, "", true);
    const std::string prefix = conversation ? "aim_chat_" : "aim_buddy_";
    if (action(c, p, 902, "", w - 68, 8, 19, 19, prefix + "minimize", false, false, false, Glyph::Minimize)) {
      if (conversation) chat_open = false; else buddy_open = false;
    }
    if (action(c, p, 903, "", w - 47, 8, 19, 19, prefix + "maximize", false, !conversation, false, Glyph::Maximize))
      maximized = !maximized;
    if (action(c, p, 904, "", w - 26, 8, 19, 19, prefix + "close", false, false, false, Glyph::Close)) {
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
    selected = i;
    if (i < 3) selected_buddy = i;
    message_input = buddies[i].draft;
    replace_composer = true; chat_open = true; scroll_bottom = true;
  }

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &c, float) override {
    load(); scale = c.screen_height / 720.f;
    Theme theme; theme.font = ink; theme.darkfont = ink; theme.background = face;
    theme.surface = face; theme.secondary = white; theme.primary = blue; theme.accent = blue; theme.corner_radius = 0; theme.roundness = 0;
    c.set_theme(theme); c.scaling_mode = ScalingMode::Proportional;
    UIStylingDefaults::get().set_default_font("AtkinsonMock", h720(16.25f));
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
    if (action(c, root.ent(), 502, buddies[selected].name + (selected == 3 ? " - Chat Room" : " - Instant Message"), 88, 691, 253, 25, "aim_restore_chat", chat_open)) chat_open = true;
    if (action(c, root.ent(), 503, "Buddy List", 347, 691, 190, 25, "aim_restore_buddies", buddy_open)) buddy_open = true;
    label(c, root.ent(), 504, "2:03 PM", 1190, 694, 83, 21, 12, ink);
    build_dialog(c, root.ent());
  }

  void build_buddy(UIContext<InputAction> &c, afterhours::Entity &root) {
    auto panel = div(c, mk(root, 100), box(150, 58, 297, 566).with_custom_background(face)
        .with_on_draw_fg([s = scale](RectangleType r) { bevel(r, false, s); }).with_debug_name("aim_buddy_window"));
    auto &p = panel.ent(); title(c, p, "AOL Instant Messenger", 297, false);
    if (action(c, p, 0, "My AIM", 10, 33, 61, 23, "aim_my_menu", false, false, true)) dialog = Dialog::Fonts;
    if (action(c, p, 1, "People", 75, 33, 60, 23, "aim_people_menu", false, false, true)) { choose(selected_buddy); dialog = Dialog::People; }
    if (action(c, p, 2, "Help", 141, 33, 45, 23, "aim_help", false, false, true)) dialog = Dialog::Help;
    div(c, mk(p, 3), box(5, 56, 287, 90).with_custom_background({226, 220, 202, 255}));
    image(c, p, 4, 0, 21, 65, 68, 76);
    label(c, p, 5, "AOL Instant", 94, 70, 190, 28, 20, {32, 41, 127, 255}, "", true);
    label(c, p, 6, "Messenger", 94, 93, 190, 26, 20, {32, 41, 127, 255}, "", true);
    label(c, p, 7, my_name, 94, 122, 180, 20, 12, ink);
    if (action(c, p, 8, "Buddy List", 10, 158, 92, 25, "aim_list", !setup)) setup = false;
    if (action(c, p, 9, "List Setup", 104, 158, 93, 25, "aim_setup", setup)) { choose(selected_buddy); setup = true; }
    div(c, mk(p, 10), box(10, 183, 277, 224).with_custom_background(white)
        .with_on_draw_fg([s = scale](RectangleType r) { bevel(r, true, s); }));
    if (setup) label(c, p, 11, "Select a buddy to manage", 20, 193, 252, 24, 13, ink);
    else group(c, p, 0, "Buddies", "3/8 online", 193);
    if (setup || groups_open[0]) {
      for (size_t i = 0; i < 3; ++i) {
        const bool picked = selected_buddy == i;
        if (button(c, mk(p, 20 + static_cast<int>(i)), box(29, 223 + i * 25, 245, 23)
            .with_label("")
            .with_font("AtkinsonMock", h720(16.25f)).with_custom_background(picked ? blue : white)
            .with_custom_text_color(picked ? white : ink).with_alignment(TextAlignment::Left)
            .with_disabled(dialog != Dialog::None)
            .with_click_activation(ClickActivationMode::Release).with_debug_name("aim_buddy_" + std::to_string(i)))) choose(i);
        div(c, mk(p, 60 + static_cast<int>(i)), box(35, 231 + i * 25, 7, 7)
            .with_custom_background(buddies[i].blocked ? afterhours::Color{128, 128, 128, 255} : afterhours::Color{0, 155, 40, 255})
            .with_ignore_pointer_events());
        label(c, p, 70 + static_cast<int>(i), buddies[i].name + (buddies[i].blocked ? " (blocked)" : ""),
              46, 223 + i * 25, 224, 23, 13, picked ? white : ink);
      }
    }
    if (setup) {
      if (action(c, p, 30, buddies[selected].blocked ? "Unblock buddy" : "Block buddy", 28, 318, 245, 28, "aim_setup_block")) dialog = Dialog::Block;
      if (action(c, p, 31, "Buddy information", 28, 354, 245, 28, "aim_setup_info")) dialog = Dialog::Info;
    } else {
      group(c, p, 1, "Family", "0/3 online", 309);
      group(c, p, 2, "Co-Workers", "0/4 online", 334);
      group(c, p, 3, "Offline", "0/12 online", 359);
    }
    const std::array<const char *, 4> buttons{"IM", "Room", "Info", "Setup"};
    for (int i = 0; i < 4; ++i)
      if (action(c, p, 40 + i, buttons[i], 18 + i * 67, 413, 59, 40, "aim_buddy_action_" + std::to_string(i))) {
        if (i == 0) choose(selected_buddy);
        else if (i == 1) { choose(3); setup = false; }
        else if (i == 2) { choose(selected_buddy); dialog = Dialog::Info; }
        else { choose(selected_buddy); setup = !setup; }
      }
    if (action(c, p, 50, away ? "Away - be right back" : "I'm Available", 12, 465, 273, 30, "aim_availability", true)) away = !away;
    div(c, mk(p, 54), box(259, 469, 20, 22).with_ignore_pointer_events()
        .with_on_draw_fg([s = scale, color = ink](RectangleType r) { draw_glyph(r, Glyph::Down, s, color); }));
    div(c, mk(p, 51), box(12, 521, 273, 32).with_custom_background({23, 66, 162, 255}));
    label(c, p, 52, "AOL", 24, 522, 72, 30, 24, white, "", true);
    label(c, p, 53, "You've got a friend.", 102, 526, 174, 22, 12, white);
  }

  void group(UIContext<InputAction> &c, afterhours::Entity &p, int index,
             const std::string &name, const std::string &count, float y) {
    const bool open = groups_open[index];
    if (button(c, mk(p, 100 + index), box(20, y, 254, 24).with_custom_background(white)
        .with_label("").with_disabled(dialog != Dialog::None)
        .with_click_activation(ClickActivationMode::Release).with_debug_name("aim_group_" + std::to_string(index)))) {
      if (index > 0)
        for (int other = 1; other < 4; ++other) groups_open[other] = false;
      groups_open[index] = !open;
    }
    div(c, mk(p, 110 + index), box(20, y, 16, 24).with_ignore_pointer_events()
        .with_on_draw_fg([open, s = scale, color = ink](RectangleType r) {
          draw_glyph(r, open ? Glyph::Down : Glyph::Right, s, color);
        }));
    label(c, p, 120 + index, name + "  " + count, 34, y, 240, 24, 12, ink);
    if (!open || index == 0) return;
    label(c, p, 130 + index, name + ": all contacts offline", 20, 384, 260, 18, 11, ink);
  }

  void build_chat(UIContext<InputAction> &c, afterhours::Entity &root) {
    const float x = maximized ? 225 : 493, y = maximized ? 30 : 90;
    const float w = maximized ? 1020 : 636, h = maximized ? 630 : 551;
    auto window = div(c, mk(root, 200), box(x, y, w, h).with_custom_background(face)
        .with_on_draw_fg([s = scale](RectangleType r) { bevel(r, false, s); }).with_debug_name("aim_window"));
    auto &p = window.ent(); auto &buddy = buddies[selected];
    title(c, p, buddy.name + (selected == 3 ? " - Chat Room" : " - Instant Message"), w, true);
    const std::array<const char *, 4> menus{"File", "Edit", "Insert", "People"};
    const std::array<Dialog, 4> dialogs{Dialog::File, Dialog::Edit, Dialog::Insert, Dialog::People};
    for (int i = 0; i < 4; ++i)
      if (action(c, p, i, menus[i], 11 + i * 48, 32, 45, 23, "aim_menu_" + std::to_string(i), false, selected == 3 && i == 3, true)) dialog = dialogs[i];
    image(c, p, 10, 1, 15, 66, 44, 44);
    label(c, p, 11, buddy.name, 72, 68, 235, 25, 14, ink, "", true);
    label(c, p, 12, selected == 3 ? "Local chat room" : buddy.blocked ? "Blocked" : "Available", 82, 94, 186, 20, 11, ink, "aim_buddy_status");
    div(c, mk(p, 13), box(73, 101, 7, 7).with_custom_background(buddy.blocked ? afterhours::Color{160, 160, 160, 255} : afterhours::Color{0, 140, 0, 255}).with_roundness(1));
    if (selected != 3) label(c, p, 14, fmt::format("Warning level: {}%", buddy.warning), 228, 94, 200, 20, 11, {75, 75, 75, 255}, "aim_warning_level");
    auto history = vstack(c, mk(p, 20), box(12, 121, w - 24, h - (show_log ? 397 : 336))
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
          .with_font(message.bold ? "AtkinsonMockBold" : "AtkinsonMock", h720(18.75f))
          .with_letter_spacing(-.5f * scale)
          .with_text_overflow(TextOverflow::Wrap).with_alignment(TextAlignment::Left)
          .with_margin(Margin{.bottom = pixels(3 * scale)}).with_background(Theme::Usage::None)
          .with_ignore_pointer_events());
    }
    div(c, mk(history.ent(), 2001), ComponentConfig{}.with_size({percent(1), pixels(32 * scale)})
        .with_label(buddy.messages.empty() ? "Local conversation. Messages stay in this demo." : "Last message received at 2:02 PM.")
        .with_font("AtkinsonMock", h720(15)).with_custom_text_color({88, 88, 88, 255})
        .with_alignment(TextAlignment::Left).with_background(Theme::Usage::None).with_ignore_pointer_events());
    if (scroll_bottom && history.ent().has<HasScrollView>()) {
      auto &scroll = history.ent().get<HasScrollView>();
      scroll.scroll_offset.y = std::max(0.f, scroll.content_size.y - scroll.viewport_or_zero().y);
      scroll_bottom = false;
    }
    if (show_log) {
      label(c, p, 2002, "Developer log sample", 12, h - 275, w - 24, 24, 12, ink);
      auto pasted = div(c, mk(p, 2000), box(12, h - 248, w - 24, 27).with_custom_background({244, 243, 235, 255})
          .with_overflow(dialog == Dialog::None ? Overflow::Scroll : Overflow::Hidden, Axis::X).with_debug_name("pasted_log"));
      if (pasted.ent().has<HasScrollView>()) {
        auto &scroll = pasted.ent().get<HasScrollView>();
        scroll.horizontal_enabled = dialog == Dialog::None;
        scroll.vertical_enabled = false;
      }
      label(c, pasted.ent(), 0, "14:02:11 GET /index.html 200 | 14:02:12 GET /style.css 200 | 14:02:14 POST /api/send 500",
            4, 2, 900, 22, 11, ink, "pasted_log_line");
    }
    const float format_y = h - 215;
    if (action(c, p, 30, "Aa", 14, format_y + 4, 26, 23, "aim_font_size", false, false, true)) {
      text_size = text_size == 18 ? 13 : text_size + 1;
    }
    if (action(c, p, 31, "B", 40, format_y + 4, 23, 23, "aim_bold", bold, false, true)) bold = !bold;
    if (action(c, p, 32, "I", 66, format_y + 4, 23, 23, "aim_italic", false, false, true)) dialog = Dialog::Fonts;
    if (action(c, p, 33, "U", 92, format_y + 4, 23, 23, "aim_underline", false, false, true)) dialog = Dialog::Fonts;
    if (action(c, p, 34, "A", 133, format_y + 4, 23, 23, "aim_color", color != 0, false, true, Glyph::Color)) color = (color + 1) % 3;
    if (action(c, p, 35, "", 159, format_y + 4, 23, 23, "aim_smiley", false, false, true)) {
      message_input += " :-)"; replace_composer = true;
    }
    if (action(c, p, 36, "", 199, format_y + 4, 23, 23, "aim_link", false, false, true, Glyph::Link)) dialog = Dialog::Insert;
    image(c, p, 38, 1, 162, format_y + 7, 17, 17);
    for (int i = 0; i < 2; ++i)
      div(c, mk(p, 80 + i), box(i == 0 ? 123 : 189, format_y + 6, 1, 19)
          .with_custom_background({128, 128, 128, 255}).with_ignore_pointer_events());
    if (action(c, p, 82, show_log ? "Hide demo log" : "Demo log...", w - 134, format_y + 4, 120, 23,
               "aim_demo_log", show_log, false, true)) show_log = !show_log;
    label(c, p, 37, "Send to: " + buddy.name, 16, h - 184, w - 30, 20, 12, ink);
    const std::string replacement = replace_composer ? message_input : std::string{};
    auto composer = afterhours::text_input::text_area(c, mk(p, 40), message_input,
        box(12, h - 164, w - 24, 79).with_custom_background(white)
        .with_custom_text_color(message_colors[color]).with_border({128, 128, 128, 255}, scale)
        .with_font(bold ? "AtkinsonMockBold" : "AtkinsonMock", h720(text_size * 1.25f))
        .with_line_height(pixels(22 * scale)).with_max_lines(3).with_submit_on_enter()
        .with_disabled(buddy.blocked || dialog != Dialog::None).with_debug_name("message_input"));
    if (replace_composer) { message_input = replacement; sync_composer(composer.ent()); replace_composer = false; }
    composer.ent().addComponentIfMissing<afterhours::text_input::HasTextInputListener>();
    composer.ent().get<afterhours::text_input::HasTextInputListener>().on_submit =
        [this](afterhours::Entity &e) { send_message(e); };
    buddies[selected].draft = message_input;
    if (message_input.empty() && dialog == Dialog::None)
      label(c, p, 41, buddy.blocked ? "Unblock this buddy to send messages." : "Type a message...",
            21, h - 158, w - 44, 27, 15, {98, 98, 98, 255}, "aim_placeholder", false, 10);
    if (selected != 3 && action(c, p, 50, "Warn", 14, h - 79, 70, 39, "warn_btn")) dialog = Dialog::Warn;
    if (selected != 3 && action(c, p, 51, buddy.blocked ? "Unblock" : "Block", 92, h - 79, 70, 39, "block_btn")) dialog = Dialog::Block;
    div(c, mk(p, 54), box(w - 91, h - 81, 74, 43).with_ignore_pointer_events()
        .with_on_draw_fg([s = scale, color = ink](RectangleType r) { raylib::DrawRectangleLinesEx(r, s, color); }));
    if (action(c, p, 52, "Send", w - 89, h - 79, 70, 39, "aim_send", false,
               buddy.blocked || dialog != Dialog::None)) send_message(composer.ent());
    div(c, mk(p, 55), box(17, h - 23, 7, 7).with_custom_background({0, 140, 0, 255}).with_ignore_pointer_events());
    label(c, p, 53, "Direct Connection Established (local demo)", 28, h - 31, w - 42, 22, 12, ink, "aim_connection");
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
    case Dialog::Fonts: heading = "Message formatting"; body = "Use Aa to change composing size, B for bold, and the underlined A to cycle blue, red, and green sender colors. Italic and underline are unavailable with the bundled fonts."; break;
    case Dialog::Clear: heading = "Clear conversation?"; body = "Remove the displayed messages from this local conversation. Your unsent draft will be kept."; break;
    case Dialog::File: heading = "File"; break;
    case Dialog::Edit: heading = "Edit"; break;
    case Dialog::Insert: heading = "Insert"; break;
    case Dialog::People: heading = "People"; break;
    case Dialog::Start: heading = "Start"; break;
    default: break;
    }
    bool open = dialog != Dialog::None;
    auto panel = afterhours::modal(c, mk(root, 701), open, afterhours::ModalConfig{}
        .with_backdrop_color({0, 0, 0, 65}).with_render_layer(11)
        .with_show_close_button(false)
        .with_panel(box(405, 232, 470, menu ? 242 : 256)
            .with_custom_background(face)
            .with_on_draw_fg([s = scale](RectangleType r) { bevel(r, false, s); })
            .with_debug_name("aim_dialog")));
    if (!open) { dialog = Dialog::None; return; }
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
