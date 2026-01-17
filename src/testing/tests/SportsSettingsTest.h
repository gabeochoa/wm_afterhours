#pragma once

#include "../test_app.h"
#include "../test_macros.h"
#include <afterhours/ah.h>

// =========================================================================
// sports_settings Tests
// =========================================================================
// Test scenarios for the sports_settings screen:
// 1. Clicking tab headers changes settings category
// 2. Clicking < > arrows changes setting values
// 3. Clicking on the label focuses/selects the row
// 4. Pressing left/right arrow keys changes value when row is focused
// 5. Pressing up/down navigates between rows
// 6. Pressing tab goes to the next row
// 7. Pressing tab then left/right changes value
// =========================================================================

// Test: Tab header clicking changes category
TEST(sports_settings_tab_click_changes_category) {
  co_await TestApp::wait_for_frames(10);

  // Should start on VIDEO tab (selected_tab = 1) showing GRAPHICS header
  TestApp::expect_ui_exists("GRAPHICS");
  TestApp::expect_ui_exists("Window mode");

  // Click on GAMEPLAY tab
  TestApp::click_button("GAMEPLAY");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(5);

  // Should now show GAMEPLAY settings
  TestApp::expect_ui_exists("GAMEPLAY");
  TestApp::expect_ui_exists("Difficulty");

  // Click on AUDIO tab
  TestApp::click_button("AUDIO");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(5);

  // Should now show AUDIO settings
  TestApp::expect_ui_exists("AUDIO");
  TestApp::expect_ui_exists("Master Volume");
}

// Test: Clicking > arrow changes setting value
TEST(sports_settings_arrow_click_changes_value) {
  co_await TestApp::wait_for_frames(10);

  // Start on VIDEO tab, should see "Borderless window" for Window mode
  TestApp::expect_ui_exists("Window mode");
  TestApp::expect_ui_exists("Borderless window");

  // Find and click the > button for Window mode (first row)
  // The > arrow is rendered after the value text
  auto *right_arrow = TestApp::find_ui_element_by_label(">");
  if (!right_arrow) {
    throw std::runtime_error("Could not find > arrow button");
  }

  // Click it
  TestApp::click_ui_element(*right_arrow);
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(5);

  // Value should have changed to "Windowed"
  TestApp::expect_ui_exists("Windowed");
}

// Test: Clicking < arrow cycles value backwards
TEST(sports_settings_left_arrow_click) {
  co_await TestApp::wait_for_frames(10);

  TestApp::expect_ui_exists("Window mode");
  TestApp::expect_ui_exists("Borderless window");

  // Find and click the < button
  auto *left_arrow = TestApp::find_ui_element_by_label("<");
  if (!left_arrow) {
    throw std::runtime_error("Could not find < arrow button");
  }

  TestApp::click_ui_element(*left_arrow);
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(5);

  // Should cycle backwards: Borderless window -> Fullscreen
  TestApp::expect_ui_exists("Fullscreen");
}

// Test: Clicking on label selects/focuses the row
TEST(sports_settings_label_click_selects_row) {
  co_await TestApp::wait_for_frames(10);

  // Click on Resolution label
  TestApp::click_button("Resolution");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(5);

  // Resolution should now be focused
  TestApp::expect_focus("Resolution");

  // Click on Max FPS label
  TestApp::click_button("Max FPS");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(5);

  // Max FPS should now be focused
  TestApp::expect_focus("Max FPS");
}

// Test: Tab key navigates to next row
TEST(sports_settings_tab_navigates_rows) {
  co_await TestApp::wait_for_frames(10);

  // First, focus Window mode by clicking it
  TestApp::click_button("Window mode");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(3);

  TestApp::expect_focus("Window mode");

  // Press Tab to go to next row
  TestApp::simulate_tab();
  co_await TestApp::wait_for_frames(5);

  TestApp::expect_focus("Resolution");

  // Tab again
  TestApp::simulate_tab();
  co_await TestApp::wait_for_frames(5);

  TestApp::expect_focus("Screen percentage");
}

