# Layout QA: advanced_modals

**Screenshots analyzed:** 62

## Issues Found

### 1. Focus ring / cursor indicator overflows text input fields in all modal forms
**Type:** overflow
**Screenshot:** login_modal_open.png, wizard_step1_open.png, feedback_open.png, centering_login_open.png, centering_feedback_open.png, centering_wizard_open.png, login_validation_error.png, feedback_validation_error.png, adv_modals_login_open.png, adv_modals_feedback_open.png, adv_modals_wizard_step1.png
**Detail:** Across all modal forms (Login, Wizard Step 1, Feedback), every text input field displays a small blue square element protruding beyond the right edge of the input container. This focus/cursor indicator extends outside the input's bounding box by several pixels. Visible on the Username/Password fields in Login, Name/Email fields in the Wizard, Subject/Message fields in Feedback, and across all centering-test and reopen-test variants of these modals.
**Suggested fix:** Constrain the focus indicator or cursor element to stay within the input field bounds. Either clip it with overflow:hidden on the input container, or adjust its width/positioning so it renders inside the field's right edge.

### 2. Dropdown menu items overlap with controls behind modal
**Type:** overlap
**Screenshot:** settings_dropdown_open.png
**Detail:** When the Language dropdown is open in the Settings modal, the dropdown option list extends downward and overlaps with the Dark Mode and Notifications toggle switches. The toggle elements are partially visible through/beside the dropdown items on the right side. The bottom "German" option row shows "ly" text fragments from the "Apply" button bleeding through on the right, indicating incomplete z-order coverage.
**Suggested fix:** Ensure the dropdown menu panel has a solid, opaque background and sufficient z-index to fully occlude all modal content behind it. The dropdown items should completely cover any underlying form controls.

### 3. Modal backdrop does not cover the top edge of the viewport
**Type:** container_bounds
**Screenshot:** login_modal_open.png, settings_open.png, feedback_open.png, wizard_step1_open.png, wizard_step2.png, wizard_step3_summary.png, centering_login_open.png, centering_settings_open.png, centering_feedback_open.png, centering_wizard_open.png
**Detail:** When any modal is open, a thin strip of un-dimmed cyan/teal accent color is visible at the very top of the screen above the darkened backdrop. This appears across all modal types (Login, Settings, Feedback, Wizard) and in both the standard and centering-test screenshots, indicating the backdrop overlay does not start at y=0 or does not cover the full viewport height.
**Suggested fix:** Ensure the modal backdrop overlay starts at the absolute top of the viewport (y=0) and spans the full screen dimensions so no underlying content peeks through at the top edge.

### 4. Settings slider handle overflows track on right side
**Type:** overflow
**Screenshot:** settings_volume_adjusted.png
**Detail:** In the Settings modal, when the Volume slider is adjusted to higher values (e.g., 79), the slider handle extends past the right edge of the slider track container, protruding into the modal padding area with a visible white/light overshoot region to the right of the track.
**Suggested fix:** Constrain the slider thumb so it cannot extend past the track bounds. Clamp the thumb position so its right edge stays within the track width.

### 5. Dark Mode focus ring extends beyond modal content area
**Type:** overflow
**Screenshot:** settings_dark_toggled.png
**Detail:** When the Dark Mode toggle row is focused/active in the Settings modal, a white focus ring/highlight box extends to the right beyond the modal container's content area, creating a visual overflow past the modal boundary.
**Suggested fix:** Constrain the focus ring to stay within the modal's content area by applying overflow:hidden to the modal or reducing the width of the focus indicator on the toggle row.
