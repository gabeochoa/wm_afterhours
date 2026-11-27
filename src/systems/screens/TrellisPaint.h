#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include "../../plugins/dock_workspace/DockWorkspace.h"
#include <afterhours/ah.h>

#include <array>
#include <string>
#include <vector>

// Trellis Paint: documents on the stage, brush/color/layers/navigator
// palettes docked around them. Drag any palette by its header to redock,
// double-click a header to zoom to it, Esc steps back out.
struct PaintApp : ScreenSystem<afterhours::ui::UIContext<InputAction>> {
  using Ctx = afterhours::ui::UIContext<InputAction>;
  static constexpr int GW = 48, GH = 30;

  struct Layer {
    std::string name;
    bool visible = true;
    std::vector<afterhours::Color> cells;
  };

  dock::Workspace<Ctx> ws;
  bool built = false;
  std::array<Layer, 3> layers;
  int active_layer = 0;
  int brush_size = 1;
  int tool = 0;  // 0 brush, 1 eraser, 2 fill
  afterhours::Color brush{214, 24, 60, 255};
  std::vector<afterhours::Color> palette{
      {214, 24, 60, 255},  {232, 106, 42, 255}, {248, 185, 40, 255},
      {62, 178, 32, 255},  {23, 86, 227, 255},  {79, 26, 165, 255},
      {240, 120, 180, 255}, {45, 52, 72, 255},  {255, 255, 255, 255},
      {150, 90, 40, 255},  {90, 200, 210, 255}, {168, 172, 186, 255}};

  void reset_layout() {
    using dock::Node;
    ws.doc.root = Node::splitv(
        false, {0.2f, 0.58f, 0.22f},
        Node::splitv(true, {0.42f, 0.58f}, Node::leaf("brush"),
                     Node::leaf("colors")),
        Node::leaf("canvas"),
        Node::splitv(true, {0.55f, 0.45f}, Node::leaf("layers"),
                     Node::leaf("navigator")));
    ws.doc.floating.clear();
    ws.frame_all();
  }

  // Every editable value back to its first-run default (seed sketch
  // included). Called by build() and by reset_all(); leaving the screen
  // recreates the system, so build() alone already defaults a re-entry —
  // reset_all makes the same guarantee explicit for the Reset button and
  // for any future in-place reuse.
  void seed_defaults() {
    const char *names[3] = {"Sketch", "Color", "Highlights"};
    for (int i = 0; i < 3; ++i) {
      layers[i].name = names[i];
      layers[i].visible = true;
      layers[i].cells.assign(GW * GH, afterhours::Color{0, 0, 0, 0});
    }
    active_layer = 0;
    brush_size = 1;
    tool = 0;
    brush = {214, 24, 60, 255};
    // seed a little flower sketch on layer 0 so the stage isn't empty
    auto dot = [&](int x, int y, afterhours::Color c) {
      if (x >= 0 && x < GW && y >= 0 && y < GH)
        layers[0].cells[static_cast<size_t>(y * GW + x)] = c;
    };
    for (int a = 0; a < 8; ++a) {
      float t = a * 3.14159f / 4.f;
      int cx = 24 + int(std::cos(t) * 5), cy = 13 + int(std::sin(t) * 4);
      for (int dx = -1; dx <= 1; ++dx)
        for (int dy = -1; dy <= 1; ++dy)
          dot(cx + dx, cy + dy, {214, 24, 60, 255});
    }
    for (int dx = -1; dx <= 1; ++dx)
      for (int dy = -1; dy <= 1; ++dy) dot(24 + dx, 13 + dy, {248, 185, 40, 255});
    for (int y = 17; y < 26; ++y) dot(24, y, {62, 178, 32, 255});
  }

  void reset_all() {
    seed_defaults();
    reset_layout();
    ws.reset_camera_history();
  }

