# Afterhours gaps, prioritized

These are the afterhours gaps reported by 12 consumer projects, merged into clusters and ranked by how many projects each would help. Sources (verbatim copies): [hanabi](hanabi.md), [hanabi index](hanabi-index.md), [floatinghotel](floatinghotel.md), [pumpkin-beachball](pumpkin-beachball.md) with [online docs](pumpkin-beachball-online-docs.md) and [online notes](pumpkin-beachball-online-notes.md), [pong-afterhours](pong-afterhours.md), [MyNameChef](MyNameChef.md), [endless-dance-chaos](endless-dance-chaos.md), [mailboy](mailboy.md), [voice_ah](voice_ah.md), [wordproc](wordproc.md), [kart-afterhours](kart-afterhours.md), [cartographer](cartographer.md) with [requests](cartographer-requests.md), and wm_afterhours's own [AFTERHOURS_GAPS.md](../AFTERHOURS_GAPS.md). When AFTERHOURS_GAPS.md restates another project's gap, only that project is counted.
Checked against afterhours e9d5f67 on 2026-11-15. Tier 1–2 status was grepped in `vendor/afterhours/src`; Tier 3–4 status is as reported.
Clusters: Tier 1: 6, Tier 2: 36, Tier 3: 40, Tier 4: 200, Likely fixed: 66 (grouped), Unsure: 22.

## Tier 1 — 3+ projects

| Gap | Area | Projects | What's missing |
|---|---|---|---|
| Replaced-logging host contract undocumented | docs | endless-dance-chaos, mailboy, kart-afterhours, cartographer | README still says "implement the four functions" and does not mention the `log_once_per` shim or that host macros must be statement-safe (`do/while(0)`). |
| Public CPU image API | rendering | floatinghotel, cartographer, pumpkin-beachball, hanabi | There is no public way to decode an image with size limits, read its dimensions without a GPU load, sample its pixels, or upload pixels as a texture; `load_texture_from_pixels` is still sokol-only in `metal_texture_detail`. |
| Gamepad injection in E2E | e2e | mailboy, cartographer, pumpkin-beachball | `test_input.h` queues keys, chars, mouse and wheel only, so there are no gamepad buttons, axes, or virtual-controller availability to drive. |
| Background jobs with frame-safe delivery | ecs-core | hanabi, floatinghotel, MyNameChef | No owned worker queue with cancellation, priority, bounded size, and a mailbox that hands results back on the ECS thread. |
| Durable atomic writes | platform/files | floatinghotel, MyNameChef, wordproc | `files::write_string_atomic` writes a fixed `.afh-tmp` sibling with no fsync, so it is neither power-loss-safe nor safe for concurrent writers. |
| Wrapped-text layout result with byte offsets | text | hanabi, floatinghotel, wm_afterhours | `wrap_text` and `measure_wrapped` build every line as a string and return no per-line byte ranges or advances, so line counts and range highlighting need a second pass (the planned `PreparedText` is not built). |

## Tier 2 — 2 projects

