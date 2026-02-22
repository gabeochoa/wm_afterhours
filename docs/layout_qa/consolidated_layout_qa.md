# Layout QA Consolidated Report

**Screens audited:** 76
**Total issues found:** 151
**Screens with issues:** 53

| Type | Count | Screens Affected |
|------|-------|-----------------|
| overflow | 63 | 43 |
| container_bounds | 47 | 36 |
| overlap | 30 | 20 |
| corners | 11 | 10 |

## Overflow Issues (63)

### absolute_positioning: 1. "TR" corner-pinned element clipped by container/screen edge
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** In section "2. Corner Pinning (absolute + translate)", the top-right ("TR") element is positioned at the far right edge and is partially clipped by the container boundary. The element's right side is cut off, and its full rounded shape is not visible.
**Suggested fix:** Adjust the translate offset or add padding so the TR element remains fully within the container bounds at 720p resolution.


### absolute_positioning: 2. "BR" corner-pinned element clipped by container/screen edge
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** The bottom-right ("BR") element in section 2 is similarly clipped on the right side, mirroring the TR issue. Its right edge is cut off by the container boundary.
**Suggested fix:** Apply the same inward offset fix as TR to ensure the BR element is fully visible within the container bounds.


### absolute_positioning: 3. "BADGE" overlay text truncated at container right edge
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** In section "3. Mixed: Flow children + Absolute overlay", the pink badge element at the top-right is clipped by the container's right edge. The label reads "BADG" with the trailing "E" cut off, and the element extends beyond the container's visible area.
**Suggested fix:** Reposition the badge element inward so it sits fully within the container, or increase the container's width to accommodate the badge. If the overlay is intentional, ensure the text is not truncated.


### advanced_modals: 1. Focus ring / cursor indicator overflows text input fields in all modal forms
**Type:** overflow
**Screenshot:** login_modal_open.png, wizard_step1_open.png, feedback_open.png, centering_login_open.png, centering_feedback_open.png, centering_wizard_open.png, login_validation_error.png, feedback_validation_error.png, adv_modals_login_open.png, adv_modals_feedback_open.png, adv_modals_wizard_step1.png
**Detail:** Across all modal forms (Login, Wizard Step 1, Feedback), every text input field displays a small blue square element protruding beyond the right edge of the input container. This focus/cursor indicator extends outside the input's bounding box by several pixels. Visible on the Username/Password fields in Login, Name/Email fields in the Wizard, Subject/Message fields in Feedback, and across all centering-test and reopen-test variants of these modals.
**Suggested fix:** Constrain the focus indicator or cursor element to stay within the input field bounds. Either clip it with overflow:hidden on the input container, or adjust its width/positioning so it renders inside the field's right edge.


### advanced_modals: 4. Settings slider handle overflows track on right side
**Type:** overflow
**Screenshot:** settings_volume_adjusted.png
**Detail:** In the Settings modal, when the Volume slider is adjusted to higher values (e.g., 79), the slider handle extends past the right edge of the slider track container, protruding into the modal padding area with a visible white/light overshoot region to the right of the track.
**Suggested fix:** Constrain the slider thumb so it cannot extend past the track bounds. Clamp the thumb position so its right edge stays within the track width.


### advanced_modals: 5. Dark Mode focus ring extends beyond modal content area
**Type:** overflow
**Screenshot:** settings_dark_toggled.png
**Detail:** When the Dark Mode toggle row is focused/active in the Settings modal, a white focus ring/highlight box extends to the right beyond the modal container's content area, creating a visual overflow past the modal boundary.
**Suggested fix:** Constrain the focus ring to stay within the modal's content area by applying overflow:hidden to the modal or reducing the width of the focus indicator on the toggle row.


### aim_chat: 1. Scrollbar extends below the chat window bottom edge
**Type:** overflow
**Screenshot:** idle_720p.png, text_area_arrow_keys.png
**Detail:** On the left side of the chat window, a vertical gray scrollbar-like element extends below the "Direct Connection Established" status bar, continuing past the bottom boundary of the chat window into the black background area. This element bleeds outside the window container in both the idle and text-entry states.
**Suggested fix:** Constrain the scrollbar/resize handle element to remain within the chat window bounds. Clip it at the bottom edge of the window container or adjust its height calculation.


### angry_birds_settings: 1. Tab bar background extends beyond the settings panel left edge
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** The tab bar row containing "Audio", "General", and "Info" tabs has a background that extends slightly beyond the left edge of the settings panel container. The gray/green colored bar for the tabs appears to bleed past the rounded container's left boundary.
**Suggested fix:** Clip the tab bar background to the settings panel's container bounds, or inset the tab bar so it starts within the panel's padded content area.


### angry_birds_settings: 2. Horizontal accent line extends beyond panel left edge
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** There is a thin olive/yellow horizontal line below the tab bar that extends beyond the left edge of the settings panel, bleeding past the container's rounded corner boundary.
**Suggested fix:** Constrain the horizontal divider/accent line width to match the content area within the settings panel, or clip it to the panel's border-radius bounds.


### auto_text_color: 1. Element overflows past the right edge of the viewport
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** On the right side of the screen, vertically aligned with the "Default Behavior" section, a dark blue rectangular element is mostly off-screen with only a thin sliver visible at the right viewport edge. This element overflows past the outer container's right boundary.
**Suggested fix:** Identify this element and either constrain it within the container bounds, adjust its position so it is fully visible, or remove it if it is unintended.


### cards: 1. Description text overflows past the right edge of the viewport
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** In the "Nested:" row, the description text next to the "Semi-Transparent" card ("This panel uses opacity to create a translu...") is truncated at the right edge of the screen. The word "translucent" (or similar) is cut off mid-word with no ellipsis or wrapping, indicating the text extends beyond the viewport/container boundary.
**Suggested fix:** Constrain the text element to its parent container width. Apply text wrapping or text-overflow ellipsis so the description stays within bounds. Alternatively, allow the container to expand vertically to accommodate the full text.


### cards: 2. "Nested:" label positioned outside its row container
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** The "Nested:" row label on the left side appears to sit outside and to the left of the card containers in that row, with its small dark background box not aligning with the row container structure used by "Styles" above. The label breaks the expected content padding.
**Suggested fix:** Align the "Nested:" label within the same container padding used by the "Styles" label above, ensuring consistent horizontal alignment of row labels.


### casual_settings: 2. "Sync" button text clipped by circular boundary
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** The green circular "Sync" button next to "Save/Load Progress" has its text label ("Sync") slightly clipped on the right side by the button's circular boundary. The rightmost character is partially truncated, and the button sits very close to the container's right edge.
**Suggested fix:** Increase the button diameter to fully accommodate the text label, or reduce the font size so the label fits cleanly within the circle.


### checkboxes: 1. Disabled checkbox section overflows container bottom edge
**Type:** overflow
**Screenshot:** idle_720p.png, custom_commands_test.png
**Detail:** In the left column, the "Disabled" section with "Disabled ON" and "Disabled OFF" rows extends past the visible bottom boundary of the left panel container. The bottom of "Disabled OFF" appears clipped, and a diamond decorator element between the rows overlaps the container border.
**Suggested fix:** Increase the left panel container height to fully accommodate the disabled checkbox rows, or clip the diamond decorator to the container bounds.


### circular_progress: 1. Fourth progress indicator overflows outside container grid
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** On the far right side of the screen, a "62%" text and "Thick ring" label appear outside the bounded container area that holds the first three indicators (Basic, Animated, Thick Ring). These elements float in open space to the right of the container with no enclosing panel, appearing to be an overflowing fourth item that escaped the top-row container's right boundary.
**Suggested fix:** Expand the top-row container to include all items, or remove the duplicate label if it is unintended overflow.


### deadspace_settings: 1. Tab labels truncated in settings tab bar
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** Several tab labels in the horizontal tab bar are truncated: "Display and Graphic" (missing 's'), "Language and Subti" (missing "tles"), and "Policies and Licens" (missing "ing"). The tab bar does not provide enough horizontal space per tab to display the full text of longer labels.
**Suggested fix:** Use abbreviated labels that fit fully (e.g., "Display", "Language", "Policies"), reduce the tab label font size, implement horizontal scrolling for the tab bar, or widen the tab bar to accommodate full labels.


### decorators: 1. with_quote() first quote text overflows container right edge
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** In the right column (with_quote() section), the first quote "The only way to do great work is to love what you do." extends to or past the right edge of the viewport. The text does not wrap within its container, causing the end of the quote to be clipped by the screen boundary.
**Suggested fix:** Constrain the quote container width so text wraps to multiple lines within the visible viewport. Ensure the with_quote() container has a max-width that respects screen bounds.


### dropdowns: 1. Dropdown menu overflows its parent container card and overlaps the card below
**Type:** overflow
**Screenshot:** dropdown_open_initial.png, dropdown_arrow_down_once.png, dropdown_arrow_down_twice.png
**Detail:** When the "With Label" dropdown (center-top card) is opened, the dropdown option list extends downward past the bottom edge of its parent card container and visually overlaps the "Disabled" card in the row below. The dropdown list items ("Dog", "Cat", "Bird", "Fish", "Disabled") extend well beyond the card's lower boundary, covering the "Disabled" title bar of the card below.
**Suggested fix:** Either render the dropdown menu in an overlay/portal layer that floats above all cards (so the overlap is intentional and z-ordered correctly), or constrain the dropdown to scroll within the card bounds. If the overlay approach is intended, ensure the card below is not receiving input events from the overlapping dropdown area.


### example_borders: 1. "Soft Shadow" and "Outline Only" labels truncated at right edge
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** In the "Corners & Shadows" row, the "Soft Shadow" label text appears to be clipped/truncated at the right side of its card. Similarly, in the "Themed Styles" row, the "Outline Only" label is partially cut off at the right edge of the visible area. The rightmost cards in several rows are positioned very close to the viewport edge.
**Suggested fix:** Add horizontal padding or margin on the right side of the overall layout container, or make the grid responsive so that rightmost cards have sufficient space for their labels.


