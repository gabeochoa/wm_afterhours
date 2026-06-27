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

---

## Example Screen

**Note:** These are internal plumbing issues (circular dependency fix, tabbing mode flag). They don't warrant a dedicated showcase screen. The circular dependency fix is verified by successful compilation. The tabbing/value-control distinction is best verified within the `FormsGallery` screen by confirming that a slider can be focused via Tab and adjusted via Up/Down arrows without interfering with Tab navigation to the next widget.

