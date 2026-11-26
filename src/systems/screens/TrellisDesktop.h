#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include "trellis/DockWorkspace.h"
#include <afterhours/ah.h>

#include <cmath>
#include <string>
#include <vector>

// Trellis Desktop: the fractal desktop rebuilt on the dock workspace.
// Windows start floating on a wallpaper stage; dock them beside each
// other, nest as deep as you like, double-click to zoom between levels.
struct TrellisDesktop : ScreenSystem<afterhours::ui::UIContext<InputAction>> {
  using Ctx = afterhours::ui::UIContext<InputAction>;
  dock::Workspace<Ctx> ws;
  bool built = false;
  float t_accum = 0.f;
  std::string note_text = "Ship the fractal desktop.";
  int counter = 0;
  bool music_playing = false;

  void reset_layout() {
    using dock::Node;
    ws.doc.root.reset();
    ws.doc.floating = {
        {"notes", {120.f, 90.f, 380.f, 300.f}},
        {"clock", {760.f, 70.f, 300.f, 240.f}},
        {"gallery", {300.f, 380.f, 420.f, 260.f}},
        {"music", {820.f, 380.f, 340.f, 230.f}},
    };
    ws.frame_all();
  }

  void seed_defaults() {
    t_accum = 0.f;
    note_text = "Ship the fractal desktop.";
    counter = 0;
    music_playing = false;
  }

  void reset_all() {
    seed_defaults();
    reset_layout();
    ws.reset_camera_history();
  }

  void build() {
    built = true;
    seed_defaults();
    ws.defs["notes"] = {"Notes", {248, 185, 40, 255},
                        [this](Ctx &c, afterhours::Entity &p, RectangleType r, float) { draw_notes(c, p, r); }};
    ws.defs["clock"] = {"Clock", {23, 86, 227, 255},
                        [this](Ctx &c, afterhours::Entity &p, RectangleType r, float) { draw_clock(c, p, r); }};
    ws.defs["gallery"] = {"Gallery", {214, 24, 60, 255},
                          [this](Ctx &c, afterhours::Entity &p, RectangleType r, float) { draw_gallery(c, p, r); }};
    ws.defs["music"] = {"Music", {79, 26, 165, 255},
                        [this](Ctx &c, afterhours::Entity &p, RectangleType r, float) { draw_music(c, p, r); }};
    ws.defs["counter"] = {"Counter", {62, 178, 32, 255},
                          [this](Ctx &c, afterhours::Entity &p, RectangleType r, float) { draw_counter(c, p, r); }};
    reset_layout();
  }

  void line(Ctx &ctx, afterhours::Entity &parent, int id, float x, float y,
            float w, const std::string &text, afterhours::Color col,
            float pt = 13.f) {
    using namespace afterhours::ui;
    using namespace afterhours::ui::imm;
    div(ctx, mk(parent, id),
        ComponentConfig{}
            .with_absolute_position(x, y)
            .with_size({pixels(w), pixels(22.f)})
            .with_label(text)
            .with_font("FredokaMockBold", pixels(pt))
            .with_custom_text_color(col)
            .with_transparent_bg()
            .with_ignore_pointer_events());
  }

  void draw_notes(Ctx &ctx, afterhours::Entity &parent, RectangleType r) {
    using namespace afterhours::ui;
    using namespace afterhours::ui::imm;
    line(ctx, parent, 0, 10.f, 6.f, r.width - 20.f, note_text,
         {60, 52, 20, 255}, 15.f);
    line(ctx, parent, 1, 10.f, 34.f, r.width - 20.f,
         "Drag me onto an edge to dock.", {85, 70, 30, 255});
    line(ctx, parent, 2, 10.f, 56.f, r.width - 20.f,
         "Double-click my header to zoom.", {85, 70, 30, 255});
    auto add = button(ctx, mk(parent, 3),
                      ComponentConfig{}
                          .with_absolute_position(10.f, r.height - 46.f)
                          .with_size({pixels(150.f), pixels(36.f)})
                          .with_label("Add line")
                          .with_font("FredokaMockBold", pixels(13.f))
                          .with_corner_radius(8.f)
                          .with_debug_name("desk_note_add"));
    if (add) note_text += " More nesting!";
  }

