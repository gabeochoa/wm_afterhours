# Screen Code Review Prompt

Use this prompt to review a screen file and generate simplification feedback.

## Prompt

You are reviewing the UI showcase screen file `src/systems/screens/{SCREEN_NAME}.h` in the afterhours UI framework.

These screens serve as **reference examples** for users building their own UIs. They must be concise, readable, and demonstrate best practices. Your job is to find ways to make the code shorter and simpler **without changing the visual output**.

### Context

The UI framework uses an immediate-mode pattern with these core functions:
- `div()` - container/text element
- `button()` - clickable button
- `slider()` - draggable slider
- `progress_bar()` - read-only progress indicator
- `circular_progress()` - radial progress ring
- `checkbox()` / `toggle_switch()` - boolean controls

All components take a `ComponentConfig` with a fluent builder API (`.with_size()`, `.with_background()`, `.with_font()`, etc.).

Common sizing helpers: `h720()`, `w1280()`, `percent()`, `screen_pct()`, `pixels()`.

### Instructions

1. Read the entire screen file carefully.
2. Identify at least **20 concrete suggestions** to make the code shorter or simpler.
3. **At most 5** suggestions may be about changes to `vendor/afterhours/` (framework-level improvements that would help this screen and others).
4. **At least 15** suggestions must be about the specific screen file code itself.
5. For each suggestion, explain:
   - What the current code does
   - What the simpler version would look like
   - Estimated lines saved
6. Prioritize by impact (most lines saved first).

### Categories to look for

**Screen-level (at least 15):**
- Repeated `ComponentConfig` boilerplate that could use a local helper lambda
- Repeated `.with_font(FONT, h720(X))` across many elements at the same size
- Repeated `.with_background(Theme::Usage::Surface)` that could be a default
- Repeated margin/padding patterns that could be extracted
- Hard-coded entity IDs that could use a loop
- Similar UI sections (e.g., multiple settings rows) that could be generated from data arrays
- Verbose `.with_debug_name("...")` on elements where it adds no value
- Unnecessary `.with_skip_tabbing(true)` on non-interactive elements
- Duplicate size declarations that could be shared variables
- `ComponentConfig{}` chains that could be shortened by inheriting from a base config
- If/else blocks that could be ternary expressions
- Unnecessary `.with_auto_text_color(true)` when it's the default
- Sections that repeat the same row pattern (label + control) that could be a helper

**Vendor-level (at most 5):**
- Missing default values in ComponentConfig that would eliminate common `.with_X()` calls
- Missing convenience overloads (e.g., `.with_size(percent(1.0f), percent(0.5f))` instead of `.with_size(ComponentSize{percent(1.0f), percent(0.5f)})`)
- Missing helper functions that multiple screens would benefit from
- Unnecessary verbosity in the API itself

### Output format

Write the output as a markdown file at `docs/code-feedback/{screen_name}.md` with this structure:

```markdown
# Code Review: {ScreenName}

**File:** `src/systems/screens/{ScreenName}.h`
**Lines:** {N}
**Estimated saveable lines:** {M}

## Screen-Level Suggestions

### 1. {Title} (~{N} lines saved)
**Current:**
```cpp
// current code snippet
```
**Suggested:**
```cpp
// simpler version
```

...repeat for all suggestions...

## Vendor-Level Suggestions

### V1. {Title} (~{N} lines saved across screens)
...

## Summary
- Total screen-level suggestions: {N}
- Total vendor-level suggestions: {N}
- Estimated total lines saveable: {N}
```
