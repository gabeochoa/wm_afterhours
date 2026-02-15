# Google Material Design 3 Audit: All Screens (2026-02-15)

Audited 72 UI screenshots from the game UI toolkit showcase against Google Material Design 3 (M3) principles. Below are the 15 most impactful issues, ranked by severity and breadth of impact.

---

## 1. Monospace / Handwritten Font Used Globally Instead of M3 Type Scale

**Severity:** Critical
**Affected screens:** All 72 screens. Particularly visible on text-heavy screens: text, text_input, text_overflow, race_results, shop_interface, forms, setting_row_showcase, flight_options, sports_settings, deadspace_settings, powerwash_settings, stepper_showcase, tab_container, language_demo, pagination, navigation_bar_demo, file_tree, modals, toasts, toggle_switches, checkboxes, radio_buttons.
**Problem:** The entire toolkit uses what appears to be a monospace or handwritten-style font (resembling "Gaegu-Bold" as confirmed in the language_demo screen footer). Every label, heading, body text, and caption uses this single font family. M3 specifies a complete type scale with distinct roles -- Display, Headline, Title, Body, Label -- each using appropriate typefaces (typically a sans-serif like Roboto). The handwritten font severely undermines readability at small sizes (captions, metadata text on shop_interface, file_tree entries, cozy_cafe chat messages) and makes the UI feel informal rather than structured.
**Guideline:** Typography -- M3 Type Scale consistency, distinct type roles, appropriate typeface selection
**Fix:** Implement the M3 type scale with a sans-serif primary typeface (e.g., Roboto). Reserve decorative/handwritten fonts for thematic game-specific screens only (e.g., potion_crafting, cozy_cafe). Define and apply distinct typographic roles: Display (largest headings), Headline, Title, Body (readable content), and Label (buttons, captions) with proper weight, size, line-height, and letter-spacing tokens.

---

## 2. Focus Indicators Use Inconsistent, Low-Contrast Styles

**Severity:** Critical
**Affected screens:** button_variants (dashed white border on "Normal"), buttons (thin white dashed), animation_basic (dark border on dark background), animation_interactive (dark border), animation_declarative (thin white dashed), checkboxes (thin white outline on "Primary"), forms (thin white outline on "Volume: 75%"), flex_alignment (thin dark outline on "All" tab), hstack_showcase (tiny white outline on "File"), simple_button (dark rounded outline), tabbing (thick yellow border), text_input (white border), pagination (dark border on left arrow), auto_text_color (dashed border on "Near Black"), stepper_showcase (white border), setting_row_showcase (white border on toggle), navigation_bar_demo (white dashed border), race_results (white border on "Replay"), kart_select (white dashed border), file_tree (white dashed border on "Home"), shop_interface (yellow/gold dashed border), radio_buttons (thin outline), toasts (thin dashed border), toggle_switches (thin white outline), scroll_view (thin gray outline), modals (thin white outline), potion_crafting (gold border), fighter_menu (green underline or white outline), powerwash_settings (blue dashed outline), deadspace_settings (teal outline), sports_settings (white dashed border).
**Problem:** Focus indicators vary wildly across screens -- some use dashed borders, some solid, some use color (yellow, white, teal, gold, blue), and some are barely visible against their backgrounds. On dark-themed screens like animation_basic and forms, a dark or thin white dashed outline on a dark element provides insufficient contrast. The tabbing screen uses a thick yellow border which is one of the better examples, but this style is not used consistently elsewhere. M3 requires visible, high-contrast focus rings with a minimum 3:1 contrast ratio against adjacent colors.
**Guideline:** Focus Indicators -- Visible, high-contrast focus rings; Accessibility -- 3:1 minimum contrast for UI components
**Fix:** Standardize on a single focus indicator style across all themes: a 3px solid offset outline using the theme's primary or inverse color, ensuring at least 3:1 contrast against the component background and surrounding surface. Consider an inner+outer ring approach (e.g., 2px white + 2px dark) for universal visibility across light and dark themes.

---

## 3. Text and Content Clipping / Overflow Across Multiple Screens

