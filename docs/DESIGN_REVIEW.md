# Visual design review of the example screens

Reviewed 2026-08-28 against the committed 1280x720 baselines in
`screenshot-baselines/screens/`.

## Verdict

The screens are not amateurish in the "wrong colours, ugly fonts" sense. The
palettes are pleasant, the corner radii are consistent, and half a dozen
screens (`shop_interface`, `race_results`, `kart_select`, `sports_settings`,
`islands_trains_settings`, `button_variants`) would pass as real product UI.

What is wrong is more specific and more damaging: **the showcase app does not
use the library's own design system.** afterhours ships a modular type scale
(`TypographyScale`, base 20px, ratio 1.25) and an 8pt spacing scale
(`DefaultSpacing`). Across 101 screens, `FontSize::` is used **zero times**,
and 55 screens use raw `pixels(N)` with no scale at all. The result is that
type size is chosen by an auto-fit binary search on a per-string basis, and
padding is chosen ad hoc. Those two facts produce most of the findings below.

The single worst impression-per-pixel problem is that a newcomer's eye lands on
type that changes size between adjacent rows for no reason, and on edges that
miss each other by 2-12px. Both are systemic, both are cheap to fix at the
library level, and fixing them raises every screen at once.

### Method

26 screens read as rendered pixels, prioritising product-like UI over widget
harnesses. All 101 analysed programmatically for background coverage, ink
bounding box, and measured WCAG contrast ratios (sRGB relative luminance per
WCAG 2.2 §1.4.3). Control heights measured by pixel run-length. Library
behaviour verified in `vendor/afterhours/src/plugins/ui/`.

---

# Part 1 — Library-level findings

These are the high-leverage ones. Each is a default or a missing API that makes
every consumer improvise, and the improvisations disagree.

## L1. Text auto-fits its box, so font size is a function of string length

**Principle:** A typographic hierarchy is a small, fixed set of sizes applied by
role. Refactoring UI, "Establish a type scale": pick 5-8 sizes and never
deviate. Size must encode importance, not string length.

**Mechanism:** `ComponentConfig::font_size` defaults to `pixels(50.f)` with
`font_size_explicitly_set = false` (`component_config.h:136-137`). When that
flag is false, `position_text_ex` runs a binary search for the largest size that
fits the element's rect (`rendering.h:441-463`), clamped only to
`MIN_FONT_SIZE = 10.0f` (`rendering.h:278`). Two siblings with the same role and
different label lengths therefore render at different sizes. 82 of 101 screens
never call `with_font_size`, so this is the normal path, not an edge case.

**Where it shows:**
- `empire_tycoon` — in one card: "New Rollercoasser - 80%" and "Expand - 35%"
  render at ~20px, "Develop Robot Masoot - 3100/min" at ~16px. Same list, same
  role, three sizes.
- `powerwash_settings` — the value column reads "2560 x 1440" at ~22px,
  "Borderless Windowed" at ~15px, "Unlimited" at ~22px. The column looks like it
  is shouting and whispering alternately.
- `sports_settings` — the help paragraph's first line renders larger than its
  second line, because each line is fitted independently.
- `setting_row_showcase` — "Music" at ~12px, "Master Volume" at ~17px, in the
  same component.

**Fix:** Make the type scale the default rather than the opt-out. Set
`ComponentConfig::font_size = TypographyScale::base()` (20px at 720p) with
`font_size_explicitly_set = true` as the library default, and keep auto-fit as
an explicit `.with_autofit()` opt-in for genuinely size-constrained cases
(badges, gauge readouts). Raise the auto-fit floor from `MIN_FONT_SIZE = 10.0f`
to `TypographyScale::MIN_ACCESSIBLE_SIZE_720P` (16.0f), which the library
already declares as its own accessibility minimum — a value nothing currently
enforces.

This one change fixes more screens than everything else in this document
combined.

## L2. Two spacing scales exist and they disagree

**Principle:** Material Design's 8dp grid, and Refactoring UI's "define a
spacing scale". Off-grid values are what makes a layout feel hand-nudged.

**Mechanism:** The library ships two independent scales:

| | xs/tiny | sm/small | md/medium | lg/large | xl |
|---|---|---|---|---|---|
| `DefaultSpacing` (`styling_defaults.h:28-38`) | 8 | 16 | 24 | 32 | 48 |
| `Spacing` enum (`layout_types.h:221-243`) | 7.2 | 14.4 | 28.8 | 57.6 | 115.2 |

