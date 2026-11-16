# decide: slider part styling (Tier 2: kart-afterhours, cartographer)

**Problem.** `imm::slider` draws a track and a handle that is a block 25% of the track wide and the track's full height. The handle moves from 0 to 75% of the track. A caller colour now reaches both parts, but it's the same colour on both, so the handle disappears into the track. There is also no fill segment and no way to set the knob size or the track thickness. cartographer passes a transparent `imm::slider` and draws its own track (8px, rounded, bordered), a fill up to the knob centre, and a 16px round knob as three divs. It also replaces the slider's drag callback so the grab maps over `track - knob`. kart doesn't use `slider` at all: it draws a meter in `on_draw_fg` and attaches `HasDragListener` / `HasLeftRightListener` by hand.

The current handle geometry (`0.25f` width, `0.75f` travel) is hard-coded in five places: the drag mapping, the grab offset, `apply_slider_value`, the handle margin and `slider_test`. AFTERHOURS_GAPS.md already says "handle redesign stays a separate decision".

## Options

1. **One `SliderStyle` on ComponentConfig.** `with_slider_style({.fill = c, .handle = c, .handle_size = pixels(16), .track_thickness = pixels(8)})`. Leave it unset and the look stays exactly as it is today. Set it and you get a knob of `handle_size` with its own colour (round when the slider's corners are round), a fill div from the track start to the knob centre, and a track `track_thickness` tall, centred under the knob. The drag, keyboard and handle margin all use `track - handle_size` in place of `0.75`. Fits the existing per-widget config pattern (`toggle_on_indicator`, `checkbox_indicators`). ComponentConfig grows by an `optional<SliderStyle>` of about 48 bytes, and `config_size_test` plus the `apply_overrides` merge need updating. About 120 lines in src plus tests and an example.
2. **A `SliderStyle` argument on `slider()`**, next to `handle_label_position`. Same behaviour as option 1, and ComponentConfig doesn't grow. But it's a second styling channel, so styling defaults and themes can't set it, and that is the "parallel mechanism" we're trying to avoid.
3. **Change the default look to track + fill + knob** for everyone, with the parts styled through option 1. This is the most native-looking result, but every slider in about 20 projects moves pixels and every baseline that shows a slider needs re-shooting.

## Recommendation

Option 1. It's opt-in, so no baselines move. It matches how toggle_switch and checkbox are styled, and it lets cartographer delete its three overlay divs and its replacement drag callback. Option 3 can follow later as a one-line change to the default once projects have tried the knob.

**Rough size:** S-M. About 120 lines in `imm_value.h` and `component_config.h`, plus `slider_test` cases for knob travel, fill width, track thickness and the grab offset with a knob, plus an example.
