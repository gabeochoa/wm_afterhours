#pragma once

#include <string>

// Global headless output directory (set by main.cpp)
extern bool g_headless_mode;
extern std::string g_headless_output_dir;

// Run headless screenshot generation for all registered screens
// Outputs PNG files to g_headless_output_dir
void run_headless_screenshots();