### example_borders: 2. "Per-Side" row labels truncated
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** In the bottom "Per-Side" row, the labels "Bottom Only", "Left+Right", and "Underline" appear to have their text cut off or tightly compressed. "Bottom Only" shows as "Bottom On" with the rest clipped, and "Left+Right" text is similarly squeezed.
**Suggested fix:** Increase card width in the Per-Side row, or allow text to wrap within the card labels to prevent truncation.


### fighter_menu: 1. System Options description text overflows off-screen to the right
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** In the right-side panel, the "System Options" description text reads "Configure vibration, save data, and a..." with the text visibly cut off at the right edge of the screen. The text extends beyond the visible viewport and is not fully readable.
**Suggested fix:** Add text wrapping to the description text container, or constrain the right panel width so that all content remains within the viewport bounds. Alternatively, use text-overflow ellipsis with a tooltip for long descriptions.


### file_tree: 1. Status bar text at bottom is clipped by container edge
**Type:** overflow
**Screenshot:** file_tree_initial.png, file_tree_after_home.png, file_tree_after_refresh.png, file_tree_after_up.png, file_tree_navigated.png, file_tree_item_activated.png, file_tree_final.png
**Detail:** The bottom status bar text (showing "No selection" or the current path like "/Users/gabeochoa/p/Fonts") is positioned at the very bottom edge of the file tree container. The text appears to sit on or very close to the container's bottom border, with the bottom portion of the text potentially clipped. In screenshots showing longer paths (e.g., "/Users/gabeochoa/p/Fonts/Radiance"), the text is partially cut off at the right.
**Suggested fix:** Add bottom padding within the container to give the status bar text breathing room, and ensure long paths are truncated with ellipsis or horizontally scrollable rather than being clipped.


### flex_alignment: 1. Cross-Axis Align demo items overflow their container boxes
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** In the bottom-left "Cross-Axis Align (AlignItems)" section, the demo items (numbered blocks 1, 2, 3) in the "Start" and "End" columns appear to extend slightly beyond their gray container boundaries. The pink numbered blocks have portions that visually bleed past the container edges, particularly at the bottom.
**Suggested fix:** Increase the container height for the Cross-Axis Align demo boxes, or reduce the size of the demo elements so they fit fully within their parent containers.


### forms: 1. Right-panel content clipped at right edge
**Type:** overflow
**Screenshot:** forms_before_scroll.png, idle_720p.png, forms_after_scroll.png
**Detail:** On the right side of the main content area, there is a partially visible element clipped by the container's right edge. A small teal/cyan fragment and what appears to be a truncated letter ("A") are visible at approximately x=1030-1040, around the vertical midpoint of the right-side settings panel. This is consistent across all three screenshots, indicating the right column's content or a sibling element extends beyond the visible container boundary.
**Suggested fix:** Ensure the right-side panel (containing settings toggles like Music, SFX, Fullscreen, V-Sync, Show FPS) and any adjacent elements fit within the parent container's bounds. Either widen the container, reduce the panel width, or apply `overflow: hidden` / clip at the correct boundary. Check if an extra element is being positioned outside the intended layout area.


### forms: 2. Slider handle overflows its track container
**Type:** overflow
**Screenshot:** forms_before_scroll.png, idle_720p.png, forms_after_scroll.png
**Detail:** The Volume slider (75%) has a visible drag handle (small white rectangle) that protrudes past the right edge of its slider track container at approximately x=595. The handle extends beyond the slider's visual bounds.
**Suggested fix:** Either extend the slider track container width to accommodate the handle at full-value positions, or clip the handle within the track bounds. Alternatively, adjust the handle positioning so it stays within the track's visual boundary at maximum values.


### forms: 3. Scrolled state shows text clipping at top of right panel
**Type:** overflow
**Screenshot:** forms_after_scroll.png
**Detail:** In the scrolled state, the top of the right-side panel shows partially clipped text. "V-Sync" and "V" label text at the very top of the panel appears cut off, with the upper portion of characters truncated by the container's top edge. This suggests the scroll position reveals content that bleeds past the container's visible clip area.
**Suggested fix:** Ensure the scrollable region clips content cleanly at its top edge, or add padding so that items at the scroll boundary are fully visible or fully hidden rather than partially clipped.


### horizontal_drag: 1. Tag overflows High Priority lane container on the right
**Type:** overflow
**Screenshot:** horizontal_drag_initial.png, horizontal_drag_after_first_move.png, horizontal_drag_after_second_move.png, horizontal_drag_final.png, horizontal_drag_reorder.png, idle_720p.png
**Detail:** In the "High Priority (2)" lane, a tag (appears to be labeled "Rea..." — likely "Reassign" or similar) is partially visible at the far right edge of the container, clipped by the lane boundary. Only the first few characters ("Rea") and the left portion of the tag are visible. The tag extends beyond the right edge of the High Priority container in all six screenshots, indicating a persistent overflow issue where the lane does not have enough horizontal space to display all its tags.
**Suggested fix:** Either enable horizontal scrolling within the priority lane so all tags are accessible, or implement wrapping so tags flow to a second row when they exceed the lane width. Alternatively, ensure the lane container is wide enough to accommodate all tags, or truncate tag text with ellipsis while keeping tags fully within bounds.


### hstack_showcase: 1. Element clipped at right edge of the screen
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** At the far right edge of the screen (approximately x=1245-1280), there is a partially visible element — what appears to be a cyan/teal rectangle with the letter "A" — that is clipped by the screen boundary. This element sits to the right of the "Toolbar Pattern" section area, suggesting it overflows the main content container or viewport.
**Suggested fix:** Ensure all elements within the hstack showcase fit within the viewport width. Check if the toolbar pattern section or a sibling element has excessive width or positioning that pushes content beyond the right edge. Add appropriate constraints or reduce element sizing to keep everything within bounds.


### images: 1. Sprite source rectangles partially visible next to icons
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** Across all four rows (Sprites, Image Buttons, With Background, Icon Row), each icon/sprite displays small artifact fragments to its right — visible as thin partial rectangles or edge lines adjacent to the gear, star, trophy, home, and play icons. These appear to be sprite source rectangles or debug bounds that are not fully clipped, bleeding past the intended icon display area. The artifacts are small but consistently present across all icon instances.
**Suggested fix:** Verify that sprite source rectangles are correctly sized and that the rendering clips to the intended icon bounds. If these are debug visualization artifacts, disable them. If they are part of the sprite sheet bleeding through, ensure the UV/source rect is precisely cropped to the icon content area.


### images: 2. "With Background" row content clipped at bottom
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** In the "With Background" row (third row), there appears to be content partially cut off at the bottom of the container. A small fragment is visible below the Settings button with background, suggesting the container height is slightly too short for its content.
**Suggested fix:** Increase the height of the "With Background" row container to fully encompass its content, or verify the vertical padding is sufficient for the child elements.


### kirby_options: 2. "Common" label partially clipped or misaligned
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** The "Common" text label above the tab bar (Controls, Display, Accessibility, Sound, Network) at approximately y=230 appears very small and slightly misaligned relative to the tab row below it. The text sits close to the upper edge of its container area with minimal padding, making it look cramped.
**Suggested fix:** Add top padding or margin above the "Common" label to give it more breathing room within the content panel, ensuring it has consistent spacing relative to the elements above and below it.


### layout_bug_repros: 1. Red child overflows cyan parent container (Bug Repro #1)
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** In the "[NOT A BUG] 1: percent(1.0) inside absolute parent" section at the top-left, the red child element labeled "percent(1.0) child" extends beyond the right edge of the cyan parent container. The annotation confirms: "Expected: red fits inside cyan | Bug: red overflows right."
**Suggested fix:** Ensure that `percent(1.0)` sizing accounts for the parent's padding/border so the child fits entirely within the cyan container bounds. If the child is sized at 100% of the parent, the parent's padding must be subtracted from the available width.


### layout_bug_repros: 2. Toggle container overflows at bottom (Bug Repro #7)
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** In the "[NOT A BUG] 7: toggle_switch vertical space" section on the right side, the green-bordered container holding three toggles (Toggle A, B, C) has its bottom content clipping or overflowing. The annotation reads "Expected: 3 toggles fit inside green border | Bug: overflow bottom." The third toggle row is very close to or touching the container boundary.
**Suggested fix:** Increase the container height or use auto-sizing to ensure all three toggle rows plus padding fit within the green border.


### meters_gauges: 1. Shield bar extends beyond parent container
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** In the "Stacked Bars (Health + Shield)" section, the shield bar (labeled "Shield: 29%") extends noticeably past the right edge of its parent container. The bar's track overshoots the container boundary by several pixels into the dark background area.
**Suggested fix:** Constrain the shield bar's maximum width to respect the parent container's right padding/boundary. Ensure the stacked bar layout accounts for both bars fitting within the available container width.


### meters_gauges: 2. Slider handle/track extends beyond the Interactive Control container
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** In the "Interactive Control" section at the bottom, the white slider track/handle element extends past the right edge of the green progress bar fill area. The white track portion protrudes beyond the container's right boundary.
**Suggested fix:** Ensure the slider track is clipped to the container bounds, or adjust the container width to accommodate the full slider track length including the handle.


### nine_slice_borders: 2. Dialog text truncated in Interactive Elements section
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** In the "Interactive Elements" row at the bottom, the dialog panel contains text "This is a dialog using 9-slice. Corn..." which is truncated. The text overflows the visible area and is clipped rather than wrapping or being fully contained.
**Suggested fix:** Either increase the dialog panel width to fit the full text, enable text wrapping within the container, or ensure text ellipsis is applied cleanly.


### parcel_corps_settings: 1. Settings panel content clipped at bottom edge of viewport
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** The settings panel on the left extends to the very bottom of the viewport. The "Icon Legend" section is cramped against the bottom, and a teal element is partially visible below the legend, clipped by the screen boundary. The panel does not provide enough space to fully render all its content.
**Suggested fix:** Add bottom padding to the settings panel, or make the settings list scrollable so the legend and any elements below it are fully visible without clipping at the viewport edge.


