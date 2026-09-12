# Mock parity audit

The target is `mocks.html`. Published gallery: https://pxl.cl/d8w3j.
The Compare view overlays the source image on the mock or current C++ screenshot.
Drag the divider, or use the range control with the arrow keys.
The afterhours pin is frozen at `e3f13a7827e333460d38078fde716e1e4a5a68ef`.

## Captures

Run builds, tests, captures and processing jobs with `nice -n 10`.

`python3 scripts/bundle_mocks.py` produces `output/mocks-standalone.html`
and a SHA-256 asset manifest. The export embeds comparison images and fonts.

With local Chrome exposing CDP on port 9322:

```
nice -n 10 node scripts/capture_mocks.mjs --comparisons
```

This captures the mock canvases at 1280×720, without gallery controls or scaling,
into `output/screen-audit/mock-targets`. Use `--screen` to capture one screen.
Use `--url` to check the standalone export outside the repository.

For native captures:

```
nice -n 10 ./output/ui_tester.exe --headless-screenshots --screen SCREEN --resolution 720p --image-output output/screen-audit/SCREEN/after/
nice -n 10 ./output/ui_tester.exe --headless-screenshots --screen SCREEN --resolution 1080p --image-output output/screen-audit/SCREEN/after/
```

Use separate processes per resolution: combined capture currently crashes on
collection cleanup, reproduced with unchanged `example_borders`.

Before building, run `python3 scripts/check_screen_fonts.py HEADER...` to catch
font filenames accidentally used as registry aliases.

Every screen needs fresh native captures, matched interaction states, and
independent visual reviews. Old native baselines only detect regressions.
Keep existing gameplay and add meaningful mock interactions. Gallery phase
shortcuts are review controls, not new game mechanics. Decorative textures
must not contain interactive controls, changing values, or labels.

## Progress

Screens follow the gallery dropdown order. Findings and their disposition are
recorded in `docs/AFTERHOURS_GAPS.md`; this table records delivery and checks.

| Screen | Status | Verification |
|---|---|---|
| potion_crafting | Updated; minor visual gaps recorded | E2E 40 passed including Tab/Enter, stock consumption, shortage, tabs, 1080 resize; fresh 720p/1080p images; two independent reviews. |
| angry_birds_settings | Updated; minor visual gaps recorded | E2E 128 and responsive/containment checks passed; 720p/1080p and dialog captures; independent visual review. |
| cozy_cafe | Updated; cosmetic gaps recorded | E2E 129 passed, including Tab/Enter, service depletion, music and resize; 720p/1080p reviewed. |
| casual_settings | Updated; minor visual gaps recorded | E2E 130 passed including dialog containment, keyboard use, save/load and 1080 resize; independent visual review. |
| fighter_menu | Updated; perspective/type gaps recorded | E2E 131 passed; pointer/keyboard option selection and resize; final 720p/1080p independent review. |
| deadspace_settings | Pending | |
| empire_tycoon | Pending | |
| flight_options | Pending | |
| shop_interface | Pending | |
| islands_trains_settings | Pending | |
| kirby_options | Pending | |
| marlo_kart | Pending | |
| minesweeper_lab | Pending | |
| mini_motorways_settings | Pending | |
| neon_strike | Pending | |
| parcel_corps_settings | Pending | |
| powerwash_settings | Pending | |
| race_results | Pending | |
| sports_settings | Pending | |
| rubber_bandits_menu | Pending | |
| kart_select | Pending | |
| guess_who_lab | Pending | |
| aim_chat | Pending | |
| media_library | Pending | |
| offsite_backup | Pending | |
| secure_tunnel | Pending | |
