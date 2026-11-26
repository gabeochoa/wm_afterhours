# Trellis → Puffin + afterhours: interaction learnings and 150 things to bring to ah

Sources read 2026-11-25: trellisui.com (site + docs strings + example bundles), trelliswm.app (home, how-it-works, full guide at /docs/).

**The one idea (both products):** spaces that nest, a camera that zooms, and nothing ever out of reach. TrellisUI = fractal dockable workspaces for web apps (Dan Fessler, successor to react-dockable; core is plain TS, zero deps; React adapter + `<trellis-workspace>` element). TrellisWM = the same idea as a native Mac window manager (Swift/AppKit/Accessibility/ScreenCaptureKit, menu-bar app).

## What Puffin can learn (interaction-level)

1. **Zoom is navigation, not magnification.** Pinch/scroll moves between *levels* of a nested tree and "settles on the space that fits best" — discrete semantic stops, not free-floating scale. Puffin's canvas/views should snap to panel/group/workspace levels.
2. **One layout motion everywhere.** TrellisWM brags about "one layout motion for every split, dock and close" (460ms family). A single house curve/duration for all layout changes makes the whole app feel coherent; views resize when motion *settles*, not every frame.
3. **Content stays mounted through every move.** Docking/tabbing/floating/hiding moves a container with CSS transforms; iframes keep sessions, canvases keep state. For Puffin: never tear down a view to move it — reparent the live view.
4. **Pictures, never recordings.** TrellisWM animates window *snapshots* (still pictures) during motion and only shows live content at rest — cheap, private, and lets the animation run at full frame rate regardless of app redraw cost.
5. **Semantic zoom / small-panel degradation.** Too small to use → app colour + icon tile (with split lines drawn on groups); double-click to go in. Every zoomed-out state stays legible and clickable.
6. **Drop-target grammar is spatial and total:** edge of a panel = dock beside (split), middle = tab-stack, screen edge = dock beside everything, drag out = float/tear-off. One gesture vocabulary, previewed live ("a space opens for it").
7. **Neighbours make room.** Docking animates siblings aside instead of jumping; minimize hands space to neighbours and restores to the *same spot*.
8. **Browser-style spatial history.** Back/forward through *where you've been* (camera states), plus numbered saved views (⌘+1..9) shown in the menu bar with the current one filled in.
9. **Frame-a-rectangle zoom.** Hold modifier + drag a rect → camera fits it. Esc cancels. The fastest "show me these things together" gesture there is.
10. **Double-click means focus/frame, double-click again returns.** Title bar (frame window), desktop (frame desktop), group tile (enter group). One gesture, reversible.
11. **The green button as dock/float toggle.** Reuse a control users already know; hold modifier for its original meaning. Zero new chrome to learn.
12. **Escape hatches are first-class.** "Float All Windows on Desktop", per-app "Don't Arrange", quit-and-everything-returns-visible. Radical layout systems need an obvious, total undo.
13. **Progressive, opt-in onboarding.** Nothing moves until you dock something yourself; a Getting Started card advances as you perform each move, and can be restarted from Settings.
14. **Every action is a URL** (`trelliswm://view?n=1`). Raycast/Alfred/Stream Deck/shell drive the WM. Command surfaces should be addressable strings, not just menu items.
15. **Menu-bar presence model.** Configurable, reorderable micro-controls (back/forward, saved views, everything); can collapse to just an icon. Ambient apps should let users tune their own footprint.
16. **Permissions explained in-product, one at a time**, optional ones clearly optional (Screen Recording only affects motion fidelity), with graceful degraded visuals (colour+icon boxes) when denied.
17. **Conflict management with the OS.** Setup offers one click to disable competing OS behaviours (edge tiling, Stage Manager) and one click to restore them on uninstall.
18. **Keyboard completeness:** roving-focus tabs, arrow-key dividers, Shift+F10 panel menu, remappable keymap, neighbour navigation in 4 directions, reduced-motion support.

## 150 items to bring to afterhours (tiling WM support)

Legend: H=hanabi, W=wordproc, F=floatinghotel, P=puffin-style tools generally. Effort: S/M/L.