| Gap | Area | Projects | What's missing |
|---|---|---|---|
| Wall-clock E2E waits and deadlines | e2e | hanabi, floatinghotel | `wait`, `wait_until` and `set_timeout` all count down host `dt`, so a deadline depends on tick rate and time scale and cannot wait on a real-time worker. |
| Graphics-layer key reads bypass the injector | e2e | hanabi, floatinghotel | `graphics::is_key_pressed` has no test-mode branch, so shortcuts that read raw key state never see injected keys. |
| `type` injects bytes, not codepoints | e2e | floatinghotel, wm_afterhours | `test_input` stores `char char_value`, so typing `café` queues UTF-8 bytes. |
| Test-scoped file roots and temp-dir fixture | e2e | floatinghotel, MyNameChef | Only settings has `set_path_override`; nothing redirects config, save and temp paths together for a test and restores them afterwards. |
| Driving a live window resize in tests | e2e | hanabi, floatinghotel | The runner cannot reproduce a native live resize or wait for its layout and presentation. |
| Computed layout not inspectable from code | e2e | hanabi, floatinghotel | `dump_ui` writes rounded rects without padding, margin, gap or ellipsis state, and `UIComponent::children` is cleared each frame, so the resolved tree cannot be walked (the interactive `LayoutInspector` is display-only). |
| E2E command registration boilerplate | e2e | pumpkin-beachball, pong-afterhours | `arg_as` exists, but there is no `require_state` helper, typed-argument command shape, or `testing::command("name", fn)` registration. |
| Axis activation edges | input | cartographer, pumpkin-beachball | A held trigger or stick stays in `inputs_pressed` every frame; there is no edge after deadzone or `is_gamepad_axis_pressed(pad, axis, dir)`. |
| Tab order by tree or visual position, with scopes | input | hanabi, floatinghotel | Tab follows entity allocation order and cannot be scoped to a subtree or focus group. |
| Focus-scoped shortcut dispatch | input | floatinghotel, cartographer | Nothing routes a chord from the focused control through its scopes to the app with consume-once semantics. |
| Programmatic focus of a text input | input | hanabi, floatinghotel | `set_focus` on the composite wrapper is dropped and a field built this frame cannot be focused, so callers must reach the inner field. |
| Drag-group hit tests ignore scroll and clip | input | hanabi, wm_afterhours | `HandleDragGroups*` still tests raw `child_cmp.rect()` instead of the scrolled, clipped hit rect. |
| Hover tint tied to click listeners | input | hanabi, floatinghotel | A clickable container always gets the hover fill; `skip_hover_override` has no config setter and there is no hover-within state. |
| Codepoint fallback placeholder | text | wm_afterhours, floatinghotel | Nothing visible is drawn for a codepoint no loaded face covers. |
| Grapheme-aware text operations | text | floatinghotel, wm_afterhours | Ellipsis still binary-searches by byte and labels have no grapheme mapping for wrap, selection or emoji sequences. |
| Text-fit and ellipsize helper | text | hanabi, pumpkin-beachball | There is no public "how much fits in this width" query or `ellipsize_text`; truncation lives inside the UI renderer and costs O(n) measures. |
| Font sizes are not platform point sizes | text | hanabi, floatinghotel | A fontstash or nominal pixel size does not match a native or browser point size. |
| Selectable read-only text | text | hanabi, floatinghotel | No selectable label, and no text-source selection that stays valid when virtualized rows leave the viewport. |
| Persisted storage codecs with versions | platform/files | floatinghotel, MyNameChef | No versioned decode with migrations and typed errors for settings or save files. |
| Native macOS desktop integration | platform/files | hanabi, floatinghotel | No menu bar adapter, status item, notifications or global hotkeys (`tools/mk_bundle.sh` covers only `.app` packaging). |
| Memory accounting and budgets | ecs-core | hanabi, floatinghotel | No byte-budgeted cache, memory-pressure event, GPU-bytes report or per-system retained-byte attribution. |
| Frame `dt` is callback time | rendering | hanabi, wm_afterhours | Sokol `get_frame_time` returns `sapp_frame_duration()`, not elapsed time since the last admitted frame. |
| Live resize paints stale or late frames | rendering | hanabi, floatinghotel | The macOS backend updates the drawable size without drawing before returning to AppKit, so resize steps show stale content. |
| Per-frame autorelease pool on Metal | rendering | hanabi, floatinghotel | Only the PNG writer has an `@autoreleasepool`; frames, headless or windowed, never drain autoreleased Metal objects. |
| GPU object retirement timing | rendering | hanabi, floatinghotel | Destroying a GPU object frees it a frame later with no back-pressure and no safe retire for a texture already in flight. |
| Non-rectangular child clipping | rendering | wm_afterhours, cartographer | `HasClipChildren` uses only rectangular scissors, so rounded parents and clip paths cannot clip children. |
| `translation.h` includes fmt before `<cstdlib>` | build/backend | endless-dance-chaos, mailboy | fmt 11.2 with clang 21 fails on `::malloc` because `translation.h` includes fmt first. |
| Host type macros undocumented | docs | mailboy, cartographer | `developer.h` now has raylib defaults, but no README section lists the required macros or include order. |
| Accessibility semantics | ui-widgets | hanabi, wm_afterhours | No accessible name or role layer; icon-only controls have only a tooltip. |
| Retained widget state ignores the caller's value | ui-widgets | hanabi, wm_afterhours | `toggle_switch` and `slider` seed their state once (`init_state`, `HasSliderState`), so an outside change leaves them stale. |
| Tree view row renderer | ui-widgets | floatinghotel, wm_afterhours | `TreeViewConfig` has only label, id and expandable callbacks, with no row renderer, virtualization or keyboard traversal. |
| Menu row padding and styling | ui-widgets | wm_afterhours, wordproc | Rows do not take padding, theme-usage colours or hover styling from the menu config, labels sit flush at the edge, and the shortcut style is fixed. |
| Slider part styling | ui-widgets | kart-afterhours, cartographer | The handle is a fixed 25% block with no fill segment, knob size or separate handle colour. |
| Focus ring styling | ui-widgets | hanabi, cartographer | Offset and thickness exist, but there is no single-outline mode, halo, per-state style, transition, movement check, or app-drawn ring hook. |
| Scrollbar space policy | ui-layout | hanabi, floatinghotel | The scrollbar is on or off; there is no overlay mode and no reserved gutter for content. |
| Grid snapping disagrees with content size | ui-layout | floatinghotel, wm_afterhours | Intrinsic and scroll extents sum unsnapped sizes while placement snaps, so content can overflow its column or scroll range. |

## Tier 3 — 1 project, broadly useful

