# Transparent controls contrast check

`TransparentControlsLab` uses explicit RGB values and alpha fills. These calculations
cover the two checkerboard colors separately, including hover and disabled styling.
They verify the chosen colors mathematically. The root screenshot batch must still
confirm actual renderer output, focus-ring visibility and control interaction.

The opaque light backing is RGB 242,246,250; dark is 4,10,18. Pattern cells alternate
202,216,231 and 230,237,245. Light/pattern text and borders use 23,36,53; dark uses
248,250,253. The light/pattern fill is 36,76,114 at alpha 32, rising to 60 on hover.
The dark fill is 215,231,250 at alpha 28, rising to 50 on hover. Alpha uses 0–255.

| Backing | Composited default RGB | Composited hover RGB | Default text | Hover text | Disabled text | Focus against backing |
|---|---|---|---:|---:|---:|---:|
| Light | (216, 225, 233) | (194, 206, 218) | 11.84:1 | 9.80:1 | 17.03:1 | 4.31:1 |
| Dark | (27, 34, 43) | (45, 53, 63) | 15.33:1 | 11.87:1 | 4.59:1 | 4.24:1 |
| Pattern A | (181, 198, 216) | (163, 183, 203) | 8.98:1 | 7.61:1 | 12.86:1 | 3.23:1 |
| Pattern B | (206, 217, 229) | (184, 199, 214) | 10.95:1 | 9.09:1 | 15.66:1 | 3.97:1 |

Calculation uses byte-channel source-over compositing, rounded to the nearest byte:
`result = round(fill * alpha / 255 + backing * (1 - alpha / 255))`.
Contrast uses sRGB relative luminance and `(lighter + 0.05) / (darker + 0.05)`.
The enabled border uses the text color, so it has the same contrast against the
button fill. Its contrast against the backing is also above 3:1 in all four cases.

Disabled calculations follow the current native renderer: `Theme::disabled_variant`
mixes fill RGB 70% toward the page background, desaturates 50%, and scales fill alpha
to 30%. The label renderer darkens the explicit text color by 50%. The screen sets
`disabled_opacity` to 0.3 explicitly. These disabled labels exceed 4.5:1 even though
inactive controls have a WCAG contrast exception. The adjacent Unavailable label
provides an explicit state cue beyond color.

The focus color is 38,111,229. The ring has a negative 5px offset at the 720p design
size, placing it outside the button over the backing. Its thickness is 3px. Both
scale with the screen. The native renderer adds its own contrast edges. A ring
inside the hover fill would fall below 3:1 in some cells, which is why the inset
configuration was rejected.

Run `294_transparent_controls.e2e` and review its default, hover, focus and narrow
captures. Its state assertions verify actions and disabled nonactivation; they do
not prove color contrast or ring pixels. Sokol blending remains an upstream question.
