#pragma once

// Local WM plugin: RollingNumber (Batch P-A, WM-local only).
//
// Canonical implementation for the rolling-odometer utility. The historical
// path src/rolling_number.h is now a compatibility shim that includes this
// header, so existing consumers (RollingNumberLab, tests/rolling_number_test.cpp)
// keep working unchanged. Behavior is unchanged: six digits, ease-out-cubic
// roll over `duration`, clamped to [0, maximum], and reduced-motion aware
// (retarget/advance with reduced_motion=true settle immediately).
//
// Promotion gate: future candidate for afterhours only after this plugin has
// held stable across the local test suite + targeted e2e; no afterhours changes
// in this batch. See README.md in this directory.

#include <afterhours/src/plugins/animation.h>
#include <algorithm>
#include <array>
#include <cmath>

namespace rolling_number {

constexpr float duration = .65f;
constexpr int maximum = 999999;

struct Number {
  std::array<afterhours::motion::Track<float>, 6> tracks{};
  std::array<double, 6> position{};
  std::array<double, 6> from{};
  std::array<double, 6> to{};
  int target = 0;
  float elapsed = duration;
};

inline double wrap(double position) {
  return position - 10.0 * std::floor(position / 10.0);
}

inline void sync(Number &number) {
  for (std::size_t i = 0; i < number.tracks.size(); ++i)
    number.position[i] = number.tracks[i].value();
  float furthest = duration;
  for (const auto &tr : number.tracks)
    if (tr.active()) furthest = std::min(furthest, tr.elapsed());
  number.elapsed = furthest;
}

inline void settle(Number &number) {
  int value = number.target;
  for (std::size_t i = number.tracks.size(); i-- > 0;) {
    number.tracks[i].from(static_cast<float>(value % 10));
    value /= 10;
  }
  sync(number);
  number.from = number.to = number.position;
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
  const afterhours::motion::Timeline roll{.keys = {{0.f, 0.f}, {duration, 1.f}},
                                          .curve = afterhours::motion::curves::ease_out_cubic};
  number.from = number.position;
  for (std::size_t i = number.tracks.size(); i-- > 0;) {
    const double digit = value % 10;
    value /= 10;
    const double current = wrap(number.position[i]);
    double distance = digit - current;
    if (increasing && distance < 0) distance += 10;
    if (!increasing && distance > 0) distance -= 10;
    number.to[i] = number.position[i] + distance;
    number.tracks[i].to(static_cast<float>(number.to[i]), roll);
  }
  number.elapsed = 0;
}

inline void advance(Number &number, float dt, bool reduced_motion = false) {
  if (reduced_motion) {
    settle(number);
    return;
  }
  for (auto &tr : number.tracks) tr.advance(std::max(0.f, dt));
  sync(number);
}

// Plugin facade: a named, versioned entry point that re-exports the local
// API under rolling_number::plugin:: so future consumers (and a possible
// afterhours promotion) have one stable surface to depend on.
namespace plugin {

using ::rolling_number::Number;
using ::rolling_number::advance;
using ::rolling_number::retarget;
using ::rolling_number::settle;
using ::rolling_number::sync;
using ::rolling_number::wrap;

struct Descriptor {
  const char *name = "rolling_number";
  const char *version = "1.0.0";
  // Six digits (Number::tracks size). Kept as a literal so the descriptor
  // stays constexpr without pulling Track construction into a constant
  // expression.
  int digit_count = 6;
  float roll_duration = duration;
  int max_value = maximum;
  // True: retarget/advance honor reduced_motion (immediate settle).
  bool reduced_motion_aware = true;
};

inline constexpr Descriptor descriptor{};

} // namespace plugin

} // namespace rolling_number