`Spacing` is screen-percentage based, so at 720p it lands on fractional pixels.
23 screens use **both**. An element on `DefaultSpacing::small()` (16px) beside
one on `Spacing::sm` (14.4px) is 1.6px out of alignment, permanently, and
fractional padding gives soft, off-by-one edges everywhere.

**Where it shows:** `themes` is the clearest case — the content column's rows
alternate between left edge x=283 and x=295, and have **nine different right
edges** (1035, 1040, 1069, 1090, 1122, 1133, 1135, 1163). Nothing in that screen
shares a vertical line with anything else.

**Fix:** Delete the `Spacing` enum or redefine it to return the same integral
values as `DefaultSpacing` (8/16/24/32/48). Round `screen_pct` results to whole
pixels at layout time. Then port the 55 raw-`pixels(N)` screens onto the scale —
the current literal histogram (16, 18, 28, 36, 40, 24, 44, 20, 22, 30, 14, 26,
38, 45, 55, 70…) is every integer, i.e. no scale at all.

## L3. The semantic palette has `error` but no `success` or `warning`

**Principle:** HIG, "Color": use colour consistently to communicate meaning; a
status colour must mean one thing across the app. WCAG 1.4.1 additionally
requires that colour is not the sole carrier of meaning.

**Mechanism:** `Theme` defines `error` (`theme.h:218`) and nothing else
semantic. Consumers substitute whatever is to hand.

**Where it shows:** `toasts` — `ToastShowcase.h:117` maps warning to
`theme.accent` and success to `theme.secondary`. The rendered result is a
**purple "Success Toast"** and a **crimson "Warning Toast"** sitting next to a
red "Error Toast" that is nearly the same hue. On the one screen whose entire
purpose is semantic status colour, the semantics are wrong.