| Gap | Area | Projects | What's missing |
|---|---|---|---|
| Unread injected press passes silently | e2e | cartographer | A step passes when no system read the injected press. |
| Screenshot failure does not fail the test | e2e | pumpkin-beachball | The screenshot callback returns void, so a backend that cannot capture only warns. |
| Single-script run prints no pass/fail line | e2e | endless-dance-chaos | `print_results` prints a tally only in batch mode; single scripts report only the exit code. |
| Built-in handler shadows an app command | e2e | kart-afterhours | A new built-in silently takes over a same-named app handler, with no warning or opt-out. |
| Unknown handler cannot defer to another state | e2e | cartographer | `register_unknown_handler` fails commands meant for a state that is not active yet. |
| Machine-readable test results | e2e | MyNameChef | No JSON or JUnit records with IDs, skip reasons and defined exit codes. |
| "Running under E2E" query | e2e | pumpkin-beachball | Test mode is visible only as `test_input::is_test_mode()`, with no documented signal for hiding nondeterministic HUD readouts. |
| Held raw-key and modifier commands | e2e | floatinghotel | `key` releases modifiers after two frames and hold/release cover only mapped actions. |
| Element-addressed gestures beyond click | e2e | hanabi | No by-name or by-text drag, hover, wheel, double-click or triple-click. |
| Debug-name lookup is ambiguous | e2e | hanabi | `click_ui` and `find_component_center` take the first match with no `in=` scope or name index. |
| One-call E2E setup | e2e | pong-afterhours | `parse_e2e_args` and `configure_runner` exist, but no single call wires handlers, tick, screenshots and exit code. |
| Key repeat with delay and rate | input | wm_afterhours | No per-action repeat with initial delay and rate on keys, buttons and axes; slider and tray repeat are per-frame or drop the remainder. |
| Enum-keyed action queries | input | wm_afterhours | The collector exposes int-action vectors only, with no `is_held(Action)` or `is_pressed(Action)`. |
| Input-collector system base | input | pong-afterhours | No `InputSystem<...>` base that hands the collector to game systems. |
| Exact modifier matching | input | wordproc | Chords accept extra modifiers, so Ctrl+Z and Ctrl+Shift+Z collide. |
| Disabled state requires `HasLabel` | input | wm_afterhours | `HandleClicks` checks only `HasLabel::is_disabled`, so controls built from child labels stay clickable. |
| Initial focus control | input | cartographer | No "no initial focus" or "focus this first" option, and automatic refocus sets `Grab` modality and lights the ring. |
| Configurable fixed timestep | ecs-core | pumpkin-beachball | `fixed_tick_all` uses a hard-coded `FIXED_TICK_RATE = 1/120` with no max-steps clamp. |
| Standard transform and velocity components | ecs-core | pong-afterhours | No shared position (with `prev`) and velocity components. |
| Serializable RNG streams | ecs-core | MyNameChef | `RandomEngine` is a seeded global with no named streams or full-state save and restore. |
| App cache directory | platform/files | hanabi | No cache directory separate from the config directory. |
| Local date and time | platform/files | pumpkin-beachball | The platform exposes only elapsed `get_time()`. |
| Sokol `run()` ignores headless | platform/files | wordproc | `MetalPlatformAPI::run` always enters `sapp_run`, whatever `RunConfig::display` says. |
| Native window resize on sokol | platform/files | floatinghotel | `set_window_size` and `set_window_min_size` log `@notimplemented` for a real window. |
| `<cxxabi.h>` guarded by compiler | build/backend | pumpkin-beachball | `core/system.h` should use `__has_include`; clang with the MSVC STL fails. |
| Concepts use C compound literals | build/backend | pumpkin-beachball | `(const char *){}` in `graphics_common.h` breaks GCC with `-Werror=pedantic`. |
| NONE backend measures text as zero | text | pumpkin-beachball | Without `set_measure_text_fn` every width check passes, and `PlatformAPI::measure_text` uses a different estimate under the same name. |
| Measure and draw use different font inputs | text | wm_afterhours | Autolayout measures with `font_name` and spacing 1 while rendering uses the weight variant and `1+letter_spacing`. |
| Sokol ignores letter spacing | text | wm_afterhours | Sokol measure and draw drop the spacing parameter. |
| Raylib missing-glyph fallback | text | wm_afterhours | Raylib `draw_text_ex` does not split runs per codepoint to fall back across faces as sokol does. |
| Font loading from memory, deduped | text | floatinghotel | Sokol rereads the font file on every load and there is no `load_font_from_memory`. |
| Default font leftovers | text | cartographer | The input debug overlay calls `raylib::DrawText` and font_helper error paths return `GetFontDefault`, ignoring `set_default_font`. |
| Hard character breaks in wrap | text | wm_afterhours | Long words are not broken, and wrap needs an explicit font size. |
| Antialiasing on sokol | rendering | hanabi | Sokol `sample_count` is fixed at 1; raylib has `enable_msaa`. |
| Lightweight frame-rate stats | rendering | pumpkin-beachball | `get_fps()` is instantaneous and E2E perf FPS uses a different clock. |
| Audio device lifecycle in `run()` | audio | voice_ah | The graphics run config does not open or close the audio device. |
| Sound playback wrappers | audio | voice_ah | No stop, pitch, pan, pause or resume wrappers. |
| `crash::report` re-entrancy | other | endless-dance-chaos | The handler reports through `log_error`, which recurses when the host's `log_error` aborts. |
| `distance_sq` missing on raylib | other | wm_afterhours | `afterhours::distance_sq` exists only for the fallback `MyVec2`. |
| Minimal consumer build examples | docs | MyNameChef | No tested minimal builds for pure ECS, logic-only tests and offscreen rendering. |