**Severity:** Critical
**Affected screens:** cards (right-side text "This panel uses opacity to create a translo..." clipped), fighter_menu (sub-tab labels "System Opt" "Game Optic" "Display Opt" "Sound & La" "Network Op" "Button Settings" all truncated without ellipsis), modals (right-side buttons "Simple M..." "Info Dial..." "Op..." "Backgroun..." "Confirm:..." clipped at viewport edge), stepper_showcase (Card Selector text "Heavy armor, melee attacks, high de..." clipped), empire_tycoon ("^ = Trending Up (arrow)" partially clipped at right edge), text_overflow (intentional demo showing overflow issues), cozy_cafe ("Patience: Lo..." text overlapping), parcel_corps_settings (quest text "[X] SIGN UP A BUSINESS TO YOUR DELIVERY APP" clipped at right edge), hstack_showcase (right-edge elements clipped), kirby_options ("Accessibility" label overlaps with nearby text).
**Problem:** Multiple screens exhibit text or UI elements being cut off at container boundaries without proper truncation handling (ellipsis, wrapping, or scrolling). The fighter_menu sub-tab labels are particularly egregious -- all 6 tab labels are truncated because they don't fit their containers. The modals screen clips its trigger buttons at the right viewport edge. This creates both usability and accessibility failures since users cannot read the full labels.
**Guideline:** Layout -- Proper content containment; Typography -- Text overflow handling; Accessibility -- All content must be perceivable
**Fix:** Implement consistent text overflow strategies: use text-overflow ellipsis with tooltips for constrained single-line labels, enable text wrapping for multi-line contexts, and ensure no interactive elements are placed beyond the viewport boundary. For tab labels like fighter_menu, use scrollable tab bars or abbreviation with tooltip on hover/focus.

---

## 4. Insufficient Color Contrast on Multiple Screens

**Severity:** Critical
**Affected screens:** button_variants (gray "Disabled" text on gray background, "Ghost" row labels nearly invisible), buttons ("Disabled" button is gray-on-gray), flight_options ("High-G Turn Settings" and "Reduced Collision Damage" -- light blue text on medium blue background), checkboxes ("Disabled ON" and "Disabled OFF" with very low contrast gray text), forms (teal text on dark teal background for toggle labels like "Music X", "SFX X"), toggle_switches (disabled "Locked Setting (ON)" and "Unavailable Option (OFF)" barely visible), text (body text in Typography Showcase is low contrast brown/cream), setting_row_showcase (gray slider track against dark background), themes ("D" label on disabled swatch barely visible), cozy_cafe ("Patience: 0%" green text on dark background is small and low contrast), kirby_options ("Accessibility" text overlapping with garbled characters).
**Problem:** Disabled states across many components use low-opacity overlays or gray-on-gray combinations that fall well below the WCAG 4.5:1 ratio for normal text and 3:1 for large text. The flight_options screen has active (non-disabled) items like "High-G Turn Settings" with light blue text on a medium blue background that also fails contrast requirements. While disabled elements are exempt from WCAG in some interpretations, M3 still recommends maintaining 38% opacity minimum (which maps to roughly 3:1 for on-surface text) so users can identify what the control is.
**Guideline:** Accessibility -- Color contrast 4.5:1 for text, 3:1 for UI components; Color System -- Proper on-surface and on-primary color roles
**Fix:** For disabled states, use M3's recommended 38% opacity on content colors (ensuring the underlying text remains at least 3:1 contrast). For active states like flight_options, use proper on-surface-variant or on-primary colors that meet 4.5:1 against their container. Audit all color pairings with a contrast checker tool and adjust theme color tokens accordingly.

---

## 5. Inconsistent Corner Radii -- No Unified Shape Scale

**Severity:** Major
**Affected screens:** button_variants (mix of pill-rounded filled buttons and sharp-cornered outline labels), buttons (pill shapes for "Coral" and "Teal" alongside rounded-rect others), simple_button (large pill "Click Me!" alongside smaller rounded-rect buttons), cards (sharp-cornered "Primary" and "Sharp" cards alongside rounded "Surface" and "Custom"), example_borders (demonstrates "Pill", "Rounded", "Top Only", "Diagonal" all on one screen), nine_slice_borders (all sharp 90-degree corners), decorative_frame (large rounded corners), animation screens (all use rounded square shapes), bevel_borders (all sharp corners), circular_progress (rounded cards), forms (large pill-shaped sliders alongside sharp-cornered toggles), casual_settings / angry_birds_settings (standard rounded), race_results (sharp corners on table, rounded on buttons), kart_select (rounded circles for characters, sharp rectangles for stat bars), stepper_showcase (sharp rectangles).
**Problem:** The toolkit has no consistent shape scale. M3 defines a shape scale with specific corner radius tokens (Extra Small = 4dp, Small = 8dp, Medium = 12dp, Large = 16dp, Extra Large = 28dp, Full = circular). Instead, the toolkit mixes fully rounded pill shapes, squared-off rectangles, moderately rounded rectangles, and large rounded rectangles seemingly at random, even within the same screen.
**Guideline:** Shape -- Consistent corner radii from the M3 shape scale token system
**Fix:** Define a shape scale with 5-6 radius tokens and apply them systematically: small interactive elements (chips, small buttons) get Small radius, medium containers (cards, text fields) get Medium, large containers (sheets, dialogs) get Large or Extra Large. Document which shape token applies to which component category and enforce it in the component API.

