# Afterhours UI — Layout Bug Audit

Originally generated 2026-07-18 from 720p headless screenshots (`make screenshots` →
`output/*_720p.png`), inspected by 8 parallel adversarial QA subagents (~10 screens each)
plus manual cross-checks. **Status refreshed 2026-07-28.**

Goal: mature the UI library enough to build real apps (e.g. `~/p/wordproc`).

Audit against the **Definition of Done checklist in `STYLE_GUIDE.md`** — that's the objective
bar every screen below is measured against (no overflow, no orphans, columns align, contrast
passes, fits 720p in EN/KO/JA, no empty panels, primary actions never clip).

## ✅ FRESH RE-AUDIT — 2026-07-28 (all 80 screens, regenerated 720p, 8 QA agents)
Re-ran `make screenshots` and graded every screen against the **Definition of Done in
`STYLE_GUIDE.md`**. **The catastrophic pre-fix bugs are gone** — the render-sort fix + ~30
follow-up commits verified holding. Of 80 screens: **~55 CLEAN**, 4 high, ~15 medium, rest low.
The huge per-screen section below is the **2026-07-18 PRE-FIX record** (kept for history); the
lists here are the CURRENT truth.

### Verified FIXED (were high-severity pre-fix, now CLEAN on fresh shots)
modals, advanced_modals · scroll_view, scroll_click_bug (rows no longer dropped) ·
radio_buttons (first-option circle present) · setting_row_showcase, powerwash_settings,
sports_settings, shop_interface (column drift resolved) · self_align (content back inside panel) ·
fighter_menu, images, file_tree · toggle_switches first-row off-by-one (track now renders).

### HIGH — still broken (fix first)
- **parcel_corps_settings** — OK/Cancel/Apply clipped by the bottom screen edge (~y712); primary
  actions unusable. (DoD: primary actions never clip.)
- **text_input** — bottom rows collide: "Disabled Field" label + a stray glyph + Cancel/Save
  buttons stack on one row (~y529); disabled field box clipped by panel bottom (~y580).
- **forms** — "Show HUD" dropdown row clipped by the right panel's bottom edge (~x845 y410).
- **layout_patterns** — panel almost entirely empty: only 3 boxes top-left, no title/pattern
  content. (Still open from pre-fix; empty-panel.)

### MEDIUM — clearly wrong, still legible
- **flight_options** — disabled rows "High-G Turn" / "Reduced Collision Damage" faint blue-on-blue
  (low contrast); setting rows show no right-side control column.
- **forms** — Volume slider focus handle escapes past the row's right edge (~x600 y172).
- **hstack_showcase** — Start/Center/End alignment panels render empty (~y330-460).
- **toggle_switches** — last row "Unavailable Option (OFF)" clipped by panel bottom, track hidden.
- **scroll_clip_bug** — panel 4 (green) clips differently from panels 1-3 (they clip row 14 + a
  sliver of 15; green fits 1-14). Inconsistent clip geometry.
- **circular_progress** — "Thin Ring" panel overlaps the "40px" Size-Variations card.
- **cards** — "Item 1" chip overlapped/clipped by the Semi-Transparent panel ("Item I").
- **meters_gauges** — orphan circular-gauge ring floating in the gutter between panels (~x773 y343).
- **stepper_showcase** — Card-Selector description overflows the red card's right border.
- **empire_tycoon** — top-right avatar circles clipped/overlapped by the money panel.
- **auto_text_color** — stray blue partial rect escaping the right panel edge (~x1255 y170).
- **checkboxes** — stray diamond/plus glyph floating between Disabled rows (~x380 y471).
- **parcel_corps_settings** — Language-row "<" button misaligned vs the other decrement arrows.

### LOW — cosmetic (see per-agent notes; not blocking)
Faint inactive-tab contrast (mini_motorways), tight button gaps (toasts), truncated nav labels
(kirby_options), orphan marks (cozy_cafe, deadspace_settings), text-effect legibility
(text_shadow, text_stroke), animation single-frame artifacts (animation_spring/declarative/
looping — mid-animation, not bugs), and intentional demos (text_overflow, example_borders,
decorators, bevel_borders, colors, absolute_positioning, layout_bug_repros).

### Cross-cutting theme in the CURRENT findings
The remaining high/medium bugs cluster into **two** patterns, not nine:
1. **Bottom-edge clipping** — content/controls in the last row of a panel get cut by the panel or
   screen edge (parcel_corps OK/Cancel, text_input disabled field, forms dropdown,
   toggle_switches last row).
2. **Overlap / orphan escaping a container** — a sibling panel or stray element paints over/past
   its neighbor (cards, circular_progress, empire_tycoon, auto_text_color, meters_gauges,
   checkboxes). Smaller, per-screen.

### 🔎 ROOT-CAUSE VERDICT for the bottom-edge-clipping cluster — OUR SCREENS, not afterhours
Traced all 4 source files. Every one is a **wm_afterhours screen packing more content than the
fixed container it declares** — afterhours renders the declared sizes faithfully and the excess
clips/spills at the parent's bounds. There is **no afterhours layout bug here.** Two flavors:

- **Absolute-position arithmetic overflow** — `ParcelCorpsSettings.h`: the whole screen is
  hand-computed absolute coords. Phone frame is 680px tall (y=20→700) but the OK/Cancel/Apply
  footer math lands at `phone_footer_y ≈ 709` (height 28 → bottom 737) — *below the phone frame
  AND below the 720px screen*. The stacked rows simply march past the frame. (`ParcelCorpsSettings.h:315`)
- **Fixed container height < its content** — the container is given a hard height and `no_wrap`,
  so oversized children spill/clip:
  - `TextInputDemo.h:82` — `body` vstack is `pixels(400)` but holds 7 fields ≈ 478px; the overflow
    collides with the footer laid out after the declared 400px.
  - `ToggleSwitchShowcase.h:45` — card is `screen_pct(0.96)` `no_wrap`; content slightly exceeds it
    so the last row ("Unavailable Option") clips at the card bottom. (Comments here document a
    repeated hand-tuning battle — 0.85→0.95→0.96, status bar deleted — the tell-tale sign of a
    fixed-height-vs-content mismatch.)
  - `Forms.h:193` — right column is `Overflow::Scroll` with 12 checkboxes *on purpose* ("make
    content overflow for scroll"); the cut row at the scroll boundary is expected scroll behavior,
    **not a bug**. (Slider-handle-escapes-row is a separate, minor question — possibly the slider
    widget; low priority.)

**The lazy root-cause fix is our-side, one pattern:** stop hardcoding a fixed height and let the
container fit its content — use `children()` height (or `expand()` where it should fill), or reduce
the content/spacing so it fits at 720p. For ParcelCorps, drive the footer Y off the container
instead of a running hand-sum, or shrink rows/gaps. No `vendor/afterhours` change needed.

**Automated warning — DONE (2026-07-28).** `make screenshots` now warns on this whole class so
you don't need a manual screenshot audit. `collect_layout_problems()` in
`src/headless_screenshots.cpp` walks the settled UI tree per screen and logs
`[Headless][validate] <screen>: N issue(s): …`, then a run total. It flags:
- **off-screen** — a non-clipped element's rect leaves the 720p viewport (catches parcel_corps'
  OK/Cancel/Apply); and
