# CSS Grid Layout

**Status:** Not implemented  
**Priority:** Low

## Problem

Only Row/Column flex layout is available.

## Suggested Implementation

```cpp
ElementResult grid(HasUIContext auto &ctx, EntityParent ep_pair,
                   int columns, int rows = -1,  // -1 = auto rows
                   ComponentConfig config = ComponentConfig());

ComponentConfig{}
    .with_grid_gap(DefaultSpacing::small())
    .with_grid_columns(3)
    .with_grid_auto_rows(pixels(100));
```

---

## Example Screen: ChessBoardDragDrop (shared with `32_drag_and_drop.md`)

**File:** `src/systems/screens/ChessBoardDragDrop.h`
**CLI:** `--screen=chess_drag_drop`
**Category:** Widgets

The chess board from `32_drag_and_drop.md` doubles as the primary CSS Grid showcase. The 8x8 board is built with `with_grid_columns(8)` and `with_grid_auto_rows()`, making it the most demanding grid layout test case in the project.

### How the Chess Board Exercises CSS Grid

1. **8x8 Fixed Grid** — The board itself is a container with `with_grid_columns(8)` and 64 square children. All cells are equal size, demonstrating even column/row distribution. Each cell uses `with_grid_auto_rows(pixels(square_size))` for uniform row height.

2. **Grid Gap = 0** — Chess boards have no gap between squares, demonstrating that `with_grid_gap(pixels(0))` produces flush edges. The alternating colors prove cells tile perfectly without overlap or spacing artifacts.

3. **Grid-within-Grid (Move Log)** — The move log panel uses a 2-column grid (`with_grid_columns(2)`) for the move number + notation pairs: `| 1. | e4   e5 |`. This demonstrates a different column count alongside the board.

4. **Grid-within-Grid (Captured Pieces)** — The captured piece trays use `with_grid_columns(8)` with `with_grid_auto_rows(children())` so captured pieces wrap to a new row after 8. Demonstrates auto-row sizing based on content.

5. **Fixed vs Auto Sizing** — Board squares use fixed pixel sizes. Captured piece cells use `children()` height. The move log uses `1fr` columns. All three grid sizing modes exercised on one screen.

### Additional Grid-Specific Layout Sections

Below the chess board, the screen includes a collapsible "Grid Demo" section with:

6. **Responsive Grid** — A row of 12 colored swatches using `with_grid_columns(N)` where N changes at breakpoints (6 at 1920px, 4 at 1280px, 3 at 800px). A label shows the current column count.

7. **Uneven Columns** — A 3-column grid with `1fr 2fr 1fr` weights. The center column holds the board controls, flanked by narrower panels. Demonstrates proportional column sizing.

### Features Exercised

- `with_grid_columns(8)` for the 8x8 board
- `with_grid_columns(2)` for the move log
- `with_grid_gap(pixels(0))` for flush tiling
- `with_grid_auto_rows(pixels(N))` for fixed row height
- `with_grid_auto_rows(children())` for content-based row height
- Responsive column count via breakpoints
- Nested grids (board, move log, captured trays all on one screen)

### Verification

- Board: 64 squares in a perfect 8x8 arrangement, no gaps
- All squares are exactly the same pixel size
- Move log: move numbers left-aligned, notation right-aligned in 2-column grid
- Captured pieces wrap to a new row after 8 items
- Responsive swatches: column count changes when window is resized
- Uneven columns: center column is visually ~2x wider than side columns

### E2E Test Plan

**Test file:** `src/testing/tests/ChessDragDropTest.h` (shared with `32_drag_and_drop.md`)

Grid layout is verified as part of the chess board tests. Additional grid-specific assertions:

#### New Custom Commands Needed

- `expect_element_size(label, w, h, tolerance)` — check element dimensions. Needed to verify all 64 board squares have identical pixel size.
- `simulate_window_resize(w, h)` — change window/reference resolution. Needed to test responsive swatch column count.

#### Screenshots

1. `chess_initial` (shared) — verifies the 8x8 grid renders as a perfect board
2. `grid_responsive_6col` — responsive swatches at 1920px
3. `grid_responsive_3col` — same swatches at 800px
4. `grid_captured_wrap` — captured pieces tray wrapping to second row after 8 captures

#### Test Script

```cpp
TEST(grid_board_uniform_squares) {
  co_await TestApp::wait_for_frames(10);

  // Verify two squares have the same size
  auto *a1 = TestApp::find_ui_element_by_label("a1");
  auto *h8 = TestApp::find_ui_element_by_label("h8");
  if (!a1 || !h8) throw std::runtime_error("Board squares not found");

  auto a1_rect = a1->get<afterhours::ui::UIComponent>().rect();
  auto h8_rect = h8->get<afterhours::ui::UIComponent>().rect();

  float tol = 2.0f;
  if (std::abs(a1_rect.width - h8_rect.width) > tol ||
      std::abs(a1_rect.height - h8_rect.height) > tol) {
    throw std::runtime_error("Board squares are not uniform size");
  }

  // Verify square (square, not rectangular)
  if (std::abs(a1_rect.width - a1_rect.height) > tol) {
    throw std::runtime_error("Board square is not square");
  }

  auto snap = TestApp::capture_snapshot("chess_initial");
}

TEST(grid_no_gaps_between_squares) {
  co_await TestApp::wait_for_frames(10);

  // a1 right edge should touch b1 left edge (no gap)
  auto *a1 = TestApp::find_ui_element_by_label("a1");
  auto *b1 = TestApp::find_ui_element_by_label("b1");
  if (!a1 || !b1) throw std::runtime_error("Board squares not found");

  auto a1_rect = a1->get<afterhours::ui::UIComponent>().rect();
  auto b1_rect = b1->get<afterhours::ui::UIComponent>().rect();

  float gap = b1_rect.x - (a1_rect.x + a1_rect.width);
  if (std::abs(gap) > 1.0f) {
    throw std::runtime_error("Gap between a1 and b1: " + std::to_string(gap));
  }
}

TEST(grid_responsive_swatches) {
  co_await TestApp::wait_for_frames(10);

  // At default resolution
  TestApp::expect_ui_exists("Columns:");
  auto snap1 = TestApp::capture_snapshot("grid_responsive_6col");

  // Resize smaller
  simulate_window_resize(800, 600);
  co_await TestApp::wait_for_frames(10);

  auto snap2 = TestApp::capture_snapshot("grid_responsive_3col");
}
```

