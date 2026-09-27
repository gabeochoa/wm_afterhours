#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/ui/text_input/text_input.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Pseudo-localization against a whole app: a fake social feed dense
// enough that longer and right-to-left copy has nowhere to hide. The
// mode switch rebuilds every label in the app at once; likes, requests
// and the composer still work in every language.
struct PseudoLocaleLab : ScreenSystem<UIContext<InputAction>> {
  struct Post {
    int id;
    std::string author;
    std::string when;
    std::string text;
    int likes;
    int comments;
    int shares;
  };

  PseudoLocale mode = PseudoLocale::None;
  std::string draft;
  std::string search_text;
  std::set<int> liked;
  std::set<int> confirmed;
  std::set<int> dismissed;
  int next_post_id = 1000;
  std::vector<Post> mine;
  bool following_market = false;

  std::vector<Post> feed() const {
    std::vector<Post> all = mine;
    all.push_back({1, "Maya Chen", "2 hrs",
                   "We finally shipped the community garden signup sheet. "
                   "Twenty families already claimed a plot for spring, and "
                   "the waitlist is open if your block wants in.",
                   48, 12, 4});
    all.push_back({2, "The Night Market", "5 hrs",
                   "Saturday lineup is set: fourteen food stalls, two "
                   "stages, and the lantern parade at nine. Bring cash for "
                   "the dumpling stand, it sells out every single week.",
                   212, 46, 31});
    all.push_back({3, "Tom Okafor", "Yesterday",
                   "Marathon training, week nine. The long run is done, "
                   "the knees have filed a formal complaint, and the "
                   "group photo is mostly everyone lying on the grass.",
                   96, 23, 2});
    return all;
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = afterhours::ui::theme_presets::neon_dark();
    context.theme = theme;
    context.scaling_mode = ScalingMode::Proportional;
    UIStylingDefaults::get().set_grid_snapping(false);
    UIStylingDefaults::get().set_pseudo_locale(mode);
    const float scale = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    UIStylingDefaults::get().set_default_font("AtkinsonMock", pixels(20 * scale));

    const afterhours::Color ink{233, 238, 248, 255}, muted{158, 174, 199, 255};
    const afterhours::Color app_bg{16, 22, 34, 255}, panel{27, 37, 53, 255};
    const afterhours::Color panel_hi{35, 47, 66, 255}, hairline{52, 66, 88, 255};
    const afterhours::Color brand{80, 134, 246, 255}, green{62, 186, 110, 255};

    auto root = div(context, mk(entity), ComponentConfig{}
        .with_size({screen_pct(1), screen_pct(1)}).with_background(Theme::Usage::Background)
        .with_corner_radius(0).with_padding(Padding::all(w1280(16))).with_debug_name("pll_root"));
    auto abs = [&](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * scale), pixels(h * scale)})
          .with_absolute_position(x * scale, y * scale);
    };
    auto text = [&](int id, const std::string &label, float x, float y, float w, float h,
                    float size, afterhours::Color color = afterhours::Color{233, 238, 248, 255}, bool bold = false) {
      return div(context, mk(root.ent(), id), abs(x, y, w, h).with_label(label)
          .with_font(bold ? "AtkinsonMockBold" : "AtkinsonMock", pixels(size * scale))
          .with_custom_text_color(color).with_text_overflow(TextOverflow::Wrap)
          .with_ignore_pointer_events());
    };
    auto btn = [&](int id, const std::string &label, float x, float y, float w, float h,
                   afterhours::Color bg, afterhours::Color fg, const char *debug,
                   afterhours::Entity &parent) {
      return button(context, mk(parent, id), abs(x, y, w, h).with_label(label)
          .with_font("AtkinsonMock", pixels(17 * scale)).with_custom_background(bg)
          .with_custom_text_color(fg).with_corner_radius(6 * scale)
          .with_debug_name(debug));
    };

    // No image assets in the lab: people and pictures are shapes,
    // colours and patterns. A person gets a colour from their name and
    // their initials in a circle; a story gets its person's colour.
    auto person_color = [](const std::string &name) {
      static constexpr afterhours::Color palette[] = {
          {219, 118, 96, 255},  {96, 165, 250, 255},  {62, 186, 110, 255},
          {240, 190, 70, 255},  {186, 130, 245, 255}, {245, 140, 180, 255}};
      size_t h = 0;
      for (char c : name)
        h = h * 31 + static_cast<unsigned char>(c);
      return palette[h % 6];
    };
    auto initials = [](const std::string &name) {
      std::string out;
      if (!name.empty())
        out += name.front();
      if (const size_t sp = name.find(' ');
          sp != std::string::npos && sp + 1 < name.size())
        out += name[sp + 1];
      return out;
    };
    auto avatar = [&](afterhours::Entity &parent, int id, float x, float y,
                      float size, const std::string &name) {
      auto a = div(context, mk(parent, id), ComponentConfig{}
          .with_size({pixels(size * scale), pixels(size * scale)})
          .with_absolute_position(x * scale, y * scale)
          .with_custom_background(person_color(name))
          .with_corner_radius(size / 2 * scale));
      div(context, mk(a.ent(), 0), ComponentConfig{}
          .with_size({pixels(size * scale), pixels(size * scale)})
          .with_absolute_position(0, 0).with_label(initials(name))
          .with_font("AtkinsonMockBold", pixels(size * 0.4f * scale))
          .with_custom_text_color(afterhours::Color{16, 21, 31, 255})
          .with_alignment(TextAlignment::Center).with_ignore_pointer_events());
    };
    auto avatar_flow = [&](afterhours::Entity &parent, int id, float size,
                           const std::string &name) {
      auto a = div(context, mk(parent, id), ComponentConfig{}
          .with_size({pixels(size * scale), pixels(size * scale)})
          .with_custom_background(person_color(name))
          .with_corner_radius(size / 2 * scale));
      div(context, mk(a.ent(), 0), ComponentConfig{}
          .with_size({pixels(size * scale), pixels(size * scale)})
          .with_absolute_position(0, 0).with_label(initials(name))
          .with_font("AtkinsonMockBold", pixels(size * 0.4f * scale))
          .with_custom_text_color(afterhours::Color{16, 21, 31, 255})
          .with_alignment(TextAlignment::Center).with_ignore_pointer_events());
    };
    auto btn_flow = [&](int id, const std::string &label, float w, float h,
                        afterhours::Color bg, afterhours::Color fg,
                        const char *debug, afterhours::Entity &parent) {
      return button(context, mk(parent, id), ComponentConfig{}
          .with_size({pixels(w * scale), pixels(h * scale)}).with_label(label)
          .with_font("AtkinsonMock", pixels(17 * scale)).with_custom_background(bg)
          .with_custom_text_color(fg).with_corner_radius(6 * scale)
          .with_debug_name(debug));
    };

    text(0, "Pseudo-locale lab", 16, 0, 500, 36, 26, ink, true);
    auto mode_btn = [&](int id, const char *label, float x, PseudoLocale want,
                        const char *debug) {
      const bool active = mode == want;
      if (button(context, mk(root.ent(), id), abs(x, 44, 170, 34).with_label(label)
              .with_font("AtkinsonMock", pixels(16 * scale))
              .with_custom_background(active ? brand : panel_hi)
              .with_custom_text_color(active ? afterhours::Color{10, 16, 28, 255} : ink)
              .with_corner_radius(6 * scale).with_debug_name(debug)))
        mode = want;
    };
    mode_btn(2, "Off", 16, PseudoLocale::None, "pl_off");
    mode_btn(3, "Double words", 198, PseudoLocale::DoubleWords, "pl_double");
    mode_btn(4, "RTL words", 380, PseudoLocale::RtlWords, "pl_rtl");
    text(5, "A whole app in the fake language: feed, sidebars, composer, buttons.",
         580, 48, 660, 30, 16, muted);

    // The app frame.
    auto frame = div(context, mk(root.ent(), 10), abs(16, 92, 1228, 612)
        .with_custom_background(app_bg).with_corner_radius(10 * scale));
    auto &f = frame.ent();

    // Top bar.
    auto bar = div(context, mk(f, 11), abs(0, 0, 1228, 54)
        .with_custom_background(panel).with_corner_radius(10 * scale));
    div(context, mk(root.ent(), 12), abs(40, 104, 170, 32).with_label("friendface")
          .with_font("AtkinsonMockBold", pixels(24 * scale)).with_custom_text_color(brand)
          .with_alignment(TextAlignment::Left).with_ignore_pointer_events());
    auto search = text_input(context, mk(bar.ent(), 13), search_text,
        ComponentConfig{}.with_size({pixels(230 * scale), pixels(36 * scale)})
            .with_absolute_position(180 * scale, 9 * scale)
            .with_placeholder("Search friendface")
            .with_font("AtkinsonMock", pixels(16 * scale))
            .with_custom_background(panel_hi).with_corner_radius(18 * scale)
            .with_debug_name("pl_search"));
    (void)search;
    const char *navs[] = {"Home", "Watch", "Groups", "Games"};
    for (int i = 0; i < 4; i++)
      btn(14 + i, navs[i], 454.f + i * 118.f, 9, 106, 36,
          i == 0 ? panel_hi : panel, i == 0 ? ink : muted,
          fmt::format("pl_nav_{}", i).c_str(), bar.ent());
    btn(18, "Alerts", 1080, 9, 84, 36, panel_hi, ink, "pl_alerts", bar.ent());
    div(context, mk(bar.ent(), 19), abs(1152, 5, 20, 20)
        .with_custom_background(afterhours::Color{224, 84, 84, 255})
        .with_corner_radius(10 * scale).with_label("3")
        .with_font("AtkinsonMock", pixels(12 * scale))
        .with_custom_text_color(afterhours::Color{255, 255, 255, 255})
        .with_alignment(TextAlignment::Center).with_ignore_pointer_events());
    btn(20, "Gabe", 1176, 9, 44, 36, brand, afterhours::Color{10, 16, 28, 255},
        "pl_profile", bar.ent());

    // Left sidebar.
    auto side = div(context, mk(f, 21), abs(16, 70, 232, 520));
    auto &s = side.ent();
    auto side_row = [&](int id, float y, const std::string &label, bool bold = false,
                        afterhours::Color color = afterhours::Color{233, 238, 248, 255}) {
      return div(context, mk(s, id), ComponentConfig{}
          .with_size({pixels(232 * scale), pixels(38 * scale)})
          .with_absolute_position(0, y * scale).with_label(label)
          .with_font(bold ? "AtkinsonMockBold" : "AtkinsonMock", pixels(17 * scale))
          .with_custom_text_color(color)
          .with_padding(Padding{.left = pixels(12 * scale)})
          .with_corner_radius(6 * scale));
    };
    avatar(s, 61, 2, 3, 32, "Gabe Ochoa");
    div(context, mk(s, 62), ComponentConfig{}
        .with_size({pixels(180 * scale), pixels(26 * scale)})
        .with_absolute_position(44 * scale, 6 * scale).with_label("Gabe Ochoa")
        .with_font("AtkinsonMockBold", pixels(17 * scale)).with_custom_text_color(ink)
        .with_ignore_pointer_events());
    const char *items[] = {"Friends", "Memories", "Saved items", "Groups",
                           "Watch later", "Feeds", "Events", "Birthdays"};
    for (int i = 0; i < 8; i++)
      side_row(23 + i, 46.f + i * 42.f, items[i]);
    text(31, "Your shortcuts", 44, 552, 220, 26, 15, muted, true);
    side_row(32, 422, "Weekend Hikers");
    side_row(33, 464, "Analog Photography");

    // Right sidebar.
    auto right = div(context, mk(f, 70), abs(932, 70, 280, 520));
    auto &r = right.ent();
    text(71, "Friend requests", 948, 162, 240, 28, 18, ink, true);
    struct Req { int id; const char *name; const char *mutual; };
    const Req reqs[] = {{72, "Priya Nair", "12 mutual friends"},
                        {75, "Leo Martins", "3 mutual friends"}};
    float ry = 198;
    for (const auto &q : reqs) {
      if (dismissed.count(q.id)) { ry += 76; continue; }
      avatar(r, q.id, 0, ry - 162, 40, q.name);
      text(q.id + 100, q.name, 1000, ry, 220, 24, 16, ink, true);
      text(q.id + 200, q.mutual, 1000, ry + 24, 220, 22, 14, muted);
      if (confirmed.count(q.id)) {
        text(q.id + 300, "Request confirmed", 1000, ry + 46, 220, 24, 14, green);
      } else {
        if (btn(q.id + 1, "Confirm", 52, ry + 46 - 162, 100, 28, brand,
                afterhours::Color{10, 16, 28, 255},
                fmt::format("pl_confirm_{}", q.id).c_str(), r))
          confirmed.insert(q.id);
        if (btn(q.id + 2, "Delete", 160, ry + 46 - 162, 84, 28, panel_hi, ink,
                fmt::format("pl_delete_{}", q.id).c_str(), r))
          dismissed.insert(q.id);
      }
      ry += 76;
    }
    text(80, "Contacts", 948, 366, 200, 28, 18, ink, true);
    const char *contacts[] = {"Maya Chen", "Tom Okafor", "Priya Nair",
                              "Leo Martins", "Ana Ruiz", "Sam Whitaker"};
    for (int i = 0; i < 6; i++) {
      const float cy = 402.f + i * 38.f;
      auto crow = hstack(context, mk(root.ent(), 81 + i), ComponentConfig{}
          .with_size({pixels(280 * scale), pixels(26 * scale)})
          .with_absolute_position(948 * scale, cy * scale)
          .with_align_items(AlignItems::Center).with_gap(pixels(10 * scale)));
      div(context, mk(crow.ent(), 0), ComponentConfig{}
          .with_size({pixels(9 * scale), pixels(9 * scale)})
          .with_custom_background(green).with_corner_radius(5 * scale));
      div(context, mk(crow.ent(), 1), ComponentConfig{}
          .with_size({pixels(240 * scale), pixels(26 * scale)})
          .with_label(contacts[i])
          .with_font("AtkinsonMock", pixels(16 * scale)).with_custom_text_color(ink)
          .with_ignore_pointer_events());
    }
    auto sponsored = div(context, mk(r, 96), abs(0, 476, 280, 52)
        .with_custom_background(panel).with_corner_radius(8 * scale));
    div(context, mk(sponsored.ent(), 0), ComponentConfig{}
        .with_size({pixels(52 * scale), pixels(34 * scale)})
        .with_absolute_position(10 * scale, 9 * scale)
        .with_custom_background(afterhours::Color{146, 96, 214, 255})
        .with_corner_radius(6 * scale));
    for (int stripe = 0; stripe < 2; stripe++)
      div(context, mk(sponsored.ent(), 1 + stripe), ComponentConfig{}
          .with_size({pixels(52 * scale), pixels(5 * scale)})
          .with_absolute_position(10 * scale, (17.f + stripe * 11.f) * scale)
          .with_custom_background(afterhours::Color{255, 255, 255, 70}));
    div(context, mk(sponsored.ent(), 3), ComponentConfig{}
        .with_size({pixels(16 * scale), pixels(16 * scale)})
        .with_absolute_position(38 * scale, 18 * scale)
        .with_custom_background(afterhours::Color{240, 190, 70, 255})
        .with_corner_radius(8 * scale));
    text(97, "Sponsored: Night Market tickets are on sale now.", 1024, 644, 196, 44, 13, muted);

    // Feed column (scrolls).
    auto feed_col = div(context, mk(f, 50), ComponentConfig{}
        .with_size({pixels(600 * scale), pixels(542 * scale)})
        .with_absolute_position(280 * scale, 70 * scale)
        .with_flex_direction(FlexDirection::Column)
        .with_gap(pixels(12 * scale))
        .with_overflow(Overflow::Scroll, Axis::Y)
        .with_debug_name("pl_feed"));
    auto &fd = feed_col.ent();
    auto stories = hstack(context, mk(fd, 51), ComponentConfig{}
        .with_size({pixels(584 * scale), pixels(140 * scale)}).with_gap(pixels(8 * scale)));
    const char *story_names[] = {"Ana Ruiz", "Sam W.", "Priya Nair", "Leo M."};
    for (int i = 0; i < 4; i++) {
      auto card = div(context, mk(stories.ent(), 52 + i), ComponentConfig{}
          .with_size({pixels(140 * scale), pixels(140 * scale)})
          .with_custom_background(person_color(story_names[i]))
          .with_corner_radius(10 * scale));
      div(context, mk(card.ent(), 1), ComponentConfig{}
          .with_size({pixels(64 * scale), pixels(64 * scale)})
          .with_absolute_position(38 * scale, 16 * scale)
          .with_custom_background(afterhours::Color{255, 255, 255, 46})
          .with_corner_radius(32 * scale));
      avatar(card.ent(), 2, 50, 76, 40, story_names[i]);
      div(context, mk(card.ent(), 3), ComponentConfig{}
          .with_size({pixels(140 * scale), pixels(28 * scale)})
          .with_absolute_position(0, 112 * scale)
          .with_custom_background(afterhours::Color{12, 17, 26, 190})
          .with_label(story_names[i])
          .with_font("AtkinsonMock", pixels(14 * scale)).with_custom_text_color(ink)
          .with_alignment(TextAlignment::Center).with_ignore_pointer_events());
    }
    auto composer = div(context, mk(fd, 70), ComponentConfig{}
        .with_size({pixels(600 * scale), pixels(108 * scale)})
        .with_custom_background(panel).with_corner_radius(10 * scale));
    avatar(composer.ent(), 74, 14, 14, 40, "Gabe Ochoa");
    auto draft_field = text_input(context, mk(composer.ent(), 71), draft,
        ComponentConfig{}.with_size({pixels(404 * scale), pixels(40 * scale)})
            .with_absolute_position(64 * scale, 14 * scale)
            .with_placeholder("What's on your mind, Gabe?")
            .with_font("AtkinsonMock", pixels(16 * scale))
            .with_custom_background(panel_hi).with_corner_radius(20 * scale)
            .with_debug_name("pl_composer"));
    if (btn(72, "Post", 484, 14, 100, 40, brand, afterhours::Color{10, 16, 28, 255},
            "pl_post", composer.ent())) {
      auto &state = draft_field.ent().get<afterhours::text_input::HasTextInputState>();
      const std::string body = state.text();
      if (!body.empty()) {
        mine.insert(mine.begin(), Post{next_post_id++, "Gabe Ochoa", "Just now",
                                       body, 0, 0, 0});
        state.storage.clear();
        draft.clear();
      }
    }
    div(context, mk(composer.ent(), 73), ComponentConfig{}
        .with_size({pixels(568 * scale), pixels(30 * scale)})
        .with_absolute_position(16 * scale, 66 * scale)
        .with_label("Live video      Photo or video      Feeling or activity")
        .with_font("AtkinsonMock", pixels(15 * scale)).with_custom_text_color(muted)
        .with_alignment(TextAlignment::Center).with_ignore_pointer_events());

    int pid = 200;
    for (const auto &post : feed()) {
      const int lines = std::min(4, 1 + static_cast<int>(post.text.size()) / 62);
      const float text_h = static_cast<float>(lines) * 23.f;
      const float panel_h = 52.f + text_h + 30.f + 44.f + 16.f;
      auto card = div(context, mk(fd, pid), ComponentConfig{}
          .with_size({pixels(600 * scale), pixels(panel_h * scale)})
          .with_custom_background(panel).with_corner_radius(10 * scale));
      auto &c = card.ent();
      auto header = hstack(context, mk(c, pid + 1), ComponentConfig{}
          .with_size({pixels(572 * scale), pixels(38 * scale)})
          .with_absolute_position(14 * scale, 10 * scale)
          .with_gap(pixels(10 * scale)));
      avatar_flow(header.ent(), 0, 38, post.author);
      auto who = div(context, mk(header.ent(), 1), ComponentConfig{}
          .with_size({pixels(430 * scale), pixels(38 * scale)})
          .with_flex_direction(FlexDirection::Column));
      div(context, mk(who.ent(), 0), ComponentConfig{}
          .with_size({pixels(430 * scale), pixels(22 * scale)}).with_label(post.author)
          .with_font("AtkinsonMockBold", pixels(16 * scale)).with_custom_text_color(ink)
          .with_ignore_pointer_events());
      div(context, mk(who.ent(), 1), ComponentConfig{}
          .with_size({pixels(430 * scale), pixels(16 * scale)}).with_label(post.when)
          .with_font("AtkinsonMock", pixels(13 * scale)).with_custom_text_color(muted)
          .with_ignore_pointer_events());
      div(context, mk(c, pid + 4), ComponentConfig{}
          .with_size({pixels(572 * scale), pixels(text_h * scale)})
          .with_absolute_position(14 * scale, 54 * scale).with_label(post.text)
          .with_font("AtkinsonMock", pixels(16 * scale)).with_custom_text_color(ink)
          .with_text_overflow(TextOverflow::Wrap).with_ignore_pointer_events());
      const bool is_liked = liked.count(post.id) > 0;
      auto counts = hstack(context, mk(c, pid + 5), ComponentConfig{}
          .with_size({pixels(572 * scale), pixels(22 * scale)})
          .with_absolute_position(14 * scale, (58 + text_h) * scale));
      div(context, mk(counts.ent(), 0), ComponentConfig{}
          .with_size({pixels(200 * scale), pixels(22 * scale)})
          .with_label(fmt::format("{} likes", post.likes + (is_liked ? 1 : 0)))
          .with_font("AtkinsonMock", pixels(14 * scale)).with_custom_text_color(muted)
          .with_ignore_pointer_events());
      spacer(context, mk(counts.ent(), 1));
      div(context, mk(counts.ent(), 2), ComponentConfig{}
          .with_size({pixels(260 * scale), pixels(22 * scale)})
          .with_label(fmt::format("{} comments \u00b7 {} shares", post.comments, post.shares))
          .with_font("AtkinsonMock", pixels(14 * scale)).with_custom_text_color(muted)
          .with_alignment(TextAlignment::Right).with_ignore_pointer_events());
      const float bar_y = 88.f + text_h;
      auto bar = hstack(context, mk(c, pid + 6), ComponentConfig{}
          .with_size({pixels(572 * scale), pixels(36 * scale)})
          .with_absolute_position(14 * scale, bar_y * scale)
          .with_gap(pixels(8 * scale)));
      if (btn_flow(pid + 7, is_liked ? "Liked" : "Like", 185, 36,
                   is_liked ? brand : panel, is_liked ? afterhours::Color{10, 16, 28, 255} : ink,
                   fmt::format("pl_like_{}", post.id).c_str(), bar.ent())) {
        if (is_liked) liked.erase(post.id);
        else liked.insert(post.id);
      }
      btn_flow(pid + 8, "Comment", 185, 36, panel, ink,
               fmt::format("pl_comment_{}", post.id).c_str(), bar.ent());
      btn_flow(pid + 9, "Share", 185, 36, panel, ink,
               fmt::format("pl_share_{}", post.id).c_str(), bar.ent());
      pid += 20;
    }
  }
};

REGISTER_EXAMPLE_SCREEN(pseudo_locale_lab, "System Demos",
                        "pseudo-localization over a fake social app: double-words and RTL-words stress",
                        PseudoLocaleLab)
