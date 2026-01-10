# Font Update Plan

> This document outlines how to integrate the new fonts into the UI showcase screens.

---

## Available Fonts

### Main UI Fonts
| File | Style | Intended Use |
|------|-------|--------------|
| `eqprorounded-regular.ttf` | Rounded geometric | Default/fallback, galleries |
| `Gaegu-Bold.ttf` | Handwritten | CozyCafe headers |
| `EBGaramond-Regular.ttf` | Classic serif | EmpireTycoon (corporate elegance) |
| `EBGaramond-VariableFont_wght.ttf` | Variable weight serif | EmpireTycoon (if variable font support exists) |
| `NotoSansKR.ttf` | Clean sans-serif | Body text fallback |
| `NotoSansMonoCJKjp-Bold.otf` | Bold monospace | NeonStrike (tactical/tech) |

### Clock/Numeric Fonts (in `clock_fonts/`)
| File | Style | Intended Use |
|------|-------|--------------|
| `SFRoundedNumeric-VF.otf` | Rounded numerals | CozyCafe money/stats |
| `SFNumeric-VF.otf` | Clean numerals | EmpireTycoon financial data |
| `SFStencilNumeric-VF.otf` | Stencil numerals | NeonStrike HUD/mission timer |
| `NewYorkNumeric-VF.otf` | Elegant serif numerals | EmpireTycoon stock prices |
| `ADTSlabNumeric-VF.otf` | Slab serif numerals | Alternative for tycoon |

### Icon Fonts
| File | Use |
|------|-----|
| `SymbolsNerdFont-Regular.ttf` | UI icons (all screens) |

---

## Screen-to-Font Mapping

### CozyCafe (Cozy Kraft Theme)
- **Headers/Titles:** `Gaegu-Bold.ttf` — Handwritten café chalkboard feel
- **Body/Buttons:** `eqprorounded-regular.ttf` — Friendly, readable
- **Numbers (coins, wait times):** `SFRoundedNumeric-VF.otf` — Soft, matches cozy vibe

### NeonStrike (Neon Dark Theme)
- **Headers/Mission Name:** `NotoSansMonoCJKjp-Bold.otf` — Tactical terminal look
- **Labels/Stats:** `NotoSansMonoCJKjp-Bold.otf` — Consistent tech aesthetic
- **Numbers (damage, accuracy):** `SFStencilNumeric-VF.otf` — Military stencil style

### EmpireTycoon (Ocean Navy Theme)
- **Headers/Company Name:** `EBGaramond-Regular.ttf` — Corporate prestige
- **Body/Labels:** `EBGaramond-Regular.ttf` — Consistent serif elegance
- **Numbers (money, stats):** `SFNumeric-VF.otf` or `NewYorkNumeric-VF.otf` — Clean financial look

### Component Galleries (Buttons, Forms, Cards)
- **All text:** `eqprorounded-regular.ttf` — Neutral, lets components shine

---

## Code Changes Required

### 1. Load Fonts in `src/preload.cpp`

Currently loads only one font. Update `make_singleton()` to load all fonts:

```cpp
// In make_singleton(), after ui::add_singleton_components<InputAction>(sophie);

auto& font_manager = sophie.get<ui::FontManager>();

// Main fonts
font_manager
    .load_font("gaegu", 
        files::get_resource_path("fonts", "Gaegu-Bold.ttf").string().c_str())
    .load_font("garamond", 
        files::get_resource_path("fonts", "EBGaramond-Regular.ttf").string().c_str())
    .load_font("mono", 
        files::get_resource_path("fonts", "NotoSansMonoCJKjp-Bold.otf").string().c_str())
    .load_font("default", 
        files::get_resource_path("fonts", "eqprorounded-regular.ttf").string().c_str());

// Numeric fonts for special number displays
font_manager
    .load_font("numbers_rounded", 
        files::get_resource_path("fonts/clock_fonts", "SFRoundedNumeric-VF.otf").string().c_str())
    .load_font("numbers_stencil", 
        files::get_resource_path("fonts/clock_fonts", "SFStencilNumeric-VF.otf").string().c_str())
    .load_font("numbers_clean", 
        files::get_resource_path("fonts/clock_fonts", "SFNumeric-VF.otf").string().c_str())
    .load_font("numbers_serif", 
        files::get_resource_path("fonts/clock_fonts", "NewYorkNumeric-VF.otf").string().c_str());

// Icons
font_manager
    .load_font("icons", 
        files::get_resource_path("fonts", "SymbolsNerdFont-Regular.ttf").string().c_str());
```

### 2. Update CozyCafe.h

Replace `.with_font(UIComponent::DEFAULT_FONT, ...)` with specific fonts:

```cpp
// Title "~ Blossom Cafe ~"
.with_font("gaegu", 32.0f)

// Stats and labels
.with_font("default", 16.0f)

// Coin count, wait times (numeric values)
.with_font("numbers_rounded", 16.0f)
```

### 3. Update NeonStrike.h

```cpp
// Mission name "OPERATION: BLACKOUT"
.with_font("mono", 20.0f)

// All labels, stats
.with_font("mono", 14.0f)

// Damage/accuracy numbers
.with_font("numbers_stencil", 12.0f)
```

### 4. Update EmpireTycoon.h

```cpp
// Company name "NEXUS INDUSTRIES"
.with_font("garamond", 18.0f)

// All labels, headers
.with_font("garamond", 14.0f)

// Financial figures ($2.8M, stock prices)
.with_font("numbers_clean", 16.0f)
// or
.with_font("numbers_serif", 16.0f)
```

### 5. Gallery Screens (Buttons.h, Forms.h, Cards.h)

No changes needed — continue using `UIComponent::DEFAULT_FONT` which maps to `eqprorounded-regular.ttf`.

---

## Optional: Add Fonts to Theme Struct

For a more integrated approach, extend `Theme` in `vendor/afterhours/src/plugins/ui/theme.h`:

```cpp
struct Theme {
    // ... existing color fields ...
    
    // Font configuration
    std::string header_font = UIComponent::DEFAULT_FONT;
    std::string body_font = UIComponent::DEFAULT_FONT;
    std::string number_font = UIComponent::DEFAULT_FONT;
};
```

Then update theme presets in `src/theme_presets.h`:

```cpp
inline Theme cozy_kraft() {
    Theme t(/* colors */);
    t.header_font = "gaegu";
    t.body_font = "default";
    t.number_font = "numbers_rounded";
    return t;
}
```

This is optional but would allow screens to automatically pick up the right fonts when applying a theme.

---

## Testing Checklist

After implementation:

- [ ] CozyCafe displays handwritten headers with Gaegu
- [ ] CozyCafe coin counts use rounded numeric font
- [ ] NeonStrike has monospace tactical look throughout
- [ ] NeonStrike damage stats use stencil numbers
- [ ] EmpireTycoon headers show elegant Garamond serif
- [ ] EmpireTycoon financial data uses clean numeric font
- [ ] Gallery screens remain neutral with default font
- [ ] No font loading errors in console
- [ ] All text is readable at various sizes

---

## Font Licensing Notes

- **EB Garamond:** SIL Open Font License (free for all use)
- **Gaegu:** SIL Open Font License (free for all use)
- **Noto Sans:** SIL Open Font License (free for all use)
- **SF Fonts (clock_fonts/):** Apple proprietary — verify licensing for distribution

