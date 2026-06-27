# SortableJS-Inspired Demo Screen Ideas for afterhours

## Context

[SortableJS](https://sortablejs.github.io/Sortable/) is a JavaScript library for
reorderable drag-and-drop lists. Its examples page showcases a set of interaction
patterns that map well to `afterhours`' existing `with_draggable_children()` /
`DragGroupState` system and `with_overflow(Overflow::Scroll)` scroll views.

This document proposes **new demo screens** to showcase these capabilities, inspired
by SortableJS examples but adapted for game / native UI contexts.

### What afterhours already has

**Drag and drop:**
- `with_draggable_children()` on a container makes its children reorderable
- `DragGroupState` tracks dragging state and emits `Event{source_group, source_index, target_group, target_index}`
- Cross-group dragging works (move items between containers in different groups)
- Existing demos: `DragDropShowcase` (kanban columns), `HorizontalDragShowcase` (horizontal tag lanes)

**Scroll:**
- `with_overflow(Overflow::Scroll, Axis::X/Y)` creates scrollable containers
- `HasScrollView` tracks scroll offset, content size, viewport size
- Supports vertical and horizontal scroll, invert scroll option
- Existing demo: `ScrollViewShowcase` (vertical + horizontal scroll side-by-side)

### SortableJS Features Catalog

From the examples page:

| Feature | Description | afterhours Support |
|---------|-------------|-------------------|
| Simple list | Reorder items in a single list | Exists |
| Shared lists | Move items between two lists (same group) | Exists |
| Cloning | Drag creates a copy, original stays | **Not supported** |
| Disable sorting | Source list is read-only, can only drag from it | **Not supported** |
| Handles | Drag only via a handle grip, not the whole item | **Not supported** |
| Filter | Some items marked undraggable | **Not supported** |
| Thresholds | Configurable swap zone vs. dead zone | **Not supported** |
| Grid | Drag and reorder in a 2D grid | **Not supported** |
| Nested sortables | Sortable lists inside sortable items | **Not supported** |
| MultiDrag | Select multiple items, drag as a batch | **Not supported** |
| Swap | Items swap positions instead of inserting | **Not supported** |

---

## Proposed Demo Screens

### Screen 1: InventorySortGrid

**File:** `src/systems/screens/InventorySortGrid.h`
**CLI:** `--screen=inventory_sort_grid`
**Category:** Game UI

**Concept:** A 2D inventory grid (like Diablo / Minecraft) where items can be
dragged between slots to rearrange. This exercises drag-and-drop on a **grid layout**
rather than a linear list, which is a new capability to demo.

**Layout:**
1. **Main inventory** — a 6x4 grid of square cells (24 slots). About half populated
   with items (colored squares with 1-letter abbreviation and item name below).
   Items: Sword (S, red), Shield (Sh, blue), Potion (P, green), Gold (G, yellow),
   Scroll (Sc, purple), Gem (Gm, cyan), Key (K, orange), Ring (R, pink).
2. **Equipment bar** — a single horizontal row of 4 slots below the grid, labeled
   "Equipped". Items can be dragged between the grid and the equipment bar.
3. **Trash zone** — a small red-tinted area at the bottom-right labeled with a trash
   icon. Dropping an item here removes it. A confirmation count shows how many items
   have been trashed this session.
4. Each grid cell is a drop target. Empty cells show a subtle dashed border. Occupied
   cells show the item. Dragging an item to an occupied cell **swaps** the two items.

**Features exercised:**
- Grid-based drag and drop (not just column/row)
- Swap behavior (drop on occupied cell = swap, not insert)
- Cross-container dragging (grid ↔ equipment bar)
- Drop-to-delete (trash zone)
- Empty state visual (dashed border for empty slots)
- Scroll not needed (fixed grid size)

**What this requires from afterhours (new):**
- Grid-aware drag targeting (2D index instead of linear index)
- Swap mode (option on `with_draggable_children()` or separate config)
- Delete-on-drop zone (a drop target that removes the item instead of inserting)

---

### Screen 2: QuestLogSortable

**File:** `src/systems/screens/QuestLogSortable.h`
**CLI:** `--screen=quest_log_sortable`
**Category:** Game UI

**Concept:** A quest log where the player can reorder quests by priority, pin quests
to the top, and drag quests between "Active" and "Completed" columns. This combines
**sorting**, **filtering**, and **cross-list transfer** in a game-relevant context.

**Layout:**
1. **Left column: Active Quests** — a scrollable list of quest cards. Each card shows:
   quest name (bold), description (muted, 1 line), reward summary (gold icon + number),
   and a drag handle (≡ icon on the left).
2. **Right column: Completed Quests** — same card format but with a strikethrough on
   the name and a green checkmark. Dragging from active → completed "completes" the quest.
3. **Pinned section** — at the top of the Active column, a "Pinned" area with a subtle
   divider. Pinned quests are always visible above the scroll. The player can drag
   quests into/out of the pinned area.
4. **Quest count** — header of each column shows "Active (5)" / "Completed (3)".
5. **Scroll** — both columns scroll independently when they have too many quests.

**Features exercised:**
- Drag handle (only the ≡ icon initiates drag, not the whole card)
- Cross-list transfer with semantic meaning (active → completed)
- Scroll + drag combined (scroll a long list, then drag within it)
- Pinned/sticky section within a sortable list
- Rich card content in draggable items (icon, multi-line text, badge)

**What this requires from afterhours (new):**
- Drag handle support (only a child element triggers the drag, not the whole item)
- Scroll + drag coexistence (scrollable containers with draggable children)

---

### Screen 3: PartyBuilderDragDrop

**File:** `src/systems/screens/PartyBuilderDragDrop.h`
**CLI:** `--screen=party_builder`
**Category:** Game UI

**Concept:** A classic RPG party builder where the player drags character cards from
a roster into party slots. The roster is a scrollable source list. The party is a
fixed row of 4 slots. This exercises **clone-from-source** and
**disable-sort-on-source** patterns from SortableJS.

**Layout:**
1. **Roster** (left, 60% width) — a scrollable grid/list of 12 character cards. Each
   card shows: character portrait (colored avatar circle with initial), name, class,
   level, and a brief stat bar (HP/ATK/DEF as small colored bars). The roster is
   **non-sortable** — you cannot reorder characters within it. You can only drag
   **from** it.
2. **Party** (right, 40% width) — a vertical column of 4 large slots labeled
   "Slot 1" through "Slot 4". Empty slots show a dashed border with "Drag a hero
   here". Filled slots show the character card. Party slots **are** sortable (you
   can reorder the party).
