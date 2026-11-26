// Batch P-A plugin API hardening test for the RollingNumber local plugin.
// Covers only the new plugin surface (descriptor + plugin facade); the
// pre-existing behavior suite stays in tests/rolling_number_test.cpp.
#include "../src/plugins/rolling_number/rolling_number.h"
#include <array>
#include <cassert>
#include <string_view>

int main() {
  using namespace rolling_number;

  // Plugin descriptor is stable and self-describing.
  assert(std::string_view(plugin::descriptor.name) == "rolling_number");
  assert(std::string_view(plugin::descriptor.version) == "1.0.0");
  assert(plugin::descriptor.digit_count == 6);
  assert(plugin::descriptor.roll_duration == duration);
  assert(plugin::descriptor.max_value == maximum);
  assert(plugin::descriptor.reduced_motion_aware);

  // Facade re-exports the same implementation.
  plugin::Number alias;
  plugin::retarget(alias, 42, true);
  assert(alias.target == 42);
  assert((alias.position == std::array<double, 6>{0, 0, 0, 0, 4, 2}));

  // wrap() is periodic by 10, including negatives.
  assert(wrap(10.0) == 0.0);
  assert(wrap(19.0) == 9.0);
  assert(wrap(-1.0) == 9.0);
  assert(wrap(9.5) == 9.5);

  // Reduced motion settles immediately; animated path still lands exactly on
  // the target after a full duration.
  plugin::Number n;
  plugin::retarget(n, 7, true);
  assert(n.position[5] == 7);
  plugin::retarget(n, 8);
  assert(n.elapsed == 0);
  plugin::advance(n, duration);
  assert(n.position[5] == 8);

  // Clamping at both ends, through the facade.
  plugin::retarget(n, 9999999, true);
  assert(n.target == maximum);
  for (auto digit : n.position) assert(digit == 9);
  plugin::retarget(n, -5, true);
  assert(n.target == 0);
  for (auto digit : n.position) assert(digit == 0);
}
