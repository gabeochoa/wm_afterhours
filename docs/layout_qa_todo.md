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

---

## Overflow Issues

### absolute_positioning
- [ ] #1 "TR" corner-pinned element clipped by container edge
- [ ] #2 "BR" corner-pinned element clipped by container edge
- [ ] #3 "BADGE" overlay text truncated at container right edge

### advanced_modals
- [x] #1 Focus ring / cursor indicator overflows text input fields (framework fix)
- [x] #4 Settings slider handle overflows track (framework fix)
- [ ] #5 Dark Mode focus ring extends beyond modal content area

### aim_chat
- [ ] #1 Scrollbar extends below chat window bottom edge

### angry_birds_settings
- [!] #1 Tab bar background extends beyond panel left edge (rounded corner clipping)
- [!] #2 Horizontal accent line extends beyond panel left edge (rounded corner clipping)

### auto_text_color
- [ ] #1 Element overflows past right edge of viewport

### cards
- [ ] #1 Description text overflows past right edge of viewport
- [ ] #2 "Nested:" label positioned outside its row container

### casual_settings
- [ ] #2 "Sync" button text clipped by circular boundary

### checkboxes
- [ ] #1 Disabled checkbox section overflows container bottom edge

### circular_progress
- [ ] #1 Fourth progress indicator overflows outside container grid

### deadspace_settings
- [ ] #1 Tab labels truncated in settings tab bar

### decorators
- [ ] #1 with_quote() first quote text overflows container right edge

### empire_tycoon
- [ ] #2 Right-side info panel text clipped at screen edge

### example_borders
- [ ] #1 "Soft Shadow" / "Outline Only" labels truncated at right viewport edge
- [ ] #2 "Per-Side" row labels truncated

### fighter_menu
- [ ] #1 Right panel and description text overflow off-screen

### file_tree
- [ ] #1 Status bar text clipped at container bottom edge

### flex_alignment
- [ ] #1 Cross-Axis Align demo items overflow their container boxes

### forms
- [ ] #1 Right-panel content clipped at right edge
- [x] #2 Slider handle overflows track (framework fix)
- [ ] #3 Scrolled state shows text clipping at top of right panel

### horizontal_drag
- [ ] #1 Tag overflows High Priority lane container on right

### hstack_showcase
- [ ] #1 Element clipped at right edge of screen

### images
- [ ] #1 Sprite source rectangles partially visible next to icons
- [ ] #2 "With Background" row content clipped at bottom

### kirby_options
- [ ] #2 "Common" label partially clipped or misaligned

### meters_gauges
- [ ] #1 Shield bar track extends beyond parent container
- [x] #2 Slider handle/track extends beyond Interactive Control container (framework fix)

### nine_slice_borders
- [ ] #2 Dialog text truncated in Interactive Elements section

### parcel_corps_settings
- [ ] #1 Settings panel content clipped at bottom edge of viewport
- [ ] #2 Objective panel text runs flush against right screen edge

### potion_crafting
- [ ] #1 Potion bottle stopper overflows brewing panel top boundary

### radio_buttons
- [ ] #2 Focus rectangle on "Apple" overflows radio group container

### rubber_bandits_menu
- [ ] #1 Tooltip text clipped at bottom viewport edge
- [ ] #2 Character selector panel tight against bottom edge

### scroll_click_bug
- [!] #1 Bottom button clipped by scroll container boundary (scroll clipping)
- [!] #2 Top and bottom buttons clipped after scrolling (scroll clipping)

### scroll_view
- [!] #1 Vertical scroll container clips bottom item at container border (scroll clipping)

### self_align
- [!] #1 "FlexStart (left)" overflows container left edge (rounded corner clipping)
- [!] #2 "FlexEnd (right)" overflows container right edge (rounded corner clipping)

### setting_row_showcase
- [ ] #2 "Music Volume" slider row cramped at container bottom

### shop_interface
- [ ] #2 Icon badge text clipped in item list
- [ ] #3 Cart panel price values crowd right container edge

### sports_settings
- [ ] #2 "Anti-Aliasing method" value text crowds stepper controls