### parcel_corps_settings: 2. Objective panel text runs flush against right screen edge
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** In the upper-right area, the objective/quest panel text "[X] SIGN UP A BUSINESS TO YOUR APP" and "[ ] Lv0 SIGN UP 4 BUSINESSES TO YOUR APP" extends to the very edge of the viewport. The text runs flush against the right screen boundary with no margin.
**Suggested fix:** Constrain the objective panel to a fixed max-width with right margin, or enable text wrapping/truncation so content does not extend beyond the safe area.


### potion_crafting: 1. Potion bottle stopper overflows brewing panel top boundary
**Type:** overflow
**Screenshot:** idle_720p.png, potion_crafting_after_first_tab.png, potion_crafting_final.png, potion_crafting_initial.png, potion_crafting_recipe_navigated.png, potion_crafting_tab_clicked.png
**Detail:** In the center brewing panel, the potion bottle graphic's stopper/cap extends above the panel's top content area. The bottle neck protrudes past the container boundary into the header region ("Brewing: Mana Elixir" / "Brewing: Healing Potion" / "Brewing: Antidote"). This is visible in all 6 screenshots and is consistent regardless of which recipe is selected.
**Suggested fix:** Either increase the top padding of the brewing panel to accommodate the full bottle graphic, reduce the bottle size, or move the bottle graphic downward within the panel so the stopper does not cross the container boundary.


### radio_buttons: 1. Focus rectangle on "Apple" option overflows its radio group container
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** In the "Favorite Fruit" radio group (left column), the "Apple" option has a large white focus/selection rectangle that extends to the full width of the panel and pushes against or past the container's left and right inner edges. The focus outline is visually much wider than the other radio button rows (Orange, Watermelon), creating an inconsistent appearance and potentially overflowing the container's inner padding bounds.
**Suggested fix:** Constrain the focus rectangle width to the radio group's content area. Clamp the focus indicator's width to the container's inner padding bounds so it matches the width of the other option rows.


### rubber_bandits_menu: 1. Tooltip text clipped at bottom viewport edge
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** The green tooltip bar at the very bottom of the screen ("Change the game's settings") is positioned flush against the bottom edge of the viewport. The text's bottom portion appears slightly cropped by the screen boundary, with the bottom of the letters touching or being cut off.
**Suggested fix:** Add bottom margin or padding to the tooltip bar so it sits a few pixels above the viewport's bottom edge, ensuring the full text height is visible.


### rubber_bandits_menu: 2. Character selector panel tight against bottom edge
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** The "SELECT CHARACTER" panel at the bottom-center is positioned low enough that the "FAVOURITE" label at the bottom of the panel sits very close to the viewport edge. The panel's bottom border has minimal clearance from the lower boundary.
**Suggested fix:** Move the character selector panel upward or add bottom margin to ensure the full panel including the "FAVOURITE" label has clearance from the viewport edge.


### scroll_click_bug: 1. Bottom button clipped by scroll container boundary
**Type:** overflow
**Screenshot:** idle_720p.png, scroll_click_before.png
**Detail:** In the initial/non-scrolled view, "Button 6" at the bottom of the scroll container is partially cut off by the container's bottom edge. Only the top portion of the button is visible — roughly the upper third — with the rest clipped by the container's boundary. The scroll container's height does not evenly accommodate the visible buttons, leaving the last one partially overflowing.
**Suggested fix:** Adjust the scroll container height to either fully show or fully hide the last button at the boundary. Alternatively, add a visible scrollbar or scroll indicator to communicate that additional content exists below the visible area.


### scroll_click_bug: 2. Bottom buttons clipped after scrolling
**Type:** overflow
**Screenshot:** scroll_click_after_click.png, scroll_click_after_scroll.png
**Detail:** After scrolling down, "Button 10" at the bottom of the list is similarly clipped by the scroll container's bottom edge. The button is partially visible with its lower portion cut off, confirming the container height creates a consistent partial-button cutoff regardless of scroll position.
**Suggested fix:** Size the scroll container so that buttons are either fully visible or fully hidden at the boundary. Consider snapping the scroll position to show complete buttons.


### scroll_click_bug: 3. Top button clipped after scrolling
**Type:** overflow
**Screenshot:** scroll_click_after_click.png, scroll_click_after_scroll.png
**Detail:** After scrolling down, "Button 4" at the top of the visible area is partially clipped by the container's top edge. The top portion of the button is cut off, mirroring the bottom clipping issue. Both the topmost and bottommost visible buttons are partially rendered after scrolling.
**Suggested fix:** Apply the same container height adjustment as the bottom clipping fix to ensure consistent full-button visibility, or add overflow clipping with a visual fade indicator at both the top and bottom edges to signal truncated content.


### scroll_view: 1. Vertical scroll container clips bottom item at an awkward position
**Type:** overflow
**Screenshot:** idle_720p.png, scroll_view_before_scroll.png, scroll_view_after_scroll.png
**Detail:** The last visible item in the vertical scroll list (e.g., "Item 5" in the initial state, "Item 11" after scrolling) is clipped at the container's bottom border. Roughly half of the item row is visible, with the text cut off mid-row. The rounded bottom corners of the container cut into the item's rectangular background, creating a jagged visual. This is consistent across scroll positions except when scrolled to the bottom (where Item 20 fits cleanly).
**Suggested fix:** Add bottom padding inside the scroll container, or snap the viewport height to a multiple of item height plus spacing so items are either fully visible or barely peek in to invite scrolling.


### self_align: 1. "FlexStart (left)" element overflows the container's left edge
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** The green "FlexStart (left)" button extends beyond the left border of the rounded-corner demonstration container. The button's left edge sits outside the container's border line, and its top-left corner visually bleeds past the container's rounded corner, breaking the containment boundary.
**Suggested fix:** Add left padding inside the container or constrain the child element so it respects the container's border and border-radius. The element should be fully contained within the parent's visible bounds.


### self_align: 2. "FlexEnd (right)" element overflows the container's right edge
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** The orange-brown "FlexEnd (right)" button extends beyond the right border of the rounded-corner container. The button's right edge sits outside the container border, and its bottom-right corner bleeds past the container's rounded corner.
**Suggested fix:** Add right padding inside the container or constrain the child so it stays within the container's border-radius clipping area. Apply overflow clipping or equivalent to the container.


### setting_row_showcase: 2. "Music Volume" slider row is cramped at the container bottom
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** The "Music Volume" row at the bottom of the VOLUME section sits at the very bottom edge of the container. The slider track and value label ("65%") are present but the row has minimal or no bottom padding, making it look cramped against the container's bottom border.
**Suggested fix:** Add bottom padding inside the container so the last settings row has breathing room before the container edge.


### shop_interface: 2. Icon badge text clipped in item list
**Type:** overflow
**Screenshot:** idle_720p.png, shop_buy_tab.png, shop_cart_details.png, shop_initial.png, shop_item_selected.png, shop_sell_tab.png
**Detail:** The icon badges in the left column (SWD, ARM, FIR, TCH, ANT, SHL) have their text tightly packed within the small square badge containers. "SWD" for Iron Sword appears slightly clipped at the right edge. This same issue appears to varying degrees on other 3-character badges.
**Suggested fix:** Widen the icon badge containers slightly, or reduce the font size within the badges, so that all abbreviations fit with adequate padding.


### shop_interface: 3. Cart panel price values crowd right container edge
**Type:** overflow
**Screenshot:** idle_720p.png, shop_buy_tab.png, shop_cart_details.png, shop_initial.png
**Detail:** In the cart section on the right side, the price values "150 G", "30 G", and the total "180 G" are positioned at the far right edge of the cart container. The "180 G" total value appears to press against the right border of the panel with minimal padding.
**Suggested fix:** Add right padding inside the cart container to ensure price values don't crowd the container's right border.


### sports_settings: 2. "Anti-Aliasing method" value text crowds its stepper controls
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** The highlighted row "Anti-Aliasing method" has the value "TSR (Temporal Super Resolution)" which is a long string. The text fills the entire space between the `<` and `>` stepper arrows with very little padding, and the text's background highlight box crowds adjacent elements. The visual bar indicators to the right are also tightly packed.
**Suggested fix:** Allow the value text area to accommodate longer strings with proper padding, or truncate/abbreviate long values with an ellipsis. Ensure the stepper arrows have consistent spacing from the value text.


### stepper_showcase: 1. Card description text overflows right edge of the viewport
**Type:** overflow
**Screenshot:** idle_720p.png, stepper_initial.png, stepper_card_back_to_rogue.png, stepper_card_healer.png, stepper_card_mage.png, stepper_card_rogue.png, stepper_final.png, stepper_resolution_advanced.png, stepper_resolution_advanced_twice.png, stepper_resolution_back.png
**Detail:** In the Card Selector panel, the Warrior card's description text "Heavy armor, melee attacks, high defense." is clipped at the right edge of the viewport. The word "high" is partially cut off, with only the "h" visible before the text disappears beyond the screen boundary. This is consistent across all 10 screenshots since the card is always visible.
**Suggested fix:** Constrain the Card Selector panel and its child card so they fit within the viewport. Either reduce the panel width, add a right margin, or enable text wrapping so the description stays within the visible area.


### stepper_showcase: 2. "HealerWarriorMage" stepper text lacks spacing between options
**Type:** overflow
**Screenshot:** idle_720p.png, stepper_initial.png, stepper_card_back_to_rogue.png, stepper_card_healer.png, stepper_card_mage.png, stepper_card_rogue.png, stepper_final.png, stepper_resolution_advanced.png, stepper_resolution_advanced_twice.png, stepper_resolution_back.png
**Detail:** The Card Selector stepper displays "HealerWarriorMage" as a single concatenated string between the `<` and `>` arrows. The individual option names (Healer, Warrior, Mage) run together without any separator, spacing, or visual differentiation, making it unclear which option is currently selected.
**Suggested fix:** Display only the currently selected option name (e.g., "Warrior") in the stepper rather than concatenating all available options. Alternatively, add separators or spacing between the names.