## Tier 4 — 1 project, niche

### e2e
- Coordinate precondition assertion — nothing asserts that a point lies inside a given byte range or text run (hanabi)
- Modifier reads bypass the injected reader — `check_single_action_impl` calls backend `get_current_modifiers()`, so backend-free unit tests log `@notimplemented` (hanabi)
- Headless cadence harness — the production frame callback cannot run headlessly to test frame admission (hanabi)
- Supersampled capture — `ui_scale` zooms layout instead of rasterizing at Retina density (hanabi)
- Render checkpoint for UI lookups — lookup can read a view drawn before the latest logic ticks when hosts batch ticks (floatinghotel)
- Test reset keeps focus state — `UIContext::reset()` keeps `has_interacted`, so single and batch captures differ (wm_afterhours)
- Presented-frame screenshots — capture reads the canvas, never the letterboxed frame the player sees (pumpkin-beachball)
- Multi-process orchestration — networked games hand-roll spawning, ports, log barriers and log collection (pumpkin-beachball)
- Harness argument spellings — `parse_e2e_args` rejects `--test-script=path` and `--e2e-timeout` (wordproc)
- Terminal console commands — no built-in `expect_terminal_*` commands for `terminal::Console` (cartographer)
- Audio assertions — no record device to assert that a sound played (voice_ah)
- Profiling passes when empty — `builtin_profile::enable()` does not warn when profiling is compiled out and overwrites a custom PerfProvider (endless-dance-chaos)

### input
- Trackpad and wheel share one speed — precise and notched deltas arrive as the same float (hanabi)
- Horizontal scroll views ignore Shift+wheel — `HandleScrollInput` maps Y only onto vertical views (hanabi)
- No middle button in the UI context — `MousePointerState` tracks only left and right (hanabi)
- Activate without focus — every click on a `HasClickListener` moves keyboard focus (hanabi)
- Focusable set rebuilt every frame — it is a `std::set` cleared and refilled each frame (hanabi)
- Non-consuming input-activity snapshot — only raylib's internal `host_input_activity` exists (hanabi)
- External pointer owner — while a native menu tracks, a press underneath still registers (hanabi)
- Cmd chords never become key events on macOS — sokol's `performKeyEquivalent:` handles only Tab (hanabi)
- Non-layered input map cannot remap (hanabi)
- Overlapping absolute buttons give ambiguous hits — tab and close-button hits are done by hand (hanabi)
- Wheel hit test ignores ancestor scroll and clip — `HandleScrollInput` uses `cmp.rect()` (floatinghotel)
- `text_input` requires host enum names — editing works only if the app's InputAction declares `TextBackspace` and friends (floatinghotel)
- Layout-to-screen coordinate conversion — draggable panels convert between logical and mouse coordinates by hand (floatinghotel)
- Arrows escape a focused single-line field — the inner field lacks `ConsumesDirectionalInput` (floatinghotel)
- Focus-return tokens — modals and menus restore a raw entity ID with no logical identity or fallback (floatinghotel)
- Focus fallback for removed rows — focus jumps to the first focusable when a virtualized row leaves (floatinghotel)
- Single Escape owner — menus, pickers and dialogs can all act on one Escape press (floatinghotel)
- Click-count gesture — only text inputs count double and triple clicks (floatinghotel)
- Parent gestures do not arbitrate with child buttons (floatinghotel)
- Focus origin vs `with_select_on_focus` — the synthesized click clears native select-all (wm_afterhours)
- Gesture hardware availability — the gesture backend cannot report hardware support (wm_afterhours)
- Hover and focus queries by debug name — `is_hot` takes an entity id only (cartographer)
- Inert while fading — elements at opacity 0 still take clicks and focus (cartographer)
- Raw-draw text-edit helper — games outside the UI system rebuild backspace, cursor and paste (pumpkin-beachball)
- Input mapping DSL — no builder for action bindings, rated low value by its reporter (pong-afterhours)

