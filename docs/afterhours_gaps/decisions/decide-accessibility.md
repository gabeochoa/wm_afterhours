# decide: accessibility semantics (Tier 2: hanabi, wm_afterhours)

**Problem.** hanabi #112 / #458 / #527: icon-only controls have debug names, pixels and now a tooltip (`with_tooltip`), but no accessible name, role, description or value. VoiceOver can't read an icon button. hanabi keeps its own `src/a11y_bridge.mm`, and its icon buttons are built `.with_label(" ")`. wm lists the same gap. Two layers are mixed up in this row:
1. **Semantics in the tree.** Store a name, role and value on each UI element, so tests, `dump_ui` and an eventual bridge can read them.
2. **A platform bridge** that exposes the tree to VoiceOver (NSAccessibility), UIA on Windows, or AT-SPI on Linux, kept in sync each frame, with focus and actions routed back.

## Options

1. **Semantics only.** A `HasAccessibility { role, name, value, description }` component set from `ComponentConfig::with_accessible(role, name)`. Built-in widgets fill in defaults: a button's role is Button and its name is its label; a slider's value is its percent; a checkbox's value is on/off; an icon button falls back to its tooltip as the name. `dump_ui` writes it, and E2E gets `expect_accessible name=...` so hanabi's "every wordless control names itself" audit moves upstream. Headless, testable, no platform code. One ComponentConfig field, so `config_size_test` changes.
2. **Option 1 plus a macOS NSAccessibility bridge**: one `NSAccessibilityElement` per semantic node, rebuilt or diffed each frame, with focus and press routed into `UIContext`. It's Darwin-only, needs a real window to test (no headless verification), and hanabi's `.mm` would be the starting point.
3. **Do nothing upstream**; hanabi keeps its bridge.

## Recommendation

Option 1. It's the testable half, it fixes the "a space is the only label" problem for every consumer's tests and tooling, and it's the data any future bridge needs. Treat the platform bridge (option 2) as a separate decision, with the same reasoning as the desktop-integration note. Since this is widget-level work, it belongs to the UI/widget agent after Gabe picks.

**Rough size:** option 1 is M: a component, a config field, defaults in about 8 widgets, a dump_ui field and an E2E command. Option 2 is L.