### text: 1. Body text truncated with ellipsis indicating horizontal overflow
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** In the left column under "Text Size Hierarchy," the body text reads "Body text for readable content that flows natu..." — it is cut off with an ellipsis. The text content exceeds the available container width and is being truncated. Since this is a typography showcase screen intended to demonstrate readable body text, the truncation defeats the purpose of the demonstration.
**Suggested fix:** Widen the left column container or enable multi-line text wrapping so the full body text sentence is displayed without truncation.


### text_input: 1. Bottom status bar and action buttons clipped at viewport edge
**Type:** overflow
**Screenshot:** idle_720p.png, focus_and_type.png, keyboard_input_test.png
**Detail:** The bottom status bar containing instructional text ("Enter your details above" / "Username: a" / "Submitted! User: , Email:") and the "Tab / Enter" hint extends to the very bottom of the viewport. The OK/Cancel/Apply buttons sit flush against the bottom edge with no visible margin below them. This is consistent across all three screenshots and the bottom portion appears clipped.
**Suggested fix:** Add bottom margin or padding below the action buttons row so they don't sit flush against the viewport edge. Reduce content height above or move the status bar up to fit comfortably within the 720p frame.


### text_overflow: 1. Bottom-right explanatory text extends close to viewport edge
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** The text at the bottom-right reading "Minimum touch target size is 44px. Smaller containers trigger overflow warnings." extends very close to the right edge of the viewport with minimal right margin. On narrower displays or with slight layout shifts, this text could be clipped.
**Suggested fix:** Constrain this text element's width to stay within the viewport bounds with adequate margin, or enable text wrapping so it flows to a second line instead of stretching to the edge.


### text_overflow: 2. Red-bordered containers show intentional overflow for demo purposes
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** In the "Problem: Text Too Large" section, several containers display text overflowing their bounds (e.g., "This is way too..." with red corner indicators, "Height is too small!" in a height-constrained container, and "..." in a tiny 44x44 box). These are intentional demonstrations of overflow behavior with debug mode indicators enabled. The screen title confirms "Debug mode is ON — red corners appear when text is too large for its container."
**Suggested fix:** No fix needed — these are intentional overflow demonstrations serving as educational examples of the debug system.


### text_shadow: 1. Title "Text Drop Shadow" clipped at top of viewport
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** The main title "Text Drop Shadow" at the top of the screen has its upper portion cut off by the top edge of the viewport. The ascenders of the title text and the shadow effect above them are partially obscured. The subtitle "Add depth and improve legibility with text shadows" also sits very close to the top edge with minimal clearance.
**Suggested fix:** Add sufficient top padding/margin above the title so the full text including its shadow offset is visible within the viewport. Push the entire content layout down to give the title clearance.


### text_stroke: 1. Tip bar at the bottom is partially clipped at the viewport edge
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** The green "Tip: Use 2-6px for readable text, 6-10px for decorative titles and headers" bar at the bottom of the screen extends to the very bottom of the viewport. The bottom border/padding of the tip container appears cut off with no margin below it, making it look clipped rather than self-contained.
**Suggested fix:** Add bottom margin or reduce overall content height so the tip bar and its bottom padding are fully visible within the 720p viewport.


### themes: 1. Active theme label "Ocean Navy" truncated with orphaned parenthesis
**Type:** overflow
**Screenshot:** idle_720p.png, themes_test.png
**Detail:** In the left sidebar under "Select Theme," the currently active theme is displayed as "> Ocean Navy (" — there is clearly additional text after the opening parenthesis that is cut off by the sidebar container width. The orphaned "(" character makes the truncation appear as a bug rather than intentional ellipsis. This is visible identically in both screenshots.
**Suggested fix:** Widen the sidebar panel to accommodate the full selected-theme label including any active indicator text, or apply proper ellipsis truncation that doesn't leave orphaned punctuation visible.


### toasts: 1. Toast notifications clip against right screen edge
**Type:** overflow
**Screenshot:** toasts_spam_triggered.png, toasts_final.png, toasts_quick_triggered.png
**Detail:** When multiple toasts are stacked in the bottom-right corner, several toast messages extend flush to the right edge of the viewport with no visible right margin. In `toasts_spam_triggered.png` with 4+ simultaneous toasts, the "[!] Warning: Check your settings" and "[*] Custom colored toast!" text reaches the screen boundary. The same right-edge flush positioning is visible in `toasts_final.png` and `toasts_quick_triggered.png`.
**Suggested fix:** Add a consistent right margin (e.g., 8–16px) to the toast container so toasts never touch the viewport edge. Ensure the toast max-width accounts for this margin.


### vstack_showcase: 1. "Sidebar + Content" right panel overflows container and viewport
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** In the "Sidebar + Content" section (bottom-right), the right-side content panel items ("Content" header, "Alerts", "Activity", "Stats") extend beyond the rounded container boundary and reach the right edge of the viewport. The colored bars bleed past the container's right border, breaking visual containment.
**Suggested fix:** Constrain child element widths within the content panel so they respect the container's inner bounds and rounded corners. Add right padding or set a max-width on the content panel.


## Container Bounds Issues (47)

### advanced_modals: 3. Modal backdrop does not cover the top edge of the viewport
**Type:** container_bounds
**Screenshot:** login_modal_open.png, settings_open.png, feedback_open.png, wizard_step1_open.png, wizard_step2.png, wizard_step3_summary.png, centering_login_open.png, centering_settings_open.png, centering_feedback_open.png, centering_wizard_open.png
**Detail:** When any modal is open, a thin strip of un-dimmed cyan/teal accent color is visible at the very top of the screen above the darkened backdrop. This appears across all modal types (Login, Settings, Feedback, Wizard) and in both the standard and centering-test screenshots, indicating the backdrop overlay does not start at y=0 or does not cover the full viewport height.
**Suggested fix:** Ensure the modal backdrop overlay starts at the absolute top of the viewport (y=0) and spans the full screen dimensions so no underlying content peeks through at the top edge.


### auto_text_color: 2. "Near Black" swatch lacks visible container background
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** In the "Default Behavior" section top row, the "Near Black" label appears without a visible pill/card background, unlike "Dark Blue", "Dark Purple", and "Dark Green" which all have visible container backgrounds. The near-black background merges with the dark page background, making the container bounds invisible.
**Suggested fix:** If this is intentional (demonstrating auto text color on near-black), consider adding a subtle border so the swatch boundaries are discernible. If unintentional, ensure the container renders with a visible background.


### cards: 3. "Styles" label vertical misalignment with card elements
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** In the "Styles" row, the "Styles" label and the "Surface" text-only card sit at a different vertical position than the taller card elements ("Primary", "Sharp", "Custom") which extend above them. This creates uneven vertical alignment within the row. Additionally, the "Surface" card variant renders with no visible border or background differentiation from the parent container, making its spatial extent ambiguous.
**Suggested fix:** Vertically center all elements in the Styles row. Consider adding a subtle border to the Surface card so its bounds are distinguishable from the background.


### casual_settings: 3. Asymmetric left/right padding in main container
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The left side of the settings container has noticeably more internal padding than the right side. The Audio section starts well-inset from the left edge, but the right-column buttons (Credits, Support, Terms and Privacy) and the OK/Cancel/Apply buttons extend closer to the right border with less margin.
**Suggested fix:** Ensure consistent horizontal padding on both sides of the container.


### casual_settings: 4. "Terms and Privacy" button breaks two-column grid symmetry
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The Menu section uses a two-column layout (Notifications/Credits, Language/Support), but "Terms and Privacy" sits alone in the right column with no counterpart on the left, creating a visual gap and asymmetric bottom row.
**Suggested fix:** Center "Terms and Privacy" across both columns if it is the only item in the last row, or span it full-width.


### checkboxes: 2. Status text sits outside main container
**Type:** container_bounds
**Screenshot:** idle_720p.png, custom_commands_test.png
**Detail:** The "Group selected: 2 of 4" and "MinMax selected: 1 of 3" text at the bottom of the screen sits outside and below the main content container boundary, floating independently in the background area.
**Suggested fix:** Extend the main container to include the status text, or add a dedicated footer row within the container for selection status information.


### checkboxes: 3. Unchecked rows lack consistent indicator space
**Type:** container_bounds
**Screenshot:** idle_720p.png, custom_commands_test.png
**Detail:** The "Secondary" and "Box Only" checkbox rows fill their full width with background color, leaving no visible reserved space for a checkmark indicator on the right side. Checked rows like "Primary" and "Accent" show "V" marks, but unchecked rows don't reserve consistent space for the indicator area.
**Suggested fix:** Reserve consistent right-side space for the checkmark indicator in all labeled checkbox rows, regardless of checked/unchecked state.


### circular_progress: 3. Left container clips against left screen edge
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The container holding "Basic" and "Thin Ring" sections has its left border partially clipped by the left edge of the viewport. Only a sliver of the left border is visible, suggesting the container extends past or sits flush with the screen edge.
**Suggested fix:** Add left margin to ensure the container's left border is fully visible within the viewport.


### circular_progress: 4. Size Variations row has inconsistent container sizing
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The bottom row of size variations (28px, 40px, 52px, 64px, 80px) has inconsistent card styling. The first two sizes (28px, 40px) share or overlap with the Thin Ring section container, while the remaining three (52px, 64px, 80px) have distinct individual cards of increasing height, creating a staircase appearance.
**Suggested fix:** Apply uniform card height to all size variation containers and ensure each has its own distinct card background.


### cozy_cafe: 2. Bottom-right navigation icons extend to screen edge
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The Inventory, Research, and Crafting icon buttons in the bottom-right extend very close to the right edge of the viewport. "Crafting" and its label sit nearly flush against the right boundary. The red notification badges ("2" on Inventory, "1" on Research) extend even further toward the edge.
**Suggested fix:** Add right and bottom margin to the navigation icon group to provide consistent spacing from viewport edges.


### cozy_cafe: 3. Music slider lacks containing panel
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The music volume slider and "Music: 70%" label at the bottom-left float independently without a visible containing panel, unlike "Today's Specials" and "Customers" above which have clear bordered containers. The slider sits in unstructured space between the specials panel and the chat messages area.
**Suggested fix:** Wrap the music slider in a container panel consistent with the style of other sections, or integrate it into an existing panel.