---

## 6. Lack of Surface Tonal Elevation -- Flat Container Hierarchy

**Severity:** Major
**Affected screens:** modals (dark panels on dark background with no tonal differentiation), drag_drop (columns same tonal level as background), navigation_bar_demo (all bars same shade of blue), forms (all controls on same dark teal background), toasts (trigger buttons and sections all at same elevation), toggle_switches (all rows on same gray surface), text_input (input fields same purple as container surface), themes (component preview area not differentiated from sidebar), setting_row_showcase (all sections same dark surface), language_demo (menu and content panel at same elevation), checkboxes (complex hierarchy but no tonal separation between groups), hstack_showcase (all example boxes same dark card color), vstack_showcase (all containers same dark shade).
**Problem:** M3 uses tonal surface elevation (Surface, Surface+1 through Surface+5) to create visual hierarchy without relying on drop shadows. In these screens, nested containers, overlays, and different hierarchy levels all use the same or nearly the same surface color, making it difficult to distinguish parent-child relationships or foreground from background. The modals screen is particularly flat -- sections like "BASIC MODALS", "HELPER FUNCTIONS", "MODAL STACKING" all appear as the same dark slabs.
**Guideline:** Elevation -- Surface hierarchy via tonal elevation; Color System -- Surface tonal elevation tokens
**Fix:** Implement M3 tonal surface colors: Surface (base), Surface Container Low, Surface Container, Surface Container High, Surface Container Highest. Apply progressively higher tonal values to elements that sit above others (e.g., cards on a page, dialogs above cards). Each theme should define this 5-level tonal scale.

---

## 7. Button Styling Does Not Follow M3 Component Patterns

**Severity:** Major
**Affected screens:** button_variants (custom "Filled/Outline/Ghost" taxonomy instead of M3), buttons (custom "States: Normal/Accent/Secondary/Disabled" instead of M3 roles), simple_button (oversized coral pill button), casual_settings / angry_birds_settings (custom rounded buttons with mixed colors), kart_select (green "READY!" button, dark "< Back"), race_results (dark "Replay", dark red "Quit Cup", green "Next Race >>"), language_demo (dark gold "Hello!" with blue menu buttons), tabbing (multicolor OPTION A/B/C/D with magenta, cyan, lime, orange), toasts (mixed color trigger buttons), empire_tycoon (yellow action buttons), potion_crafting (gold "Brew!" button), shop_interface (green "Add to Cart", yellow "Purchase"), flight_options (blue "OK" with outlined "Cancel" and "Apply").
**Problem:** M3 defines five button types with specific visual and semantic roles: Filled (primary action), Filled Tonal (secondary), Outlined (tertiary/alternative), Elevated (lifted), and Text (low-emphasis). The toolkit instead uses a custom taxonomy ("Filled/Outline/Ghost" with "Normal/Accent/Secondary" color variants) that does not map to M3's component hierarchy. Button prominence is conveyed through arbitrary color choices rather than M3's structured hierarchy. Many game screens use entirely custom button styles that bear no resemblance to M3 patterns.
**Guideline:** Components -- Standard M3 button component patterns (Filled, Filled Tonal, Outlined, Elevated, Text)
**Fix:** Rename and restructure button variants to match M3: Filled (highest emphasis, uses Primary color), Filled Tonal (uses Secondary Container), Outlined (border only, uses Outline color), Elevated (with shadow), Text (no container). Map the existing "Accent" to Filled, "Normal" to Filled Tonal, "Ghost" to Text, and add Outlined/Elevated. Ensure each button type follows M3's color mapping precisely.

---

## 8. Checkboxes and Radio Buttons Use Non-Standard Visual Patterns

