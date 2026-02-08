# Design Audit: cozy_cafe

**Audit Date:** 2026-02-07
**Guidelines Used:** Apple Human Interface Guidelines (1987 Classic)
**Screen:** Main gameplay dashboard for "Blossom Cafe" — a cozy cafe management game UI

---

## Critical Issues (Fix Immediately)

### 1. Patience Bars Use Color as the Sole State Indicator (HIG §3.1, §8.1)
The customer patience indicators (the small green progress bars next to "Patience:") rely entirely on color fill to communicate remaining patience. A filled green bar means the customer is patient; an empty bar means they are not. There is no pattern, shape, label percentage, or secondary encoding. Colorblind users (particularly those with red-green deficiency) cannot distinguish a full green bar from an empty one at a glance, and the tiny size compounds the problem.

**Recommendation:** Add a numeric percentage label overlaid on or beside the bar, use a striped or hatched pattern for the filled portion, or change the shape of the bar itself as patience drains (e.g., segmented blocks that disappear). The "60%" and "0%" text is present but rendered at an extremely small size inside a tiny colored badge — it needs to be larger and placed outside the badge for legibility.

### 2. Patience Badge Text Is Nearly Illegible (HIG §3.2, §8.1)
The percentage labels ("60%" and "0%") inside the small patience badges are rendered in a very small font on a green or beige background. At normal viewing distance, these are extremely difficult to read. The badge is roughly 30×15 pixels of visual space, and the text inside competes with the background color. This fails the HIG requirement that text and thin lines have sufficient contrast to be easily visible.

**Recommendation:** Increase the badge size or move the percentage text outside the badge entirely. Use a minimum font size that remains legible at the screen's intended viewing distance. Ensure a contrast ratio of at least 4.5:1 between the text and its background.

### 3. Time Remaining Badges Lack Sufficient Contrast (HIG §3.2, §8.1)
The "2m" and "5m" time badges next to each customer use dark text on small colored backgrounds (green and pink respectively). The "5m" badge uses dark text on a desaturated pink/rose background, and at this small size the contrast is marginal. These badges convey critical gameplay information (time until customer leaves) but are among the hardest elements to read on the entire screen.

**Recommendation:** Increase badge size, use higher-contrast color pairings (e.g., white text on a dark background, or dark text on a light background with a strong border), and ensure the font size meets minimum legibility standards.

### 4. Rating Stars Are Unreadable at Current Size (HIG §3.2, §1.10)
The star rating display in the top header shows what appears to be a 4/5 star rating with a "Customers Served: 25" subtitle, but the entire rating area is rendered at such a small scale that individual stars are barely distinguishable from one another. The "Customers Served: 25" text is extremely small and uses low-contrast brown text on a light tan/cream background. This information represents key player progress metrics but is effectively invisible.

**Recommendation:** Enlarge the rating display substantially. Use clearly defined, larger star icons with a visible filled/unfilled distinction. Increase the "Customers Served" text to a readable body-text size and improve its contrast.

---

## Major Issues (Fix Soon)

### 5. Inconsistent Panel Header Styling Creates Visual Fragmentation (HIG §1.4, §1.10)
The left panel ("Today's Specials") uses a dark brown header with white text, while the right panel ("Customers") uses a sage/muted green header with white text. These are sibling panels at the same hierarchical level, yet they use completely different header colors. This inconsistency violates the HIG principle that identical structural elements should share identical styling. Users may interpret the different colors as indicating different importance levels or different types of content.

**Recommendation:** Use a single, consistent header color for all top-level panel headers. If color differentiation is desired for thematic reasons, use subtle shade variations of the same hue rather than entirely different hues (brown vs. green).

### 6. Three Distinct Button Styles on One Screen (HIG §1.4, §2.7)
The screen uses at least three visually distinct button treatments:
- **Colored rounded bars** (Lavender Latte, Honey Toast, Matcha Cake) — large, rounded, pastel-filled with dark outlined text
- **Outlined flat button** (Promote Special) — white/cream fill with a dark border and dark text, plus an icon
- **Large pink filled button** (Serve Next) — wide, prominent pink fill with white text
- **Icon buttons with labels** (Inventory, Research, Crafting) — small circular icons with text beneath and red notification badges

Users must learn four different visual languages for interactive elements on a single screen. The HIG requires consistent use of standard controls.

**Recommendation:** Establish a clear button hierarchy (primary, secondary, tertiary) and apply it consistently. All primary actions should share one style, all secondary actions another. The bottom navigation icons are acceptable as a distinct pattern since they serve as persistent navigation.

