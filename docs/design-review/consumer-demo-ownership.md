# Ownership of the remaining visual exploration tasks

Source review, September 13, 2026. This separates useful WM demonstrations from
changes to the afterhours contract. It does not mark an approximation as native support.

| Request | WM can do with current APIs | Work that belongs in afterhours |
|---|---|---|
| Expressive typography | Compose an oversized short title and overlapping word blocks with `with_font`, `with_absolute_position`, `with_translate`, `with_scale` and opacity. Keep ordinary labels readable, add a static arrangement, and make motion user triggered. | Native rotated/curved text, text shaping, grapheme-aware character effects, reusable stagger/interruption semantics and renderer-consistent transforms. |
| Transparent components | Demonstrate native buttons over dark, light and patterned backing using alpha background colors, explicit foreground/border, focus and disabled states. `AlphaBlendRepro` already isolates alpha and opacity. | Fix a backend that fails alpha compositing; define renderer-independent blending and effect ordering. The existing Raylib specimen does not resolve the reported Sokol issue. |
| Per-component shader backgrounds | A WM drawing callback can own a procedural background experiment and renderer-specific resources. Such a screen only proves that callback. | A supported shader configuration, local coordinates, uniforms/time, clipping, resource lifetime, batching and unsupported-renderer fallback. Do not call a CPU pattern or color animation shader support. |
| Rolling counters | App state can move clipped digit labels between old and target values; include 9→10, 10→9, negative values, interruption, and an instant/reduced-motion option. | A reusable number transition contract and real blur across supported renderers. A fading duplicate label is not a blur implementation. |
| Independent chart plugin | WM can consume an independently owned adapter and document explicit dependencies/registration. Current `ChartLab` already demonstrates application-owned drawing around the chart. | Extracting `src/plugins/charts.h` and `src/plugins/ui/line_chart.h`, deciding the supported public boundary and versioning, then migrating upstream callers. Do not vendor a second copy inside WM and call the extraction complete. |
| Custom chart indicators | Already present in `ChartLab`: circle/square markers, legends, selected sample, threshold line, and text explaining a threshold outside the plot. | Native stroke, hover-label and explicit axis-bound options remain separate tracked gaps. |

## WM examples and remaining screen options

1. Implemented `TypographyCompositionLab.h`, registered as `typography_composition`.
   It composes MAKE and WAVES as overlapping whole-word blocks with an ordinary reading
   order below. Replay restarts a deterministic, dt-driven one-second arrangement;
   Reduced motion immediately settles both words. `293_typography_composition.e2e`
   covers replay, interruption, keyboard activation, reduced motion and a narrow viewport.
   Runtime and fresh visual verification are pending the root batch. Curved/rotated text
   and grapheme animation remain outside this example.
2. Implemented `TransparentControlsLab.h`, registered as `transparent_controls`, with
   native Add to list buttons on light, dark and patterned backings. The second button
   on each panel is disabled; per-panel counts make activation observable.
   `294_transparent_controls.e2e` covers hover, focus, keyboard/pointer activation,
   disabled nonactivation, reset and a narrow viewport. See [contrast calculations](transparent-controls-contrast.md).
   Runtime and fresh visual verification are pending the root batch.
3. A rolling-value specimen with four explicit target buttons for 9, 10, -1 and 1000,
   plus a reduced-motion switch. Preserve the full current value in one ordinary label.
   Test a second target during transition and carry/borrow. Label any lack of real blur
   in the research record and leave the full animation requirement open.

The named implemented examples use existing APIs. Remaining screen options are not
implementation commitments. Native effects still need upstream work.

## Existing chart-indicator verification

`tests/e2e_scripts/176_chart_lab_design.e2e` covers keyboard sample selection,
hover preview, the 8.33ms threshold label, an out-of-range threshold, empty/single/
constant/negative/multiple datasets, live pause/resume, and three viewport sizes.
`153_chart_lab.e2e` supplies a shorter interaction check. The source renders different
marker shapes and the threshold through `with_on_draw_fg`; visual capture is required
to verify those drawings because the text assertions alone cannot see a square or line.

No additional chart API is needed to close the application-indicator review once that
existing test and fresh visuals pass. Keep chart extraction and native styling open.
