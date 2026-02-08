# Design Audit: Kart Select Screen

**Audit Date:** 2026-02-07
**Screen:** `kart_select` — Character and Vehicle Selection Screen
**Methodology:** Apple Human Interface Guidelines (1987) — Full Audit
**Auditor:** Automated Design Audit

---

## Screen Description

A full-screen racing game character/vehicle selection UI with a three-panel layout on a dark navy blue background. The top-left displays "SELECT YOUR RACER" in an italic gold serif-style font with a metallic sheen, and the top-right shows "Grand Prix – Mushroom C..." (truncated) alongside a bordered "P1" player indicator badge. The main content area is divided into three rounded-rectangle panels: the left panel contains a 4×2 grid of character icons (Turbo, Blaze, Drift, Tank, Zippy, Rumble, Flash, Pebble), each represented by a colored circle with a single letter initial; the center panel displays a large preview of the currently highlighted character "Drift" as a cyan circle with "D" alongside stat bars for SPD, ACC, HND, and WGT rendered as colored horizontal bars; the right panel lists vehicle options (Standard, Speedster, Off-Road, Classic) with "VEHICLE" as a cyan header and "Standard" highlighted in yellow. The bottom bar contains a dark "< Back" button on the left, a centered summary reading "Drift + Standard," small controller hints (A: Select, B: Back, L/R: Switch Kart), and a bright green "READY!" button on the right.

---

## Critical Issues (Fix Immediately)

