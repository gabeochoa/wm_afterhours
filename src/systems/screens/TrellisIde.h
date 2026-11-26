#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include "trellis/DockWorkspace.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/ui/form.h>

#include <map>
#include <string>
#include <vector>

// Trellis IDE ("Pulse"): explorer, editor tabs, terminal, problems and
// outline, plus a live preview that keeps running when you move it —
// panel state is keyed by panel id, never by tree position.
struct TrellisIde : ScreenSystem<afterhours::ui::UIContext<InputAction>> {
  using Ctx = afterhours::ui::UIContext<InputAction>;
  dock::Workspace<Ctx> ws;
  bool built = false;
  float t_accum = 0.f;
  int preview_ticks = 0;

  std::map<std::string, std::vector<std::string>> files;
  std::string current_file = "main.cpp";
  std::vector<std::string> term_log{"pulse shell — try: help, build, clear"};
  std::string term_input;

  void reset_layout() {
    using dock::Node;
    ws.doc.root = Node::splitv(
        false, {0.17f, 0.60f, 0.23f}, Node::leaf("explorer"),
        Node::splitv(true, {0.68f, 0.32f},
                     Node::tabs({"file_main", "file_ui", "file_readme"}, 0),
                     Node::splitv(false, {0.55f, 0.45f},
                                  Node::leaf("terminal"),
                                  Node::leaf("problems"))),
        Node::splitv(true, {0.55f, 0.45f}, Node::leaf("preview"),
                     Node::leaf("outline")));
    ws.doc.floating.clear();
    ws.frame_all();
  }

  void seed_defaults() {
    t_accum = 0.f;
    preview_ticks = 0;
    current_file = "main.cpp";
    term_log = {"pulse shell — try: help, build, clear"};
    term_input.clear();
  }

  void reset_all() {
    seed_defaults();
    reset_layout();
    ws.reset_camera_history();
  }

