# Layout QA: forms

**Screenshots analyzed:** 3

## Issues Found

### 1. Right-panel content clipped at right edge
**Type:** overflow
**Screenshot:** forms_before_scroll.png, idle_720p.png, forms_after_scroll.png
**Detail:** On the right side of the main content area, there is a partially visible element clipped by the container's right edge. A small teal/cyan fragment and what appears to be a truncated letter ("A") are visible at approximately x=1030-1040, around the vertical midpoint of the right-side settings panel. This is consistent across all three screenshots, indicating the right column's content or a sibling element extends beyond the visible container boundary.
**Suggested fix:** Ensure the right-side panel (containing settings toggles like Music, SFX, Fullscreen, V-Sync, Show FPS) and any adjacent elements fit within the parent container's bounds. Either widen the container, reduce the panel width, or apply `overflow: hidden` / clip at the correct boundary. Check if an extra element is being positioned outside the intended layout area.

### 2. Slider handle overflows its track container
**Type:** overflow
**Screenshot:** forms_before_scroll.png, idle_720p.png, forms_after_scroll.png
**Detail:** The Volume slider (75%) has a visible drag handle (small white rectangle) that protrudes past the right edge of its slider track container at approximately x=595. The handle extends beyond the slider's visual bounds.
**Suggested fix:** Either extend the slider track container width to accommodate the handle at full-value positions, or clip the handle within the track bounds. Alternatively, adjust the handle positioning so it stays within the track's visual boundary at maximum values.

### 3. Scrolled state shows text clipping at top of right panel
**Type:** overflow
**Screenshot:** forms_after_scroll.png
**Detail:** In the scrolled state, the top of the right-side panel shows partially clipped text. "V-Sync" and "V" label text at the very top of the panel appears cut off, with the upper portion of characters truncated by the container's top edge. This suggests the scroll position reveals content that bleeds past the container's visible clip area.
**Suggested fix:** Ensure the scrollable region clips content cleanly at its top edge, or add padding so that items at the scroll boundary are fully visible or fully hidden rather than partially clipped.
