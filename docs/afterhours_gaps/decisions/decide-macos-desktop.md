# decide: native macOS desktop integration (Tier 2: hanabi, floatinghotel)

**Problem.** hanabi #33a (with #5, #465-#474) and floatinghotel ask for the native desktop surface a game framework doesn't have: a real NSMenu menu bar (floatinghotel draws its own in `menu_bar_system.h`), an NSStatusItem menu-bar extra, UserNotifications, global hotkeys, and deep links. `tools/mk_bundle.sh` covers only `.app` packaging. hanabi does all of this in app-side `.mm` files. Every piece is Objective-C++, macOS-only, needs bundle/entitlement setup to test (notifications need an authorization prompt; hanabi #469 is "PLATFORM-GATED"), and can't be exercised headless in CI here.

## Options

1. **Keep it app-side; document it.** Write a short doc listing the hooks afterhours already gives a `.mm` file (window handle access, `frame_loop::request_frame()` to wake from a callback, `jobs::Pool` delivery), plus hanabi's bridge as the reference. Nothing to maintain.
2. **A thin `plugins/macos_desktop.h` + `.mm`, one feature per PR**, starting with a native menu bar. Have it take the same `MenuItem` list as `imm::menu` and deliver picks as an event drained in a system. Then a status item, notifications, and hotkeys. Each is about 150-300 lines of ObjC++, compiled only on Darwin with a no-op elsewhere, and tested by a macOS-only test that builds menus and calls action selectors directly. The cost is a permanent platform surface in a cross-platform game library, plus Windows and Linux equivalents someone will eventually ask for.
3. **A separate `afterhours-desktop` repo** that vendors afterhours, for the two desktop apps. That keeps the game library free of it.

## Recommendation

Option 1 now, and option 3 if a third desktop app shows up. Only the two desktop-style apps want this, and both already ship working app-side code. Moving it into afterhours buys little and adds a surface nobody can test headless. If Gabe wants it in-tree, start option 2 with the native menu bar only, since floatinghotel would delete real code for that.

**Rough size:** option 1 is one doc. Option 2 is M per feature, about four PRs.
