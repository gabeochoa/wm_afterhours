# Alerts and Feedback Components

**Status:** Not implemented  
**Priority:** Medium  
**Source:** Component Gallery Analysis

---

## Alert / Banner

### Description

A way of informing the user of important changes in a prominent way. Unlike toasts, alerts are typically static and remain visible until dismissed.

**Also known as:** Notification, Feedback, Message, Banner, Callout

### Use Cases in Game UI
- "New content available!" banners
- Error messages in forms
- Warning about unsaved changes
- Info about maintenance/updates
- Success confirmation after saving

### Suggested Implementation

```cpp
enum class AlertLevel { Info, Success, Warning, Error };

struct AlertConfig {
    AlertLevel level = AlertLevel::Info;
    bool dismissible = true;
    std::optional<std::string> icon;
    std::optional<std::string> action_label;  // "Retry", "Details", etc.
};

ElementResult alert(HasUIContext auto &ctx, EntityParent ep_pair,
                    const std::string &message,
                    AlertConfig config = AlertConfig());

// Usage:
alert(ctx, mk(parent, 0), "Settings saved successfully!", 
      AlertConfig{.level = AlertLevel::Success});

alert(ctx, mk(parent, 1), "Connection lost. Retrying...",
      AlertConfig{.level = AlertLevel::Warning, .action_label = "Retry"});
```

### Features Needed
- Color-coded by severity (info=blue, success=green, warning=yellow, error=red)
- Optional dismiss button (X)
- Optional icon
- Optional action button
- Animated entrance/exit

---

## Empty State

### Description

An indication to the user that there is no data to display in the current view. Often includes an alternative action (e.g., try a different search term).

### Use Cases in Game UI
- Empty inventory: "No items yet. Visit the shop!"
- No search results: "No matches found"
- Empty friends list: "Add friends to play together"
- No saved games: "Start a new adventure"

### Suggested Implementation

```cpp
struct EmptyStateConfig {
    std::optional<TextureConfig> illustration;
    std::optional<std::string> action_label;
    std::optional<std::function<void()>> on_action;
};

ElementResult empty_state(HasUIContext auto &ctx, EntityParent ep_pair,
                          const std::string &title,
                          const std::string &description,
                          EmptyStateConfig config = EmptyStateConfig());

// Usage:
empty_state(ctx, mk(parent, 0),
    "No Items",
    "Your inventory is empty. Visit the shop to get started!",
    EmptyStateConfig{
        .illustration = shop_icon,
        .action_label = "Go to Shop",
        .on_action = []{ navigate_to_shop(); }
    }
);
```

### Features Needed
- Centered layout
- Optional illustration/icon
- Title and description text
- Optional action button
- Subtle, non-alarming visual style

---

## Example Screen: AlertsFeedbackShowcase

**File:** `src/systems/screens/AlertsFeedbackShowcase.h`
**CLI:** `--screen=alerts_feedback`
**Category:** Widgets

### Layout

A screen showing all alert types and empty states:

1. **Alert Levels** — Four stacked `alert()` banners: Info (blue), Success (green), Warning (yellow), Error (red). Each has an icon, message text, and a dismiss (X) button. Dismissing an alert removes it with a fade-out.

2. **Dismissible vs Persistent** — Two alerts: one with `dismissible = true` (has X button), one with `dismissible = false` (no X, stays until programmatically removed).

3. **Alert with Action** — A warning alert: "Your session expires in 5 minutes" with an action button "Extend Session". Clicking the action button dismisses the alert and shows a success toast.

4. **Empty State** — Below the alerts, an `empty_state()` component: centered illustration placeholder, title "No Notifications", description "You're all caught up!", and an action button "Refresh".

5. **Dynamic Alerts** — Buttons to trigger new alerts: "Show Info", "Show Success", "Show Warning", "Show Error". Each creates a new alert at the top of the stack with auto-dismiss after 5 seconds.

### Features Exercised

- `alert()` with all `AlertLevel` variants
- Dismissible alerts with X button
- Alert with action button callback
- `empty_state()` with illustration, title, description, action
- Auto-dismiss timer
- Animated entrance/exit

### Verification

- Each alert level has distinct color (blue/green/yellow/red)
- X button removes the alert
- Action button triggers callback and dismisses
- Empty state shows centered layout with action button
- Auto-dismiss: alert disappears after 5 seconds

### E2E Test Plan

**Test file:** `src/testing/tests/AlertsFeedbackTest.h`

#### New Custom Commands Needed

None — uses existing `click_button`, `expect_ui_exists`, `expect_ui_not_exists`, `wait_for_frames`, `capture_snapshot`.

#### Screenshots

1. `alerts_initial` — 4 stacked alert banners (info/success/warning/error)
2. `alerts_dismissed` — after clicking X on info alert, only 3 remaining
3. `alerts_with_action` — warning alert with "Extend Session" action button
4. `alerts_empty_state` — empty state component visible
5. `alerts_dynamic` — newly triggered alert appearing at top

#### Test Script

```cpp
TEST(alerts_dismiss) {
  co_await TestApp::wait_for_frames(5);

  auto snap_init = TestApp::capture_snapshot("alerts_initial");

  // Dismiss the info alert (click its X button)
  TestApp::click_button("Dismiss Info");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(5);

  auto snap = TestApp::capture_snapshot("alerts_dismissed");
}

TEST(alerts_action_button) {
  co_await TestApp::wait_for_frames(5);

  // Click the action button on the warning alert
  TestApp::click_button("Extend Session");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(5);

  // Alert should be dismissed, success toast may appear
  auto snap = TestApp::capture_snapshot("alerts_with_action");
}

TEST(alerts_empty_state) {
  co_await TestApp::wait_for_frames(5);

  TestApp::expect_ui_exists("No Notifications");
  TestApp::expect_ui_exists("You're all caught up!");
  TestApp::expect_ui_exists("Refresh");

  auto snap = TestApp::capture_snapshot("alerts_empty_state");
}

TEST(alerts_dynamic_trigger) {
  co_await TestApp::wait_for_frames(5);

  TestApp::click_button("Show Error");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(5);

  auto snap = TestApp::capture_snapshot("alerts_dynamic");

  // Wait for auto-dismiss (5s = ~300 frames)
  co_await TestApp::wait_for_frames(320);

  // Dynamic alert should be gone
}
```

