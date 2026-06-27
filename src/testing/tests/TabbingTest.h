#pragma once

#include "../test_app.h"
#include "../test_macros.h"
#include <afterhours/ah.h>

TEST(tabbing) {
  co_await TestApp::wait_for_frames(5);

  // The tabbing screen uses OPTION A/B/C/D labels
  TestApp::expect_ui_exists("OPTION A");
  TestApp::expect_ui_exists("OPTION B");
  TestApp::expect_ui_exists("OPTION C");
  TestApp::expect_ui_exists("OPTION D");

  co_await TestApp::wait_for_frames(2);

  // First button gets auto-focused
  TestApp::expect_focus("OPTION A");

  // Tab through the buttons
  TestApp::simulate_tab();
  co_await TestApp::wait_for_frames(2);
  TestApp::expect_focus("OPTION B");

  TestApp::simulate_tab();
  co_await TestApp::wait_for_frames(2);
  TestApp::expect_focus("OPTION C");

  // Press Enter to click OPTION C
  // Need to wait several frames for: input processing, click detection, label
  // update
  TestApp::simulate_enter();
  co_await TestApp::wait_for_frames(5);
  TestApp::expect_ui_exists("OPTION C (1)");
}
