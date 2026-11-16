# decide: memory accounting and budgets (Tier 2: hanabi, floatinghotel)

**Problem.** There are four separate asks under one row:
- **Byte-budgeted cache** (floatinghotel U3). It carries `byte_cache.h` plus a second hand-rolled accounting cache, with 4/32/32 MiB budgets. It wants a key type + cost function, rejection of oversize entries, and stats for hits/misses/evictions/accounted bytes. The values are host-owned, and it must say clearly that accounted bytes aren't process memory.
- **Memory-pressure event** (hanabi #586). It wants an OS pressure notification, delivered once through `SystemManager`, so app caches can shrink.
- **GPU bytes in use** (hanabi #126). A 114 MB texture leak was invisible to malloc stats. sokol keeps the bookkeeping (`sg_frame_stats`, pool usage) but the backend doesn't expose it.
- **Per-system retained-byte attribution** (hanabi #585).

## Options

1. **The byte-budgeted LRU only.** `afterhours::ByteCache<K, V>` with `cost(V)`, `budget`, `get` / `put` (returning whether the entry was admitted), `erase`, `clear`, `shrink_to(bytes)`, and `stats()`. Single-threaded; callers add a lock if they need one, as floatinghotel already does. Header-only, std only, fully unit-testable, and floatinghotel can delete two caches. About 150 lines + tests + example.
2. **Option 1 plus a pressure hook.** `memory::on_pressure(fn)` registry. macOS uses a `dispatch_source` memory-pressure source; Linux and Windows are no-ops at first. Callbacks are queued and run at the start of the next `SystemManager::run`, so handlers are frame-safe. Caches from option 1 register `shrink_to(budget / 2)`. The OS side is only testable by simulating the event, but the queue/drain part is unit-testable.
3. **GPU bytes.** `graphics::gpu_texture_bytes()`, computed from the format and mip chain at create and destroy in each backend's texture path (raylib, sokol/Metal, sokol/Vulkan). Accurate, but it touches every backend, and Metal changes have to be tested from floatinghotel.

Per-system attribution (#585) needs allocator hooks per system. Defer it; nothing short of a tracking allocator does it honestly.

## Recommendation

Do option 1 now as its own small PR; it's concrete and two caches move onto it. Option 2 next, if Gabe agrees afterhours should own an OS event. Option 3 is a backend-touching PR for whoever owns the Metal/sokol work. Defer #585.

**Rough size:** 1 is S-M, 2 is M, 3 is M (three backends).