### deadspace_settings: 3. Action buttons float mid-panel in large empty content area
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The main content area for "Controls" shows only a title and placeholder text, with OK/Cancel/Apply buttons positioned at mid-right. There is a large expanse of empty space below the buttons to the bottom of the panel, and the buttons float without being anchored to the content area's bottom boundary.
**Suggested fix:** Position the action buttons at the bottom of the content area, or vertically constrain the content area to fit actual content with buttons anchored at the bottom.


### decorators: 3. with_grid_bg() panels have inconsistent vertical spacing and sizing
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The three panels in the with_grid_bg() center column have uneven vertical spacing and different heights. The gap between the first and second panel differs from the gap between the second and third. The small green/yellow square indicators on the left side of the panels also vary in vertical positioning.
**Suggested fix:** Normalize the vertical gap between all three grid background panels and apply consistent panel heights throughout the column.


### dropdowns: 2. Bottom status bar text may be clipped at narrow widths
**Type:** container_bounds
**Screenshot:** dropdown_after_arrow_select.png, idle_720p.png
**Detail:** The bottom summary bar ("Basic=Alpha, Pet=Bird, Month=January, Indicator=Option A, Disabled=Frozen, Styled=Ruby") is a single long line. While it fits in the current 720p resolution, the text is approaching the container's horizontal edges, and at slightly narrower viewports it could overflow.
**Suggested fix:** Add text wrapping or horizontal scrolling to the summary bar, or reduce font size to ensure it stays within bounds at all supported resolutions.


### empire_tycoon: 3. Right-side decorative elements clipped at screen edge
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** On the far right side of the screen, there appear to be decorative elements (what looks like a parasol/umbrella icon) that are partially cut off at the right edge of the viewport. The top-right area also shows items that appear clipped.
**Suggested fix:** Ensure decorative elements either fit fully within the viewport or are intentionally designed to bleed off-screen. If they should be visible, adjust their positioning or the viewport width.


### empire_tycoon: 4. Chat log text in bottom-left lacks contained boundary
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The "GlobalChat" and "DevTeam" message lines at the bottom left sit in what appears to be a semi-transparent container, but the container's left edge appears to extend nearly to the screen edge with minimal padding. The container boundary is not well-defined compared to other UI panels.
**Suggested fix:** Add a clearer bounded container with consistent padding for the chat log area, matching the visual style of other panels on the screen.


### example_borders: 3. Row label text ("Width & Color", "Corners & Shadows") left-aligned outside card grid
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The row section labels on the left side ("Width & Color", "Corners & Shadows", "Themed Styles", "Interactive", "Per-Side") are positioned to the left of the card grid. "Corners & Shadows" wraps to two lines due to limited horizontal space. This left-label layout causes the cards to start further right, contributing to the right-edge truncation issues.
**Suggested fix:** Consider placing row labels above their respective rows instead of to the left, or allocate more horizontal space for the label column so it doesn't compress the card grid area.


### fighter_menu: 2. Right panel extends beyond viewport boundary
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The entire right-side panel containing "System Options" with its icon, title, and description appears to extend past the right edge of the screen. The panel's right border is not visible, suggesting it is clipped by the viewport.
**Suggested fix:** Constrain the right panel's width or adjust its horizontal position so it fits fully within the 720p viewport. Ensure the panel's right border and padding are visible.


### fighter_menu: 3. Tab row items "Sound & Language", "Network Options", "Button Settings" appear visually compressed
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The secondary tab bar below the main navigation ("System Options", "Game Options", "Display Options", "Sound & Language", "Network Options", "Button Settings") shows the rightmost tabs with darker/grayed styling that appears compressed. The tab labels may be running into each other with insufficient spacing.
**Suggested fix:** Either reduce the number of visible tabs and add horizontal scrolling, or decrease tab label font size to ensure all tabs fit with adequate padding.


### file_tree: 2. File list can potentially overflow container without scrollbar indication
**Type:** container_bounds
**Screenshot:** file_tree_item_activated.png, file_tree_navigated.png
**Detail:** When the "Fonts/" directory is expanded, the file list grows significantly (showing many .ttf and .otf files). The list appears to extend to the very bottom of the container with the last visible entries close to the status bar. There is no visible scrollbar or scroll indicator to show that more content may exist below the visible area.
**Suggested fix:** Add a visible scrollbar or scroll indicator when the file list exceeds the container height, so users know there is more content to scroll through.


### file_tree: 3. Highlight selection bar extends full width without padding
**Type:** container_bounds
**Screenshot:** file_tree_item_activated.png, file_tree_navigated.png, file_tree_final.png
**Detail:** The blue selection highlight bar extends to the full width of the container, reaching the left and right edges. While functional, this causes the highlight to touch the container borders directly with no inset padding, which looks slightly unpolished compared to the text content which has left padding.
**Suggested fix:** Inset the selection highlight by a few pixels on the left and right so it doesn't touch the container borders, or ensure it matches the text's horizontal padding.


### flex_alignment: 2. "Gap" tab label near right edge of tab bar
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The tab bar at the top ("All", "Vertical", "Horizontal", "Cross-Axis", "Self Align", "Gap") has the rightmost "Gap" tab positioned at the very edge of the viewport. While the text is still readable, it has minimal right padding and sits flush against the screen boundary.
**Suggested fix:** Add right padding or margin to the tab bar container, or allow horizontal scrolling for the tab bar so edge tabs have proper spacing.


### hstack_showcase: 2. "Churn" card extends to screen edge with no right margin
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** In the bottom card row (Users, Revenue, Orders, Growth, Active, Churn), the "Churn" card at the far right extends very close to or touches the right edge of the viewport at approximately x=1230. Unlike the left side which has visible margin/padding, the right side has no breathing room, creating an asymmetric layout.
**Suggested fix:** Add right padding or margin to the card row container to match the left side spacing, or constrain the row width so all cards fit symmetrically within the viewport with equal margins on both sides.


### kirby_options: 1. "Edit" and "Erase" buttons positioned outside the main content container
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The "Edit" and "Erase" buttons on the left side (approximately x=65-110, y=175-255) are positioned outside the main white/cream content panel. They float in the margin area between the screen edge and the main container's left border. While this may be intentional as a sidebar control pattern, the buttons lack any container of their own and appear disconnected from the main layout hierarchy.
**Suggested fix:** If intentional, consider adding a subtle container or grouping visual to anchor these buttons. If unintentional, move them inside the main content panel boundary or into a dedicated sidebar container that aligns with the overall layout structure.


### meters_gauges: 3. Animated progress spinner positioned outside its section container
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** In the "Animated Progress" section, a small circular spinner/indicator element is positioned to the right of and below the progress bar. It sits in empty space rather than being aligned within the progress bar container or at a meaningful position relative to the bar's fill.
**Suggested fix:** Either anchor the spinner indicator to the end of the progress bar fill, or position it centered within the section container so it reads as intentionally placed.


### mini_motorways_settings: 3. Tab bar text sizes are inconsistent and cramped
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The tab bar ("Game Video Audio Language Controls Cross-Save Credits") has inconsistent text sizes. "Game" appears largest as the active tab, while "Language", "Controls", "Cross-Save" appear at notably smaller sizes. The "Cross-Save" label is especially small and cramped between adjacent tabs, creating an uneven layout.
**Suggested fix:** Normalize tab text sizes or ensure all inactive tabs use the same font size. Differentiate the active tab with an underline or background rather than dramatically different sizing that causes cramped layout.


### neon_strike: 4. Minimap and weapon icons extend to screen edges with no margin
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The minimap container in the bottom-left corner extends very close to the bottom and left screen edges. Similarly, the weapon inventory icons in the bottom-right corner touch or nearly touch the right screen boundary. Standard HUD practice calls for a safe margin from screen edges.
**Suggested fix:** Add margin/padding to both the minimap container and weapon icon row to maintain a consistent safe zone from all screen edges.


### pagination: 2. Pagination button rows not aligned to header bar width
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** Each section header bar (e.g., "Basic Pagination", "Difficulty Selector", "Option Selector") spans nearly the full width of the main container, but the button rows beneath them only extend about halfway across, leaving a large empty space to the right. This creates a visual imbalance between the full-width headers and the left-hugging button groups.
**Suggested fix:** If the buttons are intentionally left-aligned, this is acceptable. Otherwise, consider centering the button rows beneath their headers or giving the button group a container that visually connects it to the header width.


### potion_crafting: 2. Tab content does not update when "Inventory" tab is selected
**Type:** container_bounds
**Screenshot:** potion_crafting_tab_clicked.png
**Detail:** When the "Inventory" tab is visually highlighted/selected, the content below still shows the Recipes list, brewing area, and ingredients grid — identical to the Recipes tab view. The tab container's content region does not reflect the expected tab switch.
**Suggested fix:** Ensure each tab (Recipes, Inventory, Journal) maps to distinct content panels, or display a placeholder to indicate the tab was switched if content is not yet implemented.


### potion_crafting: 3. Ingredient grid columns have slight width imbalance
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** In the "Ingredients" panel on the right, the two-column grid has a minor width discrepancy — the right column (Fire Root x3, Honey Dew x8, Star Dust x1) appears to have slightly more horizontal space than the left column (Moonpetal x5, Sea Salt x12, Shadow Moss x2).
**Suggested fix:** Set both columns to equal width using a uniform grid or flex layout with equal column sizing.


### powerwash_settings: 3. Help panel positioned with large gap from settings panel
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The "Help: Anti-Aliasing" panel on the right side is positioned with a significant horizontal gap from the main settings panel on the left. The two panels don't appear visually connected, leaving a large empty area in the center-right of the screen. This makes the help text feel disconnected from the setting it describes.
**Suggested fix:** Move the help panel closer to the settings panel, or add a visual connector element (line, background fill) to make the relationship between the selected setting and its help text clearer.


