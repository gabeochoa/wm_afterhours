// src/backends/image_backend.h
#pragma once

#include "backend_concept.h"
#include "../engine/headless_gl.h"
#include "../rl.h"
#include <string>
#include <filesystem>
#include <fmt/format.h>

namespace backend {

struct ImageBackendConfig {
    int width = 1920;
    int height = 1080;
    bool auto_save = false;
    int save_every_n_frames = 1;
    std::filesystem::path output_dir = "output";
    std::string filename_pattern = "frame_{:04d}.png";
};

struct ImageBackend {
    ImageBackendConfig image_config{};
    BackendConfig config{};
    HeadlessGL gl{};
    raylib::RenderTexture2D render_texture{};
    int frame_count = 0;
    bool initialized = false;

    bool init(const BackendConfig& cfg);
    void shutdown();
    bool is_headless() const { return true; }

    void begin_frame();
    void end_frame();
    void clear(raylib::Color color);

    bool capture_frame(const std::string& path);

    raylib::RenderTexture2D& get_render_texture() {
        return render_texture;
    }

    // ImageBackend-specific configuration
    void set_image_config(const ImageBackendConfig& cfg) {
        image_config = cfg;
    }
};

static_assert(Backend<ImageBackend>, "ImageBackend must satisfy Backend concept");

} // namespace backend
