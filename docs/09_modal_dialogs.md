# Modal Dialogs & Overlay Components

## Overview

This document covers modal dialogs and related overlay components. We focus on **modals** (user-initiated dialogs) for implementation, with pop-ups and lightboxes documented as future work.

**Key Distinction:**
- **Modals** - User-initiated dialogs for workflows (delete, share, confirm, settings)
- **Pop-ups** - System-initiated overlays for marketing/announcements (future work)
- **Lightboxes** - Image enlargement overlays (future work)

---

## Implementation Status

### Current State
- **Status:** Not implemented in Afterhours core
- **Workaround:** Custom `win95_widgets.cpp` bypasses the UI framework entirely

### Target Implementation
- **Location:** `vendor/afterhours/src/plugins/modal.h`
- **Pattern:** Follow toast plugin pattern (ECS components + dedicated systems + static API)
- **Demo:** `src/systems/screens/ModalShowcase.h`

---

## Architecture

Following the toast plugin pattern in `vendor/afterhours/src/plugins/toast.h`:

```
┌─────────────────────────────────────────────────────────────┐
│                     modal:: namespace                        │
├─────────────────────────────────────────────────────────────┤
│  Components          │  Systems              │  Static API  │
│  ─────────────────   │  ──────────────────   │  ─────────── │
│  ModalRoot           │  ModalInputBlock      │  open()      │
│  Modal               │  ModalBackdropRender  │  close()     │
│                      │  ModalCloseWatcher    │  is_open()   │
│                      │  ModalFocusTrap       │  alert_*()   │
└─────────────────────────────────────────────────────────────┘
```

### Core Components

```cpp
namespace afterhours {

struct modal : developer::Plugin {

  // Singleton to track modal stack
  struct ModalRoot : BaseComponent {
    std::vector<EntityID> modal_stack;  // For z-ordering, newest at back
  };

  // ClosedBy modes (following HTML dialog spec)
  enum class ClosedBy {
    Any,          // Close on escape OR click outside (light dismiss)
    CloseRequest, // Close on escape only (default for modal)
    None          // Manual close only
  };

  // Dialog result
  enum class DialogResult {
    Pending,    // Dialog still open
    Confirmed,  // OK/Yes/Confirm clicked
    Cancelled,  // Cancel/No clicked or Escape pressed
    Dismissed,  // Backdrop clicked or X button
  };

  // Component attached to modal entities
  struct Modal : BaseComponent {
    bool is_modal = true;                    // true = showModal(), false = show()
    DialogResult result = DialogResult::Pending;
    std::string return_value;                // Like HTML dialog.returnValue
    Color backdrop_color = {0, 0, 0, 128};
    
    ClosedBy closed_by = ClosedBy::CloseRequest;  // Default for modal dialogs
    
  bool show_close_button = true;
    size_t open_order = 0;
    EntityID previously_focused_element = -1;  // Restore focus on close
    EntityID autofocus_element = -1;           // Initial focus target
  };

  // ... systems and API below ...
};

} // namespace afterhours
```

### Key Systems

1. **ModalInputBlockSystem** - Blocks input to elements outside top modal's tree. Checks `is_modal` flag to determine blocking behavior.

2. **ModalBackdropRenderSystem** - Renders backdrop for modal dialogs. Uses high render layer to appear above regular UI.

3. **ModalCloseWatcherSystem** - Handles close requests based on `closed_by` mode:
   - Escape key (if `CloseRequest` or `Any`)
   - Click outside (if `Any` only, with proper pointerdown/pointerup tracking)

4. **ModalFocusTrapSystem** - Constrains tabbing to modal's child tree, restores focus on close.

---

## Composable Usage (Primary API)

Modals are fully composable - put any UI components inside. Pass the open state directly into the modal call:

```cpp
// Pass open state INTO the modal call - content block only runs when visible
if (auto dialog = modal(ctx, mk(entity, "my_dialog"), show_my_dialog,
      ModalConfig{}
        .with_size(pixels(400), pixels(300))
        .with_title("Confirm Delete"))) {
  
  // This block only runs when modal is visible!
  div(ctx, mk(dialog.ent(), 0), 
      ComponentConfig{}.with_label("Are you sure you want to delete this item?"));
  
  auto button_row = div(ctx, mk(dialog.ent(), 1),
      ComponentConfig{}.with_flex_direction(FlexDirection::Row));
  
  if (button(ctx, mk(button_row.ent(), 0), 
             ComponentConfig{}.with_label("Delete"))) {
    show_my_dialog = false;  // Just set to false, modal handles cleanup
    perform_delete();
  }
  if (button(ctx, mk(button_row.ent(), 1), 
             ComponentConfig{}.with_label("Cancel"))) {
    show_my_dialog = false;
  }
}
// When show_my_dialog becomes false, modal automatically:
// - Closes and removes from modal_stack
// - Restores focus to previously focused element
// - Fires close event
```

