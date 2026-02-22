# Layout QA: text_input

**Screenshots analyzed:** 3

## Issues Found

### 1. Bottom status bar and action buttons clipped at viewport edge
**Type:** overflow
**Screenshot:** idle_720p.png, focus_and_type.png, keyboard_input_test.png
**Detail:** The bottom status bar containing instructional text ("Enter your details above" / "Username: a" / "Submitted! User: , Email:") and the "Tab / Enter" hint extends to the very bottom of the viewport. The OK/Cancel/Apply buttons sit flush against the bottom edge with no visible margin below them. This is consistent across all three screenshots and the bottom portion appears clipped.
**Suggested fix:** Add bottom margin or padding below the action buttons row so they don't sit flush against the viewport edge. Reduce content height above or move the status bar up to fit comfortably within the 720p frame.

### 2. Form content left-aligned leaving right half of screen empty
**Type:** container_bounds
**Screenshot:** idle_720p.png, focus_and_type.png, keyboard_input_test.png
**Detail:** All form elements (Username, Email, Password, Search fields, and Submit button) are clustered in the left ~40% of the main content area. The entire right half of the dark rounded container is empty, creating a strongly unbalanced layout with significant wasted space.
**Suggested fix:** Center the form elements horizontally within the container, or widen the form inputs to better utilize the available width.

### 3. Labels centered while inputs are left-aligned
**Type:** container_bounds
**Screenshot:** idle_720p.png, focus_and_type.png, keyboard_input_test.png
**Detail:** The field labels ("Username:", "Email:", "Password:", "Search (no label version):") are centered relative to the viewport, while the input fields and the "Submit" button are left-aligned within the content area. This creates a misalignment where labels are not directly above their corresponding input fields.
**Suggested fix:** Left-align the labels to match the input field positioning, or consistently center both labels and inputs within a form column.
