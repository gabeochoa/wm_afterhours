# Drag and Drop

**Status:** Not implemented  
**Priority:** Low

## Problem

No way to drag items between containers.

## Suggested Implementation

```cpp
ComponentConfig{}.with_draggable(true);
ComponentConfig{}.with_drop_target(true);
// Plus callbacks for drag start, over, drop
```

---

## Example Screen: ChessBoardDragDrop

**File:** `src/systems/screens/ChessBoardDragDrop.h`
**CLI:** `--screen=chess_drag_drop`
**Category:** Widgets

### Concept

A playable chess board where pieces are moved by dragging them between squares. This exercises every aspect of drag-and-drop — valid/invalid targets, visual feedback, ghost preview, drop acceptance logic — in a universally understood context.

### Layout

1. **Board** — An 8x8 grid of alternating light/dark squares. Each square is a drop target. Squares are labeled internally by algebraic notation (a1–h8) for debug names. Light squares use a warm cream, dark squares use a muted olive/brown.

2. **Pieces** — Unicode chess symbols (♔♕♖♗♘♙ / ♚♛♜♝♞♟) rendered as large text labels on their starting squares. Each piece element has `with_draggable(true)`. White pieces start on ranks 1–2, black pieces on ranks 7–8.

3. **Drag Ghost** — While dragging, a semi-transparent copy of the piece symbol follows the cursor at ~60% opacity. The source square shows a subtle tint to indicate where the piece came from.

4. **Valid Move Highlighting** — When a piece is picked up, squares it can legally move to get a colored dot overlay (green circle in center). Invalid squares remain unchanged. This exercises per-square drop acceptance logic.

5. **Drop Feedback** — Dropping on a valid square moves the piece there. Dropping on an invalid square (or off-board) returns the piece to its origin. Capturing an opponent's piece removes it with a brief fade.

6. **Move Log** — A narrow panel on the right showing the last 10 moves in algebraic notation: "1. e4  e5", "2. Nf3  Nc6", etc. Each entry appends on successful drop.

7. **Captured Pieces** — Two rows above/below the board showing captured pieces for each side.

8. **Status Bar** — Below the board: "White to move" / "Black to move" indicator, plus a "Reset Board" button.

### Features Exercised

- `with_draggable(true)` on piece elements
- `with_drop_target(true)` on all 64 squares
- Drag start callback: compute legal moves, highlight valid squares
- Drag over callback: show/hide acceptance indicator per square
- Drop callback: validate move, update board state, append to move log
- Drop rejection: piece returns to origin square with snap-back
- Ghost element rendering during drag (semi-transparent piece at cursor)
- Conditional drop acceptance (only legal squares accept the drop)
- Visual state changes on drag hover (valid square highlight, capture preview)

### Verification

- Dragging a pawn from e2 shows green dots on e3 and e4 (initial double move)
- Dropping the pawn on e4 moves it there, move log shows "1. e4"
- Dragging the pawn from e4 to e3 (backwards) is rejected — piece snaps back
- Dragging a pawn onto an opponent's piece diagonally captures it (piece appears in captured row)
- Dropping off-board or on an occupied friendly square returns the piece to origin
- "Reset Board" restores all pieces to starting positions and clears the move log
- Turn indicator alternates between "White to move" and "Black to move"

### E2E Test Plan

**Test file:** `src/testing/tests/ChessDragDropTest.h`

#### New Custom Commands Needed

- `drag_from_to(from_label, to_label, hold_frames)` — click element `from_label` center, hold mouse, linearly interpolate mouse position to element `to_label` center over `hold_frames` frames, then release. Implementation:

```cpp
static void drag_from_to(const std::string &from_label,
                         const std::string &to_label,
                         int hold_frames) {
  auto *from_el = find_ui_element_by_label(from_label);
  auto *to_el = find_ui_element_by_label(to_label);
  if (!from_el) throw std::runtime_error("Drag source not found: " + from_label);
  if (!to_el) throw std::runtime_error("Drag target not found: " + to_label);

  auto from_rect = from_el->get<afterhours::ui::UIComponent>().rect();
  auto to_rect = to_el->get<afterhours::ui::UIComponent>().rect();

  vec2 from_center = {from_rect.x + from_rect.width / 2.0f,
                      from_rect.y + from_rect.height / 2.0f};
  vec2 to_center = {to_rect.x + to_rect.width / 2.0f,
                    to_rect.y + to_rect.height / 2.0f};

  // Press at source
  test_input::set_mouse_position(from_center);
  test_input::simulate_mouse_button_press(raylib::MOUSE_BUTTON_LEFT);
}

// Caller must co_await between press, interpolation frames, and release:
//   drag_start("from", "to");
//   for (int i = 0; i < hold_frames; i++) {
//     set_mouse_lerp(from, to, i / (float)hold_frames);
//     co_await wait_for_frames(1);
//   }
//   release_mouse_button();
```

