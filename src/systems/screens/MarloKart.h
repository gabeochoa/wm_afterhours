#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/ui/grid.h>
#include <algorithm>
#include <array>
#include <cmath>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct MarloKartScreen : ScreenSystem<UIContext<InputAction>> {
  static constexpr int kRacers = 8;
  static constexpr int kLaps = 3;
  static constexpr int kCupRaces = 4;
  static constexpr float kFixedStep = 1.f / 120.f;

  enum struct Phase { Title, Driver, Cup, Race, Results, Trophy };
  enum struct Item { None, Boost, Shell, Peel, Bolt };

  struct Driver {
    const char *name;
    float speed;
    float accel;
    float grip;
    float weight;
    afterhours::Color color;
  };

  struct Kart {
    const char *name;
    float speed;
    float accel;
    float grip;
    float weight;
  };

  struct Cup {
    const char *name;
    afterhours::Color color;
    std::array<const char *, kCupRaces> tracks;
  };

  struct Racer {
    int driver = 0;
    float prog = 0.f;
    float speed = 0.f;
    float slow = 0.f;
    float boost = 0.f;
    int coins = 0;
    Item item = Item::None;
    float finish_time = -1.f;
    float next_coin = 0.1f;
    float next_box = 0.25f;
  };

  std::array<Driver, kRacers> drivers{{
      {"Marlo", 0.62f, 0.70f, 0.72f, 0.55f, {228, 62, 62, 255}},
      {"Luca", 0.70f, 0.60f, 0.62f, 0.62f, {60, 180, 96, 255}},
      {"Pip", 0.42f, 0.95f, 0.92f, 0.22f, {255, 176, 64, 255}},
      {"Bruno", 0.92f, 0.32f, 0.40f, 0.96f, {122, 96, 208, 255}},
      {"Zaria", 0.80f, 0.55f, 0.58f, 0.70f, {236, 96, 178, 255}},
      {"Koda", 0.50f, 0.82f, 0.86f, 0.34f, {64, 196, 214, 255}},
      {"Tessa", 0.74f, 0.66f, 0.78f, 0.48f, {246, 214, 72, 255}},
      {"Ozzy", 0.96f, 0.28f, 0.34f, 1.00f, {150, 132, 110, 255}},
  }};

  std::array<Kart, 4> karts{{
      {"Standard", 0.5f, 0.5f, 0.5f, 0.5f},
      {"Speedster", 0.9f, 0.3f, 0.35f, 0.6f},
      {"Rally", 0.45f, 0.7f, 0.9f, 0.4f},
      {"Bruiser", 0.75f, 0.35f, 0.4f, 0.95f},
  }};

  std::array<Cup, 4> cups{{
      {"Acorn Cup",
       {124, 196, 84, 255},
       {"Sunset Loop", "Coral Bay", "Piston Alley", "Fern Hollow"}},
      {"Bolt Cup",
       {252, 196, 60, 255},
       {"Voltway", "Dune Drift", "Clocktower Run", "Iron Falls"}},
      {"Comet Cup",
       {96, 176, 246, 255},
       {"Glacier Pass", "Neon Docks", "Quarry Spiral", "Tidal Flats"}},
      {"Delta Cup",
       {214, 106, 232, 255},
       {"Ember Ridge", "Sky Terrace", "Undertow", "Rainbow Rift"}},
  }};

  static constexpr std::array<int, kRacers> kPoints{15, 12, 10, 8, 6, 4, 2, 1};
  static constexpr std::array<const char *, 3> kEngineNames{"50cc", "100cc",
                                                            "150cc"};
  static constexpr std::array<float, 3> kEngineMult{0.74f, 0.87f, 1.0f};

  struct TrackShape {
    static constexpr float w = 1.0f;
    float h = 0.62f;
    float r = 0.18f;

    static TrackShape for_track(int cup, int track) {
      const int k = cup * kCupRaces + track;
      return TrackShape{0.46f + 0.055f * (float)(k % 6),
                        0.09f + 0.022f * (float)(k % 8)};
    }

    float sx() const { return w - 2.f * r; }
    float sy() const { return h - 2.f * r; }
    float arc() const { return 1.57079633f * r; }
    float total() const { return 2.f * sx() + 2.f * sy() + 4.f * arc(); }

    bool is_corner(float frac) const {
      float s = std::fmod(frac, 1.f) * total();
      if (s < 0.f)
        s += total();
      if (s < sx())
        return false;
      s -= sx();
      if (s < arc())
        return true;
      s -= arc();
      if (s < sy())
        return false;
      s -= sy();
      if (s < arc())
        return true;
      s -= arc();
      if (s < sx())
        return false;
      s -= sx();
      if (s < arc())
        return true;
      s -= arc();
      return s >= sy();
    }

    afterhours::Vector2Type unit_at(float frac) const {
      float s = std::fmod(frac, 1.f) * total();
      if (s < 0.f)
        s += total();
      const float a = arc();
      if (s < sx())
        return {r + s, 0.f};
      s -= sx();
      if (s < a) {
        const float t = (s / a) * 1.57079633f - 1.57079633f;
        return {w - r + r * std::cos(t), r + r * std::sin(t)};
      }
      s -= a;
      if (s < sy())
        return {w, r + s};
      s -= sy();
      if (s < a) {
        const float t = (s / a) * 1.57079633f;
        return {w - r + r * std::cos(t), h - r + r * std::sin(t)};
      }
      s -= a;
      if (s < sx())
        return {w - r - s, h};
      s -= sx();
      if (s < a) {
        const float t = 1.57079633f + (s / a) * 1.57079633f;
        return {r + r * std::cos(t), h - r + r * std::sin(t)};
      }
      s -= a;
      if (s < sy())
        return {0.f, h - r - s};
      s -= sy();
      const float t = 3.14159265f + (s / a) * 1.57079633f;
      return {r + r * std::cos(t), r + r * std::sin(t)};
    }

    afterhours::Vector2Type map(float frac, RectangleType rr) const {
      const float scale = std::min(rr.width / w, rr.height / h);
      const float ox = rr.x + (rr.width - w * scale) * 0.5f;
      const float oy = rr.y + (rr.height - h * scale) * 0.5f;
      const auto p = unit_at(frac);
      return {ox + p.x * scale, oy + p.y * scale};
    }
  };

  static Size strict_w1280(float px) { return screen_pct(px / 1280.f, 1.f); }

  Phase phase = Phase::Title;
  int engine = 2;
  int driver_idx = 0;
  int kart_idx = 0;
  int cup_idx = 0;
  int race_in_cup = 0;
  int nav_row = 0;
  bool paused = false;

  std::array<Racer, kRacers> racers{};
  std::array<int, kRacers> points{};
  float race_time = 0.f;
  float countdown = 0.f;
  float accum = 0.f;
  bool drifting = false;
  float drift_charge = 0.f;
  std::array<int, kRacers> last_order{};
  std::array<int, kRacers> last_points{};

  float stat(int which) const {
    const Driver &d = drivers[(size_t)driver_idx];
    const Kart &k = karts[(size_t)kart_idx];
    const float raw = which == 0   ? (d.speed * 0.6f + k.speed * 0.4f)
                      : which == 1 ? (d.accel * 0.6f + k.accel * 0.4f)
                      : which == 2 ? (d.grip * 0.6f + k.grip * 0.4f)
                                   : (d.weight * 0.6f + k.weight * 0.4f);
    return std::clamp(raw, 0.05f, 1.f);
  }

  const char *track_name() const {
    return cups[(size_t)cup_idx].tracks[(size_t)race_in_cup];
  }

  void start_cup() {
    points.fill(0);
    race_in_cup = 0;
    start_race();
  }

  void start_race() {
    for (int i = 0; i < kRacers; i++) {
      racers[(size_t)i] = Racer{};
      racers[(size_t)i].driver = i;
    }
    std::swap(racers[0].driver, racers[(size_t)driver_idx].driver);
    race_time = 0.f;
    countdown = 2.6f;
    accum = 0.f;
    drifting = false;
    drift_charge = 0.f;
    paused = false;
    phase = Phase::Race;
  }

  TrackShape track_shape() const {
    return TrackShape::for_track(cup_idx, race_in_cup);
  }

  float top_speed_for(const Racer &r) const {
    const Driver &d = drivers[(size_t)r.driver];
    float sp = d.speed, gr = d.grip;
    if (&r == &racers[0]) {
      const Kart &k = karts[(size_t)kart_idx];
      sp = sp * 0.6f + k.speed * 0.4f;
      gr = gr * 0.6f + k.grip * 0.4f;
    }
    float top = (0.215f + 0.055f * sp) * kEngineMult[(size_t)engine];
    top *= 1.f + 0.006f * (float)r.coins;
    const bool corner = track_shape().is_corner(r.prog);
    if (corner)
      top *= 0.78f + 0.16f * gr;
    return top;
  }

  float accel_rate_for(const Racer &r) const {
    const Driver &d = drivers[(size_t)r.driver];
    float ac = d.accel;
    if (&r == &racers[0])
      ac = ac * 0.6f + karts[(size_t)kart_idx].accel * 0.4f;
    return 1.6f + 3.2f * ac;
  }

  int position_of(int racer) const {
    auto order = running_order();
    for (int i = 0; i < kRacers; i++)
      if (order[(size_t)i] == racer)
        return i + 1;
    return kRacers;
  }

  std::array<int, kRacers> running_order() const {
    std::array<int, kRacers> order{};
    for (int i = 0; i < kRacers; i++)
      order[(size_t)i] = i;
    std::stable_sort(order.begin(), order.end(), [&](int a, int b) {
      const Racer &ra = racers[(size_t)a];
      const Racer &rb = racers[(size_t)b];
      const bool fa = ra.finish_time >= 0.f;
      const bool fb = rb.finish_time >= 0.f;
      if (fa != fb)
        return fa;
      if (fa && fb)
        return ra.finish_time < rb.finish_time;
      return ra.prog > rb.prog;
    });
    return order;
  }

  bool race_over() const {
    for (const Racer &r : racers)
      if (r.finish_time < 0.f)
        return false;
    return true;
  }

  Item roll_item(int place) const {
    if (place <= 1)
      return Item::Peel;
    if (place <= 3)
      return Item::Shell;
    if (place <= 5)
      return Item::Boost;
    return Item::Bolt;
  }

  static const char *item_name(Item it) {
    switch (it) {
    case Item::Boost:
      return "TURBO";
    case Item::Shell:
      return "HOMER";
    case Item::Peel:
      return "PEEL";
    case Item::Bolt:
      return "BOLT";
    default:
      return "EMPTY";
    }
  }

  static afterhours::Color item_color(Item it) {
    switch (it) {
    case Item::Boost:
      return {252, 120, 80, 255};
    case Item::Shell:
      return {96, 196, 246, 255};
    case Item::Peel:
      return {248, 212, 72, 255};
    case Item::Bolt:
      return {206, 126, 250, 255};
    default:
      return {96, 104, 128, 255};
    }
  }

  void use_item(int who) {
    Racer &r = racers[(size_t)who];
    const Item it = r.item;
    if (it == Item::None)
      return;
    r.item = Item::None;
    auto order = running_order();
    int place = 0;
    for (int i = 0; i < kRacers; i++)
      if (order[(size_t)i] == who)
        place = i;

    switch (it) {
    case Item::Boost:
      r.boost = std::max(r.boost, 1.3f);
      break;
    case Item::Shell:
      if (place > 0)
        racers[(size_t)order[(size_t)place - 1]].slow = 1.3f;
      break;
    case Item::Peel:
      if (place + 1 < kRacers)
        racers[(size_t)order[(size_t)place + 1]].slow = 1.1f;
      break;
    case Item::Bolt:
      for (int i = 0; i < kRacers; i++)
        if (i != who && racers[(size_t)i].finish_time < 0.f)
          racers[(size_t)i].slow = 0.9f;
      break;
    default:
      break;
    }
  }

  void step(float dt) {
    if (countdown > 0.f) {
      countdown -= dt;
      return;
    }
    race_time += dt;

    const float lead = racers[0].prog;
    for (int i = 0; i < kRacers; i++) {
      Racer &r = racers[(size_t)i];
      if (r.finish_time >= 0.f)
        continue;

      float top = top_speed_for(r);
      const bool corner = track_shape().is_corner(r.prog);

      if (i == 0) {
        if (drifting) {
          if (corner) {
            drift_charge += dt;
            top *= 1.04f;
          } else {
            drift_charge = std::max(0.f, drift_charge - dt * 2.f);
            top *= 0.86f;
          }
        }
      } else {
        const Driver &d = drivers[(size_t)r.driver];
        const float skill = 0.94f + 0.05f * d.grip;
        top *= skill;
        const float band = std::clamp(0.05f * (lead - r.prog), -0.05f, 0.07f);
        top *= 1.f + band;
        if (r.item != Item::None && r.prog > 0.35f)
          use_item(i);
      }

      if (r.boost > 0.f) {
        top *= 1.38f;
        r.boost -= dt;
      }
      if (r.slow > 0.f) {
        top *= 0.42f;
        r.slow -= dt;
      }

      r.speed += (top - r.speed) * accel_rate_for(r) * dt;
      r.prog += r.speed * dt;

      while (r.coins < 10 && r.prog >= r.next_coin) {
        r.coins++;
        r.next_coin += 0.1f;
      }
      while (r.prog >= r.next_box) {
        r.next_box += 0.25f;
        if (r.item == Item::None)
          r.item = roll_item(position_of(i));
      }

      if (r.prog >= (float)kLaps) {
        r.prog = (float)kLaps;
        r.finish_time = race_time;
      }
    }

    if (race_over())
      finish_race();
  }

  void finish_race() {
    last_order = running_order();
    last_points.fill(0);
    for (int i = 0; i < kRacers; i++) {
      const int who = last_order[(size_t)i];
      last_points[(size_t)who] = kPoints[(size_t)i];
      points[(size_t)who] += kPoints[(size_t)i];
    }
    phase = Phase::Results;
  }

  void release_drift() {
    if (drift_charge >= 2.4f)
      racers[0].boost = std::max(racers[0].boost, 2.0f);
    else if (drift_charge >= 1.5f)
      racers[0].boost = std::max(racers[0].boost, 1.3f);
    else if (drift_charge >= 0.7f)
      racers[0].boost = std::max(racers[0].boost, 0.8f);
    drift_charge = 0.f;
  }

  static std::string fmt_time(float t) {
    if (t < 0.f)
      return "--:--.---";
    const int m = (int)(t / 60.f);
    const float s = t - (float)m * 60.f;
    return fmt::format("{}:{:06.3f}", m, s);
  }

  static std::string ordinal(int n) {
    static constexpr std::array<const char *, 9> suf{"th", "st", "nd", "rd",
                                                     "th", "th", "th", "th",
                                                     "th"};
    return fmt::format("{}{}", n, suf[(size_t)std::clamp(n, 0, 8)]);
  }

  afterhours::Color bg{14, 18, 34, 255};
  afterhours::Color panel{26, 33, 58, 255};
  afterhours::Color panel_hi{38, 48, 82, 255};
  afterhours::Color line{62, 78, 128, 255};
  afterhours::Color ink{240, 245, 255, 255};
  afterhours::Color ink_dim{160, 174, 210, 255};
  afterhours::Color gold{252, 206, 70, 255};
  afterhours::Color go_green{96, 214, 124, 255};
  afterhours::Color deep{10, 13, 26, 255};

  void apply_theme(UIContext<InputAction> &context) {
    Theme theme;
    theme.font = ink;
    theme.darkfont = deep;
    theme.font_muted = ink_dim;
    theme.background = bg;
    theme.surface = panel;
    theme.primary = afterhours::Color{96, 196, 246, 255};
    theme.secondary = afterhours::Color{30, 38, 66, 255};
    theme.accent = gold;
    theme.error = afterhours::Color{228, 82, 82, 255};
    theme.corner_radius = 10.f;
    theme.roundness = 0.f;
    theme.segments = 8;
    context.theme = theme;
    context.scaling_mode = ScalingMode::Adaptive;
    UIStylingDefaults::get().set_default_font("EqProRounded", h720(19.f));
  }

  ElementResult page(UIContext<InputAction> &context,
                     afterhours::Entity &entity, const char *name) {
    return vstack(context, mk(entity),
                  ComponentConfig{}
                      .with_size(ComponentSize{percent(1.f), percent(1.f)})
                      .with_custom_background(bg)
                      .with_corner_radius(0.f)
                      .with_padding(Spacing::md)
                      .with_gap(h720(12.f))
                      .with_no_wrap()
                      .with_debug_name(name));
  }

  ElementResult chip(UIContext<InputAction> &context, afterhours::Entity &par,
                     int idx, const std::string &text, afterhours::Color fg,
                     float w, const std::string &dbg) {
    return div(context, mk(par, idx),
               ComponentConfig{}
                   .with_label(text)
                   .with_size(ComponentSize{h720(w), h720(34.f)})
                   .with_custom_background(panel_hi)
                   .with_custom_text_color(fg)
                   .with_alignment(TextAlignment::Center)
                   .with_font_size(h720(17.f))
                   .with_corner_radius(8.f)
                   .with_debug_name(dbg));
  }

  void header(UIContext<InputAction> &context, afterhours::Entity &root,
              int idx, const std::string &title, const std::string &right) {
    auto bar = hstack(context, mk(root, idx),
                      ComponentConfig{}
                          .with_size(ComponentSize{percent(1.f), h720(48.f)})
                          .with_align_items(AlignItems::Center)
                          .with_gap(h720(10.f))
                          .with_no_wrap()
                          .with_debug_name("mk_header"));

    div(context, mk(bar.ent(), 0),
        ComponentConfig{}
            .with_label(title)
            .with_size(ComponentSize{expand(), h720(44.f)})
            .with_background(Theme::Usage::None)
            .with_custom_text_color(gold)
            .with_font("Fredoka", h720(30.f))
            .with_debug_name("mk_title"));

    div(context, mk(bar.ent(), 1),
        ComponentConfig{}
            .with_label(right)
            .with_size(ComponentSize{strict_w1280(260.f), h720(34.f)})
            .with_background(Theme::Usage::None)
            .with_custom_text_color(ink_dim)
            .with_alignment(TextAlignment::Right)
            .with_font_size(h720(17.f))
            .with_debug_name("mk_header_right"));
  }

  void advance_race() {
    if (race_in_cup + 1 >= kCupRaces) {
      phase = Phase::Trophy;
    } else {
      race_in_cup++;
      start_race();
    }
  }

  void toggle_drift() {
    if (drifting) {
      drifting = false;
      release_drift();
    } else {
      drifting = true;
    }
  }

  void handle_nav(UIContext<InputAction> &context) {
    const bool left = context.pressed_or_repeat(InputAction::WidgetLeft);
    const bool right = context.pressed_or_repeat(InputAction::WidgetRight);
    const bool up = context.pressed_or_repeat(InputAction::WidgetUp);
    const bool down = context.pressed_or_repeat(InputAction::WidgetDown);
    const bool confirm = context.pressed(InputAction::WidgetPress);
    const bool back = context.pressed(InputAction::MenuBack);
    const bool alt = context.pressed(InputAction::WidgetMod);

    switch (phase) {
    case Phase::Title:
      if (left)
        engine = (engine + 2) % 3;
      if (right)
        engine = (engine + 1) % 3;
      if (confirm)
        phase = Phase::Driver;
      break;

    case Phase::Driver: {
      int row = nav_row;
      int col = row < 2 ? driver_idx % 4 : kart_idx;
      if (left)
        col = (col + 3) % 4;
      if (right)
        col = (col + 1) % 4;
      if (up)
        row = (row + 2) % 3;
      if (down)
        row = (row + 1) % 3;
      nav_row = row;
      if (row < 2)
        driver_idx = row * 4 + col;
      else
        kart_idx = col;
      if (confirm)
        phase = Phase::Cup;
      if (back)
        phase = Phase::Title;
      break;
    }

    case Phase::Cup: {
      int row = cup_idx / 2;
      int col = cup_idx % 2;
      if (left || right)
        col = (col + 1) % 2;
      if (up || down)
        row = (row + 1) % 2;
      cup_idx = row * 2 + col;
      if (confirm)
        start_cup();
      if (back)
        phase = Phase::Driver;
      break;
    }

    case Phase::Race:
      if (back)
        paused = !paused;
      if (confirm) {
        if (paused)
          paused = false;
        else
          toggle_drift();
      }
      if (alt && !paused)
        use_item(0);
      break;

    case Phase::Results:
      if (confirm)
        advance_race();
      if (back)
        phase = Phase::Title;
      break;

    case Phase::Trophy:
      if (confirm || back)
        phase = Phase::Title;
      break;
    }
  }

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context,
                     float dt) override {
    apply_theme(context);
    handle_nav(context);

    if (phase == Phase::Race && !paused) {
      accum += std::min(dt, 0.1f);
      while (accum >= kFixedStep) {
        accum -= kFixedStep;
        step(kFixedStep);
        if (phase != Phase::Race)
          break;
      }
    }

    switch (phase) {
    case Phase::Title:
      build_title(entity, context);
      break;
    case Phase::Driver:
      build_driver(entity, context);
      break;
    case Phase::Cup:
      build_cup(entity, context);
      break;
    case Phase::Race:
      build_race(entity, context);
      break;
    case Phase::Results:
      build_results(entity, context);
      break;
    case Phase::Trophy:
      build_trophy(entity, context);
      break;
    }
  }

  void build_title(afterhours::Entity &entity, UIContext<InputAction> &context) {
    auto root = vstack(
        context, mk(entity),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.f), percent(1.f)})
            .with_background(Theme::Usage::None)
            .with_corner_radius(0.f)
            .with_padding(Spacing::md)
            .with_no_wrap()
            .with_on_draw_bg([this](RectangleType rr) {
              afterhours::draw_rectangle_gradient_v(
                  rr, afterhours::Color{22, 32, 72, 255}, deep);
              const float cell = rr.height / 16.f;
              for (int row = 0; (float)row * cell < rr.height; row++)
                for (int col = 0; (float)col * cell < rr.width; col++)
                  if ((row + col) % 2 == 0)
                    afterhours::draw_rectangle(
                        {rr.x + (float)col * cell, rr.y + (float)row * cell,
                         cell, cell},
                        afterhours::Color{255, 255, 255, 6});
            })
            .with_debug_name("mk_title_page"));

    div(context, mk(root.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.f), expand(0.9f)})
            .with_background(Theme::Usage::None));

    div(context, mk(root.ent(), 1),
        ComponentConfig{}
            .with_label("MARLO KART")
            .with_size(ComponentSize{percent(1.f), h720(84.f)})
            .with_background(Theme::Usage::None)
            .with_alignment(TextAlignment::Center)
            .with_font("BlackOpsOne", h720(74.f))
            .with_custom_text_color(gold)
            .with_text_shadow(afterhours::Color{120, 58, 8, 255}, 4.f, 5.f)
            .with_debug_name("mk_logo"));

    div(context, mk(root.ent(), 2),
        ComponentConfig{}
            .with_label("AFTERHOURS GRAND PRIX")
            .with_size(ComponentSize{percent(1.f), h720(40.f)})
            .with_background(Theme::Usage::None)
            .with_alignment(TextAlignment::Center)
            .with_custom_text_color(ink_dim)
            .with_letter_spacing(3.f)
            .with_font_size(h720(19.f))
            .with_debug_name("mk_tagline"));

    auto eng = hstack(context, mk(root.ent(), 3),
                      ComponentConfig{}
                          .with_size(ComponentSize{percent(1.f), h720(56.f)})
                          .with_justify_content(JustifyContent::Center)
                          .with_align_items(AlignItems::Center)
                          .with_gap(h720(12.f))
                          .with_no_wrap()
                          .with_debug_name("mk_engine_row"));

    for (int i = 0; i < 3; i++) {
      const bool on = i == engine;
      if (button(context, mk(eng.ent(), i),
                 ComponentConfig{}
                     .with_label(kEngineNames[(size_t)i])
                     .with_size(ComponentSize{h720(120.f), h720(46.f)})
                     .with_custom_background(on ? gold : panel_hi)
                     .with_custom_text_color(on ? deep : ink)
                     .with_alignment(TextAlignment::Center)
                     .with_font("Fredoka", h720(24.f))
                     .with_corner_radius(10.f)
                     .with_click_activation(ClickActivationMode::Release)
                     .with_debug_name(fmt::format("mk_engine_{}", i)))) {
        engine = i;
      }
    }

    auto cta = hstack(context, mk(root.ent(), 4),
                      ComponentConfig{}
                          .with_size(ComponentSize{percent(1.f), h720(100.f)})
                          .with_justify_content(JustifyContent::Center)
                          .with_align_items(AlignItems::Center)
                          .with_no_wrap()
                          .with_debug_name("mk_cta_row"));

    if (button(context, mk(cta.ent(), 0),
               ComponentConfig{}
                   .with_label("START")
                   .with_size(ComponentSize{h720(260.f), h720(64.f)})
                   .with_custom_background(go_green)
                   .with_custom_text_color(deep)
                   .with_alignment(TextAlignment::Center)
                   .with_font("Fredoka", h720(34.f))
                   .with_corner_radius(14.f)
                   .with_shadow(ShadowStyle::Soft, 0.f, 6.f)
                   .with_click_activation(ClickActivationMode::Release)
                   .with_debug_name("mk_start"))) {
      phase = Phase::Driver;
    }

    div(context, mk(root.ent(), 5),
        ComponentConfig{}
            .with_label("Left and Right pick the class, Enter starts. "
                        "Four tracks, one trophy.")
            .with_size(ComponentSize{percent(1.f), h720(26.f)})
            .with_background(Theme::Usage::None)
            .with_alignment(TextAlignment::Center)
            .with_custom_text_color(ink_dim)
            .with_font_size(h720(16.f))
            .with_debug_name("mk_title_hint"));

    div(context, mk(root.ent(), 6),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.f), expand(1.f)})
            .with_background(Theme::Usage::None));
  }

  void build_driver(afterhours::Entity &entity,
                    UIContext<InputAction> &context) {
    auto root = page(context, entity, "mk_driver_page");
    header(context, root.ent(), 0, "SELECT DRIVER",
           fmt::format("{}  -  Grand Prix", kEngineNames[(size_t)engine]));

    auto body = hstack(context, mk(root.ent(), 1),
                       ComponentConfig{}
                           .with_size(ComponentSize{percent(1.f), expand()})
                           .with_gap(h720(12.f))
                           .with_no_wrap()
                           .with_debug_name("mk_driver_body"));

    auto left = vstack(context, mk(body.ent(), 0),
                       ComponentConfig{}
                           .with_size(ComponentSize{expand(1.7f), percent(1.f)})
                           .with_gap(h720(10.f))
                           .with_no_wrap()
                           .with_debug_name("mk_driver_left"));

    auto grid_panel =
        grid(context, mk(left.ent(), 0),
             GridConfig{}
                 .with_rows(2)
                 .with_cols(4)
                 .with_row_height(h720(148.f))
                 .with_gap(h720(8.f)),
             ComponentConfig{}
                 .with_size(ComponentSize{percent(1.f), h720(340.f)})
                 .with_custom_background(panel)
                 .with_padding(Spacing::sm)
                 .with_corner_radius(12.f)
                 .with_no_wrap()
                 .with_debug_name("mk_driver_grid"));

    for (int i = 0; i < kRacers; i++) {
      afterhours::OptEntity row_opt = grid_row(grid_panel, i / 4);
      if (!row_opt.valid())
        continue;
      afterhours::Entity &row = row_opt.asE();
      const Driver &d = drivers[(size_t)i];
      const bool on = i == driver_idx;
      const bool cursor = on && nav_row < 2;

      auto cell =
          button(context, mk(row, i % 4),
                 ComponentConfig{}
                     .with_size(ComponentSize{grid_track(grid_panel, i % 4).x_axis,
                                              h720(144.f)})
                     .with_custom_background(cursor ? ink
                                             : on    ? gold
                                                     : panel_hi)
                     .with_border(cursor ? gold
                                  : on   ? afterhours::Color{255, 240, 180, 255}
                                         : line,
                                  cursor ? 5.f : on ? 2.f : 1.f)
                     .with_corner_radius(10.f)
                     .with_click_activation(ClickActivationMode::Release)
                     .with_debug_name(fmt::format("mk_driver_{}", i)));
      if (cell) {
        driver_idx = i;
        nav_row = i / 4;
      }

      const afterhours::Color dc = d.color;
      div(context, mk(cell.ent(), 0),
          ComponentConfig{}
              .with_size(ComponentSize{percent(1.f), expand()})
              .with_background(Theme::Usage::None)
              .with_ignore_pointer_events()
              .with_on_draw_fg([dc](RectangleType rr) {
                const float rad = std::min(rr.width, rr.height) * 0.34f;
                const afterhours::Vector2Type c{rr.x + rr.width * 0.5f,
                                                rr.y + rr.height * 0.46f};
                afterhours::draw_circle_v(c, rad + 3.f,
                                          afterhours::Color{0, 0, 0, 70});
                afterhours::draw_circle_v(c, rad, dc);
                afterhours::draw_circle_v({c.x, c.y + rad * 0.42f}, rad * 0.5f,
                                          afterhours::Color{255, 255, 255, 45});
              })
              .with_debug_name(fmt::format("mk_driver_face_{}", i)));

      div(context, mk(cell.ent(), 1),
          ComponentConfig{}
              .with_label(d.name)
              .with_size(ComponentSize{percent(1.f), h720(26.f)})
              .with_background(Theme::Usage::None)
              .with_alignment(TextAlignment::Center)
              .with_custom_text_color((on || cursor) ? deep : ink)
              .with_font("Fredoka", h720(19.f))
              .with_ignore_pointer_events());
    }

    auto side = vstack(context, mk(body.ent(), 1),
                       ComponentConfig{}
                           .with_size(ComponentSize{expand(1.f), percent(1.f)})
                           .with_custom_background(panel)
                           .with_padding(Spacing::sm)
                           .with_gap(h720(8.f))
                           .with_corner_radius(12.f)
                           .with_no_wrap()
                           .with_debug_name("mk_driver_side"));

    const Driver &sel = drivers[(size_t)driver_idx];
    const afterhours::Color sel_col = sel.color;
    div(context, mk(side.ent(), 9),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.f), h720(150.f)})
            .with_background(Theme::Usage::None)
            .with_on_draw_fg([sel_col](RectangleType rr) {
              const afterhours::Vector2Type c{rr.x + rr.width * 0.5f,
                                              rr.y + rr.height * 0.5f};
              const float rad = std::min(rr.width, rr.height) * 0.44f;
              afterhours::draw_circle_v(c, rad + 6.f,
                                        afterhours::Color{252, 206, 70, 90});
              afterhours::draw_circle_v(c, rad, sel_col);
              afterhours::draw_circle_v({c.x, c.y + rad * 0.45f}, rad * 0.52f,
                                        afterhours::Color{255, 255, 255, 45});
            })
            .with_debug_name("mk_driver_portrait"));

    div(context, mk(side.ent(), 0),
        ComponentConfig{}
            .with_label(sel.name)
            .with_size(ComponentSize{percent(1.f), h720(38.f)})
            .with_background(Theme::Usage::None)
            .with_alignment(TextAlignment::Center)
            .with_custom_text_color(gold)
            .with_font("Fredoka", h720(28.f))
            .with_debug_name("mk_driver_name"));

    static constexpr std::array<const char *, 4> stat_names{"SPEED", "ACCEL",
                                                            "GRIP", "WEIGHT"};
    for (int i = 0; i < 4; i++) {
      auto row = hstack(context, mk(side.ent(), 1 + i),
                        ComponentConfig{}
                            .with_size(ComponentSize{percent(1.f), h720(28.f)})
                            .with_align_items(AlignItems::Center)
                            .with_gap(h720(8.f))
                            .with_no_wrap());

      div(context, mk(row.ent(), 0),
          ComponentConfig{}
              .with_label(stat_names[(size_t)i])
              .with_size(ComponentSize{percent(0.2f), h720(24.f)})
              .with_background(Theme::Usage::None)
              .with_custom_text_color(ink_dim)
              .with_font_size(h720(15.f)));

      progress_bar(context, mk(row.ent(), 1), stat(i),
                   ComponentConfig{}
                       .with_size(ComponentSize{percent(0.66f), h720(18.f)})
                       .with_corner_radius(9.f)
                       .with_debug_name(fmt::format("mk_stat_{}", i)),
                   ProgressBarLabelStyle::None);

      div(context, mk(row.ent(), 2),
          ComponentConfig{}
              .with_label(fmt::format("{}", (int)std::lround(stat(i) * 10.f)))
              .with_size(ComponentSize{percent(0.08f), h720(24.f)})
              .with_background(Theme::Usage::None)
              .with_alignment(TextAlignment::Right)
              .with_custom_text_color(ink)
              .with_font_size(h720(15.f)));
    }

    div(context, mk(side.ent(), 5),
        ComponentConfig{}
            .with_label(fmt::format("{} + {}", sel.name,
                                    karts[(size_t)kart_idx].name))
            .with_size(ComponentSize{percent(1.f), expand()})
            .with_background(Theme::Usage::None)
            .with_alignment(TextAlignment::Center)
            .with_custom_text_color(ink_dim)
            .with_font_size(h720(17.f))
            .with_debug_name("mk_loadout"));

    auto vehicles = vstack(context, mk(left.ent(), 1),
                           ComponentConfig{}
                               .with_size(ComponentSize{percent(1.f), expand()})
                               .with_custom_background(panel)
                               .with_padding(Spacing::sm)
                               .with_gap(h720(6.f))
                               .with_corner_radius(12.f)
                               .with_no_wrap()
                               .with_debug_name("mk_vehicles"));

    div(context, mk(vehicles.ent(), 0),
        ComponentConfig{}
            .with_label("VEHICLE")
            .with_size(ComponentSize{percent(1.f), h720(24.f)})
            .with_background(Theme::Usage::None)
            .with_custom_text_color(ink_dim)
            .with_letter_spacing(2.f)
            .with_font_size(h720(15.f)));

    auto kart_row = hstack(context, mk(vehicles.ent(), 1),
                           ComponentConfig{}
                               .with_size(ComponentSize{percent(1.f), expand()})
                               .with_gap(h720(8.f))
                               .with_no_wrap()
                               .with_debug_name("mk_kart_row"));

    for (int i = 0; i < (int)karts.size(); i++) {
      const bool on = i == kart_idx;
      const bool cursor = on && nav_row == 2;
      auto card =
          button(context, mk(kart_row.ent(), i),
                 ComponentConfig{}
                     .with_size(ComponentSize{expand(), percent(1.f)})
                     .with_custom_background(cursor ? ink
                                             : on    ? gold
                                                     : panel_hi)
                     .with_border(cursor ? gold
                                  : on   ? afterhours::Color{255, 240, 180, 255}
                                         : line,
                                  cursor ? 5.f : on ? 2.f : 1.f)
                     .with_corner_radius(10.f)
                     .with_click_activation(ClickActivationMode::Release)
                     .with_debug_name(fmt::format("mk_kart_{}", i)));
      if (card) {
        kart_idx = i;
        nav_row = 2;
      }

      const afterhours::Color body_col =
          on ? afterhours::Color{60, 48, 20, 255} : drivers[(size_t)i].color;
      div(context, mk(card.ent(), 0),
          ComponentConfig{}
              .with_size(ComponentSize{percent(1.f), expand()})
              .with_background(Theme::Usage::None)
              .with_ignore_pointer_events()
              .with_on_draw_fg([body_col](RectangleType rr) {
                const float w = rr.width * 0.62f;
                const float h = rr.height * 0.34f;
                const float x = rr.x + (rr.width - w) * 0.5f;
                const float y = rr.y + rr.height * 0.42f;
                afterhours::draw_rectangle_rounded({x, y, w, h}, 0.45f, 8,
                                                   body_col);
                afterhours::draw_rectangle_rounded(
                    {x + w * 0.22f, y - h * 0.55f, w * 0.5f, h * 0.7f}, 0.4f, 8,
                    afterhours::Color{230, 238, 250, 200});
                const afterhours::Color tyre{24, 26, 34, 255};
                afterhours::draw_circle_v({x + w * 0.2f, y + h}, h * 0.38f,
                                          tyre);
                afterhours::draw_circle_v({x + w * 0.8f, y + h}, h * 0.38f,
                                          tyre);
              }));

      div(context, mk(card.ent(), 1),
          ComponentConfig{}
              .with_label(karts[(size_t)i].name)
              .with_size(ComponentSize{percent(1.f), h720(26.f)})
              .with_background(Theme::Usage::None)
              .with_alignment(TextAlignment::Center)
              .with_custom_text_color((on || cursor) ? deep : ink)
              .with_font("Fredoka", h720(18.f))
              .with_ignore_pointer_events());
    }

    nav_bar(context, root.ent(), 2, "mk_driver_back", Phase::Title, "CONTINUE",
            "mk_driver_next",
            "Arrows move the cursor   Enter confirms   Esc goes back",
            [this]() { phase = Phase::Cup; });
  }

  template <typename Fn>
  void nav_bar(UIContext<InputAction> &context, afterhours::Entity &root,
               int idx, const std::string &back_name, Phase back_to,
               const std::string &next_label, const std::string &next_name,
               const std::string &hint, Fn on_next) {
    auto bar = hstack(context, mk(root, idx),
                      ComponentConfig{}
                          .with_size(ComponentSize{percent(1.f), h720(54.f)})
                          .with_align_items(AlignItems::Center)
                          .with_gap(h720(10.f))
                          .with_no_wrap()
                          .with_debug_name("mk_nav"));

    if (button(context, mk(bar.ent(), 0),
               ComponentConfig{}
                   .with_label("BACK")
                   .with_size(ComponentSize{h720(120.f), h720(46.f)})
                   .with_custom_background(panel_hi)
                   .with_custom_text_color(ink)
                   .with_alignment(TextAlignment::Center)
                   .with_font_size(h720(19.f))
                   .with_corner_radius(10.f)
                   .with_click_activation(ClickActivationMode::Release)
                   .with_debug_name(back_name))) {
      phase = back_to;
    }

    div(context, mk(bar.ent(), 1),
        ComponentConfig{}
            .with_label(hint)
            .with_size(ComponentSize{expand(), h720(28.f)})
            .with_background(Theme::Usage::None)
            .with_alignment(TextAlignment::Center)
            .with_custom_text_color(ink_dim)
            .with_font_size(h720(15.f))
            .with_debug_name("mk_nav_hint"));

    if (button(context, mk(bar.ent(), 2),
               ComponentConfig{}
                   .with_label(next_label)
                   .with_size(ComponentSize{h720(240.f), h720(46.f)})
                   .with_custom_background(go_green)
                   .with_custom_text_color(deep)
                   .with_alignment(TextAlignment::Center)
                   .with_font("Fredoka", h720(24.f))
                   .with_corner_radius(10.f)
                   .with_click_activation(ClickActivationMode::Release)
                   .with_debug_name(next_name))) {
      on_next();
    }
  }

  void build_cup(afterhours::Entity &entity, UIContext<InputAction> &context) {
    auto root = page(context, entity, "mk_cup_page");
    header(context, root.ent(), 0, "SELECT CUP",
           fmt::format("{}  -  {}", drivers[(size_t)driver_idx].name,
                       karts[(size_t)kart_idx].name));

    auto body = hstack(context, mk(root.ent(), 1),
                       ComponentConfig{}
                           .with_size(ComponentSize{percent(1.f), expand()})
                           .with_gap(h720(12.f))
                           .with_no_wrap()
                           .with_debug_name("mk_cup_body"));

    auto cup_grid = grid(context, mk(body.ent(), 0),
                         GridConfig{}
                             .with_rows(2)
                             .with_cols(2)
                             .with_row_height(h720(250.f))
                             .with_gap(h720(10.f)),
                         ComponentConfig{}
                             .with_size(ComponentSize{expand(1.4f), percent(1.f)})
                             .with_custom_background(panel)
                             .with_padding(Spacing::sm)
                             .with_corner_radius(12.f)
                             .with_no_wrap()
                             .with_debug_name("mk_cup_grid"));

    for (int i = 0; i < (int)cups.size(); i++) {
      afterhours::OptEntity row_opt = grid_row(cup_grid, i / 2);
      if (!row_opt.valid())
        continue;
      afterhours::Entity &row = row_opt.asE();
      const Cup &c = cups[(size_t)i];
      const bool on = i == cup_idx;

      auto card =
          button(context, mk(row, i % 2),
                 ComponentConfig{}
                     .with_size(ComponentSize{grid_track(cup_grid, i % 2).x_axis,
                                              h720(246.f)})
                     .with_custom_background(on ? panel_hi : panel)
                     .with_border(on ? c.color : line, on ? 3.f : 1.f)
                     .with_corner_radius(12.f)
                     .with_padding(Spacing::xs)
                     .with_click_activation(ClickActivationMode::Release)
                     .with_debug_name(fmt::format("mk_cup_{}", i)));
      if (card)
        cup_idx = i;

      const afterhours::Color cc = c.color;
      auto head = hstack(context, mk(card.ent(), 0),
                         ComponentConfig{}
                             .with_size(ComponentSize{percent(1.f), h720(36.f)})
                             .with_align_items(AlignItems::Center)
                             .with_gap(h720(8.f))
                             .with_no_wrap()
                             .with_ignore_pointer_events());

      div(context, mk(head.ent(), 0),
          ComponentConfig{}
              .with_size(ComponentSize{h720(30.f), h720(30.f)})
              .with_background(Theme::Usage::None)
              .with_on_draw_fg([cc](RectangleType rr) {
                const afterhours::Vector2Type ctr{rr.x + rr.width * 0.5f,
                                                  rr.y + rr.height * 0.5f};
                const float rad = std::min(rr.width, rr.height) * 0.46f;
                afterhours::draw_poly(ctr, 6, rad, 0.f, cc);
              }));

      div(context, mk(head.ent(), 1),
          ComponentConfig{}
              .with_label(c.name)
              .with_size(ComponentSize{expand(), h720(30.f)})
              .with_background(Theme::Usage::None)
              .with_custom_text_color(on ? ink : ink_dim)
              .with_font("Fredoka", h720(21.f)));

      auto cols = hstack(context, mk(card.ent(), 1),
                         ComponentConfig{}
                             .with_size(ComponentSize{percent(1.f), expand()})
                             .with_gap(h720(6.f))
                             .with_no_wrap()
                             .with_ignore_pointer_events());

      auto list = vstack(context, mk(cols.ent(), 0),
                         ComponentConfig{}
                             .with_size(ComponentSize{percent(0.58f),
                                                      percent(1.f)})
                             .with_gap(h720(2.f))
                             .with_no_wrap()
                             .with_ignore_pointer_events());

      for (int t = 0; t < kCupRaces; t++) {
        div(context, mk(list.ent(), t),
            ComponentConfig{}
                .with_label(fmt::format("{}. {}", t + 1, c.tracks[(size_t)t]))
                .with_size(ComponentSize{percent(1.f), expand()})
                .with_background(Theme::Usage::None)
                .with_custom_text_color(on ? ink : ink_dim)
                .with_font_size(h720(17.f))
                .with_ignore_pointer_events());
      }

      div(context, mk(cols.ent(), 1),
          ComponentConfig{}
              .with_size(ComponentSize{percent(0.4f), percent(1.f)})
              .with_background(Theme::Usage::None)
              .with_ignore_pointer_events()
              .with_on_draw_fg([cc, i](RectangleType rr) {
                draw_track(TrackShape::for_track(i, 0), rr,
                           afterhours::Color{40, 50, 80, 255}, cc);
              }));
    }

    auto side = vstack(context, mk(body.ent(), 1),
                       ComponentConfig{}
                           .with_size(ComponentSize{expand(1.f), percent(1.f)})
                           .with_custom_background(panel)
                           .with_padding(Spacing::sm)
                           .with_gap(h720(8.f))
                           .with_corner_radius(12.f)
                           .with_no_wrap()
                           .with_debug_name("mk_cup_side"));

    div(context, mk(side.ent(), 0),
        ComponentConfig{}
            .with_label("RACE 1 OF 4")
            .with_size(ComponentSize{percent(1.f), h720(28.f)})
            .with_background(Theme::Usage::None)
            .with_custom_text_color(ink_dim)
            .with_font_size(h720(15.f)));

    div(context, mk(side.ent(), 1),
        ComponentConfig{}
            .with_label(cups[(size_t)cup_idx].tracks[0])
            .with_size(ComponentSize{percent(1.f), h720(40.f)})
            .with_background(Theme::Usage::None)
            .with_custom_text_color(gold)
            .with_font("Fredoka", h720(26.f))
            .with_debug_name("mk_cup_first_track"));

    div(context, mk(side.ent(), 2),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.f), expand()})
            .with_custom_background(deep)
            .with_corner_radius(10.f)
            .with_on_draw_fg([cup = cup_idx](RectangleType rr) {
              draw_track(TrackShape::for_track(cup, 0), rr,
                         afterhours::Color{70, 84, 128, 255},
                         afterhours::Color{120, 136, 186, 255});
            })
            .with_debug_name("mk_cup_map"));

    nav_bar(context, root.ent(), 2, "mk_cup_back", Phase::Driver,
            "START GRAND PRIX", "mk_cup_start",
            "Arrows pick a cup   Enter starts the Grand Prix   Esc goes back",
            [this]() { start_cup(); });
  }

  static void draw_track(const TrackShape &shape, RectangleType rr,
                         afterhours::Color edge, afterhours::Color road) {
    const int steps = 160;
    const float thick =
        std::max(4.f, std::min(rr.width, rr.height) * 0.038f);
    for (int pass = 0; pass < 2; pass++) {
      const float w = pass == 0 ? thick * 1.6f : thick;
      const afterhours::Color c = pass == 0 ? edge : road;
      afterhours::Vector2Type prev = shape.map(0.f, rr);
      for (int i = 1; i <= steps; i++) {
        const afterhours::Vector2Type p =
            shape.map((float)i / (float)steps, rr);
        afterhours::draw_line_ex(prev, p, w, c);
        prev = p;
      }
    }
    const afterhours::Vector2Type start = shape.map(0.f, rr);
    afterhours::draw_rectangle(
        {start.x - thick * 0.2f, start.y - thick, thick * 0.4f, thick * 2.f},
        afterhours::Color{235, 240, 250, 255});
  }

  void build_race(afterhours::Entity &entity, UIContext<InputAction> &context) {
    auto root = vstack(context, mk(entity),
                       ComponentConfig{}
                           .with_size(ComponentSize{percent(1.f), percent(1.f)})
                           .with_custom_background(bg)
                           .with_corner_radius(0.f)
                           .with_no_wrap()
                           .with_debug_name("mk_race_page"));

    auto content = vstack(context, mk(root.ent(), 0),
                          ComponentConfig{}
                              .with_size(ComponentSize{percent(1.f),
                                                       percent(1.f)})
                              .with_background(Theme::Usage::None)
                              .with_padding(Spacing::md)
                              .with_gap(h720(12.f))
                              .with_no_wrap()
                              .with_debug_name("mk_race_content"));

    const Racer &me = racers[0];
    const auto order = running_order();
    const int place = position_of(0);
    const int lap = std::min(kLaps, (int)me.prog + 1);

    auto top = hstack(context, mk(content.ent(), 0),
                      ComponentConfig{}
                          .with_size(ComponentSize{percent(1.f), h720(64.f)})
                          .with_align_items(AlignItems::Center)
                          .with_gap(h720(10.f))
                          .with_no_wrap()
                          .with_debug_name("mk_race_top"));

    div(context, mk(top.ent(), 0),
        ComponentConfig{}
            .with_label(ordinal(place))
            .with_size(ComponentSize{strict_w1280(110.f), h720(58.f)})
            .with_custom_background(place == 1 ? gold : panel_hi)
            .with_custom_text_color(place == 1 ? deep : ink)
            .with_alignment(TextAlignment::Center)
            .with_font("Fredoka", h720(34.f))
            .with_corner_radius(10.f)
            .with_debug_name("mk_place"));

    div(context, mk(top.ent(), 1),
        ComponentConfig{}
            .with_label(fmt::format("LAP {}/{}", lap, kLaps))
            .with_size(ComponentSize{strict_w1280(130.f), h720(58.f)})
            .with_custom_background(panel_hi)
            .with_custom_text_color(ink)
            .with_alignment(TextAlignment::Center)
            .with_font("Fredoka", h720(24.f))
            .with_corner_radius(10.f)
            .with_debug_name("mk_lap"));

    div(context, mk(top.ent(), 2),
        ComponentConfig{}
            .with_label(fmt::format("{}  -  {}", track_name(),
                                    cups[(size_t)cup_idx].name))
            .with_size(ComponentSize{expand(), h720(40.f)})
            .with_background(Theme::Usage::None)
            .with_alignment(TextAlignment::Center)
            .with_custom_text_color(ink_dim)
            .with_font_size(h720(18.f))
            .with_debug_name("mk_track_name"));

    div(context, mk(top.ent(), 3),
        ComponentConfig{}
            .with_label(fmt_time(race_time))
            .with_size(ComponentSize{strict_w1280(150.f), h720(58.f)})
            .with_custom_background(panel_hi)
            .with_custom_text_color(ink)
            .with_alignment(TextAlignment::Center)
            .with_font_size(h720(22.f))
            .with_debug_name("mk_clock"));

    if (button(context, mk(top.ent(), 4),
               ComponentConfig{}
                   .with_label("II")
                   .with_size(ComponentSize{strict_w1280(56.f), h720(46.f)})
                   .with_custom_background(panel_hi)
                   .with_custom_text_color(ink)
                   .with_alignment(TextAlignment::Center)
                   .with_corner_radius(10.f)
                   .with_click_activation(ClickActivationMode::Release)
                   .with_debug_name("mk_pause"))) {
      paused = !paused;
    }

    auto body = hstack(context, mk(content.ent(), 1),
                       ComponentConfig{}
                           .with_size(ComponentSize{percent(1.f), expand()})
                           .with_gap(h720(12.f))
                           .with_no_wrap()
                           .with_debug_name("mk_race_body"));

    auto board = vstack(context, mk(body.ent(), 0),
                        ComponentConfig{}
                            .with_size(ComponentSize{percent(0.26f),
                                                     percent(1.f)})
                            .with_custom_background(panel)
                            .with_padding(Spacing::xs)
                            .with_gap(h720(4.f))
                            .with_corner_radius(12.f)
                            .with_no_wrap()
                            .with_debug_name("mk_board"));

    for (int i = 0; i < kRacers; i++) {
      const int who = order[(size_t)i];
      const Racer &r = racers[(size_t)who];
      const Driver &d = drivers[(size_t)r.driver];
      const bool mine = who == 0;

      auto row = hstack(context, mk(board.ent(), i),
                        ComponentConfig{}
                            .with_size(ComponentSize{percent(1.f), h720(32.f)})
                            .with_custom_background(mine ? panel_hi : panel)
                            .with_align_items(AlignItems::Center)
                            .with_gap(h720(6.f))
                            .with_padding(Padding::all(h720(3.f)))
                            .with_corner_radius(6.f)
                            .with_no_wrap()
                            .with_debug_name(fmt::format("mk_row_{}", i)));

      div(context, mk(row.ent(), 0),
          ComponentConfig{}
              .with_label(fmt::format("{}", i + 1))
              .with_size(ComponentSize{strict_w1280(24.f), h720(24.f)})
              .with_background(Theme::Usage::None)
              .with_alignment(TextAlignment::Center)
              .with_custom_text_color(i == 0 ? gold : ink_dim)
              .with_font_size(h720(16.f)));

      const afterhours::Color dc = d.color;
      div(context, mk(row.ent(), 1),
          ComponentConfig{}
              .with_size(ComponentSize{strict_w1280(16.f), h720(16.f)})
              .with_background(Theme::Usage::None)
              .with_on_draw_fg([dc](RectangleType rr) {
                afterhours::draw_circle_v(
                    {rr.x + rr.width * 0.5f, rr.y + rr.height * 0.5f},
                    std::min(rr.width, rr.height) * 0.5f, dc);
              }));

      div(context, mk(row.ent(), 2),
          ComponentConfig{}
              .with_label(mine ? fmt::format("{} (you)", d.name) : d.name)
              .with_size(ComponentSize{expand(), h720(24.f)})
              .with_background(Theme::Usage::None)
              .with_custom_text_color(mine ? ink : ink_dim)
              .with_font_size(h720(16.f)));

      const Racer &leader = racers[(size_t)order[0]];
      std::string gap = "LEADER";
      if (r.finish_time >= 0.f)
        gap = fmt_time(r.finish_time);
      else if (i > 0)
        gap = fmt::format("+{:.1f}s",
                          (leader.prog - r.prog) / std::max(r.speed, 0.01f));

      div(context, mk(row.ent(), 3),
          ComponentConfig{}
              .with_label(gap)
              .with_size(ComponentSize{strict_w1280(86.f), h720(24.f)})
              .with_background(Theme::Usage::None)
              .with_alignment(TextAlignment::Right)
              .with_custom_text_color(ink_dim)
              .with_font_size(h720(15.f)));
    }

    div(context, mk(board.ent(), 8),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.f), expand()})
            .with_background(Theme::Usage::None));

    static constexpr std::array<const char *, 6> kInfoLabels{
        "ENGINE", "DRIVER", "KART", "ITEM", "ENTER", "SHIFT"};
    const std::array<std::string, 6> info{
        kEngineNames[(size_t)engine], drivers[(size_t)me.driver].name,
        karts[(size_t)kart_idx].name,  item_name(me.item),
        "drift",                       "use item"};

    for (int i = 0; i < 6; i++) {
      auto row = hstack(context, mk(board.ent(), 9 + i),
                        ComponentConfig{}
                            .with_size(ComponentSize{percent(1.f), h720(26.f)})
                            .with_align_items(AlignItems::Center)
                            .with_no_wrap());

      div(context, mk(row.ent(), 0),
          ComponentConfig{}
              .with_label(kInfoLabels[(size_t)i])
              .with_size(ComponentSize{percent(0.5f), h720(22.f)})
              .with_background(Theme::Usage::None)
              .with_custom_text_color(afterhours::Color{112, 126, 164, 255})
              .with_font_size(h720(14.f)));

      div(context, mk(row.ent(), 1),
          ComponentConfig{}
              .with_label(info[(size_t)i])
              .with_size(ComponentSize{percent(0.5f), h720(22.f)})
              .with_background(Theme::Usage::None)
              .with_alignment(TextAlignment::Right)
              .with_custom_text_color(ink_dim)
              .with_font_size(h720(15.f)));
    }

    auto mid = vstack(context, mk(body.ent(), 1),
                      ComponentConfig{}
                          .with_size(ComponentSize{percent(0.725f),
                                                   percent(1.f)})
                          .with_gap(h720(10.f))
                          .with_no_wrap()
                          .with_debug_name("mk_race_mid"));

    std::array<std::pair<float, afterhours::Color>, kRacers> dots{};
    for (int i = 0; i < kRacers; i++)
      dots[(size_t)i] = {racers[(size_t)i].prog,
                         drivers[(size_t)racers[(size_t)i].driver].color};

    auto map = vstack(context, mk(mid.ent(), 0),
                      ComponentConfig{}
                          .with_size(ComponentSize{percent(1.f), expand()})
                          .with_background(Theme::Usage::None)
                          .with_justify_content(JustifyContent::Center)
                          .with_align_items(AlignItems::Center)
                          .with_no_wrap()
                          .with_on_draw_bg([this, dots,
                                            shape = track_shape()](
                                               RectangleType rr) {
                            afterhours::draw_rectangle_rounded(rr, 0.06f, 8,
                                                               deep);
                            const RectangleType inner{
                                rr.x + rr.width * 0.05f,
                                rr.y + rr.height * 0.07f, rr.width * 0.9f,
                                rr.height * 0.86f};
                            draw_track(shape, inner,
                                       afterhours::Color{54, 66, 104, 255},
                                       afterhours::Color{96, 112, 158, 255});
                            for (size_t i = 0; i < dots.size(); i++) {
                              const auto p =
                                  shape.map(dots[i].first, inner);
                              afterhours::draw_circle_v(
                                  p, 9.f, afterhours::Color{0, 0, 0, 140});
                              afterhours::draw_circle_v(p, 7.f, dots[i].second);
                            }
                            const auto me_pt =
                                shape.map(dots[0].first, inner);
                            afterhours::draw_circle_lines(
                                (int)me_pt.x, (int)me_pt.y, 12.f,
                                afterhours::Color{255, 255, 255, 220});
                          })
                          .with_debug_name("mk_minimap"));

    div(context, mk(map.ent(), 0),
        ComponentConfig{}
            .with_label(fmt::format("{}", lap))
            .with_size(ComponentSize{percent(1.f), h720(110.f)})
            .with_background(Theme::Usage::None)
            .with_alignment(TextAlignment::Center)
            .with_custom_text_color(afterhours::Color{58, 72, 112, 255})
            .with_font("BlackOpsOne", h720(96.f))
            .with_ignore_pointer_events()
            .with_debug_name("mk_infield_lap"));

    div(context, mk(map.ent(), 1),
        ComponentConfig{}
            .with_label(fmt::format("LAP OF {}", kLaps))
            .with_size(ComponentSize{percent(1.f), h720(28.f)})
            .with_background(Theme::Usage::None)
            .with_alignment(TextAlignment::Center)
            .with_custom_text_color(afterhours::Color{52, 64, 100, 255})
            .with_letter_spacing(4.f)
            .with_font_size(h720(17.f))
            .with_ignore_pointer_events());

    auto ctl = hstack(context, mk(mid.ent(), 1),
                      ComponentConfig{}
                          .with_size(ComponentSize{percent(1.f), h720(116.f)})
                          .with_custom_background(panel)
                          .with_align_items(AlignItems::Center)
                          .with_justify_content(JustifyContent::SpaceBetween)
                          .with_padding(Spacing::sm)
                          .with_corner_radius(12.f)
                          .with_no_wrap()
                          .with_debug_name("mk_controls"));

    const float top_now = top_speed_for(me);
    const float ratio =
        top_now > 0.f ? std::clamp(me.speed / (top_now * 1.4f), 0.f, 1.f) : 0.f;

    circular_progress(context, mk(ctl.ent(), 0), ratio,
                      ComponentConfig{}
                          .with_size(ComponentSize{strict_w1280(98.f), h720(98.f)})
                          .with_label(fmt::format("{}", (int)(me.speed * 340.f)))
                          .with_custom_background(me.boost > 0.f
                                                      ? afterhours::Color{252,
                                                                          150,
                                                                          70,
                                                                          255}
                                                      : go_green)
                          .with_custom_text_color(ink)
                          .with_font("Fredoka", h720(22.f))
                          .with_border(afterhours::Color{52, 64, 104, 255}, 11.f)
                          .with_debug_name("mk_speedo"));

    auto drift_col =
        vstack(context, mk(ctl.ent(), 1),
               ComponentConfig{}
                   .with_size(ComponentSize{strict_w1280(280.f), h720(98.f)})
                   .with_gap(h720(8.f))
                   .with_no_wrap()
                   .with_debug_name("mk_drift_col"));

    const int tier = drift_charge >= 2.4f   ? 3
                     : drift_charge >= 1.5f ? 2
                     : drift_charge >= 0.7f ? 1
                                            : 0;
    const std::string drift_label =
        drifting ? fmt::format("RELEASE  (tier {})", tier) : "DRIFT";

    if (button(context, mk(drift_col.ent(), 0),
               ComponentConfig{}
                   .with_label(drift_label)
                   .with_size(ComponentSize{percent(1.f), h720(62.f)})
                   .with_custom_background(drifting ? gold : panel_hi)
                   .with_custom_text_color(drifting ? deep : ink)
                   .with_alignment(TextAlignment::Center)
                   .with_font("Fredoka", h720(22.f))
                   .with_corner_radius(10.f)
                   .with_click_activation(ClickActivationMode::Release)
                   .with_debug_name("mk_drift"))) {
      if (drifting) {
        drifting = false;
        release_drift();
      } else {
        drifting = true;
      }
    }

    progress_bar(context, mk(drift_col.ent(), 1),
                 std::clamp(drift_charge / 2.4f, 0.f, 1.f),
                 ComponentConfig{}
                     .with_size(ComponentSize{percent(1.f), h720(16.f)})
                     .with_corner_radius(8.f)
                     .with_debug_name("mk_charge"),
                 ProgressBarLabelStyle::None);

    if (button(context, mk(ctl.ent(), 2),
               ComponentConfig{}
                   .with_label(item_name(me.item))
                   .with_size(ComponentSize{strict_w1280(150.f), h720(74.f)})
                   .with_custom_background(item_color(me.item))
                   .with_custom_text_color(deep)
                   .with_alignment(TextAlignment::Center)
                   .with_font("Fredoka", h720(24.f))
                   .with_corner_radius(12.f)
                   .with_disabled(me.item == Item::None)
                   .with_click_activation(ClickActivationMode::Release)
                   .with_debug_name("mk_item"))) {
      use_item(0);
    }

    div(context, mk(ctl.ent(), 3),
        ComponentConfig{}
            .with_label(fmt::format("COINS {}", me.coins))
            .with_size(ComponentSize{strict_w1280(120.f), h720(48.f)})
            .with_custom_background(panel_hi)
            .with_custom_text_color(gold)
            .with_alignment(TextAlignment::Center)
            .with_font_size(h720(18.f))
            .with_corner_radius(10.f)
            .with_debug_name("mk_coins"));

    if (countdown > 0.f) {
      const int n = (int)std::ceil(countdown - 0.6f);
      div(context, mk(root.ent(), 2),
          ComponentConfig{}
              .with_label(n <= 0 ? "GO!" : fmt::format("{}", n))
              .with_size(ComponentSize{screen_pct(1.f), screen_pct(1.f)})
              .with_absolute_position(0.f, 0.f)
              .with_custom_background(afterhours::Color{6, 9, 20, 150})
              .with_custom_text_color(n <= 0 ? go_green : gold)
              .with_alignment(TextAlignment::Center)
              .with_font("BlackOpsOne", h720(120.f))
              .with_corner_radius(0.f)
              .with_overlay(2)
              .with_ignore_pointer_events()
              .with_debug_name("mk_countdown"));
    } else if (paused) {
      const float sw = (float)Settings::get().get_screen_width();
      const float sh = (float)Settings::get().get_screen_height();
      const float sheet_w = sw * (420.f / 1280.f);
      const float sheet_h = sh * (270.f / 720.f);
      auto sheet = vstack(context, mk(root.ent(), 3),
                          ComponentConfig{}
                              .with_size(ComponentSize{strict_w1280(420.f),
                                                       h720(270.f)})
                              .with_absolute_position((sw - sheet_w) * 0.5f,
                                                      (sh - sheet_h) * 0.5f)
                              .with_custom_background(panel)
                              .with_border(gold, 3.f)
                              .with_padding(Spacing::md)
                              .with_gap(h720(10.f))
                              .with_corner_radius(14.f)
                              .with_overlay(2)
                              .with_no_wrap()
                              .with_debug_name("mk_pause_sheet"));

      div(context, mk(sheet.ent(), 0),
          ComponentConfig{}
              .with_label("PAUSED")
              .with_size(ComponentSize{percent(1.f), h720(44.f)})
              .with_background(Theme::Usage::None)
              .with_alignment(TextAlignment::Center)
              .with_custom_text_color(gold)
              .with_font("Fredoka", h720(32.f)));

      if (button(context, mk(sheet.ent(), 1),
                 ComponentConfig{}
                     .with_label("RESUME")
                     .with_size(ComponentSize{percent(1.f), h720(46.f)})
                     .with_custom_background(go_green)
                     .with_custom_text_color(deep)
                     .with_alignment(TextAlignment::Center)
                     .with_font_size(h720(20.f))
                     .with_corner_radius(10.f)
                     .with_click_activation(ClickActivationMode::Release)
                     .with_debug_name("mk_resume"))) {
        paused = false;
      }

      if (button(context, mk(sheet.ent(), 2),
                 ComponentConfig{}
                     .with_label("RESTART RACE")
                     .with_size(ComponentSize{percent(1.f), h720(46.f)})
                     .with_custom_background(panel_hi)
                     .with_custom_text_color(ink)
                     .with_alignment(TextAlignment::Center)
                     .with_font_size(h720(20.f))
                     .with_corner_radius(10.f)
                     .with_click_activation(ClickActivationMode::Release)
                     .with_debug_name("mk_restart"))) {
        start_race();
      }

      if (button(context, mk(sheet.ent(), 3),
                 ComponentConfig{}
                     .with_label("QUIT TO TITLE")
                     .with_size(ComponentSize{percent(1.f), h720(46.f)})
                     .with_custom_background(panel_hi)
                     .with_custom_text_color(ink)
                     .with_alignment(TextAlignment::Center)
                     .with_font_size(h720(20.f))
                     .with_corner_radius(10.f)
                     .with_click_activation(ClickActivationMode::Release)
                     .with_debug_name("mk_quit"))) {
        phase = Phase::Title;
      }
    }
  }

  void build_results(afterhours::Entity &entity,
                     UIContext<InputAction> &context) {
    auto root = page(context, entity, "mk_results_page");
    header(context, root.ent(), 0, "RACE RESULTS",
           fmt::format("{}  -  race {} of {}", track_name(), race_in_cup + 1,
                       kCupRaces));

    auto body = hstack(context, mk(root.ent(), 1),
                       ComponentConfig{}
                           .with_size(ComponentSize{percent(1.f), expand()})
                           .with_gap(h720(12.f))
                           .with_no_wrap()
                           .with_debug_name("mk_results_body"));

    auto table = vstack(context, mk(body.ent(), 0),
                        ComponentConfig{}
                            .with_size(ComponentSize{expand(1.7f), percent(1.f)})
                            .with_custom_background(panel)
                            .with_padding(Spacing::sm)
                            .with_gap(h720(4.f))
                            .with_corner_radius(12.f)
                            .with_no_wrap()
                            .with_debug_name("mk_results_table"));

    for (int i = 0; i < kRacers; i++) {
      const int who = last_order[(size_t)i];
      const Racer &r = racers[(size_t)who];
      const Driver &d = drivers[(size_t)r.driver];
      const bool mine = who == 0;

      auto row = hstack(context, mk(table.ent(), i),
                        ComponentConfig{}
                            .with_size(ComponentSize{percent(1.f), h720(42.f)})
                            .with_custom_background(mine ? panel_hi : panel)
                            .with_align_items(AlignItems::Center)
                            .with_gap(h720(8.f))
                            .with_padding(Padding::all(h720(4.f)))
                            .with_corner_radius(8.f)
                            .with_no_wrap()
                            .with_debug_name(fmt::format("mk_res_{}", i)));

      div(context, mk(row.ent(), 0),
          ComponentConfig{}
              .with_label(ordinal(i + 1))
              .with_size(ComponentSize{h720(58.f), h720(30.f)})
              .with_background(Theme::Usage::None)
              .with_custom_text_color(i == 0 ? gold : ink_dim)
              .with_font("Fredoka", h720(20.f)));

      const afterhours::Color dc = d.color;
      div(context, mk(row.ent(), 1),
          ComponentConfig{}
              .with_size(ComponentSize{h720(20.f), h720(20.f)})
              .with_background(Theme::Usage::None)
              .with_on_draw_fg([dc](RectangleType rr) {
                afterhours::draw_circle_v(
                    {rr.x + rr.width * 0.5f, rr.y + rr.height * 0.5f},
                    std::min(rr.width, rr.height) * 0.5f, dc);
              }));

      div(context, mk(row.ent(), 2),
          ComponentConfig{}
              .with_label(mine ? fmt::format("{} (you)", d.name) : d.name)
              .with_size(ComponentSize{expand(), h720(30.f)})
              .with_background(Theme::Usage::None)
              .with_custom_text_color(ink)
              .with_font_size(h720(19.f)));

      div(context, mk(row.ent(), 3),
          ComponentConfig{}
              .with_label(fmt_time(r.finish_time))
              .with_size(ComponentSize{h720(120.f), h720(30.f)})
              .with_background(Theme::Usage::None)
              .with_alignment(TextAlignment::Right)
              .with_custom_text_color(ink_dim)
              .with_font_size(h720(17.f)));

      div(context, mk(row.ent(), 4),
          ComponentConfig{}
              .with_label(fmt::format("+{}", last_points[(size_t)who]))
              .with_size(ComponentSize{h720(56.f), h720(30.f)})
              .with_background(Theme::Usage::None)
              .with_alignment(TextAlignment::Right)
              .with_custom_text_color(gold)
              .with_font("Fredoka", h720(20.f)));
    }

    auto side = vstack(context, mk(body.ent(), 1),
                       ComponentConfig{}
                           .with_size(ComponentSize{expand(1.f), percent(1.f)})
                           .with_custom_background(panel)
                           .with_padding(Spacing::sm)
                           .with_gap(h720(4.f))
                           .with_corner_radius(12.f)
                           .with_no_wrap()
                           .with_debug_name("mk_standings"));

    div(context, mk(side.ent(), 0),
        ComponentConfig{}
            .with_label(fmt::format("{} STANDINGS", cups[(size_t)cup_idx].name))
            .with_size(ComponentSize{percent(1.f), h720(32.f)})
            .with_background(Theme::Usage::None)
            .with_custom_text_color(gold)
            .with_font("Fredoka", h720(20.f)));

    auto cup_order = standings_order();
    for (int i = 0; i < kRacers; i++) {
      const int who = cup_order[(size_t)i];
      const Driver &d = drivers[(size_t)racers[(size_t)who].driver];
      auto row = hstack(context, mk(side.ent(), 1 + i),
                        ComponentConfig{}
                            .with_size(ComponentSize{percent(1.f), h720(30.f)})
                            .with_custom_background(who == 0 ? panel_hi : panel)
                            .with_align_items(AlignItems::Center)
                            .with_gap(h720(6.f))
                            .with_padding(Padding::all(h720(2.f)))
                            .with_corner_radius(6.f)
                            .with_no_wrap());

      div(context, mk(row.ent(), 0),
          ComponentConfig{}
              .with_label(d.name)
              .with_size(ComponentSize{expand(), h720(24.f)})
              .with_background(Theme::Usage::None)
              .with_custom_text_color(who == 0 ? ink : ink_dim)
              .with_font_size(h720(17.f)));

      div(context, mk(row.ent(), 1),
          ComponentConfig{}
              .with_label(fmt::format("{}", points[(size_t)who]))
              .with_size(ComponentSize{h720(44.f), h720(24.f)})
              .with_background(Theme::Usage::None)
              .with_alignment(TextAlignment::Right)
              .with_custom_text_color(gold)
              .with_font("Fredoka", h720(19.f)));
    }

    const bool last = race_in_cup + 1 >= kCupRaces;
    nav_bar(context, root.ent(), 2, "mk_results_quit", Phase::Title,
            last ? "VIEW TROPHY" : "NEXT RACE", "mk_results_next",
            "Enter continues   Esc quits to the title",
            [this]() { advance_race(); });
  }

  std::array<int, kRacers> standings_order() const {
    std::array<int, kRacers> order{};
    for (int i = 0; i < kRacers; i++)
      order[(size_t)i] = i;
    std::stable_sort(order.begin(), order.end(), [&](int a, int b) {
      return points[(size_t)a] > points[(size_t)b];
    });
    return order;
  }

  void build_trophy(afterhours::Entity &entity,
                    UIContext<InputAction> &context) {
    auto root = page(context, entity, "mk_trophy_page");
    const auto order = standings_order();
    int my_place = kRacers;
    for (int i = 0; i < kRacers; i++)
      if (order[(size_t)i] == 0)
        my_place = i + 1;

    const afterhours::Color medal = my_place == 1   ? gold
                                    : my_place == 2 ? afterhours::Color{206,
                                                                        214,
                                                                        230,
                                                                        255}
                                    : my_place == 3 ? afterhours::Color{206,
                                                                        138,
                                                                        72, 255}
                                                    : ink_dim;

    header(context, root.ent(), 0, "TROPHY CEREMONY",
           cups[(size_t)cup_idx].name);

    div(context, mk(root.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.f), h720(150.f)})
            .with_background(Theme::Usage::None)
            .with_on_draw_fg([medal](RectangleType rr) {
              const float cx = rr.x + rr.width * 0.5f;
              const float top = rr.y + rr.height * 0.12f;
              const float s = rr.height * 0.76f;
              const float bowl_w = s * 0.62f;
              const float bowl_h = s * 0.46f;

              const float ring = bowl_h * 0.42f;
              for (int side = -1; side <= 1; side += 2) {
                const afterhours::Vector2Type hc{
                    cx + (float)side * (bowl_w * 0.52f), top + bowl_h * 0.34f};
                afterhours::draw_circle_v(hc, ring, medal);
                afterhours::draw_circle_v(hc, ring * 0.55f,
                                          afterhours::Color{14, 18, 34, 255});
              }

              afterhours::draw_triangle({cx - bowl_w * 0.5f, top},
                                        {cx - bowl_w * 0.22f, top + bowl_h},
                                        {cx + bowl_w * 0.5f, top}, medal);
              afterhours::draw_triangle({cx - bowl_w * 0.22f, top + bowl_h},
                                        {cx + bowl_w * 0.22f, top + bowl_h},
                                        {cx + bowl_w * 0.5f, top}, medal);
              afterhours::draw_rectangle(
                  {cx - bowl_w * 0.09f, top + bowl_h, bowl_w * 0.18f,
                   s * 0.22f},
                  medal);
              afterhours::draw_rectangle({cx - bowl_w * 0.34f,
                                          top + bowl_h + s * 0.22f,
                                          bowl_w * 0.68f, s * 0.09f},
                                         medal);
              afterhours::draw_rectangle({cx - bowl_w * 0.5f,
                                          top + bowl_h + s * 0.31f, bowl_w,
                                          s * 0.11f},
                                         medal);
            })
            .with_debug_name("mk_trophy_art"));

    div(context, mk(root.ent(), 2),
        ComponentConfig{}
            .with_label(fmt::format("{} PLACE  -  {} POINTS", ordinal(my_place),
                                    points[0]))
            .with_size(ComponentSize{percent(1.f), h720(52.f)})
            .with_background(Theme::Usage::None)
            .with_alignment(TextAlignment::Center)
            .with_custom_text_color(medal)
            .with_font("Fredoka", h720(38.f))
            .with_debug_name("mk_trophy_place"));

    auto list = vstack(context, mk(root.ent(), 3),
                       ComponentConfig{}
                           .with_size(ComponentSize{percent(0.6f), expand()})
                           .with_self_align(SelfAlign::Center)
                           .with_custom_background(panel)
                           .with_padding(Spacing::sm)
                           .with_gap(h720(4.f))
                           .with_corner_radius(12.f)
                           .with_no_wrap()
                           .with_debug_name("mk_trophy_list"));

    for (int i = 0; i < kRacers; i++) {
      const int who = order[(size_t)i];
      const Driver &d = drivers[(size_t)racers[(size_t)who].driver];
      auto row = hstack(context, mk(list.ent(), i),
                        ComponentConfig{}
                            .with_size(ComponentSize{percent(1.f), h720(32.f)})
                            .with_custom_background(who == 0 ? panel_hi : panel)
                            .with_align_items(AlignItems::Center)
                            .with_gap(h720(8.f))
                            .with_padding(Padding::all(h720(3.f)))
                            .with_corner_radius(6.f)
                            .with_no_wrap());

      div(context, mk(row.ent(), 0),
          ComponentConfig{}
              .with_label(ordinal(i + 1))
              .with_size(ComponentSize{h720(56.f), h720(24.f)})
              .with_background(Theme::Usage::None)
              .with_custom_text_color(i == 0 ? gold : ink_dim)
              .with_font_size(h720(17.f)));

      div(context, mk(row.ent(), 1),
          ComponentConfig{}
              .with_label(d.name)
              .with_size(ComponentSize{expand(), h720(24.f)})
              .with_background(Theme::Usage::None)
              .with_custom_text_color(ink)
              .with_font_size(h720(18.f)));

      div(context, mk(row.ent(), 2),
          ComponentConfig{}
              .with_label(fmt::format("{}", points[(size_t)who]))
              .with_size(ComponentSize{h720(48.f), h720(24.f)})
              .with_background(Theme::Usage::None)
              .with_alignment(TextAlignment::Right)
              .with_custom_text_color(gold)
              .with_font("Fredoka", h720(19.f)));
    }

    auto cta = hstack(context, mk(root.ent(), 4),
                      ComponentConfig{}
                          .with_size(ComponentSize{percent(1.f), h720(58.f)})
                          .with_justify_content(JustifyContent::Center)
                          .with_align_items(AlignItems::Center)
                          .with_no_wrap());

    if (button(context, mk(cta.ent(), 0),
               ComponentConfig{}
                   .with_label("BACK TO TITLE")
                   .with_size(ComponentSize{h720(260.f), h720(48.f)})
                   .with_custom_background(go_green)
                   .with_custom_text_color(deep)
                   .with_alignment(TextAlignment::Center)
                   .with_font("Fredoka", h720(24.f))
                   .with_corner_radius(10.f)
                   .with_click_activation(ClickActivationMode::Release)
                   .with_debug_name("mk_trophy_done"))) {
      phase = Phase::Title;
    }
  }
};

REGISTER_EXAMPLE_SCREEN(marlo_kart, "Game Mockups",
                        "Playable kart Grand Prix: title, select, race, podium",
                        MarloKartScreen)