### radio_buttons: 2. Red debug/wireframe borders visible on all three radio group containers
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** All three radio group containers ("Favorite Fruit", "Size", "Color") display thin red/pink/magenta border outlines that appear to be debug wireframe borders rather than intentional design. These borders are visible around each card container and create a double-border effect with the outer rounded container.
**Suggested fix:** Remove the debug border rendering. If these are intentional borders, they should match the design system's border color rather than using a bright red/pink diagnostic color.


### scroll_view: 2. Status text and scrollbar sit outside container borders
**Type:** container_bounds
**Screenshot:** idle_720p.png, scroll_view_before_scroll.png, scroll_view_after_scroll.png, scroll_view_scrolled_to_bottom.png, scroll_view_horiz_scrolled.png
**Detail:** The "Item X of 20 (X%)" label for the vertical scroll and the "Item 1 of 15 (0%)" label plus the horizontal scrollbar for the horizontal scroll are positioned below and outside their respective container borders. These elements appear orphaned from the containers they describe, floating in the gap between the container bottom edge and the page content area.
**Suggested fix:** Either move these status indicators inside the container (below the scroll viewport but within the border), or visually associate them by placing them in a dedicated footer area that connects to the container.


### setting_row_showcase: 1. Main container's left edge is clipped at screen boundary
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The main settings panel's left border and rounded corner are barely visible or cut off at the left edge of the viewport. The container appears to start at or very near x=0, so the left border and rounded corner are clipped or pushed against the screen edge, while the right side has generous padding. This asymmetry makes the left border appear missing.
**Suggested fix:** Add equal horizontal margin/padding on both sides of the main container so the left rounded corner and border are fully visible and match the right side's spacing.


### shop_interface: 1. Empty/blank row appears in item list when navigating
**Type:** container_bounds
**Screenshot:** shop_final.png, shop_navigated_item.png
**Detail:** When the selection cursor moves past certain items, an empty/blank row appears in the item list (visible between "Mana Potion" and "Fire Scroll"). This empty row takes up the same vertical space as a populated item row but contains no content, creating a visual gap in the list. The "Leather Armor" row disappears and is replaced by a blank highlighted area.
**Suggested fix:** Investigate why a blank row is rendered when the selected item scrolls or is navigated away. The list should not render empty rows; it should either hide the row or show the correct item data.


### sports_settings: 1. Settings rows extend to the far left screen edge with no left margin
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The settings rows (Window mode, Resolution, Screen percentage, etc.) start at the very left edge of the screen with minimal or no left margin. The row labels and their highlighted backgrounds begin at approximately x=0. Meanwhile, the right side has ample space occupied by the description panel. This creates an asymmetric layout where the left edge feels clipped.
**Suggested fix:** Add a left margin or padding to the settings list container so that row labels and backgrounds have breathing room from the screen edge.


### tab_container: 2. Tab bar left edge does not align with content panel left edge
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The tab bar begins at a slightly different horizontal position than the white content panel beneath it. The left edge of the "Profile" tab extends slightly further left than the content panel's left edge, creating a visible horizontal misalignment between the navigation strip and its associated content area.
**Suggested fix:** Align the tab bar's left and right edges with the content panel edges, or have both share the same parent container width for consistent alignment.


### tab_container: 3. Content panel has excessive unused vertical space
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The white content panel extends well below the last text entry ("Last login: Today at 10:30 AM"), leaving a large empty white area in the lower third of the panel. Additionally, there is significant gray empty space between the bottom of the content panel and the bottom of the viewport. The overall layout under-fills the 720p viewport.
**Suggested fix:** Either size the content panel to fit its content (auto-height) and vertically center the tab assembly, or stretch the content panel to fill more of the available viewport height.


### text: 2. Bottom content sits flush with container edge
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The last line of text ("Elegant typography creates visual hierarchy and guides the reader's eye") sits very close to the bottom edge of the rounded container. There is noticeably less bottom padding than top padding, and the text nearly touches the rounded corner curve at the bottom of the panel. The bottom content has inadequate clearance from the container boundary.
**Suggested fix:** Increase the bottom padding of the main container to match the top padding, ensuring the final line of text has adequate clearance from the rounded corner region.


### text_input: 2. Form content left-aligned leaving right half of screen empty
**Type:** container_bounds
**Screenshot:** idle_720p.png, focus_and_type.png, keyboard_input_test.png
**Detail:** All form elements (Username, Email, Password, Search fields, and Submit button) are clustered in the left ~40% of the main content area. The entire right half of the dark rounded container is empty, creating a strongly unbalanced layout with significant wasted space.
**Suggested fix:** Center the form elements horizontally within the container, or widen the form inputs to better utilize the available width.


### text_input: 3. Labels centered while inputs are left-aligned
**Type:** container_bounds
**Screenshot:** idle_720p.png, focus_and_type.png, keyboard_input_test.png
**Detail:** The field labels ("Username:", "Email:", "Password:", "Search (no label version):") are centered relative to the viewport, while the input fields and the "Submit" button are left-aligned within the content area. This creates a misalignment where labels are not directly above their corresponding input fields.
**Suggested fix:** Left-align the labels to match the input field positioning, or consistently center both labels and inputs within a form column.


### text_stroke: 2. Title has minimal top margin
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The main heading "Text Stroke / Outline" sits close to the top of the viewport with minimal top padding. While not clipped, the title's thick stroke effect has very little breathing room above it and could be cropped on displays with slight overscan.
**Suggested fix:** Add top padding or margin to the main container to give the title adequate spacing from the viewport edge.


### text_stroke: 3. Left and right content columns lack consistent vertical alignment
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The left column's first example ("NO STROKE / WITH STROKE") starts higher than the right column's first entry ("Outline Thickness: STROKE" at 2px). The two columns don't share a consistent top alignment or baseline grid. The left column also extends lower (WHITE comparison box + "invisible" / "visible!" labels) than the right column's last entry (10px - heavy).
**Suggested fix:** Align the starting vertical positions of both columns and ensure they share a consistent vertical rhythm or at least a common top edge.


### themes: 2. Color swatch labels truncated to single letters
**Type:** container_bounds
**Screenshot:** idle_720p.png, themes_test.png
**Detail:** In the "Component Preview" section, the top row of color swatches shows labels as single characters: "P", "S", "A", "D" — these likely represent "Primary", "Secondary", "Accent", and "Disabled" (or similar). The swatch containers are too narrow to display full label text, while other labels on the same screen (e.g., "Checkbox", "Toggle", "Hard", "Soft") are fully spelled out.
**Suggested fix:** Widen the swatch containers to fit full label text, or use a two-line layout with the label below each swatch.


### toasts: 2. Toast stack overflows below the visible screen area
**Type:** container_bounds
**Screenshot:** toasts_final.png, toasts_spam_triggered.png
**Detail:** When 5 toasts are stacked simultaneously (visible in `toasts_final.png`), the bottom-most toast ("[i] This sticks around for a while...") is positioned at the very bottom edge of the 720p viewport with its lower portion barely visible or clipped. There is no max-stack limit preventing toasts from pushing below the visible area.
**Suggested fix:** Implement a maximum visible toast count (e.g., 3–4) with older toasts being dismissed when the limit is reached, or adjust the toast stack origin to accommodate the maximum number of simultaneous toasts within the viewport.


### toggle_switches: 1. Bottom row clipped by viewport
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The "Unavailable Option (OFF)" row at the bottom of the "Disabled" section is partially cut off by the bottom edge of the screen. The lower portion of the row and its rounded corners are not fully visible, indicating the overall container height exceeds the 720p viewport.
**Suggested fix:** Either make the settings panel scrollable so all content is accessible, or reduce vertical spacing/padding between sections so all rows fit within the 720p viewport.


### vstack_showcase: 2. Top section label bars extend flush to screen edges
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The pink label bars ("Start", "Center", "End", "Between", "Around") in the top JustifyContent section extend flush to the screen edges on both sides. The "Around" label reaches the right edge and the layout leaves no outer margin, making the content appear to touch or nearly touch the viewport boundary.
**Suggested fix:** Add horizontal margin or padding to the outermost container so the label bars do not touch the screen edges, ensuring consistent outer spacing.


## Overlap Issues (30)

### advanced_modals: 2. Dropdown menu items overlap with controls behind modal
**Type:** overlap
**Screenshot:** settings_dropdown_open.png
**Detail:** When the Language dropdown is open in the Settings modal, the dropdown option list extends downward and overlaps with the Dark Mode and Notifications toggle switches. The toggle elements are partially visible through/beside the dropdown items on the right side. The bottom "German" option row shows "ly" text fragments from the "Apply" button bleeding through on the right, indicating incomplete z-order coverage.
**Suggested fix:** Ensure the dropdown menu panel has a solid, opaque background and sufficient z-index to fully occlude all modal content behind it. The dropdown items should completely cover any underlying form controls.


### aim_chat: 2. Small artifact element at left edge of "Send to" bar
**Type:** overlap
**Screenshot:** idle_720p.png, text_area_arrow_keys.png
**Detail:** At the left edge of the "Send to: SmarterChild" bar, there is a small rectangular element (appears to be a leftover padding/border artifact) that slightly protrudes or overlaps with the bar's left boundary. This is consistent across both screenshots.
**Suggested fix:** Review the left-side element positioning in the "Send to" row and remove or properly align any stray border/padding elements.


### casual_settings: 1. Close button (X) overlaps container top-right corner
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** The red circular close button ("X") in the top-right of the settings panel straddles the container's rounded corner border. It sits partially inside and partially outside the beige panel boundary, breaking the clean rounded corner edge.
**Suggested fix:** Move the close button fully inside the container with adequate margin from the top and right edges, or position it as an intentional external element with a consistent offset from the corner.


### circular_progress: 2. Floating "88%" label and truncated text overlap between sections
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** Between the top row container and the "Size Variations" section, a "88%" label and truncated text (appears to read "in" — likely truncation of "Thin") float in the gap between the two rows. These elements don't clearly belong to either container section and overlap the boundary area.
**Suggested fix:** Ensure percentage labels and variant names are positioned within their respective container panels rather than floating in the inter-container gap.


