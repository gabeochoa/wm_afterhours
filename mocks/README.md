# Mock comparisons

`../mocks.html` is the authored design gallery. `mock.js` independently resolves
exported layout intent as CSS where modeled; it is a different tool.

```sh
node mocks/test_mock.mjs
./mocks/build.sh --no-serve
./mocks/analyze.py
```

Run from the WM root. Fresh selected trees, screenshots and a viewer are prepared
in `output/wm-followthrough/mock-review/`; `capture-manifest.json` records hashes.
A live browser comparison remains pending because the required local browser setup
was unavailable. Node checks pass but do not test browser layout or native pixels.

## What the model checks

- Native dumps include font file/family, size, weight spans, spacing, inset/offset,
  wrapping intent, minimum size and pixel corner radius. Fonts load before measuring.
- Text-sized leaves contain in-flow labels even when their ink is hidden.
  Auto-fit is approximate; browser and Raylib advances/fallback/baselines can differ.
- Absolute origins replay native coordinates, subtracting margins once. They do not
  independently verify native placement. The former double-margin error is fixed.
- Reports separate candidate differences from text/scroll limits and inherited offsets.
  Categories can overlap. Never copy native solved text height to manufacture agreement.

Use the same viewport, state, fonts and dated tree/image pair. Inspect a contact sheet,
then uncertain crops at full size. Compare fresh browser results before reporting
new counts. The August 92-screen report, 98 stored trees and later 120-tree capture
are different datasets; their totals cannot be combined or treated as current failures.

## Lessons from older audits

Strictness 1 means no shrinking; NoWrap means no wrapping. Scroll containers deliberately
hold offscreen content, negative margins can request overlap, and gaps add to margins.
Five of seven original geometry checks were wrong because they ignored these rules.
The remaining overflow-warning tolerance question is in the root todo.

Scroll margin double-counting and cross-axis expand were fixed. Historical AIM trees
contained all three messages; zero CSS intrinsic text height was not evidence of missing
native entities. Dumped reachable children cannot prove absence of detached entities.
Old baseline updates prove stability only; [design review](../docs/DESIGN_GUIDELINES.md)
still requires direct comparison against the authored mock.
