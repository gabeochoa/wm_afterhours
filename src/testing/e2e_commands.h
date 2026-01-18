// App-specific E2E Command Handlers
// Screen navigation commands specific to this application
#pragma once

#include "../systems/ExampleScreenRegistry.h"

#include <afterhours/src/plugins/e2e_testing/e2e_testing.h>

namespace e2e_commands {

using namespace afterhours;

// Handle 'open_screen screen_name' - navigates to a specific screen
template <typename ScreenManager>
struct HandleOpenScreenCommand : System<testing::PendingE2ECommand> {
  ScreenManager *screen_manager = nullptr;

  virtual void for_each_with(Entity &, testing::PendingE2ECommand &cmd,
                             float) override {
    if (cmd.is_consumed() || !cmd.is("open_screen"))
      return;
    if (!cmd.has_args(1)) {
      cmd.fail("open_screen requires screen name");
      return;
    }
    if (!screen_manager) {
      cmd.fail("screen_manager not set");
      return;
    }

    // Try to find and open the screen by name
    const auto &name = cmd.arg(0);
    if (screen_manager->open_screen(name)) {
      cmd.consume();
    } else {
      cmd.fail("Screen not found: " + name);
    }
  }
};

// Handle 'next_screen' - goes to next screen in showcase
struct HandleNextScreenCommand : System<testing::PendingE2ECommand> {
  virtual void for_each_with(Entity &, testing::PendingE2ECommand &cmd,
                             float) override {
    if (cmd.is_consumed() || !cmd.is("next_screen"))
      return;

    testing::test_input::push_key(keys::PAGE_DOWN);
    cmd.consume();
  }
};

// Handle 'prev_screen' - goes to previous screen in showcase
struct HandlePrevScreenCommand : System<testing::PendingE2ECommand> {
  virtual void for_each_with(Entity &, testing::PendingE2ECommand &cmd,
                             float) override {
    if (cmd.is_consumed() || !cmd.is("prev_screen"))
      return;

    testing::test_input::push_key(keys::PAGE_UP);
    cmd.consume();
  }
};

// Handle 'goto_screen screen_name' - navigates to a specific screen by name
struct HandleGotoScreenCommand : System<testing::PendingE2ECommand> {
  std::function<void(const std::string &)> goto_screen_fn;

  virtual void for_each_with(Entity &, testing::PendingE2ECommand &cmd,
                             float) override {
    if (cmd.is_consumed() || !cmd.is("goto_screen"))
      return;
    if (!cmd.has_args(1)) {
      cmd.fail("goto_screen requires screen name");
      return;
    }
    if (!goto_screen_fn) {
      cmd.fail("goto_screen callback not set");
      return;
    }

    goto_screen_fn(cmd.arg(0));
    cmd.consume();
  }
};

// Register app-specific commands
template <typename ScreenManager>
inline void register_app_commands(SystemManager &sm) {
  sm.register_update_system(std::make_unique<HandleNextScreenCommand>());
  sm.register_update_system(std::make_unique<HandlePrevScreenCommand>());
}

} // namespace e2e_commands
