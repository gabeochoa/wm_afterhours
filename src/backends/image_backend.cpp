// src/backends/image_backend.cpp
#include "image_backend.h"
#include "../log.h"
#include <afterhours/src/plugins/files.h>

// OpenGL functions (glFlush, glFinish) are available via headless_gl headers
// which include <OpenGL/gl3.h> on macOS

namespace backend {

bool ImageBackend::init(const BackendConfig& cfg) {
    config = cfg;

    // Use image_config dimensions if set, otherwise use cfg
    int w = image_config.width > 0 ? image_config.width : cfg.width;
    int h = image_config.height > 0 ? image_config.height : cfg.height;

    // Initialize headless GL context
    if (!gl.init(w, h)) {
        log_warn("[ImageBackend] Failed to create headless GL context");
        return false;
    }
    log_info("[ImageBackend] Initialized headless GL context");

    // Load GL extensions
    raylib::rlLoadExtensions(gl.get_proc_address());
    log_info("[ImageBackend] Loaded GL extensions");

    // Initialize rlgl
    raylib::rlglInit(w, h);
    raylib::rlSetBlendMode(raylib::RL_BLEND_ALPHA);
    log_info("[ImageBackend] Initialized rlgl");

    // Create render texture
    render_texture = raylib::LoadRenderTexture(w, h);
    log_info("[ImageBackend] Created render texture");

    // Ensure output directory exists
    if (!image_config.output_dir.empty()) {
        std::filesystem::create_directories(image_config.output_dir);
    }

    initialized = true;
    return true;
}

void ImageBackend::shutdown() {
    if (initialized) {
        raylib::UnloadRenderTexture(render_texture);
        raylib::rlglClose();
        gl.shutdown();
        initialized = false;
        log_info("[ImageBackend] Shutdown complete");
    }
}

void ImageBackend::begin_frame() {
    raylib::BeginTextureMode(render_texture);
}

void ImageBackend::end_frame() {
    raylib::EndTextureMode();
    frame_count++;

    if (image_config.auto_save &&
        (frame_count % image_config.save_every_n_frames == 0)) {
        std::string filename = fmt::format(
            fmt::runtime(image_config.filename_pattern),
            frame_count
        );
        std::filesystem::path path = image_config.output_dir / filename;
        capture_frame(path.string());
    }
}

void ImageBackend::clear(raylib::Color color) {
    raylib::ClearBackground(color);
}

bool ImageBackend::capture_frame(const std::string& path) {
    // Ensure GPU operations complete
    glFlush();
    glFinish();

    // Get image from texture
    raylib::Image img = raylib::LoadImageFromTexture(render_texture.texture);
    raylib::ImageFlipVertical(&img);

    // Export using raylib (uses stb_image_write internally)
    bool success = raylib::ExportImage(img, path.c_str());

    if (success) {
        log_info("[ImageBackend] Saved frame to: {}", path);
    } else {
        log_warn("[ImageBackend] Failed to save frame to: {}", path);
    }

    raylib::UnloadImage(img);
    return success;
}

} // namespace backend
