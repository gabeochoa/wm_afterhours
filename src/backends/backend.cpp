// src/backends/backend.cpp
//
// Backend implementation - change the backend here.
// Only this file needs recompilation when switching backends.
//

#include "backend.h"
#include "raylib_backend.h"

#if defined(AFTERHOURS_USE_IMAGE_BACKEND)
#include "image_backend.h"
#endif

namespace backend {

// Backend selection - change this typedef to switch backends
#if defined(AFTERHOURS_USE_IMAGE_BACKEND)
    using CurrentBackend = ImageBackend;
#else
    using CurrentBackend = RaylibBackend;
#endif

// Global backend instance (internal linkage via anonymous namespace)
namespace {
    CurrentBackend g_backend;
    ImageConfig g_image_config;  // Stored config for when ImageBackend is used
}

bool init(const BackendConfig& cfg) {
#if defined(AFTERHOURS_USE_IMAGE_BACKEND)
    // Apply stored image config before initialization
    ImageBackendConfig img_cfg;
    img_cfg.width = g_image_config.width;
    img_cfg.height = g_image_config.height;
    img_cfg.auto_save = g_image_config.auto_save;
    img_cfg.save_every_n_frames = g_image_config.save_every_n_frames;
    img_cfg.output_dir = g_image_config.output_dir;
    img_cfg.filename_pattern = g_image_config.filename_pattern;
    g_backend.set_image_config(img_cfg);
#endif
    return g_backend.init(cfg);
}

void shutdown() {
    g_backend.shutdown();
}

bool is_headless() {
    return g_backend.is_headless();
}

void begin_frame() {
    g_backend.begin_frame();
}

void end_frame() {
    g_backend.end_frame();
}

void clear(raylib::Color color) {
    g_backend.clear(color);
}

bool capture_frame(const std::string& path) {
    return g_backend.capture_frame(path);
}

raylib::RenderTexture2D& get_render_texture() {
    return g_backend.get_render_texture();
}

void set_image_config(const ImageConfig& cfg) {
    g_image_config = cfg;
#if defined(AFTERHOURS_USE_IMAGE_BACKEND)
    // If backend is ImageBackend, also set it directly
    ImageBackendConfig img_cfg;
    img_cfg.width = cfg.width;
    img_cfg.height = cfg.height;
    img_cfg.auto_save = cfg.auto_save;
    img_cfg.save_every_n_frames = cfg.save_every_n_frames;
    img_cfg.output_dir = cfg.output_dir;
    img_cfg.filename_pattern = cfg.filename_pattern;
    g_backend.set_image_config(img_cfg);
#endif
}

} // namespace backend
