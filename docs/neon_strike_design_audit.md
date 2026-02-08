# Design Audit: Neon Strike HUD Screen

**Audit Date:** 2026-02-07
**Screen:** `neon_strike` — First-Person Shooter In-Game HUD
**Methodology:** Apple Human Interface Guidelines (1987) — Full Audit
**Auditor:** Automated Design Audit

---

## Screen Description

A full-screen first-person shooter (FPS) heads-up display rendered on a near-black background. The top-center features a compass/directional indicator with a circular dial showing N/S/E/W cardinal labels. The top-right displays "SCORE: 15,000" alongside a small circled "(U)" icon, and directly below that an amber/gold-bordered box reads ">> SECURE OBJECTIVE B" in a decorative stencil-style font. Centered beneath the compass, a kill feed shows three lines of event messages in muted light gray ("Alpha_Six eliminated [Emepine] with M4", "[Tsha_Rio] eliminated [Emepine] with SCAR", "Alpha_Six – Under fire!"). A cyan cross-shaped crosshair sits at the screen center. The right side shows a team chat message "@ * Alpha_Six: Under fire!" in white text. The left edge has four vertically stacked dark olive/green square icons representing killstreak abilities, with one labeled "UAV." The bottom-left area shows a "KILLSTREAK" label with two small square indicators, a "7B Pop <192>" label, and a dark green minimap with a red dot. Bottom-center features a large "35/210" ammo counter in oversized numerals, a "80 HEALTH" bar with a teal/blue fill, and a smaller unlabeled "45%" bar beneath. The bottom-right displays three weapon/equipment slot boxes with small icons including what appears to be a document, a knife, and another item, with a small "x2" counter on one.

---

## Critical Issues (Fix Immediately)

### 1. Unlabeled "45%" Bar Has No Context or Meaning
**Principle Violated:** WYSIWYG (1.5), Feedback and Dialog (1.7), See-and-Point (1.3)
Directly beneath the "80 HEALTH" bar sits a second, smaller bar displaying "45%" with a blue fill. This bar has no label, no icon, and no identifier of any kind. The player is left to guess what this represents — armor? shield? stamina? ability cooldown? The HIG demands "no secrets — what's on screen is what exists" and that feedback be "brief, direct, and use user vocabulary." A bar with only a percentage violates all three of these requirements. During intense gameplay, an unlabeled secondary status bar is worse than no bar at all because it introduces confusion at the worst possible moment.
→ **Fix:** Add a clear text label (e.g., "ARMOR," "SHIELD," or "STAMINA") above or to the left of the 45% bar, matching the style of the "80 HEALTH" label. Use a different fill color from the health bar to reinforce the distinction visually. If the bar represents a fundamentally different system, consider also using a different shape or pattern (e.g., segmented fill vs. continuous fill).

### 2. Minimap Has Dangerously Low Contrast
**Principle Violated:** Color Guidelines (3.2), Accessibility (8.1), Aesthetic Integrity (1.10)
The minimap in the bottom-left corner uses a very dark green background with barely visible darker green detail, bordered by a thin dark outline. The only clearly visible element is a single small red dot. At gameplay speed, a player needs to glance at the minimap and instantly extract positional information — but the contrast between the map's background and its features is so low that the map reads as an undifferentiated dark rectangle. The HIG explicitly warns that "text and thin lines have sufficient contrast to be easily visible" and that "small objects use high-contrast colors." A minimap where map features are invisible fails its core purpose.
→ **Fix:** Significantly increase the contrast between map terrain/features and the map background. Use lighter line work for roads, boundaries, or terrain features. Add a semi-transparent lighter background to the map area. Ensure player and enemy indicators use colors with strong contrast against the map background — and use shapes (triangle for player direction, dots for enemies) rather than color alone.