### cozy_cafe: 1. Customer row text overlaps with patience/progress indicators
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** In the "Customers" panel, both customer rows show text collisions. "Emma - Cappuccio" has the "Patience:" label running directly into the 50% progress badge. "Oliver - Croissant" has "Patience: LO" (likely "LOW") truncated and overlapping with the 0% progress badge. The name text, patience label, and progress bar compete for the same horizontal space.
**Suggested fix:** Place the patience label on its own line below the customer name, or enforce minimum spacing between name text, patience label, and progress badge. Consider a multi-line row layout if horizontal space is insufficient.


### cozy_cafe: 4. Notification badges overflow icon containers
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** The notification badges ("2" on Inventory, "1" on Research) overlap the top-right corners of their respective icon containers, extending beyond the icon boundary. While this is a common UI pattern, the badges protrude past the container bounds.
**Suggested fix:** This may be intentional by design convention. If unintended, adjust badge positioning to sit just inside the icon container boundary.


### decorators: 4. Stray rendering artifact inside with_grid_bg() top panel
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** The top panel in the with_grid_bg() column contains a small misplaced colored square/dot near the center of the panel. This appears to be a stray rendering artifact or unintended child element overlapping the otherwise empty grid background.
**Suggested fix:** Investigate whether this is an unintended child element or rendering artifact and remove it if it is not part of the design.


### empire_tycoon: 1. Left sidebar icons overlap with navigation labels
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** In the left sidebar, the category icons (Rides, Food, Upgrades, Finance) have icon artwork that visually overlaps or bleeds into adjacent rows. Specifically, the "Food" row icon overlaps downward into the "Upgrades" area, and the icons in general appear to stack tightly with insufficient spacing between them. The "Rides" icon above "Food" also shows a similar tight overlap.
**Suggested fix:** Increase vertical padding or margin between sidebar navigation items to give each icon adequate space, or reduce the icon size so they fit cleanly within their row bounds.


### empire_tycoon: 2. Notification badge overlaps sidebar item boundary
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** The red notification badge (exclamation mark) on the "Upgrades" sidebar item extends beyond the top-right corner of the sidebar card boundary, partially overlapping the area outside the card.
**Suggested fix:** This may be intentional badge behavior, but if badges should stay within the card boundary, adjust the badge position inward or add overflow-visible only for the badge element.


### layout_bug_repros: 3. "Controller Cu..." label text truncated and overlapping stepper controls
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** In the controller customization area, the label "Controller Cu..." is clipped/truncated, and fragments of text ("ntiv") appear between the "<" stepper button and the "Default" value text, creating a visual collision where label text, stepper arrows, and value text overlap in a crowded region.
**Suggested fix:** Widen the label container or allow it to ellipsize cleanly so the full controller label text is legible and does not collide with the adjacent stepper controls.


### layout_bug_repros: 4. Z-order overlap between RED and BLUE elements (Bug Repro #4)
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** In the "[NOT A BUG] 4: Z-order" section at the lower-left, the blue element ("BLUE mk(2000) ON TOP?") overlaps the red element ("RED"). The annotation states: "Expected: RED(5000) on top | Bug: BLUE(2000) on top (entity ID order, not mk ID)." The z-order is determined by entity creation order rather than the expected marker ID.
**Suggested fix:** This is annotated as "[NOT A BUG]." If the intended behavior is for higher mk IDs to render on top, update the render sorting to use mk ID instead of entity ID.

*Note: Issues #1, #2, and #4 are marked [NOT A BUG] in the annotations, indicating known behavioral characteristics rather than regressions, but they still represent visible layout anomalies.*


### mini_motorways_settings: 1. Controller Customization row elements overlapping
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** In the "Controller Cu..." row near the bottom of the settings panel, the label text is truncated ("Controller Cu...") and visually collides with the "<" stepper button. Partially visible text fragments ("ntiv") appear between the "<" button and the "Default" value text, creating a cluttered overlapping region where label, stepper arrows, and value text all crowd together.
**Suggested fix:** Ensure the controller customization row has sufficient width for the full label text, or properly truncate with ellipsis before it reaches the stepper arrows. Enforce non-overlapping spacing between the label, left arrow, value text, and right arrow.


### mini_motorways_settings: 2. "Colorblind Mode OFF" text crowds into the toggle switch
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** The "Colorblind Mode" row shows the "OFF" status text sitting immediately adjacent to the toggle switch with no visible gap. The text appears to touch or overlap the toggle control's left edge, unlike other toggle rows which have clearer separation.
**Suggested fix:** Add horizontal spacing (margin or padding) between the "OFF" status label and the toggle switch to maintain consistent spacing with other setting rows.


### neon_strike: 1. Ability icons overlap each other vertically in left sidebar
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** The left-side ability panel contains four ability icons stacked vertically (UAV, Recon, Shield, Strike). The icons and their labels are spaced tightly enough that adjacent items overlap — the "Recon" label crowds into the bottom of the "UAV" container, and "Shield" overlaps with "Recon". The asterisk markers to the left of each ability also appear crowded together.
**Suggested fix:** Increase the vertical spacing (gap or margin) between ability icon containers in the left sidebar so each icon and its label have clear separation from neighboring items.


### neon_strike: 2. Kill feed container sits too close to compass widget
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** The kill feed notification area (showing elimination messages) is positioned directly below the compass (N/S/E/W) widget at the top center. The top edge of the kill feed container sits very close to the compass "S" label, creating tight coupling between two distinct HUD elements with insufficient separation.
**Suggested fix:** Add vertical margin between the compass widget and the kill feed container to create clear visual separation between these independent HUD elements.


### neon_strike: 3. Ammo counter text overlaps with health bar area
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** The ammo counter "35/210" text at the bottom-center of the screen crowds into the health bar container ("80 HEALTH" with the blue bar). The large ammo text extends into the left side of the health bar region, creating a visual collision between these two HUD elements.
**Suggested fix:** Add horizontal margin or padding between the ammo counter and the health bar container, or reposition the ammo counter further to the left so it doesn't intrude into the health bar's bounding area.


### nine_slice_borders: 1. "Double Width" and "Double Ornate" border panels overlap each other
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** In the "Double-Width & Tinting" row, the "Double Width" panel and the "Double Ornate" panel are positioned so close together that their decorative 9-slice borders overlap. The ornate corner decorations of the right edge of "Double Width" visually merge with the left edge decorations of "Double Ornate," creating a messy artifact where the two border patterns collide. Both labels are also truncated ("Double Widt" and "Double Ornat").
**Suggested fix:** Add horizontal margin between the "Double Width" and "Double Ornate" panels so their 9-slice border decorations have clear separation. Widen panels slightly to accommodate full label text.


### nine_slice_borders: 3. "Interactive Elements" section header overlaps with Double-Width row
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** The section header "Interactive Elements" (gold text near bottom-left) overlaps with the bottom border decorations of the "Double-Width & Tinting" row above it. The header text sits very close to or on top of the lower border ornaments of the panels in the row above.
**Suggested fix:** Add top margin to section headers or bottom margin to content rows to create clear separation between sections and their preceding content.


### parcel_corps_settings: 3. Slider handle extends past track boundary
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** The volume slider handles (Audio Volume, Music Volume) extend slightly beyond the right edge of their slider track containers. The white square handle sits at a position where it visually clips past the track border.
**Suggested fix:** Ensure the slider handle is constrained within the track bounds, or give the track sufficient padding so the handle doesn't visually overflow at any position.


### powerwash_settings: 1. "CONTROLS" label overlaps with "Close" button text
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** At the bottom-left of the screen, the category tab label "CONTROLS" and the "X Close" action hint text overlap each other. The word "CONTROLS" is partially rendered on top of or directly adjacent to the "Close" label, making both difficult to read. The two text elements occupy the same vertical and horizontal space.
**Suggested fix:** Separate the tab bar row from the action-hint row with distinct vertical positioning. Place the category tabs (GENERAL, GAMEPLAY, VIDEO, AUDIO, CONTROLS) on one line and the action hints (X Close, O Reset, [] Select) on a separate line below with adequate spacing.


### radio_buttons: 3. Radio button filled indicators render as partial shapes
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** The filled radio button indicators for the selected options (Orange in Fruit, Small in Size, Blue in Color) appear as half-circle or partial-fill shapes rather than complete filled circles. This suggests the filled indicator is either being clipped by its container or rendered with incorrect geometry. The unfilled radio indicators also appear as oblong/capsule shapes rather than circles.
**Suggested fix:** Ensure radio button indicators render as complete circles. Verify the filled state uses a full circular fill rather than a partial or clipped shape. Check that the indicator container dimensions are square and not stretched.


### rubber_bandits_menu: 3. Notification badge overlaps with user status pill
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** In the top-right corner, the yellow notification circle with "1" and the star badge sit very close to the edge of the "thia9uers - Online" status pill. The notification badge's position could overlap the status pill on slightly different viewport sizes.
**Suggested fix:** Ensure a minimum gap between the notification badge and the status pill, or anchor them with explicit spacing constraints.


### stepper_showcase: 3. Resolution stepper left arrow has inconsistent focus rectangle
**Type:** overlap
**Screenshot:** idle_720p.png, stepper_initial.png, stepper_card_back_to_rogue.png, stepper_card_healer.png, stepper_card_mage.png, stepper_card_rogue.png, stepper_final.png, stepper_resolution_advanced.png, stepper_resolution_advanced_twice.png, stepper_resolution_back.png
**Detail:** The left arrow `<` for the Resolution stepper has a visible rectangular focus/selection outline drawn around it. This small box around the `<` character is inconsistent with the other stepper rows (Quality, Language, Difficulty, Framerate) where no such outline appears on the left arrows.
**Suggested fix:** Remove the focus rectangle on the Resolution stepper's left arrow to match the visual appearance of the other stepper rows, or apply the focus style consistently across all stepper rows when focused.


### text_shadow: 2. "COLORED" text shadow bleeds into description text below
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** The "COLORED" text (cyan with red shadow) has a large shadow offset that extends downward, overlapping with the description text "Cyan text with red shadow for retro 3D look" beneath it. The shadow creates a visual collision between the decorative text and its label.
**Suggested fix:** Increase the vertical spacing between the "COLORED" example and its description text to account for the shadow offset, preventing the shadow from overlapping adjacent content.


