// Regression test: compute_letterbox_layout must never divide by zero.
// Minimized (0x0) windows previously produced inf/NaN rects (static_cast of
// inf is UB). Degenerate inputs now return an empty layout. Valid inputs are
// behavior-preserving.
#include "../src/external.h"
#include "../src/systems/LetterboxLayout.h"
#include <cassert>
#include <cmath>

static void assert_empty(const LetterboxLayout &layout) {
  assert(layout.bar_left == 0 && layout.bar_right == 0 &&
         layout.bar_top == 0 && layout.bar_bottom == 0);
  assert(layout.dst.x == 0.f && layout.dst.y == 0.f && layout.dst.width == 0.f &&
         layout.dst.height == 0.f);
  assert(std::isfinite(layout.dst.x) && std::isfinite(layout.dst.y) &&
         std::isfinite(layout.dst.width) && std::isfinite(layout.dst.height));
}

int main() {
  // Valid: same aspect -> no bars, dst covers the window.
  {
    const auto l = compute_letterbox_layout(1920, 1080, 1280, 720);
    assert(l.bar_left == 0 && l.bar_right == 0 && l.bar_top == 0 &&
           l.bar_bottom == 0);
    assert(l.dst.x == 0.f && l.dst.y == 0.f && l.dst.width == 1920.f &&
           l.dst.height == 1080.f);
  }
  // Valid pillarbox: square content in a wide window -> side bars.
  {
    const auto l = compute_letterbox_layout(1920, 1080, 720, 720);
    assert(l.dst.width == 1080.f && l.dst.height == 1080.f);
    assert(l.bar_left == 420 && l.bar_right == 420 && l.bar_top == 0 &&
           l.bar_bottom == 0);
    assert(l.dst.x == 420.f && l.dst.y == 0.f);
  }
  // Valid letterbox: wide content in a tall-ish window -> top/bottom bars.
  {
    const auto l = compute_letterbox_layout(800, 600, 1920, 1080);
    assert(l.dst.width == 800.f && l.dst.height == 450.f);
    assert(l.bar_left == 0 && l.bar_right == 0 && l.bar_top == 75 &&
           l.bar_bottom == 75);
    assert(l.dst.x == 0.f && l.dst.y == 75.f);
  }
  // Degenerate: zero/negative in any dimension -> empty layout, no UB.
  assert_empty(compute_letterbox_layout(0, 0, 1280, 720));
  assert_empty(compute_letterbox_layout(1920, 1080, 0, 0));
  assert_empty(compute_letterbox_layout(0, 1080, 1280, 720));
  assert_empty(compute_letterbox_layout(1920, 0, 1280, 720));
  assert_empty(compute_letterbox_layout(1920, 1080, 0, 720));
  assert_empty(compute_letterbox_layout(1920, 1080, 1280, 0));
  assert_empty(compute_letterbox_layout(-1, 1080, 1280, 720));
  assert_empty(compute_letterbox_layout(1920, 1080, -1, 720));
  assert_empty(compute_letterbox_layout(1920, 1080, 1280, -1));
  assert_empty(compute_letterbox_layout(-1, -1, -1, -1));

  return 0;
}
