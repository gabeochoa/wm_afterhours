#pragma once
// Focused pluginization test for Batch 3 Lane G (src/devtools/layout_audit/).
//
// Pure compile + entry-point presence checks only: no ECS run, no
// UIContext, no raylib, no filesystem writes, so the test is deterministic
// in headless --run-all-tests. It proves the canonical headers (the plugin's
// source of truth) compile under the real include graph and that every
// public entry point remains unchanged after relocation; behavior and JSON
// output are proven by the layout/screenshot smoke commands in the PR
// test plan (this test deliberately does not execute layout_summary or
// focus_audit against a live UI collection).

#include "../../devtools/layout_audit/focus_audit.h"
#include "../../devtools/layout_audit/layout_summary.h"
#include "../../devtools/layout_audit/ui_tree_dump.h"
// The old shim headers must keep resolving to the same declarations.
#include "../focus_audit.h"
#include "../layout_summary.h"
#include "../ui_tree_dump.h"

#include "../test_macros.h"

#include <stdexcept>
#include <string>

namespace layout_audit_test {

inline void la_assert(bool condition, const std::string &message) {
  if (!condition) {
    throw std::runtime_error(message);
  }
}

} // namespace layout_audit_test

TEST(layout_audit_canonical_entry_points) {
  using namespace layout_audit_test;

  // Canonical + shim headers must declare identical entry points. Taking
  // addresses proves the declarations compiled, not just that the header
  // names exist as macros; calling them against a live UI collection is
  // headless-smoke territory, not this pure test.
  la_assert(&layout_summary::write != nullptr,
            "layout_summary::write must exist");
  la_assert(&layout_summary::write_to_file != nullptr,
            "layout_summary::write_to_file must exist");

  la_assert(&ui_tree_dump::build != nullptr, "ui_tree_dump::build must exist");
  la_assert(&ui_tree_dump::build_all != nullptr,
            "ui_tree_dump::build_all must exist");
  la_assert(&ui_tree_dump::size_json != nullptr,
            "ui_tree_dump::size_json must exist");

  la_assert(&focus_audit::snapshot != nullptr,
            "focus_audit::snapshot must exist");
  la_assert(&focus_audit::write != nullptr, "focus_audit::write must exist");
  la_assert(&focus_audit::capture_crop != nullptr,
            "focus_audit::capture_crop must exist");
  la_assert(&focus_audit::register_systems != nullptr,
            "focus_audit::register_systems must exist");

  co_return;
}
