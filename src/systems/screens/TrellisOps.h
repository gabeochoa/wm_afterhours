#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include "../../plugins/dock_workspace/DockWorkspace.h"
#include <afterhours/ah.h>

#include <cmath>
#include <deque>
#include <string>
#include <vector>

// Trellis Ops dashboard: live charts, logs and a runbook panel on a
// dockable workspace. Charts keep streaming while panels move.
struct TrellisOps : ScreenSystem<afterhours::ui::UIContext<InputAction>> {
  using Ctx = afterhours::ui::UIContext<InputAction>;
  dock::Workspace<Ctx> ws;
  bool built = false;
  float t_accum = 0.f;
  float log_timer = 0.f;
  int log_seq = 0;
  std::deque<std::string> logs;
  std::deque<bool> runbook_done{false, false, false, false};
  bool paused = false;

  void reset_layout() {
    using dock::Node;
    ws.doc.root = Node::splitv(
        true, {0.62f, 0.38f},
        Node::splitv(false, {0.62f, 0.38f}, Node::leaf("chart"),
                     Node::leaf("metrics")),
        Node::splitv(false, {0.58f, 0.42f}, Node::leaf("logs"),
                     Node::leaf("runbook")));
    ws.doc.floating.clear();
    ws.frame_all();
  }

  void seed_defaults() {
    t_accum = 0.f;
    log_timer = 0.f;
    log_seq = 0;
    logs.clear();
    logs.push_back("ops: deploy pulse-api v1.4.2 started");
    logs.push_back("ops: health checks passing (12/12)");
    runbook_done = {false, false, false, false};
    paused = false;
  }

  void reset_all() {
    seed_defaults();
    reset_layout();
    ws.reset_camera_history();
  }

