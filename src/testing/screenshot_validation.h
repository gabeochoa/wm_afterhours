#pragma once

#include <string>

namespace screenshot_validation {

// Set whether to update baselines instead of comparing
void set_update_baselines(bool update);

// Get current update-baselines mode
bool is_update_baselines();

// Calculate pixel difference percentage between two images
// Returns 0.0 if images match exactly, 100.0 if completely different
float calculate_image_diff_percentage(const std::string &path1,
                                      const std::string &path2);

// Take a screenshot and save to specified path
void save_screenshot_to(const std::string &path);

// Validate current screen against baseline (returns true if diff <= 1%)
bool validate_screen_against_baseline(const std::string &screen_name);

} // namespace screenshot_validation

