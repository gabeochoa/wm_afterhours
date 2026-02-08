# Design Audit: Race Results Screen

**Audit Date:** 2026-02-07
**Screen:** `race_results` — Post-Race Results and Standings Display
**Methodology:** Apple Human Interface Guidelines (1987) — Full Audit
**Auditor:** Automated Design Audit

---

## Screen Description

A full-screen post-race results UI on a near-black background with a decorative checkered-flag border spanning the top edge. The upper-left displays "RACE RESULTS" in an italic gold serif-style font with a metallic sheen. The upper-right shows contextual breadcrumb text: "Rainbow Circuit | Mushroom Cup – Race 3/4" in a light gray sans-serif. The main content area is divided into a large left panel and two stacked right panels. The left panel contains an eight-row results table with columns for placement (#), RACER name, TIME, and PTS (points). The player's row (2nd place, labeled "YOU") is highlighted with a blue background fill. Placement ordinals (1st–8th) are rendered in gold, racer names in white, times in white, and points in green with a "+" prefix. Column headers (#, RACER, TIME, PTS) use small, muted gray uppercase text. The upper-right panel displays "CUP STANDINGS" in bold white uppercase, listing the top four drivers with cumulative points. The lower-right panel shows "BEST LAP" in muted uppercase with a large green time value "0:48.220" and a small "New Personal Best!" annotation. The bottom bar contains three action buttons: "Replay" (green dashed outline, left), "Quit Cup" (red/salmon filled, center-left), and "Next Race >>" (green solid outline, far right), with controller hints "A: Select  B: Back  X: Replay" centered in very small text.

---

## Critical Issues (Fix Immediately)

### 1. Points Column Uses Color as the Sole Value Indicator
**Principle Violated:** Color Guidelines (3.1), Accessibility (8.1)
The PTS column renders all point values in green text with a "+" prefix (+15, +12, +10, etc.). Green is the only visual cue distinguishing these as positive point gains — there is no shape, icon, background treatment, or typographic variation to reinforce the meaning. For users with deuteranopia or protanopia (red-green color blindness), the green point values against the dark navy background may be difficult to distinguish from the white time values in the adjacent column. The HIG explicitly states that "color coding should be redundant with shape, position, or pattern" and that designs must "work in black and white first." In grayscale, the green points would appear nearly identical to the white times, eliminating the visual separation between columns.
→ **Fix:** Add a secondary visual indicator beyond color — such as a subtle background pill/badge behind each point value, a small upward arrow icon, or bold weight differentiation. Ensure the points column is distinguishable from the time column through position, typography weight, or iconography, not color alone.

### 2. Player Row Highlight Has Insufficient Text-to-Background Contrast
**Principle Violated:** Color Guidelines (3.2), Accessibility (8.1)
The player's row (2nd place) uses a medium-blue background fill to highlight "YOU" as the player's result. The text within this row — particularly the yellow-gold "2nd" ordinal and the white "YOU" label — sits against this blue fill. While the white "YOU" text achieves acceptable contrast, the gold ordinal "2nd" against the mid-blue background creates a yellow-on-blue combination that may strain readability, especially at typical TV viewing distances (6–10 feet). More critically, the blue row highlight is the sole mechanism distinguishing the player's result from the seven AI racers. The HIG warns that "text and thin lines must have sufficient contrast to be easily visible" and that "color should not be the only way to distinguish elements."
→ **Fix:** Increase the brightness of the player row highlight or add a secondary indicator — a left-edge accent bar, a small player icon, or a border outline — to reinforce player identification beyond the blue fill. Ensure all text within the highlighted row meets a minimum 4.5:1 contrast ratio against the blue background.

### 3. Three Distinct Button Styles Create Visual Chaos
**Principle Violated:** Consistency (1.4), Aesthetic Integrity (1.10)
The three bottom action buttons each use a completely different visual treatment: "Replay" has a yellow-green dashed/dotted outline with white text on a dark fill; "Quit Cup" has a filled salmon/red background with white text and no border emphasis; "Next Race >>" has a solid green border with white text on a dark fill. These three buttons perform comparable navigation actions but look nothing alike. The inconsistency forces users to decode each button's visual language independently rather than recognizing a unified "button" pattern. The HIG requires that "visual styles are consistent throughout" and that "same action produces same result" — while these are different actions, they are the same control type (button) and should share a consistent base styling with clear primary/secondary/destructive differentiation.
→ **Fix:** Establish a single button style system. All three buttons should share the same base shape, border treatment, and text styling. Differentiate by color semantics only: green fill or accent for the primary action ("Next Race"), neutral/gray for secondary ("Replay"), and red accent for destructive ("Quit Cup"). Each button should be immediately recognizable as a button through shared structural language.

### 4. "Quit Cup" Destructive Action Lacks Warning Affordance
**Principle Violated:** Forgiveness (1.8), User Control (1.6), Feedback and Dialog (1.7)
"Quit Cup" is a destructive, potentially irreversible action — it would abandon the player's cup progress (currently leading with 38 points in Race 3 of 4). Despite this severity, the button sits casually in the bottom bar with no additional warning, confirmation gate, or visual gravitas beyond a muted red/salmon fill. Its color treatment is actually less eye-catching than the green "Next Race" and "Replay" buttons. The HIG mandates that "users are warned before irreversible actions" and that "risky actions show warnings but let the user proceed if confirmed." A single tap on "Quit Cup" with no confirmation dialog would violate user trust and the principle of forgiveness.
→ **Fix:** Ensure "Quit Cup" triggers a confirmation dialog ("Are you sure you want to quit? You are currently in 1st place with 1 race remaining." with "Keep Racing" as default and "Quit Cup" as the destructive option). Visually, make the destructive button less prominent than the safe options — smaller or more subdued — so it is discoverable but not accidentally invoked. Never let a destructive action fire without confirmation.

---

## Major Issues (Fix Soon)

### 5. Column Header Typography Is Nearly Invisible
**Principle Violated:** See-and-Point (1.3), Aesthetic Integrity (1.10), Accessibility (8.1)
The results table column headers (#, RACER, TIME, PTS) are rendered in very small, low-contrast muted gray text against the dark navy panel background. These headers serve a critical organizational function — they tell users what each column represents — yet they are the least visible text elements on the entire screen. The "PTS" abbreviation is particularly problematic because without a clearly readable header, users must infer that the green numbers represent points. The HIG states that "all available actions are visible" and that the interface should support "recognition over recall." Invisible headers force recall.
→ **Fix:** Increase the column header text size to at least match the body row text size. Increase the contrast by using a brighter gray or white with reduced opacity. Consider adding a subtle underline or bottom border to the header row to visually separate it from the data rows and reinforce its structural role.

### 6. Title Typography Sacrifices Readability for Decoration
**Principle Violated:** Aesthetic Integrity (1.10), Consistency (1.4)
"RACE RESULTS" uses a heavy italic serif font with a gold metallic gradient effect, identical to the kart select screen's "SELECT YOUR RACER" title treatment. While this maintains thematic consistency with the racing game's brand, it creates a stark typographic disconnect from every other text element on screen — the column headers, racer names, standing entries, button labels, and hints all use clean sans-serif fonts. The italic serif title is purely decorative and harder to read than the functional sans-serif text. The HIG warns against "decoration that distracts from function" and requires "different things look different" — here, the title looks so different from functional text that it reads more as a logo or illustration than a navigational heading.
→ **Fix:** Simplify the title to a bold sans-serif that harmonizes with the rest of the UI, or reduce the decorative treatment (remove the metallic gradient, reduce italic angle). If the stylized treatment is essential for brand identity, limit it to this one element and ensure the visual gap between decorative and functional typography is less extreme.

### 7. Massive Empty Space in the Lower-Center Creates Dead Zone
**Principle Violated:** Aesthetic Integrity (1.10), Perceived Stability (1.9)
The results table occupies approximately 60% of the left panel's vertical height, leaving a large block of empty dark space below the 8th-place row. Similarly, the right side has a gap between the cup standings panel and the best lap panel. This dead zone — roughly 30% of the total screen area — contains no content, no decorative treatment, and no visual anchoring. The emptiness makes the layout feel unfinished and the content feel like it has drifted upward. In a game context where players are excited about their results, wasted space represents missed opportunities to display additional engaging information (lap-by-lap breakdown, position history graph, personal stats).
→ **Fix:** Either compress the layout to eliminate the dead zone (reduce panel heights to match content, bring buttons closer to the table), or fill the space with additional relevant content — a mini lap-time chart, a "race highlights" summary, or a visual graphic of the track. If the space must remain empty for layout rhythm, add a subtle visual anchor (a horizontal rule, a faint track silhouette) to make it feel intentional.

### 8. Cup Standings Point Values Lack Visual Hierarchy Relative to Position
**Principle Violated:** Aesthetic Integrity (1.10), Feedback and Dialog (1.7)
The Cup Standings panel lists four drivers with their cumulative points (38, 42, 30, 25 pts), all rendered in the same white text at the same size and weight. Despite the player ("YOU") leading the championship at 38 points and this being Race 3 of 4 — a tense competitive moment — there is no visual differentiation between 1st place and 4th place in the standings. Points gaps are not visualized. The leader is not visually emphasized. The HIG requires a "clear visual hierarchy — most important elements stand out," and in a racing game's championship context, the standings hierarchy is extremely important to player engagement.
→ **Fix:** Visually differentiate standings positions: bold or highlight the leader, use decreasing text weight or subtle color dimming for lower positions, and consider showing the points gap (e.g., "+4 ahead" or "-8 behind"). Add a small indicator (crown icon, colored accent) for 1st place to reinforce the competitive hierarchy.

### 9. "PTS" Abbreviation Violates Plain Language Principles
**Principle Violated:** Plain Language (7.1), See-and-Point (1.3)
The column header "PTS" is a three-letter abbreviation for "Points." While common in sports contexts, the HIG explicitly warns against "cryptic abbreviations or codes" and instructs designers to use "user vocabulary, not programmer vocabulary." The column has ample horizontal space to display the full word "POINTS" — the abbreviation saves only a few pixels. Combined with the near-invisible header text (Issue #5), "PTS" creates an unnecessary comprehension barrier, particularly for younger players or non-native English speakers who are the target audience for a kart-style racing game.
→ **Fix:** Replace "PTS" with "POINTS." The column width easily accommodates the full word. Ensure all headers use full words where space permits. If abbreviation is truly necessary, use a universally understood form and ensure the header is prominently displayed.

### 10. Controller Hint Text Is Illegibly Small
**Principle Violated:** See-and-Point (1.3), Accessibility (8.1, 8.3), Plain Language (7.1)
The controller hints at the bottom center — "A: Select  B: Back  X: Replay" — are rendered in an extremely small font size that is barely readable even in the screenshot. On a television screen viewed from a typical couch distance (6–10 feet), this text would be virtually invisible. These hints serve a critical discoverability function: they tell the player that X triggers a replay (a non-obvious shortcut), that B goes back, and that A selects. Without readable hints, the player must either guess at controls or rely on muscle memory. The HIG requires that "keyboard shortcuts are alternatives, never the only way" and that all actions be discoverable by "scanning."
→ **Fix:** Increase the hint text size significantly (at minimum 2× current size). Use controller button icons (colored circles or shapes matching the gamepad) alongside text labels. Place the hints in a dedicated, visually distinct footer strip rather than floating as near-invisible fine print.

### 11. Best Lap Panel Buries a Celebratory Moment
**Principle Violated:** Feedback and Dialog (1.7), Aesthetic Integrity (1.10)
The "BEST LAP" panel in the lower-right shows "0:48.220" in green with a small "New Personal Best!" annotation beneath it. Achieving a new personal best lap time is a significant player accomplishment — one of the most emotionally rewarding moments in a racing game — yet the panel is small, tucked into a corner, and visually understated. The "New Personal Best!" text is rendered in an extremely small font that is easy to overlook entirely. The HIG states that "immediate feedback confirms every action" and that feedback should match the significance of the event. A personal best deserves prominent, celebratory feedback — not a footnote.
→ **Fix:** When a new personal best is achieved, increase the visual prominence of this panel: use a larger font for "New Personal Best!", add a subtle glow, shimmer, or color pulse animation, or display a small trophy/star icon. Make the celebration proportional to the achievement. If no personal best was set, the panel can remain understated.

---

## Minor Issues (Consider Fixing)

### 12. Checkered Flag Border Is Purely Decorative Visual Noise
**Principle Violated:** Aesthetic Integrity (1.10), Color Guidelines (3.2)
The top edge of the screen features a checkered-flag pattern border running the full width. While thematically appropriate for a racing game, this pattern adds visual noise without conveying any information. The high-contrast black-and-white checkerboard pattern is one of the most visually active patterns possible, and placing it at the screen's top edge means it competes for attention with the title and contextual text immediately below. The HIG warns against "decoration that distracts from function" and states that "graphics should support understanding, not just decoration."
→ **Fix:** Reduce the visual weight of the checkered border — fade it to a lower opacity, make it a subtle emboss rather than full-contrast, or limit it to a thin strip rather than a full band. Alternatively, remove it entirely and let the title treatment alone carry the racing theme.

### 13. Inconsistent Ordinal Styling Creates Subtle Hierarchy Confusion
**Principle Violated:** Consistency (1.4), Aesthetic Integrity (1.10)
The placement column uses ordinal suffixes (1st, 2nd, 3rd, 4th, etc.) with the number and suffix rendered as a single text element in gold/yellow. However, the visual treatment does not differentiate between podium positions (1st–3rd) and non-podium positions (4th–8th) in any way beyond the text content itself. In racing culture, the top three positions carry special significance (podium finishers). The flat, identical treatment of all eight positions flattens the emotional hierarchy of the results and misses an opportunity to celebrate the player's 2nd-place podium finish.
→ **Fix:** Add visual differentiation for podium positions: a subtle gold/silver/bronze color or icon for 1st/2nd/3rd, or a slightly larger font size. Positions 4th–8th can maintain the current neutral treatment. This reinforces the natural hierarchy players expect from racing results.

### 14. "Next Race >>" Uses Non-Standard Directional Indicator
**Principle Violated:** Consistency (1.4), Plain Language (7.1)
The "Next Race >>" button appends two greater-than signs as a directional indicator. This is a non-standard typographic convention — the standard approach in game UIs is either a right-pointing arrow icon (→) or a chevron (›). The double greater-than signs ">>" carry programmer connotations (bitwise shift, output redirection) rather than user-facing navigation semantics. The HIG requires "user vocabulary, not programmer vocabulary" and consistent visual conventions.
→ **Fix:** Replace ">>" with a proper right-arrow icon (→) or a single chevron (›) that visually communicates forward navigation. Alternatively, remove the directional indicator entirely — the button label "Next Race" is self-explanatory and the right-side placement already implies forward progression.

### 15. No Visual Connection Between Results Table and Cup Standings
**Principle Violated:** Feedback and Dialog (1.7), WYSIWYG (1.5)
The results table (left panel) and cup standings (right panel) display related but disconnected information. The race results show that the player finished 2nd and earned +12 points, while the cup standings show the player has 38 total points. However, there is no visual connection between these two data sets — no animation showing points being added, no highlighting of which standings changed, no indication of whether the player gained or lost positions in the championship after this race. The user must mentally calculate and cross-reference: "I had 26 points, earned 12, now I have 38." The HIG states "no secrets — what's on screen is what exists" — but the relationship between race results and standings updates is hidden.
→ **Fix:** Add visual cues connecting the results to the standings: show a "+12" annotation next to the player's standings entry, highlight drivers whose standings positions changed, or use a brief animation showing points accumulating. This makes the cause-and-effect relationship between race performance and championship standings explicit.

### 16. Player Label "YOU" Is Impersonal and Inconsistent with Other Racers
**Principle Violated:** Consistency (1.4), Plain Language (7.1), Metaphors (1.1)
The player's racer name is displayed as "YOU" in the results table and cup standings, while all AI racers have unique character names (Zippy, Blaze, Drift, etc.). This creates a tonal inconsistency — seven entries read like a roster of named characters, then one entry breaks the pattern with a generic pronoun. In a game where the player presumably selected a character on the kart select screen (e.g., "Drift"), displaying "YOU" instead of the chosen character name removes the player's connection to their selected avatar. The HIG metaphor principle suggests maintaining the fiction of the game world.
→ **Fix:** Display the player's selected character name (e.g., "Drift") with a secondary indicator that this is the player — such as a small "P1" badge, a star icon, or the blue highlight alone. This maintains naming consistency across all eight entries while still clearly identifying the player's row.

### 17. Race Context Breadcrumb Uses Thin Low-Contrast Separator
**Principle Violated:** Aesthetic Integrity (1.10), Accessibility (8.1)
The contextual header "Rainbow Circuit | Mushroom Cup – Race 3/4" uses a thin pipe character (|) and an en-dash (–) as separators between three pieces of information: track name, cup name, and race number. These thin punctuation marks are low-contrast against the dark background and create a visual hierarchy that is flat — all three pieces of information appear at the same level of importance despite "Race 3/4" being arguably the most critical context (how far through the cup is the player?). The small font size compounds the readability issue.
→ **Fix:** Increase the font size of the breadcrumb or use visual separation beyond punctuation — such as distinct colored segments, bullet icons, or spatial grouping. Consider emphasizing "Race 3/4" with a bolder weight or different color to foreground the progression context.

---

## Strengths

- **Clear results table structure communicates placement effectively:** The left-aligned table with placement, name, time, and points columns follows the natural reading pattern for race results. Users can immediately scan down the # column to find their position and across the row to see their time and points.
- **Player row highlighting provides immediate identification:** The blue background fill on the player's row ensures users can instantly locate their own result among eight entries without scanning every name — a strong application of the feedback principle despite the color-only limitation.
- **Contextual breadcrumb provides race series awareness:** The "Rainbow Circuit | Mushroom Cup – Race 3/4" text gives players immediate context about where they are in the championship progression, supporting situational awareness without requiring memorization.
- **Three-action button bar supports all logical next steps:** The bottom bar offers Replay (try again), Quit Cup (exit), and Next Race (continue) — covering every reasonable player intent after viewing results. This supports user control by not forcing a single path.
- **Cup standings provide competitive context alongside individual results:** Showing the championship standings next to the race results lets players immediately understand the implications of their finish — whether they gained or lost ground in the cup — without requiring a separate screen.
- **Best lap recognition rewards incremental skill improvement:** The "New Personal Best!" annotation in the best lap panel acknowledges player improvement even when they didn't win the race, providing positive reinforcement and encouraging continued play.

---

## Overall Score: 5/10

The race results screen establishes a solid informational foundation — the table-based results layout, championship standings sidebar, and best lap panel collectively present all the data a player needs to understand their race outcome and competitive position. The three-action button bar appropriately covers all logical next steps. However, the visual execution undermines the information architecture through several compounding issues. The most critical problems are the inconsistent button styling (three completely different visual treatments for the same control type), the reliance on color alone for the points column and player identification, and the absence of a confirmation gate on the destructive "Quit Cup" action. The typography is fragmented between the decorative serif title and functional sans-serif body text, column headers are nearly invisible, and the controller hints are illegibly small at TV viewing distances. The layout wastes significant screen real estate in the lower-center dead zone while simultaneously underplaying a celebratory moment (personal best lap). The cup standings, while well-positioned, fail to visualize the competitive hierarchy or connect back to the race results that produced them. Resolving the button consistency problem, adding non-color redundancy to the points and player identification systems, and gating the destructive quit action with a confirmation dialog would produce the most significant usability and trust improvements.
