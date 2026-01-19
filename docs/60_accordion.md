# Accordion

**Status:** Not implemented  
**Priority:** Medium  
**Source:** Component Gallery Analysis

## Description

An accordion is a vertical stack of interactive headings used to toggle the display of further information. Each item can be 'collapsed' (showing just a short label) or 'expanded' (showing full content).

**Also known as:** Collapse, Collapsible sections, Details, Disclosure, Expandable, Expander

## Use Cases in Game UI
- Settings categories that can be expanded/collapsed
- FAQ or help sections
- Inventory categories
- Quest log with expandable quest details
- Character stats/skills with expandable descriptions

## Suggested Implementation

```cpp
struct AccordionConfig {
    bool allow_multiple_open = false;  // Only one section open at a time
    bool animate_transition = true;
    float animation_duration = 0.2f;
};

ElementResult accordion(HasUIContext auto &ctx, EntityParent ep_pair,
                        AccordionConfig config = AccordionConfig());

ElementResult accordion_item(HasUIContext auto &ctx, EntityParent ep_pair,
                             const std::string &header,
                             bool &is_expanded,
                             ComponentConfig config = ComponentConfig());

// Usage:
accordion(ctx, mk(parent, 0)) {
    accordion_item(ctx, mk(current, 0), "Graphics Settings", graphics_expanded) {
        // Content when expanded
        slider(ctx, mk(current, 0), "Brightness", brightness);
        toggle(ctx, mk(current, 1), "Fullscreen", fullscreen);
    }
    accordion_item(ctx, mk(current, 1), "Audio Settings", audio_expanded) {
        slider(ctx, mk(current, 0), "Master Volume", master_vol);
        slider(ctx, mk(current, 1), "Music Volume", music_vol);
    }
}
```

## Features Needed
- Expand/collapse toggle (click header or chevron icon)
- Animated height transition
- Chevron/arrow indicator showing state
- Optional: only one section open at a time
- Keyboard navigation (Enter/Space to toggle)
- ARIA attributes for accessibility

## Visual States
- Collapsed: Shows header with closed chevron (▶ or ▼)
- Expanded: Shows header with open chevron (▼ or ▲) + content
- Hover: Highlight on header
- Focus: Focus ring on header

## Related
- See `25_tree_node.md` for simpler single-level collapsible headers

