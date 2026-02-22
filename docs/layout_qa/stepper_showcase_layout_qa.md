# Layout QA: stepper_showcase

**Screenshots analyzed:** 10

## Issues Found

### 1. Card description text overflows right edge of the viewport
**Type:** overflow
**Screenshot:** idle_720p.png, stepper_initial.png, stepper_card_back_to_rogue.png, stepper_card_healer.png, stepper_card_mage.png, stepper_card_rogue.png, stepper_final.png, stepper_resolution_advanced.png, stepper_resolution_advanced_twice.png, stepper_resolution_back.png
**Detail:** In the Card Selector panel, the Warrior card's description text "Heavy armor, melee attacks, high defense." extends past the right edge of the card and panel. The text ", high defense." portion overflows beyond the card's right boundary, rendering outside the card's visible area. This is consistent across all 10 screenshots since the card is always visible.
**Suggested fix:** Constrain the card's text content so it wraps within the card bounds. Either reduce the card text font size, enable text wrapping, or increase the card width so the full description fits within the visible area.

### 2. "HealerWarriorMage" stepper text lacks spacing between options
**Type:** overflow
**Screenshot:** idle_720p.png, stepper_initial.png, stepper_card_back_to_rogue.png, stepper_card_healer.png, stepper_card_mage.png, stepper_card_rogue.png, stepper_final.png, stepper_resolution_advanced.png, stepper_resolution_advanced_twice.png, stepper_resolution_back.png
**Detail:** The Card Selector stepper displays "HealerWarriorMage" as a single concatenated string between the `<` and `>` arrows. The individual option names (Healer, Warrior, Mage) run together without any separator, spacing, or visual differentiation, making it unclear which option is currently selected.
**Suggested fix:** Display only the currently selected option name (e.g., "Warrior") in the stepper rather than concatenating all available options. Alternatively, add separators or spacing between the names.

### 3. Resolution stepper left arrow has inconsistent focus rectangle
**Type:** overlap
**Screenshot:** idle_720p.png, stepper_initial.png, stepper_card_back_to_rogue.png, stepper_card_healer.png, stepper_card_mage.png, stepper_card_rogue.png, stepper_final.png, stepper_resolution_advanced.png, stepper_resolution_advanced_twice.png, stepper_resolution_back.png
**Detail:** The left arrow `<` for the Resolution stepper has a visible rectangular focus/selection outline drawn around it. This small box around the `<` character is inconsistent with the other stepper rows (Quality, Language, Difficulty, Framerate) where no such outline appears on the left arrows.
**Suggested fix:** Remove the focus rectangle on the Resolution stepper's left arrow to match the visual appearance of the other stepper rows, or apply the focus style consistently across all stepper rows when focused.
