# Design Audit: Parcel Corps Settings

**Screen:** In-game Settings panel  
**Source:** Game UI toolkit showcase  
**Audited Against:** Apple Human Interface Guidelines (1987 Desktop Interface)  
**Date:** 2026-02-07

---

## Audit Summary: Parcel Corps Settings

### Critical Issues (Fix Immediately)

1. **Color-Only Distinctions (Accessibility §8.1)**: The icon legend system uses colored circles (green, red, blue, pink, purple) as the primary way to categorize settings into groups (Language, Effects, Quality, Display, Sync). While a few icons include differentiating marks (X, ~, dots), the system fundamentally relies on color to convey category meaning. Colorblind users (particularly deuteranopia/protanopia) cannot distinguish the green "Language" icon from the red "Effects" icon, or the blue "Quality" icon from the purple "Sync" icon.  
   → **Fix**: Use distinct, recognizable shapes or symbolic icons for each category instead of colored circles. A globe for Language, a gear for Quality, a monitor for Display, a speaker for Audio, etc. Color can supplement but must not be the primary differentiator.

2. **Technical Jargon (Plain Language §7.1)**: Multiple settings use programmer/engineer vocabulary that general users won't understand: "MSAA" (Multi-Sample Anti-Aliasing), "VSync" (Vertical Sync), and "Texture Quality." The HIG is explicit: use user vocabulary, not programmer vocabulary. These terms require external knowledge to understand.  
   → **Fix**: Replace or supplement technical terms with plain-language descriptions. "MSAA" → "Edge Smoothing," "VSync" → "Screen Tearing Fix" or "Sync to Display," "Texture Quality" → "Surface Detail." Alternatively, add brief inline descriptions beneath each label.

3. **No Close/Back/Cancel Button (User Control §1.6, Forgiveness §1.8)**: The settings panel has no visible close button, back arrow, or cancel mechanism. There is no way for the user to dismiss the panel or discard changes. The HIG mandates that users must always be able to quit, save, or cancel. Being trapped in a settings panel with no escape route is a critical violation of user control.  
   → **Fix**: Add a clearly visible close/back button (e.g., an "X" in the top-right corner of the panel, or a "Back" / "Done" button). If settings are not auto-saved, also provide a "Cancel" option that reverts changes.

4. **No Save/Apply Confirmation (Feedback §1.7)**: There is no visible "Save," "Apply," or "Done" button, and no indication whether settings changes are applied immediately (auto-save) or require explicit confirmation. Users have no feedback about whether their changes have taken effect. This violates the feedback principle — every action should have immediate, visible confirmation.  
   → **Fix**: Either (a) show an explicit "Apply" / "Save" button that confirms changes, or (b) display transient confirmation feedback (e.g., a brief "Saved" toast or checkmark animation) when auto-saving each change.

---

### Major Issues (Fix Soon)

5. **No Grouping or Sectioning of Related Settings (Aesthetic Integrity §1.10)**: All 10 settings are presented in a single flat list with no visual grouping. Display settings (Resolution, Full Screen, MSAA, Texture Quality), effect settings (Motion Blur, VSync), and audio settings (Audio Volume, Music Volume) are all intermixed. The icon legend at the bottom attempts to solve this but creates an additional cognitive burden. The HIG calls for clear visual hierarchy where different things look different and related items are grouped.  
   → **Fix**: Group settings under labeled section headers: "Display," "Graphics," "Audio," "Accessibility." Use spacing, dividers, or subtle background color shifts to visually separate groups. This eliminates the need for the icon legend entirely.

