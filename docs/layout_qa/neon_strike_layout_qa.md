# Layout QA: neon_strike

**Screenshots analyzed:** 1

## Issues Found

### 1. Ability icons overlap each other vertically in left sidebar
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** The left-side ability panel contains four ability icons stacked vertically (UAV, Recon, Shield, Strike). The icons and their labels are spaced tightly enough that adjacent items overlap — the "Recon" label crowds into the bottom of the "UAV" container, and "Shield" overlaps with "Recon". The asterisk markers to the left of each ability also appear crowded together.
**Suggested fix:** Increase the vertical spacing (gap or margin) between ability icon containers in the left sidebar so each icon and its label have clear separation from neighboring items.

### 2. Kill feed container sits too close to compass widget
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** The kill feed notification area (showing elimination messages) is positioned directly below the compass (N/S/E/W) widget at the top center. The top edge of the kill feed container sits very close to the compass "S" label, creating tight coupling between two distinct HUD elements with insufficient separation.
**Suggested fix:** Add vertical margin between the compass widget and the kill feed container to create clear visual separation between these independent HUD elements.

### 3. Ammo counter text overlaps with health bar area
**Type:** overlap
**Screenshot:** idle_720p.png
**Detail:** The ammo counter "35/210" text at the bottom-center of the screen crowds into the health bar container ("80 HEALTH" with the blue bar). The large ammo text extends into the left side of the health bar region, creating a visual collision between these two HUD elements.
**Suggested fix:** Add horizontal margin or padding between the ammo counter and the health bar container, or reposition the ammo counter further to the left so it doesn't intrude into the health bar's bounding area.

### 4. Minimap and weapon icons extend to screen edges with no margin
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The minimap container in the bottom-left corner extends very close to the bottom and left screen edges. Similarly, the weapon inventory icons in the bottom-right corner touch or nearly touch the right screen boundary. Standard HUD practice calls for a safe margin from screen edges.
**Suggested fix:** Add margin/padding to both the minimap container and weapon icon row to maintain a consistent safe zone from all screen edges.
