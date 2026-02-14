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

// Run headless screenshot generation for all registered screens
// at each resolution in g_headless_resolutions.
// Outputs PNG files to g_headless_output_dir as {screen}_{label}.png
void run_headless_screenshots();

// Run all registered coroutine-based tests in headless mode.
// Returns the number of test failures (0 = all passed).
int run_all_tests_headless();
