# Consolidated Design Audit: checkboxes

**Auditors:** Apple HIG, Google Material, Sun Java L&F, Microsoft Win95

---

## Agreed by 4/4 auditors

### 1. Checkbox checked indicator "v" is not a standard checkmark
**Problem:** The checked state uses a lowercase "v" character instead of a proper checkmark (✓). This looks informal and potentially confusing — is it a "v" for value, or a checkmark?
**Flagged by:** Apple (#1), Google (#1), Sun (#1), Microsoft (#1)
**Fix:** Use a proper checkmark character (✓ or ✔) or render an actual checkmark icon. The "v" text approach undermines the professional quality of the showcase.

### 2. "Box Only" checkboxes have no labels and no explanation
**Problem:** The four small squares labeled "Box Only" show tiny colored checkboxes but with no adjacent labels explaining what they represent. As a showcase, this doesn't teach users how to use label-less checkboxes properly.
**Flagged by:** Apple (#2), Google (#2), Sun (#2), Microsoft (#2)
**Fix:** Add example labels or a caption explaining when box-only checkboxes are appropriate (e.g., matrix grids, compact toolbars).

---

## Agreed by 3/4 auditors

### 3. Multi-Select and MinMax sections have low contrast between selected and unselected
**Problem:** In the Multi-Select group, selected items (Option A with "v", Option C with "v") and unselected items (Option B, Option D) use the same background color. Only the "v" text differentiates them.
**Flagged by:** Apple (#3), Google (#3), Sun (#3)
**Fix:** Add a background color change for selected items — slightly highlighted background or left-edge accent to reinforce the checked state visually.

### 4. Disabled states are hard to distinguish from unchecked
**Problem:** "Disabled ON" shows a "v" and "Disabled OFF" shows no indicator, but both look similar to their enabled counterparts. The disabled styling is too subtle.
**Flagged by:** Google (#4), Sun (#4), Microsoft (#4)
**Fix:** Make disabled checkboxes clearly distinct — reduced opacity, gray fill, and "not-allowed" visual cue. Disabled ON should still show the checkmark but grayed out.

---

## Agreed by 2/4 auditors

### 5. Status text at bottom is small and uses "|" as separator
**Problem:** "Selected: Group 2/4 | MinMax 1/3" is small text at the bottom. The pipe separator and compact format make it look like debug output rather than user-facing information.
**Flagged by:** Apple (#5), Microsoft (#5)
**Fix:** Use a more readable format with clearer spacing and section labels. Increase font size for the summary bar.

---

## Summary

| Agreement | Count | Action |
|-----------|-------|--------|
| 4/4 | 2 | **Do** — universal consensus |
| 3/4 | 2 | **Probably do** — strong consensus |
| 2/4 | 1 | **Maybe** — review individually |
| 1/4 | 0 | **Skip unless compelling** |