Also `cozy_cafe`, where one pink is used for a menu item (Lavender Latte), the
primary CTA (Serve Next), and an urgency chip (Oliver's "5m") — three meanings,
one colour.

**Fix:** Add `success` and `warning` to `Theme` alongside `error`, defaulting to
green `{80,170,90}` and amber `{225,165,45}`, and route `toast::send_success` /
`send_warning` through them.

## L4. `TextAlignment::Center` has become the house style, and it destroys the grid

**Principle:** Refactoring UI, "align to a grid"; HIG, "Layout": use alignment
to create relationships. Centred labels in a vertical list produce a ragged left
edge, which reads as sloppy even when every element is individually correct.

**Mechanism:** This is not a wrong default — `TextAlignment::None` correctly
falls through to Left (`rendering.h:492-493`). It is a convention problem: the
examples use `Center` 372 times vs `Left` 169. Screen authors copy the examples.

**Where it shows:**
- `islands_trains_settings` — the section labels DISPLAY / CONTROLS / AUDIO /
  TUTORIAL start at x=473, 464, 476, 466. They should all start at x=440, the
  left edge of the rows they label. Four labels, four left edges.
- `mini_motorways_settings` — four setting labels centred as a block, so they
  end at x=911, 900, 946, 932. A settings list with a ragged right edge next to
  a column of toggles that *is* aligned.
- `shop_interface` — item names are centred in their column (Iron Sword 133-218,
  Leather Armor 116-235). Standard table practice: text columns left, numeric
  columns right, headers matching their column. Here the "Item" header sits at
  x=146 over names that centre on x=176.
- `race_results` — same, "RACER" header at x=95 over centred names.

**Fix:** Left-align text in list rows and table cells; right-align numerics.
Reserve `Center` for buttons, dialog bodies, and standalone titles. Consider
making `menu.h`'s `TextAlignment::Left` row default (`menu.h:167`) the pattern
the examples demonstrate.

## L5. Chip and button padding is a function of the label, not a constant

**Principle:** Consistent internal padding is what makes a set of controls read
as one family.

**Where it shows:** `toasts` — the "Info Toast" chip has ~14px of horizontal
padding; "Quick (displays 1s)" has ~1px, with the text touching both edges, and
its right edge nearly collides with the neighbouring chip. Same component, same
row.

**Fix:** Give chips/buttons a minimum horizontal padding (16px) that the label
cannot eat into; size the box as `max(min_width, text_width + 2*padding)`.

---

# Part 2 — Screen-level findings, ranked by damage

## S1. `font_weight` advertises a feature that does not work

All five rows — Light 300, Regular 400, Medium 500, SemiBold 600, Bold 700 —
render **identically**. The screen's own caption admits it: "theme.h src/ui -
regular weight, same baseline".

This is the most damaging single screenshot in the set. A newcomer scanning the
gallery for "can this library do typography?" gets a definitive no. Either wire
up the weights or delete the screen; shipping a baseline that documents a
missing capability is worse than having no screen.

## S2. `flight_options` — the main control list is invisible

Measured contrast of the option labels against their own button fill:

| Row | Ratio | WCAG AA (4.5:1) |
|---|---|---|
| Control Type | 2.37:1 | fail |
| High-G Turn Settings | **1.02:1** | fail |
| Reduced Collision Damage | **1.02:1** | fail |
| Vibration (Unavailable) | 1.11:1 | fail (disabled, exempt, but unreadable) |

1.02:1 is text the same colour as its background. The primary interactive
element of the screen cannot be read. Also on this screen: a **duplicate tab
underline** — one at y=95 spanning x=135-258 under the active tab, and a second
stray rule at y=112 spanning x=135-215.

Fix: the option-row label colour must come from `auto_text_color` against the
actual fill, not from a hardcoded light blue.

## S3. `powerwash_settings` — navigation labels at 1.71:1

The left-hand category list ("Screen Resolution", "Window Mode", "Vsync"…) is
light blue `rgb(165,190,215)` on a light blue fill `rgb(85,145,215)` — **1.71:1**
against a 4.5:1 requirement. Only the selected row is legible. This is the
screen's navigation.

Fix: unselected label `rgb(20,35,60)` on that fill gives 6.6:1.

## S4. Everything-in-the-top-left-corner screens

Measured 99th-percentile ink bounding box as a fraction of the 1280x720 frame:

| Screen | content bbox | ink coverage |
|---|---|---|
| `layout_patterns` | **4.4%** at (44,40)-(451,139) | 4.5% |
| `text_input_lab` | 12.5% at (50,48)-(400,376) | 3.1% |
| `hover_lab` | 23.2% at (52,39)-(573,450) | 7.9% |
| `font_weight` | 23.6% at (52,34)-(677,382) | 2.2% |
| `popover_lab` | 25.8% at (50,31)-(416,681) | 6.1% |
| `composer_lab` | 35.1% at (52,41)-(669,566) | 14.7% |
| `menu_showcase` | 95.1% background | — |

`layout_patterns` is the worst: a screen named for layout shows three coloured
boxes in the top-left corner and 95% empty frame. Whatever it is testing, it
demonstrates that the library cannot fill a window.

(Modal screens — the `dialog_*` family, `tabbing`, `simple_button` — also score
low here and that is correct; a modal *should* be a small object on a dimmed
field. They are excluded from this finding.)

Fix: these are lab harnesses, not showcases. Either give them a title bar, a
centred content column with a max width, and page margins matching the rest of
the set (32px), or move them out of the reviewed gallery.

## S5. Large dead zones inside otherwise-finished screens

Distinct from S4: these screens are laid out properly and then stop.

- `angry_birds_settings` — three toggles at the top of the dialog body, then
  ~200px of empty cream, then the footer. The panel is sized for content that
  is not there.
- `deadspace_settings` — the settings panel occupies y=85-322; the remaining
  400px is empty, with three clipped stray rules poking in at the bottom-left
  (y≈698).
- `mini_motorways_settings` — 88.8% single-colour background. The entire left
  half of the screen is empty while every control crowds the right edge.
- `flight_options` — a 175px vertical hole between the option block (ends y=300)
  and the help text (y=477).
- `kart_select` — each of the three panels stretches to y=610 with content
  ending at y=260, 350, 295.

Fix: size panels to their content (`Dim::Children`) rather than stretching them,
or fill the space with the content the design implies. A settings dialog with
three switches should be 280px tall, not 480px.

## S6. Nothing shares an edge — `themes`, `navigation_bar_demo`, `cards`

**Principle:** Refactoring UI, "align everything"; HIG, "Layout".

- `themes` — nine different right edges in one column (see L2). The checkbox row
  is the worst single widget in the set: a full-width bar with the label
  "Checkbox" centred at x=495 and its check mark at x=893, neither at the row's
  centre (714) nor at either edge. The active theme chip renders its chevrons
  **inverted** — `⟩ Ocean Navy ⟨` instead of `⟨ Ocean Navy ⟩`.
- `navigation_bar_demo` — section header bars span x=230-1050, the nav controls
  they head span x=230-920. 130px narrower, no reason. Worse, every element on
  the screen is the same blue at the same height, so the title, the section
  labels, and the interactive controls are visually identical — there is no way
  to tell what is clickable.
- `cards` — the "Surface" and "Background" swatches are white/near-white on a
  white card and are simply invisible. The three nested items (Item 1/2/3) touch
  with zero gap. Vertical gaps between the four card rows are 62 / 75 / 70px.

Fix for `themes`: one content column, single left edge x=283, single right edge
x=1163, every row full width. Fix for `navigation_bar_demo`: section headers get
`FontSize` one step down, no fill, `TextAlignment::Left`; only the interactive
rows keep the blue fill.

## S7. Control heights below the 44pt minimum

**Principle:** HIG "Layout" specifies a minimum 44x44pt tappable area. That is a
*touch* guideline — for a pointer-driven desktop app macOS tolerates ~28pt, so
this is a soft finding for the desktop-styled screens. It is a hard finding for
`angry_birds_settings`, `casual_settings`, `mini_motorways_settings` and
`parcel_corps_settings`, which are explicitly mobile/handheld game clones, and
for anything gamepad-driven where a large target aids readability.

Measured heights:

| Control | Height |
|---|---|
| `dialog_danger` Cancel/Delete | 44px — correct |
| `kart_select` VEHICLE row | 38px |
| `angry_birds_settings` OK/Cancel/Apply | 36px |
| `mini_motorways_settings` OK/Cancel/Apply | 36px |
| `shop_interface` Add to Cart | 36px |
| `deadspace_settings` tab strip | 32px |
| `flight_options` tab strip | 28px |
| `parcel_corps_settings` OK/Cancel/Apply | 28px |
| `setting_row_showcase` toggle | 28px |

Fix: a `min_height` of 44px on the button and tab components, which
simultaneously fixes the 28px cases and gives auto-fit text more room (L1).

## S8. `parcel_corps_settings` — overlapping and clipped rows

The worst-composed screen in the set. Row labels are centred, so their left
edges land at x=108, 110, 112, 118, 130, 132, 133, 138, 155, 155 — ten rows, ten
left edges. The `<` `>` stepper buttons overlap the label text ("Texture
Quality" runs underneath the `<`). Icon badges overhang the row's left edge
(x=75 against a row starting at x=64). The "Music Volume" row is clipped
mid-height by the scroll container. The "Resolution" row overlaps its
neighbour.

Fix: left-align labels at a single x, give the label a fixed-width flex child so
the stepper cannot overlap it, and give the scroll container a height that is a
whole multiple of the row height.

## S9. `empire_tycoon` — overlapping chrome and an unreadable wordmark

- The "DREAM INCORPORATED" wordmark measures **2.22:1** against the sky gradient
  behind it. Stroked light-on-light text.
- Top-right: the two circular icons overlap both the money card above and the
  Happiness card below, and a third white card peeks out from behind at y≈190.
  A z-order/layout collision, not a design choice.
- The left nav rail's first tile ("Rides") has its icon clipped by the tile
  bounds.
- The "^ = Trending Up" legend is meaningless because every row shows `^`.
- Copy errors visible in the baseline: "Rollercoasser", "Masoot".

## S10. Mixed alignment inside a single small dialog — `dialog_danger`

The title "Delete save?" is left-aligned at x=422; the body text is centred; the
buttons are centred as a group. Three alignments in a 540x275 box. The title also
renders at the same size as the body, so it does not read as a title, and it
floats 73px above the body while the body sits 50px above the buttons —
the title is grouped with nothing (violates proximity: HIG "Layout", Gestalt
proximity).

Padding is 52px left / 95px right / 34px top / 52px bottom. Four different
values on four sides.

Fix: uniform 32px padding, title at `TypographyScale::size(1)` (25px) and body
at base (20px), both left-aligned, 8px between title and body, 24px above the
button row. The rest of the `dialog_*` family shares this structure and would
inherit the fix.

## S11. Value labels rendered across a progress fill boundary — `cozy_cafe`

Emma's patience bar renders "50%" straddling the fill edge, giving **1.74:1**
over the filled portion. The two patience rows are also not aligned with each
other: bars start at x=743 and x=756 and are 100px and 90px wide, in the same
component, one row apart.

Fix: place the value outside the track, or give it a solid plate. Give the label
column a fixed width so both bars start at the same x.

## S12. `button_variants` — the ghost variant is invisible and disabled is more prominent than enabled

Ghost buttons render with no fill and no border, so "Normal", "Accent" and
"Secondary" in the Ghost row are indistinguishable from plain text — but the
*disabled* ghost button renders with a grey fill, making the disabled state the
most prominent thing in the row. In the Outline row, "Disabled" renders filled
rather than outlined; the variant is dropped when the disabled state is applied.

Disabled label contrast measures **1.88:1** (`button_variants`, `themes`).
WCAG 1.4.3 exempts disabled controls, so this is not a conformance failure, but
1.88:1 is below the point where a user can tell the control exists. HIG
recommends disabled content remain identifiable.

Fix: ghost gets a 1px border at 40% opacity on hover/focus at minimum; the
disabled modifier should adjust opacity, not replace the variant's fill. Target
~3:1 for disabled labels.

## S13. Smaller measured contrast failures

| Screen | Element | Ratio | Verdict |
|---|---|---|---|
| `mini_motorways_settings` | version string | 2.99:1 | fail |
| `menu_showcase` | disabled "Revert" | 3.93:1 | exempt but weak |
| `setting_row_showcase` | footer caption | 3.90:1 | fail |
| `shop_interface` | "Item"/"Type" column headers (~11px) | 4.24:1 | fail (small text) |
| `race_results` | "5th"–"8th" position labels | 4.57:1 | marginal pass |
| `shop_interface` | "Consumable" type cells | 4.60:1 | marginal pass |

Everything else measured passes comfortably. **Contrast is not this project's
main problem** — apart from S2 and S3, the palettes are sound.

## S14. Section headers indented further than the items they head

`setting_row_showcase`: the TOGGLES / OPTIONS / VOLUME headers sit at x=230
while their rows sit at x=195. The header is indented 35px *further right* than
its own content, which inverts the hierarchy. The gap between sections (50px) is
identical to the gap between rows within a section (50px), so proximity does no
grouping at all. The three headers are green, blue and red with no semantic
difference between the sections — decorative colour.

`sports_settings` has the same problem in a different form: section labels
("Display", "Anti-Aliasing & Performance", "Quality") are centred over
left-aligned rows, and the screen has three competing left edges — tabs at
x=50, the "Graphics" heading at x=110, the rows at x=70.

Fix: section header shares the left edge of its rows; 32px above a header, 8px
below it, 0 extra between rows. One colour for all section headers.

## S15. Table headers that do not sit over their columns

`shop_interface` and `race_results` both place column headers at x-positions
unrelated to the data beneath them, and set them at ~11px against ~15px body —
below the library's own declared 16px accessibility minimum.

Fix: header shares its column's alignment and x-origin exactly; header at 16px
in a muted colour rather than 11px.

---

# What is already good

Worth saying plainly, because it is most of the set:

- **Colour palettes.** `cozy_cafe`, `islands_trains_settings`, `kart_select`,
  `sports_settings` and `race_results` all have coherent, restrained palettes
  with a clear accent. Nothing looks like default-tool colour.
- **`sports_settings`** has genuinely correct row rhythm — 40px pitch,
  consistent throughout, grouped into three labelled sections.
- **`islands_trains_settings`** is the best-composed screen in the set: one
  column, consistent row height, real section grouping, generous card padding.
  Its only real flaw is the centred section labels (L4) and the close button
  orphaned in the screen corner rather than attached to the card.
- **`race_results`** and **`shop_interface`** read as shipped game UI. Their
  problems are alignment details, not composition.
- **Corner radii and elevation** are consistent across the whole set. Nobody
  mixed a 4px radius with a 16px radius.
- **`accessibility`** does what it claims: measured auto-contrast output was
  6.00:1 and 8.98:1 on the samples tested.

# Fastest path to raising the floor

In order of impact per unit of work:

1. **L1** — default `font_size` to `TypographyScale::base()` and raise the
   auto-fit floor to 16px. Fixes the size-jitter on every screen at once.
2. **L2** — collapse the two spacing scales to one integral 8pt scale.
   Fixes soft edges everywhere.
3. **S2 + S3** — two hardcoded label colours. Ten minutes, removes the only two
   genuinely unreadable screens.
4. **S1** — fix or delete `font_weight`. It is the screenshot that most directly
   says "this library can't do that".
5. **L4** — change the examples' house style from `Center` to `Left` for list
   and table text. Mechanical, high visual payoff.
6. **L3 + S7** — add `success`/`warning` to `Theme`; add a 44px `min_height` to
   buttons and tabs.
7. **S4** — give the six corner-crammed lab screens a standard frame, or drop
   them from the gallery.

Items 1, 2, 5 and 6 are library changes. They are worth doing first not only
because they are cheaper than editing 101 screens, but because the screens
currently demonstrate that afterhours' design system can be ignored — which is
the opposite of what a showcase is for.