### A. Layout tree core (1-18)
1. `dock::Node` tree: Split{axis, weights[]} | Tabs{ids, active} | Leaf{panel_id} — one serializable layout document. (M, H/W/F)
2. `LayoutDocument` JSON (de)serialize + version field + migration hook. (S, all)
3. Layout builder API (fluent split/tabs/leaf constructors) mirroring the doc. (S, all)
4. Rect solver: tree + root rect → per-panel rects, min-size clamping with weight redistribution. (M, all)
5. One `Stage` per workspace (the free/floating desktop node); stage cannot contain a stage. (S, H/F)
6. Floating layer: panels with free rects + z-order above the tiled tree. (M, all)
7. Nested depth unlimited; path addressing (`root/2/0`) for nodes. (S, all)
8. Tree ops as pure functions: insert beside, tabify, remove+heal (collapse single-child splits), swap, move. (M, all)
9. Remove-heals-tree invariant tests (no empty splits/tabs ever persist). (S, all)
10. Weight normalization + min sizes per view type respected by solver. (S, W/F)
11. Layout diffing: old rects → new rects map for animation interpolation. (M, all)
12. Controlled mode: app owns the document, workspace emits change events (undo/server sync). (M, H/F)
13. Uncontrolled mode: storage key → auto-persist to disk/settings. (S, all)
14. Layout snapshots: named saves (saved views), per-workspace. (S, H/W)
15. Multi-workspace: several documents, one active (per-display in WM terms). (M, H)
16. Panel identity stable across moves (id never changes when reparented). (S, all)
17. Singleton view types (opening reuses/focuses existing panel). (S, H/F)
18. `open(view, placement)` with placement enum: beside/into/float/stage-edge/auto. (M, all)

### B. Views & registry (19-34)
19. View-type registry: register(type, factory, title, icon). (S, all)
20. View params (typed payload per instance, serialized in doc). (S, W/F)
21. Dynamic titles + badges from view state (hooks/callbacks). (S, H/F)
22. Close guards (dirty check → veto/confirm). (S, W)
23. Allow-rules per type: where it may dock (edges only, no float, stage only...). (M, F/W)
24. Allow-rules enforced during drags *and* in open() placement fallback. (M, all)
25. Views render once into a persistent surface; moves never re-create state. (M, all) — in immediate mode: panel state lives outside the tree, keyed by panel id.
26. Hidden views stay mounted (state preserved, skipped paint). (S, H)
27. View lifecycle events: opened, focused, moved, hidden, restored, closed. (S, all)
28. Focus tracking per workspace; focused panel concept distinct from keyboard focus. (S, all)
29. Focus-follows-open; opening an existing view frames it (camera move). (S, H)
30. Panel menus: per-type items + built-ins (float, dock, close, frame, hide), extensible/filterable. (M, all)
31. Custom panel chrome: icon + accessory slots in header. (S, F/H)
32. Drag handle = header/title bar only; content drags never start panel drags. (S, all)
33. Panel header shows at all zoom levels until semantic-zoom threshold. (S, all)
34. Unknown view type in a loaded doc → placeholder panel, never crash/drop. (S, all)

### C. Drag, drop, dock (35-58)
35. Drag a panel by header: lifted ghost follows pointer, origin shows gap. (M, all)
36. Drop zones per target panel: 4 edges (split), center (tabs). (M, all)
37. Drop zones on workspace edges (dock beside root/stage). (S, H/F)
38. Drop on tab bar position = insert tab at index. (S, H/W)
39. Live drop preview: prospective rect shaded, siblings pre-shift ("a space opens"). (M, all)
40. Drop-target precedence rules when zones overlap (center < edges < tab bar...), documented + tested. (S, all)
41. Hover-dwell to tabify (pause over middle ~300ms before tab target arms). (S, all)
42. Invalid targets (allow-rules) show no preview / not-allowed cursor. (S, F)
43. Esc mid-drag cancels and animates back to origin layout. (S, all)
44. Drag whole tab group by its bar. (S, H)
45. Tear a tab out by dragging it off the bar → floats or docks. (M, H/W)
46. Drag between workspaces/windows (pop-out to OS window where backend allows). (L, H)
47. Divider drag resizes both neighbours only; everything else stays put. (S, all)
48. Divider drag with min-size push-through (push next divider when neighbour at min). (M, W)
49. Double-click divider = equalize siblings. (S, all)
50. Keyboard divider move (focus divider, arrows adjust weights). (S, W/H)
51. Floating panels: drag anywhere on header, resize from edges/corners. (M, all)
52. Float ↔ dock toggle action (button + shortcut + menu). (S, all)
53. Docked panel dragged to free area → floats at drop point with sensible default size. (S, all)
54. Snap floating panels to workspace edges/neighbours while dragging (guides). (M, F)
55. Raise-on-press z-order for floating panels. (S, all)
56. Drag threshold so clicks on headers/tabs don't start drags. (S, all)
57. During drags, render dragged panel as snapshot (cheap, no content reflow). (M, H/F)
58. Auto-scroll/zoom-out when dragging near viewport edge in zoomed view. (M, H)

