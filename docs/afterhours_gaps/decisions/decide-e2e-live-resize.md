# decide: driving a live window resize in tests (hanabi, floatinghotel)

## Problem
The e2e `resize W H` command makes one programmatic size change. It sets `ProvidesCurrentResolution`, then calls `window_manager::set_window_size`, which is a single `setFrame:` on Cocoa/raylib, `metal_set_window_size` if the app provides it, or an offscreen resize when headless. Three things are missing:

1. **hanabi #594.** No test can reach AppKit's live-resize tracking loop (`_resizeWithEvent:`, will-start/did-end notifications, one `windowDidResize` per step). hanabi wrote about 430 lines of ObjC++ (`src/resize_drive.mm`) that post NSEvents to drag the corner, plus a gate script.
2. **hanabi #595.** During a live resize, sokol paints on the next display-link tick, so 7 to 24 of about 170 applied sizes are never drawn. afterhours has no resize-step hook. hanabi calls `-[MTKView draw]` from its own `NSWindowDidResizeNotification` observer.
3. **floatinghotel.** sokol's native `set_window_size` is `@notimplemented` (the app supplies `metal_set_window_size`). Nothing can wait until a resize has been laid out and presented, and a single `resize` never exercises several intermediate sizes.

## Options
A. **Port hanabi's macOS driver.** Add `live_resize_drag DW DH STEPS`, `expect_live_resize_steps`, `expect_sizes_skipped` and the same-step draw (#595) to a private .mm in the sokol backend. This fixes #594 and #595 for real. It's macOS and sokol only, about 500 lines of ObjC++, needs a visible native window, and can't run headless or in CI. It also has the posted-event timing limits that hanabi documents.

B. **Cross-backend stepped resize plus a settle wait in the runner.** `resize_steps W H STEPS` applies one intermediate size per frame through the existing path. `wait_resized W H` retries until the resolution singleton and the backend's drawable both equal W×H and one frame has been presented at that size. It works headless and on every backend and is about 80 lines plus a test. It covers floatinghotel's "wait for layout and presentation" and per-step layout checks, but not AppKit's tracking loop or #595.

C. **B, plus only the #595 fix.** Add the same-step draw in sokol's resize path, or `graphics::on_resize_step(cb)`, so apps no longer reach into the view. Leave the NSEvent driver in hanabi.

## Recommendation
C, done as two PRs. B first (small, testable headless, useful to both projects), then the #595 hook as its own sokol/macOS PR, verified from floatinghotel (Metal). The NSEvent drag driver (A) should stay app-side unless hanabi wants to delete its 430 lines. It can't be gated headlessly, and its measurements depend on posted-event timing.

Separately, native sokol `set_window_size` (floatinghotel's `metal_set_window_size`, coalesced and applied between draw callbacks) is its own gap and shouldn't be bundled with this.

## Rough size
B: about 80 lines in command_handlers.h/runner.h plus a test and an example. #595 hook: about 40 lines of .mm plus a native check. A: about 500 lines of .mm plus a gate script.