- **child escapes parent** — a non-absolute child spills >8px past a non-clipping parent's
  bottom/right (catches text_input's fields escaping the fixed-height `body`).

It suppresses the obvious false positives: elements inside a Scroll/Hidden (`HasClipChildren`)
container, absolute-positioned children, and hairline (<8px) edge overflows. First full run:
**69 issues across 12 screens**, correlating with the manual findings above (parcel_corps,
text_input, cards, meters_gauges, deadspace, circular_progress, auto_text_color…). Demo screens
(flex_alignment, layout_bug_repros) produce expected noise — dismiss those.

Note: the ECS `ValidateChildContainment`/`ScreenBounds` validators in
`vendor/afterhours/validation_systems.h` do NOT fire in the batch path (they filter the main
entity collection; UI widgets live in the UI collection), which is why the check walks the UI
collection directly via `ui_query()` — the same mechanism `layout_summary` uses. Wiring the ECS
validators into the UI-collection context would be the afterhours-side follow-up, but is not
needed for the warning to work.

### Fixes applied — 2026-07-28 (warning: 69 issues/12 screens → 15/2)
All root causes were our screens declaring fixed-size containers smaller than their content, or
misusing a dim unit — none were afterhours bugs. Fixed + verified (warning-clean + visual):
- **text_input** — `body` `pixels(400)` → `children()`.
- **parcel_corps_settings** — 44px rows/chevrons → 40px so the footer fits the phone frame.
- **cards** — `nested_container`/`info_panel` `pixels(130)` → `children()`.
- **hstack_showcase** — cards row `pixels(56)` → `expand()`.
- **circular_progress** — cards `h720(170)` (wrong axis) → `w1280(250)`.
- **meters_gauges** — anim ring 60→44px + left-only margin; gauge rings 90→72px.
- **language_demo** — footer 55→70px (content 425→410) to hold two 16px lines.
- **deadspace_settings** — panel width `screen_w-290` → `-330` (was running off-screen).
- **auto_text_color** — trimmed root/section padding + separator margins so all 5 sections fit.
- **forms** — two-column content `percent(0.60)` → `0.66` (left column no longer clips).
- **flex_alignment** — demo `container` `percent(0.85/0.70)` → `expand()` (fills space after title).

**Remaining (2 screens, 15 issues) — known/acceptable, not fixed:**
- **cozy_cafe** (4) — cosmetic bleed from the vendored `decorative_frame` (its inner fill layers
  keep full size but translate inward, spilling 4–14px off the right/bottom where nothing shows).
  Real fix is in `vendor/afterhours/imm_components.h` (subtract translate from fill size); skipped
  as invisible.
- **flex_alignment** (11) — the container-overflow bug is fixed; residual flags are dense-demo box
  cramping (26px boxes in narrow demo columns) + the 6-tab `tab_container` widget. Low-value demo
  noise; left as-is.

### Still open (unchanged, not layout)
- **Batch renderer state leak** — investigation done, root cause identified (batch path in
  `create_screen_systems()` omits a system the real game loop runs), fix NOT applied. Blocks
  trustworthy screenshot-diff CI gating. See dedicated section below. NOTE: several fresh
  findings above (empty panels, faint dropdown arrows) may be artifacts of this same batch path —
  confirm against a full-game-loop capture before fixing per-screen.

## ⭐ ROOT CAUSE FOUND & FIXED (render-command sort) — committed & pushed
The single highest-impact bug behind the modals breakage, the radio/toggle/stepper "first-child
missing its control" pattern, AND the batch-renderer non-determinism was **one bug** in
`vendor/afterhours/src/plugins/ui/rendering.h`: the render-command sort tiebroke equal-`layer`
commands by `entity.id`. Entity IDs are recycled across the 79 batch screens, so a parent could
receive a higher recycled id than its own children and paint its opaque background OVER them —
hiding titles, labels, first-row controls, and whole sections. Fix: **stable-sort by layer only**,
preserving the document pre-order (parent-before-child) the commands are already queued in
(`std::ranges::stable_sort` / stable bubble sort on Win32). Explicit `render_layer` still governs
true z-order (dropdowns, modals, focus rings), so overlays are unaffected.

- Both independent planner agents produced the byte-identical patch.
- **Verification:** applied, rebuilt, regenerated all 79 screens. 76 changed; 8 adversarial
  before/after judges rated them **73 BETTER, 3 SAME, 0 WORSE (no regressions)**. Batch
  non-determinism also resolved (`forms` now byte-identical across rebuilds).
- **Shipped:** `vendor/afterhours` branch `fix-render-command-sort` (merged with latest
  origin/main), pushed. Parent repo pin committed.

## Follow-up fixes — branch `ui-layout-fixes` (both repos, pushed)
On top of the render-sort fix, 10 more issues fixed and individually verified (each rebuilt,
screenshotted, and checksum-diffed to confirm it changed ONLY its target screen):

**Library widget fixes (`vendor/afterhours`, high value for real apps):**
1. `stepper` — multi-visible steppers ran labels together ("HealerWarriorMage"); added a gap when
   `num_visible > 1` (single-value steppers unchanged).
2. `slider` — handle mispositioned on percent-sized tracks (an 80% slider showed the knob at
   ~30%); the fraction was double-applied against the track width. Fixed for percent/screen-percent
   dims; pixels dims unchanged.

**Per-screen fixes (`wm_afterhours`):**
3. Tab bars on 4 settings screens (PowerWash/MiniMotorways/Sports/Flight) — fixed-pixel bars too
   narrow forced ellipsis; fill width + (for the crowded ones) smaller font + no-wrap.
4. stepper_showcase — card description overflowed the card; smaller font.
5. cozy_cafe — customer name overlapped "Patience:"; stacked name on its own line.
6. nine_slice_borders — "Double Width"/"Double Ornate" overflowed thick borders + dialog clipped;
   smaller fonts / shorter text.
7. example_borders — "Hard/Soft Shadow", "Outline Only", per-side labels overflowed cards; smaller
   fonts.
8. sports_settings — row label column too narrow ("Anti-Aliasing me..."); widened label / trimmed
   value column.
9. angry_birds_settings — inactive tab text unreadable (dark-on-dark); lightened theme font_muted.
10. decorators — status dot touched the "O" in ONLINE; added a gap. fighter_menu — card
    descriptions overflowed; shortened + smaller font.

## Known remaining (lower priority, not yet fixed)
- **meters_gauges** — inline meter track/fill misalignment and the off-screen slider are FIXED
  (pixel-height meters + left-anchored slider row + the progress_bar track/fill sizing fixes). One
  minor unnamed nub may still show at the circular-gauges panel edge (cosmetic).
- **checkboxes "Choice 1" dim** — verified NOT a bug: it's the min-constraint disabled state
  (the sole selection is locked so it can't be unchecked). Intentional.
- **kirby_options Edit/Erase** — verified NOT clipped (layout-summary confirms x=65, on-screen);
  audit false positive.

## Fixed this session (see AFTERHOURS_GAPS.md #4–#12 for library fixes)
- **fighter_menu** — sub-tabs restacked vertically (no more character-panel overlap); descriptions
  now use `TextOverflow::Wrap`.
- **images** — sprite bleed fixed (source rect derived from each texture's real size).
- **Static-label word-wrap** — added as `TextOverflow::Wrap` (library feature).

The per-screen findings below were the PRE-FIX state (kept for the record); most are resolved by
the render-sort fix. Remaining genuine issues after the fix (text overflow/clipping, tab-bar
sizing, a few contrast cases) should be re-audited from fresh post-fix screenshots.

## How to reproduce
```bash
make screenshots            # regenerate output/<screen>_720p.png for all 79 screens
open output/modals_720p.png # inspect a specific screen
```

## Severity legend
- **high** — broken/unusable, overlaps, off-screen, missing content, unreadable
- **medium** — clearly wrong but screen still legible
- **low** — cosmetic / possibly intentional demo artifact

## Cross-cutting patterns (fix these in the library, not per-screen)
These recur across many screens and are the highest-leverage fixes:

1. **First-child in a group renders label/control-less or collapsed.** Radio groups
   (`radio_buttons`: Orange/Red have no circle), steppers (`stepper_showcase`,
   `sports_settings`, `setting_row_showcase` top rows show a lone `>` with no label/value),
   toggles (`toggle_switches` first row has no track). Strongly suggests an off-by-one in how
   the first row of a repeated group is emitted.
2. **Label ↔ control column drift in settings-style rows.** Labels pinned left, controls pinned
   far right with a dead gap, and counts mismatch (N labels vs M value rows). Seen in
   `setting_row_showcase`, `powerwash_settings`, `parcel_corps_settings`, `shop_interface`,
   `sports_settings`. Row layout / vertical alignment between the two columns is unreliable.
3. **Text overflows / clips its container's right edge.** `example_borders` (labels collide:
   "Bottom OnlyLeft+Righ"), `nine_slice_borders` (labels clipped by ornamental border),
   `stepper_showcase` description spills past panel, many truncated tab labels.
4. **Tab bars truncate + cluster to one side.** `mini_motorways_settings`, `flight_options`,
   `fighter_menu`, `deadspace_settings`, `powerwash_settings`, `sports_settings`,
   `flight_options` — tab strips are too narrow, ellipsize every label, and bunch right leaving
   a dead zone.
5. **Scroll/clip regions drop or unevenly space rows.** `scroll_view`, `scroll_click_bug`,
   `scroll_clip_bug`, `file_tree` — items missing mid-list, large uneven gaps, bottom row clipped
   at panel edge. Likely a clip-rect / row-advance bug in the scroll container.
6. **Modal system does not render as a modal.** `modals` — triggers scatter as loose widgets,
   a "Notice" button floats disconnected, no overlay/backdrop. Highest-priority single screen.
7. **Content escapes / collapses out of its container panel.** `self_align` (demo renders below
   the empty panel), `layout` / `layout_patterns` (missing "Left Panel", mostly-empty panels),
   several "empty right half" settings cards.
8. **Stray/orphaned glyphs & elements.** Floating checkmark `v` glyphs (`checkboxes`,
   `toggle_switches`, `themes`), lone corner brackets (`decorators`, `deadspace_settings`),
   stranded arrows/dots. Suggests decorators/indicators emitted without their parent row.
9. **Cross-screen state leak in the batch renderer (non-determinism across rebuilds).** In the
   single-process `--headless-screenshots` batch, 7 screens (`forms`, `images`, `hstack_showcase`,
   `file_tree`, `islands_trains_settings`, `mini_motorways_settings`, `flex_alignment`) render
   DIFFERENTLY depending on the binary build lineage, even with identical source. Same binary
   re-run → identical output (deterministic run-to-run), but a rebuilt binary shifts these 7.
   Example: `forms` gains/loses its "V-Sync" dropdown row label between builds. This means a
   screen's render depends on residual state left by a previously-rendered screen (consistent
   with the `Singleton map is missing value for component 15 (ToastRoot)` warnings emitted during
   `make screenshots`). Impact: makes screenshot baselines flaky and hides real regressions.
   **Workflow guard while fixing:** always re-baseline from the current clean binary immediately
   before applying a patch, so before/after diffs are same-binary-lineage. See the dedicated
   "Batch renderer state leak" section below for the investigation.

---

## Game Mockups

### modals — **BROKEN, top priority**
- **[high]** Not a modal at all: trigger controls are scattered as loose widgets on the base
  panel; no centered dialog, no dimmed backdrop.
- **[high]** Purple "Notice" button floats completely disconnected in the upper-middle
  (~x705, y279), no container/row.
- **[medium]** Huge empty gap between the "Basic:" top panel and the "Blocking:" row at the
  bottom — modal content never renders.
- **[medium]** Bottom "Blocking:", "Background Button", "Clicks: 0" are three boxes with
  mismatched widths stranded bottom-left, not grouped.
- **[medium]** No overlay/backdrop dimming anywhere — the defining modal feature is missing.

### advanced_modals
- CLEAN — initial state (no modal open); grid + status bar aligned. (Manually confirmed.)

### empire_tycoon
- **[high]** Currency "$1,250,980" (top-right) overlapped by a cloud sprite drawn over the digits.
- **[high]** Bottom-left "GlobalChat / DevTeam" chat popup collides with "Next Milestone" label
  and its 65% progress bar.
- **[medium]** Empty yellow box below the "Upgrades" side button (~x70, y480) — missing icon/label.
- **[medium]** In "Current Projects", a third "^" trend arrow floats with no project row.
- **[low]** "Rides" left icon has a red X overlaid; top hamburger icon appears doubled (~x970, y50).

### cozy_cafe
- **[high]** "Emma - Cappuccino" name runs into the "Patience:" label (~x810, y192) — overlap.
- **[medium]** "Rating:" stars render as empty outlined boxes, not filled star glyphs.
- **[medium]** Teal circle floats alone below the panels (~x314, y547), disconnected.
- **[low]** Top "Rating / Customers Served" panel slightly clipped at the very top edge.
- **[low]** Small empty box stranded bottom-right (~x1245, y698).

### shop_interface
- **[high]** Item icon badges on far left (SWD, ARM, TCH) clipped by the list panel's left edge.
- **[high]** List columns inconsistent per row: Iron Sword has no category; selected Health
  Potion shows name+price but no category/stock; the "Magic / 3 Low / 320 G" row has no item
  name; Torch has no stock/price; Steel Shield has no price. Columns don't line up.
- **[low]** Row striping/selection highlight (Antidote band) doesn't align to cell boundaries.

### potion_crafting
- **[high]** Ingredients panel 2nd row right: the "H" ingredient shows only its icon, no
  name/quantity text (cf. "Fire Root x3") — empty/collapsed cell.
- **[low]** Recipe timer labels "60s"/"20s" stranded far left, not under their centered names.
- **[low]** Recipe list inconsistently styled (only "Mana Elixir" yellow; some entries lack a
  description line) → uneven vertical spacing.

### kart_select
- **[high]** Stat bars misaligned with labels: top red bar has no label; "ACC 6/10" aligns to
  the orange bar and "HND 9/10" to green (shifted by one row); "WGT 4/10" has no bar (off-by-one).
- **[medium]** Kart grid slots inconsistent: "Rumble"/"Turbo"/"Blaze" show empty tiles while
  others have colored circles; labels don't line up under tiles.
- **[low]** Empty tile at bottom-right of the 2x3 grid.

### race_results
- **[medium]** Results table 3rd-place row: TIME (2:35.100) and +10 points shown but the RACER
  name cell is empty.
- **[low]** Data mismatch in CUP STANDINGS (YOU 1st w/ 38 pts, Zippy 2nd w/ 42 pts) — logic, not layout.

### neon_strike
- **[medium]** Left ability slots ("Recon"/"Shield"/"Strike") have inconsistent label positions;
  "Shield"/"Strike" boxes appear empty (missing icons).
- **[low]** Two loose icon boxes top-left (drone + crossed-out) float unlabeled.
- **[low]** Bottom-right "x2" grenade tile has a tiny "1" clipped at its corner.
- **[low]** HUD baselines slightly off ("35/210" vs "80 HEALTH / 45%"). Mostly intentional HUD.

### rubber_bandits_menu
- **[medium]** Green dot floats near top-center (~x848,y55) and a yellow star badge top-right
  (~x1227,y97) — stray elements.
- **[medium]** "SELECT CHARACTER" panel: LB/RB at far edges with a large empty gap; character
  preview region empty/collapsed.
- **[medium]** "Supporter pack available" panel: grey-on-pale-yellow text nearly unreadable.
- **[low]** "Change the game's settings" tooltip bar pinned to and cut off by the bottom edge.

### aim_chat
- **[medium]** Stray thin blue vertical bar stranded at the far left of the message-input row
  (misplaced caret / leftover element).
- **[low]** Scrollbar sits detached outside the white message area's right border.
- **[low]** Text-input underline spans only part of the field width, floats below placeholder.

---

## Settings screens (game-style)

### setting_row_showcase
- **[high]** Massive column misalignment: labels ("Vibration", "OPTIONS", "VOLUME", "Master/Music
  Volume") at ~x200 while all controls float at ~x950-1010, dead gap across every row.
- **[high]** Top three controls (top-right checkbox-toggle, radio dot, "Vibration" pill) have no
  visible aligned labels — orphaned widgets upper-right.
- **[medium]** "English"/"High" stepper rows have no left labels.
- **[medium]** Large empty band between "OPTIONS" (~y283) and "VOLUME" (~y427) headers.

### powerwash_settings
- **[high]** Center value column: 9 value rows overlap/z-fight (an open dropdown drawn over the
  others); "2x" appears twice back-to-back.
- **[high]** `<`/`>` arrows misaligned with value rows; several rows miss the right `>` arrow.
- **[medium]** 6 left labels vs 9 value rows — nothing lines up.
- **[low]** Bottom tab labels truncated: "GEN...", "GA...", "AUDIO", "CON...".

### parcel_corps_settings
- **[high]** Rows misaligned; labels ("Full Screen", "Texture Quality", "VSync", "Audio/Music
  Volume") straddle the border between two rows and overlap the colored icon dots.
- **[high]** "2560 x 1440" value right-aligned with no left label; row's icon is a bare red circle.
- **[high]** "MSAA" row (disabled) `<` selector overlaps the "Texture Quality" row below.
- **[medium]** Bottom "OK/Cancel/Apply" buttons clipped off the bottom edge.
- **[medium]** "Subtitles"/"Full Screen" toggles overlap internal divider lines.
- **[medium]** "Language" row: `<` + "English UK" separated by a gap, no right `>` — asymmetric.
- **[low]** Top-right chat/killfeed columns' baselines don't align; text near right edge.

### sports_settings
- **[high]** First row under "Display" is empty — `<` and `>` arrows but no label/value.
- **[medium]** "GAMEPL..." first tab truncated with ellipsis.
- **[medium]** "1.9" row (bottom of Display) has arrows + value but no left label.
- **[medium]** Inconsistent steppers: "Dynamic resolution" has `<` but no `>`; "Framerate
  target" (66 FPS) and "Motion blur" (7) show `>` but no `<`.

### mini_motorways_settings
- **[high]** Top tab bar "Vi... Au... La..." heavily truncated — strip too narrow.
- **[high]** Massive empty void center/left; controls (toggle, orange button, radio, "Default >")
  stranded far right, labels disconnected — layout collapsed.
- **[medium]** Orange rectangular control + small circle below it float unlabeled mid-right.
- **[medium]** "OK"/"Cancel" centered low, not in a dialog; "OK" text nearly clipped, mismatched
  button styling/size.
- **[low]** "Night Mode OFF" label widely separated from its toggle.

### flight_options
- **[high]** Low contrast on selected rows: "High-G Turn Settings" / "Reduced Collision Damage"
  light text on light-blue fill, hard to read.
- **[medium]** Tab labels truncated ("FLIGHT...", "KEYBO...", "FLT STI...", "GRAPHI...").
- **[medium]** Tab row spacing badly uneven (left pair, big gap, right cluster).
- **[medium]** Bottom controls disconnected: "Cancel" centered mid-screen while "OK/Esc/BACK"
  sit lower-left.
- **[low]** Large empty dead region across the middle/bottom.

### islands_trains_settings
- **[high]** Whole screen washed out by translucent greenish overlay bands; section headers
  ("DISPLAY"/"CONTROLS"/"AUDIO"/"TUTORIAL"/"RESET TO DEFAULTS") nearly invisible.
- **[high]** Overlapping semi-transparent panels/bands — z-order/opacity problem across full screen.
- **[low]** "KEYBOARD"/"PLAY TUTORIAL" disabled rows blend into the low-contrast background.

### deadspace_settings
- **[high]** Tab row (Audio, Language & Subs, Accessibility, Policies & Licenses, Credits) jammed
  into the right half; large dead zone across the left of the header.
- **[high]** "Credits" tab + "Apply" button flush to the far-right edge with almost no padding.
- **[high]** "SELECT" appears duplicated, stacked on two lines, bottom-center — stray label.
- **[medium]** "Resume Game" (top nav) text touches/overlaps its box's top border — bad centering.
- **[medium]** SETTINGS panel content region empty under the tabs — large collapsed area.
- **[low]** Two partial corner-bracket fragments cut off bottom-left (~x100-250, y700).

### casual_settings
- **[medium]** "Data" section header (right column) has no content beneath — entire right half of
  the card empty.

### kirby_options
- **[high]** Lower ~two-thirds of the screen empty — content missing/collapsed below the tab row.
- **[high]** Large yellow center button has no label text.
- **[medium]** "Sound" tab stranded far right, big gap from the "Controls"/"Display" pair.
- **[low]** Top-row buttons (L, Mail, Star, R, Options) scattered; "Options" pill hangs below row.

### fighter_menu
- **[high]** Tab bar broken/misaligned: "Offline"/"Customize" left, big gap, then "Sound &
  Language | Network Options | Button Settings" floating mid-right; empty tab slot ~x300-420.
- **[high]** Two "^" arrows + a dash float dead-center (~x650,y360), disconnected.
- **[medium]** Right "System Options" preview panel has an empty white box below the @ icon.
- **[medium]** Bottom hint ": Select" missing its button glyph (others have theirs).
- **[low]** Huge empty middle/left region.

---

## Component galleries

### radio_buttons
- **[medium]** Left "Fruit" group: "Orange" is a label with NO radio circle while "Watermelon"
  below has one — first option missing its button.
- **[medium]** Right "Color" group: "Red" plain text with NO circle (Green/Blue/Yellow have them)
  — same first-option defect.
- **[low]** "Color" header rendered as a filled teal box, inconsistent with plain "Fruit" header.

### checkboxes
- **[high]** Stray checkmark "v" glyphs float disconnected: one far-right in "Multi-Select"
  (Option C row), one top-right of the purple "Choice" panel, three small boxes below "Box Only".
- **[high]** Overlap/z-order: a purple box overlaps the "Multi-Select" panel; "Disabled OFF" grey
  panel overlaps another panel with an arrow protruding (~x380,y510).
- **[medium]** Mismatched row widths; dark notch protruding at bottom of "Multi-Select" panel.

### toggle_switches
- **[high]** "Notifications" row: toggle is a bare white circle with a stray "|" glyph to its left
  and no track/pill — switch track missing/collapsed.
- **[high]** Row below Notifications is a large empty dark bar with a knob at its right end and no
  label — stranded control overlapping Notifications' spacing.
- **[medium]** Small maroon toggle with "o" (~x858-912, y238-267) floats with no label.
- **[medium]** "Preferences": three empty rounded bars (~y365-505) with a stray "v" glyph — labels
  missing.
- **[medium]** "Disabled": "Unavailable Option (OFF)" button overlaps a second grey bar and runs
  to the bottom edge (clipped).

### stepper_showcase
- **[high]** Right panel: "HealerWarriorMage" rendered with no spaces between the three words.
- **[high]** Left panel top row shows only a `>` arrow with no label/value; "English" row has a
  value but no label and no arrows.
- **[medium]** Right-panel description "Heavy armor, melee attacks, high defense." spills to/past
  the panel's right border.
- **[low]** Left-panel rows unevenly spaced.

### dropdowns
- **[high]** "Ruby v" dropdown (bottom-right) floats with no surrounding panel/card.
- **[high]** "Custom Indicators" + "Option A [-]" (bottom-left) sit outside any container.
- **[medium]** "Many Options (12)" panel (top-right) has a red/pink border unlike the others and
  an empty body (no options shown).
- **[low]** "Disabled" panel body empty — large collapsed area.

### circular_progress
- **[high]** "Thin Ring" panel has a large empty rounded-rect tab protruding from its right side
  (~x350-510, y380-475) — stranded element.
- **[high]** "52px" (~x640) and "80px" (~x1100) labels float alone with no accompanying ring.
- **[medium]** Panel usage inconsistent: "Basic"/"Animated" rings have no panel while others do;
  panel sizes/positions uneven.

### meters_gauges
- **[high]** "XP" bar (top-right): yellow fill overflows past the right end of the track; the
  green-over-yellow "84%" layering looks broken beyond the container.
- **[medium]** Orphaned partial circular gauge arc floats between "Animated Progress" and
  "Circular Gauges" (~x775, y355).
- **[medium]** Two stranded rounded button stubs poke out of the right edge (~x1210,y407 and
  ~x1230,y560), clipped at screen edge.
- **[medium]** "Interactive Control" section empty aside from its title.
- **[low]** Third circular gauge has a green ring but no % label (others show 25%/50%).
- **[low]** "1%" progress label with barely-visible fill; "Health: 65%"/"Shield: 29%" chips
  overlap the fill start.

### buttons
- **[high]** "Sizes:" panel (second row) completely empty — size samples missing/collapsed.

### button_variants
- **[medium]** Bottom theme row: "Ghost" variant labels extremely low-contrast, nearly unreadable.
- **[medium]** A faint "Ghost" label stranded alone in the empty middle column between "Neon Dark"
  and "Sage" panels.

### cards
- **[high]** "Nested" section: white "Semi-Transparent" title box overlaps the translucent panel;
  "Item 1" label clipped where the box's left edge cuts through it (~x555-615).
- **[high]** "Item 1" brown box overlaps the left edge of the translucent "Item 2"/"Item 3" panel
  — z-order collision.
- **[medium]** Large white nested card on the left is empty — missing child.
- **[low]** "Uses opacity for a translucent effect." caption floats far right, disconnected.
- **[low]** Bottom "Theme Colors" row: "Surface" swatch missing its color box.

### bevel_borders
- **[high]** Column headers "3px"/"4px"/"5px" don't align to any boxes — float over gaps between
  the Raised and Sunken rows.
- **[high]** "Raised" row has only two boxes ("2px","3px") while "Sunken" has six ("1px"-"6px");
  Raised missing 1/4/5/6px, big empty area on its right.
- **[medium]** Raised-row boxes shifted left relative to Sunken-row boxes — grid doesn't line up.

### nine_slice_borders
- **[high]** "Double Width" label overlapped/clipped by the right ornamental border of its frame.
- **[high]** "Double Ornate" label overflows and is clipped by the right border.
- **[medium]** "This is a dialog using 9-slice. Corners..." body text truncated/clipped at the
  right edge.
- **[low]** Large empty region between title bar and "Wide Panel" box.
- **[low]** Section headers sit tightly above panels with uneven left-edge spacing.

### example_borders
- **[high]** Row 5 "Per-Side": "Bottom Only" and "Left+Right" labels collide and overflow —
  reads "Bottom OnlyLeft+Righ".
- **[high]** Row 2 "Hard Shadow"/"Soft Shadow" labels clipped/overflow the right edge of cards.
- **[medium]** Row 3 "Outline Only" label overflows/clips the right edge.

### decorative_frame
- **[low]** KraftPaper frame (left): dark-brown backing layer offset up-left of the tan fill,
  exposing rounded corners — likely the intended "layered" look. Otherwise clean.

### decorators
- **[medium]** Filled square overlaps the first letter of "ONLINE" (~x213, y348).
- **[medium]** Two stranded L-shaped corner brackets float in the gap between left/center panels
  (~x425-435, y310 & y400) enclosing nothing.
- **[low]** "SECTOR 7-G" text sits on top of the grid overlay (low readability).

### separators
- **[medium]** Right card "Color Gallery" off-by-one: "Blue" has a blue line, "Green" has NO line,
  "Purple" has a purple line, bottom orange line has NO label.
- **[medium]** Right card top nav: orange vertical separator floats alone at far right with empty
  space around it.
- **[low]** Left column large empty upper-left quadrant; "Partial width - 60% centered" caption
  sits well below the separators it describes.

### auto_text_color
- **[low]** "Default Behavior" row lopsided: "Dark Blue" far left, "Dark Green"/"Light Green" far
  right, big middle gap.
- **[low]** "Explicit Control Options" row: both buttons pushed left/center, right third empty.

### accessibility
- **[medium]** "Auto-Contrast on Various Backgrounds" grid: row 2 has only 7 swatches vs row 1's 8,
  with a conspicuous empty gap in the 3rd column — missing/collapsed swatch or broken wrapping.

### colors
- **[low]** Top theme-color row shows only "Primary" (far left) and "Error/Danger" (far right)
  with a large middle gap — likely intended, but reads as missing swatches.

### toasts
- **[medium]** "Quick (displays 1s)" button: text overflows — ")" sits on/past the right edge,
  "1s" clipped.
- **[medium]** "With Undo Action" toast: text touches the right edge of its pill; "Undos: 0" sits
  cramped just outside the pill.
- **[low]** "INTERACTIVE TOASTS" header is a thin full-width strip lacking the taller panel bg the
  other sections have.
- **[low]** Uneven horizontal spacing between "Info Toast" and "Warning Toast".

### tray
- **[low]** "H-Beta" horizontal tray item sits alone/centered with no siblings and no section
  label (unlike the labeled "Vertical Tray").
- **[low]** Large empty region in the lower half below "V-Alpha".

### tab_container
- **[high]** Large empty white region fills the top ~40% of the content panel; text block pushed
  low — missing element or bad vertical alignment.
- **[medium]** Active tab is "Profile" but content shown is account info — wrong panel content for
  the selected tab.

### tabbing
- **[medium]** Only a single "OPTION A" button present, then a large empty region to the "Total
  Clicks: 0" footer — sibling options (B, C, …) appear to have failed to render.
- **[low]** Footer sits well above the panel bottom; lots of unused space.

### pagination
- **[high]** Lone `>` next-arrow floats disconnected mid-screen (~x603, y385).
- **[high]** "Option Selector (5 options)" row broken: `<` + "Red" far left, huge gap, "Purple" +
  `>` center-right — not a contiguous group.
- **[medium]** "Selected: Blue" label bottom-left contradicts the visible options (Red, Purple).
- **[medium]** Top ~60% of the panel empty.

### navigation_bar_demo
- CLEAN — evenly stacked, centered, consistent spacing, arrows aligned, legible.

---

## Text / typography

### text
- **[high]** Two empty dark boxes in the right column (below "Primary Gold Text", ~y295 & y388)
  contain no text — missing color-treatment samples.
- **[medium]** "Body text for readable content that flows natu..." truncated with ellipsis.
- **[low]** Large gap between "Text Size Hierarchy" header and its body line — missing heading size.

### text_input
- **[high]** Missing field labels: focused Username, Password, and Search fields have no labels
  while Email/Bio/Readonly/Disabled do — inconsistent.
- **[medium]** Uneven vertical spacing — larger gaps above Password and Search fields.

### text_input_sizes
- **[high]** Largest (bottom) input has no size label while all others (24px-80px) are labeled.
- **[medium]** In taller boxes (60px/80px/largest) the "Type here..." placeholder is top-aligned,
  not vertically centered.

### text_overflow (intentional debug demo — flag anything unintended)
- **[medium]** Middle empty dark box below the "44x44 box" row (~x660-760,y420-465) is empty with
  no label/content — reads as stranded.
- **[low]** In the "different text sizes" row, the 4th box is empty (no "ABC") while 3rd/5th show
  overflow borders; gaps between the 5 boxes are uneven.
- **[low]** Empty small box under "Good: Text Fits Container" — likely intentional tiny-container
  demo but reads as stranded.

### text_shadow
- **[high]** Light gray/lavender background makes most labels ("NO SHADOW" yellow, "COLORED",
  "COMBO", "LIGHT" white) extremely low-contrast; the white "LIGHT" text is nearly invisible.
- **[high]** "Shadow Offset Examples" (right): offset labels (2/4/6/8/10px) at far right almost
  unreadable AND vertically misaligned with the "SHADOW" samples; three SHADOW samples crammed
  together overlapping (y≈275-430).
- **[medium]** "HARD" sample sits inside a dark panel while other samples are on the bare light
  background — inconsistent container treatment.
- **[medium]** Large empty regions (bottom half, right side).

### text_stroke
- **[medium]** "WITH STROKE" label (top) has a faint ghost/duplicate behind it; "BOLD" sample
  shows a doubled/overlapping outline artifact.
- **[low]** Below the "WHITE" light panel, faint clipped text "invisible" partially cut off by the
  panel edge / green tip bar.
- **[low]** "Outline Thickness" samples: top "STROKE" has no thickness label (labels start at
  "4px - medium" for the second) — column offset by one row.

---

## Layout / stack primitives

### layout
- **[high]** "Left Panel" missing: only "Center Panel"/"Right Panel" shown, large empty gap on the
  left — row not balanced.
- **[medium]** White stacked panel contains only "Stacked Item 1" with a big empty area below —
  remaining items missing/collapsed.
- **[low]** "Stacked Item 1" offset right within the outer panel — uneven left/right padding.

### layout_patterns
- **[high]** Only three colored boxes (1,2,3) render top-left; the vast majority of the large
  panel is empty — missing/collapsed content.
- **[low]** Faint stray arc/artifact top-left above box "1".

### layout_bug_repros (intentional bug-repro screen — most items expected)
- **[medium]** Title "Layout B..." clipped/cut off top-left with a stray circular icon overlapping
  the "B"; "[NOT" below is a truncated label fragment. (This is chrome, not a demonstrated bug.)
- **[high]** (intended) red "percent(1.0) child" overflows its cyan container.
- **[high]** (intended) "Tab B"/"Tab C" docked at far top-right screen origin outside the green border.
- **[high]** (intended) "Child 1" touches/overflows the top border of its yellow container.
- **[high]** (intended) blue "BLUE mk(2000) ON TOP?" drawn over red (z-order).
- **[medium]** Toggle B (middle, green panel) has no visible label — bare knob + tiny "o".

### vstack_showcase
- **[high]** Top-left panel's colored bars (pink+green) flush against the top/left inner edges
  with no padding while other panels have padding.
- **[medium]** Cyan title bar and pink/blue header tabs extend to/past panel edges and overlap
  rounded corners.
- **[medium]** Bottom row clipped at screen bottom — several panels cut off (~y690).
- **[medium]** "Stretch" panel nearly empty with one small box at the bottom — content collapsed.
- **[low]** Far-right top panel completely empty (no bars) while siblings have them.
- **[low]** "Sidebar + Content": sidebar and content rows vertically misaligned.

### hstack_showcase
- **[high]** Top row shows "Item 2"/"Item 3" but "Item 1" missing — large empty gap on the left.
- **[high]** Right edge clipped: an "A" tab and a green element cut off (~x1230+).
- **[medium]** "Toolbar Pattern" panel overlaps/crowds the "A" button to its right.
- **[low]** Lone stranded cyan square in the leftmost panel of row 2.

### flex_alignment
- **[medium]** Blue "2" box floats alone in empty space (~x290,y273) with no container/siblings.
- **[medium]** "Around" panel (top-center): a "1" element clipped at its bottom edge.
- **[low]** Left-side blue 1/2/3 boxes scattered with inconsistent gaps — ambiguous groupings.

### self_align
- **[high]** The large central SelfAlign panel is completely EMPTY — the Left/Center/Right demo
  elements render BELOW the panel instead of inside it (content escaped its container).
- **[low]** Tiny orange sliver clipped at the empty panel's bottom-right corner.

### absolute_positioning
- **[low]** Section 1: pink `translate(300,10)` box overlaps the green `translate(100,10)` box's
  label — likely intentional translate demo.
- **[low]** Section 3: pink "BADGE" on Flow C pokes above the panel's top edge — expected for an
  absolute-overlay demo but breaks the outline.

---

## Scroll / drag / tree

### scroll_view
- **[high]** Vertical Scroll: Item 1, Item 2, then a large empty gap where Item 3 should be, then
  Item 4 — missing/collapsed row.
- **[high]** Horizontal Scroll: boxes "1","2", gap, "4" — box "3" missing.
- **[medium]** Grey rounded pill floats stranded top-left (~x200-395,y165-190) — orphaned element.

### scroll_click_bug
- **[high]** Button list gap: Button 0/1/2 shown, Buttons 3 & 4 entirely missing, then Button 5/6.
- **[high]** "Button 6" clipped at the bottom edge of the panel — overflows the clip region.

### scroll_clip_bug
- **[high]** Column 1 "Control": large uneven gaps, many rows missing (shows 6,8,9,10,14; absent
  1-5,7,11-13); bottom blue row clipped at panel edge.
- **[high]** Column 2 "ClipChild": uneven gaps with empty bands (Clip 1,4,7-11,13,14).
- **[high]** Column 4 "Scroll in scroll": Inner 2 missing; a green line overflows below "Inner 13"
  past the panel bottom.
- **[medium]** Column 3 "Nested wrapper": uneven gaps; "Nest 14" clipped at panel edge.

### drag_drop
- **[high]** Third column ("Setup CI"/"Update docs") has no column header/title, unlike "To Do (3)"
  and "In Progress (3)".
- **[medium]** Inconsistent column containers: "To Do" has a full panel bg; "In Progress" and the
  third group have none.
- **[low]** Large empty region below the To Do items (drop-zone, but unbalanced).

### horizontal_drag
- **[high]** "High Priority (2)" claims 2 items but its lane is completely empty.
- **[high]** "Medium Priority (3)" claims 3 but shows only 2 tags — one missing.
- **[medium]** Bottom lane (unlabeled) entirely empty — stranded container with no header.

### file_tree
- **[high]** Very uneven row spacing: normal gaps between baseline_screenshots/docs, then a large
  blank gap before "resources/", another before "src/", and a large gap before
  "PROGRAM_ISSUES.md" — missing/collapsed rows or broken vertical layout.
- **[medium]** "PROGRAM_ISSUES.md" lacks the ">" prefix and uses a different indent than the
  directory rows — misaligned.

---

## Themes / images / misc

### themes
- **[high]** Right content panel widgets extend to the right edge; top-right shows a stray/duplicate
  rounded panel corner peeking out behind the main panel (~x1150-1260, y15-110).
- **[medium]** "A" button (~x680-875,y145-178) floats centered with a single-letter label and lots
  of empty space around it.
- **[medium]** Horizontal slider/segmented bar (~x290-1140,y205-240) has a blue segment amid green
  ones with no labels — reads broken.
- **[medium]** Large empty middle-right region (y≈360-520) with only a small "Surface Text" box
  floating at the right.
- **[low]** "Ocean Navy" sidebar button shows chevron artifacts "> Ocean Navy <" and is offset
  from the buttons above/below.
- **[low]** Checkbox row shows a stray "v" glyph near the right (~x890) with no adjacent control.

### images
- **[medium]** Icons have stray clipped fragments to their right (sprite-sheet bleed) next to
  Star/Trophy and in the "Icon Row".
- **[medium]** Row 2 misaligned: "Settings" label far left while the red play-forward icon is
  centered, no visual connection.
- **[medium]** "Icon Row": "Settings" label overlaps the first (star) icon; captions sit under
  clipped icons.
- **[low]** Row 3 gear icon has a clipped fragment; its "Settings" label offset right.

### language_demo
- **[high]** Large left speech-bubble panel is empty except the "Continue" button — dialogue text
  missing.
- **[medium]** "KO: ..." and "JA: ..." lines float on the right, disconnected from any panel.
- **[low]** Empty rounded bar at the bottom of the bubble panel.

### simple_button
- **[low]** Large empty region at the top of the white card (above "No clicks yet...") — content
  bottom-weighted, possible missing header.
- **[low]** "Secondary" button wider than and not aligned to the "Small/Pill/Buttons" row below.

---

---

## Batch renderer state leak — INVESTIGATION (root cause NOT the pass count)
**Symptom:** In `make screenshots` (single-process `--headless-screenshots` batch), 7 screens
render differently across binary rebuilds despite identical source: `forms`, `images`,
`hstack_showcase`, `file_tree`, `islands_trains_settings`, `mini_motorways_settings`,
`flex_alignment`. Same binary re-run → identical (deterministic). Rebuilt binary → these 7 shift.
Example: batch `forms` is MISSING its "Sliders"/"Progress Bars" section headers, "Brightness: 50%"
slider, and dropdown labels (SFX/Fullscreen/V-Sync `v` indicators) that a full render shows.

**Hypothesis TESTED AND REJECTED — it is not the settle-pass count.** The batch loop at
`src/headless_screenshots.cpp:429` runs only 2 tick+render passes. I tried bumping this to a
converge-until-stable loop (2..12 passes, break when summed UIComponent rects stop changing).
Result: it did NOT fix any of the 7 screens (batch `forms` was byte-identical before/after). The
only 4 screens it changed (`meters_gauges`, `text_input`, `text_input_sizes`, `animation_looping`)
changed because extra passes ADVANCE TIME-BASED ANIMATIONS (meters "Animated Progress" 1%→6%,
text_input caret blink) — a mild regression, not a fix. So layout IS settled within 2 passes; the
extra passes only move animations forward. Reverted.

**What the evidence actually points to:** the same screen rendered through the FULL game loop
(via e2e `goto_screen forms; wait 0.5`) is COMPLETE and correct, and is STABLE regardless of which
screen preceded it (rendered `forms` after `dropdowns` and after `simple_button` → byte-identical,
and both match the isolated settled capture). So the incompleteness is specific to the BATCH
renderer's reduced pipeline in `create_screen_systems()` (headless_screenshots.cpp:289) — it wires
up a different/smaller set of update+layout+render systems than the real game loop. The missing
headers/labels are elements whose emission or layout depends on a system the batch path doesn't
run (or runs in the wrong order). The rebuild-to-rebuild variation is because whatever partial
state those elements land in is sensitive to entity-ID allocation order, which shifts on rebuild.

**Next step (not yet done):** diff the system registration in `create_screen_systems()` against
the real game loop in `src/game.cpp` and against the e2e runner's setup — find which
update/layout/render system the batch omits (candidate: a UI layout/settle system, or a
before/after-UI update). Add the missing system to the batch path, then re-baseline and confirm
batch `forms` matches its settled e2e capture.

**Impact:** makes screenshot baselines flaky and can hide/false-flag regressions. Fix BEFORE
trusting screenshot-diff regression gating in CI. Workflow guard meanwhile: re-baseline from the
current clean binary immediately before each patch apply.

---

## Screens reported CLEAN (verify against regressions later)
- advanced_modals (initial state), navigation_bar_demo, adaptive_scaling,
  animation_interactive, animation_basic (mid-animation caveat), decorative_frame (mostly).

## Animation caveat
`animation_basic`, `animation_declarative`, `animation_looping`, `animation_spring` are
single-frame captures of running animations — several "missing"/faded/collapsed boxes are likely
mid-animation states, not true layout bugs. Re-audit with `--time-scale` at a settled keyframe
before treating these as bugs:
- **animation_looping**: "Glow" column has a label but no box (medium — verify).
- **animation_spring**: "Boing!" tiny collapsed square; "Drop"/"Rocker" have labels but no boxes
  (medium — verify).
- **animation_declarative**: irregular grid gaps; "Appear" cell near-invisible low contrast.

---

## Suggested fix order (highest leverage first) — updated 2026-07-28
Reconciled against shipped commits. `[shipped]` = fix landed, re-verify from fresh
screenshots; `[open]` = still needs work.

1. `[shipped, re-verify]` **modals** — centering/stacking/dialog-layout + RealWorldModals.
2. `[shipped, re-verify]` **First-child-of-group off-by-one** — subsumed by the render-command
   stable-sort root-cause fix (⭐ top of file). Re-check `radio_buttons`, `stepper_showcase`,
   `toggle_switches`, `setting_row_showcase`, `sports_settings`.
3. `[open]` **Settings row two-column alignment** — `setting_row_showcase`,
   `powerwash_settings`, `parcel_corps_settings`, `shop_interface`. (Align to shared edges,
   STYLE_GUIDE §3.) Re-audit; the render-sort fix may have resolved several.
4. `[partial]` **Text overflow / right-edge clipping** — word-wrap shipped; still verify
   `example_borders`, `nine_slice_borders`, `stepper_showcase`.
5. `[open]` **Scroll/clip container row dropping** — `scroll_view`, `scroll_click_bug`,
   `scroll_clip_bug` (file_tree reported fixed). Highest remaining-value functional bug.
6. `[partial]` **Tab-bar sizing (truncate + cluster)** — sports_settings/fighter_menu fixed;
   re-verify `mini_motorways_settings`, `flight_options`, `deadspace_settings`,
   `powerwash_settings`. (STYLE_GUIDE §5: fit all tabs at 720p in EN/KO/JA.)
7. `[open]` **Content-escapes-panel** — `self_align`, `layout`, `layout_patterns`.
8. `[open]` **Batch renderer state leak** — fix the batch pipeline before trusting
   screenshot-diff CI (see investigation section). Not layout, but gates the whole workflow.
9. `[open]` Per-screen cosmetic cleanups (stray glyphs, orphaned decorators, contrast).
