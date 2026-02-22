# Layout QA: potion_crafting

**Screenshots analyzed:** 6

## Issues Found

### 1. Potion bottle stopper overflows brewing panel top boundary
**Type:** overflow
**Screenshot:** idle_720p.png, potion_crafting_after_first_tab.png, potion_crafting_final.png, potion_crafting_initial.png, potion_crafting_recipe_navigated.png, potion_crafting_tab_clicked.png
**Detail:** In the center brewing panel, the potion bottle graphic's stopper/cap extends above the panel's top content area. The bottle neck protrudes past the container boundary into the header region ("Brewing: Mana Elixir" / "Brewing: Healing Potion" / "Brewing: Antidote"). This is visible in all 6 screenshots and is consistent regardless of which recipe is selected.
**Suggested fix:** Either increase the top padding of the brewing panel to accommodate the full bottle graphic, reduce the bottle size, or move the bottle graphic downward within the panel so the stopper does not cross the container boundary.

### 2. Tab content does not update when "Inventory" tab is selected
**Type:** container_bounds
**Screenshot:** potion_crafting_tab_clicked.png
**Detail:** When the "Inventory" tab is visually highlighted/selected, the content below still shows the Recipes list, brewing area, and ingredients grid -- identical to the Recipes tab view. The tab container's content region does not reflect the expected tab switch.
**Suggested fix:** Ensure each tab (Recipes, Inventory, Journal) maps to distinct content panels, or display a placeholder to indicate the tab was switched if content is not yet implemented.

### 3. Ingredient grid columns have slight width imbalance
**Type:** container_bounds
**Screenshot:** idle_720p.png
**Detail:** In the "Ingredients" panel on the right, the two-column grid has a minor width discrepancy -- the right column (Fire Root x3, Honey Dew x8, Star Dust x1) appears to have slightly more horizontal space than the left column (Moonpetal x5, Sea Salt x12, Shadow Moss x2).
**Suggested fix:** Set both columns to equal width using a uniform grid or flex layout with equal column sizing.
