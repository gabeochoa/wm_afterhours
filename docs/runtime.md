# Running WM

`make run` builds and launches at niceness 10. Profiling records by default but
starts hidden. Use F3 or the Profiler button to show it; F3, Escape or Hide closes
the panel. The screen continues updating behind it. Hide it to use screen controls.

## Find a screen

Backtick opens the browser and pauses the current screen. Search finds names,
categories and descriptions, including collapsed categories. Up/down moves the
cursor; left/right collapses or expands categories when not editing search.
Enter or a row click activates a screen. Escape, backtick or Close returns.
Categories start collapsed; clearing search restores their collapse state.
The white row is the cursor; `*` marks the loaded screen.

Comma/period and Page Up/Page Down cycle screens while the browser is closed.
Moving the browser cursor does not reload a screen.

## Profiler controls

- Pause view freezes the display; Stop recording stops collection. Reset clears history.
- Recent average covers up to 600 recorded frames; overall average covers completed
  frames since reset. Both include frames where a system did not run as zero and
  combine repeated calls in a frame. The table also shows the latest frame.
- Refresh defaults to every rendered frame; selectable 120/60/30 Hz values cap it.
  It cannot refresh faster than the app. CPU/RAM probes run at most twice per second.
- The panel uses 20px Atkinson Hyperlegible. Library callers can configure
  `ProfilerOptions::font_size` and `refresh_hz`.

```sh
make run RUN_ARGS='--profile --screen=chart_lab'
make run RUN_ARGS=
make run ENABLE_PROFILING=0
```

Empty `RUN_ARGS` starts with recording stopped. `ENABLE_PROFILING=0` compiles the
profiler out; changing it invalidates objects automatically. Recording otherwise
continues across screen changes and while hidden. See [measurements](performance.md)
and the [library profiler API](../vendor/afterhours/docs/profiling.md).
