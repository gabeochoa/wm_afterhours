#pragma once

#include "../test_app.h"
#include "../test_macros.h"
#include <afterhours/ah.h>

// =========================================================================
// Responsive Layout Validation Tests
// =========================================================================
// Verify that screens render correctly at the minimum supported resolution
// (720p = 1280x720). Any UI element that extends beyond the viewport at
// this resolution indicates the screen is not properly responsive.
//
// These tests resize the window to 720p, wait for layout recalculation,
// then walk the entire UI tree checking for viewport overflow.
// =========================================================================

// Test: SportsSettings screen fits within 720p viewport
TEST(sports_settings_responsive_720p) {
  // Let the screen render at default resolution first
  co_await TestApp::wait_for_frames(10);
  TestApp::expect_ui_exists("Window mode"); // Sanity check

  // Resize to minimum supported resolution (720p)
  TestApp::simulate_window_resize(1280, 720);
  co_await TestApp::wait_for_frames(5);

  // Assert all elements fit within the 720p viewport
  TestApp::assert_no_viewport_overflow(1280, 720);
}

// Test: SportsSettings screen fits within 1080p viewport
TEST(sports_settings_responsive_1080p) {
  co_await TestApp::wait_for_frames(10);
  TestApp::expect_ui_exists("Window mode");

  // Resize to a common larger resolution
  TestApp::simulate_window_resize(1920, 1080);
  co_await TestApp::wait_for_frames(5);

  TestApp::assert_no_viewport_overflow(1920, 1080);
}

// Test: Layout actually adapts when resolution changes (not hardcoded)
// This detects screens that use fixed pixel positions regardless of resolution.
TEST(sports_settings_layout_adapts_on_resize) {
  // Render at 720p and record a known element's position
  TestApp::simulate_window_resize(1280, 720);
  co_await TestApp::wait_for_frames(5);

  auto *elem_720 = TestApp::find_ui_element_by_label("Window mode");
  if (!elem_720 || !elem_720->has<afterhours::ui::UIComponent>()) {
    throw std::runtime_error("Could not find 'Window mode' element at 720p");
  }
  auto rect_720 = elem_720->get<afterhours::ui::UIComponent>().rect();

  // Resize to 1080p and record the same element's position
  TestApp::simulate_window_resize(1920, 1080);
  co_await TestApp::wait_for_frames(5);

  auto *elem_1080 = TestApp::find_ui_element_by_label("Window mode");
  if (!elem_1080 || !elem_1080->has<afterhours::ui::UIComponent>()) {
    throw std::runtime_error("Could not find 'Window mode' element at 1080p");
  }
  auto rect_1080 = elem_1080->get<afterhours::ui::UIComponent>().rect();

  // If position AND size are identical at both resolutions, the layout
  // is using hardcoded pixels and is NOT responsive.
  bool same_x = std::abs(rect_720.x - rect_1080.x) < 1.0f;
  bool same_y = std::abs(rect_720.y - rect_1080.y) < 1.0f;
  bool same_w = std::abs(rect_720.width - rect_1080.width) < 1.0f;
  bool same_h = std::abs(rect_720.height - rect_1080.height) < 1.0f;

  if (same_x && same_y && same_w && same_h) {
    throw std::runtime_error(
        "Layout is NOT responsive: 'Window mode' element has identical "
        "position/size at 720p (" +
        std::to_string((int)rect_720.x) + "," +
        std::to_string((int)rect_720.y) + " " +
        std::to_string((int)rect_720.width) + "x" +
        std::to_string((int)rect_720.height) + ") and 1080p (" +
        std::to_string((int)rect_1080.x) + "," +
        std::to_string((int)rect_1080.y) + " " +
        std::to_string((int)rect_1080.width) + "x" +
        std::to_string((int)rect_1080.height) +
        "). Screen likely uses hardcoded pixel positions.");
  }

  // Restore to default
  TestApp::simulate_window_resize(1280, 720);
  co_await TestApp::wait_for_frames(3);
}

// =========================================================================
// CasualSettings Responsive Tests
// =========================================================================

// Test: CasualSettings screen fits within 720p viewport
TEST(casual_settings_responsive_720p) {
  co_await TestApp::wait_for_frames(10);
  TestApp::expect_ui_exists("Settings"); // Title sanity check

  TestApp::simulate_window_resize(1280, 720);
  co_await TestApp::wait_for_frames(5);

  TestApp::assert_no_viewport_overflow(1280, 720);
}

// Test: CasualSettings screen fits within 1080p viewport
TEST(casual_settings_responsive_1080p) {
  co_await TestApp::wait_for_frames(10);
  TestApp::expect_ui_exists("Settings");

  TestApp::simulate_window_resize(1920, 1080);
  co_await TestApp::wait_for_frames(5);

  TestApp::assert_no_viewport_overflow(1920, 1080);
}