3. **Cloning** — dragging from the roster **clones** the character (the original stays
   in the roster). Dragging the same character again replaces the previous instance if
   they're already in the party.
4. **Party stats summary** — below the party slots, a summary bar shows the team's
   total HP, ATK, DEF as aggregated bars.

**Features exercised:**
- Clone-on-drag from source (SortableJS `pull: 'clone'`)
- Source list is non-sortable but draggable-from (SortableJS `sort: false, put: false`)
- Target list is sortable (reorder within party)
- Fixed-slot drop targets (exactly 4 slots, not a growing list)
- Scroll in the source roster
- Aggregated data derived from drop state (party stats summary)

**What this requires from afterhours (new):**
- Clone mode on drag (source keeps the item, target gets a copy)
- Read-only source (drag-from but no reorder or drop-into)

---

### Screen 4: RecipeStepBuilder

**File:** `src/systems/screens/RecipeStepBuilder.h`
**CLI:** `--screen=recipe_step_builder`
**Category:** Game UI

**Concept:** A potion/recipe crafting screen where the player drags ingredient steps
into a recipe sequence. Steps are ordered and can be rearranged. Some steps have
prerequisites (they turn red if placed out of order). This exercises
**nested sortables** and **filtered/conditional items**.

**Layout:**
1. **Ingredient palette** (left sidebar) — a scrollable vertical list of available
   ingredients, grouped by category (Herbs, Minerals, Liquids) with section headers.
   Each ingredient shows an icon and name. Ingredients are cloned on drag.
