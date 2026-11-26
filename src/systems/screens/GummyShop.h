#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/effects.h>
#include <afterhours/src/plugins/files.h>
#include <afterhours/src/plugins/ui_motion.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <optional>
#include <string>

namespace um = afterhours::ui_motion;

// Gummy candy product card, recreated 1:1 from the reference clip: mint
// gingham backdrop, buttercream polka-dot product panel with a pink studio
// photo box (three translucent gummy bears), and live jelly candy controls
// floating on the gingham. Every glossy surface is shaded by
// resources/shaders/gummy_ui.fs (SDF rims, subsurface caustics, specular
// pills, frosted glass).
//
// Motion follows the Motion Reference Library entries:
//   toggle-morph (250-333ms knob, leading edge first, track fills with it),
//   tab-underline-slide (333-417ms, front edge leads, labels swap mid-move),
//   button-press (dip to ~0.94 then spring back with a small overshoot),
//   chip-select (fill crossfade 167-208ms), parameter-scrub (value follows
//   the drag every frame), count-up / progress-fill (house-curve fill),
//   reaction-pop (heart pops 0.82 -> 1.0 on a bouncy spring).
struct GummyShop : ScreenSystem<afterhours::ui::UIContext<InputAction>> {
  enum struct Key : size_t {
    ProgFill,
    HeartFill,
    SliderBlob,
    SegFront,
    SegBack,
    TogFront,
    TogBack,
  };

  std::optional<afterhours::effects::Effect> fx;
  raylib::Texture2D bear_orange{};
  raylib::Texture2D bear_red{};
  raylib::Texture2D bear_green{};
  bool assets_loaded = false;

  float elapsed = 0.f;
  int flavor = 0;  // first radio bubble filled red at rest
  bool toggle_on = true;
  float volume = 1.f;
  bool slider_dragging = false;
  int size_index = 0;  // Small selected at rest
  int qty = 1;
  std::array<bool, 3> extras{false, false, false};
  bool gift_check = false;
  bool liked = false;
  int likes = 12;
  int rating = 0;  // stars start empty glass