### ui-widgets
- Multi-line is a separate widget — `text_input` has no multi-line mode or Shift+Enter (hanabi)
- `text_area` re-wraps every frame — `layout_cache.rebuild` runs unconditionally (hanabi)
- `text_input` blurs itself on Escape (hanabi)
- `text_input` padding derived from height — it overrides the caller's padding (hanabi)
- Placeholder colour is theme-wide — always `font_muted` (hanabi)
- Callbacks outlive their frame — imm has no `on_submit` result (hanabi)
- Popover lifecycle — dismisses before its body runs, drops the debug name, and keeps an entity while closed (hanabi)
- `tab_container` ellipsis never applies — `min_width` is set to `Dim::Text` (hanabi)
- Exit animations — retired widgets do not live long enough to animate out (hanabi)
- Button variants drop the per-widget text inset (hanabi)
- Disclosure control widget — `tree_view` still draws text arrows though a polyline chevron primitive exists (floatinghotel)
- `text_area` line rows wider than the field — widths are scaled twice under Adaptive zoom (floatinghotel)
- Divider hit area equals visible thickness (floatinghotel)
- Toast wrapping and interaction — fixed height, no close, hover pause, merging or stack limit (floatinghotel)
- Empty `Padding{}` means default padding (floatinghotel)
- Modal centering mixes physical and scaled units (floatinghotel)
- Post-build action phase — model-replacing actions run mid-render (floatinghotel)
- Tooltip only when truncated — no truncation result or policy (floatinghotel)
- Delayed busy indicator (floatinghotel)
- Folded-range adapter for virtualized readers (floatinghotel)
- Convenience dialog presentation config — `modal::confirm` and friends hardcode font, size and padding (wm_afterhours)
- Dropdown visible-row cap (wm_afterhours)
- Pagination index bugs — page i+1 used as a zero-based index and Previous subtracts twice (wm_afterhours)
- Chart styling and axis domains — fixed 2px stroke, always-on hover values, data-only bounds (wm_afterhours)
- Fast virtual-list glides blank the viewport — the `max_span` clamp can drop current rows (wm_afterhours)
- `text_area` ignores readonly and disabled (wm_afterhours)
- Toast fade never rendered — opacity is written only when `HasOpacity` already exists (wm_afterhours)
- Caret blink uses a fixed 0.016 dt and ignores reduced motion (wm_afterhours)
- `ease_scroll` glides under instant motion (wm_afterhours)
- Virtual-list leading spacer — needs layout-offset support to remove (wm_afterhours)
- Slider knob compression and crowded tab labels (wm_afterhours)
- Dropdown tray render layer — the open list is only `render_layer+1`, so later widgets draw over it (voice_ah)
- Horizontal dropdown placement — the overlay corrects Y but not X (wordproc)
- Tab control variants — no active indicator or paper-tab style (cartographer)
- `with_icon` sizing and placement — fixed at 60% height, start-aligned, button only (cartographer)
- Tooltip focus, placement and refresh — hot-only, hardcoded Below, theme-wide radius, ignores rebuilt config (cartographer)
- Theme-bypass lint — nothing flags custom colours in shared UI (cartographer)
- Imm gradient and line components (cartographer)
- List-menu navigation for raw-draw games (pumpkin-beachball)

### ui-layout
- Imm subtree built twice collides — callers must salt `otherID` by hand (hanabi)
- Geometry on the build frame — a widget cannot read its own position or final height when built (hanabi)
- Absolute trailing anchors and percent sizing — absolute children place only from the leading edge (hanabi)
- `Dim::Percent` ignores the child's margin (hanabi)
- Unpadded element gets a screen fraction as padding (hanabi)
- Baseline alignment in rows (hanabi)
- `ctx.theme` is one global at render — per-widget colour overrides leak frame-wide (hanabi)
- Draw over children and window chrome — no after-children draw callback (hanabi)
- Selection and hover inset geometry — the fill is the row box and the highlight is the hit rect (hanabi)
- Draw-only elements — a decorative mark costs a full entity (hanabi)
- Two view trees in one window (hanabi)
- Flex shrink squeezes fixed controls below their children (floatinghotel)
- In-frame layout invalidation — expanding controls warn until the next pass (floatinghotel)
- Absolute children count as scroll content (floatinghotel)
- General `scroll_to` — clamp and easing undo a direct `scroll_offset` write (floatinghotel)
- Sticky children (floatinghotel)
- Logical scroll anchors — anchoring pins an entity, with no item key or result (floatinghotel)
- Layout-generation acknowledgement (floatinghotel)
- Virtual-list visible range and keyed updates (floatinghotel)
- Virtual-list overscan hard-coded at 4 (floatinghotel)
- Virtual-list `scroll_to_item` (floatinghotel)
- Virtual-list extents drift in float32 (floatinghotel)
- `icon_row` drops container config (wm_afterhours)
- A div's own label ignores flex alignment (wm_afterhours)
- Cross-axis Stretch does not size (wm_afterhours)
- `with_rotation` with rotated hit test and focus ring (cartographer)

### text
- Installed-font catalog and collection faces (hanabi)
- Font unload and the 16-font cap (hanabi)
- First-draw pre-warm (hanabi)
- Sokol atlas never shrinks (floatinghotel)
- Global text scale (floatinghotel)
- Resolved text origin — no runtime query for where a label's first glyph lands (floatinghotel)
- Styled labels bypass ellipsis — bold runs drawing small and ignored padding are unverified (floatinghotel)
- Font name stored twice on `HasLabel` and `UIComponent` (floatinghotel)
- `select_word_at` has a fixed word policy (floatinghotel)
- Monospace cell-advance measurement (floatinghotel)
- Typed text-cache keys and chunk state (floatinghotel)
- Text normalization to valid UTF-8 (floatinghotel)
- Translation glyph loading drops 4-byte UTF-8 (wm_afterhours)
- `with_letter_spacing(0)` is a no-op (wm_afterhours)
- Text-unit changes restart from the enter pose (wm_afterhours)
- Per-unit motion skipped for wrapped text with no diagnostic (wm_afterhours)
- Access-key underlines (wordproc)
- `load_font_with_codepoints` hides the size argument (cartographer)

