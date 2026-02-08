# List Box and Table

**Status:** Not implemented  
**Priority:** Low

---

## List Box

### Problem

No scrollable list with selection. Dropdown works for small option sets but not for large lists.

### Suggested Implementation

```cpp
template <typename Container>
ElementResult list_box(HasUIContext auto &ctx, EntityParent ep_pair,
                       const Container &items,
                       size_t &selected_index,
                       ComponentConfig config = ComponentConfig());
```

---

## Table / Data Grid

### Problem

No structured table layout with columns.

### Suggested Implementation

```cpp
ElementResult table(HasUIContext auto &ctx, EntityParent ep_pair,
                    const std::vector<std::string> &headers,
                    int num_columns,
                    ComponentConfig config = ComponentConfig());
```

### Features Needed
- Column headers
- Resizable columns
- Sortable columns (click header)
- Row selection
- Scrollable body with fixed header

---

## Example Screen: ListTableShowcase

**File:** `src/systems/screens/ListTableShowcase.h`
**CLI:** `--screen=list_table`
**Category:** Widgets

### Layout

A split screen showing both list and table components:

1. **List Box** — A scrollable list of 30 items ("Item 001" through "Item 030"). Single selection via click. The selected item name is shown in a label below. Keyboard: arrow keys navigate, Enter selects.

2. **Data Table** — A 4-column table: Name | Type | Value | Status. 15 rows of mock data. Column headers are clickable to sort (ascending/descending toggle with arrow indicator). The table body scrolls while headers remain fixed.

3. **Multi-Select List** — A list with checkboxes for multi-selection. A label shows "Selected: 3 items". "Select All" and "Clear" buttons above.

4. **Filterable List** — A `text_input` above a list. Typing filters the list in real time. Shows matching count: "Showing 5 of 30".

### Features Exercised

- `list_box()` with single selection and keyboard navigation
- `table()` with column headers, sortable columns, fixed header
- Scrollable body with `scroll_view` integration
- Row selection highlighting
- Column sort indicators

### Verification

- Clicking a list item highlights it and updates selection label
- Clicking a table header toggles sort direction
- Scrolling the table body keeps headers fixed
- Arrow keys navigate the list, wrapping at edges
- Filter input reduces visible items in real time

### E2E Test Plan

**Test file:** `src/testing/tests/ListTableTest.h`

#### New Custom Commands Needed

- `scroll_on_element(label, delta)` — position mouse over element and simulate scroll wheel. Needed for scrolling the list and table body.
- `type_into_field(label, text)` — focus text input and type. Needed for the filterable list.
- `expect_element_count(label_prefix, expected)` — count elements with a label prefix. Needed to verify filter reduces visible items.

#### Screenshots

1. `list_table_initial` — list and table at default state
2. `list_table_item_selected` — list item highlighted after click, selection label updated
3. `list_table_sorted_asc` — table sorted by Name ascending (arrow indicator visible)
4. `list_table_sorted_desc` — table sorted by Name descending
5. `list_table_filtered` — filterable list after typing, showing reduced items
6. `list_table_scrolled` — table body scrolled down, headers still fixed

#### Test Script

```cpp
TEST(list_table_select_item) {
  co_await TestApp::wait_for_frames(5);

  auto snap_init = TestApp::capture_snapshot("list_table_initial");

  // Click an item in the list
  TestApp::click_button("Item 005");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(3);

  TestApp::expect_ui_exists("Selected: Item 005");
  auto snap = TestApp::capture_snapshot("list_table_item_selected");
}

TEST(list_table_sort) {
  co_await TestApp::wait_for_frames(5);

  // Click Name column header to sort
  TestApp::click_button("Name");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(3);

  auto snap_asc = TestApp::capture_snapshot("list_table_sorted_asc");

  // Click again to reverse sort
  TestApp::click_button("Name");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(3);

  auto snap_desc = TestApp::capture_snapshot("list_table_sorted_desc");
}

TEST(list_table_filter) {
  co_await TestApp::wait_for_frames(5);

  type_into_field("Filter", "Item 01");
  co_await TestApp::wait_for_frames(5);

  TestApp::expect_ui_exists("Showing");
  auto snap = TestApp::capture_snapshot("list_table_filtered");
}

TEST(list_table_keyboard_nav) {
  co_await TestApp::wait_for_frames(5);

  // Click first item to focus list
  TestApp::click_button("Item 001");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(3);

  // Arrow down to navigate
  TestApp::simulate_arrow_key(raylib::KEY_DOWN);
  co_await TestApp::wait_for_frames(3);
  TestApp::simulate_arrow_key(raylib::KEY_DOWN);
  co_await TestApp::wait_for_frames(3);

  // Enter to select
  TestApp::simulate_enter();
  co_await TestApp::wait_for_frames(3);

  TestApp::expect_ui_exists("Selected: Item 003");
}
```

