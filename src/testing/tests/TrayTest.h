#pragma once

#include "../test_app.h"
#include "../test_macros.h"
#include <afterhours/ah.h>

// Horizontal tray: arrow keys move selection, enter activates
TEST(tray_horizontal_navigation) {
  co_await TestApp::wait_for_frames(10);

  // Verify all horizontal tray buttons exist
  TestApp::expect_ui_exists("H-Alpha");
  TestApp::expect_ui_exists("H-Beta");
  TestApp::expect_ui_exists("H-Gamma");

  // Tab to the first tray (may be h or v depending on entity order)
  // Then tab until we land on horizontal tray
  TestApp::simulate_tab();
  co_await TestApp::wait_for_frames(3);

  // Check which tray we're on; if vertical, tab once more
  // We know exactly two tab stops exist; use a second tab to switch
  auto *ctx = afterhours::EntityHelper::get_singleton_cmp<
      afterhours::ui::UIContext<InputAction>>();
  auto opt =
      afterhours::ui::UICollectionHolder::getEntityForID(ctx->visual_focus_id);
  if (opt.has_value() && opt.asE().has<afterhours::ui::HasLabel>() &&
      opt.asE().get<afterhours::ui::HasLabel>().label == "V-Alpha") {
    TestApp::simulate_tab();
    co_await TestApp::wait_for_frames(3);
  }

  // Visual focus should be on H-Alpha
  TestApp::expect_visual_focus("H-Alpha");

  // Right arrow moves selection to H-Beta
  TestApp::simulate_arrow_key(raylib::KEY_RIGHT);
  co_await TestApp::wait_for_frames(3);
  TestApp::expect_visual_focus("H-Beta");

  // Right arrow moves selection to H-Gamma
  TestApp::simulate_arrow_key(raylib::KEY_RIGHT);
  co_await TestApp::wait_for_frames(3);
  TestApp::expect_visual_focus("H-Gamma");

  // Right arrow wraps to H-Alpha
  TestApp::simulate_arrow_key(raylib::KEY_RIGHT);
  co_await TestApp::wait_for_frames(3);
  TestApp::expect_visual_focus("H-Alpha");

  // Left arrow wraps to H-Gamma
  TestApp::simulate_arrow_key(raylib::KEY_LEFT);
  co_await TestApp::wait_for_frames(3);
  TestApp::expect_visual_focus("H-Gamma");

  // Enter activates H-Gamma
  TestApp::simulate_enter();
  co_await TestApp::wait_for_frames(5);
  TestApp::expect_ui_exists("H-Gamma (1)");
}

// Vertical tray: up/down arrows move selection
TEST(tray_vertical_navigation) {
  co_await TestApp::wait_for_frames(10);

  TestApp::expect_ui_exists("V-Alpha");
  TestApp::expect_ui_exists("V-Beta");
  TestApp::expect_ui_exists("V-Gamma");

  // Tab to the first tray, then navigate to vertical tray
  TestApp::simulate_tab();
  co_await TestApp::wait_for_frames(3);

  auto *ctx = afterhours::EntityHelper::get_singleton_cmp<
      afterhours::ui::UIContext<InputAction>>();
  auto opt =
      afterhours::ui::UICollectionHolder::getEntityForID(ctx->visual_focus_id);
  if (opt.has_value() && opt.asE().has<afterhours::ui::HasLabel>() &&
      opt.asE().get<afterhours::ui::HasLabel>().label == "H-Alpha") {
    TestApp::simulate_tab();
    co_await TestApp::wait_for_frames(3);
  }

  // Visual focus should be on V-Alpha
  TestApp::expect_visual_focus("V-Alpha");

  // Down arrow moves to V-Beta
  TestApp::simulate_arrow_key(raylib::KEY_DOWN);
  co_await TestApp::wait_for_frames(3);
  TestApp::expect_visual_focus("V-Beta");

  // Down arrow moves to V-Gamma
  TestApp::simulate_arrow_key(raylib::KEY_DOWN);
  co_await TestApp::wait_for_frames(3);
  TestApp::expect_visual_focus("V-Gamma");

  // Down arrow wraps to V-Alpha
  TestApp::simulate_arrow_key(raylib::KEY_DOWN);
  co_await TestApp::wait_for_frames(3);
  TestApp::expect_visual_focus("V-Alpha");

  // Up arrow wraps to V-Gamma
  TestApp::simulate_arrow_key(raylib::KEY_UP);
  co_await TestApp::wait_for_frames(3);
  TestApp::expect_visual_focus("V-Gamma");

  // Enter activates V-Gamma
  TestApp::simulate_enter();
  co_await TestApp::wait_for_frames(5);
  TestApp::expect_ui_exists("V-Gamma (1)");
}