### rendering
- Boxes rasterize 1px bigger and up-left (hanabi)
- Widget origins are not rounded (hanabi)
- `on_draw_fg` gets a scaled rect and no scale (hanabi)
- Timers cannot publish their next visual deadline to the frame loop (hanabi)
- Frame phases cannot run separately (hanabi)
- Window exposure and backing-scale changes hidden (hanabi)
- Metal pointer letterbox uses a stale resolution (hanabi)
- Staging texture leaks per screenshot without ARC (floatinghotel)
- sokol_gl shader compiles at startup (floatinghotel)
- Partial rounded outlines ignore corner masks (wm_afterhours)
- Auto-text fallback colour differs by backend (wm_afterhours)
- Draw-call coalescing across state changes (wm_afterhours)
- Rotation ignores transform origin (wm_afterhours)
- Sprite animation drops leftover time and uses raw dt (wm_afterhours)
- Particle drag flips sign at low fps (wm_afterhours)
- Triangle winding differs between backends (pumpkin-beachball)
- Texture usability query (pumpkin-beachball)
- Raw-draw juice helpers (pumpkin-beachball)
- Imm always draws after app systems (cartographer)
- Chamfered corners (cartographer)
- Offscreen layers and nested render textures (cartographer)
- No shipped blur shader (cartographer)
- Effect uniforms and matrix compose helper (cartographer)
- Per-draw filter on imm sprites (cartographer)
- `InstanceBatcher` drain without drawing (cartographer)

### ecs-core
- `ComponentConfig` strings copied per widget per frame (hanabi)
- Component writes cost two cache misses (hanabi)
- System deactivate hook (hanabi)
- Temp-entity query warning does not name the caller (floatinghotel)
- Supported hook to replace a bridge system (floatinghotel)
- Declared ownership with cascade delete (wm_afterhours)
- RAII collection binding (MyNameChef)
- Session reset contract (MyNameChef)
- Simulation driver with run states, speed and `run_until` (MyNameChef)
- Ordered event phases (MyNameChef)
- Canonical snapshots and diffs (MyNameChef)
- Staged world restore (MyNameChef)
- Budgeted incremental loader (cartographer)

### platform/files
- OS window-focus query (hanabi)
- Sokol drag-and-drop cannot be enabled (hanabi)
- Startup waits for the display link (floatinghotel)
- Deferred or hidden initial window (floatinghotel)
- Persisted window state applied before creation (floatinghotel)
- Confined path resolution (MyNameChef)
- Nonblocking native dialogs (wordproc)
- Web crash stack includes `execinfo.h` (wordproc)
- Comment-preserving or JSON5 settings (cartographer)
- Lightweight settings without nlohmann/json (pumpkin-beachball)

### build/backend
- Web builds reference Metal symbols (wordproc)
- `files.cpp` Windows guard re-enables raylib (cartographer)
- Prebuilt GNS artifacts and release metadata (pumpkin-beachball)

### docs
- `log_file` under replaced logging — host logs bypass `log_sink_fn` (endless-dance-chaos)
- Serial dispatch not listed as Breaking in the CHANGELOG (cartographer)
- Wire-versioning policy undocumented (pumpkin-beachball)

### audio
- Sound from float samples (voice_ah)
- `SoundLibrary::add` for generated sounds (voice_ah)
- AudioStream wrapper (voice_ah)
- Sound playback position (voice_ah)
- `export_wav` from samples (voice_ah)
- Silent sokol and none audio stubs (voice_ah)

### motion
- Instant-mode `to()` shows the from-value for a frame (wm_afterhours)
- Single-value `on_change` is a one-frame no-op (wm_afterhours)
- `with_reset` restarts the track every fire (wm_afterhours)
- Motion presets silently dropped by `motion_rules()` (wm_afterhours)
- `delay()` on an empty track is silent (wm_afterhours)

### gameplay helpers
- Character-controller velocity helper (pumpkin-beachball)
- Projectile-intercept and braking-arrival steering (pumpkin-beachball)
- Ready-up lobby helper (pumpkin-beachball)
- Swept-ellipse and ellipse-vs-ellipse collision (pumpkin-beachball)

### networking
- Snapshot buffer, interpolation and send pacing (pumpkin-beachball)
- Optional ECS session layer for afterhours-online (pumpkin-beachball)
- Endpoint parsing and DNS resolution (pumpkin-beachball)
- Per-peer RTT tracker (pumpkin-beachball)
- Typed-packet codec (pumpkin-beachball)
- Handshake-server helper (pumpkin-beachball)
- Connect-attempt state machine (pumpkin-beachball)
- Edge latch and input redundancy (pumpkin-beachball)
- Reusable Steam backend seam (pumpkin-beachball)
- Datagram-level fault injection (pumpkin-beachball)
- Second server bind succeeds on Windows (pumpkin-beachball)

## Likely fixed upstream

