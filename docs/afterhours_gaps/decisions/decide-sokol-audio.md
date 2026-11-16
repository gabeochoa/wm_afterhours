# decide: real audio on the sokol backend (voice_ah #8, raylib usage audit priority #2)

**Problem.** Under sokol (Metal, Vulkan) and `none`, `plugins/audio_helpers.h` swaps in no-op stubs, so `PlaySound` is silent with no log line. The raylib usage audit counts 169 audio uses across 11 projects. floatinghotel and hanabi are the sokol consumers; neither plays sound today, but any game moving to sokol loses audio silently. gap/sound-wrappers and gap/audio-lifecycle cover the API shape on raylib, with stubs and a warning on sokol.

## Options

1. **Vendor miniaudio** (single header, what raylib's raudio is built on: CoreAudio, WASAPI, ALSA/Pulse, WebAudio). Back `SoundType` / `MusicType` / `WaveType` with `ma_sound` / `ma_decoder` under a non-raylib `audio_helpers.h`: load (WAV/MP3/FLAC/OGG via stb_vorbis), play/stop/pause/resume, volume/pitch/pan, music streaming via `ma_sound` stream flags, device init/close tied to `Config::audio`. Behaviour then matches raylib. About 400-600 lines plus a ~90k-line vendored header and one impl TU, like sokol_impl. It's testable headless with miniaudio's null backend (`ma_device` with `ma_backend_null`), so CI can assert play/stop state without hardware.
2. **sokol_audio** (already vendored): it's only a PCM push/callback stream, with no decoding, mixing, pitch or pan. All of that would have to be written on top, which is far more code than option 1, with worse format support.
3. **Warnings only.** A one-time `log_warn` from the stub `InitAudioDevice` / `PlaySound` (voice_ah's short-term ask), so a Metal app at least knows it's silent. About 10 lines.

## Recommendation

Option 3 now (tiny, removes the silent failure), and option 1 when the first sokol game needs sound. miniaudio is the same engine raylib uses, so sound behaves the same on both backends. Option 2 means reimplementing a mixer.

**Rough size:** 3 is XS; 1 is L (vendor drop + backend + null-device tests).
