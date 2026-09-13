#pragma once

#include "../components.h"
#include "../game.h"
#include "../render_backend.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/ui/text_stroke.h>

struct BeginWorldRender : afterhours::System<> {
  virtual void once(float) const override {
    afterhours::ui::text_stroke::begin_frame();
    if (render_backend::draw_directly_to_window) {
      render_backend::BeginDrawing();
      render_backend::ClearBackground(raylib::BLACK);
      return;
    }
    render_backend::BeginTextureMode(mainRT);
    render_backend::ClearBackground(raylib::BLACK);
  }
};

struct EndWorldRender : afterhours::System<> {
  virtual void once(float) const override {
    if (render_backend::draw_directly_to_window) return;
    render_backend::EndTextureMode();
  }
};

struct BeginPostProcessingRender : afterhours::System<> {
  virtual void once(float) const override {
    if (render_backend::draw_directly_to_window) return;
    render_backend::BeginDrawing();
    render_backend::ClearBackground(raylib::BLACK);
  }
};

struct EndDrawing : afterhours::System<> {
  virtual void once(float) const override { render_backend::EndDrawing(); }
};
