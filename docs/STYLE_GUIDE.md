# Afterhours UI — Style Guide

How to lay out, color, and word a screen in this codebase so it reads as one system.

> Replaces the old `STYLE_GUIDE_V2.md` (a CSS "scrapbook deck" guide for an unrelated
> web project — no relation to this raylib UI). Principles here are adapted from the
> interfaces.dev `better-layout` / `better-colors` / `better-writing` skills and
> translated into afterhours idioms. No CSS: this is a raylib immediate-ish ECS UI.

The design tokens are real code, not aspirations:
- **Palette** — `Theme::create().with_palette({...})` in `src/theme_presets.h`
- **Type scale** — `FontSizing` tiers `Small 14 / Medium 20 / Large 28 / XL 38` (`ui/theme.h`)
- **Corner radius** — `.with_roundness(f)` (0 = square, ~0.12 = the cozy default)
- **Sizing** — element dims are `pixels`, `children` (fit to content), `percent` (of
  parent), or `screen_pct`. Prefer `children` / `percent` over hardcoded `pixels`.

---

## 1. Color — semantics, not decoration

Use the semantic palette slot, never a raw `Color{...}` literal in a screen. One meaning
per slot; a screen that needs a color it can't name from this list needs a theme change,
not an inline literal.

| Slot | Means | Use for |
| --- | --- | --- |
| `background` | the page | outermost fill |
| `surface` | a raised region | panels, cards, rows |
| `primary` | the app's main action/identity | primary button, panel titles |
| `secondary` | supporting action | secondary buttons, accents |
| `accent` | draws the eye, sparingly | highlights, selection |
| `error` | something is wrong | error text, destructive confirm |
| `font` / `darkfont` | text on light / dark | body text |
| `font_muted` | de-emphasized text | captions, disabled, metadata |

Rules:
- **One primary action per view.** If two things are `primary`, neither reads as primary.
- **Contrast is non-negotiable.** Text vs its background must clear WCAG AA (4.5:1 body,
  3:1 large). Existing themes annotate their ratios inline (e.g. `// dark teal (5.8:1)`) —
  keep that discipline when adding or editing a theme. `font_muted` must *still* pass 4.5:1;
  muted is not an excuse for unreadable.
- **Never encode state in color alone.** Selected/disabled/error must also change shape,
  weight, an icon, or a label — colorblind users and grayscale screenshots must still parse it.
- New palette values live in `theme_presets.h`, prefixed by intent, with the ratio in a
  comment. Don't scatter theme colors across screen code.

## 2. Type — one scale, restraint wins

- Size by role using the `FontSizing` tiers, not arbitrary values: XL for a screen title,
  Large for section headers, Medium for body/controls, Small for captions/metadata.
- **A role is one decision: size + line-height + weight together.** Don't pick a size and
  then improvise spacing/weight per screen. Headings ~1.1 line-height; body 1.4–1.6;
  anything wrapping to 3+ lines needs ≥1.4.
- **Emphasis is a weight step, not a size step.** Bold the label; don't bump it to the next tier.
- **A child heading never renders larger than its parent;** a heading is never smaller than
  body text (except a deliberate small-caps overline).
- **Max ~2 sizes visible in one group.** A row of settings is all Medium; the section
  header above it is Large. Mixing four sizes in one panel reads as noise.
- **Long-form measure: 60–75 characters per line** (~560–680px at Medium). Cap the wrap
  width on descriptions/tooltips/dialogue; full-width body text is hard to read.
- Let text define the box: size controls from their label (`children` width), never a
  fixed pixel width sized to one English string. See §5 (i18n).
- Language scaling already lives in the theme (`language_fonts` size_scale for KR/JA) —
  don't re-scale per screen.

## 3. Layout — space communicates structure

**Group with space, not lines.** Negative space is the primary grouping tool; a `surface`
panel behind a group is second; separator lines are a last resort for dense lists.

- **The 2× rule:** the gap *between* groups must be ≥ 2× the gap *within* a group. If rows
  in a section sit 8px apart, sections sit 16px+ apart. Uniform spacing + dividers to
  compensate is the smell this replaces.
- **Align to shared edges.** Pick a small set of alignment edges and put everything on
  them. A label column and a control column each get one edge; every row honors both.
  Stray edges read as broken even when nobody can name why (this is the root of most
  "settings row two-column drift" findings in the audit).
