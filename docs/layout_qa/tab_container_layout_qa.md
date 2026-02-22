# Layout QA: tab_container

**Screenshots analyzed:** 1

## Issues Found

### 1. Tab bar top corners are sharp while content panel has rounded corners
**Type:** corners
**Screenshot:** idle_720p.png
**Detail:** The tab bar strip across the top has sharp rectangular corners on the "Profile" tab (top-left) and "Settings" tab (top-right), while the white content panel below uses large rounded corners. This creates a visual disconnect — the tab bar and content panel appear as two separate elements rather than a unified tab container. The sharp top-left corner of the first tab is particularly noticeable against the rounded content below.
**Suggested fix:** Apply matching border-radius to the top-left corner of the first tab and the top-right corner of the last tab so the tab bar and content panel form a cohesive rounded shape.

### 2. Tab bar left edge does not align with content panel left edge
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The tab bar begins at a slightly different horizontal position than the white content panel beneath it. The left edge of the "Profile" tab extends slightly further left than the content panel's left edge, creating a visible horizontal misalignment between the navigation strip and its associated content area.
**Suggested fix:** Align the tab bar's left and right edges with the content panel edges, or have both share the same parent container width for consistent alignment.

### 3. Content panel has excessive unused vertical space
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** The white content panel extends well below the last text entry ("Last login: Today at 10:30 AM"), leaving a large empty white area in the lower third of the panel. Additionally, there is significant gray empty space between the bottom of the content panel and the bottom of the viewport. The overall layout under-fills the 720p viewport.
**Suggested fix:** Either size the content panel to fit its content (auto-height) and vertically center the tab assembly, or stretch the content panel to fill more of the available viewport height.
