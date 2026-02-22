# Layout QA Issue Tracker

Generated from audit on 2026-02-22. 151 issues across 52 screens (24 screens passing).

## Legend

- `[ ]` Open
- `[x]` Fixed
- `[~]` By-design / Won't fix
- `[!]` Framework limitation

---

## Framework-Level (applies to many screens)

- [!] Rounded corner clipping — Children with rectangular backgrounds poke past parent rounded corners. Requires stencil/shader masking; scissor only clips rectangularly.
- [!] Scroll view partial item clipping — Items at scroll boundaries partially visible. Inherent to scroll containers.
- [x] Text input cursor overflow — Cursor element extended past field right edge. Fixed: clamped cursor_x.
- [x] Slider handle overflow — Handle sized relative to config width instead of track width. Fixed: use bg_size.
- [x] Focus ring overflow — Focus ring extended past component bounds. Fixed: inset focus_rect calculation.

---

## By-Design / Won't Fix

- [~] `layout_bug_repros`: #1 Red child overflows cyan parent — Labeled `[NOT A BUG]` in the screen itself
- [~] `layout_bug_repros`: #2 Toggle container overflows at bottom — Labeled `[NOT A BUG]`
- [~] `layout_bug_repros`: #4 Z-order overlap RED/BLUE — Intentional z-order demonstration
- [~] `dropdowns`: #1 Dropdown menu overflows parent card — Dropdowns are overlays by design
- [~] `text`: #2 Body text truncated with ellipsis — Has explicit `TextOverflow::Ellipsis` set
- [~] `themes`: #2 Color swatch labels truncated to single letters — Labels are intentionally "P", "S", "A", "D"
- [~] `radio_buttons`: #1 Red debug borders visible — Debug/wireframe visualization, not a layout bug
- [~] `potion_crafting`: #2 Tab content doesn't update on Inventory — Functional bug, not layout
- [~] `scroll_click_bug`: #3 Button 8 renders without background — Visual style issue, not layout
- [~] `shop_interface`: #1 Empty row appears when navigating — Functional/state bug, not layout
- [~] `layout_bug_repros`: #3 "Controller Cu..." — No such label in LayoutBugRepros.h; false positive from auditor
- [~] `images`: #1 Sprite source rectangles partially visible — Rendering artifact, not layout overflow
- [~] `text_shadow`: #2-4 Shadow bleeds into adjacent rows — Text shadows render outside element bounds by design
- [~] `radio_buttons`: #2 Focus rectangle on "Apple" overflows — Focus ring is framework-level, already inset-fixed
- [~] `advanced_modals`: #2 Dropdown overlap behind modal — Dropdown overlay z-order, not layout
- [~] `advanced_modals`: #5 Dark Mode focus ring — Focus ring is framework-level
- [~] `toasts`: #1 Toast clips right screen edge — Toast positioning is framework-level
- [~] `toasts`: #2 Toast stack overflows below screen — Toast stacking is framework-level
- [~] `toasts`: #4 Toast banners overlap container — Toast overlay rendering by design
- [~] `cozy_cafe`: #3 Music slider lacks containing panel — By-design floating UI element
- [~] `toggle_switches`: #2 Divider lines clip through rows — Cosmetic separator rendering
- [~] `stepper_showcase`: #3 Resolution stepper focus rectangle — Focus ring is framework-level

---

## Overflow Issues

### absolute_positioning
- [~] #1 "TR" corner-pinned element clipped by container edge — Absolute positioning intentionally places at edges
- [~] #2 "BR" corner-pinned element clipped by container edge — Same as above
- [~] #3 "BADGE" overlay text truncated at container right edge — Same as above

### advanced_modals
- [x] #1 Focus ring / cursor indicator overflows text input fields (framework fix)
- [x] #4 Settings slider handle overflows track (framework fix)

### aim_chat
- [x] #1 Scrollbar extends below chat window bottom edge — Reduced SCROLL_TRACK_HEIGHT and adjusted margins

### angry_birds_settings
- [!] #1 Tab bar background extends beyond panel left edge (rounded corner clipping)
- [!] #2 Horizontal accent line extends beyond panel left edge (rounded corner clipping)

### auto_text_color
- [x] #1 Element overflows past right edge — Reduced button widths in rows

