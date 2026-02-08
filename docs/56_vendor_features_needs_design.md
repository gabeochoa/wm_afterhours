# Vendor Features -- Resolved

All items from this document have been resolved.

## 1. `.with_text()` combo method -- **NO**
Users should use themes (`set_default_font()`) instead of a combo method.

## 2. Generic `fallback()` higher-order component -- **NO**
Screen authors can write their own if/else. A generic HOC system adds lambda overhead for minimal gain over plain conditionals.

## 3. `with_720p_size(w, h)` -- **YES, IMPLEMENTED**
Added to ComponentConfig. Uses `w1280()` for width, `h720()` for height. 102 call sites migrated across 16 screens.

## 4. `TextStyle` struct with enum-based keys -- **NO**
Themes + `inherit_from()` cover this. Redundant.

## 5. Default font per-screen / Theme font inheritance -- **YES (migration task)**
`UIStylingDefaults::get().set_default_font()` already exists. Only 6 of 64 screens use it. Need to migrate remaining screens and add docs.

## 6. `ComponentConfig::inherit_style()` -- **NO**
`inherit_from()` already covers this. Redundant.
