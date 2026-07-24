#pragma once

#include <string>
#include <vector>

// Global headless output directory (set by main.cpp)
extern bool g_headless_mode;
extern std::string g_headless_output_dir;

// Resolution configuration for multi-resolution screenshots
struct HeadlessResolution {
  int width;
  int height;
  std::string label; // e.g. "480p", "720p", "1080p"
};

// Resolutions to capture (set by main.cpp, empty = default 720p only)
extern std::vector<HeadlessResolution> g_headless_resolutions;

// If non-empty, only this screen is captured (set by main.cpp via --screen).
extern std::string g_headless_screen_filter;

// Run headless screenshot generation for all registered screens
// at each resolution in g_headless_resolutions.
// Outputs PNG files to g_headless_output_dir as {screen}_{label}.png
void run_headless_screenshots();

// Run all registered coroutine-based tests in headless mode.
// Returns the number of test failures (0 = all passed).
int run_all_tests_headless();

// Run focus ring test: tab through a screen capturing a screenshot after each
// tab. If screen_filter is empty, tests all screens. Otherwise only the named
// screen. max_tabs controls how many tab presses per screen. Outputs to
// g_headless_output_dir/focus_test/{screen}/tab_{n}.png
void run_focus_ring_test(const std::string &screen_filter, int max_tabs);

// Render one screen headlessly and print/write a flat layout summary.
// output_path: file path, or "-" / empty for stdout. Returns 0 on success.
int run_layout_summary(const std::string &screen_name, int width, int height,
                       int frame_count, const std::string &output_path);
