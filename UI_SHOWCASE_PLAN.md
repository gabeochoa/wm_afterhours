# Afterhours UI Showcase

> Build a showcase app with game UI mockups (cozy, shooter, tycoon) and component galleries, plus add theming infrastructure to the library.

---

## Phase 1: Navigation & Infrastructure

Get the demo framework running smoothly before adding content.

### Tasks

- [x] **1.1** Change CLI from `--<name>` to `--screen=<name>` format in `src/main.cpp`
- [x] **1.2** Add `,` and `.` as screen navigation alternatives (keep PageUp/PageDown)
- [x] **1.3** Add screen name HUD in bottom-left showing `--screen=<name>`
- [x] **1.4** Create `src/systems/screens/` directory and move existing Example*.h files there

### Files Modified
- `src/main.cpp` - CLI parsing
- `src/game.cpp` - Navigation keys, HUD rendering

### Files Created
- `src/systems/RenderScreenHUD.h` - HUD system for screen name display

---

## Phase 2: Library Theming System

Add theming infrastructure to afterhours library.

### Tasks

- [x] **2.1** Extend existing `Theme` struct to add `surface` and `font_muted` colors (minimal breaking change)
- [x] **2.2** Create `theme_presets.h` with 5 preset themes and `apply()` function
- [x] **2.3** Add `HasShadow` component supporting both hard offset and soft blur styles
- [x] **2.4** Update `rendering.h` to draw shadows before elements
- [x] **2.5** Add `.with_shadow()` to ComponentConfig builder
- [x] **2.6** Include `theme_presets.h` in main `ui.h`

### Additional Files Modified
- `vendor/afterhours/src/plugins/color.h` - Added `lighten()` function

### Files Created
- `vendor/afterhours/src/plugins/ui/theme_presets.h`

### Files Modified
- `vendor/afterhours/src/plugins/ui/theme.h` (add surface, font_muted - backwards compatible)
- `vendor/afterhours/src/plugins/ui/components.h`
- `vendor/afterhours/src/plugins/ui/rendering.h`
- `vendor/afterhours/src/plugins/ui/component_config.h`
- `vendor/afterhours/src/plugins/ui.h`

---

## Phase 3: Game UI Mockups

Create three visually distinct game menu mockups.

### Tasks

- [x] **3.1** Create `CozyCafe.h` - Cozy cafe sim menu
  - Kraft/cream theme
  - Daily specials panel, customer list, inventory grid
  - Soft shadows, rounded corners, warm feel
  - Components: buttons, dropdowns, sliders (volume)

- [x] **3.2** Create `NeonStrike.h` - Tactical shooter loadout screen
  - Neon dark theme (cyan/magenta on black)
  - Weapon select, loadout slots, mission briefing
  - Sharp edges, high contrast, glowing accents
  - Components: buttons, stat bars, dropdown

- [x] **3.3** Create `EmpireTycoon.h` - Business tycoon dashboard
  - Ocean navy theme (navy/gold)
  - Financial overview, build menu, employee panel
  - Clean grids, professional look, data-heavy
  - Components: buttons, sliders, checkboxes, tabs

### Files Created
- `src/systems/screens/CozyCafe.h`
- `src/systems/screens/NeonStrike.h`
- `src/systems/screens/EmpireTycoon.h`

---

## Phase 4: Component Galleries

Individual screens demonstrating each component type.

### Tasks

- [x] **4.1** Create `Buttons.h` - Button styles gallery
  - Normal, hover, pressed, disabled states
  - Different sizes (sm, md, lg)
  - Different themes applied (Ocean Navy)
  - Button groups, custom colors, sharp corners

- [x] **4.2** Create `Forms.h` - Form components gallery
  - Sliders with labels (Neon Dark theme)
  - Checkboxes (enabled/disabled)
  - Dropdowns (resolution, quality, language)
  - Two-column layout

- [x] **4.3** Create `Cards.h` - Card/panel layouts
  - Cards with theme colors (Cozy Kraft)
  - Nested cards
  - Different corner radius options
  - Semi-transparent panels

