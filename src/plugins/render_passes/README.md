# Render Passes local plugin

WM-local plugin (Batch 3 Lane H). Not afterhours upstream. No
`vendor/afterhours` edits were made for this extraction, and none are
needed to keep it local.

## Purpose

Named WM-local home for WM's post-UI blur pass: the render system that
drains Afterhours' UI blur-request queue once per frame after UI has
rendered, dispatching each request through Afterhours' `BlurPass` on
whichever surface WM is actually drawing to. Behavior-preserving
relocation only: no blur order, radius, rect, scale, or clear
semantics change.

Layout choice: `src/plugins/render_passes/` (not
`src/devtools/render_passes/`) because this is a render pass, not a
devtool — it registers as a render system in `game.cpp` on the
production path, not behind a devtools flag. It sits beside the other
`src/plugins/*` locals (`gallery_shell`, `rolling_number`,
`theme_pack`, `font_registry`, `artwork`) that were extracted the same
way (canonical header + shim at the old path + README). The sibling
profiling chrome lives in `src/devtools/profiling/` (see that
README).

## Public API

Canonical header (in this directory):

- `ApplyBlurPass.h` — `include "plugins/render_passes/ApplyBlurPass.h"`
  - `ApplyBlurPass : afterhours::System<>`
    - `std::optional<afterhours::effects::BlurPass> pass` — lazily
      constructed on the first non-empty frame; reused thereafter.
    - `virtual ~ApplyBlurPass() {}` — owns the two render textures
      inside `BlurPass`; unregistering the system cleans them up.
    - `once(float)` — if `afterhours::ui::blur_requests()` is empty,
      return; else construct `pass` if needed and dispatch, in queue
      order:
      - Direct-to-window (`render_backend::draw_directly_to_window`):
        `pass->apply_screen(r.rect, r.radius, scale, screen_px)`
        where `screen_px = (GetRenderWidth, GetRenderHeight)` and
        `scale = screen_px.x / max(1, GetScreenWidth)`.
      - Render-texture: `pass->apply(mainRT, r.rect, r.radius)`.
      - Then `requests.clear()` — each request is applied exactly
        once, then dropped.

Compatibility: `src/systems/ApplyBlurPass.h` is a shim including the
canonical header. Every existing include path and API is preserved:
`game.cpp` (`systems/ApplyBlurPass.h`) and
`src/devtools/wm_devtools.h` (`../systems/ApplyBlurPass.h`) compile
unmodified. Only the two relative includes inside the canonical file
(`../game.h`, `../render_backend.h`) were adjusted to
`../../game.h`, `../../render_backend.h` for the new depth.

## Current consumers

- `game.cpp` — registers `ApplyBlurPass` as a render system at four
  construction sites (lines ~169, ~297, ~468, ~746). Registration
  order relative to the other render systems is the post-UI order
  pinned by the promotion gate below; this plugin does not change it.
- Game blur via UI: any screen using `ComponentConfig::with_blur`
  (AH `HasBlur` -> `blur_requests()` in `ui/rendering.h`) and relying
  on this pass to actually paint the blur — notably `TransitionsLab`
  (transition blur), `Forms`, `ParcelCorpsSettings`, and the
  "Blur radius" row of `AnimationStressLab` (blur-preset radii
  painted through the same queue). `AnimationStressLab` /
  `CurvedTextStressLab` / `SystemProfileLab` are profiling
  consumers (see `src/devtools/profiling/README.md`); for blur, the
  consumer list is the game-blur screens above plus the profiler
  overlay's own panels (which render above the pass and must not be
  blurred by it — the `wm_profiler` input gate's layer-2000 rule is
  the visual counterpart).

## What Afterhours already covers (read-only vendor comparison)

Read from `vendor/afterhours/` in this checkout; no vendor edits:

