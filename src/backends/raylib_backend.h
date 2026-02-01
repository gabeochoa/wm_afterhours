// src/backends/raylib_backend.h
#pragma once

#include "backend_concept.h"
#include "../rl.h"

namespace backend {

struct RaylibBackend {
    raylib::RenderTexture2D render_texture{};
    BackendConfig config{};
    bool initialized = false;

    bool init(const BackendConfig& cfg) {
        config = cfg;
        raylib::SetConfigFlags(raylib::FLAG_WINDOW_RESIZABLE | raylib::FLAG_MSAA_4X_HINT);
        raylib::InitWindow(cfg.width, cfg.height, cfg.title.c_str());
        raylib::SetTargetFPS(cfg.target_fps);
        render_texture = raylib::LoadRenderTexture(cfg.width, cfg.height);
        initialized = true;
        return true;
    }

    void shutdown() {
        if (initialized) {
            raylib::UnloadRenderTexture(render_texture);
            raylib::CloseWindow();
            initialized = false;
        }
    }

    bool is_headless() const { return false; }

    void begin_frame() {
        raylib::BeginTextureMode(render_texture);
    }

    void end_frame() {
        raylib::EndTextureMode();
    }

    void clear(raylib::Color color) {
        raylib::ClearBackground(color);
    }

    bool capture_frame(const std::string& path) {
        raylib::Image img = raylib::LoadImageFromTexture(render_texture.texture);
        raylib::ImageFlipVertical(&img);
        bool success = raylib::ExportImage(img, path.c_str());
        raylib::UnloadImage(img);
        return success;
    }

    raylib::RenderTexture2D& get_render_texture() {
        return render_texture;
    }
};

static_assert(Backend<RaylibBackend>, "RaylibBackend must satisfy Backend concept");

} // namespace backend
