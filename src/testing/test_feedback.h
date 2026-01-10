#pragma once

#include <deque>
#include <string>
#include <vector>

namespace test_feedback {

struct Toast {
  std::string message;
  float duration;
  float time_alive;
  std::string color; // "info", "success", "error"
};

extern std::deque<Toast> toasts;

void show_toast(const std::string &message, const std::string &color = "info",
                float duration = 2.0f);
void update_toasts(float dt);

} // namespace test_feedback