### stepper_showcase
- [ ] #1 Card description text overflows right edge of viewport
- [ ] #2 "HealerWarriorMage" stepper text lacks spacing between options

### text_input
- [ ] #1 Bottom status bar and action buttons clipped at viewport edge

### text_shadow
- [ ] #1 Title "Text Drop Shadow" clipped at top of viewport

### text_stroke
- [ ] #1 Tip bar at bottom partially clipped at viewport edge
- [ ] #2 "invisible" / "visible!" labels clipped below comparison box

### themes
- [ ] #1 Active theme label "Ocean Navy" truncated

### toasts
- [ ] #1 Toast notifications clip against right screen edge

### vstack_showcase
- [ ] #1 "Sidebar + Content" right panel overflows container and viewport

---

## Container Bounds Issues

### advanced_modals
- [ ] #3 Modal backdrop does not cover top edge of viewport

### auto_text_color
- [ ] #2 "Near Black" swatch lacks visible container background

### cards
- [ ] #3 "Styles" label vertical misalignment with card elements

### casual_settings
- [ ] #3 "Terms and Privacy" button breaks two-column grid symmetry
- [ ] #4 Asymmetric left/right padding in main container

### checkboxes
- [ ] #2 Status text sits outside main container

### circular_progress
- [ ] #3 Left container clips against left screen edge
- [ ] #4 Size Variations row has inconsistent container sizing

### cozy_cafe
- [ ] #2 Bottom-right navigation icons extend to screen edge
- [ ] #3 Music slider lacks containing panel

### deadspace_settings
- [ ] #3 Action buttons float mid-panel in large empty content area

### decorators
- [ ] #3 with_grid_bg() panels have inconsistent vertical spacing

### dropdowns
- [ ] #2 Bottom status bar text may be clipped at narrower viewports

### empire_tycoon
- [ ] #4 Right-side decorative elements clipped at viewport edge

### example_borders
- [ ] #3 Row label text left-aligned too tightly against card grid

### fighter_menu
- [ ] #2 Secondary tab labels visually compressed on the right
- [ ] #3 Center content area has excessive unused vertical space

### file_tree
- [ ] #2 File list lacks visible scroll indicator
- [ ] #3 Selection highlight bar extends full width without inset padding

### flex_alignment
- [ ] #2 "Gap" tab label at right edge of tab bar

### horizontal_drag
- [ ] #2 Large empty space below Medium Priority lane

### hstack_showcase
- [ ] #2 "Churn" card extends to screen edge with no right margin

### kirby_options
- [ ] #1 "Edit" and "Erase" buttons positioned outside main content container
- [ ] #3 "Data..." button truncated at right edge of text input

### meters_gauges
- [ ] #3 Outer container corners clipped by viewport on left/right edges

### mini_motorways_settings
- [ ] #3 Tab bar text sizes are inconsistent and cramped

### neon_strike
- [ ] #4 Minimap and weapon icons extend to screen edges with no margin

### pagination
- [ ] #2 Pagination button rows not aligned to header bar width

### potion_crafting
- [ ] #3 Ingredient grid columns have slight width imbalance

### powerwash_settings
- [ ] #2 Category tabs wrap across two lines

### scroll_view
- [ ] #2 Status text and scrollbar sit outside container borders

### setting_row_showcase
- [ ] #1 Main container left edge clipped at screen boundary

### sports_settings
- [ ] #1 Settings rows extend to far left screen edge with no left margin

### tab_container
- [ ] #2 Tab bar left edge does not align with content panel left edge
- [ ] #3 Content panel has excessive unused vertical space

### text
- [ ] #3 Bottom content sits flush with container edge

### text_input
- [ ] #2 Form content left-aligned leaving right half of screen empty
- [ ] #3 Labels centered while inputs are left-aligned

### text_shadow
- [ ] #5 Tip bar at bottom flush with viewport edges

### text_stroke
- [ ] #3 Title has minimal top margin

### toasts
- [ ] #2 Toast stack overflows below visible screen area

### toggle_switches
- [ ] #1 Bottom row clipped by viewport

### vstack_showcase
- [ ] #2 Top section label bars extend flush to screen edges