### 7. Menu Item Color Coding Lacks Redundancy (HIG §3.1, §8.1)
The three specials — Lavender Latte (pink), Honey Toast (green), Matcha Cake (tan/beige) — are differentiated primarily by their background color. While they do have text labels, the color is the dominant visual signal and carries implicit meaning (possibly representing drink categories or rarity). A colorblind user would see three similarly-valued neutral bars with text, losing whatever the color coding communicates.

**Recommendation:** Add a secondary visual indicator to reinforce what the colors represent: an icon for each category (cup for drinks, plate for food, cake for desserts), a pattern, or a small category label. Color should supplement meaning, never carry it alone.

### 8. Chat/Message Area Has No Visual Hierarchy or Interaction Affordance (HIG §1.10, §1.3)
The bottom-left area shows what appears to be a chat or notification log ("Guildmate23: need help with the recipe?" and "DevTeam_Support: Check out the new update!") but it has minimal visual styling — just text with small colored avatar squares, no borders, no scrollbar, no clear indication that it is interactive or scrollable. It blends into the background and competes with the music slider above it. Users may not realize this area exists or that they can interact with it.

**Recommendation:** Give the chat area a distinct container with a visible border or subtle background fill, add a scrollbar or "scroll to see more" indicator, and provide visual affordances for interaction (hover states, click targets). Consider adding a header label such as "Messages" or "Chat."

### 9. Flat Visual Hierarchy in Top Header Bar (HIG §1.10)
The top header packs four distinct pieces of information into a single row — cafe name ("Blossom Cafe"), currency ("Gold: $1347"), star rating, and customers served — all at roughly similar visual weight. The cafe name uses a serif/decorative font at a moderate size, but the currency and rating displays use small text in bordered pills that blend together. The most important element (likely the cafe name or the currency) does not stand out sufficiently from the secondary information.

**Recommendation:** Establish a clear hierarchy: make the cafe name the largest and most prominent element, use a secondary tier for currency (which is actively changing), and a tertiary tier for cumulative stats like rating and customers served. Use size, weight, and spacing to differentiate these tiers.

### 10. Music Slider Lacks Standard Affordances (HIG §2.2, §1.7)
The music volume slider at the bottom-left uses a dark olive track on the left side and a lighter muted-green track on the right, with a round teal thumb. While functional, it lacks a label icon (no speaker/volume icon), has no min/max indicators, and its filled-track color (dark olive) is very similar to the surrounding brown/tan palette, making it hard to distinguish at a glance. The "Music: 70%" text below is small and separated from the slider, weakening the association.

**Recommendation:** Add a speaker icon at the left end and a max-volume icon at the right end. Move the "Music: 70%" label closer to or directly above the slider. Use a more distinct fill color for the active portion of the track so it stands out from the muted background palette.

---

## Minor Issues (Consider Fixing)

### 11. Inconsistent Spacing Between Customer Entries (HIG §1.4, §1.9)
The two customer entries (Emma and Oliver) appear to have generous vertical spacing between them, but the spacing between the last customer entry and the "Serve Next" button is much larger. This asymmetry makes the Customers panel feel unbalanced. Additionally, the internal padding within each customer row is not consistent — the patience bar and time badge are pushed to the right edge while the name and order are left-aligned, creating a wide gap of empty space in the middle.

**Recommendation:** Use consistent vertical spacing between all rows in the customer list and between the last row and the action button. Consider a table-like layout with aligned columns for name, order, patience, and time.

### 12. "Promote Special" Button Icon Is Ambiguous (HIG §1.3, §8.3)
The small green icon to the right of "Promote Special" appears to be a tiny flag or banner, but at its current size it is difficult to identify. The HIG requires that icons have obvious meanings. Users may not understand what "promoting" a special does, and the icon does not clarify it.

**Recommendation:** Use a larger, more recognizable icon (e.g., a megaphone, a star badge, or a spotlight) and consider adding a tooltip or brief description explaining what promotion does. If the icon represents currency cost, make that explicit.

### 13. Decorative Serif Font on Headers Reduces Scanability (HIG §1.10, §7.1)
The "Today's Specials" and "Customers" headers, as well as the menu item text (Lavender Latte, Honey Toast, etc.), use a decorative serif or hand-drawn style font. While this fits the cozy game aesthetic, it reduces scanability compared to a clean sans-serif, especially at the sizes used for the menu items. Users must parse letter forms that are less standardized.

**Recommendation:** Reserve decorative type for the cafe name and branding elements. Use a clean, high-legibility font for functional headers and interactive text labels. If the decorative font is retained, ensure it is rendered at a large enough size with sufficient weight to remain instantly readable.

