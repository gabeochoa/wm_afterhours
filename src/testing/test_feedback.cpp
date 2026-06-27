#include "test_feedback.h"

namespace test_feedback {

std::deque<Toast> toasts;

void show_toast(const std::string &message, const std::string &color,
                float duration) {
  toasts.push_back({message, duration, 0.0f, color});
  if (toasts.size() > 5) {
    toasts.pop_front();
  }
}

void update_toasts(float dt) {
  for (auto it = toasts.begin(); it != toasts.end();) {
    it->time_alive += dt;
    if (it->time_alive >= it->duration) {
      it = toasts.erase(it);
    } else {
      ++it;
    }
  }
}

} // namespace test_feedback
