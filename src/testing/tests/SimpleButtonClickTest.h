#pragma once

#include "../test_macros.h"
#include <afterhours/ah.h>

TEST(simple_button_click) {
  co_await TestApp::wait_for_frames(5);

  TestApp::expect_ui_exists("Click Me");

  TestApp::click_button("Click Me");

  co_await TestApp::wait_for_frames(2);

  TestApp::expect_ui_exists("Clicks: 1");

  TestApp::click_button("Click Me");

  co_await TestApp::wait_for_frames(2);

  TestApp::expect_ui_exists("Clicks: 2");
}
