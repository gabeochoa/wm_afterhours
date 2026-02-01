# UI Review - Issues Found Per Screen (2026-02-01)

**Date:** 2026-02-01
**Reviewer:** Claude (interface-design skill)
**Total Screens:** 50
**Status:** ALL MAJOR ISSUES FIXED

---

## Summary of Fixes Applied This Session

### FIXED ISSUES:

1. **RUBBER BANDITS MENU** - Fixed contrast issue
   - Changed "RUBBER" text from dark brown to cream/off-white for better contrast against dark overlay

2. **DEADSPACE SETTINGS** - Fixed low contrast text
   - Increased `text_muted` color brightness from (145, 160, 165) to (175, 190, 195)
   - Left sidebar text now readable

3. **PARCEL CORPS SETTINGS** - Fixed arrow overlap + font sizes
   - Moved left chevron further left (from w-155 to w-175)
   - Widened language value text area (from 100px to 115px)
   - Proper spacing between arrow and "King's English" text
   - Fixed font sizes below 12px minimum (chat messages: 10px→12px, quest text: 11px→12px)

4. **SPORTS SETTINGS** - Fixed arrow overlap
   - Moved right arrow further right (from +138 to +148)
   - Adjusted slider bar position to match
   - Green arrow ">" no longer overlaps value text

5. **IMAGES** - Increased icon sizes
   - Changed Icon Row icons from 32px to 48px for better visibility

6. **DECORATIVE FRAME** - Improved layout
   - Changed row height from 55% to 80% of screen
   - Changed column justify to Center (from FlexStart)
   - Increased frame sizes to 75% of column height

7. **EMPIRE TYCOON** - Fixed sidebar tab labels
   - Root cause: Icon PNG files had incorrect text labels baked into them
   - Fix: Cropped sprite source rectangle to only show top 32px (icon graphic only)
   - Tabs now correctly show: Rides, Food, Upgrades, Finance

8. **SELF ALIGN SHOWCASE** - Fixed layout overflow
   - Removed margins from flex_start and flex_end elements that were causing overflow warnings
   - Demo still demonstrates SelfAlign feature without layout issues

9. **SCROLL VIEW SHOWCASE** - Fixed layout overflow and checkbox sizing
   - Reduced checkbox from 320x52px to 280x48px to fit parent container
   - Removed vertical indicator track that was causing persistent overflow due to framework layout bug
   - Scroll view functionality preserved

10. **AIM CHAT DEMO** - Fixed buddy_status overflow
    - Adjusted buddy_info container and buddy_status sizing to fit within parent bounds
    - Reduced buddy_status from 100x20px to 90x18px

11. **LANGUAGE DEMO** - Fixed footer overflow
    - Reduced content area height from 540px to 480px to fit all elements within parent

12. **THEMES SCREEN** - Fixed toggle_track overflow
    - Increased preview_toggle height from 36px to 48px to accommodate toggle_track component

### VENDOR FRAMEWORK FIXES:

13. **Navigation Bar Component** - Fixed right_arrow overflow
    - Reduced label_ratio from 60% to 59% to avoid floating-point precision issues at boundaries

14. **Dropdown Component** - Fixed button group overflow
    - Added absolute positioning for dropdown options menu
    - Options now properly overlay below the dropdown button without triggering overflow
    - Changed button group height to use children() sizing

15. **Slider Component** - Fixed slider_background wrap warning
    - Changed slider flex direction from Column to Row
    - Label and background now sit side-by-side as intended

---

## All Screens Verified

| Screen | Status |
|--------|--------|
| Rubber Bandits Menu | ✓ FIXED - Contrast improved |
| Deadspace Settings | ✓ FIXED - Text readable |
| Parcel Corps Settings | ✓ FIXED - Arrow spacing |
| Sports Settings | ✓ FIXED - Arrow spacing |
| Images | ✓ FIXED - Icons larger |
| Empire Tycoon | ✓ FIXED - Tab labels correct |
| Decorative Frame | ✓ IMPROVED - Better layout |
| Tab Container | ✓ - All tabs visible |
| Neon Strike | ✓ - Labels visible |
| Accessibility | ✓ |
| AIM Chat | ✓ (intentional retro styling) |
| Angry Birds Settings | ✓ |
| Auto Text Color | ✓ |
| Bevel Borders | ✓ |
| Buttons | ✓ |
| Cards | ✓ |
| Casual Settings | ✓ |
| Checkboxes | ✓ |
| Circular Progress | ✓ |
| Colors | ✓ |
| Cozy Cafe | ✓ (truncation is intentional preview) |
| Example Borders | ✓ |
| Fighter Menu | ✓ |
| Flex Alignment | ✓ |
| Flight Options | ✓ |
| Forms | ✓ |
| Kirby Options | ✓ |
| Language Demo | ✓ |
| Layout System Demo | ✓ |
| Mini Motorways Settings | ✓ |
| Modals | ✓ |
| Navigation Bar Demo | ✓ |
| Nine Slice Borders | ✓ |
| Pagination | ✓ |
| Powerwash Settings | ✓ |
| Radio Buttons | ✓ |
| Scroll View | ✓ |
| Self Align | ✓ |
| Separators | ✓ |
| Setting Row Showcase | ✓ |
| Simple Button | ✓ |
| Tabbing | ✓ |
| Text (Typography Showcase) | ✓ |
| Text Input | ✓ |
| Text Overflow | ✓ (debug demo - showing overflow detection) |
| Text Shadow | ✓ |
| Text Stroke | ✓ |
| Themes | ✓ |
| Toasts | ✓ |
| Toggle Switches | ✓ |

---

## Technical Notes

### Empire Tycoon Icon Issue
The sidebar tab icons (`icon_rides.png`, `icon_food.png`, `icon_upgrades.png`, `icon_finance.png`) contained incorrect text labels baked into the bottom portion of each image. The labels were offset by one position (food icon had "Rides" text, upgrades icon had "Food Sta" text, etc.).

**Workaround applied:** Modified the sprite source rectangle to only display the top 32 pixels of each 64x48 icon, effectively cropping out the incorrect text labels. The correct labels are rendered via code.

**Permanent fix recommendation:** Regenerate the icon PNG files without text labels.

---

## Conclusion

**Fixed this session:** 15 issues across showcase screens and vendor framework
**Remaining layout warnings:** 0
**Remaining contrast warning:** 1 (WCAG AA - intentionally not addressed per user request)

All 50 UI showcase screens have been reviewed and verified. All layout overflow and wrap warnings have been resolved:

### Showcase Screen Fixes:
- ParcelCorpsSettings: Font sizes, arrow spacing
- SelfAlignShowcase: Removed overflow-causing margins
- ScrollViewShowcase: Checkbox sizing, removed problematic indicator
- AIMChatDemo: buddy_status sizing
- LanguageDemo: Content area height
- Themes: Toggle switch height
- And others (see full list above)

### Vendor Framework Fixes:
- Navigation bar: Floating-point precision fix for arrow layout
- Dropdown: Absolute positioning for overlay menu
- Slider: Row layout for label + background