### 3. Killstreak Icons Are Indistinguishable at Glance Speed
**Principle Violated:** Metaphors (1.1), See-and-Point (1.3), Accessibility (8.1)
The four killstreak ability icons stacked on the left edge use dark olive/green backgrounds with faint, low-contrast line art that is nearly impossible to parse at a glance. The icons appear to show different military equipment (drone, turret, etc.), but the details dissolve into the dark background. Only one icon has a text label ("UAV"); the other three rely entirely on their illegible imagery. During active gameplay, a player cannot afford to study each icon — they need to recognize abilities instantly. The HIG requires that "icons and visual elements resemble real-world objects users already understand" and that recognition be immediate.
→ **Fix:** Increase the icon line weight and contrast dramatically — use bright white or amber outlines on the dark backgrounds. Add text labels to all four killstreak icons, not just "UAV." Consider using a lighter background for the icon containers (e.g., a medium gray-green) so the icon art has room to breathe. Differentiate active vs. locked icons with brightness or a lock overlay rather than subtle tonal shifts.

### 4. Red Dot on Green Minimap Fails Red-Green Colorblind Users
**Principle Violated:** Color Guidelines (3.1), Accessibility (8.1)
The minimap's primary indicator is a red dot on a green background — the single worst color combination for the approximately 8% of males with red-green color vision deficiency (deuteranopia/protanopia). This is the exact scenario the HIG's color guidelines were written to prevent: "color is the only way to distinguish elements" and "no consideration for colorblind users." A player with red-green color blindness would see the dot and background as nearly identical shades, rendering the minimap's most important feature invisible.
→ **Fix:** Use shape and size differentiation in addition to color. Make the player indicator a directional triangle or chevron rather than a simple dot. Add a bright outline or pulsing animation. Consider offering a colorblind mode that uses blue/orange instead of red/green. At minimum, add a contrasting border ring around the dot so its shape, not just its hue, distinguishes it from the background.

---

## Major Issues (Fix Soon)

### 5. Fragmented Color Palette Creates Visual Dissonance
**Principle Violated:** Consistency (1.4), Color Guidelines (3.1), Aesthetic Integrity (1.10)
The HUD simultaneously uses at least four unrelated accent colors: cyan/teal (crosshair), amber/gold (objective border, score text), teal-blue (health bar fill), and red (minimap dot). The killstreak icons add a fifth with olive-green, and the "45%" bar introduces yet another shade of blue distinct from the health bar. These colors have no systematic relationship — they don't follow a complementary, analogous, or triadic harmony. The result is a HUD that feels assembled from different games rather than designed as a unified system. The HIG recommends a "limited palette (4–7 colors max)" where colors have clear, consistent semantic meaning.
→ **Fix:** Define a strict semantic color system: one accent for interactive/objective elements (gold), one for health/status (teal), one for warnings/enemies (red), and one for the crosshair (white or gold). Ensure each color appears in only one semantic role. Reduce the total number of accent hues to 3–4 and use value/saturation variations rather than entirely different hues for related elements.

### 6. ">> SECURE OBJECTIVE B" Uses a Decorative Font That Impairs Legibility
**Principle Violated:** Aesthetic Integrity (1.10), Feedback and Dialog (1.7), Plain Language (7.1)
The current objective — arguably the most time-critical information on the entire HUD — is rendered in a stencil-style decorative typeface with broken letter strokes. This font choice prioritizes military-themed aesthetics over rapid readability. In a fast-paced FPS, players glance at the objective for a fraction of a second; every millisecond spent decoding broken letterforms is a millisecond of lost gameplay awareness. The HIG states that graphics should "support understanding, not just decoration" and that feedback should be "brief, direct, and use user vocabulary."
→ **Fix:** Replace the stencil font with a clean, highly legible sans-serif typeface at the same or larger size. Keep the amber border treatment for visual identity. If the military stencil aesthetic is essential to the game's brand, use it only for non-critical decorative elements (e.g., loading screens, menu titles) and never for in-gameplay text that must be read instantly.

