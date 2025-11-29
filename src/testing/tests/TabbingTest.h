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

  TestApp::simulate_tab();
  co_await TestApp::wait_for_frames(2);
  TestApp::expect_focus("Button 1");

  TestApp::simulate_tab();
  co_await TestApp::wait_for_frames(2);
  TestApp::expect_focus("Button 2");

  TestApp::simulate_tab();
  co_await TestApp::wait_for_frames(2);
  TestApp::expect_focus("Button 3");

  TestApp::simulate_tab();
  co_await TestApp::wait_for_frames(2);
  TestApp::expect_focus("Button 4");

  TestApp::simulate_shift_tab();
  co_await TestApp::wait_for_frames(2);
  TestApp::expect_focus("Button 3");

  TestApp::simulate_enter();
  co_await TestApp::wait_for_frames(2);
  TestApp::expect_ui_exists("Button 3 (1)");
}
