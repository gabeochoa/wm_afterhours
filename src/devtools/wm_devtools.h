#pragma once
// Aggregate compatibility header for the WM devtools plugin package.
//
// Thin wrappers only: each include below is the verbatim public header for
// one manifest entry. No implementation is moved, no call site is
// rewired, and including this header changes no runtime behavior (it only
// makes the existing headers reachable through one plugin entry point).
// If any include stops compiling cleanly, drop that line and keep the
// manifest entry -- the manifest is the source of truth, this file is
// convenience.
//
// Registry: see "devtools_manifest.h" (namespace wm::devtools).

#include "devtools_manifest.h" // registry (stdlib-only)

#include "../headless_screenshots.h" // headless_screenshots
#include "../testing/screenshot_validation.h" // screenshot_validation
#include "../testing/layout_summary.h" // layout_summary
#include "../testing/ui_tree_dump.h" // ui_tree_dump
#include "../testing/focus_audit.h" // focus_audit
#include "../frame_stats.h" // frame_stats
#include "../systems/ProfilerOverlay.h" // profiler_overlay
#include "../testing/e2e_integration.h" // e2e_integration
#include "../testing/e2e_commands.h" // e2e_commands
#include "../systems/ApplyBlurPass.h" // apply_blur_pass
#include "../font_config.h" // font_config