### 7. Ammo Counter "35/210" Is Disproportionately Large
**Principle Violated:** Aesthetic Integrity (1.10), Consistency (1.4)
The ammo counter dominates the bottom-center of the screen with oversized numerals ("35/210") that are approximately 3–4x larger than any other text element on the HUD. While ammo count is important, it is displayed with dramatically more visual prominence than equally critical information like health (shown in a small labeled bar), the current objective (shown in a bordered box), or the minimap. This creates a lopsided visual hierarchy where one data point screams while others whisper. The HIG warns against a "flat visual hierarchy — everything same prominence," but the inverse problem — one element at extreme prominence — is equally disruptive.
→ **Fix:** Reduce the ammo counter to approximately 60–70% of its current size. Bring it into visual proportion with the health bar and other bottom-screen HUD elements. Use font weight (bold) rather than extreme size to maintain its importance. Consider grouping the ammo counter adjacent to the weapon slot icons on the right for logical spatial association.

### 8. Health Bar and Ammo Counter Have No Spatial Grouping Logic
**Principle Violated:** Consistency (1.4), Aesthetic Integrity (1.10), See-and-Point (1.3)
The player status information is scattered across the bottom of the screen without clear grouping: the ammo counter sits at center-left, the health bar floats at center, the weapon slots are at the far right, and the minimap/killstreak info is at the far left. There is no spatial logic connecting related elements — ammo and weapon slots are separated by the health bar, and health has no visual connection to the armor/shield bar below it. The HIG principle that "different things look different" implies its complement: related things should look related and be spatially grouped.
→ **Fix:** Group related elements together: place ammo count adjacent to (or integrated with) the weapon/equipment slots on the right. Group health and the 45% bar into a unified status cluster. Consider a consistent bottom bar layout with clear left/center/right zones: navigation (minimap + compass) on the left, player status (health + armor) in the center, and combat readiness (ammo + weapons + killstreaks) on the right.

### 9. Kill Feed Text Has Insufficient Contrast and No Background
**Principle Violated:** Color Guidelines (3.2), Feedback and Dialog (1.7), Accessibility (8.1)
The kill feed messages at the top-center of the screen are rendered in medium-gray text directly over the game viewport with no background panel, shadow, or contrast-enhancing treatment. Against the current dark background the text is marginally readable, but in actual gameplay when the scene behind the text varies (bright skies, explosions, light-colored walls), these messages would frequently become invisible. The HIG requires that "text and thin lines have sufficient contrast to be easily visible" — text overlaid on unpredictable backgrounds cannot guarantee this.
→ **Fix:** Add a semi-transparent dark background panel behind the kill feed area, or apply a consistent text shadow/outline (2px dark stroke) to all kill feed text. Use full white rather than medium gray for the text color. Consider adding a subtle fade-in/fade-out animation so messages draw the eye when they appear.

### 10. Weapon/Equipment Slots Lack Labels and Affordance Cues
**Principle Violated:** See-and-Point (1.3), Metaphors (1.1), Feedback and Dialog (1.7)
The three weapon/equipment boxes in the bottom-right display small, low-contrast icons (what appears to be a document, a knife, and a cursor/pointer icon) with no text labels. A small "x2" counter appears on the leftmost box, but with no context for what it counts. The boxes have no visual differentiation for active vs. inactive states — all three appear in the same muted gray-brown tone. A player cannot determine at a glance which weapon is currently equipped, what the items are, or which slot is selected. The HIG states that "all available actions are visible" and that users should be able to identify options by looking, not remembering.
→ **Fix:** Add text labels below each slot (e.g., "PRIMARY," "MELEE," "TACTICAL"). Clearly highlight the currently equipped weapon with a bright border or background color distinct from the unselected slots. Increase icon size and contrast within the boxes. Display the weapon name alongside or below the active slot.

---

## Minor Issues (Consider Fixing)

