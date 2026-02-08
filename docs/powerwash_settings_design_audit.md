# Design Audit: PowerWash Settings

**Screen:** In-game Video Settings panel  
**Source:** Game UI toolkit showcase  
**Audited Against:** Apple Human Interface Guidelines (1987 Desktop Interface)  
**Date:** 2026-02-07

---

## Audit Summary: PowerWash Settings

### Critical Issues (Fix Immediately)

1. **Wildly Inconsistent Label Typography Destroys Visual Hierarchy (Aesthetic Integrity §1.10)**: The setting labels in the left column use drastically different font sizes with no clear rationale. "Vsync" and "SSAO" are rendered in a much larger, heavier typeface than neighboring labels like "Screen Resolution," "Window Mode," and "Target Framerate," which appear in noticeably smaller text. "Anti-Aliasing" is also larger than "Render Scale," "Model Quality," and "Texture Quality." This creates a chaotic visual rhythm where the eye is pulled to the largest labels regardless of their importance. The HIG states that different things should look different and same things should look the same — all setting labels serve the same role and should share the same typographic treatment.  
   → **Fix**: Use a single, consistent font size and weight for all setting labels. If some settings are more important, use subtle visual cues (grouping, ordering) rather than random size variation.

2. **Heavy Use of Technical Jargon (Plain Language §7.1)**: Multiple labels use programmer and graphics-engineer vocabulary that typical users won't understand: "Vsync" (Vertical Sync), "Anti-Aliasing," "SSAO" (Screen Space Ambient Occlusion), and "Render Scale." The HIG is explicit that user-facing text must use user vocabulary, not programmer vocabulary. "SSAO" in particular is an opaque four-letter acronym with no inline explanation — even the help panel would need to be focused on it to reveal its meaning.  
   → **Fix**: Replace or supplement technical terms: "Vsync" → "Sync to Display," "Anti-Aliasing" → "Edge Smoothing," "SSAO" → "Ambient Shadows" or "Lighting Detail," "Render Scale" → "Resolution Scaling." Alternatively, place brief plain-language descriptions beneath each technical label.

3. **No Save/Apply Confirmation Mechanism (Feedback §1.7, User Control §1.6)**: The settings panel has no visible "Apply," "Save," or "Done" button. There is no indication whether changes take effect immediately (auto-apply) or require explicit confirmation. For video settings in particular — where changing resolution or window mode can render the screen unreadable — this is dangerous. Users have no feedback about whether their changes have taken effect and no way to explicitly commit or revert a batch of changes.  
   → **Fix**: Add an explicit "Apply" button with a revert timer (common pattern: "Keep these settings? Reverting in 15 seconds...") for potentially disruptive video settings changes. At minimum, provide visible confirmation feedback when settings are auto-applied.

4. **Tab Bar Uses Inconsistent Typography (Consistency §1.4, Aesthetic Integrity §1.10)**: The bottom tab bar showing GENERAL, GAMEPLAY, VIDEO, AUDIO, and CONTROLS uses wildly different font sizes. "VIDEO" (the active tab) is rendered in a very large, bold typeface, "AUDIO" is also oversized, while "GENERAL," "GAMEPLAY," and "CONTROLS" are much smaller. The size disparity makes inactive tabs hard to read and creates visual imbalance. Active/inactive state should be indicated by a consistent mechanism (highlight, underline, color shift), not by making the active tab text three times larger than its neighbors.  
   → **Fix**: Use a uniform font size for all tab labels. Indicate the active tab with a background highlight, underline, or color change rather than dramatic size scaling. All tabs should be equally readable at all times.

---

### Major Issues (Fix Soon)

5. **No Grouping of Related Settings (Aesthetic Integrity §1.10, See-and-Point §1.3)**: All nine video settings are presented in a single flat list with no visual separation between logically distinct groups. Display settings (Screen Resolution, Window Mode), performance settings (Target Framerate, Vsync), quality/rendering settings (Anti-Aliasing, SSAO, Render Scale, Model Quality, Texture Quality) are all intermixed. Users scanning for a specific setting must read every label rather than jumping to a relevant section.  
   → **Fix**: Group settings under labeled section headers (e.g., "Display," "Performance," "Quality") with spacing or subtle dividers between groups. This enables faster scanning and creates a meaningful hierarchy.

