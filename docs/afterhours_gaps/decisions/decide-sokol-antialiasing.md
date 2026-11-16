# decide: antialiasing on sokol (Tier 3: hanabi #92 / #106)

**Problem.** The sokol backend pins MSAA off in both setup paths: `desc.sample_count = 1` for windowed (`sapp_desc`, backend.h:943) and `desc.environment.defaults.sample_count = 1` for headless (:1005). `sgl` has no per-primitive AA, so every shape (dot, chevron, arc, ring) has binary edges while text, sampled from an AA'd atlas, is smooth. hanabi measures the aliasing as worth more of its reference diff than drawing the right glyph. raylib honours `Config::enable_msaa` (`FLAG_MSAA_4X_HINT`). sokol has no knob at all: `RunConfig` has `flags`, and sokol ignores `FLAG_MSAA_4X_HINT`. The init comment at :340 even says the default sgl context "matches the swapchain (4x MSAA)".

Two very different sizes of work:
- **Windowed** is small: `desc.sample_count = (cfg.flags & FLAG_MSAA_4X_HINT) ? 4 : 1`. The default sgl context and pipelines take the sample count from `sglue_environment()`, and offscreen targets already draw through per-sample-count sgl contexts (`sgl_context_for`). But it can only be verified with a real window on Metal; nothing headless here can show it.
- **Headless/offscreen** (what hanabi's reference captures and every E2E screenshot use) needs MSAA render textures: a 4x color+depth attachment plus a 1x resolve image, a pass that resolves, and `tex_view` / capture / readback pointing at the resolve image. `load_render_texture`, `RenderTextureType`, both capture paths (Metal, Vulkan) and the resize path all change. That's moderate work, and it must be verified on Metal and Vulkan.

## Options

1. **Windowed flag only.** Map `FLAG_MSAA_4X_HINT` (and/or add `RunConfig::msaa_samples`) to `sapp_desc.sample_count`. About 5 lines; hanabi's live window gets AA. Screenshots stay aliased, so windowed and captured output differ.
2. **Windowed + MSAA offscreen targets with resolve**, honouring `Config::enable_msaa` in headless too. Windowed and captured output then match. M-sized, touching drawing_helpers.h, both capture backends and the resize path. Testable headless by drawing a diagonal and asserting partial-coverage pixels in the readback.
3. **Analytic AA in the shape helpers** (feathered edge geometry in `draw_circle` / `draw_line_ex` / rings). Works at 1x without MSAA, but every primitive needs rewriting, and #106 shows blended fringes over unknown backgrounds have their own problems.

## Recommendation

Option 2, in two PRs: (a) the MSAA render target + resolve with a headless coverage test, then (b) the windowed flag. That keeps E2E captures and the live window in agreement, which is the whole point of hanabi's parity work. Option 1 alone makes the window and the screenshots disagree.

**Rough size:** (a) M, (b) XS. Both need Metal runs from floatinghotel/hanabi per the backend-testing rule.