### 11. "7B Pop <192>" Uses Cryptic Jargon
**Principle Violated:** Plain Language (7.1), See-and-Point (1.3)
Below the "KILLSTREAK" label sits the text "7B Pop <192>" — a string that is completely opaque to anyone who doesn't already know what it means. "7B" could be a zone identifier, "Pop" might mean population, and "<192>" is wrapped in angle brackets for unclear reasons. The HIG explicitly prohibits "jargon or technical terms" and "cryptic abbreviations or codes" in user-facing text. Even experienced FPS players would need to learn this game's specific shorthand.
→ **Fix:** Expand the label to something human-readable: "Zone 7B — 192 players," "Server: 7B (192 online)," or whatever the data actually represents. If screen space is limited, use a tooltip or hold-to-expand interaction. At minimum, remove the angle brackets which serve no communicative purpose.

### 12. Compass Cardinal Labels Are Too Small
**Principle Violated:** Color Guidelines (3.2), See-and-Point (1.3)
The compass at the top-center of the screen displays N/S/E/W labels in a very small font size. While the central compass dial itself is reasonably sized, the cardinal direction letters are diminutive — particularly "E" and "W" on the sides. In a game where directional callouts are critical for team communication ("enemy to the East"), the compass labels should be instantly readable. The current size requires focused attention to confirm a direction.
→ **Fix:** Increase the cardinal label font size by 30–50%. Use full white or bright amber for the labels to increase contrast. Consider adding intermediate direction markers (NE, NW, SE, SW) and degree ticks for more precise navigation reference.

### 13. "(U)" Icon in Top-Right Corner Lacks Context
**Principle Violated:** Metaphors (1.1), See-and-Point (1.3)
A small circle containing the letter "U" appears next to the score in the top-right corner. It has no label, no tooltip, and no visual cue about its function. Is it a user profile button? An upgrade indicator? A currency symbol? The HIG requires that icons "resemble real-world objects users already understand" — a circled letter "U" has no widely recognized meaning in gaming or general UI conventions.
→ **Fix:** Replace the "(U)" with a recognizable icon (e.g., a person silhouette for profile, a star for rank, a coin for currency) or add a small text label beside it. If it's a button, add hover/focus affordance. If it's purely informational, integrate the information it represents into the score display or a dedicated status area.

### 14. Inconsistent Border Treatments Across Components
**Principle Violated:** Consistency (1.4), Aesthetic Integrity (1.10)
The screen uses at least three different border styles: the minimap has a rounded-corner border with thick dark strokes, the objective box has a sharp-cornered amber/gold border with a double-line treatment, the killstreak icons have subtle dark borders with slightly rounded corners, and the weapon slots use yet another border style with sharp corners and thinner lines. The HIG states that "same action produces same result" and extends this to visual consistency — structural UI elements should share a consistent border language.
→ **Fix:** Standardize all HUD container borders to one style. Choose either rounded or sharp corners and apply consistently. Use one border weight throughout (e.g., 2px). Differentiate containers by fill color or opacity rather than border style. Reserve the amber/gold border for the single highest-priority element (the objective).

### 15. KILLSTREAK Indicator Boxes Are Too Small to Parse
**Principle Violated:** See-and-Point (1.3), Feedback and Dialog (1.7), Color Guidelines (3.2)
The "KILLSTREAK" label is accompanied by two tiny square indicators (approximately 8–10px each) that appear to show different fill states. At this size, the indicators are effectively invisible — a player cannot determine their status without leaning forward and squinting. The HIG warns that "small objects use high-contrast colors" and that feedback should be immediately clear.
→ **Fix:** Increase the indicator size to at least 16–20px. Use clearly differentiated colors (filled bright amber for earned, dark gray for unearned). Add a number or icon inside each indicator to show which killstreak reward it represents. Consider integrating these indicators directly with the killstreak ability icons on the left side of the screen to avoid redundant scattered elements.