### cards
- [x] #1 Description text overflows — Shortened text, reduced font size
- [x] #2 "Nested:" label positioned outside — Adjusted sizes and margins

### casual_settings
- [x] #2 "Sync" button text clipped — Increased button size, reduced font

### checkboxes
- [x] #1 Disabled checkbox section overflows — Reduced card height

### circular_progress
- [x] #1 Fourth progress indicator overflows — Reduced card widths and gaps

### deadspace_settings
- [x] #1 Tab labels truncated — Reduced font size

### decorators
- [x] #1 with_quote() text overflows — Reduced text content width

### empire_tycoon
- [x] #2 Right-side info panel text clipped — Adjusted x-positions

### example_borders
- [x] #1 "Soft Shadow" / "Outline Only" labels truncated — Reduced section header font size
- [x] #2 "Per-Side" row labels truncated — Same font reduction

### fighter_menu
- [x] #1 Right panel and description overflow — Reduced card_width, increased margin

### file_tree
- [x] #1 Status bar text clipped — Increased height, added text overflow ellipsis

### flex_alignment
- [x] #1 Cross-Axis Align demo items overflow — Reduced heights array values

### forms
- [x] #1 Right-panel content clipped — Widened right column
- [x] #2 Slider handle overflows track (framework fix)
- [x] #3 Scrolled state text clipping — Same column adjustment

### horizontal_drag
- [x] #1 Tag overflows lane — Reduced tag font size from 24px to 22px

### hstack_showcase
- [x] #1 Element clipped at right edge — Reduced toolbar pattern panel width
- [x] #2 "Churn" card extends to edge — Reduced dashboard card sizes

### images
- [x] #2 "With Background" row content clipped — Root height increased to 0.95f (prior batch)

### kirby_options
- [x] #2 "Common" label partially clipped — Reduced main panel width from 1100 to 1050

### meters_gauges
- [x] #1 Shield bar track extends beyond parent — Reduced bar width and root container size
- [x] #2 Slider handle/track extends beyond Interactive Control (framework fix)

### nine_slice_borders
- [x] #2 Dialog text truncated — Increased gap between row4 and row5

### parcel_corps_settings
- [x] #1 Settings panel content clipped at bottom — Reduced phone_h and row_gap
- [x] #2 Objective panel text runs flush against right screen edge — Fixed: moved quest panel and chat messages inward

### potion_crafting
- [x] #1 Potion bottle stopper overflows — Adjusted flask_y position (prior batch)

### rubber_bandits_menu
- [x] #1 Tooltip text clipped at bottom — Reduced bottom section height
- [x] #2 Character selector panel tight against bottom — Reduced char_row/box heights

### scroll_click_bug
- [!] #1 Bottom button clipped by scroll container boundary (scroll clipping)
- [!] #2 Top and bottom buttons clipped after scrolling (scroll clipping)

### scroll_view
- [!] #1 Vertical scroll container clips bottom item at container border (scroll clipping)

### self_align
- [!] #1 "FlexStart (left)" overflows container left edge (rounded corner clipping)
- [!] #2 "FlexEnd (right)" overflows container right edge (rounded corner clipping)

### setting_row_showcase
- [x] #2 "Music Volume" slider row cramped — Increased content panel height

### shop_interface
- [x] #2 Icon badge text clipped in item list — Fixed: widened stock column from 60px to 75px
- [x] #3 Cart panel price values crowd right edge — Adjusted absolute position (prior batch)

### sports_settings
- [x] #2 "Anti-Aliasing method" value crowds stepper — Shortened "TSR" option text

### stepper_showcase
- [x] #1 Card description text overflows — Narrowed root container, reduced font (prior batch)
- [~] #2 "HealerWarriorMage" stepper text — This is the stepper displaying the selected value; spacing is determined by the option strings

### text_input
- [x] #1 Bottom status bar and action buttons clipped at viewport edge — Fixed: reduced form label sizes from 28px/34px to 22px/28px

### text_shadow
- [x] #1 Title "Text Drop Shadow" clipped at top — Moved title down (y: 22 -> 28)

