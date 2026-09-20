#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
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
      {"controls", "Controls", {{"checkbox", "Checkbox check"}, {"toggle", "Toggle"}, {"like", "Like button"}, {"icon_swap", "Icon swap"}, {"learn_more", "Learn more hover"}, {"error_shake", "Error state shake"}}},
      {"navigation", "Navigation and overlays", {}},
      {"status", "Status and loading", {}},
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

  enum struct Key : size_t { CheckDraw, LikeFill, LearnShift, LearnSpread, Shake, ErrorHold };

  void reset_examples() {
    checked = false;
    toggled = false;
    liked = false;
    likes = 0;
    icon_b = false;
    error = false;
    submits = 0;
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
                         bool selected, const std::string &debug) {
      return button(context, mk(parent, id),
                    box(x, y, w, 40).with_label(text).with_font("AtkinsonMock", pixels(17 * s))
                        .with_custom_background(selected ? coral : afterhours::Color{244, 240, 246, 255})
                        .with_custom_text_color(ink).with_corner_radius(8 * s).with_debug_name(debug)
                        .on_hover({.scale = 1.03f}).on_press({.scale = 0.96f}));
    };

    div(context, mk(entity, 0), ComponentConfig{}.with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background({255, 245, 235, 255}).with_corner_radius(0).with_debug_name("bg"));
    label(entity, 1, "Transitions Lab", 40, 24, 600, 30);

    for (size_t g = 0; g < groups.size(); ++g)
      if (tab(entity, 10 + int(g), groups[g].title, 40 + g * 240.f, 70, 228, g == group, std::string("group_") + groups[g].slug))
        group = g;
    const Group &current = groups[group];
    if (example[group] >= current.examples.size()) example[group] = 0;
    for (size_t e = 0; e < current.examples.size(); ++e)
      if (tab(entity, 20 + int(e), current.examples[e].title, 40 + e * 200.f, 122, 188, e == example[group],
              std::string("ex_") + current.examples[e].slug))
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
    label(stage.ent(), 9, std::string("motion: ") + (in_flight ? "in flight" : "settled"), 60, 400, 400, 20, false, "motion_state");
  }
};

REGISTER_EXAMPLE_SCREEN(transitions_lab, "Animations",
                        "Transitions Lab: the 43 transitions on working controls",
                        TransitionsLab)
