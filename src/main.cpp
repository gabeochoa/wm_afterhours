#ifdef BACKWARD
#include "backward/backward.hpp"
namespace backward {
backward::SignalHandling sh;
} // namespace backward
#endif

#include "argh.h"
#include "game.h"
#include "preload.h"
#include "settings.h"
#include "systems/ExampleScreenRegistry.h"
#include "systems/screens/AIMChatDemo.h"
#include "systems/screens/AngryBirdsSettings.h"
#include "systems/screens/AutoTextColorShowcase.h"
#include "systems/screens/Buttons.h"
#include "systems/screens/Cards.h"
#include "systems/screens/CasualSettings.h"
#include "systems/screens/CheckboxShowcase.h"
#include "systems/screens/CircularProgressShowcase.h"
#include "systems/screens/CozyCafe.h"
#include "systems/screens/DeadSpaceSettings.h"
#include "systems/screens/DecorativeFrameShowcase.h"
#include "systems/screens/EmpireTycoon.h"
#include "systems/screens/ExampleAccessibility.h"
#include "systems/screens/ExampleBevelBorders.h"
#include "systems/screens/ExampleBorders.h"
#include "systems/screens/ExampleColors.h"
#include "systems/screens/ExampleFlexAlignment.h"
#include "systems/screens/ExampleLayout.h"
#include "systems/screens/ExampleNineSliceBorders.h"
#include "systems/screens/ExampleSeparators.h"
#include "systems/screens/ExampleSimpleButton.h"
#include "systems/screens/ExampleTabbing.h"
#include "systems/screens/ExampleText.h"
#include "systems/screens/ExampleTextOverflow.h"
#include "systems/screens/ExampleTextShadow.h"
#include "systems/screens/ExampleTextStroke.h"
#include "systems/screens/FighterMenu.h"
#include "systems/screens/FlightOptions.h"
#include "systems/screens/Forms.h"
#include "systems/screens/ImageShowcase.h"
#include "systems/screens/IslandsTrainsSettings.h"
#include "systems/screens/KirbyOptions.h"
#include "systems/screens/LanguageDemo.h"
#include "systems/screens/MiniMotorwaysSettings.h"
#include "systems/screens/NavigationBarShowcase.h"
#include "systems/screens/NeonStrike.h"
#include "systems/screens/ParcelCorpsSettings.h"
#include "systems/screens/PowerWashSettings.h"
#include "systems/screens/RubberBanditsMenu.h"
#include "systems/screens/SettingRowShowcase.h"
#include "systems/screens/SportsSettings.h"
#include "systems/screens/TextInputDemo.h"
#include "systems/screens/Themes.h"
#include "systems/screens/ToggleSwitchShowcase.h"
#include "systems/screens/ModalShowcase.h"
#include "systems/screens/PaginationShowcase.h"
#include "systems/screens/RadioGroupShowcase.h"
#include "systems/screens/ScrollViewShowcase.h"
#include "systems/screens/SelfAlignShowcase.h"
#include "systems/screens/TabContainerShowcase.h"
#include "systems/screens/ToastShowcase.h"
#include "testing/e2e_integration.h"
#include "testing/test_macros.h"
#include "testing/tests/all_tests.h"
#include <cstdio>
#include <iostream>

#ifdef AFTER_HOURS_ENABLE_MCP
#include "engine/input_injector.h"
#include <afterhours/src/plugins/mcp_server.h>
#ifndef _WIN32
#include <unistd.h>
#endif
#endif

using namespace afterhours;

#ifdef AFTER_HOURS_ENABLE_MCP
bool g_mcp_mode = false;
int g_saved_stdout_fd = -1; // Used by MCP to write JSON to original stdout
#endif