  void build() {
    built = true;
    seed_defaults();

    ws.defs["canvas"] = {"Untitled-1", {214, 24, 60, 255},
                         [this](Ctx &ctx, afterhours::Entity &parent,
                                RectangleType r, float) {
                           draw_canvas(ctx, parent, r, false);
                         },
                         260.f, 180.f};
    ws.defs["brush"] = {"Brush", {23, 86, 227, 255},
                        [this](Ctx &ctx, afterhours::Entity &parent,
                               RectangleType r, float) {
                          draw_brush(ctx, parent, r);
                        },
                        238.f, 248.f};
    ws.defs["colors"] = {"Color", {79, 26, 165, 255},
                         [this](Ctx &ctx, afterhours::Entity &parent,
                                RectangleType r, float) {
                           draw_colors(ctx, parent, r);
                         },
                         238.f, 222.f};
    ws.defs["layers"] = {"Layers", {62, 178, 32, 255},
                         [this](Ctx &ctx, afterhours::Entity &parent,
                                RectangleType r, float) {
                           draw_layers(ctx, parent, r);
                         },
                         232.f, 250.f};
    ws.defs["navigator"] = {"Navigator", {248, 185, 40, 255},
                            [this](Ctx &ctx, afterhours::Entity &parent,
                                   RectangleType r, float) {
                              draw_canvas(ctx, parent, r, true);
                            },
                            180.f, 150.f};
    reset_layout();
  }

  afterhours::Color composite(int x, int y) const {
    for (int i = 2; i >= 0; --i) {
      if (!layers[i].visible) continue;
      auto c = layers[i].cells[static_cast<size_t>(y * GW + x)];
      if (c.a > 0) return c;
    }
    return {255, 255, 255, 255};
  }

  void draw_canvas(Ctx &ctx, afterhours::Entity &parent, RectangleType r,
                   bool mini) {
    using namespace afterhours::ui;
    using namespace afterhours::ui::imm;
    div(ctx, mk(parent, 0),
        ComponentConfig{}
            .with_absolute_position(0.f, 0.f)
            .with_size({pixels(r.width), pixels(r.height)})
            .with_transparent_bg()
            .with_ignore_pointer_events()
            .with_on_draw_fg([this, r](RectangleType) {
              const float cw = r.width / GW, ch = r.height / GH;
              for (int y = 0; y < GH; ++y)
                for (int x = 0; x < GW; ++x) {
                  auto c = composite(x, y);
                  afterhours::draw_rectangle(
                      {r.x + x * cw, r.y + y * ch, cw + 1.f, ch + 1.f}, c);
                }
            })
            .with_debug_name(mini ? "paint_nav_canvas" : "paint_canvas"));
    if (mini) return;
    // painting input (manual, follows ctx.mouse)
    if (ctx.mouse.left_down && ctx.mouse.pos.x >= r.x &&
        ctx.mouse.pos.x < r.x + r.width && ctx.mouse.pos.y >= r.y &&
        ctx.mouse.pos.y < r.y + r.height) {
      const int gx = int((ctx.mouse.pos.x - r.x) / r.width * GW);
      const int gy = int((ctx.mouse.pos.y - r.y) / r.height * GH);
      auto &cells = layers[active_layer].cells;
      if (tool == 2) {
        std::fill(cells.begin(), cells.end(), brush);
      } else {
        const int rad = brush_size - 1;
        for (int dy = -rad; dy <= rad; ++dy)
          for (int dx = -rad; dx <= rad; ++dx) {
            int x = gx + dx, y = gy + dy;
            if (x >= 0 && x < GW && y >= 0 && y < GH)
              cells[static_cast<size_t>(y * GW + x)] =
                  tool == 1 ? afterhours::Color{0, 0, 0, 0} : brush;
          }
      }
    }
  }

