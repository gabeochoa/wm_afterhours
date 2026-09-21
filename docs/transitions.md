# Transitions todo

Additional feedback from the second review, September 21, 2026.
Examples are in the Transitions Lab, `transitions_lab`.

- [x] Confetti burst: remove bouncing and let the confetti settle into a pile on the floor like real confetti.
- [x] Like burst: replace this with a different example, since we already had this button.
- [x] Smoke ring: make the smallest rings fade out first.
- [x] Get Pro button: fix the missing gradient. It currently looks black.
- [x] Image placeholder: fade the image in with dots, pixel by pixel.
- [x] Smoky dissolve: make it dissolve. It currently looks like it breaks into pieces.
- [x] Gooey plus menu: investigate and fix the blank example.
- [x] Organic shimmer: investigate why only the bottom corner appears, as reported for Image #12, and fix the rendering.
- [x] 3D tilt: increase the amount of tilting.
- [x] Drag and drop: remove the smoke. Have the item plop down, slide into place like a puzzle piece, then fade out.
- [x] Run these examples and capture screenshots or renders at relevant animation stages. Inspect the captures against the requested behavior before marking the fixes complete.

All items above were addressed on September 21, 2026. Get Pro, gooey and
organic shimmer were blank because `make run` draws straight to the window:
`gl_FragCoord` is in retina pixels there while the shader regions were in
logical pixels, and blur requests were dropped. Both paths now match.
Capture the windowed path with `--test-script-dir <dir> --direct-window`
and a window screenshot; headless e2e only exercises the render texture.