  void draw_clock(Ctx &ctx, afterhours::Entity &parent, RectangleType r) {
    using namespace afterhours::ui;
    using namespace afterhours::ui::imm;
    const float t = t_accum;
    div(ctx, mk(parent, 0),
        ComponentConfig{}
            .with_absolute_position(0.f, 0.f)
            .with_size({pixels(r.width), pixels(r.height)})
            .with_transparent_bg()
            .with_ignore_pointer_events()
            .with_on_draw_fg([t](RectangleType rr) {
              const float cx = rr.x + rr.width / 2.f;
              const float cy = rr.y + rr.height / 2.f;
              const float rad = std::min(rr.width, rr.height) * 0.38f;
              afterhours::draw_circle_v({cx, cy}, rad, {235, 244, 255, 255});
              const float sa = t * 1.0472f - 1.5708f;
              const float ma = t * 0.08727f - 1.5708f;
              const float ha = t * 0.00727f - 1.5708f;
              afterhours::draw_line_ex({cx, cy},
                                       {cx + std::cos(ha) * rad * 0.5f,
                                        cy + std::sin(ha) * rad * 0.5f},
                                       5.f, {45, 52, 72, 255});
              afterhours::draw_line_ex({cx, cy},
                                       {cx + std::cos(ma) * rad * 0.75f,
                                        cy + std::sin(ma) * rad * 0.75f},
                                       3.5f, {45, 52, 72, 255});
              afterhours::draw_line_ex({cx, cy},
                                       {cx + std::cos(sa) * rad * 0.88f,
                                        cy + std::sin(sa) * rad * 0.88f},
                                       2.f, {214, 24, 60, 255});
              afterhours::draw_circle_v({cx, cy}, 5.f, {45, 52, 72, 255});
            })
            .with_debug_name("desk_clock_face"));
  }

  void draw_gallery(Ctx &ctx, afterhours::Entity &parent, RectangleType r) {
    using namespace afterhours::ui;
    using namespace afterhours::ui::imm;
    static const afterhours::Color tiles[6] = {
        {214, 24, 60, 255}, {232, 106, 42, 255}, {248, 185, 40, 255},
        {62, 178, 32, 255}, {23, 86, 227, 255},  {79, 26, 165, 255}};
    const float tw = (r.width - 40.f) / 3.f, th = (r.height - 30.f) / 2.f;
    for (int i = 0; i < 6; ++i) {
      div(ctx, mk(parent, i),
          ComponentConfig{}
              .with_absolute_position(10.f + (i % 3) * (tw + 10.f),
                                      8.f + (i / 3) * (th + 10.f))
              .with_size({pixels(tw), pixels(th)})
              .with_custom_background(tiles[i])
              .with_corner_radius(10.f)
              .with_ignore_pointer_events());
    }
  }

  void draw_music(Ctx &ctx, afterhours::Entity &parent, RectangleType r) {
    using namespace afterhours::ui;
    using namespace afterhours::ui::imm;
    line(ctx, parent, 0, 10.f, 6.f, r.width - 20.f, "Midnight Drive — Loops",
         {45, 52, 72, 255}, 15.f);
    const float prog = music_playing ? std::fmod(t_accum * 0.05f, 1.f) : 0.3f;
    div(ctx, mk(parent, 1),
        ComponentConfig{}
            .with_absolute_position(10.f, 44.f)
            .with_size({pixels(r.width - 20.f), pixels(14.f)})
            .with_custom_background(afterhours::Color{185, 193, 212, 255})
            .with_corner_radius(7.f)
            .with_ignore_pointer_events());
    div(ctx, mk(parent, 2),
        ComponentConfig{}
            .with_absolute_position(10.f, 44.f)
            .with_size({pixels((r.width - 20.f) * prog), pixels(14.f)})
            .with_custom_background(afterhours::Color{79, 26, 165, 255})
            .with_corner_radius(7.f)
            .with_ignore_pointer_events());
    auto play = button(ctx, mk(parent, 3),
                       ComponentConfig{}
                           .with_absolute_position(10.f, 72.f)
                           .with_size({pixels(110.f), pixels(38.f)})
                           .with_label(music_playing ? "Pause" : "Play")
                           .with_font("FredokaMockBold", pixels(14.f))
                           .with_corner_radius(9.f)
                           .with_debug_name("desk_music_play"));
    if (play) music_playing = !music_playing;
  }

