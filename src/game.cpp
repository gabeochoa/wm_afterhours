#include "game.h"

#include "components.h"
#include "input_mapping.h"
#include "log.h"
#include "preload.h"
#include "render_backend.h"
#include "settings.h"
#include "systems/ExampleScreenRegistry.h"
#include "systems/RenderRenderTexture.h"
#include "systems/RenderSystemHelpers.h"
#include "systems/RenderTestFeedback.h"
#include "systems/SetupSimpleButtonTest.h"
#include "systems/SetupTabbingTest.h"
#include "systems/TestSystem.h"
#include "systems/UpdateRenderTexture.h"
#include "testing/test_app.h"
#include "testing/test_input.h"
#include "testing/test_macros.h"
#include "testing/tests/SimpleButtonClickTest.h"
#include <afterhours/src/plugins/files.h>

#include <afterhours/src/plugins/animation.h>
#include <chrono>
#include <iostream>
#include <thread>

bool running = true;
raylib::RenderTexture2D mainRT;
raylib::RenderTexture2D screenRT;
raylib::Font uiFont;

void game() {
  mainRT = raylib::LoadRenderTexture(Settings::get().get_screen_width(),
                                     Settings::get().get_screen_height());
  screenRT = raylib::LoadRenderTexture(Settings::get().get_screen_width(),
                                       Settings::get().get_screen_height());
  uiFont = raylib::LoadFont(
      afterhours::files::get_resource_path("fonts", "Gaegu-Bold.ttf")
          .string()
          .c_str());

  afterhours::SystemManager systems;

  {
    afterhours::window_manager::enforce_singletons(systems);
    afterhours::ui::enforce_singletons<InputAction>(systems);
    afterhours::input::enforce_singletons(systems);
  }

  TestSystem *test_system_ptr = nullptr;

  {
    afterhours::input::register_update_systems(systems);
    afterhours::window_manager::register_update_systems(systems);

    auto test_system = std::make_unique<TestSystem>();
    test_system_ptr = test_system.get();
    systems.register_update_system(std::move(test_system));
  }

  {
    systems.register_render_system(std::make_unique<BeginWorldRender>());
    systems.register_render_system(std::make_unique<EndWorldRender>());
    systems.register_render_system(
        std::make_unique<BeginPostProcessingRender>());
    systems.register_render_system(std::make_unique<RenderRenderTexture>());
    afterhours::ui::register_render_systems<InputAction>(
        systems, InputAction::ToggleUILayoutDebug);
    systems.register_render_system(std::make_unique<EndDrawing>());
  }

  while (running && !raylib::WindowShouldClose()) {
    if (raylib::IsKeyPressed(raylib::KEY_ESCAPE)) {
      running = false;
    }
    float dt = raylib::GetFrameTime();
    systems.run(dt);

    if (test_system_ptr && test_system_ptr->is_complete()) {
      std::string error = test_system_ptr->get_error();
      if (!error.empty()) {
        std::cout << "Test '" << test_system_ptr->get_test_name()
                  << "' failed: " << error << std::endl;
        running = false;
      } else {
        std::cout << "Test '" << test_system_ptr->get_test_name() << "' passed!"
                  << std::endl;
        running = false;
      }
    }
  }
}

