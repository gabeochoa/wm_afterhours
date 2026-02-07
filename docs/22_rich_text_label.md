# Rich Text Label

**Status:** Not implemented  
**Priority:** Medium

## Problem

No way to render text with inline formatting, effects, or embedded content. Game UIs frequently need styled text with mixed formatting (bold keywords, colored names, inline icons, animated effects).

## Inspiration

Godot's RichTextLabel with BBCode support.

## Suggested Implementation

```cpp
// BBCode-style markup rendering
ElementResult rich_text(HasUIContext auto &ctx, EntityParent ep_pair,
                        const std::string &bbcode_text,
                        ComponentConfig config = ComponentConfig());

// Example usage:
rich_text(ctx, parent, 
    "[b]Bold[/b] and [i]italic[/i] text with [color=#FF0000]red[/color] words");

rich_text(ctx, parent,
    "Press [img]icons/key_a.png[/img] to [wave]jump[/wave]!");
```

## Supported Tags (Suggested)

| Tag | Description |
|-----|-------------|
| `[b]...[/b]` | Bold text |
| `[i]...[/i]` | Italic text |
| `[u]...[/u]` | Underline |
| `[s]...[/s]` | Strikethrough |
| `[color=#RRGGBB]...[/color]` | Text color |
| `[font=name]...[/font]` | Font family |
| `[size=N]...[/size]` | Font size |
| `[img]path[/img]` | Inline image/icon |
| `[url=...]...[/url]` | Clickable link |

## Animated Effects (Optional/Advanced)

| Effect | Description |
|--------|-------------|
| `[wave amp=N freq=N]...[/wave]` | Wavy text animation |
| `[shake rate=N level=N]...[/shake]` | Shaking/vibrating text |
| `[tornado radius=N freq=N]...[/tornado]` | Circular swirl effect |
| `[fade start=N len=N]...[/fade]` | Fade in/out effect |
| `[rainbow freq=N sat=N val=N]...[/rainbow]` | Rainbow color cycling |
| `[pulse freq=N]...[/pulse]` | Pulsing opacity/size |

## Features Needed
- BBCode parser with nested tag support
- Per-character positioning for effects
- Animation system integration (time-based updates)
- Inline image rendering with vertical alignment
- Fallback for unsupported tags (render as plain text)
- Optional: Custom effect registration API

## i18n Compatibility

BBCode markup works well with translation systems since translators receive the full string including tags:

```cpp
// Translation file (e.g., en.json)
// "jump_prompt": "Press [img]icons/key_a.png[/img] to [wave]jump[/wave]!"

rich_text(ctx, parent, i18n::get("jump_prompt"), config);
```

---

## Example Screen: RichTextShowcase

**File:** `src/systems/screens/RichTextShowcase.h`
**CLI:** `--screen=rich_text`
**Category:** Text & Input

### Layout

A vertical stack of rich text examples demonstrating all supported tags:

1. **Basic Formatting** — `[b]Bold[/b], [i]italic[/i], [u]underline[/u], [s]strikethrough[/s]` rendered inline.

2. **Colors & Sizes** — `"[color=#FF0000]Red[/color] [color=#00FF00]Green[/color] [size=32]Big[/size] [size=12]Small[/size]"` showing mixed inline styling.

3. **Nested Tags** — `"[b][i][color=#FFD700]Bold italic gold[/color][/i][/b]"` proving nesting works.

4. **Inline Images** — `"Collect [img]coin.png[/img] coins to unlock [img]chest.png[/img] chests!"` with inline icon rendering at text baseline.

5. **Animated Effects** — `"[wave]Wavy text[/wave]"`, `"[shake]Shaky text[/shake]"`, `"[rainbow]Rainbow text[/rainbow]"` running simultaneously with time-based animation.

6. **Game Dialog Example** — A full game dialog box: `"[b]Elder:[/b] The [color=#FF4444]ancient evil[/color] has returned. You must [wave]find the artifact[/wave] before it's too late!"` — showing practical game UI usage.

### Features Exercised

- BBCode parser with nested tag support
- Per-character positioning for wave/shake effects
- Inline image rendering with vertical alignment
- Color, font size, bold/italic tag support
- Animation system integration for text effects
- Fallback for unsupported tags (render as plain text)

### Verification

- Bold text is visually bolder than surrounding text
- Colors match specified hex values
- Inline images align with text baseline
- Wave animation has visible sinusoidal motion over time
- Unsupported tag `[custom]text[/custom]` renders as plain "text"

### E2E Test Plan

**Test file:** `src/testing/tests/RichTextTest.h`

#### New Custom Commands Needed

None — rich text is display-only. Snapshot comparison is the primary verification method.

#### Screenshots

1. `rich_text_basic_formatting` — bold, italic, underline, strikethrough rendered
2. `rich_text_colors_sizes` — colored and differently-sized text inline
3. `rich_text_nested` — nested tags (bold italic gold) rendered correctly
4. `rich_text_inline_images` — inline icons rendering at text baseline
5. `rich_text_animated` — wave/shake/rainbow effects mid-animation (snapshot captures one frame)
6. `rich_text_game_dialog` — practical game dialog with mixed formatting

#### Test Script

```cpp
TEST(rich_text_renders) {
  co_await TestApp::wait_for_frames(5);

  // All formatted text sections should exist
  TestApp::expect_ui_exists("Bold");
  TestApp::expect_ui_exists("italic");

  auto snap1 = TestApp::capture_snapshot("rich_text_basic_formatting");
  auto snap2 = TestApp::capture_snapshot("rich_text_colors_sizes");
  auto snap3 = TestApp::capture_snapshot("rich_text_nested");
  auto snap4 = TestApp::capture_snapshot("rich_text_inline_images");

  // Wait a few frames for animation to be visible
  co_await TestApp::wait_for_frames(30);
  auto snap5 = TestApp::capture_snapshot("rich_text_animated");

  // Game dialog should render
  TestApp::expect_ui_exists("Elder:");
  auto snap6 = TestApp::capture_snapshot("rich_text_game_dialog");
}

TEST(rich_text_animation_changes) {
  co_await TestApp::wait_for_frames(5);

  // Capture two frames apart to verify animation moves
  auto snap_a = TestApp::capture_snapshot("rich_text_wave_frame_a");
  co_await TestApp::wait_for_frames(15);
  auto snap_b = TestApp::capture_snapshot("rich_text_wave_frame_b");

  // Compare — they should differ (animation moved)
  auto compare = TestApp::compare_snapshot("rich_text_wave_frame_a");
  // Note: compare against frame_b would show pixel differences
}
```

