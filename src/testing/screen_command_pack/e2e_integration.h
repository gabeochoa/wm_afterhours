// E2E Testing Integration
// Full system registration and runner setup for the application
#pragma once

#include "e2e_commands.h"

#include <afterhours/src/plugins/e2e_testing/e2e_testing.h>
#include <afterhours/src/plugins/e2e_testing/harness.h>
#include <afterhours/src/plugins/e2e_testing/ui_commands.h>

#include <filesystem>

namespace e2e {

using namespace afterhours;
using namespace afterhours::testing;

/// Configuration for E2E integration
struct E2EConfig {
  // Runner callbacks
  std::function<void(const std::string &)> screenshot_callback;
  std::function<void()> reset_callback;
  std::function<std::string(const std::string &)> property_getter;
  ui_commands::HandleDumpUICommand::DumpFn dump_callback;
};

/// Register all E2E systems in the correct order
///
/// Call order in your game:
///   1. Your update systems
///   2. register_e2e_systems() - processes E2E commands
///   3. Your render systems
///   4. runner.tick() - advances script
template <typename ScreenManager>
inline void register_e2e_systems(SystemManager &sm,
                                 const E2EConfig &config = {}) {
  // Phase 1: Built-in command handlers
  register_builtin_handlers(sm);

  // Phase 2: UI command handlers (tab, enter, arrow, focus_ui, click_ui,
  // dump_ui, etc.)
  ui_commands::register_ui_commands<InputAction>(sm, config.dump_callback);

  // Phase 2: Screenshot handler (needs callback)
  sm.register_update_system(
      std::make_unique<HandleScreenshotCommand>(config.screenshot_callback));

  // Phase 3: Reset handler (needs callback)
  sm.register_update_system(
      std::make_unique<HandleResetTestStateCommand>(config.reset_callback));

  // Phase 4: App-specific command handlers
  e2e_commands::register_app_commands<ScreenManager>(sm);

  // Phase 5: Unknown command handler + Cleanup (must be last)
  register_unknown_handler(sm);
  register_cleanup(sm);
}

/// Initialize an E2ERunner with standard callbacks
inline void configure_runner(E2ERunner &runner, const E2EConfig &config) {
  if (config.screenshot_callback) {
    runner.set_screenshot_callback(config.screenshot_callback);
  }
  if (config.reset_callback) {
    runner.set_reset_callback(config.reset_callback);
  }
  if (config.property_getter) {
    runner.set_property_getter(config.property_getter);
  }
}

/// Convenience function to set up E2E testing from CLI args
///
/// Usage in main.cpp:
///   if (e2e::should_run_e2e(argc, argv)) {
///     e2e::run_e2e_tests(argc, argv, config);
///     return 0;
///   }
// Arg parsing, screenshot paths and script counting live upstream now, so
// every project gets the same flags. Re-exported here so wm's call sites and
// any other consumer keep spelling them e2e::.
using afterhours::testing::count_e2e_scripts;
using afterhours::testing::parse_e2e_args;
using afterhours::testing::screenshot_path;
using afterhours::testing::should_run_e2e;

} // namespace e2e
