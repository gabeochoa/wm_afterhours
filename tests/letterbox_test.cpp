// Regression + equivalence test: compute_letterbox_layout must never divide
// by zero, and its size/offset math is compared against the upstream
// pure helper afterhours::graphics::letterbox_content_size +
// window_centre_position (vendor/afterhours/src/window_helpers.h, header-only,
// no global state). window_manager::content_viewport is NOT called here: it
// letterboxes against the ProvidesCurrentResolution singleton, not against
// caller-supplied render-texture dimensions, so a direct comparison is
// stateful/meaningless for this helper (see docs/LETTERBOX_AUDIT.md).
#include "../src/external.h"
#include "../src/systems/LetterboxLayout.h"
#include <afterhours/src/window_helpers.h>
#include <cassert>
#include <cmath>
#include <cstdio>

static void assert_empty(const LetterboxLayout &layout) {
  assert(layout.bar_left == 0 && layout.bar_right == 0 &&
         layout.bar_top == 0 && layout.bar_bottom == 0);
  assert(layout.dst.x == 0.f && layout.dst.y == 0.f && layout.dst.width == 0.f &&
         layout.dst.height == 0.f);
  assert(std::isfinite(layout.dst.x) && std::isfinite(layout.dst.y) &&
         std::isfinite(layout.dst.width) && std::isfinite(layout.dst.height));
}

// Valid inputs only (all dims > 0): WM dst size == AH size, WM dst offset ==
// AH centre, and WM bars are exactly the window minus dst split floor/ceil.
static void check_equivalent(const char *label, int win_w, int win_h, int cw,
                             int ch) {
  const auto wm = compute_letterbox_layout(win_w, win_h, cw, ch);
  const auto ah_size =
      afterhours::graphics::letterbox_content_size(cw, ch, win_w, win_h);
  const auto ah_pos = afterhours::graphics::window_centre_position(
      0, 0, win_w, win_h, ah_size.first, ah_size.second);
  printf("%-28s win %4dx%-4d content %4dx%-4d | WM dst %dx%d @ %d,%d bars "
         "L%d R%d T%d B%d | AH size %dx%d centre %d,%d | %s\n",
         label, win_w, win_h, cw, ch, (int)wm.dst.width, (int)wm.dst.height,
         (int)wm.dst.x, (int)wm.dst.y, wm.bar_left, wm.bar_right, wm.bar_top,
         wm.bar_bottom, ah_size.first, ah_size.second, ah_pos.first,
         ah_pos.second,
         ((int)wm.dst.width == ah_size.first &&
          (int)wm.dst.height == ah_size.second && (int)wm.dst.x == ah_pos.first &&
          (int)wm.dst.y == ah_pos.second)
             ? "EQUIVALENT"
             : "DIVERGENT");
  assert((int)wm.dst.width == ah_size.first);
  assert((int)wm.dst.height == ah_size.second);
  assert((int)wm.dst.x == ah_pos.first);
  assert((int)wm.dst.y == ah_pos.second);
  // Bars partition the leftover exactly (odd totals: left/top get floor).
  assert(wm.bar_left + wm.bar_right + (int)wm.dst.width == win_w);
  assert(wm.bar_top + wm.bar_bottom + (int)wm.dst.height == win_h);
  assert(wm.bar_left == (win_w - (int)wm.dst.width) / 2);
  assert(wm.bar_top == (win_h - (int)wm.dst.height) / 2);
  assert(wm.dst.x == (float)wm.bar_left && wm.dst.y == (float)wm.bar_top);
  assert(std::isfinite(wm.dst.x) && std::isfinite(wm.dst.y) &&
         std::isfinite(wm.dst.width) && std::isfinite(wm.dst.height));
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

  // --- Equivalence/divergence table vs AH pure helper (valid inputs) ---
  printf("== WM vs AH letterbox_content_size + window_centre_position ==\n");
  check_equivalent("same-aspect", 1920, 1080, 1280, 720);
  check_equivalent("pillarbox", 1920, 1080, 720, 720);
  check_equivalent("letterbox", 800, 600, 1920, 1080);
  // Odd-size rounding: fractional scaled dims, odd bar totals (floor split).
  check_equivalent("odd-round-1000x1000/3x2", 1000, 1000, 3, 2);
  check_equivalent("odd-round-999x777/16x9", 999, 777, 16, 9);
  check_equivalent("odd-round-1920x1080/1000x999", 1920, 1080, 1000, 999);
  check_equivalent("odd-round-1x1/3x2", 1, 1, 3, 2);
  check_equivalent("odd-round-5x5/3x2", 5, 5, 3, 2);
  check_equivalent("odd-round-7x3/16x9", 7, 3, 16, 9);
  check_equivalent("odd-round-1921x1081/1280x720", 1921, 1081, 1280, 720);

  // --- Pinned divergences (non-positive inputs): WM empty vs AH identity ---
  // AH letterbox_content_size returns {window_w, window_h} when any dim <= 0.
  printf("== Pinned divergences (non-positive) ==\n");
  {
    // Content 0x0, valid window: WM empty, AH returns the full window size.
    const auto wm = compute_letterbox_layout(1920, 1080, 0, 0);
    const auto ah =
        afterhours::graphics::letterbox_content_size(0, 0, 1920, 1080);
    printf("content 0x0 win 1920x1080 | WM dst 0x0 | AH size %dx%d | DIVERGENT\n",
           ah.first, ah.second);
    assert_empty(wm);
    assert((ah == std::pair{1920, 1080}));
  }
  {
    const auto wm = compute_letterbox_layout(1920, 1080, 1280, 0);
    const auto ah =
        afterhours::graphics::letterbox_content_size(1280, 0, 1920, 1080);
    printf("content 1280x0 win 1920x1080 | WM dst 0x0 | AH size %dx%d | DIVERGENT\n",
           ah.first, ah.second);
    assert_empty(wm);
    assert((ah == std::pair{1920, 1080}));
  }
  {
    // Window 0x0, valid content: WM empty; AH size is 0x0 (same size, but AH
    // has no bar/empty-layout contract -- its caller must interpret 0x0).
    const auto wm = compute_letterbox_layout(0, 0, 1280, 720);
    const auto ah =
        afterhours::graphics::letterbox_content_size(1280, 720, 0, 0);
    printf("content 1280x720 win 0x0 | WM dst 0x0 empty | AH size %dx%d | SIZE-COINCIDENT but contract differs\n",
           ah.first, ah.second);
    assert_empty(wm);
    assert((ah == std::pair{0, 0}));
  }
  {
    // Negative window/content: AH passes the negative through verbatim.
    const auto wm = compute_letterbox_layout(-1, 1080, 1280, 720);
    const auto ah =
        afterhours::graphics::letterbox_content_size(1280, 720, -1, 1080);
    printf("content 1280x720 win -1x1080 | WM dst 0x0 empty | AH size %dx%d | DIVERGENT\n",
           ah.first, ah.second);
    assert_empty(wm);
    assert((ah == std::pair{-1, 1080}));
  }
  {
    const auto wm = compute_letterbox_layout(1920, 1080, -1, 720);
    const auto ah =
        afterhours::graphics::letterbox_content_size(-1, 720, 1920, 1080);
    printf("content -1x720 win 1920x1080 | WM dst 0x0 empty | AH size %dx%d | DIVERGENT\n",
           ah.first, ah.second);
    assert_empty(wm);
    assert((ah == std::pair{1920, 1080}));
  }

  printf("all letterbox checks passed\n");
  return 0;
}