### 16. Team Chat Message Has No Visual Container
**Principle Violated:** Aesthetic Integrity (1.10), Perceived Stability (1.9)
The team chat message "@ * Alpha_Six: Under fire!" on the right side of the screen floats as bare white text with no background panel, border, or visual grouping. It appears to hover arbitrarily in the right-center of the viewport. Without a container, it is visually indistinguishable from the kill feed messages at the top-center — both are just text floating over the game scene. The chat and kill feed serve different purposes (communication vs. event log) but are rendered identically.
→ **Fix:** Add a semi-transparent dark background panel behind team chat messages, positioned consistently in the right-center area. Use a distinct text color or icon treatment to differentiate chat messages from kill feed events (e.g., cyan for team chat, white for kill feed). Add a visible left-edge accent bar or speech-bubble container to reinforce that these are player communications.

### 17. No Visual Indication of Health Severity State
**Principle Violated:** Feedback and Dialog (1.7), Color Guidelines (3.1)
The health bar shows "80 HEALTH" with a teal-blue fill at roughly 80% capacity. This is a relatively healthy state, but the bar uses the same color it would presumably use at 100%. Effective game HUDs use progressive color changes (green → yellow → orange → red) to communicate health urgency at a glance without requiring the player to read the number. The current single-color bar forces players to read the "80" text to assess their status. The HIG states that "immediate feedback confirms every action" — taking damage should produce immediately visible feedback in the health display.
→ **Fix:** Implement a color gradient system: green/teal at 100–60%, yellow/amber at 60–30%, red/crimson below 30%. Add a subtle pulse or glow animation when health drops below critical thresholds. Consider a screen-edge vignette effect at low health to provide ambient feedback that doesn't require looking at the health bar at all.

---

## Strengths

- **Clear crosshair placement and visibility:** The cyan crosshair at screen center is well-positioned, symmetrical, and uses a color that contrasts well with the current dark scene. Its cross-hair design with a center dot provides precise aiming reference.
- **Objective callout is prominently positioned:** The ">> SECURE OBJECTIVE B" box is placed at the top-right where players naturally look for mission context. The amber border and ">>" chevrons draw attention effectively despite the font legibility issues noted above.
- **Kill feed location follows FPS conventions:** The top-center kill feed placement matches the established convention from major FPS titles, meaning experienced players will instinctively know where to look for elimination events.
- **Minimap positioned in standard bottom-left location:** The minimap's placement follows the dominant FPS convention, leveraging players' existing spatial memory from other games. Its circular compass marker provides basic orientation.
- **Health and ammo information are persistently visible:** Both critical combat stats (health and ammunition) are always on screen rather than hidden behind menus, supporting the WYSIWYG principle that "what's on screen is what exists."
- **Dark overall theme minimizes viewport obstruction:** The near-black background and dark-toned HUD elements keep the UI visually recessive during gameplay, allowing the game scene to dominate the player's attention where it matters most.

---

## Overall Score: 3/10

The neon_strike HUD screen establishes the basic skeleton of an FPS heads-up display — compass, minimap, health, ammo, kill feed, and objective are all present and positioned in roughly conventional locations. However, the visual execution severely undermines the HUD's ability to communicate critical gameplay information quickly and clearly. The most damaging problems are the unlabeled 45% bar that provides data without meaning, the dangerously low-contrast minimap that fails its core navigational purpose, the indistinguishable killstreak icons, and the red-on-green colorblind accessibility failure. Beyond these functional issues, the fragmented color palette (cyan, amber, teal-blue, olive-green, red) lacks any systematic logic, the typography mixes decorative stencil fonts with utilitarian text at wildly inconsistent scales, and related information is scattered rather than spatially grouped. The cryptic "7B Pop <192>" label and context-free "(U)" icon suggest a UI designed by engineers for themselves rather than for players under time pressure. To achieve a functional game HUD, the design needs a unified color system with clear semantic assignments, consistent typography at readable sizes, spatial grouping of related information, labels on every status indicator, and an accessibility pass to ensure color is never the sole distinguishing factor.
