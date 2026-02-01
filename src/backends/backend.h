// src/backends/backend.h
//
// Backend abstraction layer - only declares functions.
// The actual backend is selected in backend.cpp.
// Changing backends only requires recompiling backend.cpp.
//
#pragma once

#include "backend_concept.h"

namespace backend {

// Lifecycle
bool init(const BackendConfig& cfg);
void shutdown();
bool is_headless();

// Frame management
void begin_frame();
void end_frame();
void clear(raylib::Color color);

// Screenshot capture
bool capture_frame(const std::string& path);

// Render texture access
raylib::RenderTexture2D& get_render_texture();

// ImageBackend-specific configuration
// Call before init() to configure auto-save behavior
struct ImageConfig {
    int width = 1920;
    int height = 1080;
    bool auto_save = false;
    int save_every_n_frames = 1;
    std::filesystem::path output_dir = "output";
    std::string filename_pattern = "frame_{:04d}.png";
};

void set_image_config(const ImageConfig& cfg);

} // namespace backend
