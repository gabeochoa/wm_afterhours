# Existing Issues

This document tracks known issues with the test suite. These tests are expected to fail for now. Once all tests are set up, we can go in and fix them.

## Test Failures

### simple_button_click
**Status:** ❌ Failing  
**Error:** `Expected UI element to exist: Clicks: 1`

**Description:**  
The test clicks the "Click Me" button and expects to see "Clicks: 1" appear, but the UI element is not found. This suggests the button click handler may not be updating the click counter display correctly, or the UI element is not being created/rendered with the expected label.

**Test Location:** `src/testing/tests/SimpleButtonClickTest.h`

### snapshot_test
**Status:** ❌ Failing  
**Error:** `Expected UI element to exist: Clicks: 1`

**Description:**  
Similar to `simple_button_click`, the snapshot test fails because it cannot find the "Clicks: 1" element after clicking the button. This test also attempts to capture and compare snapshots, but fails before reaching that point.

**Test Location:** `src/testing/tests/SnapshotTest.h`

### tabbing
**Status:** ❌ Failing  
**Error:** `Expected element to have focus, but no element is focused`

**Description:**  
The tabbing test simulates Tab key presses to navigate between buttons, but the focus system is not working as expected. After pressing Tab, no element receives focus, causing the test to fail when checking for focus on "Button 1".

**Test Location:** `src/testing/tests/TabbingTest.h`

## Other Issues

### Files Plugin Not Initialized
**Status:** ⚠️ Warning  
**Error:** `Files plugin not initialized. Call files::init() first.`

**Description:**  
The files plugin is not being initialized before use. This may affect file operations in tests, though it doesn't appear to be causing test failures directly.

### Settings File Not Found
**Status:** ⚠️ Warning  
**Error:** `Failed to find settings file (Read): "/Users/gabe/p/wm_afterhours/settings.json"`

**Description:**  
The application is looking for a settings file that doesn't exist. This is expected for a fresh setup, but the warning appears during test execution.

### JSON Config File Write Error
**Status:** ⚠️ Warning  
**Error:** `write_json_config_file error: Couldn't open file for writing: ""`

**Description:**  
An attempt to write a JSON config file is failing because the file path is empty. This may be related to the files plugin not being initialized.

## Notes

- All three tests are currently failing, but this is expected during test setup
- Focus on getting the test infrastructure working first, then fix individual test failures
- The button click functionality appears to be the main issue affecting multiple tests
- The tabbing/focus system needs investigation to understand why focus is not being set

