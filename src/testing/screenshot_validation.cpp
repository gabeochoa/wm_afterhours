#include "screenshot_validation.h"

#include "../log.h"
#include "../rl.h"

#include <cmath>
#include <filesystem>

// External reference to the main render texture from game.cpp
extern raylib::RenderTexture2D mainRT;

namespace screenshot_validation {

// Global flag for update-baselines mode
static bool g_update_baselines = false;

void set_update_baselines(bool update) { g_update_baselines = update; }

bool is_update_baselines() { return g_update_baselines; }

float calculate_image_diff_percentage(const std::string &path1,
                                      const std::string &path2) {
  raylib::Image img1 = raylib::LoadImage(path1.c_str());
  raylib::Image img2 = raylib::LoadImage(path2.c_str());

  if (img1.data == nullptr || img2.data == nullptr) {
    if (img1.data)
      raylib::UnloadImage(img1);
    if (img2.data)
      raylib::UnloadImage(img2);
    return 100.0f; // Can't compare, treat as completely different
  }

  // Size mismatch means different
  if (img1.width != img2.width || img1.height != img2.height) {
    raylib::UnloadImage(img1);
    raylib::UnloadImage(img2);
    log_warn("Image size mismatch: {}x{} vs {}x{}", img1.width, img1.height,
             img2.width, img2.height);
    return 100.0f;
  }

  // Get raw pixel data as RGBA
  raylib::Color *pixels1 = raylib::LoadImageColors(img1);
  raylib::Color *pixels2 = raylib::LoadImageColors(img2);

  int total_pixels = img1.width * img1.height;
  long long total_diff = 0;
  long long max_diff = static_cast<long long>(total_pixels) * 255 * 3;

  for (int i = 0; i < total_pixels; i++) {
    total_diff += std::abs(static_cast<int>(pixels1[i].r) -
                           static_cast<int>(pixels2[i].r));
    total_diff += std::abs(static_cast<int>(pixels1[i].g) -
                           static_cast<int>(pixels2[i].g));
    total_diff += std::abs(static_cast<int>(pixels1[i].b) -
                           static_cast<int>(pixels2[i].b));
  }

  raylib::UnloadImageColors(pixels1);
  raylib::UnloadImageColors(pixels2);
  raylib::UnloadImage(img1);
  raylib::UnloadImage(img2);

  return (static_cast<float>(total_diff) / static_cast<float>(max_diff)) *
         100.0f;
}

void save_screenshot_to(const std::string &path) {
  raylib::Image image = raylib::LoadImageFromTexture(mainRT.texture);
  if (image.data == nullptr) {
    log_error("Failed to capture screenshot");
    return;
  }
  raylib::ImageFlipVertical(&image);
  raylib::ExportImage(image, path.c_str());
  raylib::UnloadImage(image);
}

bool validate_screen_against_baseline(const std::string &screen_name) {
  std::string baseline_dir = "baseline_screenshots";
  std::string baseline_path = baseline_dir + "/" + screen_name + ".png";
  std::string temp_path = "/tmp/validate_" + screen_name + ".png";

  // Take screenshot of current state
  save_screenshot_to(temp_path);

  // In update-baselines mode, copy to baseline and pass
  if (g_update_baselines) {
    // Ensure baseline directory exists
    std::filesystem::create_directories(baseline_dir);
    std::filesystem::copy_file(
        temp_path, baseline_path,
        std::filesystem::copy_options::overwrite_existing);
    log_info("[validate_screen] Updated baseline: {}", baseline_path);
    return true;
  }

  // Check if baseline exists
  if (!std::filesystem::exists(baseline_path)) {
    log_error("[validate_screen] Baseline not found: {}", baseline_path);
    log_error("Run with --update-baselines to create it");
    return false;
  }

  // Compare images
  float diff_pct = calculate_image_diff_percentage(baseline_path, temp_path);
  log_info("[validate_screen] {} diff: {:.4f}%", screen_name, diff_pct);

  if (diff_pct > 1.0f) {
    log_error("[validate_screen] FAILED: {} differs by {:.4f}% (threshold: 1%)",
              screen_name, diff_pct);
    // Keep the temp file for debugging
    std::string fail_path = "/tmp/validate_FAILED_" + screen_name + ".png";
    std::filesystem::copy_file(
        temp_path, fail_path,
        std::filesystem::copy_options::overwrite_existing);
    log_error("Failed screenshot saved to: {}", fail_path);
    return false;
  }

  return true;
}

} // namespace screenshot_validation

