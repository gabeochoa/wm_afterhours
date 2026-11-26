// Regression tests for the Trellis DockWorkspace safe-area + reset helpers.
// Covers owner feedback: toolbar controls hidden under the global
// "Browse screens" launcher, and reset semantics for camera/history.
// Build: clang++ -std=c++23 -DAFTER_HOURS_USE_RAYLIB -DAFTER_HOURS_ENABLE_E2E_TESTING -I vendor -Isrc -I$HOME/homebrew/include tests/dock_workspace_test.cpp -o /tmp/dock_workspace_test -L$HOME/homebrew/lib -lraylib -framework CoreFoundation -framework OpenGL -framework AppKit -framework IOKit && /tmp/dock_workspace_test
#include "../src/external.h"
#include "../src/input_mapping.h"
#include "../src/plugins/gallery_shell/ExampleScreenRegistry.h"
#include "../src/systems/screens/trellis/DockWorkspace.h"
#include <cassert>
#include <cmath>

using dock::global_launcher_rect;
using dock::rects_intersect;
using dock::serialize_doc;
using dock::toolbar_x_ending_before_launcher;

int main() {
  // Launcher geometry matches ScreenNavigator (src/plugins/gallery_shell):
  // 200x32 at (screen_width - 212, 8).
  const auto launcher = global_launcher_rect(1280.f);
  assert(launcher.x == 1068.f && launcher.y == 8.f && launcher.width == 200.f &&
         launcher.height == 32.f);

  // Old toolbar placement (x=760, five 96px buttons => right edge 1272)
  // intersected the launcher; the safe placement must not, at any width.
  for (float w : {854.f, 1280.f, 1920.f}) {
    const float page_x = 0.f;  // 1280x720 page fills the screen at s=1
    const float tx = toolbar_x_ending_before_launcher(w, page_x, 452.f);
    const dock::RectangleType toolbar_rect{page_x + tx, 10.f, 452.f, 30.f};
    assert(!rects_intersect(toolbar_rect, global_launcher_rect(w)));
    assert(toolbar_rect.x + toolbar_rect.width <=
           global_launcher_rect(w).x - 12.f);
    // And the helper must actually move an unsafe desired x leftwards.
    assert(tx == std::max(global_launcher_rect(w).x - 12.f - 452.f, 0.f));
  }

  // rects_intersect sanity: touching edges do not intersect.
  assert(!rects_intersect({0, 0, 10, 10}, {10, 0, 10, 10}));
  assert(rects_intersect({0, 0, 11, 10}, {10, 0, 10, 10}));
  assert(!rects_intersect({0, 0, 10, 10}, {0, 20, 10, 10}));

  // Header controls reserve: tab strip must stop before the F/x buttons.
  assert(dock::kHeaderControlsReserve == 80.f);

  // reset_camera_history clears camera, history, focus and drag state.
  {
    // Workspace is templated on Ctx; use a dummy context type. The reset
    // helper itself never touches the context.
    struct Dummy {};
    dock::Workspace<Dummy> ws;
    ws.view = {100, 100, 640, 360};
    ws.view_target = {100, 100, 640, 360};
    ws.history.push_back({100, 100, 640, 360});
    ws.hist_at = 0;
    ws.focused = "brush";
    ws.dragging = true;
    ws.drag_panel = "brush";
    ws.drop_edge = 4;
    ws.root_rect = {0, 0, 1256, 620};
    ws.reset_camera_history();
    assert(ws.view.x == 0.f && ws.view.width == 1256.f);
    assert(ws.view_target.width == 1256.f);
    assert(ws.history.empty() && ws.hist_at == -1);
    assert(ws.focused.empty() && !ws.dragging && ws.drop_edge == -1);
    assert(ws.drag_panel.empty());
  }

  // Layout primitives the reset path relies on still behave.
  {
    dock::Doc doc;
    using dock::Node;
    doc.root = Node::splitv(false, {0.5f, 0.5f}, Node::leaf("a"),
                            Node::leaf("b"));
    assert(serialize_doc(doc).find("H(") == 0);
    assert(doc.root->contains("a") && doc.root->contains("b"));
    assert(dock::remove_panel_from(doc.root, "a"));
    assert(doc.root->kind == Node::Kind::Leaf);
    assert(serialize_doc(doc) == "L(b)");
  }

  // Drop labels are readable (owner: drop preview wasn't obvious).
  {
    struct Dummy {};
    dock::Workspace<Dummy> ws;
    ws.drop_edge = 1;
    ws.drop_target = "canvas";
    assert(ws.drop_label() == "Split right onto canvas");
    ws.drop_edge = 4;
    assert(ws.drop_label() == "Tab onto canvas");
    ws.drop_edge = 5;
    ws.drop_target.clear();
    assert(ws.drop_label() == "Float");
    ws.drop_edge = 6;
    assert(ws.drop_label() == "Dock left");
  }
  return 0;
}
