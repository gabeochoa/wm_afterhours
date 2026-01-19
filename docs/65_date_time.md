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

