#pragma once

#include <algorithm>
#include <array>
#include <cmath>

namespace rolling_number {

constexpr float duration = .65f;
constexpr int maximum = 999999;

struct Number {
  std::array<double, 6> position{};
  std::array<double, 6> from{};
  std::array<double, 6> to{};
  int target = 0;
  float elapsed = duration;
};

inline double wrap(double position) {
  return position - 10.0 * std::floor(position / 10.0);
}

inline void settle(Number &number) {
  int value = number.target;
  for (std::size_t i = number.position.size(); i-- > 0;) {
    number.position[i] = value % 10;
    value /= 10;
  }
  number.from = number.to = number.position;
  number.elapsed = duration;
}

inline void retarget(Number &number, int value, bool reduced_motion = false) {
  value = std::clamp(value, 0, maximum);
  if (value == number.target) return;
  const bool increasing = value > number.target;
  number.target = value;
  if (reduced_motion) {
    settle(number);
    return;
  }
  number.from = number.position;
  for (std::size_t i = number.position.size(); i-- > 0;) {
    const double digit = value % 10;
    value /= 10;
    const double current = wrap(number.position[i]);
    double distance = digit - current;
    if (increasing && distance < 0) distance += 10;
    if (!increasing && distance > 0) distance -= 10;
    number.to[i] = number.position[i] + distance;
  }
  number.elapsed = 0;
}

inline void advance(Number &number, float dt, bool reduced_motion = false) {
  if (reduced_motion || number.elapsed + std::max(0.f, dt) >= duration) {
    settle(number);
    return;
  }
  number.elapsed += std::max(0.f, dt);
  const double remaining = 1.0 - number.elapsed / duration;
  const double progress = 1.0 - remaining * remaining * remaining;
  for (std::size_t i = 0; i < number.position.size(); ++i)
    number.position[i] = number.from[i] + (number.to[i] - number.from[i]) * progress;
}

}
