#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/effects.h>
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
      {"status", "Status and loading", {{"toast", "Toast open/close"}, {"badge", "Notification badge"}, {"success", "Success check"}, {"skeleton", "Skeleton loader"}, {"spinner", "Spinner to check"}, {"banners", "Banner stacking"}, {"streaming", "Streaming text"}, {"thinking", "Thinking states"}, {"reasoning", "Reasoning stream"}, {"shimmer", "Shimmer text"}}},
      {"cards", "Cards and text", {{"card_resize", "Card resize"}, {"card_stack", "Card stack hover"}, {"avatars", "Avatar group hover"}, {"texts_reveal", "Texts reveal"}, {"matrix", "Matrix dot loader"}, {"counter", "Spinning counter"}, {"popin", "Number pop-in"}}},
      {"effects", "Visual effects", {{"effect", "Shader effect"}}},
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
  bool success_shown = false;
  bool skeleton_revealed = false;
  bool spinner_done = false;
  int completions = 0;
  bool card_expanded = true;
  bool stream_playing = false;
  int stream_runs = 0;
  bool thinking_playing = true;
  int thinking_index = 0;
  bool reasoning_playing = true;
  int reasoning_steps = 0;
  bool shimmer_playing = true;
  int counter_value = 0;
  int popin_value = 1289;
  std::optional<afterhours::effects::Effect> tint_effect;
  int shader_reloads = 0;
  float tint_strength = 1.f;
  bool texts_shown = false;
  int matrix_pattern = 0;
  struct Banner { int id; std::string text; bool leaving = false; };
  std::vector<Banner> banners;
  int next_banner = 0;

  enum struct Key : size_t { CheckDraw, LikeFill, LearnShift, LearnSpread, Shake, ErrorHold, TooltipX, PillX, PillW, AccHeight, MorphW, MorphH, ToastHold, SuccessDraw, SkeletonLoad, SpinAngle, SpinCheck, SpinHold, BannerLeave, CardW, CardH, StreamCount, ThinkHold, ReasonOffset, ReasonHold, ShimmerX, Reel0, Reel1, Reel2, Reel3 };

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
    success_shown = false;
    skeleton_revealed = false;
    spinner_done = false;
    completions = 0;
    card_expanded = true;
    stream_playing = false;
    stream_runs = 0;
    thinking_playing = true;
    thinking_index = 0;
    reasoning_playing = true;
    reasoning_steps = 0;
    shimmer_playing = true;
    counter_value = 0;
    popin_value = 1289;
    for (int i = 0; i < 4; ++i) afterhours::motion::anim(static_cast<Key>(static_cast<size_t>(Key::Reel0) + i)).from(0.f);
    afterhours::motion::anim(Key::ReasonOffset).from(0.f);
    afterhours::motion::anim(Key::ReasonHold).from(0.f);
    afterhours::motion::anim(Key::StreamCount).from(0.f);
    afterhours::motion::anim(Key::ThinkHold).from(0.f);
    texts_shown = false;
    matrix_pattern = 0;
    banners.clear();
    next_banner = 0;
    afterhours::motion::anim(Key::SpinCheck).from(0.f);
    afterhours::motion::anim(Key::SpinHold).from(0.f);
    afterhours::motion::anim(Key::BannerLeave).from(0.f);
    afterhours::motion::anim(Key::ToastHold).from(0.f);
    afterhours::motion::anim(Key::SuccessDraw).from(0.f);
    afterhours::motion::anim(Key::SkeletonLoad).from(0.f);
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
        hold.from(0.f).to(1.f, motion::Timeline{.keys = {{0.f, 0.f}, {3.f, 1.f}}}).on_complete([this, alive = std::weak_ptr<int>(alive)] { if (alive.expired()) return; toast_shown = false; });
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
    else if (slug == "success") {
      auto &draw = motion::anim(Key::SuccessDraw);
      const float p = draw.value();
      if (tab(stage.ent(), 1, success_shown ? "Hide" : "Show success", 60, 84, 160, false, "success_btn")) {
        success_shown = !success_shown;
        if (success_shown) {
          draw.from(0.f).to(1.f, motion::Timeline{.keys = {{0.f, 0.f}, {0.5f, 1.f}}, .curve = motion::curves::ease_out_quad}).delay(0.08f);
        } else {
          draw.to(0.f, motion::Timeline{.keys = {{0.f, 0.f}, {0.15f, 1.f}}});
        }
      }
      const afterhours::Color green{52, 168, 96, 255};
      auto icon = div(context, mk(stage.ent(), 2),
                      box(300, 80, 56, 56).with_debug_name("success_icon").with_corner_radius(28 * s).with_custom_background(green)
                          .with_ignore_pointer_events()
                          .with_on_draw_fg([p, s](RectangleType r) {
                            if (p <= 0.f) return;
                            const Vector2Type a{r.x + r.width * 0.28f, r.y + r.height * 0.52f};
                            const Vector2Type b{r.x + r.width * 0.44f, r.y + r.height * 0.68f};
                            const Vector2Type c{r.x + r.width * 0.74f, r.y + r.height * 0.34f};
                            const float l1 = std::hypot(b.x - a.x, b.y - a.y), l2 = std::hypot(c.x - b.x, c.y - b.y);
                            const float total = (l1 + l2) * std::clamp(p, 0.f, 1.f);
                            const afterhours::Color white{255, 255, 255, 255};
                            if (total <= l1) {
                              const float t = total / l1;
                              afterhours::draw_line_ex(a, {a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t}, 4.f * s, white);
                            } else {
                              afterhours::draw_line_ex(a, b, 4.f * s, white);
                              const float t = (total - l1) / l2;
                              afterhours::draw_line_ex(b, {b.x + (c.x - b.x) * t, b.y + (c.y - b.y) * t}, 4.f * s, white);
                            }
                          })
                          .on_appear({.rotation = 80.f, .translate_y = 40.f * s, .opacity = 0.f})
                          .on_state(success_shown, {.rotation = {80.f, 0.f}, .translate_y = {40.f * s, 0.f}, .opacity = {0.f, 1.f}},
                                    motion::Spring{.response = 0.5f, .bounce = 0.3f}));
      track_motion(icon.ent());
      in_flight |= draw.active();
      label(stage.ent(), 3, "The badge rises 40 px while rotating in from 80 degrees on a spring; the check stroke draws after an 80 ms delay over 500 ms.", 60, 150, 1100, 18, true);
      label(stage.ent(), 4, std::string("success: ") + (success_shown ? "shown" : "hidden"), 60, 190, 400, 20, false, "success_state");
      label(stage.ent(), 5, "stroke: " + std::to_string(int(std::lround(p * 100.f))) + "%", 60, 220, 400, 20, false, "success_stroke");
      {
        float rot = 0.f;
        if (icon.ent().has<motion::HasTracks>()) {
          auto &t = icon.ent().get<motion::HasTracks>().floats;
          if (auto it = t.find(static_cast<size_t>(MotionProperty::Rotation)); it != t.end()) rot = it->second.value();
        }
        label(stage.ent(), 6, "rotation: " + std::to_string(rot), 60, 250, 400, 20, false, "success_rotation");
      }
    } else if (slug == "skeleton") {
      auto &load = motion::anim(Key::SkeletonLoad);
      bool restart = false;
      if (tab(stage.ent(), 1, "Load", 60, 84, 120, false, "skeleton_btn") || (replay_stamp != seen_replay && (seen_replay = replay_stamp, true))) {
        skeleton_revealed = false;
        restart = true;
        load.from(0.f).to(1.f, motion::Timeline{.keys = {{0.f, 0.f}, {1.f, 1.f}}}).on_complete([this, alive = std::weak_ptr<int>(alive)] { if (alive.expired()) return; skeleton_revealed = true; });
      }
      auto card = div(context, mk(stage.ent(), 2), box(60, 140, 360, 120).with_debug_name("skeleton_card").with_corner_radius(12 * s)
                                                        .with_custom_background({244, 240, 246, 255}).with_padding(Padding::all(pixels(16 * s))));
      const motion::Timeline pulse{.keys = {{0.f, 0.f}, {0.5f, 1.f}}, .repeat = motion::Timeline::Repeat::PingPong, .curve = motion::curves::ease_in_out_quad};
      const motion::Timeline reveal{.keys = {{0.f, 0.f}, {0.4f, 1.f}}, .curve = motion::curves::ease_in_out_quad};
      const auto bar = [&](int id, float y, float w) {
        div(context, mk(card.ent(), id), ComponentConfig{}.with_size({pixels(w * s), pixels(16 * s)}).with_absolute_position(16 * s, y * s)
                                             .with_custom_background({215, 208, 220, 255}).with_corner_radius(8 * s).with_ignore_pointer_events()
                                             .with_debug_name(std::string("skeleton_bar_") + std::to_string(id))
                                             .on_appear({.opacity = {1.f, 0.5f}}, pulse)
                                             .on_state(skeleton_revealed, {.opacity = 0.f}, reveal));
      };
      bar(0, 16, 220);
      bar(1, 48, 300);
      bar(2, 80, 180);
      div(context, mk(card.ent(), 3), ComponentConfig{}.with_size({pixels(328 * s), pixels(88 * s)}).with_absolute_position(16 * s, 16 * s)
                                          .with_label("Order #4821 shipped. Expected Thursday; tracking is in your inbox.")
                                          .with_font("AtkinsonMock", pixels(17 * s)).with_custom_text_color(ink).with_alignment(TextAlignment::Left)
                                          .with_text_overflow(TextOverflow::Wrap).with_background(Theme::Usage::None).with_ignore_pointer_events()
                                          .with_debug_name("skeleton_content")
                                          .on_appear({.opacity = 0.f})
                                          .on_state(skeleton_revealed, {.opacity = {0.f, 1.f}}, reveal));
      for (auto child_id : card.cmp().children) {
        auto child = UICollectionHolder::getEntityForID(child_id);
        if (!child.valid()) continue;
        track_motion(child.asE());
        if (restart) {
          child.asE().removeComponentIfExists<HasMotionState>();
          child.asE().removeComponentIfExists<motion::HasTracks>();
        }
      }
      in_flight |= load.active();
      label(stage.ent(), 4, "Bars pulse between opacity 1 and 0.5 while loading; after 1 s the content cross-fades in over 400 ms. Replay restarts the load.", 60, 280, 1100, 18, true);
      label(stage.ent(), 5, std::string("content: ") + (skeleton_revealed ? "revealed" : "loading"), 60, 320, 400, 20, false, "skeleton_state");
    }
    else if (slug == "spinner") {
      auto &angle = motion::anim(Key::SpinAngle);
      auto &check = motion::anim(Key::SpinCheck);
      auto &hold = motion::anim(Key::SpinHold);
      if (!angle.started())
        angle.from(0.f).to(360.f, motion::Timeline{.keys = {{0.f, 0.f}, {0.9f, 1.f}}, .repeat = motion::Timeline::Repeat::Loop}).essential();
      if (tab(stage.ent(), 1, "Complete", 60, 84, 140, false, "spinner_btn") && !spinner_done) {
        spinner_done = true;
        ++completions;
        check.from(0.f).to(1.f, motion::Timeline{.keys = {{0.f, 0.f}, {0.6f, 1.f}}, .curve = motion::curves::ease_out_quad}).delay(0.23f);
        hold.from(0.f).to(1.f, motion::Timeline{.keys = {{0.f, 0.f}, {2.f, 1.f}}}).on_complete([this, alive = std::weak_ptr<int>(alive)] { if (alive.expired()) return;
          spinner_done = false;
          afterhours::motion::anim(Key::SpinCheck).to(0.f, motion::Timeline{.keys = {{0.f, 0.f}, {0.25f, 1.f}}});
        });
      }
      const float a = angle.value(), p = check.value();
      const bool done = spinner_done;
      const afterhours::Color green{52, 168, 96, 255}, ring{200, 195, 205, 255};
      auto badge = div(context, mk(stage.ent(), 2),
                       box(300, 80, 48, 48).with_debug_name("spinner_badge").with_corner_radius(24 * s)
                           .with_custom_background(afterhours::colors::transparent()).with_ignore_pointer_events()
                           .with_on_draw_fg([a, p, done, s, green, ring](RectangleType r) {
                             const float cx = r.x + r.width / 2.f, cy = r.y + r.height / 2.f, outer = r.width / 2.f;
                             if (!done) {
                               afterhours::draw_ring(cx, cy, outer - 4.f * s, outer, 48, ring);
                               afterhours::draw_ring_segment(cx, cy, outer - 4.f * s, outer, a, a + 90.f, 24, green);
                               return;
                             }
                             if (p <= 0.f) return;
                             const Vector2Type pa{r.x + r.width * 0.28f, r.y + r.height * 0.52f};
                             const Vector2Type pb{r.x + r.width * 0.44f, r.y + r.height * 0.68f};
                             const Vector2Type pc{r.x + r.width * 0.74f, r.y + r.height * 0.34f};
                             const float l1 = std::hypot(pb.x - pa.x, pb.y - pa.y), l2 = std::hypot(pc.x - pb.x, pc.y - pb.y);
                             const float total = (l1 + l2) * std::clamp(p, 0.f, 1.f);
                             const afterhours::Color white{255, 255, 255, 255};
                             if (total <= l1) {
                               const float t = total / l1;
                               afterhours::draw_line_ex(pa, {pa.x + (pb.x - pa.x) * t, pa.y + (pb.y - pa.y) * t}, 4.f * s, white);
                             } else {
                               afterhours::draw_line_ex(pa, pb, 4.f * s, white);
                               const float t = (total - l1) / l2;
                               afterhours::draw_line_ex(pb, {pb.x + (pc.x - pb.x) * t, pb.y + (pc.y - pb.y) * t}, 4.f * s, white);
                             }
                           })
                           .on_state(spinner_done, {.scale = {1.f, 1.09f}, .background = {afterhours::colors::transparent(), green}},
                                     motion::Spring{.response = 0.35f, .bounce = 0.5f})
                           .on_change(static_cast<size_t>(completions), {.translate_y = {-3.f * s, 0.f}}, motion::Spring{.response = 0.3f, .bounce = 0.6f}));
      track_motion(badge.ent());
      in_flight |= check.active() || hold.active();
      label(stage.ent(), 3, "The ring spins on a 900 ms loop (essential, so it keeps turning under reduced motion). Complete grows a green badge with a 3 px bob and draws the check after 230 ms; it resets after 2 s.", 60, 150, 1150, 18, true);
      label(stage.ent(), 4, std::string("state: ") + (spinner_done ? "done" : "spinning"), 60, 200, 400, 20, false, "spinner_state");
      label(stage.ent(), 5, "completions: " + std::to_string(completions), 60, 230, 400, 20, false, "spinner_count");
    } else if (slug == "banners") {
      auto &leave = motion::anim(Key::BannerLeave);
      if (tab(stage.ent(), 1, "Add banner", 60, 84, 160, false, "banner_add")) {
        banners.insert(banners.begin(), Banner{next_banner, "Update " + std::to_string(next_banner + 1) + " is ready"});
        ++next_banner;
        int depth = 0;
        for (auto &b : banners) {
          if (b.leaving) continue;
          if (depth >= 3) b.leaving = true;
          ++depth;
        }
        leave.from(0.f).to(1.f, motion::Timeline{.keys = {{0.f, 0.f}, {0.31f, 1.f}}}).on_complete([this, alive = std::weak_ptr<int>(alive)] { if (alive.expired()) return;
          std::erase_if(banners, [](const Banner &b) { return b.leaving; });
        });
      }
      auto stack = div(context, mk(stage.ent(), 2), box(60, 150, 360, 220).with_debug_name("banner_stack").with_background(Theme::Usage::None));
      const RectangleType stack_rect = stack.cmp().rect();
      const bool hovered = context.mouse.pos.x >= stack_rect.x && context.mouse.pos.x <= stack_rect.x + stack_rect.width &&
                           context.mouse.pos.y >= stack_rect.y && context.mouse.pos.y <= stack_rect.y + stack_rect.height;
      const float banner_h = 56.f * s;
      const motion::Timeline settle{.keys = {{0.f, 0.f}, {0.35f, 1.f}}, .curve = motion::curves::ease_out_quad};
      int depth = 0;
      int visible = 0;
      for (auto &b : banners) {
        const int d = b.leaving ? 3 : depth;
        if (!b.leaving) { ++depth; ++visible; }
        const float want_y = hovered ? -(banner_h + 8.f * s) * d : -12.f * s * d;
        const float want_scale = hovered ? 1.f : 1.f - 0.06f * d;
        const float want_alpha = b.leaving ? 0.f : (hovered ? 1.f : 1.f - 0.32f * d);
        auto row = div(context, mk(stack.ent(), 100 + b.id),
                       ComponentConfig{}.with_size({pixels(360 * s), pixels(banner_h)}).with_absolute_position(0.f, 220 * s - banner_h)
                           .with_custom_background({40, 40, 48, 255}).with_corner_radius(12 * s).with_label(b.text)
                           .with_font("AtkinsonMock", pixels(17 * s)).with_custom_text_color(paper).with_alignment(TextAlignment::Left)
                           .with_padding(Padding::all(pixels(16 * s))).with_render_layer(10 - d).with_ignore_pointer_events()
                           .with_debug_name(std::string("banner_") + std::to_string(b.id))
                           .on_appear({.translate_y = {80.f * s, 0.f}, .opacity = {0.f, 1.f}, .scale = {0.97f, 1.f}}, settle));
        auto &tracks = row.ent().addComponentIfMissing<motion::HasTracks>();
        auto &ty = tracks.track<float>(100);
        auto &sc = tracks.track<float>(101);
        auto &al = tracks.track<float>(102);
        if (!ty.started()) ty.from(want_y);
        if (!sc.started()) sc.from(want_scale);
        if (!al.started()) al.from(want_alpha);
        if (ty.target() != want_y) ty.to(want_y, settle);
        if (sc.target() != want_scale) sc.to(want_scale, settle);
        if (al.target() != want_alpha) al.to(want_alpha, b.leaving ? motion::Mode{motion::Timeline{.keys = {{0.f, 0.f}, {0.25f, 1.f}}}} : motion::Mode{settle});
        auto &mods = row.ent().addComponentIfMissing<HasUIModifiers>();
        mods.translate_y += ty.value();
        mods.scale *= sc.value();
        row.ent().addComponentIfMissing<HasOpacity>().value *= al.value();
        track_motion(row.ent());
      }
      label(stage.ent(), 3, "A new banner rises 80 px into the front; older ones step back 12 px, shrink 6 % and dim per depth. A fourth arrival dismisses the oldest. Hover the stack to spread it.", 440, 160, 780, 18, true);
      label(stage.ent(), 4, "banners: " + std::to_string(visible), 440, 220, 400, 20, false, "banner_count");
      label(stage.ent(), 5, std::string("spread: ") + (hovered ? "yes" : "no"), 440, 250, 400, 20, false, "banner_spread");
    }
    else if (slug == "card_resize") {
      auto &cw = motion::anim(Key::CardW);
      auto &ch = motion::anim(Key::CardH);
      const float big_w = 260.f * s, big_h = 180.f * s, small_w = 160.f * s, small_h = 100.f * s;
      if (!cw.started()) cw.from(big_w);
      if (!ch.started()) ch.from(big_h);
      const motion::Timeline resize{.keys = {{0.f, 0.f}, {0.3f, 1.f}}, .curve = motion::curves::ease_out_quad};
      const float want_w = card_expanded ? big_w : small_w, want_h = card_expanded ? big_h : small_h;
      if (cw.target() != want_w) cw.to(want_w, resize);
      if (ch.target() != want_h) ch.to(want_h, resize);
      if (tab(stage.ent(), 1, card_expanded ? "Compact" : "Expand", 60, 84, 140, false, "card_btn")) card_expanded = !card_expanded;
      auto card = div(context, mk(stage.ent(), 2),
                      ComponentConfig{}.with_size({pixels(cw.value()), pixels(ch.value())}).with_absolute_position(60 * s, 140 * s)
                          .with_custom_background(paper).with_border({220, 212, 224, 255}, 1.f * s).with_corner_radius(14 * s)
                          .with_soft_shadow(2 * s, 6 * s, 16 * s, {80, 60, 100, 30}).with_padding(Padding::all(pixels(14 * s)))
                          .with_clip_children().with_debug_name("resize_card")
                          .with_label("Only width and height animate; the text reflows inside the changing box.")
                          .with_font("AtkinsonMock", pixels(15 * s)).with_custom_text_color(ink).with_alignment(TextAlignment::Left)
                          .with_text_overflow(TextOverflow::Wrap));
      in_flight |= cw.active() || ch.active();
      label(stage.ent(), 3, "The same container tweens between 260 x 180 and 160 x 100 over 300 ms.", 360, 150, 800, 18, true);
      label(stage.ent(), 4, "size: " + std::to_string(int(std::lround(cw.value() / s))) + " x " + std::to_string(int(std::lround(ch.value() / s))), 360, 190, 400, 20, false, "card_size");
    } else if (slug == "card_stack") {
      auto zone = div(context, mk(stage.ent(), 1), box(60, 84, 420, 260).with_debug_name("stack_zone").with_background(Theme::Usage::None));
      const RectangleType zr = zone.cmp().rect();
      const bool fanned = context.mouse.pos.x >= zr.x && context.mouse.pos.x <= zr.x + zr.width &&
                          context.mouse.pos.y >= zr.y && context.mouse.pos.y <= zr.y + zr.height;
      static const afterhours::Color tints[] = {{120, 160, 230, 255}, {230, 150, 120, 255}, {130, 200, 150, 255}};
      static const float base_x[] = {0.f, 14.f, 28.f}, base_rot[] = {-4.f, 0.f, 4.f};
      static const float fan_x[] = {-70.f, 0.f, 70.f}, fan_rot[] = {-12.f, 0.f, 12.f};
      const motion::Spring open{.response = 0.41f, .bounce = 0.5f}, close{.response = 0.36f, .bounce = 0.35f};
      for (int i = 0; i < 3; ++i) {
        auto card = button(context, mk(zone.ent(), i),
                           ComponentConfig{}.with_size({pixels(150 * s), pixels(200 * s)}).with_absolute_position((130 + base_x[i]) * s, 30 * s)
                               .with_custom_background(tints[i]).with_corner_radius(14 * s).with_padding(Padding::all(pixels(0)))
                               .with_soft_shadow(2 * s, 8 * s, 18 * s, {40, 30, 60, 50}).with_debug_name(std::string("stack_card_") + std::to_string(i))
                               .with_label(std::string("Card ") + std::to_string(i + 1)).with_font("AtkinsonMock", pixels(18 * s))
                               .with_custom_text_color(paper).with_alignment(TextAlignment::Center)
                               .on_appear({.rotation = base_rot[i], .opacity = i == 1 ? 1.f : 0.85f})
                               .on_state(fanned, {.translate_x = {0.f, (fan_x[i] - base_x[i] + 14.f) * s}, .rotation = {base_rot[i], fan_rot[i]}, .opacity = {i == 1 ? 1.f : 0.85f, 1.f}},
                                         fanned ? open : close)
                               .on_hover({.scale = 1.04f}, motion::Spring{.response = 0.61f}));
        track_motion(card.ent());
      }
      label(stage.ent(), 2, "Hover the stack: the three cards fan out on a spring with overshoot, back cards turn fully opaque, and the card under the pointer grows 4 %.", 60, 350, 1100, 18, true);
      label(stage.ent(), 3, std::string("fan: ") + (fanned ? "open" : "closed"), 500, 100, 400, 20, false, "stack_state");
    }
    else if (slug == "avatars") {
      static const afterhours::Color tints[] = {{120, 160, 230, 255}, {230, 150, 120, 255}, {130, 200, 150, 255}, {200, 150, 220, 255}, {230, 200, 110, 255}};
      int active = -1;
      std::array<afterhours::Entity *, 5> ents{};
      for (int i = 0; i < 5; ++i) {
        auto av = button(context, mk(stage.ent(), 1 + i),
                         ComponentConfig{}.with_size({pixels(48 * s), pixels(48 * s)}).with_absolute_position((60 + i * 40) * s, 100 * s)
                             .with_custom_background(tints[i]).with_corner_radius(24 * s).with_padding(Padding::all(pixels(0)))
                             .with_border(paper, 2.f * s).with_render_layer(5 + i).with_debug_name(std::string("avatar_") + std::to_string(i))
                             .with_label(std::string(1, char('A' + i))).with_font("AtkinsonMockBold", pixels(18 * s))
                             .with_custom_text_color(paper).with_alignment(TextAlignment::Center)
                             .on_hover({.scale = 1.05f}, motion::Spring{.response = 0.32f, .bounce = 0.6f}));
        ents[i] = &av.ent();
        if (context.was_hot(av.id())) active = i;
      }
      const motion::Spring in{.response = 0.32f}, out{.response = 0.32f, .bounce = 0.75f};
      for (int j = 0; j < 5; ++j) {
        const float want = active < 0 ? 0.f : -4.f * s * std::pow(0.45f, float(std::abs(active - j)));
        auto &lift = ents[j]->addComponentIfMissing<motion::HasTracks>().track<float>(100);
        if (!lift.started()) lift.from(0.f);
        if (lift.target() != want) lift.to(want, active < 0 ? out : in);
        ents[j]->addComponentIfMissing<HasUIModifiers>().translate_y += lift.value();
        track_motion(*ents[j]);
      }
      label(stage.ent(), 6, "Hover an avatar: it rises 4 px and grows 5 %; neighbours rise by 0.45 per step of distance. Leaving settles through a stronger overshoot.", 60, 200, 1100, 18, true);
      label(stage.ent(), 7, "active: " + std::to_string(active), 60, 240, 400, 20, false, "avatar_active");
    } else if (slug == "texts_reveal") {
      if (tab(stage.ent(), 1, texts_shown ? "Hide" : "Reveal", 60, 84, 140, false, "texts_btn")) texts_shown = !texts_shown;
      const motion::Timeline first{.keys = {{0.f, 0.f}, {0.5f, 1.f}}, .curve = motion::curves::ease_out_quad};
      const motion::Timeline second{.keys = {{0.f, 0.f}, {0.04f, 0.f}, {0.54f, 1.f}}, .curve = motion::curves::ease_out_quad};
      const motion::Timeline hide{.keys = {{0.f, 0.f}, {0.2f, 1.f}}};
      const auto line = [&](int id, const char *text, float y, const motion::Timeline &tl, const char *debug) {
        auto l = div(context, mk(stage.ent(), id),
                     box(60, y, 700, 40).with_label(text).with_font("AtkinsonMock", pixels(id == 2 ? 30 * s : 20 * s))
                         .with_custom_text_color(id == 2 ? ink : muted).with_alignment(TextAlignment::Left)
                         .with_background(Theme::Usage::None).with_ignore_pointer_events().with_debug_name(debug)
                         .on_appear({.translate_y = 12.f * s, .opacity = 0.f})
                         .on_state(texts_shown, {.translate_y = {12.f * s, 0.f}, .opacity = {0.f, 1.f}}, texts_shown ? tl : hide));
        track_motion(l.ent());
      };
      line(2, "Motion that explains itself", 150, first, "reveal_line_1");
      line(3, "Two lines rise 12 px while fading in; the second starts 40 ms after the first.", 196, second, "reveal_line_2");
      label(stage.ent(), 4, std::string("texts: ") + (texts_shown ? "shown" : "hidden"), 60, 260, 400, 20, false, "texts_state");
    }
    else if (slug == "matrix") {
      static const char *pattern_names[] = {"Scan", "Twinkle", "Orbit", "Pulse"};
      bool changed = false;
      for (int i = 0; i < 4; ++i)
        if (tab(stage.ent(), 1 + i, pattern_names[i], 60 + i * 110.f, 84, 100, i == matrix_pattern, std::string("matrix_") + std::to_string(i), 32.f) && matrix_pattern != i) {
          matrix_pattern = i;
          changed = true;
        }
      static const int twinkle[16] = {7, 2, 11, 5, 14, 9, 0, 12, 3, 15, 6, 10, 13, 1, 8, 4};
      static const int orbit[8] = {1, 2, 7, 11, 14, 13, 8, 4};
      const float cycle = 1.2f;
      const afterhours::Color base{217, 217, 217, 255}, on{133, 133, 143, 255};
      auto grid = div(context, mk(stage.ent(), 10), box(60, 150, 100, 100).with_debug_name("matrix").with_background(Theme::Usage::None));
      for (int i = 0; i < 16; ++i) {
        const int col = i % 4, row = i / 4;
        float delay = 0.f;
        bool lit = true;
        switch (matrix_pattern) {
        case 0: delay = col * cycle / 10.f; break;
        case 1: for (int k = 0; k < 16; ++k) if (twinkle[k] == i) delay = k * cycle / 16.f; break;
        case 2: { lit = false; for (int k = 0; k < 8; ++k) if (orbit[k] == i) { lit = true; delay = k * cycle / 8.f; } break; }
        case 3: { const bool inner = i == 5 || i == 6 || i == 9 || i == 10; delay = inner ? 0.f : cycle * 0.16f; break; }
        }
        ComponentConfig cfg = ComponentConfig{}.with_size({pixels(16 * s), pixels(16 * s)}).with_absolute_position(col * 24.f * s, row * 24.f * s)
                                  .with_custom_background(base).with_corner_radius(8 * s).with_ignore_pointer_events()
                                  .with_debug_name(std::string("dot_") + std::to_string(i));
        if (lit)
          cfg.on_appear({.background = {base, on}},
                        motion::Timeline{.keys = {{0.f, 0.f}, {delay, 0.f}, {delay + 0.18f, 1.f}, {delay + 0.54f, 0.f}, {cycle + delay, 0.f}},
                                         .repeat = motion::Timeline::Repeat::Loop, .curve = motion::curves::ease_in_out_quad});
        auto dot = div(context, mk(grid.ent(), i), cfg);
        if (changed) {
          dot.ent().removeComponentIfExists<HasMotionState>();
          dot.ent().removeComponentIfExists<motion::HasTracks>();
        }
        track_motion(dot.ent());
      }
      label(stage.ent(), 20, "A 4 x 4 matrix pulses colour on a 1.2 s loop: column scan, a fixed twinkle order, a perimeter orbit, or centre-out. Timed colour changes, no movement.", 200, 160, 1000, 18, true);
      label(stage.ent(), 21, std::string("pattern: ") + pattern_names[matrix_pattern], 200, 200, 400, 20, false, "matrix_state");
    }
    else if (slug == "streaming") {
      static const std::vector<std::string> words = {"Every", "word", "resolves", "on", "its", "own,", "so", "a", "reply", "reads",
                                                     "as", "it", "arrives", "instead", "of", "landing", "all", "at", "once."};
      auto &count = motion::anim(Key::StreamCount);
      if (tab(stage.ent(), 1, "Stream", 60, 84, 140, false, "stream_btn") || (replay_stamp != seen_replay && (seen_replay = replay_stamp, true))) {
        ++stream_runs;
        stream_playing = true;
        count.from(0.f).to(float(words.size()), motion::Timeline{.keys = {{0.f, 0.f}, {0.06f * words.size(), 1.f}}})
            .on_complete([this, alive = std::weak_ptr<int>(alive)] { if (alive.expired()) return; stream_playing = false; });
      }
      const int shown = std::clamp(int(count.value()), 0, int(words.size()));
      std::string text;
      for (int i = 0; i < shown; ++i) text += (i ? " " : "") + words[i];
      auto para = div(context, mk(stage.ent(), 2),
                      box(60, 150, 1100, 40).with_label(text).with_font("AtkinsonMock", pixels(22 * s)).with_custom_text_color(ink)
                          .with_alignment(TextAlignment::Left).with_background(Theme::Usage::None).with_ignore_pointer_events()
                          .with_debug_name("stream_text")
                          .with_unit_motion({.unit = TextUnit::Word, .from_y = 0.f, .from_opacity = 0.f, .duration = 0.35f, .stagger = 0.f}));
      in_flight |= count.active() || text_units_active(para.ent());
      label(stage.ent(), 3, "A new word every 60 ms; each fades in over 350 ms, so neighbours overlap. Replays the whole paragraph; not a network parser.", 60, 220, 1100, 18, true);
      label(stage.ent(), 4, "words: " + std::to_string(shown) + "/" + std::to_string(words.size()), 60, 260, 400, 20, false, "stream_count");
      label(stage.ent(), 5, "runs: " + std::to_string(stream_runs), 60, 290, 400, 20, false, "stream_runs");
    } else if (slug == "thinking") {
      static const char *states[] = {"Thinking", "Reading the request", "Checking the sources", "Drafting a reply"};
      auto &hold = motion::anim(Key::ThinkHold);
      if (thinking_playing && !hold.active())
        hold.from(0.f).to(1.f, motion::Timeline{.keys = {{0.f, 0.f}, {2.f, 1.f}}}).on_complete([this, alive = std::weak_ptr<int>(alive)] { if (alive.expired()) return; thinking_index = (thinking_index + 1) % 4; });
      if (tab(stage.ent(), 1, thinking_playing ? "Pause" : "Play", 60, 84, 120, false, "thinking_btn")) {
        thinking_playing = !thinking_playing;
        if (!thinking_playing) hold.from(0.f);
      }
      auto line = div(context, mk(stage.ent(), 2),
                      box(60, 150, 500, 40).with_label(states[thinking_index]).with_font("AtkinsonMock", pixels(24 * s)).with_custom_text_color(muted)
                          .with_alignment(TextAlignment::Left).with_background(Theme::Usage::None).with_ignore_pointer_events()
                          .with_debug_name("thinking_text")
                          .with_unit_motion({.unit = TextUnit::Word, .from_y = 8.f * s, .from_opacity = 0.f, .duration = 0.15f, .stagger = 0.05f}));
      in_flight |= text_units_active(line.ent());
      label(stage.ent(), 3, "The status swaps every 2 s; the incoming words rise 8 px and fade in, 150 ms each with a 50 ms gap. Shimmer needs gradient text (M3).", 60, 220, 1100, 18, true);
      label(stage.ent(), 4, "state: " + std::to_string(thinking_index), 60, 260, 400, 20, false, "thinking_state");
      label(stage.ent(), 5, std::string("playing: ") + (thinking_playing ? "yes" : "no"), 60, 290, 400, 20, false, "thinking_playing");
    }
    else if (slug == "reasoning") {
      static const char *lines[] = {"Reading the brief.", "The user wants a plan, not a lecture.", "Check what already exists.",
                                    "Three helpers cover most of it.", "Two gaps remain: layout and input.", "Propose the smaller change first.",
                                    "Name the risk and the fallback.", "Done thinking; write it down."};
      constexpr int line_count = 8;
      const float line_h = 26.f * s;
      auto &offset = motion::anim(Key::ReasonOffset);
      auto &hold = motion::anim(Key::ReasonHold);
      if (!offset.started()) offset.from(0.f);
      if (reasoning_playing && !hold.active() && !offset.active()) {
        hold.from(0.f).to(1.f, motion::Timeline{.keys = {{0.f, 0.f}, {0.84f, 1.f}}}).on_complete([this, line_h, alive = std::weak_ptr<int>(alive)] { if (alive.expired()) return;
          ++reasoning_steps;
          auto &off = afterhours::motion::anim(Key::ReasonOffset);
          const float next = float((reasoning_steps * 2) % line_count) * line_h;
          if (next == 0.f) off.from(0.f);
          else off.to(next, afterhours::motion::Timeline{.keys = {{0.f, 0.f}, {0.5f, 1.f}}, .curve = afterhours::motion::curves::ease_out_quad});
        });
      }
      if (tab(stage.ent(), 1, reasoning_playing ? "Pause" : "Play", 60, 84, 120, false, "reasoning_btn")) {
        reasoning_playing = !reasoning_playing;
        if (!reasoning_playing) hold.from(0.f);
      }
      auto viewport = div(context, mk(stage.ent(), 2), box(60, 140, 560, line_h * 4.f / s).with_debug_name("reasoning_viewport")
                                                          .with_custom_background({244, 240, 246, 255}).with_corner_radius(8 * s).with_clip_children());
      for (int i = 0; i < line_count * 2; ++i)
        div(context, mk(viewport.ent(), i), ComponentConfig{}.with_size({pixels(540 * s), pixels(line_h)})
                                               .with_absolute_position(10 * s, float(i) * line_h - offset.value())
                                               .with_label(lines[i % line_count]).with_font("AtkinsonMock", pixels(16 * s)).with_custom_text_color(ink)
                                               .with_alignment(TextAlignment::Left).with_background(Theme::Usage::None).with_ignore_pointer_events()
                                               .with_debug_name(std::string("reason_line_") + std::to_string(i)));
      in_flight |= offset.active();
      label(stage.ent(), 3, "The transcript steps up two lines at a time inside a fixed viewport: 840 ms hold, 500 ms move. A duplicate copy lets it wrap without a jump.", 640, 150, 580, 18, true);
      label(stage.ent(), 4, "steps: " + std::to_string(reasoning_steps), 640, 210, 400, 20, false, "reasoning_steps");
      label(stage.ent(), 5, std::string("playing: ") + (reasoning_playing ? "yes" : "no"), 640, 240, 400, 20, false, "reasoning_playing");
    } else if (slug == "shimmer") {
      auto &sx = motion::anim(Key::ShimmerX);
      const float text_w = 320.f * s;
      if (!sx.started() || (shimmer_playing && !sx.active()))
        sx.from(-80.f * s).to(text_w, motion::Timeline{.keys = {{0.f, 0.f}, {2.f, 1.f}}, .repeat = motion::Timeline::Repeat::Loop});
      if (tab(stage.ent(), 1, shimmer_playing ? "Stop" : "Play", 60, 84, 120, false, "shimmer_btn")) {
        shimmer_playing = !shimmer_playing;
        if (!shimmer_playing) sx.from(sx.value());
      }
      auto base = div(context, mk(stage.ent(), 2), box(60, 150, text_w / s, 40).with_label("Generating your summary").with_font("AtkinsonMockBold", pixels(26 * s))
                                                       .with_custom_text_color({124, 124, 124, 255}).with_alignment(TextAlignment::Left)
                                                       .with_background(Theme::Usage::None).with_ignore_pointer_events().with_debug_name("shimmer_base"));
      auto window = div(context, mk(base.ent(), 0), ComponentConfig{}.with_size({pixels(80 * s), pixels(40 * s)}).with_absolute_position(sx.value(), 0.f)
                                                        .with_background(Theme::Usage::None).with_clip_children().with_ignore_pointer_events().with_debug_name("shimmer_window"));
      div(context, mk(window.ent(), 0), ComponentConfig{}.with_size({pixels(text_w), pixels(40 * s)}).with_absolute_position(-sx.value(), 0.f)
                                            .with_label("Generating your summary").with_font("AtkinsonMockBold", pixels(26 * s)).with_custom_text_color({13, 13, 13, 255})
                                            .with_alignment(TextAlignment::Left).with_background(Theme::Usage::None).with_ignore_pointer_events().with_debug_name("shimmer_highlight"));
      in_flight |= sx.active();
      label(stage.ent(), 3, "A narrow highlight window sweeps across a stationary label every 2 s: the same text in the highlight colour, clipped to the window and offset back so it stays aligned.", 60, 220, 1100, 18, true);
      label(stage.ent(), 4, std::string("playing: ") + (shimmer_playing ? "yes" : "no"), 60, 260, 400, 20, false, "shimmer_playing");
    }
    else if (slug == "counter") {
      const float cell_h = 30.f * s;
      const auto spin_to = [&](int value) {
        counter_value = value;
        for (int col = 0; col < 4; ++col) {
          const int digit = (value / int(std::pow(10, 3 - col))) % 10;
          auto &reel = motion::anim(static_cast<Key>(static_cast<size_t>(Key::Reel0) + col));
          if (!reel.started()) reel.from(0.f);
          reel.from(0.f).to(float(3 * 10 + digit) * cell_h,
                            motion::Timeline{.keys = {{0.f, 0.f}, {1.4f, 1.f}}, .curve = motion::curves::ease_out_cubic})
              .delay(0.09f * col);
        }
      };
      if (tab(stage.ent(), 1, "Spin", 60, 84, 120, false, "counter_btn") || (replay_stamp != seen_replay && (seen_replay = replay_stamp, true)))
        spin_to((counter_value * 7 + 1234) % 10000);
      for (int col = 0; col < 4; ++col) {
        auto &reel = motion::anim(static_cast<Key>(static_cast<size_t>(Key::Reel0) + col));
        if (!reel.started()) reel.from(0.f);
        auto cell = div(context, mk(stage.ent(), 10 + col), box(60 + col * 34.f, 150, 30, 30).with_debug_name(std::string("reel_") + std::to_string(col))
                                                              .with_custom_background({244, 240, 246, 255}).with_corner_radius(6 * s).with_clip_children());
        std::string strip;
        for (int k = 0; k < 40; ++k) strip += std::to_string(k % 10) + "\n";
        div(context, mk(cell.ent(), 0), ComponentConfig{}.with_size({pixels(30 * s), pixels(cell_h * 40.f)}).with_absolute_position(0.f, -reel.value())
                                            .with_label(strip).with_font("AtkinsonMockBold", pixels(22 * s)).with_custom_text_color(ink)
                                            .with_alignment(TextAlignment::Center).with_background(Theme::Usage::None).with_ignore_pointer_events()
                                            .with_debug_name(std::string("reel_strip_") + std::to_string(col)));
        in_flight |= reel.active();
      }
      label(stage.ent(), 3, "Each digit rolls up through three full turns of a clipped 0-9 strip before landing; columns start 90 ms apart, 1.4 s each.", 240, 150, 950, 18, true);
      label(stage.ent(), 4, "value: " + std::to_string(counter_value), 240, 190, 400, 20, false, "counter_value");
    } else if (slug == "popin") {
      if (tab(stage.ent(), 1, "+1", 60, 84, 90, false, "popin_add")) ++popin_value;
      if (tab(stage.ent(), 2, "+100", 160, 84, 90, false, "popin_hundred")) popin_value += 100;
      if (tab(stage.ent(), 3, "x10", 260, 84, 90, false, "popin_tenfold")) popin_value = (popin_value * 10) % 1000000;
      auto number = div(context, mk(stage.ent(), 4),
                        box(60, 150, 600, 60).with_label(std::to_string(popin_value)).with_font("AtkinsonMockBold", pixels(48 * s)).with_custom_text_color(ink)
                            .with_alignment(TextAlignment::Left).with_background(Theme::Usage::None).with_ignore_pointer_events()
                            .with_debug_name("popin_number")
                            .with_unit_motion({.unit = TextUnit::Char, .from_y = 8.f * s, .from_opacity = 0.f, .duration = 0.5f, .stagger = 0.07f}));
      in_flight |= text_units_active(number.ent());
      label(stage.ent(), 5, "Only the digits that changed drop in from 8 px above; unchanged ones sit still. 500 ms each, 70 ms stagger among the changed.", 60, 240, 1100, 18, true);
      label(stage.ent(), 6, "value: " + std::to_string(popin_value), 60, 280, 400, 20, false, "popin_value");
    }
    else if (slug == "effect") {
      if (!tint_effect) tint_effect.emplace(afterhours::effects::Effect::load("tint"));
      if (tab(stage.ent(), 1, "Reload shaders", 60, 84, 180, false, "shader_reload_btn")) shader_reloads += afterhours::effects::reload_all();
      if (tab(stage.ent(), 2, tint_strength > 0.5f ? "Tint: on" : "Tint: off", 260, 84, 140, false, "tint_toggle")) tint_strength = tint_strength > 0.5f ? 0.f : 1.f;
      auto *fx = &*tint_effect;
      const float strength = tint_strength;
      div(context, mk(stage.ent(), 3), box(60, 150, 320, 180).with_debug_name("effect_canvas").with_corner_radius(12 * s)
                                            .with_custom_background({244, 240, 246, 255}).with_ignore_pointer_events()
                                            .with_on_draw_fg([fx, strength, s](RectangleType r) {
                                              fx->set("tint", ColorType{80, 120, 220, static_cast<unsigned char>(255 * strength)});
                                              afterhours::effects::Effect::Scope scope(*fx);
                                              const float pad = 20.f * s;
                                              afterhours::draw_rectangle({r.x + pad, r.y + pad, r.width - 2 * pad, r.height - 2 * pad}, {230, 150, 120, 255});
                                              afterhours::draw_circle_v({r.x + r.width * 0.5f, r.y + r.height * 0.5f}, 40.f * s, {130, 200, 150, 255});
                                            }));
      label(stage.ent(), 4, "Everything drawn inside Effect::Scope goes through resources/shaders/tint.fs; the uniform is set per frame. Reload re-reads the file from disk.", 400, 160, 800, 18, true);
      label(stage.ent(), 5, std::string("effect: ") + (tint_effect->ok() ? "ok" : "missing"), 400, 210, 400, 20, false, "effect_state");
      label(stage.ent(), 6, "reloads: " + std::to_string(shader_reloads), 400, 240, 400, 20, false, "effect_reloads");
    }
    label(stage.ent(), 9, std::string("motion: ") + (in_flight ? "in flight" : "settled"), 60, 400, 400, 20, false, "motion_state");
  }
};

REGISTER_EXAMPLE_SCREEN(transitions_lab, "Animations",
                        "Transitions Lab: the 43 transitions on working controls",
                        TransitionsLab)