### D. Camera, zoom, navigation (59-80)
59. Camera state: visible rect of the layout tree (node path + rect), animated. (M, all)
60. Zoom-to-node (frame a panel/group/run of neighbours to fill viewport). (M, all)
61. Step out one level / step in toward focus or pointer. (S, all)
62. Show-everything overview (fit root). (S, all)
63. Pinch/scroll zoom that settles to nearest level that fits. (M, H/F)
64. Frame-by-rectangle: drag rect → camera fits it; Esc cancels. (S, F/H)
65. Double-click header = frame panel; again = back. (S, all)
66. Double-click group tile = enter group. (S, all)
67. Maximize = frame leaf (not a separate mode). (S, all)
68. Spatial history: back/forward stack of camera states. (S, H)
69. Saved framings (numbered 1-9) per workspace + jump keys. (S, H/W)
70. Neighbour navigation: move camera/focus left/right/up/down by adjacency. (M, all)
71. Focus navigation across panels without camera move when already visible. (S, W)
72. Semantic zoom: below size threshold panel → icon+colour tile. (M, all)
73. Group tile rendering: mini split-lines preview of subtree when tiny. (M, H/F)
74. Level-of-detail hooks: view receives its on-screen scale, can simplify. (S, F)
75. Text stays crisp: re-rasterize at settle, bitmap-scale during camera motion. (M, W)
76. Camera animation: one house curve (~460ms) for every zoom/dock/close. (S, all)
77. Views resize (relayout) when motion settles, not per frame — interim transform only. (M, all)
78. Reduced-motion: camera jumps cut, layout changes fade/snap. (S, all)
79. Zoom to pointer (keep point under cursor stable). (S, F)
80. Overview labels: panel titles appear on tiles at overview zoom. (S, H)

### E. Tabs (81-92)
81. Tab strip per Tabs node with roving-focus keyboard support. (M, all)
82. Tab overflow: scroll/shrink with min tab width + overflow menu. (M, H/W)
83. Tab close buttons + middle-click close + right-click menu (close, move out, close others). (S, H/W)
84. Tab badges (dot/count) from view state. (S, F/H)
85. Tab drag reorder within strip. (S, H/W)
86. Last-active tab memory per Tabs node persisted in doc. (S, all)
87. Closing active tab activates neighbour (right, then left). (S, all)
88. Cross-app tabbing equivalent: any view types may share a strip. (S, F)
89. Tab strip doubles as drag handle for the group. (S, all)
90. Keyboard: next/prev tab, jump-to-tab-n. (S, H/W)
91. Pinned tabs (icon-only, unclosable) at strip start. (M, H)
92. Tab context: panels know siblings (e.g. editor can list open files). (S, W)

### F. Hide, tray, restore (93-104)
93. Hide panel with animation; hidden panels listed in a tray/dock model. (M, H/F)
94. App-rendered dock/tray: API exposes hidden list + restore/close actions. (S, H)
95. Minimize hands space to neighbours; restore returns to same spot/size. (M, all)
96. Restore remembers pre-hide node path even if tree changed (nearest surviving parent). (M, all)
97. Arrangements restored on restart (doc persistence covers it). (S, all)
98. Float-all / arrange-all bulk actions. (S, H/F)
99. Per-view-type "don't arrange" exclusion (always floats). (S, F)
100. Session restore never loses a window off-screen (clamp to visible). (S, all)
101. Recently-closed stack + reopen closed panel. (S, W/H)
102. Hide/show stage (desktop) toggle. (S, H)
103. Panel count limits per workspace with graceful refusal (free-tier style gate is app policy, engine just reports). (S, -)
104. Empty-workspace state (hints/drop target to open first view). (S, all)