2. **Recipe timeline** (center, 60% width) — a vertical list of recipe steps. Each
   step is a numbered card showing the ingredient, with an "×" button to remove it.
   Steps can be reordered by dragging.
3. **Sub-recipe slots** — some ingredients (e.g., "Compound Extract") expand into a
   nested sortable list of sub-ingredients. This creates a **nested sortable** — you
   can reorder sub-ingredients within the compound, and reorder the compound as a whole
   in the main timeline.
4. **Validation indicator** — if steps are in an invalid order (e.g., "Heat" before
   "Add Water"), the invalid step glows red with a warning tooltip. Valid steps glow
   green.
5. **Result preview** — a small panel on the right showing what the current recipe
   would produce (potion name, rarity, effects) based on the step order.

**Features exercised:**
- Clone from palette (ingredient palette is read-only source)
- Nested sortable lists (sub-recipe inside recipe step)
- Remove-on-click (× button to delete a step)
- Order validation / conditional highlighting (red for invalid order)
- Rich drag items with expand/collapse (compound ingredients)

**What this requires from afterhours (new):**
- Nested draggable containers (a draggable item that itself contains draggable children)
- Remove-from-list action (not just reorder/transfer)

---

### Screen 5: MultiSelectDragDemo

**File:** `src/systems/screens/MultiSelectDragDemo.h`
**CLI:** `--screen=multi_select_drag`
**Category:** Component Galleries

**Concept:** A file manager-style demo where the user can select multiple items
(click to toggle selection, or shift-click for range), then drag the selection as a
group to another container. Inspired by SortableJS's MultiDrag plugin.

**Layout:**
1. **Source panel** (left) — a list of 15 "files" with icons and names. Click to
   select (highlighted with accent border). Shift-click for range select. Selected
   count badge in the header.
2. **Target panel** (right) — starts empty. Items dragged here are removed from the
   source and added to the target. Items in the target are also sortable.
3. **Drag ghost** — when dragging a multi-selection, the ghost shows a stack of cards
   with a badge showing the count (e.g., "3 items").
4. **Selection controls** — "Select All" and "Deselect All" buttons above the source.

**Features exercised:**
- Multi-item selection (click, shift-click)
- Multi-drag (drag all selected items at once)
- Selection visual state (highlighted border on selected items)
- Count badge on drag ghost
- Select all / deselect all controls

**What this requires from afterhours (new):**
- Multi-selection state on draggable containers
- Multi-drag (move multiple items in one drag operation)
- Custom drag ghost rendering (stacked cards + count badge)

---

### Screen 6: ScrollDragCombo

**File:** `src/systems/screens/ScrollDragCombo.h`
**CLI:** `--screen=scroll_drag_combo`
**Category:** Component Galleries

**Concept:** A stress-test for scroll + drag interplay. A single tall scrollable list
of 50 items where items can be reordered by dragging. When dragging near the top or
bottom edge, the list auto-scrolls. This is the hardest UX problem in drag-and-drop.

**Layout:**
1. **Scrollable list** — 50 numbered items in a vertically scrolling container that
   shows ~10 items at a time.
2. **Auto-scroll zones** — when dragging an item and the cursor approaches the top or
   bottom 60px of the scroll container, the list auto-scrolls in that direction. Speed
   increases the closer to the edge.
3. **Scroll position indicator** — a side rail showing current scroll position and a
   marker for the dragged item's current drop position.
4. **Drop preview line** — a horizontal colored line between items showing exactly
   where the dragged item will land.

**Features exercised:**
- Long list reordering inside a scroll container
- Auto-scroll during drag (near edges)
- Drop position preview indicator
- Scroll position awareness during drag

**What this requires from afterhours (new):**
- Auto-scroll when dragging near container edges
- Better drop position preview (visual line indicator)

---

### Screen 7: SwapPuzzleGrid

**File:** `src/systems/screens/SwapPuzzleGrid.h`
**CLI:** `--screen=swap_puzzle`
**Category:** Game UI

