# Layout QA: layout_bug_repros

**Screenshots analyzed:** 1

## Issues Found

### 1. Red child overflows cyan parent container (Bug Repro #1)
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** In the "[NOT A BUG] 1: percent(1.0) inside absolute parent" section at the top-left, the red child element labeled "percent(1.0) child" extends beyond the right edge of the cyan parent container. The annotation confirms: "Expected: red fits inside cyan | Bug: red overflows right."
**Suggested fix:** Ensure that `percent(1.0)` sizing accounts for the parent's padding/border so the child fits entirely within the cyan container bounds. If the child is sized at 100% of the parent, the parent's padding must be subtracted from the available width.

### 2. Toggle container overflows at bottom (Bug Repro #7)
**Type:** overflow
**Screenshot:** idle_720p.png
**Detail:** In the "[NOT A BUG] 7: toggle_switch vertical space" section on the right side, the green-bordered container holding three toggles (Toggle A, B, C) has its bottom content clipping or overflowing. The annotation reads "Expected: 3 toggles fit inside green border | Bug: overflow bottom." The third toggle row is very close to or touching the container boundary.
**Suggested fix:** Increase the container height or use auto-sizing to ensure all three toggle rows plus padding fit within the green border.

### 3. "Controller Cu..." label text truncated and overlapping stepper controls
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** In the controller customization area, the label "Controller Cu..." is clipped/truncated, and fragments of text ("ntiv") appear between the "<" stepper button and the "Default" value text, creating a visual collision where label text, stepper arrows, and value text overlap in a crowded region.
**Suggested fix:** Widen the label container or allow it to ellipsize cleanly so the full controller label text is legible and does not collide with the adjacent stepper controls.

### 4. Z-order overlap between RED and BLUE elements (Bug Repro #4)
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** In the "[NOT A BUG] 4: Z-order" section at the lower-left, the blue element ("BLUE mk(2000) ON TOP?") overlaps the red element ("RED"). The annotation states: "Expected: RED(5000) on top | Bug: BLUE(2000) on top (entity ID order, not mk ID)." The z-order is determined by entity creation order rather than the expected marker ID.
**Suggested fix:** This is annotated as "[NOT A BUG]." If the intended behavior is for higher mk IDs to render on top, update the render sorting to use mk ID instead of entity ID.

*Note: Issues #1, #2, and #4 are marked [NOT A BUG] in the annotations, indicating known behavioral characteristics rather than regressions, but they still represent visible layout anomalies.*