6. **"Render Scale: Off" Is Semantically Confusing (WYSIWYG §1.5, Plain Language §7.1)**: The Render Scale setting shows a value of "Off." Render scale is typically a percentage (50%, 75%, 100%, 150%) — "Off" is ambiguous. Does "Off" mean native resolution? Disabled? No scaling? The setting's current state doesn't clearly communicate what the user will experience. The HIG requires that the screen accurately represent reality.  
   → **Fix**: Replace "Off" with a clear value like "100% (Native)" or "Disabled — rendering at native resolution." If the setting is truly off, explain what that means in the help panel.

7. **Help Panel Is Passive and Redundant (See-and-Point §1.3)**: The right-side help panel ("Help: Anti-Aliasing — Adjust the Anti-Aliasing setting to your preference. Current: 2x") restates information already visible in the main list. The description "Adjust the Anti-Aliasing setting to your preference" is a tautology — it says nothing about what anti-aliasing actually does or how different values affect the game. The "Current: 2x" line duplicates the value shown in the main row.  
   → **Fix**: Provide genuinely informative descriptions: "Smooths jagged edges on object outlines. Higher values look better but reduce performance." Remove the redundant "Current: 2x" since it's already visible in the main list.

8. **Small, Low-Contrast Arrow Controls (Accessibility §8.3, See-and-Point §1.3)**: The `<` and `>` chevron arrows used to cycle through setting values are small text characters on a dark background with limited contrast. They are difficult to see at a glance and provide minimal indication that they are interactive controls. The selected row's `>` arrow appears brighter, but the unselected rows' arrows nearly disappear into the background.  
   → **Fix**: Increase the size and contrast of the arrow controls. Use solid arrow icons rather than text characters. Add a subtle button background or border to make them clearly interactive elements. Ensure all arrows meet minimum contrast ratios regardless of selection state.

9. **No Undo or Revert for Individual Settings (Forgiveness §1.8)**: While a "Reset" action is mapped to the O button, there is no per-setting undo and no indication of what "Reset" resets — the current setting, all video settings, or all settings across all tabs. The HIG mandates that users should be able to recover from mistakes easily. Changing a graphics setting and wanting to go back should be straightforward.  
   → **Fix**: Clarify the Reset scope (label it "Reset All Video Settings" or "Reset to Default"). Consider adding a per-setting "Default" indicator or the ability to undo the last change. Show original/default values alongside current values so users know what they're reverting to.

10. **Selection/Focus State Relies Primarily on Subtle Color Shift (Color §3.1, Accessibility §8.1)**: The currently focused row (Anti-Aliasing) is indicated by a slightly brighter background and a highlighted `>` arrow. The color difference between the focused and unfocused rows is subtle — a slightly lighter shade of the same dark blue. Users with low vision, color vision deficiencies, or playing on poorly calibrated monitors may not perceive the focus state. The HIG warns against relying on subtle color differences for important distinctions.  
    → **Fix**: Use a more prominent focus indicator: a solid border, a noticeably different background color, a left-edge accent bar, or a combination. Ensure the focus state is distinguishable even on a monochrome display.

---

### Minor Issues (Consider Fixing)

11. **"X" Close Button Is Minimal and Ambiguous (Controls §2.7, See-and-Point §1.3)**: The close button in the top-left corner is a plain "X" character with no button shape, border, or background. While functional, it lacks the visual affordance of a button — it looks like a static text character rather than an interactive control. Additionally, "X" is also listed in the button legend at the bottom as "Close," requiring users to cross-reference.  
    → **Fix**: Give the close button a visible button shape (rounded rectangle, circle) with adequate padding and contrast. This makes it immediately recognizable as interactive without needing to consult the legend.

12. **Button Legend Uses Abstract Symbols (See-and-Point §1.3, Metaphors §1.1)**: The bottom-left button legend shows "X Close," "O Reset," and "[] Select" using what appear to be controller button references. The "[]" symbol for Select is particularly abstract — it's unclear whether this represents a square button (PlayStation), a specific key, or something else. Users unfamiliar with the specific controller mapping must guess.  
    → **Fix**: Use recognizable controller button icons (filled shapes matching the actual controller) rather than text approximations. If supporting multiple input methods, show context-appropriate legends (keyboard keys when using keyboard, controller buttons when using controller).