// Test: CasualSettings layout adapts when resolution changes
TEST(casual_settings_layout_adapts_on_resize) {
  TestApp::simulate_window_resize(1280, 720);
  co_await TestApp::wait_for_frames(5);

  auto *elem_720 = TestApp::find_ui_element_by_label("Notifications: OFF");
  if (!elem_720 || !elem_720->has<afterhours::ui::UIComponent>()) {
    throw std::runtime_error(
        "Could not find 'Notifications: OFF' element at 720p");
  }
  auto rect_720 = elem_720->get<afterhours::ui::UIComponent>().rect();

  TestApp::simulate_window_resize(1920, 1080);
  co_await TestApp::wait_for_frames(5);

  auto *elem_1080 = TestApp::find_ui_element_by_label("Notifications: OFF");
  if (!elem_1080 || !elem_1080->has<afterhours::ui::UIComponent>()) {
    throw std::runtime_error(
        "Could not find 'Notifications: OFF' element at 1080p");
  }
  auto rect_1080 = elem_1080->get<afterhours::ui::UIComponent>().rect();

  bool same_x = std::abs(rect_720.x - rect_1080.x) < 1.0f;
  bool same_y = std::abs(rect_720.y - rect_1080.y) < 1.0f;
  bool same_w = std::abs(rect_720.width - rect_1080.width) < 1.0f;
  bool same_h = std::abs(rect_720.height - rect_1080.height) < 1.0f;

  if (same_x && same_y && same_w && same_h) {
    throw std::runtime_error(
        "Layout is NOT responsive: 'Notifications: OFF' element has identical "
        "position/size at 720p (" +
        std::to_string((int)rect_720.x) + "," +
        std::to_string((int)rect_720.y) + " " +
        std::to_string((int)rect_720.width) + "x" +
        std::to_string((int)rect_720.height) + ") and 1080p (" +
        std::to_string((int)rect_1080.x) + "," +
        std::to_string((int)rect_1080.y) + " " +
        std::to_string((int)rect_1080.width) + "x" +
        std::to_string((int)rect_1080.height) +
        "). Screen likely uses hardcoded pixel positions.");
  }

  TestApp::simulate_window_resize(1280, 720);
  co_await TestApp::wait_for_frames(3);
}

// =========================================================================
// PowerWashSettings Responsive Tests
// =========================================================================

TEST(powerwash_settings_responsive_720p) {
  co_await TestApp::wait_for_frames(10);
  TestApp::expect_ui_exists("SETTINGS");
  TestApp::simulate_window_resize(1280, 720);
  co_await TestApp::wait_for_frames(5);
  TestApp::assert_no_viewport_overflow(1280, 720);
}

TEST(powerwash_settings_responsive_1080p) {
  co_await TestApp::wait_for_frames(10);
  TestApp::expect_ui_exists("SETTINGS");
  TestApp::simulate_window_resize(1920, 1080);
  co_await TestApp::wait_for_frames(5);
  TestApp::assert_no_viewport_overflow(1920, 1080);
}

// =========================================================================
// FlightOptions Responsive Tests
// =========================================================================

TEST(flight_options_responsive_720p) {
  co_await TestApp::wait_for_frames(10);
  TestApp::expect_ui_exists("OPTIONS");
  TestApp::simulate_window_resize(1280, 720);
  co_await TestApp::wait_for_frames(5);
  TestApp::assert_no_viewport_overflow(1280, 720);
}

TEST(flight_options_responsive_1080p) {
  co_await TestApp::wait_for_frames(10);
  TestApp::expect_ui_exists("OPTIONS");
  TestApp::simulate_window_resize(1920, 1080);
  co_await TestApp::wait_for_frames(5);
  TestApp::assert_no_viewport_overflow(1920, 1080);
}

// =========================================================================
// MiniMotorwaysSettings Responsive Tests
// =========================================================================

TEST(mini_motorways_settings_responsive_720p) {
  co_await TestApp::wait_for_frames(10);
  TestApp::expect_ui_exists("< Back");
  TestApp::simulate_window_resize(1280, 720);
  co_await TestApp::wait_for_frames(5);
  TestApp::assert_no_viewport_overflow(1280, 720);
}

TEST(mini_motorways_settings_responsive_1080p) {
  co_await TestApp::wait_for_frames(10);
  TestApp::expect_ui_exists("< Back");
  TestApp::simulate_window_resize(1920, 1080);
  co_await TestApp::wait_for_frames(5);
  TestApp::assert_no_viewport_overflow(1920, 1080);
}

// =========================================================================
// KirbyOptions Responsive Tests
// =========================================================================

TEST(kirby_options_responsive_720p) {
  co_await TestApp::wait_for_frames(10);
  TestApp::expect_ui_exists("Options");
  TestApp::simulate_window_resize(1280, 720);
  co_await TestApp::wait_for_frames(5);
  TestApp::assert_no_viewport_overflow(1280, 720);
}

TEST(kirby_options_responsive_1080p) {
  co_await TestApp::wait_for_frames(10);
  TestApp::expect_ui_exists("Options");
  TestApp::simulate_window_resize(1920, 1080);
  co_await TestApp::wait_for_frames(5);
  TestApp::assert_no_viewport_overflow(1920, 1080);
}

// =========================================================================
// IslandsTrainsSettings Responsive Tests
// NOTE: This screen uses decorative gradient overlays (ui_workarounds)
// with absolute positioning for vignette effects. Viewport overflow
// tests are skipped because those overlays trigger false positives.
// The panel content itself uses a layout tree.
// =========================================================================
