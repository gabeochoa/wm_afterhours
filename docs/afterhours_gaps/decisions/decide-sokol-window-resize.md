# decide: native window resize on sokol (Tier 3: floatinghotel)

**Problem.** `MetalPlatformAPI::set_window_size` resizes the headless target but logs `@notimplemented` for a real window, and `set_window_min_size` is unimplemented (backend.h:505-518). sokol_app has no runtime resize. floatinghotel ships its own `metal_set_window_size` in `src/sokol_impl.mm`, which queues and coalesces requests on the main dispatch queue and applies them after the draw callback, and it hooks the Objective-C runtime to defer first-window presentation. Its gap entry ("Afterhours can own the Objective-C++ window helper") sets the contract:
- requests made during update/draw apply between draw callbacks
- pending requests coalesce to the latest size
- units are content-area points, not framebuffer pixels
- a pending/completed resize is observable, so nothing draws a mismatched layout
- top-left anchoring and a minimum content size are explicit options
- native tests cover real drawable sizes; headless resize isn't proof

The raylib usage audit (priority #6) puts the same need at 10 projects counting window position, DPI and monitor queries.

## Options

1. **Move floatinghotel's helper upstream as-is.** A private `backends/sokol/window_macos.mm` compiled into the sokol impl TU, behind `set_window_size` / `set_window_min_size`, plus `graphics::window_resize_pending()`. It's mostly a port of working, already-verified code, but macOS-only (Windows needs `SetWindowPos` via `sapp_win32_get_hwnd`; Linux has no sokol consumer). It's verifiable only by a native-window test like floatinghotel's `check_native_window_resize.sh`, which opens a hidden window and drives the MTKView by hand, with an external timeout.
2. **Option 1 plus deferred first show** (`RunConfig::show_when_ready`), removing floatinghotel's runtime hooks too. Same file, slightly bigger.
3. **Leave it app-side.** One app needs it, and it works there.

## Recommendation

Option 1, ported by whoever can run floatinghotel's native resize probe (Metal changes have to be tested from floatinghotel). The code exists and was verified on 2026-09-12; upstreaming it lets the other sokol app (hanabi) and future ones resize without ObjC++. Add the Win32 path when a Windows sokol consumer asks. Defer option 2's deferred-show to a follow-up.

**Rough size:** M (about 150 lines of ObjC++, the coalescing queue, a pending flag, and a native test with a timeout).
