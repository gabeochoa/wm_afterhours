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