### text_shadow: 3. "COMBO" text shadow overlaps with description below
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** The "COMBO" text has a purple shadow that extends downward and overlaps with the description "Combine stroke and shadow for maximum impact." The shadow bleeds into the text line below it.
**Suggested fix:** Increase vertical spacing below the "COMBO" text element to provide clearance for the shadow effect.


### text_shadow: 4. Shadow Offset Examples panel — large shadows overlap adjacent rows
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** In the "Shadow Offset Examples" panel on the right side, the larger offset examples (6px, 8px, 10px) have shadows that visually bleed into the rows below them. The 10px "SHADOW" at the bottom has its shadow extending past the panel's bottom edge.
**Suggested fix:** Increase row height in the shadow offset examples panel to accommodate larger shadow offsets, or clip shadows to prevent bleed between rows.


### toasts: 3. Buttons in "Duration & Spam" row have no spacing between them
**Type:** overlap
**Screenshot:** idle_720p.png (visible in all screenshots)
**Detail:** In the "DURATION & SPAM" section, the first three buttons ("Quick (displays 1s)", "Long (displays 10s)", and "Spam x5 (!)") have zero gap between them — each button's right edge directly abuts the next button's left edge. Meanwhile "Custom Color" has a visible gap before it. This inconsistent spacing makes the first three buttons appear merged into a single element, unlike the "SIMPLE TOASTS" row where all buttons have clear gaps.
**Suggested fix:** Add consistent horizontal gap/margin between all buttons in the Duration & Spam row to match the spacing used in the Simple Toasts row.


### toasts: 4. Toast banners overlap Interactive Toasts container boundary
**Type:** overlap
**Screenshot:** toasts_info_triggered.png, toasts_spam_triggered.png, toasts_error_triggered.png
**Detail:** In several triggered states, the floating toast stack in the bottom-right visually overlaps the right edge of the "INTERACTIVE TOASTS" container. For example, in `toasts_info_triggered.png`, the toast banners' left edges overlap the container's right border, obscuring its rounded corner. This is likely intentional (toasts float above content as an overlay), but it partially hides the container boundary.
**Suggested fix:** If toasts are intended as floating overlays, this is acceptable behavior. If not, offset the toast stack further right or ensure it does not encroach on content containers.


### toggle_switches: 2. Divider lines clip through Preferences row containers
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** In the "Preferences" section, thin vertical divider lines appear to run through the middle of each row ("Dark Mode", "Auto-Save", "Cloud Sync"), extending beyond the row bounds. These lines overlap the row content area and break the visual containment of each row, cutting through the horizontal row backgrounds.
**Suggested fix:** Remove the mid-row dividers or constrain them to sit between rows rather than overlapping row content. If they are meant to separate the label from the checkmark, align them as internal cell dividers within the row bounds.


### vstack_showcase: 3. "AlignItems" and "Sidebar + Content" header bars are vertically misaligned
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** The green "AlignItems (cross-axis)" header bar and the blue "Sidebar + Content" header bar in the second row sit at slightly different vertical positions. The "Sidebar + Content" bar appears to sit higher, creating a visually misaligned row. The sub-labels ("Start", "Center", "Stretch", "End") under AlignItems also crowd the top edge of their respective card containers.
**Suggested fix:** Ensure both section header bars share the same vertical baseline. Add a small gap between the sub-labels and the top edge of the card containers below them.


## Corners Issues (11)

### animation_basic: 1. Header bar extends beyond container's rounded corners
**Type:** corners
**Screenshot:** idle_720p.png
**Detail:** The dark horizontal header bar containing "Animation Basics" and "Fade, Slide, and Scale animations" extends to the full width of the outer container, but the outer container has very large rounded corners. The header bar's sharp left and right edges visually poke past the rounded corner boundary of the container on both sides, creating a visible mismatch where the bar's straight edges extend into the curved area.
**Suggested fix:** Either clip the header bar to respect the parent container's border-radius, reduce the header bar width to stay within the inner curve, or apply matching border-radius to the header bar's corners.


### deadspace_settings: 2. "Resume Game" focus border clips at top edge
**Type:** corners
**Screenshot:** idle_720p.png
**Detail:** The "Resume Game" item at the top of the pause menu sidebar has a dashed focus/selection border whose top edge clips against the top boundary of the menu list container. The top border line sits at or above the container's start position, unlike the highlighted "Menu Narration" item below which has clean bounds.
**Suggested fix:** Add top padding to the pause menu list container so the focus border of the first item is fully visible without clipping.


### decorative_frame: 1. KraftPaper frame corner decorations extend outside frame bounds
**Type:** corners
**Screenshot:** idle_720p.png
**Detail:** The KraftPaper decorative frame (leftmost) has small dashed L-shaped corner bracket decorations at the top-left and top-right that extend beyond the rounded rectangle of the main frame. The dark brown layered corner elements also bleed past the frame's border radius area, protruding outside the frame boundary.
**Suggested fix:** If the corner brackets are intentional decorative flourishes, ensure they don't clip against neighboring elements. If they should be contained, clip the corner decorations to the frame bounds or position them just inside the rounded corners.


### decorative_frame: 2. Inset frame has corner mismatch between dashed border and shadow rectangle
**Type:** corners
**Screenshot:** idle_720p.png
**Detail:** The Inset decorative frame (rightmost) has a dashed outer border that extends slightly beyond the gray shadow/inset area behind it. The dashed border at the top-left and top-right corners peeks past the background shadow rectangle, creating a mismatch. The outer dashed border uses a different corner radius than the inner rounded rectangle.
**Suggested fix:** Align the dashed border outline to match the shadow rectangle bounds, or use consistent corner radius for both the outer dashed border and inner rounded shape.


### decorators: 2. Corner bracket decorations extend outside panel boundaries
**Type:** corners
**Screenshot:** idle_720p.png
**Detail:** In the with_brackets() column (left), the L-shaped corner bracket decorations on each panel extend outside the panel's border edges. The colored horizontal bar accents (cyan/teal) at the top and bottom of each panel also protrude past the panel's side boundaries. While this appears intentional as a decorative style, the brackets overflow the panel's actual bounding box.
**Suggested fix:** If the bracket extensions are by design (decorative flourishes), ensure they don't clip against neighboring elements or the viewport edge. If they should be contained, clip the brackets and accent bars to the panel's bounding box.


### pagination: 1. Left arrow button has inconsistent styling/border compared to page buttons
**Type:** corners
**Screenshot:** idle_720p.png
**Detail:** In the "Basic Pagination" row, the left arrow (`<`) button has a distinct light gray/white border and different background styling compared to all other pagination buttons (Page 1-5 and the right arrow). The other two pagination groups (Difficulty Selector, Option Selector) have their left arrow buttons styled consistently with the teal fill, but the first group's left arrow appears with a lighter, outlined style. This looks like a focus ring or default-state rendering issue where the button's container corners and fill do not match its siblings.
**Suggested fix:** Ensure the left arrow button in the Basic Pagination row uses the same container style (background fill, border radius, border color) as the right arrow button and page buttons. If this is a focus indicator, the focus ring should overlay consistently without changing the button's base appearance.


### powerwash_settings: 2. Settings panel left border clips the label column
**Type:** corners
**Screenshot:** idle_720p.png
**Detail:** The outer rounded-corner border of the settings panel passes very close to the left edge of the first label column. The "Screen Resolution" text appears to touch or nearly touch the left border of the container, with no visible inner padding on the left side.
**Suggested fix:** Add left padding inside the settings panel container so that the first column of labels has breathing room from the rounded border.


### scroll_view: 3. Horizontal scroll item "5" bleeds past container's right rounded corner
**Type:** corners
**Screenshot:** idle_720p.png, scroll_view_before_scroll.png, scroll_view_after_scroll.png, scroll_view_scrolled_to_bottom.png
**Detail:** In the Horizontal Scroll section, the rightmost visible item (labeled "5") extends to the very edge of the rounded-corner container. The square corners of the item tile visually bleed past the container's border radius on the right side, breaking the rounded-corner appearance.
**Suggested fix:** Apply overflow clipping to the scroll container so child items are masked by the rounded corners, or add inner padding so items don't touch the container edge.


### tab_container: 1. Tab bar top corners are sharp while content panel has rounded corners
**Type:** corners
**Screenshot:** idle_720p.png
**Detail:** The tab bar strip across the top has sharp rectangular corners on the "Profile" tab (top-left) and "Settings" tab (top-right), while the white content panel below uses large rounded corners. This creates a visual disconnect — the tab bar and content panel appear as two separate elements rather than a unified tab container. The sharp top-left corner of the first tab is particularly noticeable against the rounded content below.
**Suggested fix:** Apply matching border-radius to the top-left corner of the first tab and the top-right corner of the last tab so the tab bar and content panel form a cohesive rounded shape.


### text_shadow: 5. Tip bar at the bottom is flush with viewport edges
**Type:** corners
**Screenshot:** idle_720p.png
**Detail:** The light-colored tip bar ("Tip: Use small offsets (1-4px) for subtle depth, larger offsets (6-10px) for bold effects") extends to the left and right edges of the viewport with no margin, and its bottom edge is flush with the viewport bottom. The bar appears to lack rounded corners and may be partially cut off at the bottom.
**Suggested fix:** Add margin around the tip bar so it doesn't sit flush against the viewport edges, or ensure it has proper bottom padding and rounded corners if it is intended to be a contained element.


### themes: 3. Outer container corners clip near header bar
**Type:** corners
**Screenshot:** idle_720p.png, themes_test.png
**Detail:** The outermost container has large rounded corners. The "Theme Switcher" label and "Active: Ocean Navy" badge at the top-left are positioned close to the top-left rounded corner, and the bottom corners of the outer container are close to the viewport edges, leaving minimal clearance. The large border-radius reduces usable space at all four corners of the container.
**Suggested fix:** Add inner padding so content elements sit below and away from the rounded corner curves, or reduce the outer container's border-radius to a more moderate value to reclaim corner space.