  void small_label(Ctx &ctx, afterhours::Entity &parent, int id, float x,
                   float y, const std::string &text, float pt = 13.f) {
    using namespace afterhours::ui;
    using namespace afterhours::ui::imm;
    div(ctx, mk(parent, id),
        ComponentConfig{}
            .with_absolute_position(x, y)
            .with_size({pixels(220.f), pixels(22.f)})
            .with_label(text)
            .with_font("FredokaMockBold", pixels(pt))
            .with_custom_text_color(afterhours::Color{28, 36, 54, 255})
            .with_transparent_bg()
            .with_ignore_pointer_events());
  }

  void draw_brush(Ctx &ctx, afterhours::Entity &parent, RectangleType r) {
    using namespace afterhours::ui;
    using namespace afterhours::ui::imm;
    const char *tools[3] = {"Brush", "Eraser", "Fill"};
    const float tool_w = std::clamp((r.width - 36.f) / 3.f, 44.f, 68.f);
    for (int i = 0; i < 3; ++i) {
      auto b = button(ctx, mk(parent, i),
                      ComponentConfig{}
                          .with_absolute_position(10.f + i * (tool_w + 8.f), 10.f)
                          .with_size({pixels(tool_w), pixels(34.f)})
                          .with_label(tools[i])
                          .with_font("FredokaMockBold", pixels(13.f))
                          .with_custom_text_color(tool == i
                                                      ? afterhours::Color{255, 255, 255, 255}
                                                      : afterhours::Color{28, 36, 54, 255})
                          .with_custom_background(tool == i
                                                      ? afterhours::Color{23, 86, 227, 255}
                                                      : afterhours::Color{205, 214, 232, 255})
                          .with_corner_radius(8.f)
                          .with_debug_name("paint_tool"));
      if (b) tool = i;
    }
    small_label(ctx, parent, 5, 10.f, 58.f, "Brush size");
    auto minus = button(ctx, mk(parent, 6),
                        ComponentConfig{}
                            .with_absolute_position(10.f, 86.f)
                            .with_size({pixels(40.f), pixels(36.f)})
                            .with_label("-")
                            .with_font("FredokaMockBold", pixels(18.f))
                            .with_corner_radius(8.f)
                            .with_debug_name("paint_brush_minus"));
    if (minus && brush_size > 1) --brush_size;
    small_label(ctx, parent, 7, 10.f, 140.f, "Size: " + std::to_string(brush_size), 20.f);
    auto plus = button(ctx, mk(parent, 8),
                       ComponentConfig{}
                           .with_absolute_position(96.f, 86.f)
                           .with_size({pixels(40.f), pixels(36.f)})
                           .with_label("+")
                           .with_font("FredokaMockBold", pixels(18.f))
                           .with_corner_radius(8.f)
                           .with_debug_name("paint_brush_plus"));
    if (plus && brush_size < 5) ++brush_size;
    small_label(ctx, parent, 9, 10.f, 164.f, "Drag headers to dock.");
    small_label(ctx, parent, 10, 10.f, 186.f, "Double-click zooms in.");
  }

  static bool same_color(afterhours::Color a, afterhours::Color b) {
    return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
  }

  void draw_colors(Ctx &ctx, afterhours::Entity &parent, RectangleType r) {
    using namespace afterhours::ui;
    using namespace afterhours::ui::imm;
    const float cell = std::clamp((r.width - 44.f) / 4.f, 28.f, 48.f);
    for (int i = 0; i < (int)palette.size(); ++i) {
      const float x = 10.f + (i % 4) * (cell + 8.f);
      const float y = 10.f + (i / 4) * (cell + 8.f);
      auto sw = button(ctx, mk(parent, i),
                       ComponentConfig{}
                           .with_absolute_position(x, y)
                           .with_size({pixels(cell), pixels(cell)})
                           .with_custom_background(palette[static_cast<size_t>(i)])
                           .with_border(same_color(palette[static_cast<size_t>(i)], brush)
                                            ? afterhours::Color{30, 38, 52, 255}
                                            : afterhours::Color{105, 118, 145, 255},
                                        same_color(palette[static_cast<size_t>(i)], brush) ? 3.f : 1.f)
                           .with_corner_radius(10.f)
                           .with_debug_name("paint_swatch"));
      if (sw) {
        brush = palette[static_cast<size_t>(i)];
        if (tool == 1) tool = 0;
      }
    }
  }