- Retrying custom commands and asserts — `pending_command.h` `retry_with` (hanabi, endless-dance-chaos)
- Directory-mode reset between scripts — `runner.h` `set_reset_callback` (hanabi)
- Single-script runs report failure — `runner.h` drains the last command before finalizing (hanabi)
- Quoted arguments, spaces and `\n` escapes — `runner.h` `ScriptArguments::next` (hanabi, floatinghotel)
- Colour, absence, scroll and clipboard assertions — `ui_commands.h` `assert_ui bg=/fg=`, `expect_no_ui`, `scroll_x/y`, `expect_clipboard`; `draw_commands.h` `expect_drawn color=`, `expect_not_drawn` (hanabi, floatinghotel)
- Strict `assert_ui_text` — `ui_commands.h` strict match and `in=` (hanabi)
- Draw geometry and text landing in E2E — `capture.h` `dump_draws`, `expect_drawn_at text=`, `click_text` (hanabi)
- Sokol draw capture — `backends/sokol/drawing_helpers.h` `capture::record` (floatinghotel, wordproc)
- Wheel delivered once — `input_injector` `wheel_read` (hanabi)
- `expect_input_text` on multiline — `ui_commands.h` reads `HasTextAreaState` (hanabi)
- Cmd/Super injection — `core/key_codes.h` `CMD+`, `command_handlers.h` `pending_super` (hanabi, floatinghotel)
- Right and middle click polling and press edges — `test_input.h` `is_mouse_button_*`, `right_just_pressed` (hanabi, wordproc)
- E2E host-loop example — `harness.h` `parse_e2e_args`, `configure_runner` (hanabi)
- Serial dispatch and time-based waits — `runner.h` `wait SECONDS`, `command_handlers.h` `wait_until`; clock is still dt, see Tier 2 (floatinghotel, pumpkin-beachball)
- Styled-span text matching — ba39609 (floatinghotel)
- Clip-aware text visibility — `expect_text_fully_visible`, `ui_commands.h` `visible_center` (floatinghotel)
- Screen rects through every scroll ancestor — `ui_commands.h` `apply_ancestor_transform` (floatinghotel)
- Injectable clipboard — `plugins/clipboard.h` `ScopedProvider` (floatinghotel)
- `--screenshot-dir`, `--e2e-speed`, runner self-timeout — `harness.h` `E2EArgs` (kart-afterhours, mailboy)
- Handlers across SystemManagers — `e2e_testing.h` `register_shared_handlers` (cartographer)
- Unconsumed command report — `runner.h` `unconsumed_command_names` (cartographer)
- Menu-flow commands, `expect_hovered`, `mouse_down` edge — `ui_commands.h` `click_label`, `set_slider`, `expect_no_focus` (cartographer)
- Fixed-dt capture and frame recording — `graphics_common.h` `fixed_dt`, `capture_every_n_frames` (cartographer)
- Synthetic press delay documented — `input_injector.h` (cartographer)
- Scroll to end — `HasScrollView::scroll_to_bottom` (hanabi)
- Drag threshold, grab point, overlay subtree, config access — `systems.h` `press_moved`, `create_or_update_drag_overlay`; `with_draggable_children` (hanabi, wm_afterhours)
- Modal gates on scrolling — `HandleScrollInput` `is_input_allowed` (floatinghotel)
- Stale mapped actions — db1d694 (floatinghotel)
- Mouse delta as an action, collector lookup — `input_system.h` `MouseAxisWithDir`, `get_singleton_cmp` (cartographer)
- macOS keymap and edit commands — `text_input/utils.h` `apply_edit_command`, `edit_keys.h` `edit_command_from_keys` (hanabi)
- Logical presenter and mouse map — `window_manager.h` `content_viewport`, `window_to_content` (pumpkin-beachball)
- `text_area` placeholder, background, focus and line height — `text_input/text_area.h` (hanabi, floatinghotel)
- Auto-grow row count — `TextLayoutCache::line_count` (hanabi)
- Delete to line start — `text_input/utils.h` `delete_to_line_start` (hanabi)
- Colour input — `ui/color_swatch.h` (hanabi)
- Tween, stagger and change trigger — `motion::Track`, `.delay`, `text_unit_motion.h` `stagger`, `.on_change` (hanabi)
- Focus-visible and ring fixes — `context.h` `focus_visible`, `with_focus_ring_offset`, `with_focus_ring_thickness`, `rendering.h` `roundness_at`, `focus_ring_for` (hanabi, floatinghotel, cartographer, kart-afterhours)
- Toast singleton warning — b18e4a2 (floatinghotel)
- Default card radius — c6345e2 (floatinghotel)
- Menu scrolling and check/radio marks — `menu.h` `MenuItem::check/radio`, `Overflow::Scroll` (floatinghotel, wordproc)
- Tooltip anchor lookup — `tooltip.h` `UICollectionHolder::getEntityForID` (floatinghotel)
- Contrast checker and validator precedence — `color.h` `contrast_ratio`, `validation_systems.h` `resolve_label_color` (wm_afterhours, cartographer)
- Audit fixes for empty `button_group`, NaN progress, empty `tree_view` callbacks, slider values, double modal backdrop — `imm_controls.h`, `imm_value.h` (wm_afterhours)
- Validation log spam and minimum target size — `validation_config.h` `warn_once`, `enforce_min_touch_target` (cartographer, hanabi)
- Value pill, divider, settings list, `with_720p_size` — `imm_layout.h` `value_pill`, `divider_line`, `setting_row.h`, `component_config.h` (cartographer)
- Typed palette accessor — `theme.h` `Theme::from_usage` (cartographer)
- Checkbox row and slider label overflow — bd7530f, `imm_value.h` split (kart-afterhours)
- `apply_overrides` forwarding — `component_config.h` (kart-afterhours)
- Label-only padding warning — `rendering.h` `warn_ignored_label_padding` (cartographer)
- Scroll smoothing — `HasScrollView::ease_scroll` (hanabi)
- Flex-grow — `layout_types.h` `flex_grow` (hanabi)
- Text inset — `theme.h` `text_inset`, `rendering.h` `resolve_text_inset` (hanabi, floatinghotel)
- Fit-content and child measurement — `with_fit_content`, `ui/measure_config.h` (hanabi)
- Virtualization, prepend anchor and keyed identity — `imm_virtual_list.h` `RowMetrics`, `HasScrollView::anchor_scroll`, `mk_keyed` (hanabi, floatinghotel)
- Grid snap opt-out, absolute flow and per-axis translate — `autolayout.h` `skip_grid_snap`, `solve_absolute_children`; `component_init.h` (hanabi, wm_afterhours, kart-afterhours)
- Font tiers, gap units, single-side spacing, `mk()` diagnostics — ae46465, `autolayout.h` `resolve_gap`, `layout_types.h` `Margin::Left`, `entity_management.h` `mk_next` (floatinghotel, kart-afterhours, cartographer)
- Glyph coverage, fallback and atlas growth — `font_coverage.h`, `FontManager::missing_codepoints`, sokol `expand_font_atlas`, `mesh_font_fallbacks` (hanabi, floatinghotel)
- Measurement cache invalidation and styled run offsets — `invalidate_measurements_for`, `measure_memo.h` `generation`, `text_run_offsets` (hanabi)
- Rich text, styled line height and text audit fixes — `draw_runs_in_rect`, `with_line_height`, d52252f, a5dd1ab, 2f224a8 (floatinghotel, wm_afterhours)
- Headless default font — `font_helper.h` `font_is_usable` (kart-afterhours)
- Sokol blending, mipmaps and corner geometry — `backend.h` `g_blend_pip`, `drawing_helpers.h` `emit_corner_arc`, 62bc429 (hanabi)
- Headless resize and focus-ring clip — a9b1ba0, d37dfd0 (hanabi)
- Metal `target_fps` and `request_frame` — `backend.h` `g_target_fps`, `frame_loop.h` `request_frame` (hanabi, floatinghotel)
- Texture detach, stale draw commands, scrollbar layering, drawn chevrons — 72d46eb, `rendering.h` `draw_layer_scrollbars`, 700ac7e (floatinghotel)
- Opacity on transparent backgrounds and per-side borders — `color.h` `opacity_pct`, `BorderSide` (kart-afterhours, cartographer)
- Platform and build fixes — `core/system_timing.h`, `appearance.h`, `files.cpp` `resolve_resource_root`, `clipboard.h`, `file_watcher.h`, `backends/none`, e8f5a82, 62bc429, README `SOKOL_IMPL` section (hanabi, floatinghotel, MyNameChef, cartographer, endless-dance-chaos)

