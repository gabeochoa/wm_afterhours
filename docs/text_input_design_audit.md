# Design Audit: Text Input Demo

**Audited Screen:** Text Input Demo
**Date:** 2026-02-07
**Guidelines Used:** Apple Human Interface Guidelines (1987)

---

## Critical Issues (Fix Immediately)

### 1. Extremely Low Contrast Between Input Fields and Background
**HIG Violation:** Category 3 — Color Usage Rules (3.2): "Text and thin lines have sufficient contrast to be easily visible"; "Small objects use high-contrast colors."
**Problem:** The dark purple input fields (#4a3a6a-range) sit on a near-black navy background. The tonal difference is minimal, making input areas difficult to distinguish from the surrounding panel. Users must strain to identify where to type.
**Recommendation:** Use a noticeably lighter or distinctly different fill for input fields — a light neutral (gray, off-white) or at minimum increase the brightness contrast by 3:1 against the container background.

### 2. Submit Button Uses Red/Danger Color for a Primary Action
**HIG Violation:** Category 1.4 — Consistency; Category 3.1 — Color Fundamentals: "Color coding is redundant with shape/position/pattern."
**Problem:** The "Submit" button is styled in red/pink, a color universally associated with danger, destruction, or cancellation. Using it for the primary affirmative action ("Submit") sends a conflicting signal and contradicts established color conventions across platforms.
**Recommendation:** Use a neutral or affirmative color (blue, green, or the existing purple theme) for the primary submit action. Reserve red for destructive operations like "Delete" or "Cancel."

### 3. Inconsistent Input Field Border Styling Across Fields
**HIG Violation:** Category 1.4 — Consistency: "Visual styles consistent throughout"; "Same action produces same result throughout the application."
**Problem:** The Username field has a white dashed/dotted border (likely indicating focus), while the Email and Password fields have no such border and instead show a thin colored underline or edge. The Search field has yet another appearance. Three different visual treatments for the same component type (text input) within a single form.
**Recommendation:** Establish one canonical text input style with consistent border, fill, and focus states applied uniformly to all fields.

### 4. No Clear Focus State Differentiation
**HIG Violation:** Category 1.7 — Feedback and Dialog: "Immediate feedback confirms every action"; Category 5.1 — Selection: "Visual feedback shows what's selected."
**Problem:** Only the Username field appears to have a visible focus indicator (dashed white border). The other fields lack any clear visual change between focused and unfocused states. Users navigating via Tab/keyboard cannot confidently tell which field is active.
**Recommendation:** Define a single, prominent focus ring or border highlight that applies consistently to whichever field is currently active. Ensure the contrast ratio of the focus indicator against surrounding elements is at least 3:1.

---

## Major Issues (Fix Soon)

### 5. Center-Aligned Labels Reduce Scannability
**HIG Violation:** Category 1.10 — Aesthetic Integrity: "Clear visual hierarchy—most important elements stand out"; Category 2.5 — Dialog Boxes: "Most important info at top-left."
**Problem:** All labels ("Username:", "Email:", "Password:", "Search (no label version):") are center-aligned above their respective inputs. This forces the user's eye to hunt for each label rather than scanning a clean left edge. Left-aligned labels create a predictable vertical axis that accelerates form comprehension.
**Recommendation:** Left-align all labels to the leading edge of their corresponding input fields.

### 6. Meta-Label Leaks Implementation Detail into the UI
**HIG Violation:** Category 7.1 — Plain Language: "User vocabulary, not programmer vocabulary"; "No jargon or technical terms."
**Problem:** The label "Search (no label version):" is developer-facing commentary, not user-facing text. It describes the component variant rather than its function. Real users would find this confusing or meaningless.
**Recommendation:** For a showcase demo, use a descriptive comment or annotation outside the rendered UI. Within the UI itself, either show a proper label or show only the placeholder — do not mix meta-documentation into live labels.

### 7. "Show" Button Visually Inconsistent with "Submit" Button
**HIG Violation:** Category 1.4 — Consistency; Category 2.7 — Controls/Buttons: "Labels describe the action that will occur."
**Problem:** The "Show" button next to the Password field uses a similar red/pink color as "Submit" but has a different size, shape, and visual weight. Two buttons on the same screen with similar colors but different proportions and purposes undermine visual consistency. They appear related when they serve entirely different functions.
**Recommendation:** Differentiate the "Show" toggle from the primary "Submit" action. Consider making "Show" a smaller, text-only or icon-based toggle (e.g., an eye icon) that visually reads as a secondary control, not a competing call-to-action.

### 8. Purple Input Fill Doesn't Communicate Editability
**HIG Violation:** Category 1.1 — Metaphors from the Real World: "Metaphors are plain and obvious, not abstract or cryptic."
**Problem:** Traditionally, editable text fields use a lighter recessed background that metaphorically represents paper or a writing surface. The dark purple fills used here look like decorative panels or disabled areas rather than places the user is invited to type. The metaphor for "write here" is absent.
**Recommendation:** Use a lighter fill or inset/recessed visual treatment that signals editability. Even in a dark theme, input fields should be perceptibly lighter or have a visible inner shadow/inset to suggest a writable surface.

### 9. No Required Field Indicators
**HIG Violation:** Category 1.7 — Feedback and Dialog: "Clear explanation when operations can't complete"; Category 1.3 — See-and-Point.
**Problem:** None of the form fields indicate whether they are required or optional. If the user clicks "Submit" without filling a field, they have no way to predict which fields will cause an error. This violates the principle of recognition over recall.
**Recommendation:** Mark required fields with an asterisk (*) or explicit "(required)" text. Alternatively, mark the rare optional fields as "(optional)" if most are required.

### 10. Redundant and Unhelpful Status Message
**HIG Violation:** Category 7.1 — Plain Language: "Messages are concise and simple"; Category 1.7 — Feedback and Dialog: "Feedback is brief, direct."
**Problem:** The message "Enter your details above" at the bottom of the form is spatially redundant — the form is directly above and already self-evident. This message wastes screen real estate without providing actionable feedback. It should instead surface dynamic information (e.g., validation results or submission status).
**Recommendation:** Replace the static instruction with a dynamic feedback area that shows validation results, success confirmations, or error messages only when relevant. When idle, the area can be empty.

---

## Minor Issues (Consider Fixing)

### 11. Monospace/Handwritten Font Reduces Readability
**HIG Violation:** Category 1.10 — Aesthetic Integrity: "Visually confusing displays avoided."
**Problem:** The entire interface uses a handwritten or monospace-style typeface. While stylistically appropriate for a game context, this font reduces legibility for form labels and instructional text compared to a clean sans-serif. Labels like "Search (no label version):" become harder to parse in this typeface.
**Recommendation:** Consider using the stylized font only for titles and headings, while using a more legible proportional font for form labels, placeholders, and instructional text.

### 12. Colons After Every Label Add Visual Noise
**HIG Violation:** Category 1.10 — Aesthetic Integrity: "Graphics support understanding, not just decoration."
**Problem:** Every label ends with a colon ("Username:", "Email:", "Password:", "Search (no label version):"). While a minor convention, colons are unnecessary when labels are positioned directly above their fields — the spatial relationship already indicates the association. The colons add visual noise without aiding comprehension.
**Recommendation:** Remove colons from labels when they are positioned directly above their inputs. Reserve colons for inline label:field layouts where the colon disambiguates the relationship.

### 13. "Tab / Enter" Hint Placed Far from Form Fields
**HIG Violation:** Category 1.3 — See-and-Point: "All available actions are visible in menus or on screen."
**Problem:** The keyboard navigation hint "Tab / Enter" is placed at the very bottom of the screen, far from the input fields it relates to. Users focused on filling out the form may never notice this guidance.
**Recommendation:** Place keyboard hints closer to the form — either as a subtle inline annotation near the first field, or as a tooltip that appears when an input is focused.

### 14. Password Field Narrower Than Other Inputs
**HIG Violation:** Category 1.9 — Perceived Stability: "Core interface elements don't move around"; Category 1.4 — Consistency.
**Problem:** The Password input is visually narrower than the Username and Email inputs because the "Show" button consumes horizontal space to its right. This creates an uneven left-to-right boundary and breaks the consistent visual rhythm of the form column.
**Recommendation:** Make the Password input + Show button combination the same total width as the other inputs. The Show button can be placed inside the input field at the trailing edge (overlaid) rather than beside it.

### 15. No Placeholder Text in Username, Email, or Password Fields
**HIG Violation:** Category 1.7 — Feedback and Dialog; Category 2.5 — Dialog Boxes: "Fields have sensible default values."
**Problem:** The Username, Email, and Password fields are completely empty with no placeholder text to hint at expected format or content. Only the Search field has placeholder text ("Type to search..."). Users must rely entirely on the label to understand what to enter, with no formatting guidance (e.g., "name@example.com" for Email).
**Recommendation:** Add subtle placeholder text to each field showing expected format or an example value. Use a lower-contrast color for placeholders so they don't appear to be actual entered data.

### 16. Lack of Visual Grouping Between Related Form Elements
**HIG Violation:** Category 1.10 — Aesthetic Integrity: "Clear visual hierarchy—most important elements stand out"; Category 2.5 — Dialog Boxes.
**Problem:** All four input fields and the submit button are laid out in a flat vertical list with uniform spacing. There is no visual grouping to indicate which fields belong together (e.g., credentials vs. search). The form reads as an undifferentiated stack rather than structured sections.
**Recommendation:** Group related fields (e.g., Username/Email/Password as "Account Details", Search as a separate section) using subtle dividers, section headers, or varied spacing to create a clear information hierarchy.

### 17. Overall Dark Palette Sacrifices Usability for Aesthetic
**HIG Violation:** Category 3.2 — Color Usage Rules: "Interface elements (menus, windows) have consistent, neutral backgrounds."
**Problem:** The near-black background with dark purple inputs and muted text creates a visually heavy interface. While atmospherically appropriate for a game, the low-luminance palette makes it harder to distinguish interactive elements from decorative ones and reduces overall readability.
**Recommendation:** If maintaining a dark theme, increase the brightness differentiation between the background, input fields, and text. Ensure at least a 4.5:1 contrast ratio for all text and a 3:1 ratio for interactive component boundaries.

---

## Strengths

- **Clear title placement:** The "Text Input Demo" title is prominently positioned at the top of the screen in a bordered header, providing immediate context.
- **Logical field ordering:** The form fields follow a natural top-to-bottom sequence (Username → Email → Password → Search → Submit) that matches user expectations.
- **Password show/hide toggle present:** Including a "Show" button for the password field is good UX practice, allowing users to verify their input.
- **Keyboard navigation hint exists:** The "Tab / Enter" indicator at the bottom acknowledges keyboard users, even though its placement could be improved.
- **Consistent font family:** A single typeface is used throughout the screen, maintaining typographic unity even if readability is traded off.

---

## Overall Score: 3/10

The Text Input Demo screen exhibits significant usability issues stemming from low contrast ratios, inconsistent component styling, and color choices that conflict with established conventions. The dark-on-dark purple palette makes input fields hard to locate, the red submit button sends contradictory signals, and the lack of consistent focus states undermines keyboard navigation. While the layout structure and field ordering are sound, the visual execution needs substantial revision to meet Apple HIG standards for clarity, consistency, and user feedback.