// Test: Down arrow navigates to next row (WidgetNext)
TEST(sports_settings_down_arrow_navigates) {
  co_await TestApp::wait_for_frames(10);

  // Focus Window mode
  TestApp::click_button("Window mode");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(3);

  TestApp::expect_focus("Window mode");

  // Press Down arrow (KEY_DOWN is mapped to WidgetNext)
  TestApp::simulate_arrow_key(raylib::KEY_DOWN);
  co_await TestApp::wait_for_frames(5);

  TestApp::expect_focus("Resolution");
}

// Test: Up arrow navigates to previous row (WidgetBack)
TEST(sports_settings_up_arrow_navigates) {
  co_await TestApp::wait_for_frames(10);

  // Focus Resolution
  TestApp::click_button("Resolution");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(3);

  TestApp::expect_focus("Resolution");

  // Press Up arrow (KEY_UP is mapped to WidgetBack)
  TestApp::simulate_arrow_key(raylib::KEY_UP);
  co_await TestApp::wait_for_frames(5);

  TestApp::expect_focus("Window mode");
}

// Test: Left/Right arrows change value when row is focused
TEST(sports_settings_arrow_keys_change_value) {
  co_await TestApp::wait_for_frames(10);

  // Focus Window mode and verify initial value
  TestApp::click_button("Window mode");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(3);

  TestApp::expect_focus("Window mode");
  TestApp::expect_ui_exists("Borderless window");

  // Press Right arrow to change value
  TestApp::simulate_arrow_key(raylib::KEY_RIGHT);
  co_await TestApp::wait_for_frames(5);

  // Should change to Windowed
  TestApp::expect_ui_exists("Windowed");

  // Press Right again
  TestApp::simulate_arrow_key(raylib::KEY_RIGHT);
  co_await TestApp::wait_for_frames(5);

  // Should cycle to Fullscreen
  TestApp::expect_ui_exists("Fullscreen");

  // Press Left to go back
  TestApp::simulate_arrow_key(raylib::KEY_LEFT);
  co_await TestApp::wait_for_frames(5);

  // Should go back to Windowed
  TestApp::expect_ui_exists("Windowed");
}

// Test: Tab then left/right changes value on new row
// This test verifies that after tabbing to a row, arrow keys can change the
// value
TEST(sports_settings_tab_then_arrows) {
  co_await TestApp::wait_for_frames(10);

  // Click Window mode first to set initial focus and selection
  TestApp::click_button("Window mode");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(3);

  TestApp::expect_focus("Window mode");
  TestApp::expect_ui_exists("Borderless window");

  // Tab to Resolution
  TestApp::simulate_tab();
  co_await TestApp::wait_for_frames(5);
  TestApp::expect_focus("Resolution");
  TestApp::expect_ui_exists("2560x1440");

  // Press Right to change resolution
  TestApp::simulate_arrow_key(raylib::KEY_RIGHT);
  co_await TestApp::wait_for_frames(5);

  // Should change to 3840x2160
  TestApp::expect_ui_exists("3840x2160");
}

// Test: LB/RB buttons cycle through tabs
TEST(sports_settings_lb_rb_cycle_tabs) {
  co_await TestApp::wait_for_frames(10);

  // Start on VIDEO tab
  TestApp::expect_ui_exists("GRAPHICS");

  // Click RB to go to next tab (AUDIO)
  TestApp::click_button("RB");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(5);

  TestApp::expect_ui_exists("AUDIO");
  TestApp::expect_ui_exists("Master Volume");

  // Click RB again to go to CONTROLS
  TestApp::click_button("RB");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(5);

  TestApp::expect_ui_exists("CONTROLS");
  TestApp::expect_ui_exists("Vibration");

  // Click LB to go back to AUDIO
  TestApp::click_button("LB");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(5);

  TestApp::expect_ui_exists("AUDIO");
}
