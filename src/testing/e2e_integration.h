// E2E Testing Integration
// Full system registration and runner setup for the application
#pragma once

#include "e2e_commands.h"

#include <afterhours/src/plugins/e2e_testing/e2e_testing.h>

namespace e2e {

using namespace afterhours;
using namespace afterhours::testing;

/// Configuration for E2E integration
struct E2EConfig {
  // Runner callbacks
  std::function<void(const std::string &)> screenshot_callback;
  std::function<void()> reset_callback;
  std::function<std::string(const std::string &)> property_getter;
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
inline bool should_run_e2e(int argc, char *argv[]) {
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--e2e" || arg == "--test-mode" || arg == "--test-script" ||
        arg == "--test-script-dir") {
      return true;
    }
  }
  return false;
}

/// Parse E2E-related command line arguments
struct E2EArgs {
  bool enabled = false;
  std::string script_path;
  std::string script_dir;
  float timeout_seconds = 30.0f; // Default 30 second timeout
  bool slow_mode = false;        // Run tests slowly for visibility
  float slow_delay = 0.5f;       // Delay between commands in slow mode
  bool update_baselines = false; // Update baseline screenshots instead of comparing
};

inline E2EArgs parse_e2e_args(int argc, char *argv[]) {
  E2EArgs args;

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--e2e" || arg == "--test-mode") {
      args.enabled = true;
    } else if (arg == "--test-script" && i + 1 < argc) {
      args.script_path = argv[++i];
      args.enabled = true;
    } else if (arg == "--test-script-dir" && i + 1 < argc) {
      args.script_dir = argv[++i];
      args.enabled = true;
    } else if (arg == "--timeout" && i + 1 < argc) {
      args.timeout_seconds = std::stof(argv[++i]);
    } else if (arg == "--slow") {
      args.slow_mode = true;
    } else if (arg == "--slow-delay" && i + 1 < argc) {
      args.slow_delay = std::stof(argv[++i]);
      args.slow_mode = true;
    } else if (arg == "--update-baselines") {
      args.update_baselines = true;
    }
  }

  return args;
}

} // namespace e2e
