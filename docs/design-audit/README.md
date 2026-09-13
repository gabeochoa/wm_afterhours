# Reviewing baseline designs

[todo.md](../../todo.md) is the consolidated implementation checklist. It contains
all 2,535 audit findings grouped by screen, followed by the existing project
backlog. Update completion checkboxes there; the audit files retain the original
evidence and classifications.

Open [design-audit.html](../../design-audit.html) directly from this checkout.
The page loads its data and images from local files. It also works from a local
HTTP server started at the repository root.

Use the screen search to find names or words in findings. Select Current C++,
Comparison image, or Slider. Comparisons include existing handcrafted web mocks,
original inspiration images where available, and older baseline captures.
View at 100% keeps the C++ image at 1280×720 instead of scaling it to the window.

Queue checkboxes save choices in this browser. Export queue downloads the selected
findings with their baseline hashes. Queueing does not mark anything implemented.
Finding-number links can be copied to open a specific entry.

The 26 existing web mocks remain in [mocks.html](../../mocks.html). The other
screens have screenshot reviews, not newly authored HTML redesigns. The gallery's
All-screen audit link opens the review for the current mock. Embedded gallery
views hide the gallery controls so the slider compares aligned 1280×720 canvases.

## Evidence and scope

[GAPS.md](GAPS.md) contains the readable findings, screen counts and additional
capture notes. The source files are `screens-a.json`, `screens-b.json`,
`screens-c.json`, and `legacy.json`. Each screen is assigned to one source file.
`data.js` and `GAPS.md` are generated from those sources. Captures are identified
by path and SHA-256. Changes to a baseline require a new visual review.

Defects describe visible mistakes. Proposals describe possible design changes,
including improving the explanation around intentional test fixtures. The audit
does not infer missing keyboard support or broken hover states from idle images.
Some sparse screens have fewer than the requested 25 findings rather than repeat
the same issue or invent unseen behavior. All findings start with wm ownership;
a library change needs a separate reproduction of the missing behavior.

Regenerate the report after editing findings:

```sh
nice -n 10 python3 scripts/build_design_audit.py
nice -n 10 python3 scripts/build_design_audit.py --check
```

Run the commands from the repository root. The check verifies screen coverage,
finding IDs and classifications, baseline hashes and generated-file freshness.
It does not verify that a design recommendation has been implemented.

## Verification

The review page passed 282 browser checks covering all 117 baseline images,
26 embedded mocks, 15 source-image links, 32 older captures, filters, queue
persistence and export, keyboard navigation, deep links and local-file loading.
Separate checks exercised pointer and keyboard slider controls and inspected the
desktop and 390px layouts. All 26 mock canvases rendered at 1280×720 with their
comparison assets loaded and no JavaScript exceptions. C++ code and baseline
images were not changed by this audit.
