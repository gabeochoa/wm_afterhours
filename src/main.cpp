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
#include "systems/ExampleColors.h"
#include "systems/ExampleLayout.h"
#include "systems/ExampleScreenRegistry.h"
#include "systems/ExampleSimpleButton.h"
#include "systems/ExampleTabbing.h"
#include "systems/ExampleText.h"
#include "testing/test_macros.h"
#include "testing/tests/all_tests.h"
#include <iostream>

using namespace afterhours;

int main(int argc, char *argv[]) {
  argh::parser cmdl(argc, argv, argh::parser::PREFER_PARAM_FOR_UNREG_OPTION);

  if (cmdl["--help"]) {
    std::cout << "Break Ross \n\n";
    std::cout << "Usage: break_ross [OPTIONS]\n\n";
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
    std::cout << "  --<screen_name>              Show example screen (e.g., "
                 "--simple_button)\n";
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

  std::string screen_name;
  for (const auto &arg : cmdl.flags()) {
    // argh returns flags without the "--" prefix
    if (arg != "help" && arg != "list-tests" && arg != "list-screens" &&
        arg != "slow" && arg != "hold-on-end" && arg != "run-test") {
      // Remove "--" prefix if present (in case it's there)
      if (arg.size() >= 2 && arg[0] == '-' && arg[1] == '-') {
        screen_name = arg.substr(2);
      } else {
        screen_name = arg;
      }
      break;
    }
  }

  if (!screen_name.empty()) {
    if (ExampleScreenRegistry::get().has_screen(screen_name)) {
      int screenWidth, screenHeight;
      cmdl({"-w", "--width"}, 1280) >> screenWidth;
      cmdl({"-h", "--height"}, 720) >> screenHeight;

      Settings::get().load_save_file(screenWidth, screenHeight);

      Preload::get() //
          .init("Break Ross")
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
        .init("Break Ross")
        .make_singleton();
    Settings::get().refresh_settings();

    run_test(test_name, slow_mode, hold_on_end);

    Settings::get().write_save_file();

    return 0;
  }

  int screenWidth, screenHeight;
  cmdl({"-w", "--width"}, 1280) >> screenWidth;
  cmdl({"-h", "--height"}, 720) >> screenHeight;

  Settings::get().load_save_file(screenWidth, screenHeight);

  Preload::get() //
      .init("Break Ross")
      .make_singleton();
  Settings::get().refresh_settings();

  game();

  Settings::get().write_save_file();

  return 0;
}
