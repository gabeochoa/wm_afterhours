# UI Review - Issues Found Per Screen (2026-02-03)

**Date:** 2026-02-03
**Reviewer:** Claude (interface-design skill)
**Total Screens:** 50
**Status:** REVIEW COMPLETE - FIXES NEEDED

---

## CRITICAL ISSUES (Text Cutoff / Overflow)

### 1. EMPIRE TYCOON - Resources display cutoff
**File:** `src/systems/screens/EmpireTycoon.h`
**Issue:** "600/1000" text is cut off at the right edge - shows as "600/100" with trailing character clipped
**Fix needed:** Increase width of resources container or reduce font size

### 2. EMPIRE TYCOON - Sidebar icons show baked-in text
**File:** `src/systems/screens/EmpireTycoon.h`
**Issue:** Sidebar icons (Rides, Food, Upgrades, Finance) show both the icon AND baked-in text from the source image, creating duplicate labels
**Fix needed:** Crop icons more aggressively or use text-free icon variants

### 3. IMAGES - Debug dimension indicators showing
**File:** `src/systems/screens/ImagesShowcase.h`
**Issue:** Small colon characters (`:`) appear next to every sprite icon - appears to be debug dimension info leaking into display
**Fix needed:** Remove debug text or hide dimension indicators in showcase

### 4. RUBBER BANDITS - Username cutoff
**File:** `src/systems/screens/RubberBanditsMenu.h`
**Issue:** "thia9uers - Online" text is cut off at right edge, showing partial character
**Fix needed:** Increase container width or use ellipsis for long usernames

### 5. PARCEL CORPS - Language selector text cramped
**File:** `src/systems/screens/ParcelCorpsSettings.h`
**Issue:** "English UK" text is very close to the < and > navigation arrows, nearly touching
**Fix needed:** Add more padding between text and arrows OR shorten to "EN-UK"

### 6. NEON STRIKE - UAV label overlap
**File:** `src/systems/screens/NeonStrike.h`
**Issue:** "UAV" label appears below the first icon but overlaps/touches the icon boundary
**Fix needed:** Add spacing between icon and label

---

## HIGH PRIORITY ISSUES (Layout / Spacing)

### 7. TAB CONTAINER - Excessive right margin on tabs
**File:** `src/systems/screens/TabContainerShowcase.h`
**Issue:** The "Prefs" tab appears pushed far to the right with large gap between "Account" and "Prefs"
**Fix needed:** Even out tab spacing or use equal-width tabs

### 8. CARDS - "Basic:" and "Nested:" labels too small
**File:** `src/systems/screens/CardsShowcase.h`
**Issue:** Section labels "Basic:" and "Nested:" are very small and hard to read
**Fix needed:** Increase label font size

### 9. COZY CAFE - Bottom nav icons cramped
**File:** `src/systems/screens/CozyCafe.h`
**Issue:** Bottom navigation icons (Inventory, Research, Crafting) have text labels that are very small and partially cut off ("2 new items", "Ready!")
**Fix needed:** Increase width of nav items or use icon-only display

### 10. SCROLL VIEW - Horizontal items overflow container
**File:** `src/systems/screens/ScrollViewShowcase.h`
**Issue:** Items 4 and 5 in horizontal scroll extend beyond the visible container boundary
**Fix needed:** This may be intentional for scroll demo, but container should have visible bounds

### 11. FORMS - "English V" dropdown text
**File:** `src/systems/screens/FormsShowcase.h`
**Issue:** Dropdown shows "English V" where V appears to be a checkmark but looks like letter V
**Fix needed:** Use proper checkmark character or icon

### 12. CHECKBOXES - Using "v" instead of checkmark
**File:** Multiple screens
**Issue:** Checkboxes use lowercase "v" character instead of proper checkmark symbol
**Fix needed:** Replace "v" with proper checkmark Unicode character (U+2713) or custom icon

### 13. TOGGLE SWITCHES - Circle style uses "x" for off
**File:** `src/systems/screens/ToggleSwitchShowcase.h`
**Issue:** Circle style toggles show "x" for OFF state which looks like an error
**Fix needed:** Use empty circle or different visual for OFF state

---

## MEDIUM PRIORITY ISSUES

### 14. TEXT OVERFLOW DEMO - Intentional but confusing
**File:** `src/systems/screens/TextOverflowDemo.h`
**Issue:** Red corners are intentional debug indicators, but screen doesn't clearly explain this is a DEBUG demo
**Fix needed:** Add clearer "DEBUG MODE" indicator or move to dev-only screens