- **Order by importance.** The number the user came for goes top/leading, big; metadata and
  actions trail. Identifying content (name/title) leads its row; actions trail it.
- **One entry point.** The first screenful is a table of contents, not the whole book.
  Push secondary actions behind a menu/tab once they exceed two or three.
- **Controls must look interactive; static things must not.** A clickable element needs a
  fill/border/underline or a consistent control zone (toolbar, footer). A non-clickable
  badge shaped like the buttons next to it collects dead clicks.

## 4. Adaptivity — hold structure until it breaks

Targets: **720p is the baseline** (`make screenshots` renders at 1280×720). Layouts must
also survive the largest supported size.

- Prefer `children`/`percent` dims so panels flex; reserve `pixels` for things that are
  genuinely a fixed size (icons, hairlines).
- Break where the content stops fitting, not at a magic width. Collapse late — keep the
  expanded layout as long as it genuinely fits.
- **Off-screen content needs an affordance:** a scrollbar, a chevron, a peeking next item,
  a "Show N more". A list that ends exactly at the panel edge looks complete and nobody
  scrolls it.
- **Never park a primary action where it clips** — bottom edge of a resizable pane, below a
  fixed modal's fold. Keep OK/Cancel/Apply in stable chrome (a footer row that doesn't
  scroll with the content). Several audit findings are exactly this (OK/Cancel clipped off
  the bottom).

## 5. Internationalization

This UI ships English / Korean / Japanese. Translated strings are longer and taller.

- No fixed widths sized to an English label — use `children` width + wrap
  (`TextOverflow::Wrap`) or ellipsis with an expand affordance.
- No fixed heights on text containers — content grows.
- Tab strips must fit *all* tabs at 720p in *every* language without ellipsizing to
  unreadability (the recurring tab-bar finding). Fill the available width; shrink the font
  before you truncate the word.

## 6. Copy — writing that disappears

- **Clear and brief beats clever.** "Save" not "Commit your changes".
- **One term for one thing.** If the menu says "Archive", the toast says "Archived" — not
  "Moved to storage".
- **Sentence case for UI** unless a theme deliberately does otherwise; be consistent within
  a screen.
- **Buttons are verbs** ("Delete", "Add player"); labels are nouns ("Volume", "Difficulty").
- **Confirm buttons repeat the consequence** — `[Delete save] [Cancel]`, never `[Yes] [No]`
  on anything destructive.
- **Toggle/setting labels describe the ON state** ("Fullscreen", "Subtitles"), never the negative.
- **Never concatenate fragments around a variable** (`"You have " + n + " lives"`) — use a full
  templated string with pluralization, or KR/JA word order and plural rules break.
- **The best error is the one designed out.** When you can't, say what happened and what to
  do — not "Error 4".
- **Empty states earn their space:** one line of what goes here + the action that fills it,
  never a blank panel (a blank panel reads as a bug — see the many "empty right half"
  findings in the audit).

## 7. Focus & navigation (keyboard + gamepad)

This UI already has a themeable focus ring (`Theme::focus`, `focus_ring_thickness` 3px,
`focus_ring_offset` 4px). The model around it:

- **Focus-visible, not always-visible.** Show the ring for keyboard/gamepad navigation;
  don't paint it on mouse/cursor use. Never remove it without a visible replacement.
- **The ring must contrast against everything it crosses** — component fill, panel surface,
  *and* background art — not just one backdrop. Check the whole perimeter (that 4px offset helps).
- **Groups are one nav stop (roving focus).** A tab strip / radio group / toolbar is a single
  Tab/shoulder-button stop with one active member; d-pad/arrows move *within* the group and
  wrap at the ends. Tab/shoulders move *between* groups.
- **Escape/Back dismisses the last thing opened,** in order: tooltip → menu → dialog.
- **Modals trap focus** and disable everything behind them; on open, focus the first control
  (for a destructive confirm, focus the *safe* option); on close, return focus to the control
  that opened it.
- **On a screen change, move focus to the new screen's primary element** — don't strand it.
- **Auto-activate a tab on focus only if its panel renders instantly;** if switching is
  expensive, require a confirm press.

Hit targets: **44px is the floor for primary/controller-distance controls** (already the
default control height); 24px is the absolute WCAG minimum. The *hit area* is the whole
visible extent — a checkbox and its label share one target — and two controls' hit areas must
never overlap (expand up to the neighbor, then stop).