  void draw_layers(Ctx &ctx, afterhours::Entity &parent, RectangleType r) {
    using namespace afterhours::ui;
    using namespace afterhours::ui::imm;
    const float row_w = std::clamp(r.width - 80.f, 90.f, 150.f);
    const float eye_x = 18.f + row_w;
    for (int i = 2; i >= 0; --i) {
      const int row = 2 - i;
      const float y = 10.f + row * 52.f;
      auto rowb = button(
          ctx, mk(parent, i * 2),
          ComponentConfig{}
              .with_absolute_position(10.f, y)
              .with_size({pixels(row_w), pixels(44.f)})
              .with_label(layers[i].name)
              .with_font("FredokaMockBold", pixels(14.f))
              .with_custom_text_color(afterhours::Color{40, 48, 64, 255})
              .with_custom_background(active_layer == i
                                          ? afterhours::Color{214, 232, 255, 255}
                                          : afterhours::Color{205, 214, 232, 255})
              .with_corner_radius(8.f)
              .with_debug_name("paint_layer"));
      if (rowb) active_layer = i;
      auto eye = button(ctx, mk(parent, i * 2 + 1),
                        ComponentConfig{}
                            .with_absolute_position(eye_x, y)
                            .with_size({pixels(52.f), pixels(44.f)})
                            .with_label(layers[i].visible ? "On" : "Off")
                            .with_font("FredokaMockBold", pixels(13.f))
                            .with_corner_radius(8.f)
                            .with_debug_name("paint_layer_eye"));
      if (eye) layers[i].visible = !layers[i].visible;
    }
    auto clear = button(ctx, mk(parent, 9),
                        ComponentConfig{}
                            .with_absolute_position(10.f, 170.f)
                            .with_size({pixels(std::clamp(r.width - 20.f, 100.f, 210.f)), pixels(36.f)})
                            .with_label("Clear layer")
                            .with_font("FredokaMockBold", pixels(13.f))
                            .with_corner_radius(8.f)
                            .with_debug_name("paint_clear"));
    if (clear)
      std::fill(layers[active_layer].cells.begin(),
                layers[active_layer].cells.end(),
                afterhours::Color{0, 0, 0, 0});
  }

  void for_each_with(afterhours::Entity &entity, Ctx &ctx, float dt) override {
    using namespace afterhours::ui;
    using namespace afterhours::ui::imm;
    if (!built) build();
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
                        .with_debug_name("paint_page"));
    div(ctx, mk(page.ent(), 900),
        ComponentConfig{}
            .with_absolute_position(16.f * s, 10.f * s)
            .with_size({pixels(330.f * s), pixels(34.f * s)})
            .with_label("Paint — Trellis workspace")
            .with_font("FredokaMockBold", pixels(22.f * s))
            .with_custom_text_color(afterhours::Color{45, 52, 72, 255})
            .with_transparent_bg()
            .with_ignore_pointer_events());
    // Toolbar clamps itself left of the global launcher (see DockWorkspace).
    ws.toolbar(ctx, page.ent(), 760.f * s, 10.f * s,
               [this] { reset_all(); }, page.cmp().rect().x, ctx.screen_width,
               s);
    // Bottom 48px of the page is reserved for the global HUD
    // (bottom-left name + bottom-right index) so panels never hide under it.
    ws.render(ctx, page.ent(),
              {page.cmp().rect().x + 12.f * s, page.cmp().rect().y + 52.f * s,
               1256.f * s, 620.f * s},
              dt);
  }
};

REGISTER_EXAMPLE_SCREEN(trellis_paint, "Game Mockups",
                        "Trellis Paint: dockable palettes around a canvas stage",
                        PaintApp)