  void build() {
    built = true;
    seed_defaults();
    ws.defs["chart"] = {"Latency (live)", {23, 86, 227, 255},
                        [this](Ctx &c, afterhours::Entity &p, RectangleType r, float) { draw_chart(c, p, r); },
                        260.f, 170.f};
    ws.defs["metrics"] = {"Metrics", {62, 178, 32, 255},
                          [this](Ctx &c, afterhours::Entity &p, RectangleType r, float) { draw_metrics(c, p, r); },
                          190.f, 180.f};
    ws.defs["logs"] = {"Logs", {45, 52, 72, 255},
                       [this](Ctx &c, afterhours::Entity &p, RectangleType r, float) { draw_logs(c, p, r); },
                       240.f, 120.f};
    ws.defs["runbook"] = {"Runbook", {248, 185, 40, 255},
                          [this](Ctx &c, afterhours::Entity &p, RectangleType r, float) { draw_runbook(c, p, r); },
                          260.f, 190.f};
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
            .with_font("AtkinsonMock", pixels(pt))
            .with_custom_text_color(col)
            .with_transparent_bg()
            .with_ignore_pointer_events());
  }

  void draw_chart(Ctx &ctx, afterhours::Entity &parent, RectangleType r) {
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
              const int n = 48;
              Vector2Type prev{0, 0};
              for (int i = 0; i < n; ++i) {
                const float fx = i / float(n - 1);
                const float v = 0.55f + 0.28f * std::sin(t * 1.3f + fx * 9.f) +
                                0.10f * std::sin(t * 3.1f + fx * 23.f);
                Vector2Type pt{rr.x + 8.f + fx * (rr.width - 16.f),
                               rr.y + rr.height - 14.f - v * (rr.height - 36.f)};
                if (i > 0)
                  afterhours::draw_line_ex(prev, pt, 2.5f, {23, 86, 227, 255});
                prev = pt;
              }
            })
            .with_debug_name("ops_chart_line"));
    line(ctx, parent, 1, 8.f, 2.f, r.width - 16.f,
         "p99 latency — streaming, moves with the panel", {28, 36, 54, 255});
  }

  void draw_metrics(Ctx &ctx, afterhours::Entity &parent, RectangleType r) {
    const int rps = 1200 + int(300.f * std::sin(t_accum * 0.7f));
    const int err = 2 + int(1.5f + 1.5f * std::sin(t_accum * 1.9f));
    line(ctx, parent, 0, 10.f, 8.f, r.width - 20.f,
         "Requests: " + std::to_string(rps) + "/s", {28, 36, 54, 255}, 16.f);
    line(ctx, parent, 1, 10.f, 38.f, r.width - 20.f,
         "Errors: " + std::to_string(err) + "/min",
         err > 2 ? afterhours::Color{200, 70, 60, 255}
                 : afterhours::Color{62, 140, 60, 255},
         16.f);
    line(ctx, parent, 2, 10.f, 68.f, r.width - 20.f,
         paused ? "Stream: paused" : "Stream: live", {62, 70, 92, 255});
    using namespace afterhours::ui;
    using namespace afterhours::ui::imm;
    auto b = button(ctx, mk(parent, 3),
                    ComponentConfig{}
                        .with_absolute_position(
                            10.f, std::min(100.f, std::max(84.f, r.height - 44.f)))
                        .with_size({pixels(std::min(130.f, r.width - 20.f)), pixels(36.f)})
                        .with_label(paused ? "Resume" : "Pause")
                        .with_font("FredokaMockBold", pixels(13.f))
                        .with_corner_radius(8.f)
                        .with_debug_name("ops_pause"));
    if (b) paused = !paused;
  }

  void draw_logs(Ctx &ctx, afterhours::Entity &parent, RectangleType r) {
    const int max_lines = std::max(1, int(r.height / 20.f));
    int start = std::max(0, (int)logs.size() - max_lines);
    for (int i = start; i < (int)logs.size(); ++i)
      line(ctx, parent, i, 8.f, 4.f + (i - start) * 20.f, r.width - 16.f,
           logs[static_cast<size_t>(i)], {28, 36, 54, 255}, 12.f);
  }

  void draw_runbook(Ctx &ctx, afterhours::Entity &parent, RectangleType r) {
    using namespace afterhours::ui;
    using namespace afterhours::ui::imm;
    const char *steps[4] = {"1. Check error budget", "2. Roll canary to 5%",
                            "3. Watch p99 for 10 min", "4. Promote to 100%"};
    const float row_h = std::clamp((r.height - 32.f) / 4.f, 26.f, 38.f);
    for (int i = 0; i < 4; ++i) {
      auto b = button(ctx, mk(parent, i),
                      ComponentConfig{}
                          .with_absolute_position(8.f, 8.f + i * (row_h + 6.f))
                          .with_size({pixels(r.width - 16.f), pixels(row_h)})
                          .with_label(std::string(runbook_done[static_cast<size_t>(i)] ? "[done] " : "[    ] ") + steps[i])
                          .with_font("FredokaMockBold", pixels(13.f))
                          .with_custom_text_color(afterhours::Color{40, 48, 64, 255})
                          .with_custom_background(runbook_done[static_cast<size_t>(i)]
                                                      ? afterhours::Color{214, 245, 190, 255}
                                                      : afterhours::Color{205, 214, 232, 255})
                          .with_corner_radius(8.f)
                          .with_debug_name("ops_runbook_step_" + std::to_string(i)));
      if (b) runbook_done[static_cast<size_t>(i)] = !runbook_done[static_cast<size_t>(i)];
    }
  }

  void for_each_with(afterhours::Entity &entity, Ctx &ctx, float dt) override {
    using namespace afterhours::ui;
    using namespace afterhours::ui::imm;
    if (!built) build();
    if (!paused) {
      t_accum += dt;
      log_timer += dt;
      if (log_timer > 1.6f) {
        log_timer = 0.f;
        ++log_seq;
        logs.push_back("svc pulse-api req #" + std::to_string(1000 + log_seq * 37) + " 200 ok");
        while (logs.size() > 40) logs.pop_front();
      }
    }
    ctx.theme = Theme{};
    ctx.theme.background = {238, 240, 244, 255};
    ctx.theme.font = {45, 52, 72, 255};
    const float s =
        std::min(ctx.screen_width / 1280.f, ctx.screen_height / 720.f);
    auto page = div(ctx, mk(entity),
                    ComponentConfig{}
                        .with_size({pixels(1280.f * s), pixels(720.f * s)})
                        .with_absolute_position(
                            (ctx.screen_width - 1280.f * s) / 2.f,
                            (ctx.screen_height - 720.f * s) / 2.f)
                        .with_custom_background(afterhours::Color{238, 240, 244, 255})
                        .with_debug_name("ops_page"));
    div(ctx, mk(page.ent(), 900),
        ComponentConfig{}
            .with_absolute_position(16.f * s, 10.f * s)
            .with_size({pixels(330.f * s), pixels(34.f * s)})
            .with_label("Ops — Trellis dashboard")
            .with_font("FredokaMockBold", pixels(22.f * s))
            .with_custom_text_color(afterhours::Color{45, 52, 72, 255})
            .with_transparent_bg()
            .with_ignore_pointer_events());
    // Toolbar clamps itself left of the global launcher (see DockWorkspace).
    ws.toolbar(ctx, page.ent(), 760.f * s, 10.f * s,
               [this] { reset_all(); }, page.cmp().rect().x, ctx.screen_width,
               s);
    // Bottom 48px of the page is reserved for the global HUD.
    ws.render(ctx, page.ent(),
              {page.cmp().rect().x + 12.f * s, page.cmp().rect().y + 52.f * s,
               1256.f * s, 620.f * s},
              dt);
  }
};

REGISTER_EXAMPLE_SCREEN(trellis_ops, "Tools",
                        "Trellis Ops: live charts, logs and runbook workspace",
                        TrellisOps)
