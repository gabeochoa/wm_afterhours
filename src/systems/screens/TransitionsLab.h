#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/modal.h>
#include <array>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct TransitionsLab : ScreenSystem<UIContext<InputAction>> {
  std::shared_ptr<int> alive = std::make_shared<int>(0);
  struct Example {
    const char *slug;
    const char *title;
  };
  struct Group {
    const char *slug;
    const char *title;
    std::vector<Example> examples;
  };
  const std::vector<Group> groups{
      {"controls", "Controls", {{"checkbox", "Checkbox check"}, {"toggle", "Toggle"}, {"like", "Like button"}, {"icon_swap", "Icon swap"}, {"learn_more", "Learn more hover"}, {"error_shake", "Error state shake"}, {"input_clear", "Input clear"}, {"text_swap", "Text states swap"}}},
      {"navigation", "Navigation and overlays", {{"menu", "Menu dropdown"}, {"tooltip", "Tooltip"}, {"modal", "Modal open/close"}, {"panel", "Panel reveal"}, {"page", "Page side-by-side"}, {"tabs", "Tabs sliding"}, {"accordion", "Accordion"}, {"morph", "Dropdown menu morph"}}},
      {"status", "Status and loading", {{"toast", "Toast open/close"}, {"badge", "Notification badge"}}},
      {"cards", "Cards and text", {}},
      {"effects", "Visual effects", {}},
  };

  size_t group = 0;
  std::array<size_t, 5> example{};
  size_t replay_stamp = 0;
  size_t seen_replay = 0;

  bool checked = false;
  bool toggled = false;
  bool liked = false;
  int likes = 0;
  bool icon_b = false;
  bool error = false;
  int submits = 0;
  bool cleared = false;
  int swap_index = 0;
  bool menu_open = false;
  std::string menu_choice = "none";
  int tooltip_trigger = -1;
  bool modal_open = false;
  bool panel_open = false;
  bool page_two = false;
  int tab_index = 0;
  bool accordion_open = false;
  bool morph_open = false;
  bool toast_shown = false;
  int toasts = 0;
  bool badge_shown = false;

  enum struct Key : size_t { CheckDraw, LikeFill, LearnShift, LearnSpread, Shake, ErrorHold, TooltipX, PillX, PillW, AccHeight, MorphW, MorphH, ToastHold };

  void reset_examples() {
    checked = false;
    toggled = false;
    liked = false;
    likes = 0;
    icon_b = false;
    error = false;
    submits = 0;
    cleared = false;
    swap_index = 0;
    menu_open = false;
    menu_choice = "none";
    tooltip_trigger = -1;
    modal_open = false;
    panel_open = false;
    page_two = false;
    tab_index = 0;
    accordion_open = false;
    morph_open = false;
    toast_shown = false;
    toasts = 0;
    badge_shown = false;
    afterhours::motion::anim(Key::ToastHold).from(0.f);
    afterhours::motion::anim(Key::CheckDraw).from(0.f);
    afterhours::motion::anim(Key::Shake).from(0.f);
    afterhours::motion::anim(Key::ErrorHold).from(0.f);
    afterhours::motion::anim<ColorType>(Key::LikeFill).from(ColorType{170, 165, 175, 255});
  }

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context, float) override {
    namespace motion = afterhours::motion;
    context.theme = afterhours::ui::theme_presets::cozy_kraft();
    context.scaling_mode = ScalingMode::Proportional;
    const float s = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const afterhours::Color ink{55, 50, 60, 255}, muted{99, 87, 105, 255}, coral{255, 115, 105, 255},
        paper{255, 255, 255, 255}, accent{80, 120, 220, 255};
    const auto box = [s](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, y * s).with_corner_radius(0);
    };
    const auto label = [&](afterhours::Entity &parent, int id, const std::string &text, float x, float y, float w,
                           float size, bool dim = false, const std::string &debug = "") {
      return div(context, mk(parent, id), box(x, y, w, size * 1.4f).with_label(text)
          .with_font("AtkinsonMock", pixels(size * s)).with_custom_text_color(dim ? muted : ink)
          .with_alignment(TextAlignment::Left).with_background(Theme::Usage::None)
          .with_ignore_pointer_events().with_debug_name(debug));
    };
    const auto tab = [&](afterhours::Entity &parent, int id, const std::string &text, float x, float y, float w,
                         bool selected, const std::string &debug, float h = 40.f) {
      return button(context, mk(parent, id),
                    box(x, y, w, h).with_label(text).with_font("AtkinsonMock", pixels(h > 36.f ? 17 * s : 15 * s))
                        .with_custom_background(selected ? coral : afterhours::Color{244, 240, 246, 255})
                        .with_custom_text_color(ink).with_corner_radius(8 * s).with_debug_name(debug)
                        .on_hover({.scale = 1.03f}).on_press({.scale = 0.96f}));
    };

    div(context, mk(entity, 0), ComponentConfig{}.with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background({255, 245, 235, 255}).with_corner_radius(0).with_debug_name("bg"));
    label(entity, 1, "Transitions Lab", 40, 24, 600, 30);

    for (size_t g = 0; g < groups.size(); ++g)
      if (tab(entity, 10 + int(g), groups[g].title, 40 + g * 240.f, 60, 228, g == group, std::string("group_") + groups[g].slug))
        group = g;
    const Group &current = groups[group];
    if (example[group] >= current.examples.size()) example[group] = 0;
    for (size_t e = 0; e < current.examples.size(); ++e)
      if (tab(entity, 20 + int(e), current.examples[e].title, 40 + (e % 8) * 148.f, 106 + (e / 8) * 36.f, 140,
              e == example[group], std::string("ex_") + current.examples[e].slug, 32.f))
        example[group] = e;

    if (tab(entity, 40, "Replay", 40, 660, 120, false, "replay_btn")) ++replay_stamp;
    if (tab(entity, 41, "Reset", 172, 660, 120, false, "reset_btn")) reset_examples();
    bool reduced = Settings::get().get_reduced_motion_enabled();
    if (checkbox(context, mk(entity, 42), reduced,
                 box(320, 662, 260, 36).with_label("Reduced motion").with_font("AtkinsonMock", pixels(18 * s))
                     .with_custom_text_color(ink).with_debug_name("reduced_motion")))
      Settings::get().set_reduced_motion_enabled(reduced);

    auto stage = div(context, mk(entity, 50), box(40, 180, 1200, 460).with_custom_background(paper)
                                                  .with_corner_radius(16 * s).with_debug_name("stage"));
    if (current.examples.empty()) {
      label(stage.ent(), 0, "No examples in this group yet.", 24, 24, 600, 22, true);
      return;
    }
    const std::string slug = current.examples[example[group]].slug;
    label(stage.ent(), 0, current.examples[example[group]].title, 24, 20, 600, 26);

    bool in_flight = false;
    const auto track_motion = [&](afterhours::Entity &e) {
      if (!e.has<motion::HasTracks>()) return;
      auto &t = e.get<motion::HasTracks>();
      for (auto &[k, tr] : t.floats) in_flight |= tr.active();
      for (auto &[k, tr] : t.colors) in_flight |= tr.active();
    };

    if (slug == "checkbox") {
      auto &draw = motion::anim(Key::CheckDraw);
      const float p = draw.value();
      auto boxr = button(context, mk(stage.ent(), 1),
                         box(60, 90, 44, 44).with_corner_radius(10 * s).with_debug_name("check_box")
                             .with_custom_background(paper).with_border(muted, 2 * s)
                             .on_state(checked, {.background = {paper, accent}}, motion::Spring::snappy())
                             .on_press({.scale = 0.92f})
                             .with_on_draw_fg([p, s](RectangleType r) {
                               if (p <= 0.f) return;
                               const Vector2Type a{r.x + r.width * 0.26f, r.y + r.height * 0.54f};
                               const Vector2Type b{r.x + r.width * 0.43f, r.y + r.height * 0.72f};
                               const Vector2Type c{r.x + r.width * 0.76f, r.y + r.height * 0.30f};
                               const float l1 = std::hypot(b.x - a.x, b.y - a.y);
                               const float l2 = std::hypot(c.x - b.x, c.y - b.y);
                               const float total = (l1 + l2) * std::clamp(p, 0.f, 1.f);
                               const afterhours::Color white{255, 255, 255, 255};
                               if (total <= l1) {
                                 const float t = total / l1;
                                 afterhours::draw_line_ex(a, {a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t}, 3.f * s, white);
                               } else {
                                 afterhours::draw_line_ex(a, b, 3.f * s, white);
                                 const float t = (total - l1) / l2;
                                 afterhours::draw_line_ex(b, {b.x + (c.x - b.x) * t, b.y + (c.y - b.y) * t}, 3.f * s, white);
                               }
                             }));
      if (boxr) {
        checked = !checked;
        draw.to(checked ? 1.f : 0.f, checked ? motion::Mode{motion::Timeline{.keys = {{0.f, 0.f}, {0.35f, 1.f}},
                                                                             .curve = motion::curves::ease_out_quad}}
                                             : motion::Mode{motion::Timeline{.keys = {{0.f, 0.f}, {0.15f, 1.f}}}});
      }
      if (replay_stamp != seen_replay) {
        seen_replay = replay_stamp;
        if (checked)
          draw.from(0.f).to(1.f, motion::Timeline{.keys = {{0.f, 0.f}, {0.35f, 1.f}}, .curve = motion::curves::ease_out_quad});
      }
      track_motion(boxr.ent());
      in_flight |= draw.active();
      label(stage.ent(), 2, "Click the box. The mark draws in over 350 ms and retracts in 150 ms.", 130, 100, 900, 18, true);
      label(stage.ent(), 3, std::string("checked: ") + (checked ? "yes" : "no"), 60, 170, 400, 20, false, "check_state");
      label(stage.ent(), 4, "stroke: " + std::to_string(int(std::lround(p * 100.f))) + "%", 60, 200, 400, 20, false, "check_stroke");
    } else if (slug == "toggle") {
      const float travel = 24.f * s;
      auto trackr = button(context, mk(stage.ent(), 1),
                           box(60, 90, 56, 32).with_corner_radius(16 * s).with_debug_name("toggle_track")
                               .with_padding(Padding::all(pixels(0)))
                               .with_custom_background({200, 195, 205, 255})
                               .on_state(toggled, {.background = {afterhours::Color{200, 195, 205, 255}, accent}}, motion::Spring::smooth()));
      div(context, mk(trackr.ent(), 0),
          ComponentConfig{}.with_size({pixels(26 * s), pixels(26 * s)}).with_absolute_position(3 * s, 3 * s)
              .with_custom_background(paper).with_corner_radius(13 * s).with_debug_name("toggle_thumb")
              .with_ignore_pointer_events()
              .on_state(toggled, {.translate_x = {0.f, travel}}, motion::Spring::bouncy()));
      if (trackr) toggled = !toggled;
      track_motion(trackr.ent());
      for (auto child_id : trackr.cmp().children) {
        auto child = UICollectionHolder::getEntityForID(child_id);
        if (child.valid()) track_motion(child.asE());
      }
      label(stage.ent(), 2, "Click the switch. The thumb overshoots its stop and settles.", 130, 96, 900, 18, true);
      label(stage.ent(), 3, std::string("on: ") + (toggled ? "yes" : "no"), 60, 170, 400, 20, false, "toggle_state");
    }
    else if (slug == "like") {
      const afterhours::Color grey{170, 165, 175, 255}, red{244, 0, 81, 255};
      auto &fill = motion::anim<ColorType>(Key::LikeFill);
      if (!fill.started()) fill.from(grey);
      const afterhours::Color heart = fill.value();
      auto heart_btn = button(context, mk(stage.ent(), 1),
                              box(60, 84, 64, 64).with_debug_name("like_btn").with_padding(Padding::all(pixels(0)))
                                  .with_custom_background(afterhours::colors::transparent()).with_corner_radius(0)
                                  .on_change(static_cast<size_t>(likes), {.scale = {0.82f, 1.f}}, motion::Spring{.response = 0.35f, .bounce = 0.6f})
                                  .with_on_draw_fg([heart](RectangleType r) {
                                    const float w = r.width, h = r.height;
                                    afterhours::draw_circle_v({r.x + 0.3f * w, r.y + 0.36f * h}, 0.22f * w, heart);
                                    afterhours::draw_circle_v({r.x + 0.7f * w, r.y + 0.36f * h}, 0.22f * w, heart);
                                    afterhours::draw_triangle({r.x + 0.5f * w, r.y + 0.92f * h}, {r.x + 0.92f * w, r.y + 0.44f * h},
                                                              {r.x + 0.08f * w, r.y + 0.44f * h}, heart);
                                  }));
      if (heart_btn) {
        liked = !liked;
        if (liked) ++likes;
        fill.to(liked ? red : grey, motion::Timeline{.keys = {{0.f, 0.f}, {0.15f, 1.f}}});
      }
      track_motion(heart_btn.ent());
      in_flight |= fill.active();
      label(stage.ent(), 2, "Click the heart. Liking fills it and pops through a spring; unliking only drains the fill.", 150, 100, 1000, 18, true);
      label(stage.ent(), 3, std::string("liked: ") + (liked ? "yes" : "no"), 60, 170, 400, 20, false, "like_state");
      label(stage.ent(), 4, "likes: " + std::to_string(likes), 60, 200, 400, 20, false, "like_count");
    } else if (slug == "icon_swap") {
      const motion::Timeline swap{.keys = {{0.f, 0.f}, {0.25f, 1.f}}, .curve = motion::curves::ease_in_out_quad};
      auto cell = button(context, mk(stage.ent(), 1),
                         box(60, 84, 64, 64).with_debug_name("icon_swap").with_padding(Padding::all(pixels(0)))
                             .with_custom_background({244, 240, 246, 255}).with_corner_radius(12 * s));
      const auto glyph = [&](int id, const char *text, bool shown_when_b, const char *debug) {
        ComponentConfig cfg = ComponentConfig{}.with_size({pixels(64 * s), pixels(64 * s)}).with_absolute_position(0.f, 0.f)
                                  .with_label(text).with_font("AtkinsonMockBold", pixels(34 * s)).with_custom_text_color(ink)
                                  .with_alignment(TextAlignment::Center).with_background(Theme::Usage::None)
                                  .with_ignore_pointer_events().with_debug_name(debug);
        if (shown_when_b)
          cfg.on_appear({.scale = 0.25f, .opacity = 0.f}).on_state(icon_b, {.scale = {0.25f, 1.f}, .opacity = {0.f, 1.f}}, swap);
        else
          cfg.on_state(icon_b, {.scale = {1.f, 0.25f}, .opacity = {1.f, 0.f}}, swap);
        div(context, mk(cell.ent(), id), cfg);
      };
      glyph(0, "+", false, "icon_a");
      glyph(1, "x", true, "icon_b");
      if (cell) icon_b = !icon_b;
      for (auto child_id : cell.cmp().children) {
        auto child = UICollectionHolder::getEntityForID(child_id);
        if (child.valid()) track_motion(child.asE());
      }
      label(stage.ent(), 2, "Click the cell. One glyph shrinks and fades while the other grows in, 250 ms ease in-out.", 150, 100, 1000, 18, true);
      label(stage.ent(), 3, std::string("icon: ") + (icon_b ? "b" : "a"), 60, 170, 400, 20, false, "icon_state");
    }
    else if (slug == "learn_more") {
      auto &shift = motion::anim(Key::LearnShift);
      auto &spread = motion::anim(Key::LearnSpread);
      const float dx = shift.value(), deg = spread.value();
      auto link = button(context, mk(stage.ent(), 1),
                         box(60, 90, 200, 40).with_label("Learn more").with_debug_name("learn_more")
                             .with_font("AtkinsonMock", pixels(22 * s)).with_custom_text_color(accent)
                             .with_alignment(TextAlignment::Left).with_padding(Padding::all(pixels(0)))
                             .with_custom_background(afterhours::colors::transparent()).with_corner_radius(0)
                             .with_on_draw_fg([dx, deg, s, accent](RectangleType r) {
                               const float tip_x = r.x + 138.f * s + dx * s, tip_y = r.y + r.height / 2.f;
                               const float arm = 9.f * s;
                               const float base = 3.14159265f / 4.f, open_by = deg * 3.14159265f / 180.f;
                               const float up = -(base + open_by / 2.f), down = base + open_by / 2.f;
                               afterhours::draw_line_ex({tip_x, tip_y}, {tip_x - arm * std::cos(up), tip_y + arm * std::sin(up)}, 2.f * s, accent);
                               afterhours::draw_line_ex({tip_x, tip_y}, {tip_x - arm * std::cos(down), tip_y + arm * std::sin(down)}, 2.f * s, accent);
                             }));
      const bool hot = context.was_hot(link.id());
      const motion::Spring ease{.response = 0.35f};
      if (!shift.started()) shift.from(0.f);
      if (!spread.started()) spread.from(0.f);
      if (shift.target() != (hot ? 2.f : 0.f)) shift.to(hot ? 2.f : 0.f, ease);
      if (spread.target() != (hot ? 8.f : 0.f)) spread.to(hot ? 8.f : 0.f, ease);
      in_flight |= shift.active() || spread.active();
      label(stage.ent(), 2, "Hover the link. The chevron slides 2 px right and its arms open by 8 degrees.", 300, 100, 900, 18, true);
      label(stage.ent(), 3, std::string("hover: ") + (hot ? "yes" : "no"), 60, 170, 400, 20, false, "learn_hover");
      label(stage.ent(), 4, "shift: " + std::to_string(int(std::lround(dx))) + "px  spread: " + std::to_string(int(std::lround(deg))) + "deg", 60, 200, 500, 20, false, "learn_values");
    } else if (slug == "error_shake") {
      auto &shake = motion::anim(Key::Shake);
      auto &hold = motion::anim(Key::ErrorHold);
      const float sx = shake.value();
      const afterhours::Color pink{255, 235, 238, 255}, danger{220, 60, 70, 255};
      div(context, mk(stage.ent(), 1),
          box(60 + sx, 90, 360, 44).with_label("user@example").with_debug_name("error_field")
              .with_font("AtkinsonMock", pixels(20 * s)).with_custom_text_color(ink).with_alignment(TextAlignment::Left)
              .with_padding(Padding::all(pixels(10 * s))).with_corner_radius(8 * s).with_border(muted, 1.5f * s)
              .with_custom_background(paper)
              .on_state(error, {.background = {paper, pink}}, motion::Timeline{.keys = {{0.f, 0.f}, {0.15f, 1.f}}}));
      div(context, mk(stage.ent(), 2),
          box(60, 140, 500, 26).with_label("Enter a valid email address.").with_debug_name("error_message")
              .with_font("AtkinsonMock", pixels(17 * s)).with_custom_text_color(danger).with_alignment(TextAlignment::Left)
              .with_background(Theme::Usage::None).with_ignore_pointer_events()
              .on_appear({.opacity = 0.f})
              .on_state(error, {.opacity = {0.f, 1.f}}, motion::Timeline{.keys = {{0.f, 0.f}, {0.28f, 1.f}}}));
      if (tab(stage.ent(), 3, "Submit", 440, 90, 120, false, "submit_btn")) {
        ++submits;
        error = true;
        shake.from(0.f).to(6.f, motion::Timeline{.keys = {{0.f, 0.f}, {0.08f, 1.f}, {0.16f, -1.f}, {0.22f, 0.667f}, {0.28f, 0.f}},
                                                  .curve = motion::curves::ease_out_quad});
        hold.from(0.f).to(1.f, motion::Timeline{.keys = {{0.f, 0.f}, {3.28f, 1.f}}}).on_complete([this, alive = std::weak_ptr<int>(alive)] { if (alive.expired()) return; error = false; });
      }
      in_flight |= shake.active();
      label(stage.ent(), 4, "Press Submit. The field shakes 0, +6, -6, +4, 0 px over 280 ms, shows the error, and clears it after 3 s.", 60, 190, 1100, 18, true);
      label(stage.ent(), 5, std::string("error: ") + (error ? "shown" : "clear"), 60, 230, 400, 20, false, "error_state");
      label(stage.ent(), 6, "submits: " + std::to_string(submits), 60, 260, 400, 20, false, "submit_count");
    }
    else if (slug == "input_clear") {
      const motion::Timeline out{.keys = {{0.f, 0.f}, {0.4f, 1.f}}, .curve = motion::curves::ease_out_quad};
      auto field = div(context, mk(stage.ent(), 1),
                       box(60, 90, 420, 44).with_debug_name("clear_field").with_corner_radius(8 * s)
                           .with_border(muted, 1.5f * s).with_custom_background(paper).with_clip_children());
      div(context, mk(field.ent(), 0),
          ComponentConfig{}.with_size({pixels(400 * s), pixels(44 * s)}).with_absolute_position(10 * s, 0.f)
              .with_label("Search the transitions").with_font("AtkinsonMock", pixels(20 * s)).with_custom_text_color(ink)
              .with_alignment(TextAlignment::Left).with_background(Theme::Usage::None).with_ignore_pointer_events()
              .with_debug_name("clear_text")
              .on_state(cleared, {.translate_y = {0.f, 12.f * s}, .opacity = {1.f, 0.f}}, out));
      div(context, mk(field.ent(), 1),
          ComponentConfig{}.with_size({pixels(400 * s), pixels(44 * s)}).with_absolute_position(10 * s, 0.f)
              .with_label("Type to search").with_font("AtkinsonMock", pixels(20 * s)).with_custom_text_color(muted)
              .with_alignment(TextAlignment::Left).with_background(Theme::Usage::None).with_ignore_pointer_events()
              .with_debug_name("clear_placeholder")
              .on_appear({.translate_y = -12.f * s, .opacity = 0.f})
              .on_state(cleared, {.translate_y = {-12.f * s, 0.f}, .opacity = {0.f, 1.f}}, out));
      if (tab(stage.ent(), 2, "Clear", 500, 90, 100, false, "clear_btn")) cleared = true;
      if (tab(stage.ent(), 3, "Reset text", 612, 90, 130, false, "text_reset_btn")) cleared = false;
      for (auto child_id : field.cmp().children) {
        auto child = UICollectionHolder::getEntityForID(child_id);
        if (child.valid()) track_motion(child.asE());
      }
      label(stage.ent(), 4, "Clear drops the old text 12 px while it fades; the placeholder settles in from above. Fade only, no glow yet.", 60, 150, 1100, 18, true);
      label(stage.ent(), 5, std::string("text: ") + (cleared ? "cleared" : "present"), 60, 190, 400, 20, false, "clear_state");
    } else if (slug == "text_swap") {
      static const char *states[] = {"Saved", "Saving changes", "Could not save"};
      const motion::Timeline swap{.keys = {{0.f, 0.f}, {0.15f, 1.f}}, .curve = motion::curves::ease_in_out_quad};
      const size_t stamp = static_cast<size_t>(swap_index);
      const int previous = (swap_index + 2) % 3;
      auto cell = div(context, mk(stage.ent(), 1), box(60, 90, 300, 36).with_debug_name("swap_cell").with_clip_children()
                                                        .with_background(Theme::Usage::None));
      div(context, mk(cell.ent(), 0),
          ComponentConfig{}.with_size({pixels(300 * s), pixels(36 * s)}).with_absolute_position(0.f, 0.f)
              .with_label(states[previous]).with_font("AtkinsonMock", pixels(22 * s)).with_custom_text_color(ink)
              .with_alignment(TextAlignment::Left).with_background(Theme::Usage::None).with_ignore_pointer_events()
              .with_debug_name("swap_out")
              .on_appear({.translate_y = -4.f * s, .opacity = 0.f})
              .on_change(stamp, {.translate_y = {0.f, -4.f * s}, .opacity = {1.f, 0.f}}, swap));
      div(context, mk(cell.ent(), 1),
          ComponentConfig{}.with_size({pixels(300 * s), pixels(36 * s)}).with_absolute_position(0.f, 0.f)
              .with_label(states[swap_index]).with_font("AtkinsonMock", pixels(22 * s)).with_custom_text_color(ink)
              .with_alignment(TextAlignment::Left).with_background(Theme::Usage::None).with_ignore_pointer_events()
              .with_debug_name("swap_in")
              .on_change(stamp, {.translate_y = {4.f * s, 0.f}, .opacity = {0.f, 1.f}}, swap));
      if (tab(stage.ent(), 2, "Next state", 380, 88, 140, false, "swap_btn")) swap_index = (swap_index + 1) % 3;
      for (auto child_id : cell.cmp().children) {
        auto child = UICollectionHolder::getEntityForID(child_id);
        if (child.valid()) track_motion(child.asE());
      }
      label(stage.ent(), 3, "Next state lifts the old label 4 px as it fades; the new one rises in from 4 px below. 150 ms each, overlapping.", 60, 150, 1100, 18, true);
      label(stage.ent(), 4, "state: " + std::to_string(swap_index), 60, 190, 400, 20, false, "swap_state");
    }
    else if (slug == "menu") {
      if (tab(stage.ent(), 1, menu_open ? "Close menu" : "Open menu", 60, 84, 160, false, "menu_trigger")) menu_open = !menu_open;
      auto panel = div(context, mk(stage.ent(), 2),
                       box(60, 132, 220, 152).with_debug_name("menu_panel").with_corner_radius(12 * s)
                           .with_custom_background(paper).with_border({220, 212, 224, 255}, 1.f * s)
                           .with_soft_shadow(2 * s, 6 * s, 16 * s, {80, 60, 100, 30}).with_origin(0.f, 0.f)
                           .with_padding(Padding::all(pixels(8 * s)))
                           .with_flex_direction(FlexDirection::Column)
                           .on_appear({.scale = 0.97f, .opacity = 0.f})
                           .on_state(menu_open, {.scale = {0.97f, 1.f}, .opacity = {0.f, 1.f}},
                                     motion::Timeline{.keys = {{0.f, 0.f}, {0.25f, 1.f}}, .curve = motion::curves::ease_out_quad}));
      static const char *items[] = {"Rename", "Duplicate", "Archive"};
      for (int i = 0; i < 3; ++i) {
        ComponentConfig cfg = ComponentConfig{}.with_size({pixels(204 * s), pixels(40 * s)}).with_label(items[i])
                                  .with_font("AtkinsonMock", pixels(18 * s)).with_custom_text_color(ink)
                                  .with_alignment(TextAlignment::Left).with_padding(Padding::all(pixels(8 * s)))
                                  .with_custom_background(paper).with_corner_radius(6 * s)
                                  .with_debug_name(std::string("menu_item_") + std::to_string(i))
                                  .on_hover({.background = afterhours::Color{244, 240, 246, 255}}, motion::Spring::snappy());
        if (!menu_open) cfg.with_ignore_pointer_events().with_skip_tabbing(true);
        if (button(context, mk(panel.ent(), i), cfg) && menu_open) {
          menu_choice = items[i];
          menu_open = false;
        }
      }
      track_motion(panel.ent());
      label(stage.ent(), 3, "The popover scales from 0.97 at its top-left corner while fading in over 250 ms; closed it keeps no pointer or tab stop.", 320, 100, 900, 18, true);
      label(stage.ent(), 4, std::string("menu: ") + (menu_open ? "open" : "closed"), 320, 140, 400, 20, false, "menu_state");
      label(stage.ent(), 5, "choice: " + menu_choice, 320, 170, 400, 20, false, "menu_choice");
    } else if (slug == "tooltip") {
      static const char *tips[] = {"Bold the selection", "Italicise the selection", "Underline the selection"};
      static const char *names[] = {"Bold", "Italic", "Underline"};
      int hovered = -1;
      for (int i = 0; i < 3; ++i) {
        auto b = tab(stage.ent(), 1 + i, names[i], 60 + i * 140.f, 140, 120, false, std::string("tip_trigger_") + std::to_string(i));
        if (context.was_hot(b.id())) hovered = i;
      }
      if (hovered >= 0) tooltip_trigger = hovered;
      auto &tx = motion::anim(Key::TooltipX);
      const float want_x = 60.f + std::max(tooltip_trigger, 0) * 140.f;
      if (!tx.started()) tx.from(want_x);
      if (tx.target() != want_x) tx.to(want_x, motion::Timeline{.keys = {{0.f, 0.f}, {0.16f, 1.f}}, .curve = motion::curves::ease_out_quad});
      const bool shown = hovered >= 0;
      auto bubble = div(context, mk(stage.ent(), 5),
                        box(tx.value(), 92, 220, 36).with_debug_name("tooltip_bubble").with_corner_radius(8 * s)
                            .with_custom_background(paper).with_border({220, 212, 224, 255}, 1.f * s)
                            .with_label(tooltip_trigger >= 0 ? tips[tooltip_trigger] : "").with_font("AtkinsonMock", pixels(15 * s))
                            .with_custom_text_color(ink).with_alignment(TextAlignment::Center)
                            .with_soft_shadow(2 * s, 4 * s, 12 * s, {80, 60, 100, 30}).with_ignore_pointer_events()
                            .on_appear({.scale = 0.98f, .opacity = 0.f})
                            .on_state(shown, {.scale = {0.98f, 1.f}, .opacity = {0.f, 1.f}},
                                      motion::Timeline{.keys = {{0.f, 0.f}, {0.15f, 1.f}}, .curve = motion::curves::ease_out_quad}));
      track_motion(bubble.ent());
      in_flight |= tx.active();
      label(stage.ent(), 6, "Hover a button. One shared bubble fades and scales in, slides between neighbours over 160 ms, and fades out on leave.", 60, 200, 1100, 18, true);
      label(stage.ent(), 7, std::string("tooltip: ") + (shown ? "shown" : "hidden"), 60, 240, 400, 20, false, "tooltip_state");
      label(stage.ent(), 8, "trigger: " + std::to_string(tooltip_trigger), 60, 270, 400, 20, false, "tooltip_trigger");
    }
    else if (slug == "modal") {
      if (tab(stage.ent(), 1, "Open dialog", 60, 84, 160, false, "modal_open_btn")) modal_open = true;
      auto dialog = afterhours::modal(context, mk(stage.ent(), 2), modal_open,
                                      afterhours::ModalConfig{}.with_title("Fading dialog").with_size(h720(420), h720(200))
                                          .with_motion(motion::Timeline{.keys = {{0.f, 0.f}, {0.25f, 1.f}}, .curve = motion::curves::ease_out_quad},
                                                       motion::Timeline{.keys = {{0.f, 0.f}, {0.15f, 1.f}}}));
      if (dialog) {
        div(context, mk(dialog.ent(), 1), ComponentConfig{}.with_size({percent(1.f), h720(60)}).with_label("The panel fades and the backdrop with it. Input behind stays blocked until the exit settles.")
            .with_font_size(h720(16)).with_text_overflow(TextOverflow::Wrap).with_debug_name("modal_body"));
        if (button(context, mk(dialog.ent(), 2), ComponentConfig{}.with_size({h720(120), h720(40)}).with_label("Done")
                       .with_custom_background(coral).with_custom_text_color(ink).with_corner_radius(8 * s).with_debug_name("modal_done")))
          modal_open = false;
      }
      const auto &m = dialog.ent().get<afterhours::modal::Modal>();
      const char *phase = m.phase == afterhours::modal::Modal::Phase::Hidden ? "hidden" : m.phase == afterhours::modal::Modal::Phase::Entering ? "entering"
                          : m.phase == afterhours::modal::Modal::Phase::Visible ? "visible" : "exiting";
      in_flight |= m.phase == afterhours::modal::Modal::Phase::Entering || m.phase == afterhours::modal::Modal::Phase::Exiting;
      label(stage.ent(), 3, "Open 250 ms ease-out, close 150 ms. The surface motion only: the plugin owns backdrop, focus and input.", 260, 100, 950, 18, true);
      label(stage.ent(), 4, std::string("phase: ") + phase, 60, 170, 400, 20, false, "modal_phase");
    } else if (slug == "panel") {
      if (tab(stage.ent(), 1, panel_open ? "Hide panel" : "Show panel", 60, 84, 160, false, "panel_btn")) panel_open = !panel_open;
      auto frame = div(context, mk(stage.ent(), 2), box(60, 140, 320, 187).with_debug_name("panel_frame").with_corner_radius(12 * s)
                                                         .with_custom_background({244, 240, 246, 255}).with_clip_children());
      auto panel = div(context, mk(frame.ent(), 0),
                       ComponentConfig{}.with_size({pixels(320 * s), pixels(187 * s)}).with_absolute_position(0.f, 0.f)
                           .with_custom_background(accent).with_corner_radius(12 * s).with_debug_name("panel_body")
                           .with_label("Panel").with_font("AtkinsonMock", pixels(24 * s)).with_custom_text_color(paper)
                           .with_alignment(TextAlignment::Center).with_ignore_pointer_events()
                           .on_appear({.translate_y = 93.5f * s, .opacity = 0.f})
                           .on_state(panel_open, {.translate_y = {93.5f * s, 0.f}, .opacity = {0.f, 1.f}},
                                     motion::Timeline{.keys = {{0.f, 0.f}, {0.4f, 1.f}}, .curve = motion::curves::ease_out_quad}));
      track_motion(panel.ent());
      label(stage.ent(), 3, "The panel rises half its height into a clipping frame while fading in, 400 ms ease-out.", 400, 150, 800, 18, true);
      label(stage.ent(), 4, std::string("panel: ") + (panel_open ? "open" : "closed"), 400, 190, 400, 20, false, "panel_state");
    }
    else if (slug == "page") {
      auto frame = div(context, mk(stage.ent(), 1), box(60, 84, 420, 240).with_debug_name("page_frame").with_corner_radius(12 * s)
                                                         .with_custom_background({244, 240, 246, 255}).with_clip_children());
      const motion::Timeline slide{.keys = {{0.f, 0.f}, {0.25f, 1.f}}, .curve = motion::curves::ease_out_quad};
      const auto page = [&](int id, const char *title, afterhours::Color bg, bool is_two, const char *debug) {
        ComponentConfig cfg = ComponentConfig{}.with_size({pixels(420 * s), pixels(240 * s)}).with_absolute_position(0.f, 0.f)
                                  .with_custom_background(bg).with_corner_radius(12 * s).with_label(title)
                                  .with_font("AtkinsonMock", pixels(24 * s)).with_custom_text_color(paper)
                                  .with_alignment(TextAlignment::Center).with_ignore_pointer_events().with_debug_name(debug);
        if (is_two)
          cfg.on_appear({.translate_x = 8.f * s, .opacity = 0.f}).on_state(page_two, {.translate_x = {8.f * s, 0.f}, .opacity = {0.f, 1.f}}, slide);
        else
          cfg.on_state(page_two, {.translate_x = {0.f, -8.f * s}, .opacity = {1.f, 0.f}}, slide);
        return div(context, mk(frame.ent(), id), cfg);
      };
      page(0, "Page 1", accent, false, "page_one");
      page(1, "Page 2", {90, 170, 120, 255}, true, "page_two");
      if (tab(stage.ent(), 2, page_two ? "Back" : "Forward", 500, 84, 120, false, "page_btn")) page_two = !page_two;
      for (auto child_id : frame.cmp().children) {
        auto child = UICollectionHolder::getEntityForID(child_id);
        if (child.valid()) track_motion(child.asE());
      }
      label(stage.ent(), 3, "Two overlapping pages trade places: 8 px of travel and a cross-fade, 250 ms.", 500, 140, 700, 18, true);
      label(stage.ent(), 4, std::string("page: ") + (page_two ? "2" : "1"), 500, 180, 400, 20, false, "page_state");
    } else if (slug == "tabs") {
      static const char *names[] = {"Debug", "Ask", "Plan"};
      static const float widths[] = {110.f, 80.f, 90.f};
      float xs[3]; float run = 0.f;
      for (int i = 0; i < 3; ++i) { xs[i] = run; run += widths[i] + 6.f; }
      auto bar = div(context, mk(stage.ent(), 1), box(60, 84, run + 6.f, 44).with_debug_name("tabs_bar").with_corner_radius(10 * s)
                                                       .with_custom_background({241, 241, 241, 255}));
      auto &px = motion::anim(Key::PillX);
      auto &pw = motion::anim(Key::PillW);
      const float want_x = (6.f + xs[tab_index]) * s, want_w = widths[tab_index] * s;
      const motion::Timeline move{.keys = {{0.f, 0.f}, {0.25f, 1.f}}, .curve = motion::curves::ease_out_quad};
      if (!px.started()) px.from(want_x);
      if (!pw.started()) pw.from(want_w);
      if (px.target() != want_x) px.to(want_x, move);
      if (pw.target() != want_w) pw.to(want_w, move);
      div(context, mk(bar.ent(), 0), ComponentConfig{}.with_size({pixels(pw.value()), pixels(32 * s)}).with_absolute_position(px.value(), 6.f * s)
                                         .with_custom_background(paper).with_corner_radius(8 * s).with_debug_name("tabs_pill")
                                         .with_soft_shadow(0.f, 1.f * s, 3.f * s, {0, 0, 0, 30}).with_ignore_pointer_events());
      for (int i = 0; i < 3; ++i) {
        if (button(context, mk(bar.ent(), 1 + i),
                   ComponentConfig{}.with_size({pixels(widths[i] * s), pixels(32 * s)}).with_absolute_position((6.f + xs[i]) * s, 6.f * s)
                       .with_label(names[i]).with_font("AtkinsonMock", pixels(17 * s))
                       .with_custom_text_color(i == tab_index ? afterhours::Color{15, 15, 15, 255} : afterhours::Color{15, 15, 15, 200})
                       .with_alignment(TextAlignment::Center).with_padding(Padding::all(pixels(0)))
                       .with_custom_background(afterhours::colors::transparent()).with_corner_radius(8 * s)
                       .with_debug_name(std::string("tab_") + std::to_string(i))))
          tab_index = i;
      }
      in_flight |= px.active() || pw.active();
      label(stage.ent(), 3, "One pill slides under the selected tab and resizes to it, 250 ms ease-out. The pill is measured, not the content.", 60, 150, 1100, 18, true);
      label(stage.ent(), 4, "tab: " + std::to_string(tab_index), 60, 190, 400, 20, false, "tabs_state");
    }
    else if (slug == "accordion") {
      auto &h = motion::anim(Key::AccHeight);
      const float content_h = 96.f * s;
      const motion::Timeline ease{.keys = {{0.f, 0.f}, {0.25f, 1.f}}, .curve = motion::curves::ease_out_quad};
      if (!h.started()) h.from(0.f);
      const float want = accordion_open ? content_h : 0.f;
      if (h.target() != want) h.to(want, ease);
      auto column = div(context, mk(stage.ent(), 1), box(60, 84, 420, 300).with_debug_name("acc_column").with_flex_direction(FlexDirection::Column)
                                                          .with_background(Theme::Usage::None));
      auto header = button(context, mk(column.ent(), 0),
                           ComponentConfig{}.with_size({pixels(420 * s), pixels(44 * s)}).with_label("Shipping details")
                               .with_font("AtkinsonMock", pixels(19 * s)).with_custom_text_color(ink).with_alignment(TextAlignment::Left)
                               .with_custom_background({244, 240, 246, 255}).with_corner_radius(8 * s).with_padding(Padding::all(pixels(10 * s)))
                               .with_debug_name("acc_header"));
      div(context, mk(header.ent(), 0), ComponentConfig{}.with_size({pixels(24 * s), pixels(24 * s)}).with_absolute_position(386 * s, 10 * s)
                                            .with_label("v").with_font("AtkinsonMockBold", pixels(18 * s)).with_custom_text_color(ink)
                                            .with_alignment(TextAlignment::Center).with_background(Theme::Usage::None).with_ignore_pointer_events()
                                            .with_debug_name("acc_chevron").on_state(accordion_open, {.rotation = {0.f, 180.f}}, ease));
      if (header) accordion_open = !accordion_open;
      auto content = div(context, mk(column.ent(), 1), ComponentConfig{}.with_size({pixels(420 * s), pixels(h.value())}).with_clip_children()
                                                             .with_background(Theme::Usage::None).with_debug_name("acc_content")
                                                             .on_state(accordion_open, {.opacity = {0.f, 1.f}}, ease));
      div(context, mk(content.ent(), 0), ComponentConfig{}.with_size({pixels(420 * s), pixels(content_h)}).with_absolute_position(0.f, 0.f)
                                             .with_label("Orders ship within two business days. Tracking arrives by email once the parcel leaves the warehouse.")
                                             .with_font("AtkinsonMock", pixels(16 * s)).with_custom_text_color(muted).with_alignment(TextAlignment::Left)
                                             .with_padding(Padding::all(pixels(10 * s))).with_text_overflow(TextOverflow::Wrap)
                                             .with_background(Theme::Usage::None).with_ignore_pointer_events());
      div(context, mk(column.ent(), 2), ComponentConfig{}.with_size({pixels(420 * s), pixels(44 * s)}).with_label("Returns")
                                            .with_font("AtkinsonMock", pixels(19 * s)).with_custom_text_color(ink).with_alignment(TextAlignment::Left)
                                            .with_custom_background({244, 240, 246, 255}).with_corner_radius(8 * s).with_padding(Padding::all(pixels(10 * s)))
                                            .with_ignore_pointer_events().with_debug_name("acc_next"));
      in_flight |= h.active();
      track_motion(content.ent());
      label(stage.ent(), 3, "The content height is a track fed straight into with_size, so the row below reflows; the chevron flips 180 degrees. 250 ms.", 500, 100, 720, 18, true);
      label(stage.ent(), 4, std::string("accordion: ") + (accordion_open ? "open" : "closed"), 500, 150, 400, 20, false, "acc_state");
      label(stage.ent(), 5, "height: " + std::to_string(int(std::lround(h.value() / s))) + "px", 500, 180, 400, 20, false, "acc_height");
    } else if (slug == "morph") {
      auto &w = motion::anim(Key::MorphW);
      auto &hh = motion::anim(Key::MorphH);
      const float closed_w = 40.f * s, closed_h = 40.f * s, open_w = 183.f * s, open_h = 172.f * s;
      if (!w.started()) w.from(closed_w);
      if (!hh.started()) hh.from(closed_h);
      const motion::Timeline grow{.keys = {{0.f, 0.f}, {0.35f, 1.f}}, .curve = motion::curves::ease_out_quad};
      const motion::Timeline shrink{.keys = {{0.f, 0.f}, {0.25f, 1.f}}, .curve = motion::curves::ease_out_quad};
      const float want_w = morph_open ? open_w : closed_w, want_h = morph_open ? open_h : closed_h;
      if (w.target() != want_w) w.to(want_w, morph_open ? grow : shrink);
      if (hh.target() != want_h) hh.to(want_h, morph_open ? grow : shrink);
      const float right = 460.f * s, bottom = 320.f * s;
      auto surface = button(context, mk(stage.ent(), 1),
                            ComponentConfig{}.with_size({pixels(w.value()), pixels(hh.value())})
                                .with_absolute_position(right - w.value(), bottom - hh.value())
                                .with_custom_background(accent).with_corner_radius(40 * s).with_padding(Padding::all(pixels(0)))
                                .with_clip_children().with_debug_name("morph_surface")
                                .on_state(morph_open, {.corner_radius = {40.f * s, 20.f * s}}, grow));
      div(context, mk(surface.ent(), 0), ComponentConfig{}.with_size({pixels(40 * s), pixels(40 * s)}).with_absolute_position(0.f, 0.f)
                                             .with_label("+").with_font("AtkinsonMockBold", pixels(24 * s)).with_custom_text_color(paper)
                                             .with_alignment(TextAlignment::Center).with_background(Theme::Usage::None).with_ignore_pointer_events()
                                             .with_debug_name("morph_plus")
                                             .on_state(morph_open, {.translate_x = {0.f, -40.f * s}, .opacity = {1.f, 0.f}, .rotation = {0.f, 45.f}},
                                                       motion::Timeline{.keys = {{0.f, 0.f}, {0.2f, 1.f}}}));
      static const char *rows[] = {"New file", "New folder", "Upload"};
      for (int i = 0; i < 3; ++i)
        div(context, mk(surface.ent(), 1 + i), ComponentConfig{}.with_size({pixels(160 * s), pixels(36 * s)}).with_absolute_position(12 * s, (16 + i * 44) * s)
                                                   .with_label(rows[i]).with_font("AtkinsonMock", pixels(17 * s)).with_custom_text_color(paper)
                                                   .with_alignment(TextAlignment::Left).with_background(Theme::Usage::None).with_ignore_pointer_events()
                                                   .with_debug_name(std::string("morph_row_") + std::to_string(i))
                                                   .on_appear({.translate_x = 40.f * s, .opacity = 0.f})
                                                   .on_state(morph_open, {.translate_x = {40.f * s, 0.f}, .opacity = {0.f, 1.f}}, grow));
      if (surface) morph_open = !morph_open;
      in_flight |= w.active() || hh.active();
      track_motion(surface.ent());
      label(stage.ent(), 3, "A 40 px round trigger grows to 183 x 172 pinned to its bottom-right corner; radius 40 to 20, plus slides out, rows slide in. 350 ms open, 250 ms close.", 60, 100, 1150, 18, true);
      label(stage.ent(), 4, std::string("morph: ") + (morph_open ? "open" : "closed"), 500, 200, 400, 20, false, "morph_state");
      label(stage.ent(), 5, "size: " + std::to_string(int(std::lround(w.value() / s))) + " x " + std::to_string(int(std::lround(hh.value() / s))), 500, 230, 400, 20, false, "morph_size");
    }
    else if (slug == "toast") {
      auto &hold = motion::anim(Key::ToastHold);
      if (tab(stage.ent(), 1, "Show toast", 60, 84, 160, false, "toast_btn")) {
        toast_shown = true;
        ++toasts;
        hold.from(0.f).to(1.f, motion::Timeline{.keys = {{0.f, 0.f}, {3.f, 1.f}}}).on_complete([this] { toast_shown = false; });
      }
      if (tab(stage.ent(), 2, "Dismiss", 232, 84, 120, false, "toast_dismiss")) toast_shown = false;
      auto toast = div(context, mk(stage.ent(), 3), box(60, 360, 360, 56).with_debug_name("toast").with_corner_radius(12 * s)
                                                         .with_custom_background({40, 40, 48, 255}).with_label("Saved to your library")
                                                         .with_font("AtkinsonMock", pixels(18 * s)).with_custom_text_color(paper)
                                                         .with_alignment(TextAlignment::Left).with_padding(Padding::all(pixels(16 * s)))
                                                         .with_soft_shadow(2 * s, 8 * s, 20 * s, {0, 0, 0, 40}).with_ignore_pointer_events()
                                                         .on_appear({.translate_y = 16.f * s, .scale = 0.97f, .opacity = 0.f})
                                                         .on_state(toast_shown, {.translate_y = {16.f * s, 0.f}, .scale = {0.97f, 1.f}, .opacity = {0.f, 1.f}},
                                                                   motion::Timeline{.keys = {{0.f, 0.f}, {0.35f, 1.f}}, .curve = motion::curves::ease_out_quad}));
      track_motion(toast.ent());
      label(stage.ent(), 4, "The toast enters from 16 px below at scale 0.97 over 350 ms and leaves the same way; it dismisses itself after 3 s.", 60, 150, 1100, 18, true);
      label(stage.ent(), 5, std::string("toast: ") + (toast_shown ? "shown" : "hidden"), 60, 190, 400, 20, false, "toast_state");
      label(stage.ent(), 6, "toasts: " + std::to_string(toasts), 60, 220, 400, 20, false, "toast_count");
    } else if (slug == "badge") {
      if (tab(stage.ent(), 1, badge_shown ? "Clear badge" : "Notify", 60, 84, 160, false, "badge_btn")) badge_shown = !badge_shown;
      auto bell = div(context, mk(stage.ent(), 2), box(300, 84, 48, 48).with_debug_name("badge_anchor").with_corner_radius(12 * s)
                                                       .with_custom_background({244, 240, 246, 255}).with_label("@")
                                                       .with_font("AtkinsonMockBold", pixels(22 * s)).with_custom_text_color(ink).with_alignment(TextAlignment::Center));
      auto wrap = div(context, mk(bell.ent(), 0), ComponentConfig{}.with_size({pixels(18 * s), pixels(18 * s)}).with_absolute_position(36 * s, -6 * s)
                                                      .with_background(Theme::Usage::None).with_debug_name("badge_wrap").with_ignore_pointer_events()
                                                      .on_appear({.translate_x = -8.2f * s, .translate_y = 12.4f * s})
                                                      .on_state(badge_shown, {.translate_x = {-8.2f * s, 0.f}, .translate_y = {12.4f * s, 0.f}},
                                                                motion::Timeline{.keys = {{0.f, 0.f}, {0.26f, 1.f}}, .curve = motion::curves::ease_out_quad}));
      div(context, mk(wrap.ent(), 0), ComponentConfig{}.with_size({pixels(18 * s), pixels(18 * s)}).with_absolute_position(0.f, 0.f)
                                          .with_custom_background(coral).with_corner_radius(9 * s).with_debug_name("badge_dot").with_ignore_pointer_events()
                                          .with_label("3").with_font("AtkinsonMockBold", pixels(11 * s)).with_custom_text_color(paper).with_alignment(TextAlignment::Center)
                                          .on_appear({.scale = 0.f, .opacity = 0.f})
                                          .on_state(badge_shown, {.scale = {0.f, 1.f}, .opacity = {0.f, 1.f}}, motion::Spring{.response = 0.5f, .bounce = 0.35f}));
      track_motion(wrap.ent());
      for (auto child_id : wrap.cmp().children) {
        auto child = UICollectionHolder::getEntityForID(child_id);
        if (child.valid()) track_motion(child.asE());
      }
      label(stage.ent(), 3, "The wrapper slides in diagonally (260 ms) while the dot pops from scale 0 on a 500 ms spring; clearing collapses it faster.", 60, 150, 1100, 18, true);
      label(stage.ent(), 4, std::string("badge: ") + (badge_shown ? "shown" : "hidden"), 60, 190, 400, 20, false, "badge_state");
    }
    label(stage.ent(), 9, std::string("motion: ") + (in_flight ? "in flight" : "settled"), 60, 400, 400, 20, false, "motion_state");
  }
};

REGISTER_EXAMPLE_SCREEN(transitions_lab, "Animations",
                        "Transitions Lab: the 43 transitions on working controls",
                        TransitionsLab)