### 1. Character Identity Relies Almost Entirely on Color
**Principle Violated:** Color Guidelines (3.1), Accessibility (8.1)
Each of the eight racers is represented by a colored circle containing a single-letter initial (T, B, D, K, Z, R, F, P). Color is the dominant differentiator between characters — Turbo is red, Blaze is orange, Drift is cyan, etc. The single-letter initials are insufficient as standalone identifiers since several are ambiguous (what does "K" stand for? "Tank" starts with "T" but uses "K"). For users with color vision deficiencies, especially deuteranopia or protanopia, the green "Flash" and green "Rumble" circles would be nearly indistinguishable. The HIG explicitly states that "color coding should be redundant with shape, position, or pattern" and that the "design should work in black and white first."
→ **Fix:** Replace the single-letter circles with distinct character portraits, silhouettes, or unique iconography that communicates identity independent of color. If circles must remain, give each a unique shape outline, pattern fill, or secondary visual marker (e.g., a small icon representing the character's trait). Display full character names beneath each icon at a legible size.

### 2. Dual Selection States Are Ambiguous and Conflicting
**Principle Violated:** Feedback and Dialog (1.7), Consistency (1.4), See-and-Point (1.3)
The character grid shows two distinct visual states simultaneously: "Turbo" has a white/light dotted border (suggesting the cursor or focus position), while "Drift" has a bright yellow background fill (suggesting it is the confirmed/active selection). These two states use completely different visual languages — outline vs. fill — and there is no legend or indication explaining which is the cursor and which is the selection. A user encountering this screen cannot confidently determine: is Drift my current character, or is Turbo? Did I select Drift and am now browsing to Turbo, or vice versa? The HIG requires that "visual feedback shows what's selected" with clarity, not ambiguity.
→ **Fix:** Unify the selection model. If only one character can be selected at a time, use a single, clear selection indicator (e.g., a bright border plus a check mark or character enlargement). If the screen supports both a cursor and a confirmed selection, differentiate them with explicit visual language (e.g., a pulsing outline for cursor, a solid border + "✓" for confirmed selection) and add a label or legend explaining the two states.

### 3. Stat Bar Abbreviations Are Cryptic Jargon
**Principle Violated:** Plain Language (7.1), See-and-Point (1.3), Accessibility (8.3)
The four stat labels — SPD, ACC, HND, WGT — are three-letter abbreviations that require interpretation. While "SPD" (Speed) and "ACC" (Acceleration) may be guessable, "HND" (Handling) and "WGT" (Weight) are significantly less obvious, especially for younger players or non-native English speakers. The HIG warns against "cryptic abbreviations or codes" and states that interfaces should use "user vocabulary, not programmer vocabulary." Users should never need to decode labels.
→ **Fix:** Use full words: "Speed," "Accel," "Handling," "Weight" — or if space is constrained, use more recognizable abbreviations paired with small icons (a speedometer for speed, a feather for weight, a steering wheel for handling). At minimum, ensure a tooltip or expanded label is available on hover/focus.

### 4. Stat Bars Lack Scale, Numbers, and Comparative Context
**Principle Violated:** WYSIWYG (1.5), Feedback and Dialog (1.7)
The four stat bars are colored horizontal fills with no numeric values, no tick marks, no maximum reference, and no scale. Users cannot determine whether the red SPD bar represents 30% or 60% of maximum speed — the bars simply show colored fills of varying lengths against an undefined background. Without numbers or gridlines, comparing stats between characters requires switching back and forth and relying on visual memory. The HIG states "no secrets — what's on screen is what exists" and that "immediate feedback confirms every action." The current stat bars are decorative rather than informative.
→ **Fix:** Add numeric values at the end of each bar (e.g., "7/10" or "70%"). Add subtle background gridlines or tick marks showing the full scale. Consider showing ghost/shadow bars of the previously selected character for direct comparison.

---

## Major Issues (Fix Soon)

### 5. Title Typography Sacrifices Readability for Decoration
**Principle Violated:** Aesthetic Integrity (1.10), Consistency (1.4)
"SELECT YOUR RACER" uses a heavy italic serif font with a gold metallic gradient or stroke effect. While thematically appropriate for a racing game, this decorative treatment reduces readability: the italic slant compresses character spacing, the serif details add visual noise, and the gold color with what appears to be a dark outline or shadow creates a busy appearance. This font style is completely different from the clean sans-serif used for all other text on the screen (character names, stat labels, vehicle names, button labels), creating a stark typographic inconsistency. The HIG warns against "decoration that distracts from function."
→ **Fix:** Simplify the title to use a bold sans-serif font that matches the rest of the UI, or at minimum reduce the decorative effects (remove the metallic gradient, reduce italic angle). If the stylized title is essential for brand identity, ensure it is the only decorative text element and that all functional text maintains a consistent clean font family.

### 6. Truncated Mode/Course Text Obscures Critical Information
**Principle Violated:** WYSIWYG (1.5), Feedback and Dialog (1.7)
The top-right corner displays "Grand Prix – Mushroom C..." with the text clearly truncated. This is the game mode and track/cup information — critical context for the selection the user is about to make. Truncating this text means the user cannot see the full cup name and must guess or remember it. The HIG states "no secrets — what's on screen is what exists" and that users should not need to remember information.
→ **Fix:** Ensure the full mode and cup name ("Grand Prix – Mushroom Cup") is visible without truncation. Reduce the title font size if needed, abbreviate "Grand Prix" to "GP" (a widely understood abbreviation in racing), or allocate more horizontal space to this label. Never truncate navigational context.

### 7. Vehicle List Items Lack Interactive Affordance
**Principle Violated:** See-and-Point (1.3), Feedback and Dialog (1.7), Direct Manipulation (1.2)
The vehicle list (Standard, Speedster, Off-Road, Classic) displays items as plain text on a dark background. Only "Standard" has a yellow background fill indicating selection. The remaining three items — Speedster, Off-Road, Classic — appear as static white text with no border, no hover state, no interactive cue, and no visual distinction from a simple text label. A new user cannot immediately tell whether these are selectable options, informational labels, or locked/unavailable choices. The HIG requires that "all available actions are visible" and that "recognition over recall" guide the design.
→ **Fix:** Add visible row separators, subtle background fills, or border outlines to each vehicle option to communicate selectability. Consider a left-edge indicator or chevron on each row. When an item receives focus/cursor, show a distinct hover/focus state (e.g., a lighter background fill) before the user commits to selection.

### 8. Stat Bar Colors Use Confusing Semantic Associations
**Principle Violated:** Color Guidelines (3.1, 3.2), Metaphors (1.1)
The stat bars use red for Speed, orange for Acceleration, green for Handling, and blue for Weight. Red universally signals danger, error, or poor performance in UI contexts — using it for Speed creates a misleading impression that the character's speed is "bad." Similarly, green typically signals "good" or "positive," making the Handling stat appear inherently favorable regardless of its actual value. These color semantics conflict with the bars' purpose of showing neutral numeric values. The HIG warns that color should "support understanding, not contradict" the information.
→ **Fix:** Use a single neutral color (e.g., a consistent blue or white) for all stat bars to avoid false semantic associations, or use a gradient from low-to-high within each bar (red→green) so color maps to quality. Alternatively, use the character's own accent color for all their stat bars, reinforcing identity rather than introducing false quality signals.

### 9. Bottom Bar Has Inconsistent Button Styling and Visual Weight
**Principle Violated:** Consistency (1.4), Aesthetic Integrity (1.10)
The bottom bar contains two primary action buttons with drastically different visual treatments: "< Back" is a dark, low-contrast outlined button that nearly blends into the dark footer, while "READY!" is a large, bright green filled button with white bold text and an exclamation mark. The asymmetry in size, color saturation, brightness, and text treatment makes "< Back" feel like an afterthought while "READY!" screams for attention. While emphasizing the primary action is valid UX practice, the degree of imbalance here is extreme — "< Back" is so subdued that a user may not notice it exists.
→ **Fix:** Give "< Back" enough visual presence to be easily discoverable — a lighter border, slightly larger text, or a subtle background fill. Reduce the visual gap between the two buttons while still maintaining "READY!" as the primary action. Both should clearly read as interactive buttons. Consider removing the exclamation mark from "READY!" as it adds visual noise without informational value.

### 10. Controller Hint Text Is Illegibly Small
**Principle Violated:** See-and-Point (1.3), Accessibility (8.1, 8.3), Plain Language (7.1)
The controller hints at the bottom center — "A: Select  B: Back  L/R: Switch Kart" — are rendered in an extremely small font size that is barely readable even on a close-up screenshot. On a television screen viewed from a typical couch distance (6–10 feet), this text would be virtually invisible. These hints serve a critical discoverability function, especially for first-time players learning controls. The HIG requires that "all available actions are visible" and that "keyboard shortcuts are alternatives, never the only way" — but here the control mappings are the only way users learn the input scheme.
→ **Fix:** Increase the hint text size significantly (at minimum 1.5–2× current size). Consider using button icons (A/B circles, L/R bumper shapes) with larger labels. Integrate the hints into the footer bar as a first-class UI element rather than a barely-visible footnote.

---

## Minor Issues (Consider Fixing)

### 11. Center Panel Has Excessive Empty Space Below Stats
**Principle Violated:** Aesthetic Integrity (1.10), Perceived Stability (1.9)
The center character preview panel shows the character icon and four stat bars in the upper 40% of the panel, leaving roughly 60% of the panel as empty dark space. This creates a top-heavy composition within the panel and wastes valuable screen real estate that could display additional character information (description, special ability, unlock status, etc.). The emptiness also makes the panel feel unfinished or like content failed to load.
→ **Fix:** Distribute the character preview content more evenly within the panel (vertically center it), or fill the lower portion with additional character details — a short character description, a visual representation of the selected vehicle, or a comparison view. If the space is intentionally empty, reduce the panel height to match its content.

### 12. "VEHICLE" Section Header Uses a Third Accent Color
**Principle Violated:** Color Guidelines (3.1), Consistency (1.4)
The right panel's "VEHICLE" header text is rendered in cyan/teal, while the title uses gold and the selection highlight uses yellow. This introduces a third accent color competing for attention. The HIG recommends a "limited palette (4–7 colors max for coding)" and the current screen already uses gold, yellow, cyan, red, orange, green, blue, purple, and white as significant colors. The cyan "VEHICLE" header clashes with the gold "SELECT YOUR RACER" title and creates uncertainty about whether cyan or gold represents section headings.
→ **Fix:** Style the "VEHICLE" header to match the title's gold color or use white to maintain neutrality. Establish a single accent color for all section headings across the screen.

### 13. No Visual Preview of the Selected Vehicle
**Principle Violated:** WYSIWYG (1.5), Feedback and Dialog (1.7), Direct Manipulation (1.2)
While the character selection provides a large preview circle and stat bars, the vehicle selection offers no visual representation whatsoever. Users see only the text names "Standard," "Speedster," "Off-Road," and "Classic" with no imagery, icon, silhouette, or visual indicator of what these vehicles look like or how they differ. This forces users to imagine or remember vehicle appearances — directly violating the "see-and-point" principle. The asymmetry between the character panel (rich visual feedback) and the vehicle panel (text-only) creates an inconsistent experience.
→ **Fix:** Add a vehicle preview image, icon, or silhouette next to each vehicle name or in a dedicated preview area. At minimum, show stat modifiers ("+Speed, -Handling") for each vehicle so users can make informed comparisons without memorizing vehicle properties.

### 14. Character Names Below Icons Are Too Small
**Principle Violated:** See-and-Point (1.3), Accessibility (8.1)
The character names displayed beneath each circle icon in the left panel (Turbo, Blaze, Drift, Tank, Zippy, Rumble, Flash, Pebble) use a very small font size relative to the icon. At typical gaming distances, these labels may be difficult to read, especially since they serve as the primary textual identifier for each character. The names are critical for users who cannot rely on color alone to identify characters.
→ **Fix:** Increase the name label size or move the names to a more prominent position (e.g., displayed in the center preview panel when a character is focused). Ensure names remain legible at 10-foot viewing distances typical for console gaming.

### 15. "P1" Badge Appears Clipped and Lacks Context
**Principle Violated:** Aesthetic Integrity (1.10), Feedback and Dialog (1.7)
The "P1" player indicator in the top-right corner appears partially clipped by the screen edge or adjacent text, and sits awkwardly close to the truncated course name. For a multi-player racing game, the player indicator is important contextual information — it tells the user which player slot this selection screen belongs to. Its current placement and small size make it feel like an afterthought rather than an intentional design element. There is no player color association or visual treatment connecting "P1" to the selection state below.
→ **Fix:** Give the P1 badge a clear, consistent position with adequate spacing from surrounding elements. Consider color-coding the badge to match the player's assigned color (if applicable) and using that color as a subtle accent throughout their selection panel. Ensure the badge is never clipped.

### 16. Summary Line "Drift + Standard" Provides Minimal Confirmation Value
**Principle Violated:** Feedback and Dialog (1.7), WYSIWYG (1.5)
The centered bottom text "Drift + Standard" summarizes the current selection, but its placement between the two navigation buttons and above the tiny controller hints makes it easy to overlook. The plain text format ("name + name") provides no visual distinction or emphasis. For a confirmation summary — the last thing a user sees before pressing "READY!" — this element should be more prominent and include visual representations of both choices.
→ **Fix:** Increase the size and visual prominence of the summary line. Consider adding small character and vehicle icons flanking the text. Use a slightly different background treatment or border to set it apart from the surrounding footer. This summary is the user's final verification checkpoint and deserves clear visual weight.

### 17. Left Panel Character Grid Lacks Organizational Logic
**Principle Violated:** Consistency (1.4), See-and-Point (1.3)
The eight characters are arranged in a 4×2 grid with no discernible organizational principle — they are not alphabetical (Turbo, Blaze, Drift, Tank), not grouped by stat specialization, and not ordered by any visible attribute. Users looking for a specific character type must scan all eight icons each time. As the roster grows, this lack of organization would become increasingly problematic.
→ **Fix:** Organize characters by a meaningful attribute (e.g., speed tier, weight class, or playstyle category) and add subtle category labels or dividers. Alternatively, sort alphabetically. If the order is intentional (e.g., unlock order), communicate this to the user with numbering or a progress indicator.

---

## Strengths

- **Clear three-panel layout establishes logical information flow:** The left-to-right progression (choose character → preview stats → choose vehicle) creates a natural reading order and decision-making flow that guides the user through the selection process without explicit instructions.
- **Character preview with stats provides immediate feedback:** Selecting a character instantly shows a larger preview and stat bars in the center panel, giving users real-time feedback about their choice — a strong application of the feedback principle.
- **Bottom summary confirms selections before committing:** The "Drift + Standard" summary and prominent "READY!" button create a clear two-step confirmation pattern (review → commit) that supports forgiveness — users can see their full selection before finalizing.
- **Prominent primary action button:** The green "READY!" button is unmistakably the primary action, reducing decision paralysis about what to do next once selections are complete.
- **Visible selection states exist for both character and vehicle:** Both the character grid and vehicle list provide visual indication of the current selection (yellow highlight), meaning the user always knows what is currently chosen — even if the indicator style could be improved.
- **Controller hints are present:** Despite being too small, the inclusion of A/B/L/R mappings at the bottom demonstrates awareness of discoverability needs for gamepad-driven interfaces.

---

## Overall Score: 4.5/10

The kart select screen establishes a sensible information architecture — the three-panel layout with character grid, preview, and vehicle selection creates a logical decision flow that works well conceptually. However, the visual execution has significant issues that undermine usability and accessibility. The most critical problems are the near-total reliance on color to differentiate characters (an accessibility failure), the ambiguous dual selection states in the character grid (focus vs. confirmed selection using different visual languages), and the cryptic stat abbreviations paired with scale-less bars that fail to communicate meaningful information. The typography is fragmented between the decorative serif title and the functional sans-serif body text, the vehicle list provides no visual previews to match the character panel's richness, and critical context text (course name, controller hints) is either truncated or illegibly small. The color palette, while vibrant, introduces too many competing accent colors (gold, yellow, cyan, per-character colors, per-stat colors) that dilute visual hierarchy. Resolving the color-as-sole-identifier problem for characters, clarifying the selection state model, and adding scale to the stat bars would produce the most significant usability improvement.
