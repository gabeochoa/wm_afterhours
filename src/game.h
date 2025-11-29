#pragma once

#include "external.h"
#include "rl.h"

void game();
void run_test(const std::string &test_name, bool slow_mode = false,
              bool hold_on_end = false);
void run_screen_demo(const std::string &screen_name, bool hold_on_end = false);
