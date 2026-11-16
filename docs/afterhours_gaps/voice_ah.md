# afterhours gaps — voice_ah

## 2026-10-24 — Open dropdown list draws under a later text_area (render layers are global, the list only gets +1)

Submodule: `vendor/afterhours` @ 1a54a7f.

- **Symptom:** a `dropdown` sits above a `text_area` in a column. When the dropdown is opened, the
  first few options are hidden behind the text area's box and its text. The options that reach past
  the text area show normally and cover the widgets below it.
- **Repro:** a column div with a `dropdown` (8 options, default render layer) followed by
  `text_area(... .with_size({percent(1.f), pixels(110.f)}))`. Open the dropdown.
- **Root cause:**
  - `render_layer` is one global sort key. `apply_render_layer` (`component_init.h`) gives a child
    `max(config.render_layer, parent_layer)`, and every element is queued into one render list.
  - `dropdown` (`imm_containers.h`) puts the options tray at `config.render_layer + 1`, so a dropdown
    at layer 0 has its open list at layer 1.
  - `text_area` (`text_input/text_area.h`) uses the same kind of offsets for its own parts: box
    `+1`, text lines `+2`, selection `+5`, cursor `+10`.
  - The text area is queued after the dropdown, so its box (layer 1) ties the tray and draws after
    it, and its lines (layer 2) draw above it.
  - `text_input/component.h`, `imm_value.h` (slider) and `menu.h` also use offsets of +2 to +10, so
    any of them placed after an open dropdown can paint over it too.
- **Expected:** an open dropdown list (and other popups: tray, tooltip-like overlays) draws above all
  normal content, whatever widgets come after it.
- **Suggested fix:** give popups a reserved overlay band instead of `+1`. For example, the tray goes
  to `config.render_layer + OVERLAY_BASE`, where `OVERLAY_BASE` (e.g. 1000, the value the drag
  overlay in `systems.h` already uses) is above any widget's internal offset. Another option is to
  render popups in a second pass after the main list. Either way, widget-internal offsets should
  stay below the popup band. A test: dropdown followed by text_area; with the dropdown open, the
  tray's rect should be the top-most hit at a point inside the text area.
- **Workaround in voice_ah:** `src/main.cpp` gives both dropdowns `.with_render_layer(DROPDOWN_LAYER)`
  with `DROPDOWN_LAYER = 20`. The tray lands at 21, above text_area's cursor at +10. Remove it once
  dropdown popups use an overlay layer.

## 2026-10-24 — Procedural audio: generated sounds have to go around afterhours to raylib

Submodule: `vendor/afterhours` @ 1a54a7f.

voice_ah generates every sound at runtime: it turns a line of text into mono float samples, plays them,
and stops early on Skip. Every raylib call it makes is in `src/main.cpp` (`play()`, `skip()`, `main()`).

**Our own shortcut, not a gap:** `InitAudioDevice`, `CloseAudioDevice`, `PlaySound`, `UnloadSound`
and `LoadSoundFromWave` all have wrappers in `src/plugins/audio_helpers.h`. voice_ah called the
`raylib::` versions anyway. That's on the app side and we'll switch to the wrappers. The gaps below
have no wrapper.

### Every raylib call in voice_ah

| Call (`src/main.cpp`) | Used for | afterhours today |
|---|---|---|
| `raylib::InitAudioDevice()` | `main`, before `ui::run` | Wrapper exists (`afterhours::InitAudioDevice`); see gap 6 |
| `raylib::CloseAudioDevice()` | `main`, after `ui::run` | Wrapper exists (`afterhours::CloseAudioDevice`); see gap 6 |
| `raylib::Sound` | the line's sound handle | `afterhours::SoundType` exists |
| `raylib::Wave{.frameCount, .sampleRate, .sampleSize, .channels, .data}` | float samples → wave | **Gap 1**: `WaveType` is a raw typedef, no builder |
| `raylib::LoadSoundFromWave(wave)` | wave → sound | Wrapper exists (`afterhours::LoadSoundFromWave`) |
| `raylib::PlaySound(sound)` | Play | Wrapper exists (`afterhours::PlaySound`) |
| `raylib::StopSound(sound)` | Skip | **Gap 2**: no wrapper |
| `raylib::UnloadSound(sound)` | replace the previous line | Wrapper exists (`afterhours::UnloadSound`) |