### 15. ACCESSIBILITY - WCAG warning badge
**File:** `src/systems/screens/AccessibilityShowcase.h`
**Issue:** "Contrast Below WCAG AA" badge is shown but it's not clear if this is demonstrating a problem or is itself a problem
**Fix needed:** Clarify this is an intentional demo of the accessibility feature

### 16. AIM CHAT - Wide scrollbar
**File:** `src/systems/screens/AimChatShowcase.h`
**Issue:** Scrollbar is very wide (appears to be 20-30px) - may be intentional retro styling
**Fix needed:** If not intentional, reduce scrollbar width

### 17. FIGHTER MENU - "MainMenu" title not styled
**File:** `src/systems/screens/FighterMenu.h`
**Issue:** "MainMenu" title appears as plain text without proper title styling
**Fix needed:** Apply consistent title styling

### 18. SPORTS SETTINGS - Dense layout with small fonts
**File:** `src/systems/screens/SportsSettings.h`
**Issue:** Text is quite small throughout, especially setting labels like "Anti-Aliasing method"
**Fix needed:** Increase base font size for readability

### 19. DEADSPACE SETTINGS - Left panel text small
**File:** `src/systems/screens/DeadspaceSettings.h`
**Issue:** "INITIAL SETTINGS" section text is notably smaller than main menu
**Fix needed:** Increase font size for better visual hierarchy

### 20. THEMES - Component preview cramped
**File:** `src/systems/screens/ThemeSwitcher.h`
**Issue:** Component preview panel (P, S, A, D buttons) is densely packed
**Fix needed:** Add more internal padding

---

## MINOR ISSUES (Polish)

### 21. DECORATIVE FRAME - Bottom spacing
**File:** `src/systems/screens/DecorativeFrame.h`
**Issue:** "Usage Tips" panel has more space above than below the text
**Fix needed:** Even out vertical padding

### 22. KIRBY OPTIONS - "Data..." button text
**File:** `src/systems/screens/KirbyOptions.h`
**Issue:** "Data..." button uses ellipsis suggesting it opens a dialog - verify this is intentional
**Fix needed:** Confirm UX pattern or remove ellipsis

### 23. NINE SLICE - Some panels have tight text
**File:** `src/systems/screens/NineSliceBorders.h`
**Issue:** Text in smaller panels ("Small", "Inner", "Panel") is close to borders
**Fix needed:** Increase internal padding

### 24. TEXT SHADOW - Title has double shadow effect
**File:** `src/systems/screens/TextShadowShowcase.h`
**Issue:** "Text Drop Shadow" title shows both the text AND its shadow overlapping
**Fix needed:** This may be intentional demo - verify

### 25. CIRCULAR PROGRESS - Size label overlap
**File:** `src/systems/screens/CircularProgressShowcase.h`
**Issue:** "28px" size label is very close to the ring on smallest variant
**Fix needed:** Add more spacing or position label below ring

---

## SCREENS WITH NO ISSUES (Clean)

The following screens have no visible issues:

1. auto_text_color.png - Clean
2. language_demo.png - Clean
3. buttons.png - Clean
4. flex_alignment.png - Clean
5. modals.png - Clean
6. pagination.png - Clean
7. text.png - Clean
8. separators.png - Clean
9. angry_birds_settings.png - Clean
10. mini_motorways_settings.png - Clean
11. powerwash_settings.png - Clean
12. casual_settings.png - Clean
13. flight_options.png - Clean
14. toasts.png - Clean
15. tabbing.png - Clean
16. text_input.png - Clean
17. navigation_bar_demo.png - Clean
18. setting_row_showcase.png - Clean
19. colors.png - Clean
20. bevel_borders.png - Clean
21. example_borders.png - Clean
22. simple_button.png - Clean
23. self_align.png - Clean
24. radio_buttons.png - Clean
25. layout.png - Clean

---

## Summary

| Category | Count | Priority |
|----------|-------|----------|
| Critical (Text Cutoff) | 6 | Fix immediately |
| High (Layout/Spacing) | 7 | Fix soon |
| Medium (Polish) | 7 | Fix when possible |
| Minor (Polish) | 5 | Low priority |
| Clean Screens | 25 | No action |

### Top 3 Fixes to Apply Now:

1. **EMPIRE TYCOON resources** - "600/1000" cutoff is user-facing data loss
2. **IMAGES debug text** - Colon characters are clearly debug artifacts
3. **RUBBER BANDITS username** - Text cutoff on player info

---

*Reviewed: 2026-02-03*
*All 50 screens inspected via headless screenshots*