Note: Because the test framework is coroutine-based, `drag_from_to` can't be a single synchronous call. Instead, the test script calls `click_ui_element` to start the drag, uses `co_await` + `set_mouse_position` to interpolate, then `release_mouse_button` to drop. A helper macro or inline lambda can wrap this pattern.

#### Screenshots

1. `chess_initial` — board with all pieces at starting positions
2. `chess_dragging_pawn` — pawn picked up from e2, ghost following cursor, valid squares (e3, e4) highlighted with green dots
3. `chess_pawn_moved` — pawn on e4, move log shows "1. e4", turn indicator shows "Black to move"
4. `chess_capture` — white pawn capturing black pawn diagonally, captured piece in tray
5. `chess_invalid_snapback` — piece returned to origin after invalid drop
6. `chess_mid_game` — several moves in, move log populated, captured pieces visible
7. `chess_reset` — board reset to starting position after clicking "Reset Board"

#### Test Script

```cpp
TEST(chess_initial_layout) {
  co_await TestApp::wait_for_frames(10);

  // Board should show starting position
  TestApp::expect_ui_exists("♙");  // white pawn
  TestApp::expect_ui_exists("♟");  // black pawn
  TestApp::expect_ui_exists("♔");  // white king
  TestApp::expect_ui_exists("White to move");
  TestApp::expect_ui_exists("Reset Board");

  auto snap = TestApp::capture_snapshot("chess_initial");
}

TEST(chess_drag_pawn_e2_to_e4) {
  co_await TestApp::wait_for_frames(10);

  // Click pawn on e2 and start dragging
  auto *pawn = TestApp::find_ui_element_by_label("e2_piece");
  if (!pawn) throw std::runtime_error("e2 pawn not found");
  TestApp::click_ui_element(*pawn);
  co_await TestApp::wait_for_frames(3);

  // Mid-drag: capture screenshot showing ghost + valid move highlights
  auto *target = TestApp::find_ui_element_by_label("e4");
  if (!target) throw std::runtime_error("e4 square not found");
  auto target_rect = target->get<afterhours::ui::UIComponent>().rect();
  vec2 mid = {target_rect.x + target_rect.width / 2.0f,
              target_rect.y - 30.0f};  // slightly above target
  test_input::set_mouse_position(mid);
  co_await TestApp::wait_for_frames(3);

  auto snap_drag = TestApp::capture_snapshot("chess_dragging_pawn");

  // Move to e4 center and release
  test_input::set_mouse_position({target_rect.x + target_rect.width / 2.0f,
                                  target_rect.y + target_rect.height / 2.0f});
  co_await TestApp::wait_for_frames(2);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(5);

  // Pawn should now be on e4
  TestApp::expect_ui_exists("1. e4");
  TestApp::expect_ui_exists("Black to move");
  auto snap = TestApp::capture_snapshot("chess_pawn_moved");
}

TEST(chess_invalid_move_snapback) {
  co_await TestApp::wait_for_frames(10);

  // Try to move pawn backwards (e2 to e1) — invalid
  auto *pawn = TestApp::find_ui_element_by_label("e2_piece");
  if (!pawn) throw std::runtime_error("e2 pawn not found");
  TestApp::click_ui_element(*pawn);
  co_await TestApp::wait_for_frames(3);

  auto *bad_target = TestApp::find_ui_element_by_label("e1");
  if (bad_target) {
    auto rect = bad_target->get<afterhours::ui::UIComponent>().rect();
    test_input::set_mouse_position({rect.x + rect.width / 2.0f,
                                    rect.y + rect.height / 2.0f});
  }
  co_await TestApp::wait_for_frames(2);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(5);

  // Pawn should be back on e2, no move logged
  TestApp::expect_ui_exists("White to move");
  auto snap = TestApp::capture_snapshot("chess_invalid_snapback");
}

TEST(chess_reset_board) {
  co_await TestApp::wait_for_frames(10);

  // Make a move first
  auto *pawn = TestApp::find_ui_element_by_label("e2_piece");
  if (pawn) {
    TestApp::click_ui_element(*pawn);
    co_await TestApp::wait_for_frames(3);
    auto *target = TestApp::find_ui_element_by_label("e4");
    if (target) {
      auto rect = target->get<afterhours::ui::UIComponent>().rect();
      test_input::set_mouse_position({rect.x + rect.width / 2.0f,
                                      rect.y + rect.height / 2.0f});
    }
    co_await TestApp::wait_for_frames(2);
    TestApp::release_mouse_button();
    co_await TestApp::wait_for_frames(5);
  }

  TestApp::expect_ui_exists("1. e4");

  // Reset
  TestApp::click_button("Reset Board");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(5);

  // Move log should be empty, turn back to white
  TestApp::expect_ui_not_exists("1. e4");
  TestApp::expect_ui_exists("White to move");
  auto snap = TestApp::capture_snapshot("chess_reset");
}
```

