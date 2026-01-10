#pragma once

#include "../test_app.h"
#include "../test_macros.h"
#include <afterhours/ah.h>

TEST(snapshot_test) {
  co_await TestApp::wait_for_frames(5);

  TestApp::expect_ui_exists("Click Me");

  // First verify snapshot capture works
  auto result = TestApp::capture_snapshot("simple_button_initial");
  if (!result.success) {
    throw std::runtime_error("Failed to capture snapshot: " +
                             result.error_message);
  }

  co_await TestApp::wait_for_frames(2);

  // Verify clicking works
  TestApp::click_button("Click Me");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(2);

  TestApp::expect_ui_exists("Clicks: 1");

  // Capture the post-click state
  auto after_result = TestApp::capture_snapshot("simple_button_after_click");
  if (!after_result.success) {
    throw std::runtime_error("Failed to capture after-click snapshot: " +
                             after_result.error_message);
  }

  // Wait for rendering to stabilize
  co_await TestApp::wait_for_frames(2);

  // Compare against the same state we just captured (should be identical)
  auto compare_result = TestApp::compare_snapshot("simple_button_after_click");
  if (!compare_result.success) {
    throw std::runtime_error("Snapshot comparison failed: " +
                             compare_result.error_message);
  }
}
