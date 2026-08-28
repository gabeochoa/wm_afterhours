#pragma once

#include "external.h"
#include "rl.h"

// Forward declarations
namespace afterhours { namespace testing { struct E2EArgs; } }
namespace e2e {
using E2EArgs = afterhours::testing::E2EArgs;
}
namespace afterhours::testing {
class E2ERunner;
}

void game();
void run_test(const std::string &test_name, bool slow_mode = false,
              bool hold_on_end = false);
void run_screen_demo(const std::string &screen_name, bool hold_on_end = false);
int run_e2e_tests(const e2e::E2EArgs &args,
                  afterhours::testing::E2ERunner &runner);
void reset_e2e_state();
