# Translation-Aware Auto-Sizing Label

**Status:** Not implemented  
**Priority:** Medium

---

## Problem

When a UI element has a fixed size, translated text often overflows or gets clipped because different languages produce wildly different string lengths for the same content. Today you either:

1. Make every element enormous (wastes space for short languages)
2. Pick a single font size that fits your longest language (unreadable for others)
3. Manually tune sizes per-language (doesn't scale)

The real solution is a label component that **automatically picks the largest font size that fits the container for all (or a configured subset of) your translations**.

## Core Idea

Given a label key and a fixed container size, iterate through the translations for that key across all configured languages, measure each one, and choose the largest font size where every translation still fits. The UI container size stays the same everywhere — only the font size adapts.

```cpp
// Instead of this (breaks in German, clips in Japanese):
div(ctx, mk(parent), ComponentConfig{}
    .with_size({pixels(200), pixels(40)})
    .with_label("Start Game")
    .with_font("default", h720(24)));

// You do this (font size auto-selected to fit all translations):
auto_label(ctx, mk(parent), StringKey::StartGame, ComponentConfig{}
    .with_size({pixels(200), pixels(40)}));
```

## Suggested API

### Basic: Fit all languages

```cpp
/// Renders a label whose font size is automatically chosen so that
/// the translated text fits in the element's bounds for ALL registered languages.
ElementResult auto_label(HasUIContext auto &ctx, EntityParent ep_pair,
                         StringKeyEnum key,
                         ComponentConfig config = ComponentConfig());
```

### Advanced: Coverage policy

Not all languages need to fit perfectly. Thai or CJK scripts may have very different metrics. You should be able to say "fit 75% of my languages" or "fit all except Thai".

```cpp
enum struct FitPolicy {
  All,         // Every registered language must fit (default)
  Percentage,  // At least N% of languages must fit
  Exclude,     // All languages except a blocklist
  Include,     // Only languages in an allowlist
};

struct AutoLabelConfig {
  FitPolicy policy = FitPolicy::All;

  // For FitPolicy::Percentage — what fraction must fit (0.0–1.0)
  float coverage = 1.0f;  // 0.75 = "fit 75% of languages"

  // For FitPolicy::Exclude / Include
  std::vector<Language> languages;

  // Font size bounds
  Size min_font_size = h720(10);  // Never go below this
  Size max_font_size = h720(36);  // Never go above this

  // Minimum readability threshold — if the auto-chosen size is below
  // this, log a warning (the label may be unreadable)
  Size warn_below = h720(14);
};

ElementResult auto_label(HasUIContext auto &ctx, EntityParent ep_pair,
                         StringKeyEnum key,
                         AutoLabelConfig auto_config,
                         ComponentConfig config = ComponentConfig());
```

### Usage Examples

```cpp
// Fit all languages (default)
auto_label(ctx, mk(parent), StringKey::StartGame);

// Fit at least 75% of languages (let outliers clip)
auto_label(ctx, mk(parent), StringKey::StartGame,
    AutoLabelConfig{.policy = FitPolicy::Percentage, .coverage = 0.75f});

// Fit all except Thai (Thai gets special handling elsewhere)
auto_label(ctx, mk(parent), StringKey::StartGame,
    AutoLabelConfig{.policy = FitPolicy::Exclude,
                    .languages = {Language::Thai}});

// Only care about English, Korean, Japanese
auto_label(ctx, mk(parent), StringKey::StartGame,
    AutoLabelConfig{.policy = FitPolicy::Include,
                    .languages = {Language::English, Language::Korean, Language::Japanese}});

// With explicit font size bounds
auto_label(ctx, mk(parent), StringKey::StartGame,
    AutoLabelConfig{.min_font_size = h720(12), .max_font_size = h720(28)});
```

## Implementation Sketch

```cpp
template <typename StringKeyEnum>
ElementResult auto_label(HasUIContext auto &ctx, EntityParent ep_pair,
                         StringKeyEnum key,
                         AutoLabelConfig auto_config = {},
                         ComponentConfig config = ComponentConfig()) {
  auto [entity, parent] = deref(ep_pair);

  auto *provides = EntityHelper::get_singleton_cmp<ProvidesTranslation>();
  if (!provides) {
    // Fallback: just render the key name
    config.with_label("MISSING_TRANSLATION");
    return div(ctx, ep_pair, config);
  }

  // 1. Collect all translations for this key (respecting policy)
  std::vector<std::string> texts;
  for (auto lang : get_languages_for_policy(auto_config, provides)) {
    const auto &trans_map = provides->get_translations_for_language(lang);
    auto it = trans_map.find(key);
    if (it != trans_map.end()) {
      texts.push_back(it->second.get_text());
    }
  }

  // 2. Binary search for the largest font size where ALL texts fit
  float container_w = resolve_to_pixels(config.size.x_axis, screen_height);
  float lo = resolve_to_pixels(auto_config.min_font_size, screen_height);
  float hi = resolve_to_pixels(auto_config.max_font_size, screen_height);
  float best_size = lo;

  for (int i = 0; i < 10; ++i) {  // ~10 iterations for good precision
    float mid = (lo + hi) / 2.0f;
    bool all_fit = true;
    for (const auto &text : texts) {
      float text_w = measure_text_width(ctx.font, text, mid);
      if (text_w > container_w) {
        all_fit = false;
        break;
      }
    }
    if (all_fit) {
      best_size = mid;
      lo = mid;
    } else {
      hi = mid;
    }
  }

  if (best_size < resolve_to_pixels(auto_config.warn_below, screen_height)) {
    log_warn("auto_label: font size {:.1f}px for key '{}' is below readability threshold",
             best_size, magic_enum::enum_name(key));
  }

  // 3. Render with the current language's text at the computed font size
  std::string current_text = provides->get_string(key);
  config.with_label(current_text);
  config.with_font(config.font_name, pixels(best_size));

  return div(ctx, ep_pair, config);
}
```

### Caching

The font size computation is expensive (measures text across all languages). Cache by:
- `(StringKeyEnum key, container_width, font_name)` → `float best_font_size`
- Invalidate when translations change or container resizes
- Could store in an ECS component `HasAutoLabelCache` or a static `std::unordered_map`

## Per-Language Font Override

The existing `Theme::language_fonts` map already supports per-language font selection with a `size_scale` multiplier. `auto_label` should respect this — when measuring Korean text, use the Korean font and apply its `size_scale`.

```cpp
// In Theme:
std::map<Language, FontConfig> language_fonts;

// auto_label measures each language with its correct font:
FontConfig font_for_lang = ctx.theme.get_font_for_language(lang);
float adjusted_size = candidate_size * font_for_lang.size_scale;
float text_w = measure_text_width(font_for_lang.font_name, text, adjusted_size);
```

## Integration with Existing Components

`auto_label` could also be a mode on existing components rather than a standalone widget:

```cpp
// Option A: Standalone widget (proposed above)
auto_label(ctx, mk(parent), StringKey::StartGame);

// Option B: Config flag on any component that has a label
button(ctx, mk(parent), ComponentConfig{}
    .with_translation_key(StringKey::StartGame)
    .with_auto_fit_text(true));
```

Option B is more ergonomic but requires plumbing `StringKeyEnum` through `ComponentConfig` (which is currently not templated on it). Option A is simpler and doesn't require changing existing APIs.

## What This Replaces

The original `36_game_ui_widgets.md` proposed currency displays, chat boxes, and equipment slots. Those are application-level composites that don't belong in the engine. The translation-aware auto-sizing label is a genuine engine-level primitive that benefits every game with localization.