**Concept:** A 4x4 sliding/swap puzzle where dragging one tile onto another swaps
them. The goal is to arrange tiles in numeric order. Inspired by SortableJS's Swap
plugin. This is a playable mini-game that doubles as a drag-and-drop demo.

**Layout:**
1. **4x4 grid** — 15 numbered tiles + 1 empty slot. Tiles are large squares with
   bold numbers.
2. **Swap mode** — dragging tile A onto tile B swaps their positions (instead of
   inserting). The empty slot acts as a normal swap target.
3. **Move counter** — shows how many swaps the player has made.
4. **Shuffle button** — randomizes the puzzle. Animates tiles shuffling.
5. **Win detection** — when tiles are in order (1-15, empty bottom-right), a
   "Solved!" banner appears with the move count.
6. **Timer** — counts up from shuffle to solve.

**Features exercised:**
- Swap mode (not insert mode)
- Grid-based drag targets
- Win condition detection based on drag state
- Playable interaction (game, not just demo)
- Move counting / stats

**What this requires from afterhours (new):**
- Swap mode (same as InventorySortGrid)
- No actual new API needed if swap mode is implemented

---

## Summary: New Capabilities Needed

| Capability | Used In | Priority |
|------------|---------|----------|
| **Swap mode** (drop = swap positions) | InventorySortGrid, SwapPuzzleGrid | High |
| **Drag handle** (only handle sub-element triggers drag) | QuestLogSortable | High |
| **Clone mode** (source keeps item on drag) | PartyBuilderDragDrop, RecipeStepBuilder | High |
| **Read-only source** (drag from, no reorder or drop into) | PartyBuilderDragDrop, RecipeStepBuilder | Medium |
| **Auto-scroll during drag** (near edges of scroll container) | ScrollDragCombo, QuestLogSortable | High |
| **Grid drag** (2D index targeting) | InventorySortGrid, SwapPuzzleGrid | Medium |
| **Multi-select + multi-drag** | MultiSelectDragDemo | Medium |
| **Nested sortables** (draggable inside draggable) | RecipeStepBuilder | Low |
| **Filter** (mark items as non-draggable) | RecipeStepBuilder (section headers) | Low |
| **Threshold tuning** (swap zone vs. dead zone) | General polish | Low |
| **Drop-to-delete zone** | InventorySortGrid | Low |
| **Custom drag ghost** (stacked cards + badge) | MultiSelectDragDemo | Low |
| **Drop preview line** (visual insertion indicator) | ScrollDragCombo | Medium |

## Recommended Build Order

1. **SwapPuzzleGrid** — self-contained, playable, requires only swap mode. Good first
   demo because it's a game, not just a showcase.
2. **InventorySortGrid** — grid drag + swap, universally recognizable game pattern.
3. **QuestLogSortable** — drag handle + scroll + transfer. Rich game UI context.
4. **ScrollDragCombo** — stress test for auto-scroll during drag. Pure technical demo.
5. **PartyBuilderDragDrop** — clone mode + read-only source. Requires new API.
6. **RecipeStepBuilder** — nested sortables + validation. Most complex.
7. **MultiSelectDragDemo** — multi-drag. Specialized feature.

## Prioritized API Additions for afterhours

Based on what the demos need:

```cpp
// 1. Swap mode (highest priority — enables 2 demos immediately)
ComponentConfig &with_draggable_children_swap();
// or: with_draggable_children(DragMode::Swap)

// 2. Drag handle (designate a child as the drag initiator)
ComponentConfig &with_drag_handle();

// 3. Clone mode (source retains item on drag)
ComponentConfig &with_draggable_children_clone();
// or: with_draggable_children(DragMode::Clone)

// 4. Read-only source (can drag from, can't drop into or reorder)
ComponentConfig &with_drag_source_only();

// 5. Auto-scroll on drag near edges
// Possibly automatic when Overflow::Scroll + draggable_children coexist

// 6. Drop preview indicator
// Visual line between items showing insertion point — could be automatic

// 7. Filter (mark specific children as non-draggable)
ComponentConfig &with_drag_disabled();
```