**Severity:** Major
**Affected screens:** checkboxes (full-width colored rows with "V" text instead of checkmark icons; colors like crimson, purple, teal for different "styles" -- Primary, Secondary, Accent; standalone box-only variants show colored squares instead of proper checkbox icons), radio_buttons (vertical capsule-shaped indicators instead of circular radio dots; selected state shows a filled circle but the shape is elongated/oval rather than round), forms (toggle switches labeled with "X" text character instead of standard check/cross icons), toggle_switches (checkboxes in the Preferences section use "X" text to indicate checked state).
**Problem:** M3 checkboxes should display a square container with a checkmark icon when selected, using the Primary color. The toolkit instead renders full-width colored bars with the letter "V" as the check indicator. Radio buttons should be circular with a filled inner dot on selection; here they appear as tall oval/capsule shapes. The "X" used for checkbox-like toggles in forms and toggle_switches is semantically confusing (usually means "close" or "delete", not "checked"). These non-standard patterns break user expectations and violate M3's component specifications.
**Guideline:** Components -- Standard M3 checkbox and radio button patterns; Iconography -- Consistent icon style
**Fix:** Replace "V" text with a proper checkmark icon (Material Symbols "check") rendered inside a standard square checkbox container. Replace "X" indicators with proper checkmark or switch thumb positions. Make radio buttons strictly circular (not oval). Follow M3 specs: unchecked = 2dp border outline, checked = filled container with white checkmark icon.

---

## 9. ALL-CAPS and Mixed Case Labels Violate M3 Sentence Case Convention

**Severity:** Major
**Affected screens:** modals ("BASIC MODALS", "HELPER FUNCTIONS", "MODAL STACKING", "INPUT BLOCKING TEST", "RESULTS"), toasts ("SIMPLE TOASTS", "DURATION & SPAM", "INTERACTIVE TOASTS"), bevel_borders ("BEVEL BORDERS"), nine_slice_borders ("9-SLICE BORDERS"), example_borders ("BORDER STYLES"), tabbing ("KEYBOARD NAV", "OPTION A/B/C/D"), rubber_bandits_menu ("RUBBER BANDITS", "OPTIONS", "CONTROLS", "LEADERBOARD", "ARMORY", "PLAY CREDITS", "BACK"), neon_strike ("KILLSTREAK", "SCORE: 15,000", "SECURE OBJECTIVE B"), deadspace_settings ("// SETTINGS", "SELECT"), flight_options ("OPTIONS", "FLIGHT SYSTEM", "CONTROLS", "KEYBOARD", "MOUSE", "FLIGHT STICK", "DISPLAY", "GRAPHICS", "SOUND", "LANGUAGE"), sports_settings ("GAMEPLAY", "VIDEO", "AUDIO", "CONTROLS"), kart_select ("SELECT YOUR RACER", "VEHICLE"), race_results ("RACE RESULTS", "CUP STANDINGS", "BEST LAP"), powerwash_settings ("X SETTINGS", "GENERAL", "GAMEPLAY", "VIDEO", "AUDIO", "CONTROLS"), kirby_options ("MainMenu").
**Problem:** M3 mandates sentence case for virtually all text: buttons, labels, tabs, headings, and navigation items. The toolkit uses ALL CAPS extensively for section headers, tab labels, and menu items. This reduces readability (word shapes are lost in all-caps) and creates an aggressive visual tone. While game UIs may intentionally use all-caps for stylistic reasons, a toolkit claiming M3 alignment should default to sentence case.
**Guideline:** Writing/Voice -- Sentence case for labels and UI text
**Fix:** Convert all label text to sentence case by default (e.g., "Basic modals" not "BASIC MODALS", "Flight system" not "FLIGHT SYSTEM"). Provide an optional text-transform property for game-specific screens that want all-caps, but make sentence case the default in the toolkit's text rendering.

---

## 10. Tab Components Lack Standard M3 Tab Bar Patterns

