#include "../src/rolling_number.h"
#include <cassert>

int main() {
  using namespace rolling_number;
  Number n;
  retarget(n, 99, true);
  assert(n.position[4] == 9 && n.position[5] == 9);
  retarget(n, 100);
  assert(n.to[3] == 1 && n.to[4] == 10 && n.to[5] == 10);
  advance(n, duration / 3);
  auto intermediate = n.position;
  retarget(n, 99);
  assert(n.position == intermediate && n.from == intermediate);
  advance(n, duration);
  assert(n.target == 99 && n.position[3] == 0 && n.position[4] == 9 && n.position[5] == 9);
  retarget(n, 100, true);
  retarget(n, 99);
  assert(n.to[3] == 0 && n.to[4] == -1 && n.to[5] == -1);
  advance(n, duration, true);
  retarget(n, 1000000, true);
  for (auto digit : n.position) assert(digit == 9);
  retarget(n, -1, true);
  for (auto digit : n.position) assert(digit == 0);
  retarget(n, 12345);
  advance(n, -.5f);
  for (auto digit : n.position) assert(digit == 0);
  advance(n, 100.f);
  assert((n.position == std::array<double, 6>{0, 1, 2, 3, 4, 5}));
}
