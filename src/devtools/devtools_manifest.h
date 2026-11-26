#pragma once
// WM devtools plugin manifest (Batch P-C, additive layer).
//
// Names each existing WM-local devtools surface, its public entry points,
// ownership (WM-local vs AH upstream), the precise promotion gate for
// upstreaming, and the test entry point that covers it. Runtime behavior is
// unchanged by this header: it is a registry only, with no registration
// side effects, no ECS work, and no rendering. Consumers that want the
// implementation headers should include "wm_devtools.h" (the aggregate
// compatibility include); consumers that only need the registry should
// include this header, which pulls in only the standard library.

#include <array>
#include <optional>
#include <string_view>

namespace wm::devtools {

enum class Owner : unsigned char { WmLocal = 0, AfterhoursUpstream = 1 };

struct PluginDescriptor {
  std::string_view id; // stable plugin id
  std::string_view title; // human title
  std::string_view header; // repo-relative public header
  std::string_view impl; // repo-relative implementation (may equal header)
  Owner owner = Owner::WmLocal;
  std::string_view public_entry; // primary public entry points
  std::string_view promotion_gate; // precise, checkable promotion criteria
  std::string_view test_entry; // test entry point that covers this surface
  std::string_view notes; // overlap / overlap-with-upstream note
};

// Single source of truth for the WM devtools plugin package. This list is
// also mirrored in src/devtools/README.md; if they ever disagree, this
// header wins (and the test in DevtoolsManifestTest.h fails until the
// README is re-aligned).
inline constexpr std::array<PluginDescriptor, 11> kDescriptors{{
    {"headless_screenshots", "Headless screenshots + headless test runner",
     "src/headless_screenshots.h", "src/headless_screenshots.cpp",
     Owner::WmLocal,
     "run_headless_screenshots(), run_all_tests_headless(), "
     "run_focus_ring_test(), run_layout_summary()",
     "Promote only after multi-resolution stability gate passes: 480p/720p/"
     "1080p captures are byte-stable across 3 consecutive headless runs on "
     "one machine (see deferred item). Until then WM-local.",
     "./output/ui_tester.exe --headless-screenshots; "
     "PKG_CONFIG_PATH=$HOME/homebrew/lib/pkgconfig make -j4 && "
     "./output/ui_tester.exe --run-all-tests",
     "AH has graphics headless mode + e2e harness, but WM owns the "
     "screen-registry sweep, multi-resolution loop, and coroutine runner."},
    {"screenshot_validation", "Screenshot baseline validation",
     "src/testing/screenshot_validation.h",
     "src/testing/screenshot_validation.cpp", Owner::WmLocal,
     "screenshot_validation::set_update_baselines(), "
     "calculate_image_diff_percentage(), validate_screen_against_baseline()",
     "Promote only after threshold parity with AH: WM DIFF_THRESHOLD_PCT "
     "(0.05% changed-pixel share, >8/255 channel delta) must be reconciled "
     "with AH screenshot_baseline.h semantics and the baseline-drift policy "
     "(refresh only missing/failing) documented in this file.",
     "./output/ui_tester.exe --run-all-tests; make validate-screenshots",
     "Vendor overlap: afterhours/src/plugins/e2e_testing/"
     "screenshot_baseline.h + screenshot capture in harness.h/runner.h."},
    {"layout_summary", "Flat layout summary (named-element bounds)",
     "src/testing/layout_summary.h", "src/testing/layout_summary.cpp",
     Owner::WmLocal,
     "layout_summary::write(std::ostream&), write_to_file()",
     "DO NOT promote until layout-intent API is upstream: AH must expose the "
     "intent query (size specs, flex direction, justify/align, gap) that "
     "ui_tree_dump already emits. Geometry-only replay is not sufficient.",
     "./output/ui_tester.exe --layout-summary <screen>",
     "Depends on AH ui_commands::ui_query + get_screen_rect (already "
     "upstream); the WM part is the flat named-element print."},
    {"ui_tree_dump", "JSON UI tree dump with layout intent",
     "src/testing/ui_tree_dump.h", "header-only", Owner::WmLocal,
     "ui_tree_dump::build(), size_json()",
     "DO NOT promote until layout-intent API is upstream (see "
     "layout_summary): desired/min/max specs, flex_direction, "
     "justify_content, align_items, gap, font resolution must be reachable "
     "through an AH query, not by reading UIComponent fields directly.",
     "./output/ui_tester.exe --layout-summary <screen>; --dump-ui-json",
     "Header-only by design (inline JSON builders); the intent half is the "
     "point (lets a CSS layout engine independently solve the same input)."},
    {"focus_audit", "Focus geometry / focus-ring audit",
     "src/testing/focus_audit.h", "src/testing/focus_audit.cpp",
     Owner::WmLocal,
     "focus_audit::snapshot(), write(), capture_crop(), register_systems(); "
     "E2E command audit_ui",
     "Promote only after E2E runner owns the audit flag: AH ui_commands "
     "focus support + runner --focus-audit must drive HandleAudit; until "
     "then the audit_ui handler stays WM-local.",
     "./output/ui_tester.exe --focus-test --focus-audit; --run-all-tests",
     "Uses AH focus/clip/menu/tooltip systems; WM owns the snapshot/crop "
     "format and the audit_ui command."},
    {"frame_stats", "Per-frame stats ring (fps/avg/p95/max)",
     "src/frame_stats.h", "header-only", Owner::WmLocal,
     "FrameStats::push(dt)",
     "Promote only if AH profiling Snapshot (avg/p95/p99/fps in "
     "profiling.h) provably matches WM ring math (120-frame ring, recompute "
     "every 15 frames) on the same trace; replace only after parity test. "
     "Until then WM-local.",
     "./output/ui_tester.exe --run-all-tests (FrameStats sanity in "
     "DevtoolsManifestTest.h)",
     "Vendor overlap: afterhours/src/plugins/profiling.h Snapshot + "
     "select_frames(). WM ring is standalone and dependency-free."},
    {"profiler_overlay", "In-game profiler overlay chrome",
     "src/systems/ProfilerOverlay.h", "header-only", Owner::WmLocal,
     "ProfilerOverlay (System<UIContext<InputAction>>)",
     "Promote only if AH ships chrome parity: vendor ui/profiler.h "
     "profiler_panel + profiling.h default_collector are already upstream; "
     "WM overlay stays until AH provides the wm_profiler input-gate, "
     "refresh-rate cycle (every frame/120/60/30 Hz), and input-prompt "
     "shortcut hint.",
     "./output/ui_tester.exe --profile (F3 panel); --run-all-tests",
     "Thin WM chrome over upstream profiling collector + profiler_panel."},
    {"e2e_integration", "E2E system registration shim",
     "src/testing/e2e_integration.h", "header-only", Owner::WmLocal,
     "e2e::register_e2e_systems(), configure_runner(); re-exports "
     "parse_e2e_args/should_run_e2e/count_e2e_scripts/screenshot_path",
     "Promote only after AH e2e_testing exposes an app-command "
     "registration hook; until then this shim (which wires screenshot + "
     "reset handlers and re-exports AH arg parsing) stays WM-local.",
     "make test (tests/e2e_scripts/*.e2e headless, --time-scale 4)",
     "Upstream owns runner + arg parsing + screenshot paths; this file "
     "wires WM callbacks into it."},
    {"e2e_commands", "App-specific E2E commands",
     "src/testing/e2e_commands.h", "header-only", Owner::WmLocal,
     "HandleOpenScreenCommand, HandleNextScreenCommand, "
     "HandlePrevScreenCommand, HandleGotoScreenCommand, "
     "HandleValidateScreenCommand",
     "Promote only after AH screen_commands.h covers WM's screen registry; "
     "open_screen/goto_screen depend on ExampleScreenRegistry, so this "
     "likely stays WM-local permanently. validate_screen couples to "
     "screenshot_validation (see that gate).",
     "make test; .e2e scripts using open_screen/goto_screen/validate_screen",
     "Vendor overlap: afterhours/src/plugins/e2e_testing/screen_commands.h "
     "is the upstream counterpart for generic screen commands."},
    {"apply_blur_pass", "Blur post-process pass (screen-space)",
     "src/systems/ApplyBlurPass.h", "header-only", Owner::WmLocal,
     "ApplyBlurPass::once(float); consumes ui::blur_requests()",
     "PRECISE: promote to AH effects.h only when AH BlurPass exposes (a) "
     "the direct-to-window scale path (scale = GetRenderWidth() / "
     "GetScreenWidth(), screen_px from render size) AND (b) the mainRT "
     "render-texture path parity, with blur_requests() queue ownership "
     "moved upstream. Do not refactor call sites until both paths "
     "pixel-match (visual diff <= 0.05% on the blur screen). Until then "
     "manifest-only: no call-site changes.",
     "Headless screenshot of the blur screen; make validate-screenshots; "
     "--run-all-tests",
     "Uses AH effects::BlurPass; WM owns the two-path dispatch "
     "(draw_directly_to_window vs mainRT). Intentionally not refactored "
     "in Batch P-C."},
    {"font_config", "WM font registry + CJK codepoint tables",
     "src/font_config.h", "header-only", Owner::WmLocal,
     "font_config::get_all_fonts(), get_*_codepoints(), aliases; "
     "FontDef{name, filename, needs_codepoints, get_codepoints, "
     "raster_size, screens}",
     "PRECISE: promote to AH font_catalog/font_coverage only when AH "
     "FontCatalog accepts (a) per-font raster_size + codepoint-provider "
     "callbacks, (b) the alias table with FontManager::resolve_weighted "
     "'@bold' convention, and (c) per-screen font scoping. WM registry "
     "stays source of truth until KR Hangul (AC00-D7AF + Jamo 1100-11FF), "
     "JP kanji (4E00-9FFF), and fullwidth FF01-FF60 ranges round-trip "
     "through the AH loader without raylib '?' fallback. Manifest-only "
     "in this batch; no call-site changes.",
     "FontConfigTest.h (theme/font wiring); DevtoolsManifestTest.h checks "
     "get_all_fonts() non-empty + AtkinsonMock present; --run-all-tests",
     "Vendor overlap: afterhours/src/font_catalog.h + font_coverage.h "
     "have catalog/face discovery, but not the WM per-font codepoint "
     "providers or alias convention."},
}};

inline constexpr std::size_t descriptor_count() noexcept {
  return kDescriptors.size();
}

inline constexpr std::size_t count_by_owner(Owner owner) noexcept {
  std::size_t n = 0;
  for (const auto &d : kDescriptors)
    if (d.owner == owner)
      ++n;
  return n;
}

// Find a plugin by id; std::nullopt when unknown. Never throws.
inline const PluginDescriptor *find(std::string_view id) noexcept {
  for (const auto &d : kDescriptors)
    if (d.id == id)
      return &d;
  return nullptr;
}

// True when every descriptor carries all required manifest fields.
inline constexpr bool descriptors_complete() noexcept {
  for (const auto &d : kDescriptors)
    if (d.id.empty() || d.header.empty() || d.public_entry.empty() ||
        d.promotion_gate.empty() || d.test_entry.empty())
      return false;
  return true;
}

} // namespace wm::devtools