**Severity:** Major
**Affected screens:** tab_container (plain text tabs "Profile/Account/Settings" with only a blue underline indicator on the active tab; no icon support, no badge support visible), angry_birds_settings (tabs "Audio/General/Info" with an inactive green bar behind General/Info), fighter_menu (sub-tabs are plain text crammed together: "System Opt" "Game Optic" "Display Opt" etc.), flight_options (tabs as plain text in a row: "FLIGHT SYSTEM", "CONTROLS", "KEYBOARD", etc. with only a box around the active one), mini_motorways_settings (tabs "Game/Video/Audio/Language/Controls/Cross-Save/Credits" as plain text at varying sizes), powerwash_settings (tabs "GENERAL/GAMEPLAY/VIDEO/AUDIO/CONTROLS" as text with box around active), sports_settings (tabs "GAMEPLAY/VIDEO/AUDIO/CONTROLS" as text with box around active), deadspace_settings (tabs "Controls/Gameplay/Display and Graphic/Audio/..." as green text pills).
**Problem:** M3 defines two tab types -- Primary Tabs (full-width indicator line, centered text, optional icon) and Secondary Tabs (shorter indicator line). The toolkit's tab implementations are inconsistent: some use underlines, some use highlight boxes, some use colored backgrounds. None provide the M3-standard active indicator animation, proper ripple feedback, or support for icons alongside labels. The fighter_menu and mini_motorways_settings tabs overflow their containers, creating truncated labels.
**Guideline:** Components -- Standard M3 Tab patterns (Primary Tabs, Secondary Tabs)
**Fix:** Implement proper M3 Primary Tabs with: full-width active indicator (3dp height, Primary color), centered label text in sentence case, optional leading icon, proper min-width (90dp) and max-width constraints to prevent overflow. Add Secondary Tabs variant with shorter indicator. Ensure scrollable tab behavior when tabs exceed container width.

---

## 11. Toggle Switches Inconsistently Styled Across Themes

**Severity:** Major
**Affected screens:** angry_birds_settings (iOS-style green/gray toggles), casual_settings (identical iOS-style green/gray toggles), mini_motorways_settings (gray/orange rounded toggles), toggle_switches (dark crimson/gray toggles with small thumb), setting_row_showcase (blue/gray toggles with outlined thumb), forms (teal/gray pill toggles with "X" text), parcel_corps_settings (green toggles with white thumb, closer to M3), themes (gray/white toggle), islands_trains_settings (not visible but inferred from setting structure), layout_bug_repros (yellow/gray/orange toggle variants).
**Problem:** M3 specifies a single toggle switch design: a track with a thumb that moves between off and on positions. The on state uses Primary color for the track with a white thumb (with optional checkmark icon); the off state uses Surface Variant with an Outline-colored thumb. The toolkit implements at least 5 distinct toggle styles across different themes and screens, some resembling iOS toggles (green pill with white circle), some using filled crimson pills, and some using outlined circles. There is no consistency in thumb size, track proportions, or color mapping.
**Guideline:** Components -- Standard M3 Switch component; States -- Consistent on/off visual states
**Fix:** Standardize on the M3 switch specification: 52x32dp track with a 24dp (off) / 28dp (on) thumb. On state: Primary track + On-Primary thumb (optionally with check icon). Off state: Surface-Variant track + Outline thumb. Ensure all themes map these tokens consistently even if the actual colors differ.

---

## 12. No Visible Interaction State Layers (Hover, Pressed, Dragged)

**Severity:** Major
**Affected screens:** All interactive screens -- these are static screenshots so hover/pressed states cannot be directly observed, but the evidence is in what IS visible. The button_variants, buttons, and simple_button screens show only "enabled", "disabled", and "focused" states -- there is no demonstration of hover overlays, pressed dimming, or ripple effects. The animation_interactive screen demonstrates click scaling but no state layer color change. The drag_drop and horizontal_drag screens show draggable items but no visual "dragged" state differentiation. The tabbing screen shows focus but no hover state difference.
**Problem:** M3 defines 7 interaction states with specific opacity overlays on the container: Enabled (0%), Hover (8% of on-surface), Focus (10%), Pressed (10%), Dragged (16%), Disabled (12% container + 38% content). The toolkit appears to implement only Enabled, Disabled, and Focus states. No screens demonstrate hover overlays or pressed state feedback (beyond the animation_interactive scaling effect, which is motion-based rather than the M3 state-layer approach). Drag states in drag_drop show no visual change on the dragged item.
**Guideline:** States -- 7 interaction states with proper state layer overlays
**Fix:** Implement M3 state layers as semi-transparent overlays on interactive containers: 8% on-surface for hover, 10% for focus, 10% for pressed, 16% for dragged. These overlays should combine with the container color. Add ripple animation on press for buttons and other tappable elements. Show a visible "ghost" or elevated shadow on dragged items.

---

## 13. Slider/Progress Bar Components Lack Standard M3 Styling

