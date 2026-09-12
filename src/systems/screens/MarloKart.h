#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>
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

  const afterhours::Color ink{20, 44, 85, 255};
  const afterhours::Color white{255, 255, 255, 255};
  const afterhours::Color gold{255, 210, 41, 255};
  const afterhours::Color blue{22, 91, 191, 255};
  const afterhours::Color transparent{0, 0, 0, 0};
  std::array<raylib::Texture2D, 23> art{};
  bool art_loaded = false;
  float scale = 1.f;

  void apply_theme(UIContext<InputAction> &context) {
    scale = context.screen_height / 720.f;
    if (!art_loaded) {
      std::array<std::string, 23> names{"menu", "scenery", "header", "footer", "platform"};
      for (int i = 0; i < kRacers; ++i) {
        names[5 + i] = "portrait_" + std::to_string(i);
        names[13 + i] = "kart_" + std::to_string(i);
      }
      names[21] = "trophy";
      names[22] = "celebration";
      for (size_t i = 0; i < art.size(); ++i) {
        art[i] = raylib::LoadTexture(afterhours::files::get_resource_path(
            "images", "marlo_kart/" + names[i] + ".png").string().c_str());
        raylib::SetTextureFilter(art[i], raylib::TEXTURE_FILTER_BILINEAR);
      }
      art_loaded = true;
    }
    Theme theme;
    theme.font = ink;
    theme.darkfont = white;
    theme.background = blue;
    theme.surface = white;
    theme.primary = gold;
    theme.secondary = blue;
    theme.accent = gold;
    theme.corner_radius = 0;
    theme.roundness = 0;
    context.set_theme(theme);
    context.scaling_mode = ScalingMode::Proportional;
    UIStylingDefaults::get().set_default_font("FredokaMockBold", h720(25));
  }

  ComponentConfig box(float x, float y, float w, float h) const {
    return ComponentConfig{}.with_size({pixels(w * scale), pixels(h * scale)})
        .with_absolute_position(x * scale, y * scale)
        .with_background(Theme::Usage::None).with_corner_radius(0);
  }
  static void paint(raylib::Texture2D texture, RectangleType r, float rotation = 0) {
    const raylib::Vector2 origin{r.width / 2, r.height / 2};
    r.x += origin.x; r.y += origin.y;
    raylib::DrawTexturePro(texture, {0, 0, static_cast<float>(texture.width),
        static_cast<float>(texture.height)}, r, origin, rotation, raylib::WHITE);
  }
  void image(UIContext<InputAction> &c, afterhours::Entity &p, int id, int asset,
             float x, float y, float w, float h, float rotation = 0) {
    div(c, mk(p, id), box(x, y, w, h).with_ignore_pointer_events()
        .with_on_draw_bg([texture = art[asset], rotation](RectangleType r) { paint(texture, r, rotation); }));
  }
  ElementResult page(UIContext<InputAction> &c, afterhours::Entity &e,
                     const std::string &name, int background = 0) {
    auto root = div(c, mk(e), box(0, 0, 1280, 720).with_debug_name(name));
    image(c, root.ent(), 900, background, 0, 0, 1280, 720);
    return root;
  }
  ElementResult label(UIContext<InputAction> &c, afterhours::Entity &p, int id,
                     const std::string &text, float x, float y, float w, float h,
                     float size, afterhours::Color color,
                     const std::string &name = "", TextAlignment align = TextAlignment::Left,
                     const std::string &font = "ArchivoMock") {
    return div(c, mk(p, id), box(x, y, w, h).with_label(text)
        .with_font(font, h720(size * 1.25f))
        .with_custom_text_color(color).with_alignment(align)
        .with_ignore_pointer_events().with_debug_name(name));
  }
  ElementResult action(UIContext<InputAction> &c, afterhours::Entity &p, int id,
                       const std::string &text, float x, float y, float w, float h,
                       const std::string &name, afterhours::Color fill,
                       afterhours::Color color, float size = 22, bool disabled = false) {
    return button(c, mk(p, id), box(x, y, w, h).with_label(text)
        .with_font("ArchivoMock", h720(size * 1.25f)).with_custom_background(fill)
        .with_custom_text_color(color).with_alignment(TextAlignment::Center)
        .with_border(white, 2 * scale).with_corner_radius(0)
        .with_disabled(disabled).with_click_activation(ClickActivationMode::Release)
        .with_debug_name(name));
  }
  void header(UIContext<InputAction> &c, afterhours::Entity &p,
              const std::string &title, const std::string &right) {
    image(c, p, 901, 2, 0, 0, 1280, 88);
    label(c, p, 902, title, 40, 13, 760, 61, 37, ink, "mk_title", TextAlignment::Left, "ArchivoMockBold");
    label(c, p, 903, right, 827, 27, 350, 38, 18, ink, "mk_header_right", TextAlignment::Right);
    label(c, p, 904, "P1", 1195, 26, 45, 39, 21, ink, "", TextAlignment::Center);
  }
  void footer(UIContext<InputAction> &c, afterhours::Entity &p, const std::string &help) {
    image(c, p, 905, 3, 0, 654, 1280, 66);
    label(c, p, 906, help, 240, 668, 700, 38, 17, ink, "", TextAlignment::Center);
  }
  void plate(UIContext<InputAction> &c, afterhours::Entity &p, int id,
             float x, float y, float w, float h, afterhours::Color fill,
             afterhours::Color border, float radius = 12) {
    div(c, mk(p, id), box(x, y, w, h).with_custom_background(fill)
        .with_border(border, 3 * scale).with_corner_radius(radius * scale)
        .with_ignore_pointer_events());
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

  std::array<int, kRacers> standings_order() const {
    std::array<int, kRacers> order{};
    for (int i = 0; i < kRacers; i++)
      order[(size_t)i] = i;
    std::stable_sort(order.begin(), order.end(), [&](int a, int b) {
      return points[(size_t)a] > points[(size_t)b];
    });
    return order;
  }

  void build_title(afterhours::Entity &entity, UIContext<InputAction> &c) {
    auto root = page(c, entity, "mk_title_page", 1);
    auto logo = div(c, mk(root.ent(), 0), box(67, 77, 530, 259)
        .with_styled_label({{"MARLO ", white}, {"KART", gold}})
        .with_font("FredokaMockBold", h720(132))
        .with_text_overflow(TextOverflow::Wrap).with_alignment(TextAlignment::Left)
        .with_text_stroke(ink, 3 * scale).with_text_shadow(ink, 5 * scale, 7 * scale)
        .with_ignore_pointer_events().with_debug_name("mk_logo"));
    logo.ent().addComponentIfMissing<HasUIModifiers>().rotation = -5.f;
    label(c, root.ent(), 1, "AFTERHOURS GRAND PRIX", 79, 326, 490, 49, 15, white);
    for (int i = 0; i < 3; ++i)
      if (action(c, root.ent(), 10 + i, kEngineNames[i], 72 + i * 124, 408, 114, 59,
                 "mk_engine_" + std::to_string(i), engine == i ? gold : afterhours::Color{28, 92, 117, 255},
                 engine == i ? ink : white, 22)) engine = i;
    if (action(c, root.ent(), 20, "START YOUR ENGINES", 72, 485, 366, 83,
               "mk_start", gold, ink, 36)) phase = Phase::Driver;
    label(c, root.ent(), 21, "Four tracks. Eight racers. One trophy.", 73, 576, 366, 32, 14, white, "", TextAlignment::Center);
    label(c, root.ent(), 22, "Arrows: engine class     Enter: start", 73, 632, 500, 32, 12, white);
    image(c, root.ent(), 23, 13 + driver_idx, 613, 208, 645, 450, -8);
  }

  void build_driver(afterhours::Entity &entity, UIContext<InputAction> &c) {
    auto root = page(c, entity, "mk_driver_page");
    header(c, root.ent(), "SELECT DRIVER", fmt::format("{}  -  Grand Prix", kEngineNames[engine]));
    for (int i = 0; i < kRacers; ++i) {
      const float x = 40 + (i % 4) * 148, y = 116 + (i / 4) * 164;
      const bool selected = driver_idx == i;
      const bool cursor = selected && nav_row < 2;
      if (button(c, mk(root.ent(), 10 + i), box(x, y, 139, 155)
          .with_custom_background(cursor ? white : afterhours::Color{155, 211, 237, 255})
          .with_border(selected ? gold : afterhours::Color{153, 211, 255, 255}, (selected ? 5 : 2) * scale)
          .with_corner_radius(4 * scale).with_click_activation(ClickActivationMode::Release)
          .with_debug_name("mk_driver_" + std::to_string(i)))) {
        driver_idx = i; nav_row = i / 4;
      }
      image(c, root.ent(), 30 + i, 5 + i, x + 12, y + 3, 115, 115);
      plate(c, root.ent(), 250 + i, x + 4, y + 125, 131, 25, {217, 242, 250, 255}, transparent, 0);
      label(c, root.ent(), 50 + i, drivers[i].name, x + 6, y + 123, 127, 29,
            16, ink, "", TextAlignment::Center);
    }
    label(c, root.ent(), 70, "CHOOSE YOUR VEHICLE", 40, 449, 590, 33, 16, white);
    for (int i = 0; i < 4; ++i) {
      const float x = 40 + i * 148;
      const bool selected = kart_idx == i;
      const bool cursor = selected && nav_row == 2;
      if (button(c, mk(root.ent(), 80 + i), box(x, 488, 139, 110)
          .with_custom_background(cursor ? white : selected ? gold : afterhours::Color{228, 244, 250, 255})
          .with_border(selected ? gold : afterhours::Color{153, 211, 255, 255}, (selected ? 5 : 2) * scale)
          .with_corner_radius(3 * scale).with_click_activation(ClickActivationMode::Release)
          .with_debug_name("mk_kart_" + std::to_string(i)))) { kart_idx = i; nav_row = 2; }
      image(c, root.ent(), 90 + i, 13 + i, x + 10, 490, 120, 75);
      label(c, root.ent(), 100 + i, karts[i].name, x + 4, 563, 131, 32,
            15, ink, "", TextAlignment::Center);
    }
    image(c, root.ent(), 120, 4, 648, 366, 592, 112);
    image(c, root.ent(), 121, 13 + driver_idx, 729, 112, 430, 300, -8);
    label(c, root.ent(), 122, fmt::format("{} + {}", drivers[driver_idx].name, karts[kart_idx].name),
          656, 424, 581, 53, 35, white, "mk_loadout", TextAlignment::Center, "FredokaMockBold");
    constexpr std::array<const char *, 4> stats{"SPEED", "ACCELERATION", "HANDLING", "WEIGHT"};
    for (int i = 0; i < 4; ++i) {
      label(c, root.ent(), 130 + i, stats[i], 680, 487 + i * 34, 173, 29, 17, white);
      for (int j = 0; j < 10; ++j)
        div(c, mk(root.ent(), 150 + i * 10 + j), box(864 + j * 31, 494 + i * 34, 26, 16)
            .with_custom_background(stat(i) * 10 > j ? gold : afterhours::Color{21, 76, 159, 255})
            .with_corner_radius(3 * scale).with_ignore_pointer_events());
    }
    footer(c, root.ent(), "Arrows: choose driver / ride     Enter: confirm");
    if (action(c, root.ent(), 200, "BACK", 40, 664, 152, 44, "mk_driver_back", white, ink, 20)) phase = Phase::Title;
    if (action(c, root.ent(), 201, "LET'S RACE!", 1000, 663, 239, 46, "mk_driver_next", gold, ink, 23)) phase = Phase::Cup;
  }

  void build_cup(afterhours::Entity &entity, UIContext<InputAction> &c) {
    auto root = page(c, entity, "mk_cup_page");
    header(c, root.ent(), "SELECT CUP", fmt::format("{}  -  {}", drivers[driver_idx].name, karts[kart_idx].name));
    for (int i = 0; i < 4; ++i) {
      const auto &cup = cups[i];
      const float x = 40 + (i % 2) * 309, y = 115 + (i / 2) * 248;
      const bool selected = cup_idx == i;
      if (button(c, mk(root.ent(), 10 + i), box(x, y, 289, 229)
          .with_custom_background(selected ? afterhours::Color{255, 238, 166, 255} : afterhours::Color{192, 226, 246, 255})
          .with_corner_radius(15 * scale).with_border(selected ? gold : afterhours::Color{136, 205, 255, 255},
              (selected ? 5 : 2) * scale).with_click_activation(ClickActivationMode::Release)
          .with_debug_name("mk_cup_" + std::to_string(i)))) cup_idx = i;
      image(c, root.ent(), 20 + i, 21, x + 18, y + 7, 66, 66);
      label(c, root.ent(), 30 + i, cup.name, x + 92, y + 24, 185, 39, 25, ink);
      for (int j = 0; j < kCupRaces; ++j)
        label(c, root.ent(), 40 + i * 4 + j, cup.tracks[j], x + 27, y + 80 + j * 33, 244, 30,
              19, ink);
    }
    const auto &cup = cups[cup_idx];
    label(c, root.ent(), 70, cup.name, 700, 120, 530, 65, 43, white, "mk_cup_preview_title", TextAlignment::Center, "FredokaMockBold");
    label(c, root.ent(), 71, "4 tracks  /  3 laps each", 706, 188, 520, 36, 22, white, "", TextAlignment::Center);
    auto map = div(c, mk(root.ent(), 72), box(711, 251, 512, 242)
        .with_custom_background({116, 185, 82, 255}).with_border(white, 5 * scale)
        .with_corner_radius(16 * scale).with_ignore_pointer_events()
        .with_on_draw_fg([shape = TrackShape::for_track(cup_idx, 0)](RectangleType r) {
          RectangleType path{r.x + r.width * .12f, r.y + r.height * .16f, r.width * .76f, r.height * .68f};
          draw_track(shape, path, {250, 250, 238, 255}, {78, 83, 104, 255});
        }).with_debug_name("mk_cup_map"));
    label(c, root.ent(), 73, cup.tracks[0], 707, 510, 520, 44, 27, white, "mk_preview_track", TextAlignment::Center);
    label(c, root.ent(), 74, "Every race counts toward the cup!", 707, 556, 520, 37, 20, white, "", TextAlignment::Center);
    footer(c, root.ent(), "Arrows: choose cup     Enter: start Grand Prix");
    if (action(c, root.ent(), 200, "BACK", 40, 664, 152, 44, "mk_cup_back", white, ink, 20)) phase = Phase::Driver;
    if (action(c, root.ent(), 201, "START GRAND PRIX", 969, 663, 270, 46, "mk_cup_start", gold, ink, 21)) start_cup();
  }

  void build_race(afterhours::Entity &entity, UIContext<InputAction> &c) {
    auto root = page(c, entity, "mk_race_page", 1);
    const Racer &me = racers[0];
    div(c, mk(root.ent(), 1), box(0, 0, 1280, 654).with_ignore_pointer_events()
        .with_on_draw_fg([progress = me.prog, curve = track_shape().is_corner(me.prog)](RectangleType r) {
          const float s = r.width / 1280.f;
          for (float y = 299; y < 654; y += 2) {
            const float depth = (y - 299) / 355.f;
            const float bend = curve ? 42.f * std::sin(depth * 3.14159265f) : 0.f;
            const float center = 836 - depth * 200 + bend;
            const float half = 18 + depth * depth * 320;
            const float edge = 3 + depth * 17;
            const bool stripe = static_cast<int>(depth * 18 - progress * 80) % 2 == 0;
            auto line = [&](float x, float width, afterhours::Color color) {
              afterhours::draw_rectangle({r.x + x * s, r.y + y * s, width * s, 2.1f * s}, color);
            };
            line(center - half - edge * 2, half * 2 + edge * 4, {212, 207, 166, 255});
            line(center - half - edge, half * 2 + edge * 2,
                 stripe ? afterhours::Color{241, 241, 232, 255} : afterhours::Color{207, 72, 65, 255});
            line(center - half, half * 2, {99, 116, 124, 255});
            if (stripe) line(center - (1 + depth * 4), 2 + depth * 8, {246, 244, 222, 255});
          }
        }).with_debug_name("mk_road"));
    const auto order = running_order();
    const int place = position_of(0);
    const int lap = std::min(kLaps, static_cast<int>(me.prog) + 1);
    // The scenic camera is a presentation layer; progress and map positions
    // continue to come from the same fixed-step track simulation.
    const float turn = track_shape().is_corner(me.prog) ? -9.f : -3.f;
    for (int i = 7; i >= 1; --i) {
      const float gap = racers[i].prog - me.prog;
      if (gap <= 0 || gap >= .55f) continue;
      const float distance = std::clamp(gap / .55f, 0.f, 1.f);
      const float bottom = 345 + (1 - distance) * 230;
      const float depth = (bottom - 299) / 355;
      const float w = 55 + depth * 190;
      const float bend = track_shape().is_corner(me.prog) ? 42.f * std::sin(depth * 3.14159265f) : 0.f;
      const float lane = static_cast<float>(i % 3 - 1) * .48f;
      const float x = 836 - depth * 200 + bend + lane * (18 + depth * depth * 320) - w / 2;
      image(c, root.ent(), 20 + i, 13 + racers[i].driver, x, bottom - w * .6977f, w, w * .6977f, turn);
    }
    image(c, root.ent(), 30, 13 + driver_idx, 390, 306, 490, 342, turn);
    if (me.boost > 0)
      label(c, root.ent(), 31, "MINI TURBO!", 463, 278, 360, 51, 29, gold, "mk_boost", TextAlignment::Center);
    if (action(c, root.ent(), 40, me.item == Item::None ? "?" : item_name(me.item),
               35, 32, 104, 98, "mk_item", me.item == Item::None ? ink : item_color(me.item),
               white, me.item == Item::None ? 54 : 21, paused || me.item == Item::None)) use_item(0);
    label(c, root.ent(), 41, "SHIFT: ITEM", 34, 133, 170, 34, 16, white);
    label(c, root.ent(), 42, fmt::format("LAP {}/{}", lap, kLaps), 995, 26, 250, 54, 33, white, "mk_lap", TextAlignment::Right);
    label(c, root.ent(), 43, fmt_time(race_time), 990, 79, 250, 39, 26, gold, "mk_clock", TextAlignment::Right);
    for (int i = 0; i < kRacers; ++i) {
      const int who = order[i];
      const float y = 177 + i * 33;
      plate(c, root.ent(), 60 + i, 1057, y, 185, 31,
            who == 0 ? gold : afterhours::Color{16, 45, 79, 218}, transparent, 5);
      label(c, root.ent(), 70 + i, fmt::format("{}  {}", i + 1, drivers[racers[who].driver].name),
            1070, y, 160, 30, 18, who == 0 ? ink : white);
    }
    div(c, mk(root.ent(), 90), box(1004, 469, 236, 153)
        .with_custom_background({255, 255, 255, 230}).with_corner_radius(13 * scale)
        .with_ignore_pointer_events().with_debug_name("mk_minimap")
        .with_on_draw_fg([this, shape = track_shape()](RectangleType r) {
          RectangleType route{r.x + r.width * .13f, r.y + r.height * .13f, r.width * .74f, r.height * .74f};
          draw_track(shape, route, {32, 57, 92, 255}, {255, 255, 255, 255});
          for (int i = kRacers - 1; i >= 0; --i) {
            auto pos = shape.map(racers[i].prog, route);
            const float radius = (i == 0 ? 6.f : 4.f) * r.height / 153.f;
            afterhours::draw_circle(static_cast<int>(pos.x), static_cast<int>(pos.y), radius + 1.5f * r.height / 153.f, {255, 255, 255, 255});
            afterhours::draw_circle(static_cast<int>(pos.x), static_cast<int>(pos.y), radius, drivers[racers[i].driver].color);
          }
        }));
    label(c, root.ent(), 100, ordinal(place), 41, 493, 185, 113, 82, gold, "mk_place", TextAlignment::Left, "FredokaMockBold");
    label(c, root.ent(), 101, fmt::format("COINS {}", me.coins), 48, 604, 183, 37, 24, gold, "mk_coins");
    const int tier = drift_charge >= 2.4f ? 3 : drift_charge >= 1.5f ? 2 : drift_charge >= .7f ? 1 : 0;
    if (action(c, root.ent(), 110, drifting ? fmt::format("RELEASE  {}", tier) : "DRIFT",
               257, 579, 162, 51, "mk_drift", drifting ? gold : white, ink, 21, paused)) toggle_drift();
    if (action(c, root.ent(), 111, "PAUSE", 435, 579, 138, 51, "mk_pause", white, ink, 20, paused)) paused = true;
    label(c, root.ent(), 112, fmt::format("{} km/h", static_cast<int>(me.speed * 340.f)),
          600, 586, 192, 37, 25, white, "mk_speedo");
    div(c, mk(root.ent(), 113), box(258, 641, 315, 7).with_custom_background(ink)
        .with_ignore_pointer_events().with_debug_name("mk_charge")
        .with_on_draw_fg([charge = std::clamp(drift_charge / 2.4f, 0.f, 1.f)](RectangleType r) {
          r.width *= charge; afterhours::draw_rectangle(r, {255, 210, 41, 255});
        }));
    footer(c, root.ent(), fmt::format("{}  -  {}", track_name(), cups[cup_idx].name));
    label(c, root.ent(), 114, "ENTER: DRIFT", 32, 671, 208, 30, 16, ink);
    label(c, root.ent(), 115, fmt::format("RACE {} / 4", race_in_cup + 1), 1060, 671, 183, 30, 18, ink, "", TextAlignment::Right);
    if (paused) {
      div(c, mk(root.ent(), 300), box(0, 0, 1280, 720).with_custom_background({7, 31, 67, 168}).with_overlay(2));
      auto sheet = div(c, mk(root.ent(), 301), box(430, 195, 420, 324)
          .with_custom_background(white).with_border(gold, 5 * scale)
          .with_corner_radius(17 * scale).with_overlay(3).with_debug_name("mk_pause_sheet"));
      label(c, sheet.ent(), 0, "PAUSED", 32, 20, 356, 63, 41, ink, "", TextAlignment::Center);
      if (action(c, sheet.ent(), 1, "RESUME", 38, 103, 344, 48, "mk_resume", gold, ink)) paused = false;
      if (action(c, sheet.ent(), 2, "RESTART RACE", 38, 166, 344, 48, "mk_restart", blue, white)) start_race();
      if (action(c, sheet.ent(), 3, "QUIT TO TITLE", 38, 229, 344, 48, "mk_quit", blue, white)) phase = Phase::Title;
    } else if (countdown > 0) {
      const int n = static_cast<int>(std::ceil(countdown - .6f));
      div(c, mk(root.ent(), 302), box(499, 174, 282, 165)
          .with_label(n <= 0 ? "GO!" : std::to_string(n)).with_font("FredokaMockBold", h720(120))
          .with_alignment(TextAlignment::Center).with_custom_text_color(n <= 0 ? white : gold)
          .with_text_stroke(ink, 4 * scale).with_text_shadow(ink, 3 * scale, 5 * scale)
          .with_overlay(2).with_ignore_pointer_events().with_debug_name("mk_countdown"));
    }
  }

  void build_results(afterhours::Entity &entity, UIContext<InputAction> &c) {
    auto root = page(c, entity, "mk_results_page");
    header(c, root.ent(), "RACE RESULTS", fmt::format("{}  /  Race {} of 4", track_name(), race_in_cup + 1));
    plate(c, root.ent(), 0, 41, 112, 746, 481, white, white, 9);
    label(c, root.ent(), 1, "POS", 62, 118, 70, 35, 16, blue);
    label(c, root.ent(), 2, "DRIVER", 180, 118, 250, 35, 16, blue);
    label(c, root.ent(), 3, "FINISH TIME", 483, 118, 171, 35, 16, blue);
    label(c, root.ent(), 4, "POINTS", 672, 118, 93, 35, 16, blue);
    for (int i = 0; i < kRacers; ++i) {
      const int who = last_order[i];
      const float y = 153 + i * 54;
      plate(c, root.ent(), 10 + i, 45, y, 738, 53,
            who == 0 ? gold : i % 2 ? afterhours::Color{229, 241, 255, 255} : white, transparent, 0);
      image(c, root.ent(), 20 + i, 5 + racers[who].driver, 133, y + 3, 46, 46);
      label(c, root.ent(), 30 + i, std::to_string(i + 1), 64, y + 8, 55, 38, 26, ink);
      label(c, root.ent(), 40 + i, fmt::format("{}{}", drivers[racers[who].driver].name, who == 0 ? "  YOU" : ""),
            187, y + 9, 285, 36, 23, ink);
      label(c, root.ent(), 50 + i, fmt_time(racers[who].finish_time), 483, y + 10, 180, 35, 23, ink);
      label(c, root.ent(), 60 + i, fmt::format("+{}", last_points[who]), 683, y + 10, 75, 35, 23, ink);
    }
    plate(c, root.ent(), 80, 815, 112, 423, 481, {16, 73, 159, 255}, {142, 205, 255, 255});
    label(c, root.ent(), 81, fmt::format("{} STANDINGS", cups[cup_idx].name), 834, 130, 385, 49, 24, white, "mk_standings_heading", TextAlignment::Center);
    const auto order = standings_order();
    for (int i = 0; i < kRacers; ++i) {
      const int who = order[i];
      const float y = 193 + i * 45;
      label(c, root.ent(), 90 + i, fmt::format("{}. {}", i + 1, drivers[racers[who].driver].name),
            846, y, 260, 38, 23, who == 0 ? gold : white);
      label(c, root.ent(), 100 + i, std::to_string(points[who]), 1122, y, 88, 38,
            25, who == 0 ? gold : white, "", TextAlignment::Right);
    }
    label(c, root.ent(), 120, "15  /  12  /  10  /  8  /  6  /  4  /  2  /  1", 61, 604, 720, 34, 17, white);
    footer(c, root.ent(), "Four races. Every point matters.");
    if (action(c, root.ent(), 200, "QUIT", 40, 664, 152, 44, "mk_results_quit", white, ink, 20)) phase = Phase::Title;
    if (action(c, root.ent(), 201, race_in_cup + 1 >= kCupRaces ? "VIEW TROPHY" : "NEXT RACE",
               1000, 663, 239, 46, "mk_results_next", gold, ink, 23)) advance_race();
  }

  void build_trophy(afterhours::Entity &entity, UIContext<InputAction> &c) {
    auto root = page(c, entity, "mk_trophy_page", 22);
    const auto order = standings_order();
    int my_place = 1;
    for (int i = 0; i < kRacers; ++i) if (order[i] == 0) my_place = i + 1;
    header(c, root.ent(), "TROPHY CEREMONY", cups[cup_idx].name);
    image(c, root.ent(), 0, 21, 530, 102, 220, 220);
    label(c, root.ent(), 1, my_place == 1 ? "CONGRATULATIONS!" : "CUP COMPLETE!", 232, 327, 816, 73,
          49, gold, "", TextAlignment::Center, "FredokaMockBold");
    label(c, root.ent(), 2, fmt::format("{}  /  {} place  /  {} points", cups[cup_idx].name, ordinal(my_place), points[0]),
          267, 403, 746, 41, 23, white, "mk_trophy_summary", TextAlignment::Center);
    constexpr std::array<int, 3> podium{1, 0, 2};
    for (int i = 0; i < 3; ++i) {
      const int rank = podium[i], who = order[rank];
      const float x = 411 + i * 158, y = rank == 0 ? 465 : 493;
      plate(c, root.ent(), 10 + i, x, y, 142, 136,
            rank == 0 ? gold : afterhours::Color{235, 241, 255, 255}, white, 8);
      image(c, root.ent(), 20 + i, 5 + racers[who].driver, x + 34, y - 31, 75, 75);
      label(c, root.ent(), 30 + i, ordinal(rank + 1), x, y + 47, 142, 38, 25, ink, "", TextAlignment::Center);
      label(c, root.ent(), 40 + i, drivers[racers[who].driver].name, x, y + 88, 142, 34, 21, ink, "", TextAlignment::Center);
    }
    for (int i = 3; i < kRacers; ++i) {
      const int who = order[i];
      label(c, root.ent(), 60 + i, fmt::format("{}. {}  {} pts", i + 1, drivers[racers[who].driver].name, points[who]),
            52, 451 + (i - 3) * 33, 297, 32, 18, who == 0 ? gold : white);
    }
    footer(c, root.ent(), "Thanks for racing!");
    if (action(c, root.ent(), 100, "BACK TO TITLE", 975, 663, 264, 46, "mk_trophy_done", gold, ink, 23)) phase = Phase::Title;
  }
};

REGISTER_EXAMPLE_SCREEN(marlo_kart, "Game Mockups",
                       "Playable kart Grand Prix: title, select, race, podium",
                       MarloKartScreen)
