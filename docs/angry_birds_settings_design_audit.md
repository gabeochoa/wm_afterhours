# Angry Birds Settings Screen — Apple HIG Design Audit

*Audited against: Apple Human Interface Guidelines (1987 Desktop Interface)*
*Screen: Settings modal/panel from Angry Birds game UI*

---

## Audit Summary: Angry Birds Settings Screen

### Critical Issues (Fix Immediately)

1. **[Color as Sole Differentiator — HIG 3.1]**: The ON/OFF toggle buttons for Music, Sound, and Vibrate rely primarily on color (green = ON, gray = OFF) to communicate state. While the text labels "ON" and "OFF" provide a secondary cue, the green vs. gray distinction has poor differentiation for users with deuteranopia (red-green color blindness). The small size of the toggles makes the text labels harder to read, leaving color as the dominant signal.
   → **Fix**: Use a consistent toggle/switch component with distinct shape states (e.g., filled circle vs. empty circle, or a standard iOS-style slider). Ensure the ON/OFF state is distinguishable purely by shape/position, not just color.

2. **[Insufficient Contrast — HIG 3.2]**: The build metadata text ("Build: 15555.1.114203", "Version 1.11.0.12346", "Player: #281-676-956") at the bottom-left is rendered in a light gray on the cream/beige background. This fails minimum contrast requirements and is nearly illegible, especially for users with low vision.
   → **Fix**: Increase text contrast to at least 4.5:1 ratio against the background. Use a medium-gray (#666 or darker) on the cream background, or move this info to a dedicated "About" sub-screen.

3. **[Inconsistent Control Patterns — HIG 1.4 Consistency]**: The Notifications setting uses a completely different control pattern (a wide button with a red "X" icon and "Notifications: OFF" text) compared to the Music/Sound/Vibrate settings (small rounded ON/OFF toggle buttons). All four are binary on/off settings, yet they use three distinct visual treatments. This forces users to learn multiple interaction paradigms for the same conceptual action.
   → **Fix**: Unify all binary on/off settings (Music, Sound, Vibrate, Notifications) into the same toggle control type. Use one consistent pattern — either toggle switches or labeled ON/OFF buttons — for all of them.

4. **[Poor Visual Hierarchy — HIG 1.10 Aesthetic Integrity]**: The navigation buttons (Credits, Support, Terms and Privacy) on the right column are rendered with the same size, color, and visual weight as the Notifications and Language settings buttons on the left. Secondary informational links compete visually with primary interactive settings, making it impossible to scan for what matters most.
   → **Fix**: De-emphasize navigation links by using text links, smaller buttons, or a muted color. Reserve the prominent blue button style for primary actions and settings controls.

5. **[Close Button Resembles Error Icon — HIG 2.7 Controls / 1.1 Metaphors]**: The "X" close button in the top-right uses a red circle with a white X, which strongly resembles a "stop" or "error" indicator rather than a standard close/dismiss control. The color red carries a semantic meaning of danger or error, conflicting with the benign action of closing a settings panel.
   → **Fix**: Use a neutral close button (gray or matching the header color). Reserve red exclusively for destructive actions or error states. A simple "X" or "Done" text button in neutral colors would be clearer.

### Major Issues (Fix Soon)

1. **[Excessive Color Palette — HIG 3.1]**: The screen uses at least 7 distinct hue families: coral/salmon (header), green (ON toggles), gray (OFF toggle), red (close button, notification X), teal/dark-green (Sync button), steel blue (action buttons), and cream (background). The HIG recommends 4-7 colors maximum for color coding; this screen approaches the upper limit with colors serving no consistent semantic purpose.
   → **Fix**: Reduce to a cohesive palette of 3-4 functional colors. Assign semantic meaning: one accent color for interactive elements, one for active/enabled states, one for disabled/off states, and neutral tones for backgrounds and text.

2. **[No Logical Grouping — HIG 1.10 Aesthetic Integrity]**: Related settings are not visually grouped. Audio settings (Music, Sound) sit next to a haptic setting (Vibrate) with no divider. Notifications and Language are below but in a different button style. The right column mixes data management (Save/Load Progress) with navigation links (Credits, Support, Terms). There are no section headers, dividers, or visual containers to communicate relationships.
   → **Fix**: Group related settings under labeled sections: "Audio & Haptics" (Music, Sound, Vibrate), "Preferences" (Notifications, Language), "Data" (Save/Load Progress), and "About" (Credits, Support, Terms). Use section headers or visual separators.

3. **[Header Contrast — HIG 3.2]**: The white "Settings" title text on the coral/salmon (#E87F5F approximately) header bar has a contrast ratio of roughly 2.5:1, well below the 4.5:1 minimum for normal text and 3:1 for large text. This makes the title hard to read, especially in bright lighting conditions.
   → **Fix**: Either darken the header background color significantly or use dark text on a lighter header. Ensure the title meets at least 3:1 contrast ratio for large text.

4. **[Layout Asymmetry and Misalignment — HIG 1.10 Aesthetic Integrity]**: The two-column layout has uneven vertical alignment. The left column has toggles at the top, then two tall buttons, then small metadata text. The right column has a bordered "Save/Load Progress" box at the top, then three buttons of varying width. The columns don't align horizontally — the Notifications button doesn't align with Credits, and Language doesn't align with Support. The "Terms and Privacy" button extends wider than the others.
   → **Fix**: Establish a consistent grid. Align elements across columns at shared baselines. Make all buttons in a column the same width. Consider a single-column layout if the two-column approach creates more alignment problems than it solves.

5. **[Jargon and Technical Information — HIG 7.1 Plain Language]**: The bottom-left displays "Build: 15555.1.114203", "Version 1.11.0.12346", and "Player: #281-676-956". Build numbers and internal version strings are developer-facing information that means nothing to most users. The HIG explicitly warns against technical jargon in user-facing text.
   → **Fix**: Remove build information from the main settings view entirely, or simplify to just "Version 1.11" if needed. Move detailed build info to a hidden "About" screen accessible via a tap gesture (e.g., tap version number 5 times).

6. **[Sync Button Orphaned and Unclear — HIG 1.7 Feedback]**: The small "Sync" button sits inside a "Save/Load Progress" box but provides no indication of what will happen when pressed. Will it upload? Download? Both? Overwrite local data? There is no description, confirmation dialog indication (no ellipsis), or feedback mechanism visible.
   → **Fix**: Add a brief description ("Sync your progress to the cloud"), use an ellipsis on the button ("Sync...") to indicate a confirmation dialog will appear, or provide a more descriptive label like "Back Up" or "Restore".

### Minor Issues (Consider Fixing)

1. **[Inconsistent Button Sizing — HIG 1.4 Consistency]**: The ON/OFF toggle buttons are small squares, the Sync button is a small teal rectangle, the Notifications and Language buttons are medium-width rounded rectangles, and the Credits/Support/Terms buttons are wider rounded rectangles. There is no consistent sizing system for interactive controls.
   → **Fix**: Establish 2-3 standard button sizes (small, medium, large) and apply them consistently based on function: toggles for on/off, standard buttons for navigation, and accent buttons for primary actions.

2. **[Typography Inconsistency — HIG 1.4 Consistency]**: The "Settings" title appears to use a different typeface (serif or stylized) from the body text (sans-serif). The ON/OFF labels on toggles use a bold italic style, button labels use bold centered text, and the build info uses a smaller regular weight. At least 4 distinct typographic treatments are used with no clear hierarchy system.
   → **Fix**: Define a type scale with at most 3 levels: title (large, one font), body/labels (medium, one font), and metadata (small, same font). Apply consistently across all elements.

3. **[No Cancel/Done Button — HIG 2.5 Dialog Boxes / 1.8 Forgiveness]**: The only way to dismiss the settings panel is the red "X" button. The HIG specifies that dialogs should always have a Cancel button, and that the most important actions should be clearly labeled buttons at the bottom. There is no "Done", "Save", or "Cancel" button to confirm or discard changes.
   → **Fix**: Add a clearly labeled "Done" button (or "Save" if settings require explicit saving) at the bottom of the panel. Keep the X as a secondary close option.

4. **[Decorative Background Adds Visual Noise — HIG 1.10 Aesthetic Integrity]**: The dark green/olive gradient background behind the settings panel serves no functional purpose. It adds visual complexity and reduces the perceived crispness of the modal edges, especially with the rounded corners and shadow treatment.
   → **Fix**: Use a simple semi-transparent dark overlay behind the modal, or a solid muted color. The background should recede, not draw attention.

5. **[Small Touch Targets — HIG 2.7 Controls / 8.3 Motor]**: The ON/OFF toggle buttons appear to be approximately 40x40 pixels or smaller. On mobile devices, the recommended minimum touch target is 44x44 points (Apple) or 48x48 dp (Material). The small toggles may be difficult to tap accurately, especially for users with motor impairments.
   → **Fix**: Increase toggle button size to at least 44x44 points, or switch to a standard toggle/switch control that provides a larger hit area.

6. **[Notification Red X Icon Ambiguity — HIG 1.1 Metaphors]**: The small red "X" icon inside the Notifications button could be interpreted as an error state, a delete action, or a "close" action rather than indicating "notifications are turned off". The metaphor is unclear — red X means "wrong" or "delete" in most contexts, not "disabled".
   → **Fix**: Use a standard toggle switch for notifications, matching the Music/Sound/Vibrate controls. If using an icon, use a bell with a slash through it (universally understood as "notifications off") rather than a red X.

### Strengths

- **Clear title**: The "Settings" title is prominently displayed, making it immediately clear what screen the user is on.
- **Toggle state labels**: The ON/OFF toggles include text labels (not just color), providing a secondary non-color cue for state.
- **Logical settings inclusion**: The screen includes the expected settings for a mobile game (audio, haptics, notifications, language, cloud save).
- **Visual warmth**: The coral/cream color scheme aligns with the Angry Birds brand and creates a friendly, approachable feel appropriate for a casual game.
- **Close button present**: The panel provides a visible way to dismiss and return to the game.

### Overall Score: 3/10

The Angry Birds Settings screen suffers from significant consistency, contrast, and hierarchy problems. While it succeeds at brand expression and includes the right settings categories, the inconsistent control patterns (three different styles for binary toggles), poor contrast ratios (header text, metadata text), excessive color palette, lack of logical grouping, and ambiguous iconography (red X for close and notifications) create a screen that is harder to scan, understand, and use than it should be. The most critical fixes are unifying toggle control patterns, improving text contrast, and establishing a clear visual hierarchy between primary settings and secondary navigation links.