**Severity:** Major
**Affected screens:** forms (sliders show as dark pill tracks with purple block thumbs -- no rounded thumb, no visible value tooltip), setting_row_showcase (sliders are dark tracks with small green square thumbs), meters_gauges (progress bars are flat colored fills with no track differentiation; "Interactive Control" slider has a thin outlined track), cozy_cafe (music slider is a green/brown track with a circular teal thumb -- closer to M3 but still custom), sports_settings (settings use segmented block indicators rather than continuous sliders), islands_trains_settings (segmented block indicators for volume).
**Problem:** M3 sliders have specific anatomy: a rounded track (active + inactive portions in different colors), a circular thumb (20dp by default, 44dp touch target), tick marks for discrete values, and an optional value label tooltip. The toolkit's sliders use square/rectangular thumbs, inconsistent track styling (some segmented, some continuous), and no value indicators on hover. The segmented "block" style used in sports_settings and islands_trains_settings is not an M3 pattern at all.
**Guideline:** Components -- Standard M3 Slider pattern; Accessibility -- 48dp minimum touch targets
**Fix:** Implement M3 Slider with: 4dp height inactive track (Surface-Variant), 4dp height active track (Primary), 20dp circular thumb (Primary with white dot), 44dp touch target area around thumb. Support discrete mode with tick marks. Add value label indicator popup on active drag. Ensure all slider variants follow this anatomy.

---

## 14. Dialog / Modal Pattern Missing M3 Structure

**Severity:** Major
**Affected screens:** modals (shows categories but trigger buttons are clipped; no visible dialog/modal content), angry_birds_settings (settings as a modal panel but no scrim/overlay behind it -- just floats on green background), casual_settings (same -- settings panel floats on green background without scrim), flight_options (full-screen settings without dialog containment), deadspace_settings (full-screen settings page, not a dialog), mini_motorways_settings (full-screen, no dialog framing), powerwash_settings (full-screen settings), sports_settings (full-screen settings).
**Problem:** M3 Dialogs have specific structural requirements: a scrim (dimmed background overlay at 32% opacity), a container with specific min/max widths (280-560dp), a headline, optional supporting text, and up to 3 action buttons aligned to the trailing edge. The settings panels in angry_birds_settings and casual_settings float on a colored background without any scrim, making them appear disconnected. Full-screen settings pages (flight_options, sports_settings, etc.) should use a Full-Screen Dialog pattern with a top app bar containing a close/back action. The modals screen appears to demonstrate dialog triggers but the actual dialogs are not shown, and the buttons clip at the edge.
**Guideline:** Components -- Standard M3 Dialog patterns (Basic, Full-Screen); Layout -- Proper overlay and scrim behavior
**Fix:** For popup settings panels (angry_birds, casual_settings), add a scrim overlay behind the dialog. Constrain dialog width to 280-560dp. For full-screen settings (flight_options, sports_settings), add a top app bar with a close icon and title. Ensure action buttons (OK/Cancel/Apply) follow M3 dialog button placement (trailing-aligned, text buttons preferred).

---

## 15. Icon Rendering Shows Artifacts and Lacks Consistent Sizing

**Severity:** Minor
**Affected screens:** images (icons for Gear, Star, Trophy, Home, Play show small pixel artifacts -- tiny characters appearing near icon edges like "i", commas, periods around the icons), empire_tycoon (navigation icons Rides/Food/Upgrades/Finance show pixelated sprite rendering with overlapping/blurred edges), neon_strike (ability icons UAV/Recon/Shield/Strike are small pixelated sprites with asterisk markers), cozy_cafe (bottom icons for Inventory/Research/Crafting are pixelated sprite icons with badge overlays), parcel_corps_settings (left-column icons use colored circles with symbol characters @, x, *, dot), kirby_options (top row icons L/WiFi/Control/Home/Mail/Star/Gear/R are colored squares with text labels rather than actual icons).
**Problem:** M3 specifies icons should use Material Symbols at a consistent size (24dp default, with 20dp and 40dp variants). The toolkit renders icons as either pixelated sprite sheets (empire_tycoon, neon_strike, cozy_cafe) or text characters inside colored shapes (kirby_options, parcel_corps_settings). The images screen shows artifacts near icon boundaries suggesting rendering issues. Icon sizes vary significantly -- some are 24px, others 40px, others 64px -- without following a consistent scale. The sprite-based approach is valid for game UIs but creates quality issues at various resolutions.
**Guideline:** Iconography -- Consistent icon style (filled/outlined), proper sizing (24dp default), Material Symbols
**Fix:** For toolkit component demos, use vector-based icons (Material Symbols or SVG equivalents) at consistent 24dp sizing. For game-specific screens, ensure sprite icons are rendered at their native resolution or properly scaled with filtering. Implement an icon size scale (Small=20dp, Medium=24dp, Large=40dp) and apply consistently. Fix the rendering artifacts visible in the images screen by ensuring proper texture sampling/filtering.