13. **"SETTINGS" Title Provides No Context About Active Tab (Perceived Stability §1.9)**: The title simply reads "SETTINGS" even though the user is on the "VIDEO" tab. Users glancing at the top of the screen don't immediately know which settings category they're viewing. They must look down to the tab bar to determine this.  
    → **Fix**: Update the title to reflect the active tab: "SETTINGS — VIDEO" or "VIDEO SETTINGS." This provides immediate context and reduces the need for eye movement.

14. **No Indication of Value Ranges or Limits (WYSIWYG §1.5)**: When cycling through setting values with `<` and `>`, users have no indication of how many options are available, where they are in the list of options, or whether they've reached the minimum/maximum. For example, Anti-Aliasing shows "2x" — can it go higher? Is "Off" an option? Users must blindly cycle to discover the full range.  
    → **Fix**: Dim or hide the arrow on the side where no more options exist (e.g., hide `<` when at minimum). Alternatively, show a brief indicator like "2 of 4" or list all options with the current one highlighted.

15. **Large Unused Space on the Right Side (Aesthetic Integrity §1.10)**: The help panel occupies a relatively small area in the upper-right portion of the screen, leaving a large empty dark region on the right side below it. This wasted space creates an unbalanced layout. The settings list is compressed on the left while the right side is mostly vacant.  
    → **Fix**: Either expand the help panel to fill more of the right side (adding richer descriptions, preview images of quality settings, or performance impact indicators), or rebalance the layout to center the settings content more effectively.

16. **Window Mode Value Text Is Noticeably Smaller Than Other Values (Consistency §1.4)**: The "Borderless Windowed" value text is rendered in a smaller font compared to other values like "2560 x 1440," "Unlimited," and "On." This appears to be a response to the longer text string, but it breaks the consistent value typography and makes the setting look less important or secondary.  
    → **Fix**: Use a consistent font size for all setting values. If a value string is too long, abbreviate it ("Borderless") or use a slightly narrower font variant rather than shrinking the size.

17. **Tab Bar Has No Visual Container or Separation (Aesthetic Integrity §1.10)**: The bottom tab bar (GENERAL, GAMEPLAY, VIDEO, AUDIO, CONTROLS) sits directly on the dark background with only a subtle outlined box around each tab. The tabs lack a distinct container, background bar, or visual separation from the main content area above them. This makes the tab bar feel disconnected and loosely placed rather than anchored as a navigation element.  
    → **Fix**: Add a subtle background bar or separator line above the tab row to visually anchor it as a distinct navigation region. Give tabs consistent padding and spacing within a unified container.

---

### Strengths

- **Dark, Cohesive Color Theme**: The deep navy background with light text is appropriate for an in-game settings overlay. The monochromatic palette is restrained and avoids garish colors, creating a focused, calm aesthetic suitable for a simulation game.
- **Contextual Help Panel**: The right-side help panel providing descriptions of the currently focused setting is a thoughtful addition. It addresses the jargon problem partially by offering a space for explanation, even if the current descriptions need improvement.
- **Clear Tab Navigation Structure**: The five-tab organization (General, Gameplay, Video, Audio, Controls) follows a logical, standard categorization that users will find familiar from other games. The tabbed structure correctly separates concerns.
- **Consistent Control Pattern**: All settings use the same `< value >` arrow selector pattern, creating a predictable interaction model. Users learn the pattern once and can apply it to every setting.
- **Close, Reset, and Select Actions Are Labeled**: The button legend at the bottom explicitly maps controller buttons to actions, which is better than having no legend at all.
- **Visible Close Button**: Unlike some game settings screens, there is a visible "X" close mechanism in the top-left corner, providing an escape route.

---

### Overall Score: 4/10

The PowerWash settings screen establishes a reasonable foundation with its tabbed structure, consistent control pattern, and contextual help panel, but is undermined by significant typography and hierarchy issues. The wildly inconsistent font sizing — both in the setting labels (Vsync and SSAO dwarfing their neighbors) and the tab bar (VIDEO/AUDIO towering over other tabs) — creates a visually chaotic experience that violates the HIG's core aesthetic integrity principles. Heavy reliance on technical jargon without adequate plain-language alternatives alienates non-technical users, and the absence of save/apply confirmation for video settings (which can make the display unreadable) is a notable user control gap. The subtle focus state, minimal arrow controls, and semantically confusing values ("Render Scale: Off") further erode usability. Fixing the typography consistency, replacing jargon with accessible language, adding save/apply confirmation, and improving the focus state visibility would substantially elevate this from a functional but rough settings screen to a polished, user-friendly interface.