  void for_each_with(afterhours::Entity &entity,
                     afterhours::ui::UIContext<InputAction> &ctx,
                     float dt) override {
    using namespace afterhours::ui;
    using namespace afterhours::ui::imm;
    elapsed += dt;

    if (!fx) fx.emplace(afterhours::effects::Effect::load("gummy_ui"));
    if (!assets_loaded) {
      assets_loaded = true;
      bear_orange = raylib::LoadTexture(afterhours::files::get_resource_path(
          "images", "gummy_shop/bear_orange.png").c_str());
      bear_red = raylib::LoadTexture(afterhours::files::get_resource_path(
          "images", "gummy_shop/bear_red.png").c_str());
      bear_green = raylib::LoadTexture(afterhours::files::get_resource_path(
          "images", "gummy_shop/bear_green.png").c_str());
    }

    // Sampled straight from the reference frames.
    const afterhours::Color ink{45, 52, 72, 255};
    const afterhours::Color muted{118, 126, 150, 255};
    const afterhours::Color white{255, 255, 255, 255};
    const afterhours::Color green_core{62, 178, 32, 255};
    const afterhours::Color green_glow{158, 232, 92, 255};
    const afterhours::Color blue_core{23, 86, 227, 255};
    const afterhours::Color blue_glow{110, 168, 250, 255};
    const afterhours::Color yellow_core{248, 185, 40, 255};
    const afterhours::Color yellow_glow{255, 224, 120, 255};
    const afterhours::Color orange_core{232, 106, 42, 255};
    const afterhours::Color orange_glow{255, 168, 96, 255};
    const afterhours::Color red_core{214, 24, 60, 255};
    const afterhours::Color red_glow{255, 120, 140, 255};
    const afterhours::Color purple_core{79, 26, 165, 255};
    const afterhours::Color purple_glow{150, 100, 230, 255};
    const afterhours::Color grey_core{168, 172, 186, 255};
    const afterhours::Color grey_glow{205, 208, 218, 255};
    const afterhours::Color gold_core{249, 189, 34, 255};
    const afterhours::Color gold_glow{255, 226, 120, 255};
    const afterhours::Color glass_col{235, 246, 252, 255};
    const afterhours::Color lavender_core{206, 196, 228, 255};
    const afterhours::Color lavender_glow{230, 224, 245, 255};

    ctx.theme = Theme{};
    ctx.theme.background = {234, 249, 246, 255};
    ctx.theme.surface = white;
    ctx.theme.font = ink;
    ctx.theme.font_muted = muted;
    ctx.theme.accent = green_core;
    ctx.theme.focus = blue_core;
    ctx.scaling_mode = ScalingMode::Proportional;

    const float s =
        std::min(ctx.screen_width / 1280.f, ctx.screen_height / 720.f);
    auto *shader = &*fx;

    auto jelly_uniforms = [=](float mode, afterhours::Color core,
                              afterhours::Color glow, float radius,
                              float glass, float concave, float sparkle,
                              RectangleType r) {
      shader->set("u_mode", mode);
      shader->set("u_size", Vector2Type{r.width, r.height});
      shader->set("u_radius", radius * s);
      shader->set("u_pad", 20.f * s);
      shader->set("u_col_core", core);
      shader->set("u_col_glow", glow);
      shader->set("u_glass", glass);
      shader->set("u_concave", concave);
      shader->set("u_sparkle", sparkle);
    };

    auto jelly_draw = [&](float mode, afterhours::Color core,
                          afterhours::Color glow, float radius, float glass,
                          float concave, float sparkle) {
      return [=](RectangleType r) {
        if (shader->ok()) {
          RectangleType big{r.x - 20.f * s, r.y - 20.f * s,
                            r.width + 40.f * s, r.height + 40.f * s};
          jelly_uniforms(mode, core, glow, radius, glass, concave, sparkle,
                         big);
          afterhours::effects::Effect::Scope scope(*shader);
          afterhours::draw_rectangle(big, white);
        } else {
          afterhours::draw_rectangle(r, glass > 0.5f ? glass_col : core);
        }
      };
    };

    auto check_mark = [s](Vector2Type c, float k, afterhours::Color col) {
      afterhours::draw_line_ex({c.x - 5.f * k * s, c.y},
                               {c.x - 1.5f * k * s, c.y + 4.f * k * s},
                               2.5f * k * s, col);
      afterhours::draw_line_ex({c.x - 1.5f * k * s, c.y + 4.f * k * s},
                               {c.x + 5.5f * k * s, c.y - 4.5f * k * s},
                               2.5f * k * s, col);
    };

    // ---- Page: mint gingham tablecloth -------------------------------
    auto page = div(
        ctx, mk(entity),
        ComponentConfig{}
            .with_size({pixels(1280.f * s), pixels(720.f * s)})
            .with_absolute_position((ctx.screen_width - 1280.f * s) / 2.f,
                                    (ctx.screen_height - 720.f * s) / 2.f)
            .with_transparent_bg()
            .with_corner_radius(0)
            .with_on_draw_bg([=](RectangleType r) {
              if (shader->ok()) {
                shader->set("u_mode", 2.f);
                shader->set("u_size", Vector2Type{r.width, r.height});
                shader->set("u_radius", 26.f * s);
                afterhours::effects::Effect::Scope scope(*shader);
                afterhours::draw_rectangle(r, white);
              }
            })
            .with_debug_name("gummy_page"));

    auto label = [&](int id, float x, float y, float w, float h, float pt,
                     const std::string &text, afterhours::Color color,
                     TextAlignment align = TextAlignment::Left,
                     const char *name = "") {
      div(ctx, mk(page.ent(), id),
          ComponentConfig{}
              .with_absolute_position(x * s, y * s)
              .with_size({pixels(w * s), pixels(h * s)})
              .with_label(text)
              .with_alignment(align)
              .with_font("FredokaMockBold", pixels(pt * s))
              .with_custom_text_color(color)
              .with_transparent_bg()
              .with_ignore_pointer_events()
              .with_debug_name(name));
    };

    // ---- Left product panel (buttercream polka dots) ------------------
    div(ctx, mk(page.ent(), 1),
        ComponentConfig{}
            .with_absolute_position(68.f * s, 58.f * s)
            .with_size({pixels(410.f * s), pixels(604.f * s)})
            .with_transparent_bg()
            .with_ignore_pointer_events()
            .with_on_draw_bg([=](RectangleType r) {
              if (shader->ok()) {
                RectangleType big{r.x - 20.f * s, r.y - 20.f * s,
                                  r.width + 40.f * s, r.height + 40.f * s};
                jelly_uniforms(3.f, white, white, 14.f, 0.f, 0.f, 15.f * s,
                               big);
                afterhours::effects::Effect::Scope scope(*shader);
                afterhours::draw_rectangle(big, white);
              }
            })
            .with_debug_name("gummy_panel"));

    // ---- Pink studio photo box + gummy bears --------------------------
    div(ctx, mk(page.ent(), 2),
        ComponentConfig{}
            .with_absolute_position(84.f * s, 74.f * s)
            .with_size({pixels(378.f * s), pixels(326.f * s)})
            .with_transparent_bg()
            .with_ignore_pointer_events()
            .with_on_draw_bg([=](RectangleType r) {
              if (shader->ok()) {
                RectangleType big{r.x - 20.f * s, r.y - 20.f * s,
                                  r.width + 40.f * s, r.height + 40.f * s};
                jelly_uniforms(4.f, white, white, 18.f, 0.f, 0.f, 0.f, big);
                afterhours::effects::Effect::Scope scope(*shader);
                afterhours::draw_rectangle(big, white);
              }
            })
            .with_debug_name("gummy_photo"));

    const float bob_o = std::sin(elapsed * 1.7f) * 2.5f;
    const float bob_r = std::sin(elapsed * 1.7f + 0.9f) * 2.5f;
    const float bob_g = std::sin(elapsed * 1.7f + 1.8f) * 2.5f;
    div(ctx, mk(page.ent(), 3),
        ComponentConfig{}
            .with_absolute_position(84.f * s, 74.f * s)
            .with_size({pixels(378.f * s), pixels(326.f * s)})
            .with_transparent_bg()
            .with_ignore_pointer_events()
            .with_on_draw_fg([=, this](RectangleType r) {
              auto draw_bear = [&](raylib::Texture2D tex, float cx, float cy,
                                   float size) {
                if (tex.id == 0) return;
                RectangleType dst{r.x + (cx - size / 2.f) * s,
                                  r.y + (cy - size / 2.f) * s, size * s,
                                  size * s};
                raylib::DrawTexturePro(
                    tex,
                    raylib::Rectangle{0, 0, static_cast<float>(tex.width),
                                      static_cast<float>(tex.height)},
                    raylib::Rectangle{dst.x, dst.y, dst.width, dst.height},
                    raylib::Vector2{0, 0}, 0.f, raylib::WHITE);
              };
              draw_bear(bear_orange, 104.f, 172.f + bob_o, 212.f);
              draw_bear(bear_green, 278.f, 160.f + bob_g, 220.f);
              draw_bear(bear_red, 192.f, 186.f + bob_r, 224.f);
            })
            .with_debug_name("gummy_bears"));

    // SOUR tag, tilted like a sticker.
    div(ctx, mk(page.ent(), 4),
        ComponentConfig{}
            .with_absolute_position(100.f * s, 88.f * s)
            .with_size({pixels(78.f * s), pixels(34.f * s)})
            .with_rotation(-4.f)
            .with_custom_background(white)
            .with_border(afterhours::Color{190, 70, 80, 255}, 2.f * s)
            .with_corner_radius(17.f * s)
            .with_label("SOUR")
            .with_font("FredokaMockBold", pixels(15.f * s))
            .with_custom_text_color(afterhours::Color{150, 50, 60, 255})
            .with_alignment(TextAlignment::Center)
            .with_ignore_pointer_events()
            .with_debug_name("gummy_sour_tag"));

    // ---- Title / stars / price / heart ---------------------------------
    label(5, 84, 416, 380, 46, 34, "Sour cherry bears", ink,
          TextAlignment::Left, "gummy_title");

    for (int i = 0; i < 5; ++i) {
      const bool filled = i < rating;
      auto star = button(
          ctx, mk(page.ent(), 10 + i),
          ComponentConfig{}
              .with_absolute_position((84.f + i * 50.f) * s, 470.f * s)
              .with_size({pixels(46.f * s), pixels(46.f * s)})
              .with_transparent_bg()
              .with_on_draw_bg(jelly_draw(1.f, filled ? gold_core : glass_col,
                                          filled ? gold_glow : glass_col, 0.f,
                                          filled ? 0.f : 1.f, 0.f, 0.f))
              .with(um::on_press({.scale = 0.88f}))
              .with_debug_name("gummy_star_" + std::to_string(i)));
      if (star) rating = i + 1;
    }

    label(16, 84, 552, 180, 44, 36, "$4.50", ink, TextAlignment::Left,
          "gummy_price");

    auto &heart_fill =
        afterhours::motion::anim<afterhours::Color>(Key::HeartFill);
    if (!heart_fill.started()) heart_fill.from(red_core);
    const afterhours::Color heart_col = heart_fill.value();
    const bool heart_liked = liked;
    auto heart_btn = button(
        ctx, mk(page.ent(), 17),
        ComponentConfig{}
            .with_absolute_position(376.f * s, 556.f * s)
            .with_size({pixels(80.f * s), pixels(80.f * s)})
            .with_transparent_bg()
            .with_on_draw_bg(jelly_draw(0.f, liked ? red_core : glass_col,
                                        liked ? red_glow : glass_col, 40.f,
                                        liked ? 0.f : 1.f, 0.f, 0.f))
            .with(um::on_change(static_cast<size_t>(likes),
                                {.scale = {0.82f, 1.f}},
                                afterhours::motion::Spring{.response = 0.35f,
                                                             .bounce = 0.6f}))
            .with_on_draw_fg([heart_col, heart_liked](RectangleType r) {
              const float w = r.width, h = r.height;
              const float k = heart_liked ? 1.f : 0.62f;
              const float cx = r.x + 0.5f * w, cy = r.y + 0.52f * h;
              afterhours::draw_circle_v({cx - 0.16f * w * k, cy - 0.10f * h * k},
                                        0.19f * w * k, heart_col);
              afterhours::draw_circle_v({cx + 0.16f * w * k, cy - 0.10f * h * k},
                                        0.19f * w * k, heart_col);
              afterhours::draw_triangle(
                  {cx, cy + 0.34f * h * k},
                  {cx + 0.34f * w * k, cy - 0.02f * h * k},
                  {cx - 0.34f * w * k, cy - 0.02f * h * k}, heart_col);
            })
            .with_debug_name("gummy_heart"));
    if (heart_btn) {
      liked = !liked;
      likes += liked ? 1 : -1;
      heart_fill.to(liked ? white : red_core,
                    afterhours::motion::Timeline{
                        .keys = {{0.f, 0.f}, {0.15f, 1.f}}});
    }
    div(ctx, mk(page.ent(), 18),
        ComponentConfig{}
            .with_absolute_position(434.f * s, 550.f * s)
            .with_size({pixels(40.f * s), pixels(40.f * s)})
            .with_transparent_bg()
            .with_ignore_pointer_events()
            .with_on_draw_bg(
                jelly_draw(0.f, blue_core, blue_glow, 18.f, 0.f, 0.f, 0.f))
            .with_label(std::to_string(likes))
            .with_alignment(TextAlignment::Center)
            .with_font("FredokaMockBold", pixels(15.f * s))
            .with_custom_text_color(white)
            .with_debug_name("gummy_badge"));

    // ---- Flavor radio bubbles -------------------------------------------
    const std::array<afterhours::Color, 3> radio_cores{red_core, green_core,
                                                       blue_core};
    const std::array<afterhours::Color, 3> radio_glows{red_glow, green_glow,
                                                       blue_glow};
    for (int i = 0; i < 3; ++i) {
      const bool sel = flavor == i;
      const float cx = 578.f + i * 106.f;
      auto bubble = button(
          ctx, mk(page.ent(), 20 + i),
          ComponentConfig{}
              .with_absolute_position((cx - 39.f) * s, 72.f * s)
              .with_size({pixels(78.f * s), pixels(78.f * s)})
              .with_transparent_bg()
              .with_on_draw_bg(jelly_draw(0.f, glass_col, glass_col, 39.f, 1.f,
                                          0.f, 0.f))
              .with(um::on_press({.scale = 0.92f}))
              .with_debug_name("gummy_radio_" + std::to_string(i)));
      if (sel) {
        div(ctx, mk(page.ent(), 24),
            ComponentConfig{}
                .with_absolute_position((cx - 24.f) * s, 87.f * s)
                .with_size({pixels(48.f * s), pixels(48.f * s)})
                .with_transparent_bg()
                .with_ignore_pointer_events()
                .with_on_draw_bg(jelly_draw(0.f, radio_cores[static_cast<size_t>(i)],
                                            radio_glows[static_cast<size_t>(i)],
                                            24.f, 0.f, 0.f, 0.f))
                .with_debug_name("gummy_radio_core"));
      }
      if (bubble) flavor = i;
    }

    // ---- Toggle (toggle-morph: leading edge first) ------------------------
    auto &tog_front = afterhours::motion::anim(Key::TogFront);
    auto &tog_back = afterhours::motion::anim(Key::TogBack);
    if (!tog_front.started()) {
      tog_front.from(toggle_on ? 1.f : 0.f);
      tog_back.from(toggle_on ? 1.f : 0.f);
    }
    const float tog_travel = 70.f;  // 128 track, 50 knob, 4 inset each side
    const float tog_f = std::clamp(tog_front.value_or(1.f), 0.f, 1.f);
    const float tog_b = std::clamp(tog_back.value_or(1.f), 0.f, 1.f);
    const float knob_x = 1086.f + 4.f + tog_b * tog_travel;
    const float knob_w = 50.f + std::abs(tog_f - tog_b) * tog_travel * 0.45f;
    auto tog_track = button(
        ctx, mk(page.ent(), 28),
        ComponentConfig{}
            .with_absolute_position(1086.f * s, 82.f * s)
            .with_size({pixels(128.f * s), pixels(58.f * s)})
            .with_transparent_bg()
            .with_on_draw_bg(jelly_draw(0.f, toggle_on ? green_core : grey_core,
                                        toggle_on ? green_glow : grey_glow,
                                        29.f, 0.f, 0.f, 0.f))
            .with_debug_name("gummy_toggle_track"));
    div(ctx, mk(page.ent(), 29),
        ComponentConfig{}
            .with_absolute_position(knob_x * s, 86.f * s)
            .with_size({pixels(knob_w * s), pixels(50.f * s)})
            .with_transparent_bg()
            .with_ignore_pointer_events()
            .with_on_draw_bg(
                jelly_draw(0.f, yellow_core, yellow_glow, 25.f, 0.f, 0.f, 0.f))
            .with_debug_name("gummy_toggle_knob"));
    if (tog_track) {
      toggle_on = !toggle_on;
      const float target = toggle_on ? 1.f : 0.f;
      tog_front.to(target, afterhours::motion::Spring{.response = 0.22f,
                                                       .bounce = 0.f});
      tog_back.to(target, afterhours::motion::Spring{.response = 0.34f,
                                                      .bounce = 0.15f});
    }

    // ---- Slider (parameter scrub + goo trail) ------------------------------
    auto &blob = afterhours::motion::anim(Key::SliderBlob);
    if (!blob.started()) blob.from(volume);
    if (std::abs(blob.target() - volume) > 0.0005f)
      blob.to(volume, afterhours::motion::Spring{.response = 0.16f,
                                                 .bounce = 0.35f});
    const float blob_v = std::clamp(blob.value_or(volume), 0.f, 1.f);
    const float vol_now = volume;

    div(ctx, mk(page.ent(), 40),
        ComponentConfig{}
            .with_absolute_position(540.f * s, 238.f * s)
            .with_size({pixels(674.f * s), pixels(26.f * s)})
            .with_transparent_bg()
            .with_ignore_pointer_events()
            .with_on_draw_bg(jelly_draw(0.f, glass_col, glass_col, 13.f, 1.f,
                                        1.f, 0.f))
            .with_debug_name("gummy_slider_track"));
    div(ctx, mk(page.ent(), 41),
        ComponentConfig{}
            .with_absolute_position(540.f * s, 238.f * s)
            .with_size({pixels(std::max(26.f, 674.f * volume) * s),
                        pixels(26.f * s)})
            .with_transparent_bg()
            .with_ignore_pointer_events()
            .with_on_draw_bg(
                jelly_draw(0.f, blue_core, blue_glow, 13.f, 0.f, 0.f, 1.f))
            .with_debug_name("gummy_slider_fill"));
    div(ctx, mk(page.ent(), 42),
        ComponentConfig{}
            .with_absolute_position(540.f * s, 220.f * s)
            .with_size({pixels(674.f * s), pixels(62.f * s)})
            .with_transparent_bg()
            .with_ignore_pointer_events()
            .with_on_draw_fg([s, blob_v, vol_now, blue_core](RectangleType r) {
              const float kx = r.x + 31.f * s + vol_now * (r.width - 62.f * s);
              const float bx = r.x + 31.f * s + blob_v * (r.width - 62.f * s);
              const float cy = r.y + r.height * 0.5f;
              if (std::abs(kx - bx) <= 6.f * s) return;
              for (int i = 1; i <= 3; ++i) {
                const float t = i / 4.f;
                afterhours::draw_circle_v(
                    {bx + (kx - bx) * t, cy}, (26.f - i * 5.f) * s,
                    afterhours::Color{
                        blue_core.r, blue_core.g, blue_core.b,
                        static_cast<unsigned char>(90 - i * 20)});
              }
            })
            .with_debug_name("gummy_slider_goo"));
    div(ctx, mk(page.ent(), 43),
        ComponentConfig{}
            .with_absolute_position((540.f + volume * (674.f - 62.f)) * s,
                                    220.f * s)
            .with_size({pixels(62.f * s), pixels(62.f * s)})
            .with_transparent_bg()
            .with_ignore_pointer_events()
            .with_on_draw_bg(
                jelly_draw(0.f, yellow_core, yellow_glow, 31.f, 0.f, 0.f, 0.f))
            .with_debug_name("gummy_slider_knob"));
    auto slider_hit = button(
        ctx, mk(page.ent(), 44),
        ComponentConfig{}
            .with_absolute_position(540.f * s, 214.f * s)
            .with_size({pixels(674.f * s), pixels(74.f * s)})
            .with_transparent_bg()
            .with_debug_name("gummy_slider"));
    if (slider_hit) slider_dragging = true;
    if (slider_dragging) {
      if (ctx.mouse.left_down) {
        const float page_x = (ctx.screen_width - 1280.f * s) / 2.f;
        const float lx = (ctx.mouse.pos.x - page_x) / s;
        volume = std::clamp((lx - 540.f - 31.f) / (674.f - 62.f), 0.f, 1.f);
      } else {
        slider_dragging = false;
      }
    }

    // ---- Size segmented (tab-underline-slide) + stepper --------------------
    auto &seg_front = afterhours::motion::anim(Key::SegFront);
    auto &seg_back = afterhours::motion::anim(Key::SegBack);
    if (!seg_front.started()) {
      seg_front.from(static_cast<float>(size_index));
      seg_back.from(static_cast<float>(size_index));
    }
    const float sf = seg_front.value_or(0.f);
    const float sb = seg_back.value_or(0.f);
    const float seg_w = 410.f / 3.f;
    const float pill_lo = std::min(sf, sb);
    const float pill_hi = std::max(sf, sb);

    div(ctx, mk(page.ent(), 50),
        ComponentConfig{}
            .with_absolute_position(540.f * s, 330.f * s)
            .with_size({pixels(410.f * s), pixels(62.f * s)})
            .with_transparent_bg()
            .with_ignore_pointer_events()
            .with_on_draw_bg(jelly_draw(0.f, glass_col, glass_col, 31.f, 1.f,
                                        1.f, 0.f))
            .with_debug_name("gummy_seg_track"));
    div(ctx, mk(page.ent(), 51),
        ComponentConfig{}
            .with_absolute_position((540.f + 4.f + pill_lo * seg_w) * s,
                                    334.f * s)
            .with_size({pixels(((pill_hi - pill_lo) * seg_w + seg_w - 8.f) * s),
                        pixels(54.f * s)})
            .with_transparent_bg()
            .with_ignore_pointer_events()
            .with_on_draw_bg(
                jelly_draw(0.f, orange_core, orange_glow, 27.f, 0.f, 0.f, 0.f))
            .with_debug_name("gummy_seg_pill"));

    const std::array<const char *, 3> sizes{"Small", "Medium", "Large"};
    const float pill_center = (pill_lo + pill_hi) * 0.5f;
    for (int i = 0; i < 3; ++i) {
      const float near_t =
          std::clamp(1.f - std::abs(pill_center - static_cast<float>(i)), 0.f,
                     1.f);
      afterhours::Color label_col{
          static_cast<unsigned char>(ink.r + (255 - ink.r) * near_t),
          static_cast<unsigned char>(ink.g + (255 - ink.g) * near_t),
          static_cast<unsigned char>(ink.b + (255 - ink.b) * near_t), 255};
      auto seg = button(
          ctx, mk(page.ent(), 52 + i),
          ComponentConfig{}
              .with_absolute_position((540.f + i * seg_w) * s, 330.f * s)
              .with_size({pixels(seg_w * s), pixels(62.f * s)})
              .with_label(sizes[static_cast<size_t>(i)])
              .with_font("FredokaMockBold", pixels(19.f * s))
              .with_custom_text_color(label_col)
              .with_alignment(TextAlignment::Center)
              .with_transparent_bg()
              .with(um::on_press({.scale = 0.96f}))
              .with_debug_name("gummy_seg_" + std::to_string(i)));
      if (seg && size_index != i) {
        size_index = i;
        seg_front.to(static_cast<float>(i),
                     afterhours::motion::Spring{.response = 0.30f,
                                                  .bounce = 0.05f});
        seg_back.to(static_cast<float>(i),
                    afterhours::motion::Spring{.response = 0.42f,
                                                 .bounce = 0.05f});
      }
    }

    div(ctx, mk(page.ent(), 55),
        ComponentConfig{}
            .with_absolute_position(970.f * s, 330.f * s)
            .with_size({pixels(244.f * s), pixels(62.f * s)})
            .with_transparent_bg()
            .with_ignore_pointer_events()
            .with_on_draw_bg(jelly_draw(0.f, glass_col, glass_col, 31.f, 1.f,
                                        1.f, 0.f))
            .with_debug_name("gummy_stepper_track"));
    auto minus_btn = button(
        ctx, mk(page.ent(), 56),
        ComponentConfig{}
            .with_absolute_position(974.f * s, 334.f * s)
            .with_size({pixels(54.f * s), pixels(54.f * s)})
            .with_label("-")
            .with_font("FredokaMockBold", pixels(26.f * s))
            .with_custom_text_color(ink)
            .with_alignment(TextAlignment::Center)
            .with_transparent_bg()
            .with_on_draw_bg(jelly_draw(0.f, lavender_core, lavender_glow, 27.f,
                                        0.f, 0.f, 0.f))
            .with(um::on_press({.scale = 0.92f}))
            .with_debug_name("gummy_qty_minus"));
    if (minus_btn && qty > 1) --qty;
    label(57, 1052, 344, 80, 36, 28, std::to_string(qty), ink,
          TextAlignment::Center, "gummy_qty");
    auto plus_btn = button(
        ctx, mk(page.ent(), 58),
        ComponentConfig{}
            .with_absolute_position(1156.f * s, 334.f * s)
            .with_size({pixels(54.f * s), pixels(54.f * s)})
            .with_label("+")
            .with_font("FredokaMockBold", pixels(26.f * s))
            .with_custom_text_color(white)
            .with_alignment(TextAlignment::Center)
            .with_transparent_bg()
            .with_on_draw_bg(
                jelly_draw(0.f, red_core, red_glow, 27.f, 0.f, 0.f, 0.f))
            .with(um::on_press({.scale = 0.92f}))
            .with_debug_name("gummy_qty_plus"));
    if (plus_btn && qty < 99) ++qty;

    // ---- Extras chips + gift wrap checkbox (chip-select) -------------------
    struct ChipDef {
      const char *name;
      afterhours::Color core;
      afterhours::Color glow;
      afterhours::Color text;
    };
    const std::array<ChipDef, 3> chips{
        {{"Fizzy", purple_core, purple_glow, purple_core},
         {"Vegan", green_core, green_glow, {46, 140, 40, 255}},
         {"Sugared", orange_core, orange_glow, {181, 69, 25, 255}}}};
    const std::array<float, 3> chip_x{540.f, 690.f, 838.f};
    const std::array<float, 3> chip_w{138.f, 138.f, 170.f};
    for (int i = 0; i < 3; ++i) {
      const bool sel = extras[static_cast<size_t>(i)];
      const ChipDef &def = chips[static_cast<size_t>(i)];
      auto chip = button(
          ctx, mk(page.ent(), 60 + i),
          ComponentConfig{}
              .with_absolute_position(chip_x[static_cast<size_t>(i)] * s,
                                      442.f * s)
              .with_size({pixels(chip_w[static_cast<size_t>(i)] * s),
                          pixels(56.f * s)})
              .with_label(def.name)
              .with_font("FredokaMockBold", pixels(19.f * s))
              .with_custom_text_color(sel ? white : def.text)
              .with_alignment(TextAlignment::Center)
              .with_transparent_bg()
              .with_on_draw_bg(jelly_draw(0.f, sel ? def.core : glass_col,
                                          sel ? def.glow : glass_col, 28.f,
                                          sel ? 0.f : 1.f, 0.f, 0.f))
              .with_on_draw_fg([sel, s, check_mark, def](RectangleType r) {
                const Vector2Type c{r.x + 26.f * s, r.y + r.height * 0.5f};
                if (sel) {
                  check_mark(c, 1.15f, afterhours::Color{255, 255, 255, 255});
                } else {
                  afterhours::draw_line_ex({c.x - 6.f * s, c.y},
                                           {c.x + 6.f * s, c.y}, 3.f * s,
                                           def.text);
                  afterhours::draw_line_ex({c.x, c.y - 6.f * s},
                                           {c.x, c.y + 6.f * s}, 3.f * s,
                                           def.text);
                }
              })
              .with(um::on_state(sel, {.scale = {1.f, 1.02f}},
                                 afterhours::motion::Spring{.response = 0.18f,
                                                              .bounce = 0.25f}))
              .with(um::on_press({.scale = 0.94f}))
              .with_debug_name("gummy_chip_" + std::to_string(i)));
      if (chip) extras[static_cast<size_t>(i)] = !sel;
    }

    const bool gift_checked_now = gift_check;
    auto gift_box = button(
        ctx, mk(page.ent(), 64),
        ComponentConfig{}
            .with_absolute_position(1030.f * s, 444.f * s)
            .with_size({pixels(52.f * s), pixels(52.f * s)})
            .with_transparent_bg()
            .with_on_draw_bg(jelly_draw(0.f, gift_check ? red_core : glass_col,
                                        gift_check ? red_glow : glass_col, 14.f,
                                        gift_check ? 0.f : 1.f, 0.f, 0.f))
            .with_on_draw_fg([gift_checked_now, s, check_mark](RectangleType r) {
              if (!gift_checked_now) return;
              check_mark({r.x + r.width * 0.5f, r.y + r.height * 0.52f}, 1.7f,
                         afterhours::Color{255, 255, 255, 255});
            })
            .with(um::on_press({.scale = 0.92f}))
            .with_debug_name("gummy_gift_box"));
    if (gift_box) gift_check = !gift_check;
    label(65, 1096, 454, 120, 32, 20, "Gift wrap", ink, TextAlignment::Left,
          "gummy_gift_text");

    // ---- Free-delivery progress (progress-fill + count-up) ------------------
    const float unit_price = size_index == 0 ? 4.50f
                             : size_index == 1 ? 6.50f
                                               : 8.50f;
    const float cart_target =
        std::clamp(unit_price * static_cast<float>(qty) / 25.f +
                       (gift_check ? 0.08f : 0.f),
                   0.f, 1.f);
    auto &prog = afterhours::motion::anim(Key::ProgFill);
    if (!prog.started()) prog.from(0.f);
    if (std::abs(prog.target() - cart_target) > 0.001f)
      prog.to(cart_target, afterhours::motion::Timeline{
                               .keys = {{0.f, 0.f}, {0.85f, 1.f}},
                               .curve = afterhours::motion::curves::
                                   ease_out_quad});
    const float pv = std::clamp(prog.value_or(0.f), 0.f, 1.f);
    const bool free_delivery = cart_target >= 1.f;

    div(ctx, mk(page.ent(), 70),
        ComponentConfig{}
            .with_absolute_position(540.f * s, 552.f * s)
            .with_size({pixels(440.f * s), pixels(40.f * s)})
            .with_transparent_bg()
            .with_ignore_pointer_events()
            .with_on_draw_bg(jelly_draw(0.f, glass_col, glass_col, 20.f, 1.f,
                                        1.f, 0.f))
            .with_debug_name("gummy_prog_track"));
    div(ctx, mk(page.ent(), 71),
        ComponentConfig{}
            .with_absolute_position(540.f * s, 552.f * s)
            .with_size({pixels(std::max(40.f, 440.f * pv) * s),
                        pixels(40.f * s)})
            .with_transparent_bg()
            .with_ignore_pointer_events()
            .with_on_draw_bg(
                jelly_draw(0.f, green_core, green_glow, 20.f, 0.f, 0.f, 0.f))
            .with_debug_name("gummy_prog_fill"));
    div(ctx, mk(page.ent(), 72),
        ComponentConfig{}
            .with_absolute_position(996.f * s, 546.f * s)
            .with_size({pixels(218.f * s), pixels(52.f * s)})
            .with_transparent_bg()
            .with_ignore_pointer_events()
            .with_on_draw_bg(jelly_draw(
                0.f,
                free_delivery ? afterhours::Color{214, 245, 190, 255}
                              : white,
                free_delivery ? afterhours::Color{230, 250, 210, 255}
                              : white,
                12.f, 0.f, 0.f, 0.f))
            .with_label(free_delivery ? "Free delivery"
                                      : "Free delivery at $25")
            .with_font("FredokaMockBold", pixels(17.f * s))
            .with_custom_text_color(
                free_delivery ? afterhours::Color{46, 140, 40, 255} : ink)
            .with_alignment(TextAlignment::Center)
            .with_on_draw_fg([free_delivery, s, check_mark](RectangleType r) {
              if (!free_delivery) return;
              check_mark({r.x + r.width - 26.f * s, r.y + r.height * 0.5f},
                         1.1f, afterhours::Color{46, 140, 40, 255});
            })
            .with_debug_name("gummy_delivery"));
  }
};

REGISTER_EXAMPLE_SCREEN(gummy_shop, "Game Mockups",
                        "Gummy candy product card with jelly SDF controls",
                        GummyShop)
