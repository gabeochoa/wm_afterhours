# Layout QA: cozy_cafe

**Screenshots analyzed:** 1

## Issues Found

### 1. Customer row text overlaps with patience/progress indicators
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** In the "Customers" panel, both customer rows show text collisions. "Emma - Cappuccio" has the "Patience:" label running directly into the 50% progress badge. "Oliver - Croissant" has "Patience: LO" (likely "LOW") truncated and overlapping with the 0% progress badge. The name text, patience label, and progress bar compete for the same horizontal space.
**Suggested fix:** Place the patience label on its own line below the customer name, or enforce minimum spacing between name text, patience label, and progress badge. Consider a multi-line row layout if horizontal space is insufficient.

### 2. Bottom-right navigation icons extend to screen edge
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The Inventory, Research, and Crafting icon buttons in the bottom-right extend very close to the right edge of the viewport. "Crafting" and its label sit nearly flush against the right boundary. The red notification badges ("2" on Inventory, "1" on Research) extend even further toward the edge.
**Suggested fix:** Add right and bottom margin to the navigation icon group to provide consistent spacing from viewport edges.

### 3. Music slider lacks containing panel
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The music volume slider and "Music: 70%" label at the bottom-left float independently without a visible containing panel, unlike "Today's Specials" and "Customers" above which have clear bordered containers. The slider sits in unstructured space between the specials panel and the chat messages area.
**Suggested fix:** Wrap the music slider in a container panel consistent with the style of other sections, or integrate it into an existing panel.

### 4. Notification badges overflow icon containers
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** The notification badges ("2" on Inventory, "1" on Research) overlap the top-right corners of their respective icon containers, extending beyond the icon boundary. While this is a common UI pattern, the badges protrude past the container bounds.
**Suggested fix:** This may be intentional by design convention. If unintended, adjust badge positioning to sit just inside the icon container boundary.