### G. Input, keyboard, a11y (105-120)
105. Global keymap struct, remappable, with defaults matching Trellis (overview, step out/in, neighbours, back/forward, frame, save/goto view). (M, all)
106. Gestures ownership arbitration: pinch on content vs workspace (modifier or bare-desktop-only setting). (M, H)
107. Two-finger pan on empty stage pans camera. (S, F/H)
108. Escape hierarchy: cancel drag > close menu > step out. (S, all)
109. ARIA-ish semantics for native: panel roles, labels, focus order exposed to a11y layer. (M, W)
110. Focus ring visible at all zoom levels (scaled). (S, all)
111. Focus restoration when panel closes/moves (return to previously focused view). (S, W)
112. Keyboard-only docking (grab panel, arrows choose target/edge, Enter drops). (L, W/H)
113. Shortcut cheat-sheet view type generated from keymap. (S, all)
114. Command palette integration: every workspace action as a command with id string. (M, H)
115. URL/deep-link command scheme (`app://view?n=1`, `app://everything`) parsed to same commands. (M, H/P)
116. Drag with pen/touch: larger hit slop on dividers + headers. (S, F)
117. Hover states don't fire on panels under a dragged ghost. (S, all)
118. Cursor shapes: grab, grabbing, col/row-resize, not-allowed during drags. (S, all)
119. Scroll chaining: wheel over panel scrolls panel; over stage zooms (with modifier setting). (S, all)
120. Multi-display: one arrangement + camera per display; drag across joins there. (L, desktop-ish hosts)

### H. Rendering & engine integration in ah (121-138)
121. Dock workspace as opt-in plugin (`plugins/dock_workspace.h`), core ECS untouched. (M, all)
122. Immediate-mode bridge: `imm::workspace(ctx, doc, registry)` emits panel divs from solved rects each frame. (M, all)
123. Panel content clipped to panel rect (scissor) incl. during camera transforms. (S, all)
124. Camera transform via ancestor transform (reuse subtree transform work #474). (M, all)
125. Snapshot rendering during drags/zooms: render panel to texture once, transform texture. (L, H/F)
126. Dirty-rect / settle-only relayout: skip content relayout while camera animates. (M, all)
127. Divider + header + tab strip drawn from tokens (theme struct), zero hardcoded colors. (S, all)
128. Theming tokens list (surface, header, active tab, drop preview, focus ring...) with 2-4 built-in themes. (M, all)
129. Hit-testing through camera transform (inverse-map pointer). (M, all)
130. Text/input inside zoomed panels: scale-aware font sizing policy (render at settled scale). (M, W)
131. Iframe/webview-equivalent persistence guarantee documented: state keyed by panel id, never by tree position. (S, H)
132. Panels as entities: HasDockPanel component + systems in plugin (fits stage system from plugin overhaul). (M, all)
133. E2E commands: `dock <panel> <target> <edge>`, `frame <panel>`, `expect_layout <json-ish>`. (M, all)
134. Layout fuzz/property tests: random op sequences keep tree valid + rects partition root. (M, all)
135. Perf budget: solver O(n) per frame, no allocation in steady state. (M, all)
136. Example catalog entries: editor, paint, desktop, ops (this PR) as living specs. (M, all)
137. Migration shim notes for react-dockable-style APIs (Root/Panel/Window/Tab → doc). (S, -)
138. Headless screenshot support: workspace renders deterministically at fixed dt for baselines. (S, all)

### I. App-specific payoffs (139-150)
139. Hanabi: conversations as dockable panels; pin chat beside canvas, zoom out to all threads. (H)
140. Hanabi: saved framings = "rooms" (per-project panel sets) on number keys. (H)
141. Hanabi: tool-call iframes/previews stay alive while rearranging (item 25). (H)
142. Wordproc: document stage + palettes (styles, outline, comments) docked around it, Paint-style. (W)
143. Wordproc: split-view editing = same doc two panels via singleton-with-params exception. (W)
144. Wordproc: focus mode = frame document panel; Esc steps back out. (W)
145. Floatinghotel: diff list + diff view + comments as nestable panels; frame a file group. (F)
146. Floatinghotel: semantic-zoom overview of a whole stack as tiles; double-click into a diff. (F)
147. Floatinghotel: drag a file tab out to float beside its diff for cross-reference. (F)
148. Shared: one JSON layout per app persisted in settings; reset-layout command everywhere. (all)
149. Shared: command/URL scheme lets agents drive layout in e2e and demos. (all)
150. Shared: "arrange windows" auto-tile for first-run so empty workspaces demo themselves. (all)

## Suggested build order in ah
1. Doc + tree ops + solver + tests (A). 2. imm bridge + headers/tabs/dividers render (B,C subset). 3. Drag/drop with previews (C). 4. Camera + framing + history (D). 5. Hide/tray, persistence, keymap (F,G). 6. Semantic zoom + snapshot rendering (D,H). The four wm example screens (Paint/IDE/Desktop/Ops) are the acceptance demos at each step.
