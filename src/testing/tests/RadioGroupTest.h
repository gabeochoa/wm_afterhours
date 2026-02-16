#pragma once

#include "../test_app.h"
#include "../test_macros.h"
#include <afterhours/ah.h>

// Radio group tray acts as a single tab stop
TEST(radio_buttons_single_tab_stop) {
  co_await TestApp::wait_for_frames(10);

  // Verify radio labels exist
  TestApp::expect_ui_exists("Apple");
  TestApp::expect_ui_exists("Small");
  TestApp::expect_ui_exists("Red");

  // Tab to first radio group — could be fruit, size, or color
  // depending on entity creation order. Just verify we land on
  // a radio tray child and that each tab advances to a different group.
  TestApp::simulate_tab();
  co_await TestApp::wait_for_frames(3);

  // Get which group we're on
  auto *ctx = afterhours::EntityHelper::get_singleton_cmp<
      afterhours::ui::UIContext<InputAction>>();
  auto first_focus = ctx->focus_id;

  // Tab should move to the next radio group (different focus_id)
  TestApp::simulate_tab();
  co_await TestApp::wait_for_frames(3);
  auto second_focus = ctx->focus_id;
  if (first_focus == second_focus) {
    throw std::runtime_error("Tab did not move to a different radio group");
  }

  // Tab should move to the third radio group
  TestApp::simulate_tab();
  co_await TestApp::wait_for_frames(3);
  auto third_focus = ctx->focus_id;
  if (third_focus == first_focus || third_focus == second_focus) {
    throw std::runtime_error("Tab did not move to a third distinct radio group");
  }

  // Tab wraps back to first radio group
  TestApp::simulate_tab();
  co_await TestApp::wait_for_frames(3);
  if (ctx->focus_id != first_focus) {
    throw std::runtime_error("Tab did not wrap back to the first radio group");
  }
}

// Arrow keys navigate between radio options within a group
TEST(radio_buttons_arrow_navigation) {
  co_await TestApp::wait_for_frames(10);

  // Tab to first radio group
  TestApp::simulate_tab();
  co_await TestApp::wait_for_frames(3);

  // Get the focused tray's initial visual focus
  auto *ctx = afterhours::EntityHelper::get_singleton_cmp<
      afterhours::ui::UIContext<InputAction>>();
  auto initial_visual = ctx->visual_focus_id;

  // Down arrow moves to the next child
  TestApp::simulate_arrow_key(raylib::KEY_DOWN);
  co_await TestApp::wait_for_frames(3);
  auto second_visual = ctx->visual_focus_id;
  if (second_visual == initial_visual) {
    throw std::runtime_error("Down arrow did not move visual focus");
  }

  // Down arrow moves to the next child
  TestApp::simulate_arrow_key(raylib::KEY_DOWN);
  co_await TestApp::wait_for_frames(3);
  auto third_visual = ctx->visual_focus_id;
  if (third_visual == second_visual) {
    throw std::runtime_error("Second down arrow did not move visual focus");
  }

  // Up arrow moves back
  TestApp::simulate_arrow_key(raylib::KEY_UP);
  co_await TestApp::wait_for_frames(3);
  if (ctx->visual_focus_id != second_visual) {
    throw std::runtime_error("Up arrow did not move back to previous item");
  }
}

// Enter activates the focused radio option
TEST(radio_buttons_enter_selects) {
  co_await TestApp::wait_for_frames(10);

  // Verify initial state
  TestApp::expect_ui_exists("Selected: Fruit=Orange, Size=Small, Color=Blue");

  // Tab to find the fruit radio group
  // We need to identify which tab stop is the fruit group
  // by checking if the visual focus is on a child of the fruit tray
  TestApp::simulate_tab();
  co_await TestApp::wait_for_frames(3);

  auto *ctx = afterhours::EntityHelper::get_singleton_cmp<
      afterhours::ui::UIContext<InputAction>>();

  // Check if we're on the fruit group by looking at the visual focus
  // debug name
  auto check_on_fruit = [&]() -> bool {
    auto opt = afterhours::ui::UICollectionHolder::getEntityForID(
        ctx->visual_focus_id);
    if (!opt.has_value()) return false;
    if (!opt.asE().has<afterhours::ui::UIComponentDebug>()) return false;
    auto &name = opt.asE().get<afterhours::ui::UIComponentDebug>().name_value;
    return name.find("fruit_radios") != std::string::npos;
  };

  // Tab up to 3 times to find the fruit group
  for (int attempt = 0; attempt < 3 && !check_on_fruit(); attempt++) {
    TestApp::simulate_tab();
    co_await TestApp::wait_for_frames(3);
  }

  if (!check_on_fruit()) {
    throw std::runtime_error("Could not find fruit radio group via tabbing");
  }

  // Now we're on the fruit group at index 0 (Apple)
  // Press Enter to select Apple (initial is Orange, index 1)
  TestApp::simulate_enter();
  co_await TestApp::wait_for_frames(10);

  // Verify selection changed to Apple
  TestApp::expect_ui_exists("Selected: Fruit=Apple, Size=Small, Color=Blue");
}