  void draw_counter(Ctx &ctx, afterhours::Entity &parent, RectangleType r) {
    using namespace afterhours::ui;
    using namespace afterhours::ui::imm;
    line(ctx, parent, 0, 10.f, 6.f, r.width - 20.f,
         "Count: " + std::to_string(counter), {45, 52, 72, 255}, 22.f);
    auto inc = button(ctx, mk(parent, 1),
                      ComponentConfig{}
                          .with_absolute_position(10.f, 44.f)
                          .with_size({pixels(120.f), pixels(38.f)})
                          .with_label("Increment")
                          .with_font("FredokaMockBold", pixels(13.f))
                          .with_corner_radius(8.f)
                          .with_debug_name("desk_counter_inc"));
    if (inc) ++counter;
  }

  void for_each_with(afterhours::Entity &entity, Ctx &ctx, float dt) override {
    using namespace afterhours::ui;
    using namespace afterhours::ui::imm;
    if (!built) build();
    t_accum += dt;
    ctx.theme = Theme{};
    ctx.theme.background = {16, 20, 34, 255};
    ctx.theme.font = {45, 52, 72, 255};
    const float s =
        std::min(ctx.screen_width / 1280.f, ctx.screen_height / 720.f);
    auto page = div(ctx, mk(entity),
                    ComponentConfig{}
                        .with_size({pixels(1280.f * s), pixels(720.f * s)})
                        .with_absolute_position(
                            (ctx.screen_width - 1280.f * s) / 2.f,
                            (ctx.screen_height - 720.f * s) / 2.f)
                        .with_custom_background(afterhours::Color{20, 26, 46, 255})
                        .with_debug_name("desk_page"));
    div(ctx, mk(page.ent(), 900),
        ComponentConfig{}
            .with_absolute_position(16.f * s, 10.f * s)
            .with_size({pixels(330.f * s), pixels(34.f * s)})
            .with_label("Desktop — Trellis fractal windows")
            .with_font("FredokaMockBold", pixels(22.f * s))
            .with_custom_text_color(afterhours::Color{235, 240, 250, 255})
            .with_transparent_bg()
            .with_ignore_pointer_events());
    // Toolbar clamps itself left of the global launcher (see DockWorkspace);
    // +Counter sits between the title and the toolbar, clear of both.
    ws.toolbar(ctx, page.ent(), 760.f * s, 10.f * s,
               [this] { reset_all(); }, page.cmp().rect().x, ctx.screen_width);
    auto spawn = button(ctx, mk(page.ent(), 910),
                        ComponentConfig{}
                            .with_absolute_position(352.f * s, 10.f * s)
                            .with_size({pixels(150.f * s), pixels(30.f * s)})
                            .with_label("+ Counter")
                            .with_font("FredokaMockBold", pixels(13.f * s))
                            .with_corner_radius(8.f * s)
                            .with_debug_name("desk_spawn_counter"));
    if (spawn && !ws.is_floating("counter") &&
        !(ws.doc.root && ws.doc.root->contains("counter")))
      ws.doc.floating.push_back({"counter", {520.f, 200.f, 300.f, 200.f}});
    // Bottom 48px of the page is reserved for the global HUD.
    ws.render(ctx, page.ent(),
              {page.cmp().rect().x + 12.f * s, page.cmp().rect().y + 52.f * s,
               1256.f * s, 620.f * s},
              dt);
  }
};

REGISTER_EXAMPLE_SCREEN(trellis_desktop, "Game Mockups",
                        "Trellis Desktop: floating windows that dock and nest",
                        TrellisDesktop)