The other parts of voice_ah use only afterhours: the UI, the font (`FontManager::load_font`) and
screenshots (`graphics::capture_every_n_frames`, in a scratch harness).

### Gaps

1. **Building a sound from samples needs raw `raylib::Wave` fields.**
   - Every synth app needs float samples → `SoundType`. Today that means filling in
     `raylib::Wave{.frameCount, .sampleRate, .sampleSize, .channels, .data}` by hand. `WaveType` is a
     typedef, so the raylib field names leak through.
   - Two apps have written this themselves:
     - mailboy `src/audio_bank.cpp` `sound_from_float_samples` (16-bit, with a malloc and
       UnloadWave dance, plus a comment about ADL ambiguity with raylib's own functions);
     - voice_ah (32-bit float).
   - Ask: `SoundType load_sound_from_samples(std::span<const float>, int sample_rate, int channels = 1)`
     in `audio_helpers.h`, with a no-op stub for other backends. It should copy the data, so callers
     can keep their own buffer.
2. **Missing playback wrappers.**
   - `StopSound`: voice_ah's Skip stops a line partway through. There's no wrapper, so it calls
     `raylib::StopSound`.
   - `SetSoundPitch` / `SetSoundPan`: mailboy pitch-shifts one synthesized sample per note with
     `raylib::SetSoundPitch` (`src/audio_bank.cpp:276`).
   - `PauseSound` / `ResumeSound`.
   - Ask: add these next to `PlaySound` / `SetSoundVolume`.
3. **`SoundLibrary` only loads from files.**
   - `sound_system::SoundLibrary::load(filename, name)` is the only way in. A generated `SoundType`
     can't be registered, so it misses master and sound volume, `play_random_match`, aliasing and
     `PlaySoundRequest`.
   - Ask: `SoundLibrary::add(name, SoundType)`, which applies the current volume like `load` does
     and unloads with the rest.
4. **No streaming or callback audio.**
   - voice_ah renders the whole line before it can play. Real-time synthesis (sliders that change a
     line while it plays, endless babble, sample-accurate sync between text and audio) needs
     raylib's `AudioStream` + `SetAudioStreamCallback`. afterhours wraps `Music` streams but not
     raw `AudioStream`.
   - Ask: a small `AudioStreamType` wrapper (load, play, stop, unload, plus set a callback that
     fills float frames).
5. **No playback-position query.**
   - To keep the typed-out text in step with the voice, voice_ah adds up frame `dt` from Play,
     because `Sound` has no "time played". That can drift from the audio clock.
   - This would be solved by #4 (count frames in the callback), or by a `Music`-style
     `time_played` for sounds started through afterhours.
6. **`ui::run` doesn't cover audio.**
   - `ui::run` handles the window, input and the frame loop, but an app that plays sound still has
     to bracket it with `InitAudioDevice()` / `CloseAudioDevice()` in `main`.
   - Ask: `graphics::Config::audio` (default false), so `run()` / `graphics::init` start and stop
     the audio device, or an RAII `audio::Device` guard.
7. **No WAV export from samples.**
   - Offline renders and listening checks need float samples → `.wav`. `ExportWave` needs a
     `raylib::Wave`, and in the stub backend it returns false.
   - mailboy (`src/song_render.cpp`) and voice_ah (a scratch tool) each wrote a RIFF writer by hand.
   - Ask: a backend-independent `bool export_wav(path, std::span<const float>, int sample_rate, int channels = 1)`.
     It's only plain file writing, so it should work headless and in tests.
8. **No audio on the non-raylib backends, and no warning.**
   - Under sokol/Metal and `none`, `audio_helpers.h` swaps in stubs, so `PlaySound` silently does
     nothing. A Metal app gets no sound and no log line.
   - Ask: at least a one-time `log_warn` from the stub `InitAudioDevice` / `PlaySound`. Longer term,
     a sokol_audio backend (sokol is already vendored) would bring audio to those builds.
9. **Audio can't be checked in tests.**
   - Headless e2e runs can't assert that a sound played or what it contained. voice_ah works around
     this by testing the synth offline (`src/voice_test.cpp`).
   - Ask: a "record" device for tests that logs `PlaySound` calls (name or hash, time) and can dump
     what was mixed. It could share the hook point with the stubs in #8.
