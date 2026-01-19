# Styling Infrastructure Issues

**Status:** Not implemented  
**Priority:** Medium

---

## Circular Dependency with Styling Defaults

### Location
`vendor/afterhours/src/plugins/ui/context.h`

### Problem
`UIStylingDefaults` cannot be stored in `UIContext` due to circular dependency.

### Suggested Fix
Forward declarations or moving types.

---

## Tabbing with Value Controls

### Problem
Unclear how to handle components that want to use Widget Value Down/Up to control values while also participating in tabbing navigation.

### Suggested Fix
Add a mode or flag to distinguish between navigation and value adjustment contexts.