## Unsure

- Input injection in shipping builds — hanabi's own notes call the e2e-only boundary deliberate (hanabi)
- `mouse.pos` NaN before the first event — movement guards exist, hit tests unverified (hanabi)
- Shimmer or gradient-mask primitive — effect presets may cover it (hanabi)
- Entity created this frame found by id — 9eee7f7 assigns a slot at creation (hanabi)
- `tab_container` screen-absolute position and `toggle_switch` sibling spacing (floatinghotel)
- Uniform 1px border lands outside its rect at 1x (floatinghotel)
- Subpixel rounding read as clipping in geometry assertions (floatinghotel)
- Reorderable collection gesture with threshold, edge scroll and cancel (floatinghotel)
- Modal closed from another call site keeps its input gate (floatinghotel)
- Disabled menu rows pass presses through — aad267f gave them a body (wm_afterhours)
- Corner-bit order differs between raylib and Metal fills (wm_afterhours)
- Nested scroll-view decorations escape ancestor clips — scrollbars fixed in 6931c61 (wm_afterhours)
- Batched text overflow parity with immediate mode (wm_afterhours)
- Border-only guides need a dummy `HasColor` (wm_afterhours)
- Multi-resolution headless capture crash in `invalidate_entity_slot_if_any` (wm_afterhours)
- Absolute child of a button ignores `with_translate` — 55ff08f and bd7530f touched it (cartographer)
- Label word-wrap — wrap measurement exists, `with_word_wrap` still says text-area (cartographer)
- Glyph-packing failure reporting — bdea3b9 reports atlas-full (cartographer)
- Keyframe timeline with reversible playback — `motion::Timeline` may cover it (cartographer)
- Shared presentation path for menu, pause and loading screens (cartographer)
- One-call game runner — `ui::run` covers UI apps only (pong-afterhours)
- Idle pre-match client terminates on disconnect, not triaged between library and game (pumpkin-beachball)
