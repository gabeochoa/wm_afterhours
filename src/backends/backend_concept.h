// src/backends/backend_concept.h
#pragma once

#include <concepts>
#include <string>
#include "../rl.h"

namespace backend {

// Forward declarations for types used in concept
struct BackendConfig {
    int width = 1280;
    int height = 720;
    std::string title = "Afterhours";
    int target_fps = 60;
    bool vsync = true;
};

template<typename T>
concept Backend = requires(T t, const BackendConfig& cfg, const std::string& path) {
    // Lifecycle
    { t.init(cfg) } -> std::same_as<bool>;
    { t.shutdown() } -> std::same_as<void>;
    { t.is_headless() } -> std::same_as<bool>;

    // Frame
    { t.begin_frame() } -> std::same_as<void>;
    { t.end_frame() } -> std::same_as<void>;
    { t.clear(raylib::Color{}) } -> std::same_as<void>;

    // Capture
    { t.capture_frame(path) } -> std::same_as<bool>;

    // Access
    { t.get_render_texture() } -> std::same_as<raylib::RenderTexture2D&>;
};

} // namespace backend