### 14. Notification Badges on Bottom Icons Are Small and Inconsistent in Meaning (HIG §1.7, §3.2)
The red notification badges on "Inventory" (showing "2") and "Research" (showing "1") are very small red circles with white numbers. At the screen's viewing distance, these are difficult to read. Additionally, the "Research" label beneath its icon shows "Ready!" in small text, which is a different style of notification from the numeric badges — creating inconsistency in how notifications are communicated.

**Recommendation:** Increase badge size for legibility. Standardize the notification pattern: either use numeric badges for all sections or use text labels for all. If "Ready!" is a different type of notification than "2 new items," consider using a different visual treatment (e.g., a pulsing highlight vs. a numeric badge) but document the distinction clearly.

### 15. Panel Width Imbalance Creates Asymmetric Layout (HIG §1.10, §1.9)
The left panel (Today's Specials) is slightly narrower than the right panel (Customers), yet the left panel contains more content (three menu items, a promote button, a slider, and a chat area). The right panel has only two customer entries and one large button but occupies equal or greater space. This creates a cramped feeling on the left and a sparse feeling on the right, disrupting visual balance.

**Recommendation:** Either equalize panel widths and redistribute content, or intentionally size panels proportionally to their content density. The left panel could benefit from more horizontal space, or the chat area could be moved to a dedicated drawer/panel to reduce overcrowding.

### 16. "Serve Next" Button Dominates Without Clear Priority Justification (HIG §1.10, §2.7)
The "Serve Next" button is the largest, most visually prominent element on the entire screen — large pink fill, large white text, spanning the full width of the Customers panel. While it may be the primary action, it visually overwhelms everything else, including the customer information that the user needs to read before deciding to serve. The visual weight ratio between this button and the customer entries is heavily skewed.

**Recommendation:** Reduce the button's visual dominance slightly — either reduce its height, use a less saturated pink, or add more visual weight to the customer entries (larger text, clearer layout). The primary action should be prominent but not overwhelming.

### 17. No Clear Separator Between Functional Zones in Left Panel (HIG §1.10, §2.3)
The left panel contains three distinct functional zones: the specials menu (top), the promote button (middle), and the music/chat area (bottom). There are no dividers, spacing breaks, or background color changes to separate these zones. The music slider and chat area, in particular, blend together and feel disconnected from the panel's primary purpose (managing specials).

**Recommendation:** Add subtle horizontal dividers or increased spacing between functional zones. Consider using a slightly different background tint for the utility area (slider + chat) vs. the menu area to visually separate concerns.

---

## Strengths

- **Strong thematic coherence:** The warm beige/cream/brown/sage palette with rounded shapes and soft borders creates a cohesive "cozy cafe" atmosphere that reinforces the game's theme. The metaphor is clear and inviting.
- **Clear labeling on primary elements:** Customer names, orders, menu item names, and the cafe title all use plain, user-friendly language. There is no jargon or technical terminology — "Lavender Latte," "Honey Toast," and "Serve Next" are immediately understandable.
- **Good use of real-world metaphor (HIG §1.1):** The cafe management concept maps naturally to real-world experience. Users understand specials, customers, patience, and serving without explanation. The gold currency, star rating, and inventory concepts are all grounded in familiar game and real-world metaphors.
- **Music volume control is present and adjustable (HIG §4.2):** The user has direct control over audio volume with a visible slider and numeric percentage, satisfying the HIG requirement that users can adjust or turn off sound.
- **Notification badges alert users to pending items:** The red badges on Inventory and Research provide at-a-glance awareness of items requiring attention, following the principle of keeping users informed.
- **Customer information is contextually relevant:** Showing the customer name, their order, patience level, and time remaining in a single row gives the user all the information needed to make a serving decision, supporting WYSIWYG principles.

---

## Overall Score: 5/10

The cozy_cafe screen establishes an appealing, thematically coherent visual identity with warm colors and a clear real-world metaphor. The plain language, logical layout of two main panels, and contextual customer information are well-executed. However, the screen suffers from significant consistency violations (three-plus button styles, mismatched panel headers), accessibility failures (color-only patience indicators, illegible small text on badges and ratings), and visual hierarchy problems (flat header bar, oversized serve button, undifferentiated content zones). The left panel is overcrowded with functionally unrelated elements (menu items, promotion, music slider, chat) while the right panel has excess whitespace. Addressing the critical contrast and accessibility issues, establishing consistent component styling, and improving the spatial organization would bring this screen substantially closer to HIG compliance while preserving its charming cozy aesthetic.
