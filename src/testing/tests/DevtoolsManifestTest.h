#pragma once
// Compile + registry smoke for the Batch P-C devtools manifest layer.
// Pure checks only: no ECS, no raylib, no filesystem, so the test is
// deterministic in headless --run-all-tests.

#include "../test_macros.h"
#include "../../devtools/wm_devtools.h"

#include <stdexcept>
#include <string>

inline void devtools_assert(bool condition, const std::string &message) {
  if (!condition) {
    throw std::runtime_error(message);
  }
}

// The manifest names exactly the 11 existing devtools surfaces, all
// WM-local, all complete, and the two exclusion-sensitive ones
// (apply_blur_pass, font_config) are present with precise gates.
TEST(devtools_manifest_registry) {
  using namespace wm::devtools;

  devtools_assert(descriptor_count() == 11,
                  "devtools manifest must list 11 surfaces");
  devtools_assert(count_by_owner(Owner::WmLocal) == 11,
                  "all devtools surfaces are WM-local in this batch");
  devtools_assert(descriptors_complete(),
                  "every descriptor needs id/header/entry/gate/test");

  for (const char *id : {"headless_screenshots", "screenshot_validation",
                         "layout_summary", "ui_tree_dump", "focus_audit",
                         "frame_stats", "profiler_overlay",
                         "e2e_integration", "e2e_commands",
                         "apply_blur_pass", "font_config"}) {
    devtools_assert(find(id) != nullptr, std::string("missing plugin: ") + id);
    const auto *d = find(id);
    devtools_assert(!d->promotion_gate.empty(),
                    std::string("empty promotion gate: ") + id);
    devtools_assert(!d->test_entry.empty(),
                    std::string("empty test entry: ") + id);
  }
  devtools_assert(find("no_such_plugin") == nullptr,
                  "find() must return nullptr for unknown ids");

  // ApplyBlurPass + font_config are manifest-only in Batch P-C with
  // precise, checkable promotion gates.
  const auto *blur = find("apply_blur_pass");
  devtools_assert(blur->header == "src/plugins/render_passes/ApplyBlurPass.h",
                  "apply_blur_pass header path");
  devtools_assert(blur->owner == Owner::WmLocal, "apply_blur_pass is WM-local");
  const auto *font = find("font_config");
  devtools_assert(font->header == "src/plugins/font_registry/font_config.h",
                  "font_config header path");

  // The aggregate compatibility include is the point of this layer:
  // these names must be visible through wm_devtools.h without moving
  // implementation. FrameStats + the font registry come from it.
  FrameStats stats;
  stats.push(0.016f);
  devtools_assert(stats.frame == 1, "FrameStats::push counts one frame");
  devtools_assert(stats.p95Ms == 0.f,
                  "FrameStats stats recompute only every 15 frames");
  const auto &fonts = font_config::get_all_fonts();
  devtools_assert(!fonts.empty(), "font_config registry must not be empty");
  bool has_atkinson = false;
  for (const auto &def : fonts)
    if (def.name == "AtkinsonMock")
      has_atkinson = true;
  devtools_assert(has_atkinson,
                  "font_config registry must contain AtkinsonMock");

  co_return;
}
