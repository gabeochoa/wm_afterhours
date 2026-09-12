# Approved gap implementation

The user requested all approved gaps, each in its own local commit. No pushes.
Apply review feedback by rewriting the original relevant commit, preserving
later changes. Add no code comments; leave existing comments unchanged unless
they become false. Prefer early returns. Run work with `nice -n 10`, builds
with `-j2`.

Each library change has its own afterhours commit and a wm adoption commit.

| Work | Status |
|---|---|
| UP-01 Audio gains | Complete, afterhours `1fce0d9` |
| UP-02 Settings saves | Complete, afterhours `325caee` |
| UP-03 Scoped test clipboard | Complete, afterhours `b81173b`; 15/15 checks including both text widgets |
| UP-04 Native dialogs, portable API, macOS first | Complete, afterhours `544ea06`; queue and real macOS cancellation tests pass |
| UP-05 Complete binding persistence | Complete, afterhours `00a6745`; codec checks pass with no backend and raylib |
| UP-06 Automatic binding prompts with device override | Complete, afterhours `bcefe22`; modifiers, axis direction, device switching, noise, override and remapping checks pass |
| UP-09 Filesystem watcher, portable API, macOS first | Complete, afterhours `f4de4d3`; real filesystem lifecycle and overflow checks pass |
| UP-10 Explicit capture formats | Pending |
| UP-11 Default profiling UI and compile-out support | Pending |
| UP-12 Charts required by profiler, interactive wm test screen | Pending |
| wm searchable screen tree | Pending |

UP-07 and UP-08 remain deferred. Sound-feedback and periodic timers remain
skipped. The wider chart set remains TODO. Performance investigations and
older visual gaps retain their separate recorded scope.

A separate subagent is preparing a plugin architecture proposal grounded in
current consumers. That work is a plan only, not a library migration.
