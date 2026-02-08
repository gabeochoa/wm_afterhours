# Date and Time Components

**Status:** Not implemented  
**Priority:** Low  
**Source:** Component Gallery Analysis

---

## Date Input

### Description

A means of inputting a date, often separated into individual fields for day/month/year or using a formatted text input.

### Use Cases in Game UI
- Event scheduling
- Tournament registration
- Birth date entry
- Save file date display
- In-game calendar systems

### Suggested Implementation

```cpp
struct DateValue {
    int year;
    int month;  // 1-12
    int day;    // 1-31
    
    bool is_valid() const;
    std::string format(const std::string& fmt = "YYYY-MM-DD") const;
};

struct DateInputConfig {
    DateValue min_date;
    DateValue max_date;
    std::string format = "MM/DD/YYYY";
    bool show_calendar_button = true;
};

ElementResult date_input(HasUIContext auto &ctx, EntityParent ep_pair,
                         DateValue &value,
                         DateInputConfig config = DateInputConfig());

// Usage:
date_input(ctx, mk(parent, 0), birth_date,
           DateInputConfig{.max_date = DateValue::today()});
```

### Features Needed
- Formatted text input
- Input validation
- Min/max date constraints
- Calendar picker button (opens datepicker)
- Localized date formats

---

## Datepicker / Calendar

### Description

A visual calendar interface for choosing a date.

**Also known as:** Calendar, Datetime picker

### Suggested Implementation

```cpp
struct DatepickerConfig {
    DateValue min_date;
    DateValue max_date;
    bool show_today_button = true;
    bool highlight_today = true;
    std::vector<DateValue> disabled_dates;
    std::vector<DateValue> highlighted_dates;
};

ElementResult datepicker(HasUIContext auto &ctx, EntityParent ep_pair,
                         DateValue &selected_date,
                         DatepickerConfig config = DatepickerConfig());

// Date range picker
ElementResult daterange_picker(HasUIContext auto &ctx, EntityParent ep_pair,
                               DateValue &start_date,
                               DateValue &end_date,
                               DatepickerConfig config = DatepickerConfig());

// Usage:
datepicker(ctx, mk(parent, 0), event_date,
           DatepickerConfig{.min_date = DateValue::today()});
```

### Features Needed
- Month/year navigation (< >)
- Day grid display
- Today highlight
- Selected date highlight
- Disabled dates (grayed out)
- Week day headers
- Month/year dropdowns for quick navigation
- Keyboard navigation

---

## Time Input (Optional)

### Description

Input for selecting a time of day.

### Suggested Implementation

```cpp
struct TimeValue {
    int hour;    // 0-23
    int minute;  // 0-59
    int second;  // 0-59 (optional)
    
    std::string format_12h() const;  // "3:45 PM"
    std::string format_24h() const;  // "15:45"
};

ElementResult time_input(HasUIContext auto &ctx, EntityParent ep_pair,
                         TimeValue &value,
                         bool use_24h = false,
                         ComponentConfig config = ComponentConfig());
```

---

## In-Game Calendar (Game-Specific)

### Description

For games with their own calendar systems (fantasy games, farming sims, etc.).

### Suggested Implementation

```cpp
struct GameDate {
    int year;
    std::string season;  // Or month name
    int day;
};

ElementResult game_calendar(HasUIContext auto &ctx, EntityParent ep_pair,
                            GameDate &current_date,
                            const std::vector<std::string> &season_names,
                            int days_per_season,
                            ComponentConfig config = ComponentConfig());
```

---

## Example Screen: DateTimeShowcase

**File:** `src/systems/screens/DateTimeShowcase.h`
**CLI:** `--screen=date_time`
**Category:** Widgets

### Layout

A screen with date/time input components:

1. **Date Input** — A formatted date input showing "MM/DD/YYYY". Clicking the calendar button opens a datepicker. The selected date shows in a label below.

2. **Calendar Datepicker** — A full calendar widget: month/year header with < > navigation, 7-column day grid (Su-Sa), today highlighted with a circle, selected date highlighted with a filled circle. Disabled dates (weekends) shown grayed out.

3. **Date Range Picker** — Two date inputs ("Start" and "End") linked. Selecting a start date constrains the end date to be after it. The range is visualized on a mini calendar with highlighted span.

4. **Time Input** — A time input with hour/minute dropdowns (or stepper). Toggle between 12h and 24h format. Shows "3:45 PM" vs "15:45".

5. **Game Calendar** — A fantasy calendar with custom season names ("Planting", "Harvest", "Frost", "Bloom"), 28 days per season, custom year numbering. Demonstrates `game_calendar()`.

### Features Exercised

- `date_input()` with format, min/max constraints
- `datepicker()` with month navigation, today highlight, disabled dates
- `daterange_picker()` with linked start/end
- `time_input()` with 12h/24h toggle
- `game_calendar()` with custom seasons and day counts

### Verification

- Clicking a day in the calendar updates the date input
- Month navigation (< >) changes the displayed month
- Today is visually highlighted with a distinct marker
- Disabled dates cannot be selected
- Date range: end date cannot be before start date
- Time format toggle switches between "3:45 PM" and "15:45"

### E2E Test Plan

**Test file:** `src/testing/tests/DateTimeTest.h`

#### New Custom Commands Needed

None — uses existing `click_button`, `expect_ui_exists`, `expect_ui_not_exists`, `simulate_tab`, `capture_snapshot`.

#### Screenshots

1. `datetime_initial` — all date/time inputs at default values
2. `datetime_calendar_open` — calendar datepicker expanded with month grid
3. `datetime_day_selected` — after clicking a day, date input updated
4. `datetime_month_nav` — calendar after clicking ">" to next month
5. `datetime_time_format` — time input showing 12h/24h toggle result
6. `datetime_game_calendar` — fantasy calendar with custom seasons

#### Test Script

```cpp
TEST(datetime_calendar_select) {
  co_await TestApp::wait_for_frames(5);

  auto snap_init = TestApp::capture_snapshot("datetime_initial");

  // Open calendar
  TestApp::click_button("Calendar Button");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(5);

  auto snap_open = TestApp::capture_snapshot("datetime_calendar_open");

  // Click a specific day (e.g., "15")
  TestApp::click_button("15");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(3);

  auto snap = TestApp::capture_snapshot("datetime_day_selected");
}

TEST(datetime_month_navigation) {
  co_await TestApp::wait_for_frames(5);

  TestApp::click_button("Calendar Button");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(5);

  // Click next month arrow
  TestApp::click_button(">");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(3);

  auto snap = TestApp::capture_snapshot("datetime_month_nav");
}

TEST(datetime_time_format_toggle) {
  co_await TestApp::wait_for_frames(5);

  // Toggle between 12h and 24h
  TestApp::click_button("24h");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(3);

  TestApp::expect_ui_exists("15:45");
  auto snap = TestApp::capture_snapshot("datetime_time_format");
}

TEST(datetime_game_calendar) {
  co_await TestApp::wait_for_frames(5);

  TestApp::expect_ui_exists("Planting");
  TestApp::expect_ui_exists("Harvest");

  auto snap = TestApp::capture_snapshot("datetime_game_calendar");
}
```

