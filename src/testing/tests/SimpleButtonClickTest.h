#pragma once

#include "../test_macros.h"
#include <afterhours/ah.h>

TEST(simple_button_click) {
  co_await TestApp::wait_for_frames(5);

  TestApp::expect_ui_exists("Click Me");

  // Click 1: press button, wait a frame for active to be set, then release
  TestApp::click_button("Click Me");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(2);

  TestApp::expect_ui_exists("Clicks: 1");

  // Click 2: same pattern
  TestApp::click_button("Click Me");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(2);

  TestApp::expect_ui_exists("Clicks: 2");
}