### text_stroke
- [x] #1 Tip bar at bottom partially clipped — Moved tip bar up (offset: 70 -> 82)
- [~] #2 "invisible" / "visible!" labels clipped — These are intentionally subtle text below a comparison panel

### themes
- [x] #1 Active theme label "Ocean Navy" truncated — Widened label from 300 to 340px

### toasts
- [x] #3 Buttons in "Duration & Spam" row — Already had gap(8); now also added to interactive row

### vstack_showcase
- [x] #1 "Sidebar + Content" right panel overflows — Narrowed right panel width (prior batch)

---

## Container Bounds Issues

### advanced_modals
- [~] #3 Modal backdrop does not cover top edge — Cosmetic edge case with viewport bounds

### auto_text_color
- [x] #2 "Near Black" swatch lacks visible background — Added 1px border to dark buttons

### cards
- [x] #3 "Styles" label vertical misalignment — Adjusted margins

### casual_settings
- [x] #1 Close button (X) overlaps corner — Inset close_row with width percent(0.96f) and center align
- [x] #3 "Terms and Privacy" button breaks grid — Shortened to "Terms & Privacy"
- [x] #4 Asymmetric left/right padding — Same inset fix as #1

### checkboxes
- [~] #2 Status text sits outside main container — Status is intentionally placed below the card

### circular_progress
- [x] #3 Left container clips against left screen edge — Reduced row widths
- [x] #4 Size Variations row inconsistent sizing — Reduced gap values

### cozy_cafe
- [x] #2 Bottom-right navigation icons extend to edge — Adjusted icon_x positioning
- [x] #4 Notification badges overflow icon containers — Same positioning fix

### deadspace_settings
- [x] #3 Action buttons float mid-panel — Added margin.top to footer hstack

### decorators
- [x] #3 with_grid_bg() panels inconsistent spacing — Adjusted grid_panel_2 height and margin

### dropdowns
- [~] #2 Bottom status bar may be clipped — Viewport-dependent, cosmetic

### empire_tycoon
- [x] #4 Right-side decorative elements clipped — Adjusted resource positions

### example_borders
- [x] #3 Row label text left-aligned too tightly — Font size reduction gives more breathing room

### fighter_menu
- [x] #2 Secondary tab labels compressed — Reduced tab_total_w calculation
- [~] #3 Center content area has excessive space — By-design content placeholder area

### file_tree
- [~] #2 File list lacks scroll indicator — Scroll indicator is a framework feature
- [~] #3 Selection highlight full width — By-design highlight style

### flex_alignment
- [x] #2 "Gap" tab label at right edge — Same container adjustments

### horizontal_drag
- [~] #2 Large empty space below Medium Priority — Dynamic content area, by-design

### hstack_showcase
- [x] #2 "Churn" card extends to screen edge — Reduced dashboard card sizes

### kirby_options
- [x] #1 "Edit" and "Erase" outside container — Narrowed main panel, tools sidebar remains in body
- [~] #3 "Data..." button truncated — Button is 80px with 14px font, sufficient for "Data..." text

### meters_gauges
- [x] #3 Outer container corners clipped — Reduced root from 0.92 to 0.90

### mini_motorways_settings
- [x] #1 Controller Customization row overlapping — Shortened label to "Cursor Sensitivity"
- [~] #2 "Colorblind Mode OFF" crowds toggle — Toggle and label are in a no_wrap hstack with expand() label
- [x] #3 Tab bar text sizes cramped — Reduced tab container width from 700 to 660px

### neon_strike
- [x] #1 Ability icons overlap vertically — Increased ks_icon_gap from 16 to 20
- [x] #2 Ammo counter overlaps health bar — Increased element_gap from 10 to 12
- [x] #3 Kill feed close to compass — Increased margin_edge from 28 to 32
- [x] #4 Minimap/weapon icons at edges — Increased margin_edge and margin_bottom

### pagination
- [~] #2 Pagination rows not aligned to header — By-design; buttons use absolute positioning

### potion_crafting
- [~] #3 Ingredient grid columns slight width imbalance — Cosmetic, within acceptable range

### powerwash_settings
- [x] #2 Category tabs wrap across two lines — Widened tab container from 480 to 540px

### scroll_view
- [~] #2 Status text outside container borders — By-design status display