### API Signature

```cpp
// Main modal function - returns ElementResult, truthy when open
ElementResult modal(HasUIContext auto &ctx, EntityParent ep_pair,
                    bool &open,  // Reference to open state
                    ModalConfig config = ModalConfig());

// With result output - know HOW it was closed (escape, backdrop, button)
ElementResult modal(HasUIContext auto &ctx, EntityParent ep_pair,
                    bool &open,
                    DialogResult &out_result,  // Set when closed
                    ModalConfig config = ModalConfig());
```

**Benefits of this pattern:**
- Single if block, no nested conditionals
- Modal content only evaluated when visible (efficient)
- Automatic cleanup when state changes to false
- Familiar pattern (similar to ImGui's `Begin()`/`End()`)

### System Registration

```cpp
// In game init, similar to toast:
modal::enforce_singletons(systems);
modal::register_update_systems<InputAction>(systems);
modal::register_render_systems<InputAction>(systems);
```

---

## Convenience Helpers

Pre-built patterns for common dialogs, built on the composable modal system.

### modal::info

Single-button acknowledgment dialog. User reads message and dismisses.

```cpp
// Use: System notifications, awareness alerts, FYI messages
// Buttons: OK, Close, Got it
modal::info(ctx, mk(entity, "low_battery"), show_battery_warning,
    "Low Battery",                      // title
    "20% battery remaining.",           // message
    "OK");                              // button label (optional)
```

### modal::confirm

Two-button confirmation dialog. User confirms or cancels an action.

```cpp
// Use: Permission requests, destruction confirmation, yes/no decisions
// Buttons: Delete/Cancel, Allow/Don't Allow, Yes/No
DialogResult result;
if (modal::confirm(ctx, mk(entity, "delete_confirm"), show_delete_confirm, result,
    "Delete Photo?",                    // title
    "This action cannot be undone.",    // message
    "Delete",                           // confirm button
    "Cancel")) {                        // cancel button
  if (result == DialogResult::Confirmed) {
    perform_delete();
  }
}
```

### modal::fyi

Three-button dialog with tertiary/neutral option.

```cpp
// Use: Updates available, tips, "learn more" scenarios
// Buttons: Primary action, Dismiss, Learn more
DialogResult result;
if (modal::fyi(ctx, mk(entity, "update_available"), show_update_notice, result,
    "Update Available",                 // title
    "Version 2.0 is ready to install.", // message
    "Install Now",                      // primary action
    "Not Now",                          // dismiss
    "Learn More")) {                    // tertiary (optional)
  switch (result) {
    case DialogResult::Confirmed: install_update(); break;
    case DialogResult::Cancelled: dismiss(); break;
    case DialogResult::Custom: open_release_notes(); break;
  }
}
```

### Common Alert Use Cases

| Use Case | Description | Typical Actions |
|----------|-------------|-----------------|
| **Permission** | App requesting access to system resources | Allow / Don't Allow |
| **Destruction confirmation** | Before delete, remove, clear actions | Delete / Cancel |
| **System/network problems** | Connection errors, crashes | OK (acknowledge) |
| **Awareness** | Battery low, storage full, updates | OK, or action button |

### Best Practices for Alert Dialogs

1. **Limit use** - Alerts interrupt flow, use sparingly
2. **Always dismissible** - Clear way to close (except critical errors)
3. **Descriptive button labels** - "Delete" not "OK" for destructive actions
4. **Clear language** - Match brand guidelines, use "please"/"sorry" when appropriate
5. **Title + message** - Advanced users scan title, new users read both

---

## Modal Do's and Don'ts

### DO

- **Easy to close** - Escape key, click outside (optional), clear close button
  - Implementation default: `closed_by = CloseRequest`, `show_close_button = true`
- **Single purpose** - One straightforward task per modal
- **Keep it short** - Brief, concise content
- **Accessible** - Keyboard navigation, focus trapping, screen reader support

### DON'T

- **Fullscreen modals** - Just navigate to a new page instead
- **Multi-step modals** - Use wizard pattern on separate pages, not modal steps
- **Self-spawning modals** - Only user-initiated (distinguishes modals from pop-ups)
- **Marketing modals** - Newsletters, promos = pop-ups, not modals
- **Scrollable dialog body** - If scrolling needed, use `scroll_view` child container

### Modal Stacking (Modal from Modal)

Opening a modal from within another modal is supported. Use cases:
- Confirmation dialog on top of a settings dialog ("Are you sure you want to discard changes?")
- Detail view from a list modal
- Nested workflows

```cpp
// First modal - settings dialog (pass show_settings into modal)
if (auto settings = modal(ctx, mk(entity, "settings"), show_settings,
      ModalConfig{}.with_title("Settings"))) {
  
  // ... settings content ...
  
  if (button(ctx, mk(settings.ent(), 99), 
             ComponentConfig{}.with_label("Reset to Defaults"))) {
    show_reset_confirm = true;  // Open nested modal
  }
}

// Second modal - confirmation (renders on top when show_reset_confirm is true)
DialogResult reset_result;
if (auto confirm = modal(ctx, mk(entity, "reset_confirm"), show_reset_confirm,
      reset_result,  // Captures how it was closed
      ModalConfig{}
        .with_title("Reset Settings?")
        .with_size(pixels(350), pixels(150)))) {
  
  div(ctx, mk(confirm.ent(), 0),
      ComponentConfig{}.with_label("This will reset all settings to defaults."));
  
  auto buttons = div(ctx, mk(confirm.ent(), 1),
      ComponentConfig{}.with_flex_direction(FlexDirection::Row));
  
  if (button(ctx, mk(buttons.ent(), 0), ComponentConfig{}.with_label("Reset"))) {
    show_reset_confirm = false;
    reset_settings();
  }
  if (button(ctx, mk(buttons.ent(), 1), ComponentConfig{}.with_label("Cancel"))) {
    show_reset_confirm = false;
  }
}
```

**How it works:**
- `modal_stack` tracks all open modals in order
- Each modal gets its own backdrop (stacked)
- Input is blocked to everything except the **topmost** modal
- Escape closes only the topmost modal
- Focus is trapped within the topmost modal

**Best practices for stacking:**
- Keep nesting shallow (2 levels max recommended)
- Nested modals should be simpler than their parent (confirmations, not complex forms)
- Consider if the workflow could be flattened instead

### Scrollable Content Pattern

If modal content needs scrolling (e.g., terms of service), use a scroll container child:

```cpp
auto dialog = modal(ctx, mk(entity, "terms"),
    ModalConfig{}.with_size(pixels(500), pixels(400)));

// Scrollable content area - use scroll_view child, not scrollable dialog
auto scroll = scroll_view(ctx, mk(dialog.ent(), 0),
    ComponentConfig{}.with_size(percent(1.0f), percent(0.8f)));

for (const auto& paragraph : terms_paragraphs) {
  div(ctx, mk(scroll.ent()), 
      ComponentConfig{}.with_label(paragraph));
}

// Fixed footer with buttons (not scrolled)
auto footer = div(ctx, mk(dialog.ent(), 1),
    ComponentConfig{}.with_size(percent(1.0f), pixels(50)));
    
if (button(ctx, mk(footer.ent(), 0), ComponentConfig{}.with_label("I Agree"))) {
  modal::close(ctx, dialog.ent().id, DialogResult::Confirmed);
}
```

---

## HTML Dialog Spec Reference

Following web standards from the HTML `<dialog>` element:

### ClosedBy Modes

| Mode | Escape Key | Click Outside | Use Case |
|------|-----------|---------------|----------|
| `Any` | Closes | Closes | Non-critical, easily dismissible |
| `CloseRequest` | Closes | No effect | Default for modals, important actions |
| `None` | No effect | No effect | Critical dialogs requiring explicit action |

### Focus Management

1. **Store previously focused element** before opening
2. **Restore focus** to it when dialog closes
3. **Support autofocus** on a child element for initial focus
4. **Focus trapping** - Tab cycles within dialog only

```cpp
// Autofocus example - focus the name input, not the first field
auto dialog = modal(ctx, mk(entity, "edit_product"),
    ModalConfig{}.with_title("Edit Product"));

// Product number is readonly, skip it
div(ctx, mk(dialog.ent(), 0),
    ComponentConfig{}.with_label("Product Number: 12345"));

// Focus this input instead
text_input(ctx, mk(dialog.ent(), 1),
    ComponentConfig{}
      .with_label("Product Name")
      .with_autofocus(true),  // <-- Initial focus here
    product_name);
```

### Events (for advanced use)

- `beforetoggle` (cancelable) - Before open/close, can prevent
- `toggle` - After open/close completes
- `close` - After close, includes return value

---

## Lessons Learned from PR #26

A previous implementation attempt encountered several issues. These are documented here to avoid repeating them.

### Issues and Solutions

| Issue | Root Cause | Solution |
|-------|-----------|----------|
| `mk()` entity conflicts | Calling mk() every frame for modal entities | Use same pattern as scroll_view - mk() works correctly |
| Escape closes modal AND window | Input mapping has Escape for both | Consume escape in ModalCloseWatcher, add `should_close_on_escape()` utility |
| Backdrop click instant close | Mouse release from "Open Modal" button triggers dismiss | Track where press started, only close if press started outside modal |
| Modal content positioned wrong | HasUIModifiers transforms not inherited | Use absolute positioning for modal root |
| Backdrop renders on top of content | Render layer management incorrect | Dedicated render pass: backdrop → container → children |
| Layout issues with percent sizing | Circular dependencies in layout | Use explicit pixel sizes for modal container |

### Remaining Layout System Limitations

These require core layout system fixes (tracked separately):

1. **children() on labels** - Returns 0 because labels have no child elements
   - Workaround: Use explicit heights like `h720(24)`

2. **Flex layout with percent siblings** - 100% width child causes 0 width siblings
   - Workaround: Use `percent(0.9f)` to leave room

3. **No flex-grow/flex-shrink** - Can't have one element fill remaining space
   - Workaround: Calculate explicit sizes

---

## Current Workaround

Until the modal plugin is implemented, `win95_widgets.cpp` provides working dialogs:

```cpp
// Draw a message dialog
// Returns: 0 = OK, 1 = Cancel, -1 = still open
int DrawMessageDialog(raylib::Rectangle dialogRect, const char* title,
                      const char* message, bool hasCancel = false);

// Draw a simple input dialog
// Returns: 0 = OK, 1 = Cancel, -1 = still open
int DrawInputDialog(raylib::Rectangle dialogRect, const char* title,
                    const char* prompt, char* buffer, int bufferSize);
```

This works but doesn't integrate with Afterhours theming or the ECS framework.

---

## Future Work

### Pop-ups (Not Modals)

Pop-ups are **system-initiated** overlays, typically for marketing. They interrupt the user journey and should be used very sparingly.

**Characteristics:**
- Appear on page load or after time delay
- User did NOT initiate the action
- Usually marketing: discount codes, newsletter signups, sale announcements
- Should be easily dismissible
- Consider user preferences (don't show again)

**Examples:**
- Newsletter signup prompt
- First-time user onboarding
- Sale/discount notifications

**Implementation Notes:**
- Could reuse modal infrastructure with different defaults
- Should track "dismissed" state to avoid re-showing
- Consider timing (don't show immediately on page load)

### Lightboxes

A lightbox is a specialized modal for **image enlargement**.

**Characteristics:**
- User clicks a thumbnail to trigger
- Displays single image at larger size
- Often includes gallery navigation (prev/next)
- Typically has minimal UI (close button, arrows)
- Click outside or escape to close

**Use Cases:**
- Photo galleries
- Product image zoom
- Portfolio displays

**Implementation Notes:**
- Could be a specialized modal variant
- Needs image loading/scaling logic
- Gallery navigation state management

---

## Integration with Other Components

### Scrollable Content (scroll_view)

See "Scrollable Content Pattern" above. Use `scroll_view` as a child, not scrollable modal body.

### Toasts

Modals and toasts serve different purposes:
- **Modal** - Blocking, requires user action
- **Toast** - Non-blocking, informational, auto-dismiss

A modal might trigger a toast on close:
```cpp
if (modal::close(ctx, dialog_id, DialogResult::Confirmed)) {
  perform_save();
  toast::send_success(ctx, "Document saved!");
}
```

---

## Notes

- **Modal stacking supported** - Newest modal renders on top, escape closes topmost only
- Modal z-ordering: `modal_stack` tracks order, each modal gets its own backdrop
- Animation: fade in/out for backdrop would be nice (future enhancement)
- Accessibility: focus management and keyboard navigation are critical
- Testing: E2E tests should verify focus trapping, escape key, backdrop click, and stacking