void run_test(const std::string &test_name, bool slow_mode, bool hold_on_end) {
  TestRegistry &registry = TestRegistry::get();
  auto it = registry.tests.find(test_name);
  if (it == registry.tests.end()) {
    std::cout << "Test '" << test_name << "' not found" << std::endl;
    return;
  }

  test_input::slow_test_mode = slow_mode;
  test_input::test_mode = true;

  mainRT = raylib::LoadRenderTexture(Settings::get().get_screen_width(),
                                     Settings::get().get_screen_height());
  screenRT = raylib::LoadRenderTexture(Settings::get().get_screen_width(),
                                       Settings::get().get_screen_height());
  uiFont = raylib::LoadFont(
      afterhours::files::get_resource_path("fonts", "Gaegu-Bold.ttf")
          .string()
          .c_str());

  afterhours::SystemManager systems;

  {
    afterhours::window_manager::enforce_singletons(systems);
    afterhours::ui::enforce_singletons<InputAction>(systems);
    afterhours::input::enforce_singletons(systems);
  }

  TestSystem *test_system_ptr = nullptr;

  {
    afterhours::input::register_update_systems(systems);
    afterhours::window_manager::register_update_systems(systems);

    systems.register_update_system(std::make_unique<UpdateRenderTexture>());

    auto test_system = std::make_unique<TestSystem>();
    test_system_ptr = test_system.get();
    systems.register_update_system(std::move(test_system));
  }

  {
    systems.register_render_system(std::make_unique<BeginWorldRender>());
    systems.register_render_system(std::make_unique<EndWorldRender>());
    systems.register_render_system(
        std::make_unique<BeginPostProcessingRender>());
    systems.register_render_system(std::make_unique<RenderRenderTexture>());

    if (test_name == "tabbing") {
      systems.register_update_system(std::make_unique<SetupTabbingTest>());
    } else {
      systems.register_update_system(std::make_unique<SetupSimpleButtonTest>());
    }

    systems.register_render_system(std::make_unique<RenderTestFeedback>());
    afterhours::ui::register_render_systems<InputAction>(
        systems, InputAction::ToggleUILayoutDebug);
    systems.register_render_system(std::make_unique<EndDrawing>());
  }

  TestApp test = it->second();
  test_system_ptr->set_test(test_name, std::move(test));

  while (running && !raylib::WindowShouldClose()) {
    if (raylib::IsKeyPressed(raylib::KEY_ESCAPE)) {
      running = false;
    }
    float dt = raylib::GetFrameTime();
    systems.run(dt);

    if (test_input::slow_test_mode) {
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    if (test_system_ptr && test_system_ptr->is_complete()) {
      std::string error = test_system_ptr->get_error();
      if (!error.empty()) {
        std::cout << "Test '" << test_system_ptr->get_test_name()
                  << "' failed: " << error << std::endl;
        if (!hold_on_end) {
          running = false;
        }
      } else {
        std::cout << "Test '" << test_system_ptr->get_test_name() << "' passed!"
                  << std::endl;
        if (!hold_on_end) {
          running = false;
        }
      }
      if (hold_on_end) {
        std::cout << "Test complete. Window will stay open. Press ESC to exit."
                  << std::endl;
      }
    }
  }
}

void run_screen_demo(const std::string &screen_name, bool hold_on_end) {
  mainRT = raylib::LoadRenderTexture(Settings::get().get_screen_width(),
                                     Settings::get().get_screen_height());
  screenRT = raylib::LoadRenderTexture(Settings::get().get_screen_width(),
                                       Settings::get().get_screen_height());

  uiFont = raylib::LoadFont(
      afterhours::files::get_resource_path("fonts", "Gaegu-Bold.ttf")
          .string()
          .c_str());

  afterhours::SystemManager systems;

  {
    afterhours::window_manager::enforce_singletons(systems);
    afterhours::ui::enforce_singletons<InputAction>(systems);
    afterhours::input::enforce_singletons(systems);
  }

  {
    afterhours::input::register_update_systems(systems);
    afterhours::window_manager::register_update_systems(systems);

    systems.register_update_system(std::make_unique<UpdateRenderTexture>());
  }

  {
    systems.register_render_system(std::make_unique<BeginWorldRender>());

    afterhours::ui::register_render_systems<InputAction>(
        systems, InputAction::ToggleUILayoutDebug);

    systems.register_render_system(std::make_unique<EndWorldRender>());
    systems.register_render_system(
        std::make_unique<BeginPostProcessingRender>());
    systems.register_render_system(std::make_unique<RenderRenderTexture>());
    systems.register_render_system(std::make_unique<EndDrawing>());
  }

  {
    afterhours::ui::register_before_ui_updates<InputAction>(systems);

    auto screen_system =
        ExampleScreenRegistry::get().create_screen(screen_name);
    if (!screen_system) {
      std::cerr << "ERROR: Failed to create screen: " << screen_name
                << std::endl;
      return;
    }
    systems.register_update_system(std::move(screen_system));

    afterhours::ui::register_after_ui_updates<InputAction>(systems);
  }

  int frame_count = 0;
  bool demo_complete = false;
  while (running && !raylib::WindowShouldClose()) {
    if (raylib::IsKeyPressed(raylib::KEY_ESCAPE)) {
      running = false;
    }
    float dt = raylib::GetFrameTime();
    systems.run(dt);
    frame_count++;
    if (frame_count >= 5 && !demo_complete) {
      demo_complete = true;
      if (hold_on_end) {
        std::cout << "Demo complete. Window will stay open. Press ESC to exit."
                  << std::endl;
      } else {
        break; // Exit after 5 frames if not holding
      }
    }
  }
}
