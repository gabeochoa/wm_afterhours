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

