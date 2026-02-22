# Layout QA: toasts

**Screenshots analyzed:** 11

## Issues Found

### 1. Toast notifications clip against right screen edge
**Type:** overflow
**Screenshot:** toasts_spam_triggered.png, toasts_final.png, toasts_quick_triggered.png
**Detail:** When multiple toasts are stacked in the bottom-right corner, several toast messages extend flush to the right edge of the viewport with no visible right margin. In `toasts_spam_triggered.png` with 4+ simultaneous toasts, the "[!] Warning: Check your settings" and "[*] Custom colored toast!" text reaches the screen boundary. The same right-edge flush positioning is visible in `toasts_final.png` and `toasts_quick_triggered.png`.
**Suggested fix:** Add a consistent right margin (e.g., 8–16px) to the toast container so toasts never touch the viewport edge. Ensure the toast max-width accounts for this margin.

### 2. Toast stack overflows below the visible screen area
**Type:** container_bounds
**Screenshot:** toasts_final.png, toasts_spam_triggered.png
**Detail:** When 5 toasts are stacked simultaneously (visible in `toasts_final.png`), the bottom-most toast ("[i] This sticks around for a while...") is positioned at the very bottom edge of the 720p viewport with its lower portion barely visible or clipped. There is no max-stack limit preventing toasts from pushing below the visible area.
**Suggested fix:** Implement a maximum visible toast count (e.g., 3–4) with older toasts being dismissed when the limit is reached, or adjust the toast stack origin to accommodate the maximum number of simultaneous toasts within the viewport.

### 3. Buttons in "Duration & Spam" row have no spacing between them
**Type:** overlap
**Screenshot:** idle_720p.png (visible in all screenshots)
**Detail:** In the "DURATION & SPAM" section, the first three buttons ("Quick (displays 1s)", "Long (displays 10s)", and "Spam x5 (!)") have zero gap between them — each button's right edge directly abuts the next button's left edge. Meanwhile "Custom Color" has a visible gap before it. This inconsistent spacing makes the first three buttons appear merged into a single element, unlike the "SIMPLE TOASTS" row where all buttons have clear gaps.
**Suggested fix:** Add consistent horizontal gap/margin between all buttons in the Duration & Spam row to match the spacing used in the Simple Toasts row.

### 4. Toast banners overlap Interactive Toasts container boundary
**Type:** overlap
**Screenshot:** toasts_info_triggered.png, toasts_spam_triggered.png, toasts_error_triggered.png
**Detail:** In several triggered states, the floating toast stack in the bottom-right visually overlaps the right edge of the "INTERACTIVE TOASTS" container. For example, in `toasts_info_triggered.png`, the toast banners' left edges overlap the container's right border, obscuring its rounded corner. This is likely intentional (toasts float above content as an overlay), but it partially hides the container boundary.
**Suggested fix:** If toasts are intended as floating overlays, this is acceptable behavior. If not, offset the toast stack further right or ensure it does not encroach on content containers.