6. **Icon Legend Requires Cross-Referencing (See-and-Point §1.3)**: Users must scroll to the bottom of the panel, read the icon legend, memorize the color-to-category mapping, then scroll back up to understand which category each setting belongs to. This violates "recognition over recall" — users should never need to memorize mappings or cross-reference a legend.  
   → **Fix**: Replace the icon legend with inline section headers (see issue #5). If icons must remain, use universally recognizable symbols that don't require a legend to decode.

7. **Inconsistent Control Types Without Clear Rationale (Consistency §1.4)**: The panel uses three different control types — toggle switches, left/right chevron selectors, and horizontal sliders — but the rationale for which type is used where is not always clear. Language and MSAA use chevron selectors; Subtitles, Full Screen, Motion Blur, and VSync use toggles; Audio/Music Volume use sliders. Resolution shows a static value with no visible control at all. The inconsistency makes it harder to predict how to interact with each setting.  
   → **Fix**: Establish clear rules: toggles for binary on/off options, chevron selectors for discrete multi-value options, sliders for continuous ranges. Make Resolution interactive (add chevron arrows) or explicitly mark it as informational-only with a different visual style (e.g., dimmed or labeled "Auto").

8. **Resolution Row Has No Interaction Affordance (Feedback §1.7, Direct Manipulation §1.2)**: The Resolution row displays "2360 x 1440" but lacks any control element — no arrows, no dropdown, no toggle. It's ambiguous whether this is a read-only display or an interactive setting. If interactive, there's no affordance inviting the user to change it. If informational, it shouldn't look identical to the other interactive rows.  
   → **Fix**: If interactive, add chevron arrows matching the Language/MSAA/Texture Quality pattern. If read-only, visually differentiate it (dimmed text, "Detected" label, or remove it from the interactive list).

9. **Slider Bars Lack Numeric Values (WYSIWYG §1.5)**: The Audio Volume and Music Volume sliders show a filled green bar but display no percentage or numeric value. Users cannot see exactly what level they've set, cannot match levels precisely, and cannot communicate their setting to others. The screen doesn't fully represent the actual state.  
   → **Fix**: Display a numeric percentage (e.g., "75%") or value label next to each slider, updating in real time as the slider moves.

10. **Small, Low-Contrast Chevron Arrows (Accessibility §8.3, See-and-Point §1.3)**: The left/right chevron arrows (‹ ›) on the Language, MSAA, and Texture Quality rows are small gray symbols on a gray background. They are difficult to see, hard to target with a mouse or controller, and provide minimal contrast. This is an accessibility and usability concern.  
    → **Fix**: Increase the size of chevron arrows, use a higher-contrast color (white or bright accent), and increase the clickable/tappable hit area. Consider adding a subtle button background shape to make them more clearly interactive.

---

### Minor Issues (Consider Fixing)

11. **Toggle States Lack Text Labels (WYSIWYG §1.5)**: The toggle switches use position (left/right) and green fill to indicate on/off, but provide no text label ("ON"/"OFF") to confirm the state. While the visual encoding is standard, some users (especially those with color vision deficiencies) may find it ambiguous whether green means "on" or simply the default color.  
    → **Fix**: Add "ON"/"OFF" text labels inside or adjacent to each toggle, or use a clear visual distinction beyond color (e.g., the toggle thumb changes from a circle to a checkmark when on).

12. **Inconsistent Typography Scale (Aesthetic Integrity §1.10)**: "MSAA" is rendered in a noticeably larger, bolder font compared to neighboring labels like "Texture Quality" and "Resolution." The "Settings" title, setting labels, and value labels all appear to use slightly different sizes and weights without a clear typographic scale. This creates visual noise.  
    → **Fix**: Establish a consistent typographic scale: one size/weight for the panel title, one for setting labels, one for setting values. All setting labels should be the same size.

13. **Surrounding HUD Elements Create Visual Noise (Aesthetic Integrity §1.10)**: The settings panel is displayed alongside active game HUD elements — a live chat feed, a stream overlay, a speed indicator ("009 m/h"), and a "LIVE" badge. These competing elements fight for attention and distract from the settings task. The HIG advises that graphics should support understanding, not create visual clutter.  
    → **Fix**: Dim or hide non-essential HUD elements when the settings panel is open. A semi-transparent dark overlay behind the panel would focus attention on the settings.

14. **No Visible Focus/Selection State (Feedback §1.7)**: None of the settings rows show a hover, focus, or selection state. There is no indication of which setting is currently active or ready for keyboard/controller input. In a game context (likely controller-navigated), this is particularly important — users need to know where they "are" in the list.  
    → **Fix**: Add a visible highlight, border, or background color change to the currently focused/selected setting row. This is essential for controller and keyboard navigation.

15. **Icon Legend Placement at Panel Bottom (See-and-Point §1.3)**: The icon legend is placed at the very bottom of the settings panel in small text. Users may never scroll down far enough to see it or may not realize it exists. If the legend is essential for understanding the interface, it should be more prominent.  
    → **Fix**: If keeping the icon legend (rather than replacing it with section headers), move it to the top of the panel or make it always visible. Better yet, eliminate the need for a legend entirely (see issue #5).

16. **"Game Time" and Battery Indicator Are Unnecessary in Settings Context (Perceived Stability §1.9)**: The header of the settings panel shows "Game Time: 23:45" and a battery icon with signal bars. These elements mimic a phone status bar but serve no clear purpose in a game settings panel. They add visual noise without functional value, and the phone metaphor is inconsistent with a PC/console game settings context.  
    → **Fix**: Remove the game time and battery indicator from the settings panel header unless they serve a gameplay purpose. If game time is important, display it in the main HUD instead.

17. **Motion Blur and VSync Use Cryptic Category Icons (Metaphors §1.1)**: Motion Blur is tagged with a red "X" circle icon, and VSync with a purple "~" (tilde) circle. These symbols are abstract and don't connect to any real-world concept. A red X typically means "error" or "delete" — using it for "Effects" category is confusing. A tilde typically means "approximately" — using it for "Sync" is unintuitive.  
    → **Fix**: Replace abstract category markers with meaningful icons. For effects: a sparkle or wand icon. For sync: a refresh/sync arrows icon. Or, eliminate per-row category icons entirely in favor of grouped sections.

---

### Strengths

- **Clean, Dark Theme**: The dark panel background with light text is appropriate for a game settings screen and reduces eye strain during gameplay. The overall aesthetic is cohesive and game-appropriate.
- **Good Use of Toggle Switches for Binary Options**: Subtitles, Full Screen, Motion Blur, and VSync correctly use toggle switches for on/off states, which is an intuitive control choice.
- **Chevron Selectors for Multi-Value Options**: Language, MSAA, and Texture Quality correctly use left/right selectors for cycling through discrete options, showing the current value clearly.
- **Slider Bars for Continuous Values**: Audio and Music Volume appropriately use slider controls for continuous adjustment, which is the correct control type for volume.
- **Consistent Row Layout**: Each setting follows a consistent left-to-right layout pattern: icon → label → control. This creates predictable scanning patterns.
- **Rounded Corner Panel Design**: The settings panel has a clear, contained shape with rounded corners that visually separates it from the game world, making it easy to identify as an overlay.

---

### Overall Score: 4/10

The Parcel Corps settings screen demonstrates a competent basic layout with appropriate control type selection, but suffers from significant usability issues. The reliance on color-coded icons without accessible alternatives, heavy use of technical jargon, absence of close/save controls, and lack of visual grouping all represent fundamental violations of the Apple HIG's core principles. The flat, ungrouped list of settings with a cross-reference legend inverts the "See-and-Point" principle into "Remember-and-Match." Addressing the critical issues (accessibility, plain language, user control, and feedback) would substantially improve the experience. Adding section groupings and eliminating the icon legend would transform this from a functional but confusing panel into an intuitive, well-organized settings screen.
