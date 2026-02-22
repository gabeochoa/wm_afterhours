# Layout QA: animation_basic

**Screenshots analyzed:** 1

## Issues Found

### 1. Header bar extends beyond container's rounded corners
**Type:** corners
**Screenshot:** idle_720p.png
**Detail:** The dark horizontal header bar containing "Animation Basics" and "Fade, Slide, and Scale animations" extends to the full width of the outer container, but the outer container has very large rounded corners. The header bar's sharp left and right edges visually poke past the rounded corner boundary of the container on both sides, creating a visible mismatch where the bar's straight edges extend into the curved area.
**Suggested fix:** Either clip the header bar to respect the parent container's border-radius, reduce the header bar width to stay within the inner curve, or apply matching border-radius to the header bar's corners.
