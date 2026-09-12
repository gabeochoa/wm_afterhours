# Live profiling in wm

`make run` builds profiling support and starts recording with the profiler
hidden. Use the Profiler button or F3 to show it. Hide the panel with F3, Escape
or its Hide button. Recording continues while hidden and across screen changes.
Backtick opens the screen browser.
The screen keeps updating behind the panel. Hide the panel to interact with
its controls.

The profiler uses 20px Atkinson Hyperlegible text and larger chart labels.
It refreshes every rendered frame by default. The Refresh button cycles through
every frame, 120 Hz, 60 Hz and 30 Hz. A selected rate is a cap; the panel cannot
refresh faster than wm renders. CPU and memory probes remain limited to two
samples per second. Pausing the view still freezes its displayed values.

The panel uses the afterhours profiler and timing chart APIs. It shows frame
history, FPS, frame percentiles, system timings, process CPU and resident RAM.
System timings show recent and overall average milliseconds per frame, plus
the latest frame. Recent averages use the last 600 recorded frames, or the
frames collected so far during startup. The header shows the actual window
size. Overall averages cover all completed frames since reset. Both include
zero for frames where a system did not run and combine multiple calls within
a frame. Sort switches between recent and overall per-frame cost.
Pause view freezes the displayed snapshot. Stop recording stops collection.
Reset clears the collected history. The shortcut label reads the current input
binding through the shared input-prompt API.

Run a particular screen with
`make run RUN_ARGS='--profile --screen=chart_lab'`. Use `make run RUN_ARGS=` to
start with recording stopped. F3 still opens the controls. The chart and
system-profile labs remain available through the screen browser.

`make run ENABLE_PROFILING=0` compiles out profiling and omits the startup flag.
Changing this setting invalidates wm's object files automatically, so switching
back to `make run` restores profiling without a manual clean. The executable
also accepts `--profile` directly; a compiled-out build reports that it must be
rebuilt rather than silently ignoring the request.

The run target launches wm at `nice -n 10`. Performance measurements and their
limits are in `vendor/afterhours/docs/profiling-measurements.md`.

Library callers can set `ProfilerOptions::refresh_hz` to a positive rate or 0
for every frame, and `font_size` to their desired text size. Frame collection
continues on every application frame regardless of the panel refresh rate.