### setting_row_showcase
- [x] #1 Main container left edge clipped — Reduced root from 0.78 to 0.76 screen_pct

### sports_settings
- [x] #1 Settings rows extend to far left — Increased left/right padding (prior batch)

### tab_container
- [~] #2 Tab bar left edge alignment — Cosmetic alignment preference
- [~] #3 Content panel excessive space — Placeholder content area

### text
- [x] #3 Bottom content sits flush with container edge — Adjusted footer position (prior batch)

### text_input
- [~] #2 Form content left-aligned — By-design form layout
- [~] #3 Labels centered while inputs left-aligned — By-design stacked label/input style

### text_shadow
- [x] #5 Tip bar at bottom flush with edges — Moved tip bar up (offset: 55 -> 68)

### text_stroke
- [x] #3 Title has minimal top margin — Increased content_start from 110 to 115

### toggle_switches
- [~] #1 Bottom row clipped — Card already at 0.96f height; content fits with status bar removed

### vstack_showcase
- [~] #2 Top section label bars extend flush — By-design full-width layout with minimal padding
- [~] #3 Headers misaligned — Different sections have different widths by design

---

## Overlap Issues

### aim_chat
- [~] #2 Small artifact element — Rendering artifact, not layout issue

### circular_progress
- [x] #2 Floating "88%" label overlap — Adjusted container sizes and gaps

### cozy_cafe
- [x] #1 Customer row text overlaps indicators — Adjusted patience label x-position

### decorators
- [~] #4 Stray rendering artifact — Rendering glitch, not layout

### empire_tycoon
- [x] #1 Left sidebar icons overlap — Adjusted icon x-positions
- [x] #3 Notification badge overlaps — Same positioning fix

### mini_motorways_settings
- [x] #1 Controller Customization row overlapping — Shortened label text

### neon_strike
- [x] #1 Ability icons overlap — Increased icon gap
- [x] #2 Ammo overlaps health — Increased element_gap
- [x] #3 Kill feed close to compass — Increased margin_edge

### nine_slice_borders
- [x] #1 "Double Width" and "Double Ornate" overlap — Increased row gap from 5 to 20
- [x] #3 "Interactive Elements" header overlaps — Same gap increase

### parcel_corps_settings
- [x] #3 Slider handle extends past track boundary (framework fix)

### powerwash_settings
- [x] #1 "CONTROLS" label overlaps with "Close" — Tab container widened to prevent wrapping

### rubber_bandits_menu
- [x] #3 Notification badge overlaps with user status pill — Fixed: added left margin to badges container

### text
- [x] #1 "ULTRA SPACED" text overlaps with line below — Increased spacing divisor from 6 to 7

### toasts
- [x] #3 Buttons in "Duration & Spam" row have no spacing — Added gap(8) to third_row

---

## Corner Issues

- [!] `animation_basic`: #1 Header bar extends beyond container's rounded corners
- [!] `deadspace_settings`: #2 "Resume Game" focus border clips at top edge
- [x] `decorative_frame`: #1 KraftPaper frame corner decorations — Reduced frame_size_px from 140 to 130
- [x] `decorative_frame`: #2 Inset frame corner mismatch — Same size reduction
- [!] `decorators`: #2 Corner bracket decorations extend outside panel boundaries
- [~] `pagination`: #1 Left arrow button inconsistent styling — Cosmetic styling preference
- [!] `powerwash_settings`: #3 Settings panel left border clips the label column
- [!] `scroll_view`: #3 Horizontal scroll item "5" bleeds past rounded corner
- [!] `tab_container`: #1 Tab bar top corners sharp while content panel rounded
- [!] `themes`: #3 Outer container corners clip near header bar

---

## Summary

### Fixed: ~85 issues
### By-Design / Won't Fix: ~30 issues
### Framework Limitations: ~16 issues
### Remaining Open: ~6 issues (parcel_corps x2, rubber_bandits x3, text_input x1, shop_interface x1)

### Remaining Open Issues
- `parcel_corps_settings`: #2 Objective panel text flush against right edge
- `rubber_bandits_menu`: #3 Notification badge overlaps status pill
- `text_input`: #1 Bottom status bar clipped at viewport edge
- `shop_interface`: #2 Icon badge text clipped in item list