---

## Overlap Issues

### advanced_modals
- [ ] #2 Dropdown menu items overlap with controls behind modal

### aim_chat
- [ ] #2 Small artifact element at left edge of "Send to" bar

### casual_settings
- [ ] #1 Close button (X) overlaps container top-right corner

### circular_progress
- [ ] #2 Floating "88%" label and truncated text overlap between sections

### cozy_cafe
- [ ] #1 Customer row text overlaps with patience/progress indicators
- [ ] #4 Notification badges overflow icon containers

### decorators
- [ ] #4 Stray rendering artifact inside with_grid_bg() top panel

### empire_tycoon
- [ ] #1 Left sidebar icons overlap with adjacent navigation items
- [ ] #3 Notification badge overlaps sidebar item boundary

### layout_bug_repros
- [ ] #3 "Controller Cu..." label truncated, overlapping stepper controls

### mini_motorways_settings
- [ ] #1 Controller Customization row elements overlapping
- [ ] #2 "Colorblind Mode OFF" text crowds into toggle switch

### neon_strike
- [ ] #1 Ability icons overlap each other vertically in left sidebar
- [ ] #2 Ammo counter text overlaps with health bar area
- [ ] #3 Kill feed container sits too close to compass widget

### nine_slice_borders
- [ ] #1 "Double Width" and "Double Ornate" border panels overlap
- [ ] #3 "Interactive Elements" section header overlaps with Double-Width row

### parcel_corps_settings
- [x] #3 Slider handle extends past track boundary (framework fix)

### powerwash_settings
- [ ] #1 "CONTROLS" label overlaps with "Close" button text

### rubber_bandits_menu
- [ ] #3 Notification badge overlaps with user status pill

### stepper_showcase
- [ ] #3 Resolution stepper left arrow has inconsistent focus rectangle

### text
- [ ] #1 "ULTRA SPACED" text overlaps with line below it

### text_shadow
- [ ] #2 "COLORED" text shadow bleeds into description text below
- [ ] #3 "COMBO" text shadow overlaps with description below
- [ ] #4 Shadow Offset Examples — large shadows overlap adjacent rows

### toasts
- [ ] #3 Buttons in "Duration & Spam" row have no spacing
- [ ] #4 Toast banners overlap Interactive Toasts container boundary

### toggle_switches
- [ ] #2 Divider lines clip through Preferences row containers

### vstack_showcase
- [ ] #3 "AlignItems" and "Sidebar + Content" header bars misaligned

---

## Corner Issues

- [!] `animation_basic`: #1 Header bar extends beyond container's rounded corners
- [!] `deadspace_settings`: #2 "Resume Game" focus border clips at top edge
- [ ] `decorative_frame`: #1 KraftPaper frame corner decorations extend outside frame bounds
- [ ] `decorative_frame`: #2 Inset frame corner mismatch between dashed border and shadow
- [!] `decorators`: #2 Corner bracket decorations extend outside panel boundaries
- [ ] `pagination`: #1 Left arrow button has inconsistent container styling
- [!] `powerwash_settings`: #3 Settings panel left border clips the label column
- [!] `scroll_view`: #3 Horizontal scroll item "5" bleeds past container's right rounded corner
- [!] `tab_container`: #1 Tab bar top corners sharp while content panel has rounded corners
- [!] `themes`: #3 Outer container corners clip near header bar

---

## Summary by Priority

### High Priority (user-visible, clearly broken)
- Overflow: text/elements extending off-screen (stepper, fighter_menu, cards, etc.)
- Overlap: elements overlapping each other (neon_strike, nine_slice_borders, toasts)
- Missing gaps/spacing between interactive elements (toasts buttons, toggle dividers)

### Medium Priority (cosmetic, noticeable)
- Elements flush against viewport edges (sports_settings, text_stroke, toggle_switches)
- Inconsistent padding/margins within containers
- Container sizing too tight or too loose

### Low Priority / Won't Fix
- Framework rounded-corner clipping (7 screens)
- Scroll container partial item visibility (3 screens)
- Intentional demos / by-design behaviors (4 screens)
