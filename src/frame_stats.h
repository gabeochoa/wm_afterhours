#pragma once

#include <algorithm>
#include <array>
#include <cstddef>

struct FrameStats {
  static constexpr size_t CAPACITY = 120;
  std::array<float, CAPACITY> ring{};
  size_t head = 0;
  size_t count = 0;
  unsigned long frame = 0;
  float fps = 0.f;
  float avgMs = 0.f;
  float p95Ms = 0.f;
  float maxMs = 0.f;

  void push(float dt) {
    if (dt <= 0.f)
      return;
    ring[head] = dt * 1000.f;
    head = (head + 1) % CAPACITY;
    count = std::min(count + 1, CAPACITY);
    ++frame;
    if (frame % 15 != 0)
      return;
    std::array<float, CAPACITY> sorted{};
    std::copy_n(ring.begin(), count, sorted.begin());
    std::sort(sorted.begin(), sorted.begin() + static_cast<long>(count));
    float sum = 0.f;
    for (size_t i = 0; i < count; ++i)
      sum += sorted[i];
    avgMs = count ? sum / static_cast<float>(count) : 0.f;
    p95Ms = count ? sorted[static_cast<size_t>(0.95f * static_cast<float>(count - 1))] : 0.f;
    maxMs = count ? sorted[count - 1] : 0.f;
    fps = avgMs > 0.f ? 1000.f / avgMs : 0.f;
  }
};