// Tray acts as a single tab stop (children are skipped by tab)
TEST(tray_single_tab_stop) {
  co_await TestApp::wait_for_frames(10);

  // Tab to get on a tray
  TestApp::simulate_tab();
  co_await TestApp::wait_for_frames(3);

  // Determine which tray we're on
  auto *ctx = afterhours::EntityHelper::get_singleton_cmp<
      afterhours::ui::UIContext<InputAction>>();
  auto opt =
      afterhours::ui::UICollectionHolder::getEntityForID(ctx->visual_focus_id);
  bool first_is_horizontal =
      (opt.has_value() && opt.asE().has<afterhours::ui::HasLabel>() &&
       opt.asE().get<afterhours::ui::HasLabel>().label == "H-Alpha");

  if (first_is_horizontal) {
    TestApp::expect_visual_focus("H-Alpha");
    // Tab should go to vertical tray (not H-Beta)
    TestApp::simulate_tab();
    co_await TestApp::wait_for_frames(3);
    TestApp::expect_visual_focus("V-Alpha");
    // Tab wraps back to horizontal tray
    TestApp::simulate_tab();
    co_await TestApp::wait_for_frames(3);
    TestApp::expect_visual_focus("H-Alpha");
  } else {
    TestApp::expect_visual_focus("V-Alpha");
    // Tab should go to horizontal tray (not V-Beta)
    TestApp::simulate_tab();
    co_await TestApp::wait_for_frames(3);
    TestApp::expect_visual_focus("H-Alpha");
    // Tab wraps back to vertical tray
    TestApp::simulate_tab();
    co_await TestApp::wait_for_frames(3);
    TestApp::expect_visual_focus("V-Alpha");
  }
}

// Selection index persists when tabbing away and back
TEST(tray_selection_persists) {
  co_await TestApp::wait_for_frames(10);

  // Tab to get on horizontal tray
  TestApp::simulate_tab();
  co_await TestApp::wait_for_frames(3);

  // Navigate to horizontal tray if not already there
  auto *ctx = afterhours::EntityHelper::get_singleton_cmp<
      afterhours::ui::UIContext<InputAction>>();
  auto opt =
      afterhours::ui::UICollectionHolder::getEntityForID(ctx->visual_focus_id);
  if (opt.has_value() && opt.asE().has<afterhours::ui::HasLabel>() &&
      opt.asE().get<afterhours::ui::HasLabel>().label == "V-Alpha") {
    TestApp::simulate_tab();
    co_await TestApp::wait_for_frames(3);
  }

  TestApp::expect_visual_focus("H-Alpha");

  // Move selection to H-Beta
  TestApp::simulate_arrow_key(raylib::KEY_RIGHT);
  co_await TestApp::wait_for_frames(3);
  TestApp::expect_visual_focus("H-Beta");

  // Tab away to the other tray
  TestApp::simulate_tab();
  co_await TestApp::wait_for_frames(3);
  TestApp::expect_visual_focus("V-Alpha");

  // Tab back — should still be on H-Beta
  TestApp::simulate_tab();
  co_await TestApp::wait_for_frames(3);
  TestApp::expect_visual_focus("H-Beta");
}
