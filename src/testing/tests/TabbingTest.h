#pragma once

#include "../test_app.h"
#include "../test_macros.h"
#include <afterhours/ah.h>

TEST(tabbing) {
  co_await TestApp::wait_for_frames(5);

  TestApp::expect_ui_exists("Button 1");
  TestApp::expect_ui_exists("Button 2");
  TestApp::expect_ui_exists("Button 3");
  TestApp::expect_ui_exists("Button 4");

  co_await TestApp::wait_for_frames(2);

  // Button 1 gets auto-focused by try_to_grab
  TestApp::expect_focus("Button 1");

  // Tab through the buttons
  TestApp::simulate_tab();
  co_await TestApp::wait_for_frames(2);
  TestApp::expect_focus("Button 2");

  TestApp::simulate_tab();
  co_await TestApp::wait_for_frames(2);
  TestApp::expect_focus("Button 3");

  // Press Enter to click Button 3
  // Need to wait several frames for: input processing, click detection, label
  // update
  TestApp::simulate_enter();
  co_await TestApp::wait_for_frames(5);
  TestApp::expect_ui_exists("Button 3 (1)");
}