## 8. Motion — subtle, interruptible, optional

- **Add a reduce-motion game option** (this is a real gap — only a test `time_scale` exists
  today). Reduce ≠ off: kill parallax/large cross-screen movement/looping decoration and
  swap slide/scale for a crossfade; **keep** spinners, progress, instant state changes, and
  brief button feedback. A global kill switch uses ~0.01ms (not 0) so end-callbacks still fire.
- **Motion is never the only feedback channel** — every state a movement conveys must also
  change color/icon/label (this is also what makes reduce-motion safe).
- **Prefer state-interpolation over fixed keyframes** for anything interactive: lerp toward
  the current target each frame so a toggle/open/close can be interrupted and retargeted.
  Reserve keyframe timelines for one-shots (screen enter, loading).
- Durations (use the theme easing config):
  - High-frequency (row hover, tab switch, keystroke): **≤150ms, opacity/color only.**
  - Interactive state transition / elevation change: **~200ms ease-out.**
  - Press feedback: **scale to ~0.96** (never below 0.95), ~150ms; make it optional.
  - Screen/menu entrance: opacity + small translate (~12px); stagger groups ~100ms. Exit
    softer/shorter than enter (~150ms vs ~300ms). Don't animate elements already at rest on
    first render.
- Reserve expressive motion for infrequent moments (screen first-load, success, empty state);
  never on high-frequency interactions.

## 9. Elevation & surfaces

- **Concentric radius:** a nested panel's outer radius = inner radius + the padding between
  them. Only matters when layers are close (padding ≤24px); beyond that they read as separate.
- **Shadow for elevation, border for separation.** Use a soft drop-shadow to lift cards /
  dropdowns / modals (transparency adapts to any background); use hairline borders for
  dividers, list separators, and input outlines. On dark themes, layered shadows vanish — use
  a single light ring instead (~1px white at ~8% opacity, a touch brighter on hover).
- **Image/portrait separator:** a 1px neutral ring at ~10% opacity (pure black on light /
  pure white on dark), drawn just inside the edge — never the accent color (tint reads as dirt).
- **Optical alignment beats geometric centering:** nudge a play triangle ~+2px right; give a
  button's icon side ~2px less padding than its text side.

## 10. Icons

- **Outline = inactive, filled = active.** Use the pair as the state signal (active tab,
  toggled bookmark), not interchangeably.
- **One asset, recolored per state** — draw a single glyph the code tints; disabled = ~0.4
  opacity. Don't ship separate default/hover/selected files.
- **Match stroke weight to adjacent text** (regular→1.5px, semibold→2px on a 24px grid) and
  size icons to the text (~1–1.25× cap height) so the pair scales together.
- **Keep icons on native grid sizes (16/20/24)** and design/test at the smallest render size;
  fractional scaling softens them (mind mipmaps).

---

## Definition of Done — screen layout checklist

`LAYOUT_AUDIT.md` audits against this. A screen is done when:

- [ ] **No overflow / clipping.** No text spills or is cut by a container/border edge.
- [ ] **No orphans.** No stray glyph, arrow, dot, or control floating without its row/label.
- [ ] **Every control has its label; every label has its control.** Counts match; first
      row is not missing its control (the off-by-one first-child pattern).
- [ ] **Columns align.** Label column on one edge, control column on one edge, every row.
- [ ] **Grouping via the 2× rule.** Inter-group gap ≥ 2× intra-group; no divider doing a
      gap's job.
- [ ] **Content stays inside its panel.** Nothing escapes or collapses out of its container.
- [ ] **Contrast passes** in the active theme, including selected/disabled rows and muted text.
- [ ] **Fits at 720p in EN/KO/JA** — tabs included, no unreadable ellipsis.
- [ ] **Primary actions never clip;** they live in stable chrome.
- [ ] **No empty panels** — every panel has content or a real empty state.
- [ ] **State is not color-only** — shape/weight/icon (and never motion-only) also distinguishes it.
- [ ] **Focus ring is visible and contrasts** everywhere it lands; groups rove, Tab moves between them.
- [ ] **Motion respects reduce-motion** and is interruptible; nothing critical lives only in a timed element.

Verify from fresh screenshots, not from memory:
```bash
make screenshots            # all screens → output/<screen>_720p.png
make screenshots SCREEN=x   # or --screen x for one
```