int main(int argc, char *argv[]) {
  argh::parser cmdl(argc, argv, argh::parser::PREFER_PARAM_FOR_UNREG_OPTION);

#ifdef AFTER_HOURS_ENABLE_MCP
  // Check for MCP mode EARLY and redirect stdout to stderr to keep stdout clean
  // for JSON protocol
  if (cmdl["--mcp"]) {
    g_mcp_mode = true;
#ifndef _WIN32
    // Save original stdout and redirect it to stderr
    // MCP will use write() directly to the saved fd
    g_saved_stdout_fd = dup(STDOUT_FILENO);
    dup2(STDERR_FILENO, STDOUT_FILENO);
#endif
  }
#endif

  if (cmdl["--help"]) {
    std::cout << "UI Tester \n\n";
    std::cout << "Usage: ui_tester [OPTIONS]\n\n";
    std::cout << "Options:\n";
    std::cout << "  --help                        Show this help message\n";
    std::cout
        << "  -w, --width <pixels>          Screen width (default: 1280)\n";
    std::cout
        << "  -h, --height <pixels>         Screen height (default: 720)\n";
    std::cout << "  --list-tests                 List all available tests\n";
    std::cout << "  --run-test <name>            Run a specific test\n";
    std::cout
        << "  --slow                       Run test in slow mode (visible)\n";
    std::cout << "  --hold-on-end                Keep window open after test "
                 "finishes\n";
    std::cout << "  --list-screens               List all available example "
                 "screens\n";
    std::cout << "  --screen=<name>              Show example screen (e.g., "
                 "--screen=simple_button)\n";
    std::cout << "                               Navigation: , (prev) . (next) "
                 "PageUp/PageDown\n";
#ifdef AFTER_HOURS_ENABLE_MCP
    std::cout << "  --mcp                        Enable MCP server mode\n";
#endif
    std::cout << "\nE2E Testing:\n";
    std::cout << "  --e2e                        Enable E2E test mode\n";
    std::cout << "  --test-script <path>         Run single E2E script\n";
    std::cout << "  --test-script-dir <path>     Run all .e2e scripts in directory\n";
    std::cout << "  --timeout <seconds>          Set E2E timeout (default: 30)\n";
    std::cout << "  --slow                       Run tests slowly for visibility (0.5s delay)\n";
    std::cout << "  --slow-delay <seconds>       Set slow mode delay (implies --slow)\n";
    std::cout << "  --update-baselines           Update baseline screenshots instead of comparing\n";
    return 0;
  }

  if (cmdl["--list-tests"]) {
    TestRegistry &registry = TestRegistry::get();
    std::cout << "Available tests:\n";
    for (const auto &[name, func] : registry.tests) {
      std::cout << "  - " << name << "\n";
    }
    return 0;
  }

  if (cmdl["--list-screens"]) {
    ExampleScreenRegistry::get().list_screens();
    return 0;
  }

  // E2E Testing Mode
  if (e2e::should_run_e2e(argc, argv)) {
    auto e2e_args = e2e::parse_e2e_args(argc, argv);
    
    if (e2e_args.script_path.empty() && e2e_args.script_dir.empty()) {
      std::cout << "E2E mode requires --test-script or --test-script-dir\n";
      return 1;
    }

    int screenWidth, screenHeight;
    cmdl({"-w", "--width"}, 1280) >> screenWidth;
    cmdl({"-h", "--height"}, 720) >> screenHeight;

    Settings::get().load_save_file(screenWidth, screenHeight);

    Preload::get()
        .init("UI Tester - E2E Mode")
        .make_singleton();
    Settings::get().refresh_settings();

    // Set up test mode
    afterhours::testing::test_input::detail::test_mode = true;

    // Create and configure runner
    afterhours::testing::E2ERunner runner;
    runner.set_timeout(e2e_args.timeout_seconds);
    if (e2e_args.slow_mode) {
      runner.set_slow_mode(true, e2e_args.slow_delay);
    }
    runner.set_reset_callback(reset_e2e_state);

    // Load scripts
    if (!e2e_args.script_path.empty()) {
      runner.load_script(e2e_args.script_path);
    } else if (!e2e_args.script_dir.empty()) {
      runner.load_scripts_from_directory(e2e_args.script_dir);
    }

    if (!runner.has_commands()) {
      std::cout << "No E2E commands found to execute\n";
      return 1;
    }

    std::cout << "Running E2E tests...\n";

    // Always run in visual mode - headless mode disabled for debugging
    // TODO: Re-enable headless mode for CI once debugging is complete
    // Original condition: if (e2e_args.slow_mode)
    return run_e2e_tests(e2e_args, runner);
  }

  // Try --screen=<name> format first (preferred)
  std::string screen_name;
  cmdl({"--screen"}) >> screen_name;

  // Fallback: support legacy --<name> format for backwards compatibility
  if (screen_name.empty()) {
    for (const auto &arg : cmdl.flags()) {
      // argh returns flags without the "--" prefix
      if (arg != "help" && arg != "list-tests" && arg != "list-screens" &&
          arg != "slow" && arg != "hold-on-end" && arg != "run-test" &&
          arg != "mcp" && arg != "screen") {
        // Remove "--" prefix if present (in case it's there)
        if (arg.size() >= 2 && arg[0] == '-' && arg[1] == '-') {
          screen_name = arg.substr(2);
        } else {
          screen_name = arg;
        }
        break;
      }
    }
  }

  if (!screen_name.empty()) {
    if (ExampleScreenRegistry::get().has_screen(screen_name)) {
      int screenWidth, screenHeight;
      cmdl({"-w", "--width"}, 1280) >> screenWidth;
      cmdl({"-h", "--height"}, 720) >> screenHeight;

      Settings::get().load_save_file(screenWidth, screenHeight);

      Preload::get() //
          .init("UI Tester")
          .make_singleton();
      Settings::get().refresh_settings();

      bool hold_on_end = cmdl["--hold-on-end"];

      run_screen_demo(screen_name, hold_on_end);

      Settings::get().write_save_file();

      return 0;
    } else {
      std::cout << "Unknown screen: " << screen_name << "\n";
      std::cout << "Use --list-screens to see available screens\n";
      return 1;
    }
  }

  std::string test_name;
  if (cmdl({"--run-test"}) >> test_name) {
    bool slow_mode = cmdl["--slow"];
    bool hold_on_end = cmdl["--hold-on-end"];

    int screenWidth, screenHeight;
    cmdl({"-w", "--width"}, 1280) >> screenWidth;
    cmdl({"-h", "--height"}, 720) >> screenHeight;

    Settings::get().load_save_file(screenWidth, screenHeight);

    Preload::get() //
        .init("UI Tester")
        .make_singleton();
    Settings::get().refresh_settings();

    run_test(test_name, slow_mode, hold_on_end);

    Settings::get().write_save_file();

    return 0;
  }

  std::vector<std::string> screen_names =
      ExampleScreenRegistry::get().get_screen_names();
  if (screen_names.empty()) {
    std::cout << "No screens available. Run with --list-screens to see "
                 "available screens.\n";
    return 1;
  }

  int screenWidth, screenHeight;
  cmdl({"-w", "--width"}, 1280) >> screenWidth;
  cmdl({"-h", "--height"}, 720) >> screenHeight;

  Settings::get().load_save_file(screenWidth, screenHeight);

  Preload::get() //
      .init("UI Tester")
      .make_singleton();
  Settings::get().refresh_settings();

  bool hold_on_end = cmdl["--hold-on-end"];
  run_screen_demo(screen_names[0], hold_on_end);

  Settings::get().write_save_file();

  return 0;
}