- `src/plugins/effects.h` — `afterhours::effects::BlurPass`: loads the
  `"blur"` effect, owns two half-res render textures (`a`, `b`),
  `ensure(w, h)`, `usable(effect, radius, rect)` (effect ok, radius
  >= 0.25, rect >= 2x2), `clamp_to`, two-axis `blur_a(radius)`
  (radius clamped to [0, 8], `radius / 6.46` texel steps), and the two
  public entry points this plugin dispatches between:
  `apply(frame, rect, radius)` (render-texture path) and
  `apply_screen(rect, radius, pixel_scale, screen_px)`
  (direct-to-window path).
- `src/plugins/ui/components.h` — `BlurRequest{rect, radius}` and the
  `blur_requests()` queue; `src/plugins/ui/component_config.h` —
  `with_blur(radius)`; `src/plugins/ui/rendering.h` — pushes
  `{draw_rect, HasBlur.radius}` during UI rendering.
- `src/plugins/profiling.h` / `process_metrics.h` — cover collector,
  snapshot, and process CPU/memory (see
  `src/devtools/profiling/README.md`); they do not touch blur and are
  listed here only because the task names them together.

What stays WM-local here: the *dispatch* — WM knows which surface it
is drawing to (`render_backend::draw_directly_to_window` vs
`mainRT`) and the Window<->render-texture pixel scale
(`GetRenderWidth / GetScreenWidth`). AH `BlurPass` exposes both
`apply` and `apply_screen`; the choice between them, the per-frame
queue drain-and-clear, and the render-system registration order are
WM's. That matches the manifest's precise gate for `apply_blur_pass`
(read-only in this batch): promotion needs direct-to-window scale
parity + mainRT parity + `blur_requests()` ownership, pixel-matched.

## Promotion gate

No promotion in this batch. A future Afterhours candidate may take
this pass only when **all** of the following hold, proven by a
documented post-UI pass order and targeted screenshots — not by a
passing build alone:

- **Documented post-UI pass order**: the four `game.cpp` registration
  sites and their order relative to UI render, HUD, and the profiler
  overlay are written down in the promotion PR (which render systems
  run before/after `ApplyBlurPass`, and why blur must see finished UI
  pixels but never the profiler chrome at layer 1500/2000). The PR
  must show this order is unchanged from the pre-move baseline.
- **Blur order/strength parity**: headless screenshots of a
  blur-bearing screen (TransitionsLab transition blur, and the
  AnimationStressLab blur row) are byte-identical before/after, and
  `make validate-screenshots` passes with zero baseline changes.
  Radius, rect, and the direct-to-window scale
  (`GetRenderWidth/GetScreenWidth`) must pixel-match the baseline;
  AH's `usable`/`clamp_to` guards (radius >= 0.25, 2x2 min, [0,8]
  clamp, half-res textures) must produce identical pixels.
- **Queue ownership**: `blur_requests()` is drained exactly once per
  frame and cleared after dispatch (no double-apply, no stale
  requests when the queue is empty — the `requests.empty()` early
  return is part of the contract).

Until then the canonical header above is the contract, and
`--run-all-tests` plus the blur/transition e2e scripts below pin it.

## Tests

```bash
PKG_CONFIG_PATH=$HOME/homebrew/lib/pkgconfig make -j4
./output/ui_tester.exe --run-all-tests
./output/ui_tester.exe --e2e --test-script tests/e2e_scripts/334_transition_blur.e2e --headless
./output/ui_tester.exe --e2e --test-script tests/e2e_scripts/351_animation_stress_lab.e2e --headless
make validate-screenshots
```

No new unit test was needed: the compile of `game.cpp` /
`wm_devtools.h` through the shim (`systems/ApplyBlurPass.h` ->
canonical) is the smoke, `DevtoolsManifestTest.h` pins the manifest's
`apply_blur_pass` entry, and the e2e scripts above exercise the blur
queue end-to-end. (`87_blur_behavior.e2e` / `88_escape_blur.e2e` are
focus-blur, not GPU blur, and are not part of this gate.)