- [x] **4.4** Enhance existing screens (`colors`, `layout`)
  - Applied Midnight theme to colors screen
  - Applied Sage Natural theme to layout screen
  - Improved visual presentation

### Files Created
- `src/systems/screens/Buttons.h`
- `src/systems/screens/Forms.h`
- `src/systems/screens/Cards.h`

### Files Modified
- `src/systems/screens/ExampleColors.h`
- `src/systems/screens/ExampleLayout.h`

---

## Phase 5: Theme Switcher & Polish

Final polish and interactive theme demo.

### Tasks

- [x] **5.1** Create `Themes.h` - Real-time theme switching screen
  - Buttons to switch between all 5 presets
  - Preview panel showing sample components
  - Immediate visual feedback

- [x] **5.2** Update `--list-screens` output to show organized categories
- [x] **5.3** Add screen descriptions to HUD (optional subtitle)
- [x] **5.4** Document discovered library gaps in `LIBRARY_GAPS.md`

### Files Created
- `src/systems/screens/Themes.h`
- `LIBRARY_GAPS.md` (to track issues found)

---

## Screen Order (Final)

When navigating with Tab/PageDown:

1. `cozy_cafe` - Cozy game mockup (default/first screen)
2. `neon_strike` - Shooter game mockup
3. `empire_tycoon` - Tycoon game mockup
4. `buttons` - Button gallery
5. `forms` - Form components gallery
6. `cards` - Card layouts gallery
7. `colors` - Color system demo
8. `layout` - Layout system demo
9. `typography` - Text rendering demo
10. `tabbing` - Keyboard navigation demo
11. `themes` - Theme switcher

---

## Theme Color Values

### Cozy Kraft
```cpp
background:  {245, 240, 230, 255}  // Cream
surface:     {255, 250, 240, 255}  // Warm white
font:        {92, 64, 51, 255}     // Sepia brown
font_muted:  {139, 119, 101, 255}  // Light brown
primary:     {196, 167, 125, 255}  // Kraft
accent:      {212, 165, 165, 255}  // Dusty rose
```

### Neon Dark
```cpp
background:  {15, 15, 20, 255}     // Near black
surface:     {25, 25, 35, 255}     // Dark purple-gray
font:        {255, 255, 255, 255}  // White
primary:     {0, 255, 255, 255}    // Cyan
accent:      {255, 0, 128, 255}    // Hot pink
```

### Ocean Navy
```cpp
background:  {15, 30, 55, 255}     // Deep navy
surface:     {25, 45, 80, 255}     // Navy
font:        {255, 255, 255, 255}  // White
primary:     {59, 130, 246, 255}   // Blue
accent:      {251, 191, 36, 255}   // Gold
```

### Midnight
```cpp
background:  {0, 0, 0, 255}        // Pure black
surface:     {20, 20, 20, 255}     // Near black
font:        {255, 255, 255, 255}  // White
primary:     {185, 28, 28, 255}    // Deep red
accent:      {255, 214, 10, 255}   // Yellow
```

### Sage Natural
```cpp
background:  {212, 229, 210, 255}  // Sage green
surface:     {250, 250, 249, 255}  // Warm white
font:        {55, 65, 81, 255}     // Charcoal
primary:     {77, 124, 77, 255}    // Forest green
accent:      {198, 123, 92, 255}   // Terracotta
```

---

## Success Criteria

Someone looking at the three game mockups should:

1. Immediately see they're different games/genres
2. Think "this looks professional, not like programmer art"
3. Want to know how it was built

---

## Estimated Effort

| Phase | Effort | Dependencies |
|-------|--------|--------------|
| Phase 1 | Small | None |
| Phase 2 | Medium | None |
| Phase 3 | Large | Phase 1, Phase 2 |
| Phase 4 | Medium | Phase 2 |
| Phase 5 | Small | Phase 3, Phase 4 |