  void build() {
    built = true;
    seed_defaults();
    files["main.cpp"] = {
        "#include \"pulse.h\"", "", "int main() {", "  App app;",
        "  app.load_workspace(\"default\");", "  // TODO: restore camera",
        "  return app.run();", "}"};
    files["ui.h"] = {"#pragma once", "", "struct Panel {",
                     "  Rect rect;", "  bool focused;", "};", "",
                     "void draw_panel(Panel &p);"};
    files["README.md"] = {"# Pulse", "", "A tiny editor built on a",
                          "dockable workspace. Drag panels", "by their headers.",
                          "", "## TODO", "- ship it"};

    auto editor = [this](const char *fname) {
      return [this, fname](Ctx &ctx, afterhours::Entity &parent,
                           RectangleType r, float) {
        draw_editor(ctx, parent, r, fname);
      };
    };
    ws.defs["explorer"] = {"Explorer", {90, 160, 255, 255},
                           [this](Ctx &c, afterhours::Entity &p, RectangleType r, float) { draw_explorer(c, p, r); },
                           170.f, 170.f};
    ws.defs["file_main"] = {"main.cpp", {120, 200, 120, 255}, editor("main.cpp"), 260.f, 170.f};
    ws.defs["file_ui"] = {"ui.h", {120, 200, 120, 255}, editor("ui.h"), 260.f, 170.f};
    ws.defs["file_readme"] = {"README.md", {120, 200, 120, 255}, editor("README.md"), 260.f, 170.f};
    ws.defs["terminal"] = {"Terminal", {45, 52, 72, 255},
                           [this](Ctx &c, afterhours::Entity &p, RectangleType r, float) { draw_terminal(c, p, r); },
                           280.f, 150.f};
    ws.defs["problems"] = {"Problems", {214, 24, 60, 255},
                           [this](Ctx &c, afterhours::Entity &p, RectangleType r, float) { draw_problems(c, p, r); },
                           220.f, 120.f};
    ws.defs["outline"] = {"Outline", {248, 185, 40, 255},
                          [this](Ctx &c, afterhours::Entity &p, RectangleType r, float) { draw_outline(c, p, r); },
                          200.f, 210.f};
    ws.defs["preview"] = {"Live Preview", {23, 86, 227, 255},
                          [this](Ctx &c, afterhours::Entity &p, RectangleType r, float) { draw_preview(c, p, r); },
                          200.f, 180.f};
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
            .with_size({pixels(w), pixels(20.f)})
            .with_label(text)
            .with_font("AtkinsonMock", pixels(pt))
            .with_custom_text_color(col)
            .with_transparent_bg()
            .with_ignore_pointer_events());
  }

  void draw_explorer(Ctx &ctx, afterhours::Entity &parent, RectangleType r) {
    using namespace afterhours::ui;
    using namespace afterhours::ui::imm;
    line(ctx, parent, 0, 8.f, 4.f, r.width - 16.f, "PULSE", {28, 36, 54, 255});
    int i = 0;
    for (auto &[name, _] : files) {
      auto b = button(ctx, mk(parent, 10 + i),
                      ComponentConfig{}
                          .with_absolute_position(8.f, 34.f + i * 40.f)
                          .with_size({pixels(r.width - 16.f), pixels(34.f)})
                          .with_label(name)
                          .with_font("FredokaMockBold", pixels(13.f))
                          .with_custom_text_color(afterhours::Color{40, 48, 64, 255})
                          .with_custom_background(name == current_file
                                                      ? afterhours::Color{214, 232, 255, 255}
                                                      : afterhours::Color{205, 214, 232, 255})
                          .with_corner_radius(7.f)
                          .with_debug_name("ide_file"));
      if (b) {
        current_file = name;
        // activate the corresponding editor tab
        const std::string pid =
            name == "main.cpp" ? "file_main" : name == "ui.h" ? "file_ui" : "file_readme";
        if (dock::Node *h = dock::find_host(ws.doc.root.get(), pid)) {
          for (int ti = 0; ti < (int)h->panels.size(); ++ti)
            if (h->panels[ti] == pid) h->active = ti;
        }
        ws.focused = pid;
      }
      ++i;
    }
  }

  void draw_editor(Ctx &ctx, afterhours::Entity &parent, RectangleType r,
                   const char *fname) {
    auto it = files.find(fname);
    if (it == files.end()) return;
    int i = 0;
    for (auto &ln : it->second) {
      line(ctx, parent, i, 34.f, 8.f + i * 21.f, r.width - 42.f, ln,
           {28, 36, 54, 255}, 13.5f);
      line(ctx, parent, 100 + i, 4.f, 8.f + i * 21.f, 26.f,
           std::to_string(i + 1), {105, 113, 135, 255}, 11.f);
      ++i;
      if (8.f + i * 21.f > r.height - 24.f) break;
    }
  }

  void run_command(const std::string &cmd) {
    term_log.push_back("$ " + cmd);
    if (cmd == "help") {
      term_log.push_back("commands: help, build, clear, ls");
    } else if (cmd == "build") {
      term_log.push_back("building pulse... ok (0 errors, 2 TODOs)");
    } else if (cmd == "clear") {
      term_log.clear();
    } else if (cmd == "ls") {
      term_log.push_back("main.cpp  ui.h  README.md");
    } else if (!cmd.empty()) {
      term_log.push_back("unknown command: " + cmd);
    }
    while (term_log.size() > 30) term_log.erase(term_log.begin());
  }

  void draw_terminal(Ctx &ctx, afterhours::Entity &parent, RectangleType r) {
    using namespace afterhours::ui;
    using namespace afterhours::ui::imm;
    const int max_lines = std::max(1, int((r.height - 52.f) / 19.f));
    int start = std::max(0, (int)term_log.size() - max_lines);
    for (int i = start; i < (int)term_log.size(); ++i)
      line(ctx, parent, i, 8.f, 4.f + (i - start) * 19.f, r.width - 16.f,
           term_log[static_cast<size_t>(i)], {28, 36, 54, 255}, 12.5f);
    afterhours::ui::form::text(
        ctx, mk(parent, 90), "", term_input,
        ComponentConfig{}
            .with_absolute_position(8.f, r.height - 42.f)
            .with_size({pixels(std::max(r.width - 96.f, 60.f)), pixels(34.f)})
            .with_debug_name("ide_term_input"));
    auto run = button(ctx, mk(parent, 91),
                      ComponentConfig{}
                          .with_absolute_position(r.width - 80.f, r.height - 42.f)
                          .with_size({pixels(72.f), pixels(34.f)})
                          .with_label("Run")
                          .with_font("FredokaMockBold", pixels(13.f))
                          .with_corner_radius(8.f)
                          .with_debug_name("ide_term_run"));
    if (run && !term_input.empty()) {
      run_command(term_input);
      term_input.clear();
    }
  }

  void draw_problems(Ctx &ctx, afterhours::Entity &parent, RectangleType r) {
    int i = 0;
    for (auto &[name, lines] : files)
      for (auto &ln : lines)
        if (ln.find("TODO") != std::string::npos) {
          line(ctx, parent, i, 8.f, 6.f + i * 22.f, r.width - 16.f,
               name + ": " + ln, {180, 90, 30, 255}, 12.5f);
          ++i;
        }
    if (i == 0)
      line(ctx, parent, 0, 8.f, 6.f, r.width - 16.f, "No problems. Nice.",
           {62, 140, 60, 255}, 12.5f);
  }

  void draw_outline(Ctx &ctx, afterhours::Entity &parent, RectangleType r) {
    using namespace afterhours::ui;
    using namespace afterhours::ui::imm;
    const char *syms[4] = {"main()", "Panel", "draw_panel()", "# Pulse"};
    const char *targets[4] = {"file_main", "file_ui", "file_ui", "file_readme"};
    const float row_h = std::clamp((r.height - 32.f) / 4.f, 26.f, 34.f);
    for (int i = 0; i < 4; ++i) {
      auto b = button(ctx, mk(parent, i),
                      ComponentConfig{}
                          .with_absolute_position(8.f, 8.f + i * (row_h + 6.f))
                          .with_size({pixels(r.width - 16.f), pixels(row_h)})
                          .with_label(syms[i])
                          .with_font("FredokaMockBold", pixels(13.f))
                          .with_corner_radius(7.f)
                          .with_debug_name("ide_outline_" + std::to_string(i)));
      if (b) {
        const std::string pid = targets[i];
        if (dock::Node *h = dock::find_host(ws.doc.root.get(), pid))
          for (int ti = 0; ti < (int)h->panels.size(); ++ti)
            if (h->panels[ti] == pid) h->active = ti;
        ws.focused = pid;
      }
    }
  }

  void draw_preview(Ctx &ctx, afterhours::Entity &parent, RectangleType r) {
    using namespace afterhours::ui;
    using namespace afterhours::ui::imm;
    line(ctx, parent, 0, 8.f, 4.f, r.width - 16.f,
         "Never reloads — drag it anywhere.", {28, 36, 54, 255});
    const float cx = r.width / 2.f, cy = r.height / 2.f + 6.f;
    const float rad = std::min(r.width, r.height) * 0.28f;
    const float a = t_accum * 1.4f;
    div(ctx, mk(parent, 1),
        ComponentConfig{}
            .with_absolute_position(0.f, 26.f)
            .with_size({pixels(r.width), pixels(r.height - 26.f)})
            .with_transparent_bg()
            .with_ignore_pointer_events()
            .with_on_draw_fg([cx, cy, rad, a](RectangleType rr) {
              const float ox = rr.x + cx, oy = rr.y + cy - 26.f;
              afterhours::draw_circle_v({ox, oy}, rad, {214, 232, 255, 255});
              afterhours::draw_circle_v(
                  {ox + std::cos(a) * rad * 0.62f, oy + std::sin(a) * rad * 0.62f},
                  rad * 0.30f, {23, 86, 227, 255});
              afterhours::draw_circle_v(
                  {ox + std::cos(-a * 1.7f) * rad * 0.85f,
                   oy + std::sin(-a * 1.7f) * rad * 0.85f},
                  rad * 0.16f, {248, 185, 40, 255});
            })
            .with_debug_name("ide_preview_canvas"));
    line(ctx, parent, 2, 8.f, r.height - 26.f, r.width - 16.f,
         "ticks: " + std::to_string(preview_ticks), {28, 36, 54, 255});
  }

  void for_each_with(afterhours::Entity &entity, Ctx &ctx, float dt) override {
    using namespace afterhours::ui;
    using namespace afterhours::ui::imm;
    if (!built) build();
    t_accum += dt;
    ++preview_ticks;
    ctx.theme = Theme{};
    ctx.theme.background = {24, 27, 36, 255};
    ctx.theme.font = {45, 52, 72, 255};
    const float s =
        std::min(ctx.screen_width / 1280.f, ctx.screen_height / 720.f);
    auto page = div(ctx, mk(entity),
                    ComponentConfig{}
                        .with_size({pixels(1280.f * s), pixels(720.f * s)})
                        .with_absolute_position(
                            (ctx.screen_width - 1280.f * s) / 2.f,
                            (ctx.screen_height - 720.f * s) / 2.f)
                        .with_custom_background(afterhours::Color{24, 27, 36, 255})
                        .with_debug_name("ide_page"));
    div(ctx, mk(page.ent(), 900),
        ComponentConfig{}
            .with_absolute_position(16.f * s, 10.f * s)
            .with_size({pixels(330.f * s), pixels(34.f * s)})
            .with_label("Pulse — Trellis IDE")
            .with_font("FredokaMockBold", pixels(22.f * s))
            .with_custom_text_color(afterhours::Color{235, 240, 250, 255})
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

REGISTER_EXAMPLE_SCREEN(trellis_ide, "Tools",
                        "Trellis IDE: explorer, editor tabs, terminal, live preview",
                        TrellisIde)
