#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/ui/text_input/text_input.h>
#include <algorithm>
#include <array>
#include <cctype>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct TextInputDemo : ScreenSystem<UIContext<InputAction>> {
  struct Profile {
    std::string username, email, password, bio;
    bool operator==(const Profile &) const = default;
  } profile, saved_profile;
  std::string search_query;
  std::string readonly_text = "Account ID: demo-1042";
  std::string disabled_text = "Managed by workspace";
  bool show_password = false;
  std::string status_message;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = afterhours::ui::theme_presets::midnight();
    theme.secondary = {32, 34, 46, 255};
    context.theme = theme;
    context.scaling_mode = ScalingMode::Proportional;
    const float s = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const float left = (context.screen_width - 1280 * s) / 2;
    const float top = (context.screen_height - 720 * s) / 2;
    const afterhours::Color field_bg{32, 34, 46, 255};
    const afterhours::Color field_border{113, 124, 151, 255};
    const afterhours::Color card_bg{20, 23, 33, 255};
    const afterhours::Color muted{182, 194, 217, 255};
    const afterhours::Color white{242, 244, 250, 255};
    UIStylingDefaults::get().set_default_font("AtkinsonMock", pixels(20 * s));
    div(context, mk(entity, 900), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_absolute_position(0, 0).with_custom_background({10, 12, 18, 255}).with_corner_radius(0));
    auto root = div(context, mk(entity, 0), ComponentConfig{}
        .with_size({pixels(1280 * s), pixels(720 * s)})
        .with_absolute_position(left, top).with_transparent_bg().with_skip_grid_snap(true)
        .with_debug_name("text_input_root"));
    auto box = [&](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, y * s).with_transparent_bg()
          .with_corner_radius(0).with_skip_grid_snap(true);
    };
    auto text = [&](int id, std::string value, float x, float y, float w, float h,
                    float size, afterhours::Color color, const char *debug = "", bool bold = false) {
      div(context, mk(root.ent(), id), box(x, y, w, h).with_label(value)
          .with_font(bold ? "AtkinsonMockBold" : "AtkinsonMock", pixels(size * s))
          .with_alignment(TextAlignment::Left).with_custom_text_color(color)
          .with_ignore_pointer_events().with_debug_name(debug));
    };
    auto input_config = [&](float x, float y, float w, const char *placeholder, const char *debug) {
      return box(x, y, w, 44).with_custom_background(field_bg).with_border(field_border, 1.f)
          .with_font("AtkinsonMock", pixels(22 * s)).with_placeholder(placeholder).with_debug_name(debug);
    };
    // Title
    text(1, "Create profile", 48, 20, 1184, 48, 38, white, "text_input_title", true);
    // Subtitle
    text(2, "Try native account inputs. This demo stores values only in memory.", 48, 78, 1184, 30, 22, muted);
    div(context, mk(root.ent(), 3), box(48, 124, 572, 550).with_custom_background(card_bg).with_corner_radius(12 * s).with_debug_name("card"));
    div(context, mk(root.ent(), 4), box(644, 124, 588, 550).with_custom_background(card_bg).with_corner_radius(12 * s).with_debug_name("input_states"));
    text(5, "All profile fields are optional. No format validation.", 72, 137, 524, 26, 18, muted);
    const auto field = [&](int id, const char *label, const char *placeholder, std::string &value,
                           float label_y, float input_y, const char *debug) {
      text(100 + id, label, 72, label_y, 524, 28, 20, white);
      return text_input(context, mk(root.ent(), 200 + id), value, input_config(72, input_y, 524, placeholder, debug));
    };
    if (field(0, "Username", "Enter username", profile.username, 174, 202, "Username_input"))
      status_message = "Username: " + profile.username;
    if (field(1, "Email", "Enter email address", profile.email, 264, 292, "Email_input"))
      status_message = "Email: " + profile.email;
    text(6, "Example: name@example.com", 72, 339, 524, 25, 17, muted);
    text(7, "New password / optional", 72, 379, 524, 28, 20, white);
    auto pw_row = div(context, mk(root.ent(), 8), box(72, 407, 524, 44)
        .with_custom_background(field_bg).with_border(field_border, 1.f).with_debug_name("pw_row"));
    auto pw_cfg = box(0, 0, 436, 44).with_font("AtkinsonMock", pixels(22 * s))
        .with_placeholder("Enter a demo password").without_border().with_debug_name("Password_input");
    if (!show_password) pw_cfg.with_mask_char('*');
    if (text_input(context, mk(pw_row.ent(), 0), profile.password, pw_cfg)) status_message = "Password changed";
    if (button(context, mk(pw_row.ent(), 1), box(436, 0, 88, 44)
        .with_label(show_password ? "Hide" : "Show").with_alignment(TextAlignment::Right)
        .with_custom_background({73, 82, 108, 255}).with_custom_text_color(white)
        .with_font("AtkinsonMock", pixels(20 * s)).with_debug_name("pw_toggle")
        .with_on_draw_fg([s, white](RectangleType r) {
          raylib::DrawEllipseLines(static_cast<int>(r.x + 18 * s), static_cast<int>(r.y + 22 * s), 9 * s, 5 * s, white);
          raylib::DrawCircleV({r.x + 18 * s, r.y + 22 * s}, 2 * s, white);
        }))) show_password = !show_password;
    text(9, "Blank means no demo password. No account is created.", 72, 454, 524, 25, 17, muted);
    // Bio
    if (field(3, "Short bio / optional", "Write a short introduction", profile.bio, 493, 521, "Bio_input"))
      status_message = "Bio: " + profile.bio;
    // Search
    text(10, "Input states demo", 668, 137, 540, 34, 28, white, "", true);
    text(11, "Search demo profiles", 668, 180, 540, 28, 20, white);
    div(context, mk(root.ent(), 12), box(668, 213, 36, 44).with_custom_background(field_bg)
        .with_on_draw_fg([s, muted](RectangleType r) {
          raylib::DrawCircleLines(static_cast<int>(r.x + 15 * s), static_cast<int>(r.y + 19 * s), 7 * s, muted);
          raylib::DrawLineEx({r.x + 20 * s, r.y + 24 * s}, {r.x + 27 * s, r.y + 31 * s}, 2 * s, muted);
        }));
    if (text_input(context, mk(root.ent(), 13), search_query,
        input_config(704, 213, 504, "Type a name to filter", "Search_input"))) status_message = "Search: " + search_query;
    std::string folded = search_query;
    std::transform(folded.begin(), folded.end(), folded.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    const std::array<std::string, 3> names{"Alex Rivera", "Morgan Chen", "Sam Taylor"};
    std::string matches;
    for (const auto &name : names) {
      std::string candidate = name;
      std::transform(candidate.begin(), candidate.end(), candidate.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
      if (candidate.find(folded) == std::string::npos) continue;
      if (!matches.empty()) matches += ", ";
      matches += name;
    }
    text(14, matches.empty() ? "No matching demo profiles" : matches, 668, 266, 540, 26, 18, muted, "search_results");
    // Readonly field
    text(15, "Account identifier / read-only", 668, 316, 540, 28, 20, white);
    text_input(context, mk(root.ent(), 16), readonly_text,
        input_config(668, 347, 540, "", "Readonly_input").with_readonly());
    text(17, "You can select and copy this fixed demo value.", 668, 400, 540, 26, 18, muted);
    // Disabled field
    text(18, "Workspace / disabled", 668, 449, 540, 28, 20, white);
    text_input(context, mk(root.ent(), 19), disabled_text,
        input_config(668, 480, 540, "", "Disabled_input").with_disabled(true));
    text(20, "Disabled fixture: keyboard and pointer editing are off.", 668, 533, 540, 26, 18, muted);
    text(21, "Field borders are square for a consistent join.", 668, 585, 540, 26, 18, muted);
    text(22, profile == saved_profile ? "No unsaved profile changes" : "Unsaved demo profile changes",
         72, 575, 524, 26, 18, muted, "profile_changes");
    if (button(context, mk(root.ent(), 30), box(344, 618, 118, 40).with_label("Cancel")
        .with_font("AtkinsonMock", pixels(21 * s)).with_custom_background(card_bg)
        .with_custom_text_color(white).with_border(field_border, 1.f).with_debug_name("btn_cancel"))) {
      profile = saved_profile;
      search_query.clear();
      show_password = false;
      status_message = "Restored saved demo profile";
    }
    if (button(context, mk(root.ent(), 31), box(478, 618, 118, 40).with_label("Save demo")
        .with_font("AtkinsonMock", pixels(21 * s)).with_custom_background(theme.accent)
        .with_custom_text_color(theme.darkfont).with_debug_name("btn_save"))) {
      saved_profile = profile;
      status_message = "Saved: " + profile.username + " / " + profile.email;
    }
    div(context, mk(root.ent(), 32), box(668, 624, 540, 30).with_label(status_message.empty() ? "Demo form / ready" : status_message)
        .with_font("AtkinsonMock", pixels(19 * s)).with_alignment(TextAlignment::Left)
        .with_custom_text_color(white).with_text_overflow(TextOverflow::Ellipsis).with_debug_name("status_bar"));
    text(33, "Tab: next field / Shift+Tab: previous / Ctrl or Cmd: A select all, C copy, V paste, X cut",
         48, 686, 1184, 26, 18, muted, "kbd_hints");
  }
};

REGISTER_EXAMPLE_SCREEN(text_input, "Component Galleries",
                        "Text input fields with cursor and keyboard support",
                        TextInputDemo)
