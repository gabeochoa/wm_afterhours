# Layout QA: aim_chat

**Screenshots analyzed:** 2

## Issues Found

### 1. Scrollbar extends below the chat window bottom edge
**Type:** overflow
**Screenshot:** idle_720p.png, text_area_arrow_keys.png
**Detail:** On the left side of the chat window, a vertical gray scrollbar-like element extends below the "Direct Connection Established" status bar, continuing past the bottom boundary of the chat window into the black background area. This element bleeds outside the window container in both the idle and text-entry states.
**Suggested fix:** Constrain the scrollbar/resize handle element to remain within the chat window bounds. Clip it at the bottom edge of the window container or adjust its height calculation.

### 2. Small artifact element at left edge of "Send to" bar
**Type:** overlap
**Screenshot:** idle_720p.png, text_area_arrow_keys.png
**Detail:** At the left edge of the "Send to: SmarterChild" bar, there is a small rectangular element (appears to be a leftover padding/border artifact) that slightly protrudes or overlaps with the bar's left boundary. This is consistent across both screenshots.
**Suggested fix:** Review the left-side element positioning in the "Send to" row and remove or properly align any stray border/padding elements.
