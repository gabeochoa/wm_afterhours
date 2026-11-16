# decide: versioned storage codecs (floatinghotel, MyNameChef, Tier 2)

## Problem
afterhours can write a file atomically (gap/atomic) and project components into DTOs (`snapshot::take_components`). `settings::ProvidesSettings<T>` hands conversion to the user type through `to_json`/`from_json`, `to_string`/`from_string`, or bitsery. None of these has:
- a schema **version** stored with the payload
- **migrations** from older versions
- **typed decode errors** (wrong type, missing field, unsupported version, corrupt). Today `from_json` either throws or returns defaults, and the caller can't tell which happened.
- **transactional publish**: decode into a temporary and touch live state only when the whole document is valid (floatinghotel does this by hand because components can't be copy-assigned).

MyNameChef also wants a checksum envelope and snapshot "profiles" (battle, save, report). floatinghotel only wants the versioned decode with errors.

## Options
1. **Small `persist::Codec<T>` header, format-agnostic (recommended).**
   - `struct Codec<T> { int version; std::function<std::string(const T&)> encode; std::function<std::expected<T, DecodeError>(std::string_view)> decode; std::map<int, Migration> migrations; }`
   - `persist::save(path, codec, value)` writes a `{"v":N}` header line plus the payload through the atomic writer.
   - `persist::load(path, codec)` reads the header and runs migrations `v -> v+1 ... -> current` on the raw payload string. It returns `expected<T, DecodeError>`. `DecodeError` is an enum (`missing_file`, `unsupported_version`, `corrupt`, `invalid`) plus a message.
   - The caller publishes the value into components itself, so the transactional part is just "load returns a value, not an entity".
   - No JSON dependency; the payload format is the user's. Settings can adopt it later behind its existing API.
   - About 150 lines plus a test and an example. Fits one PR.
2. **Extend `settings` with `version` + `migrate(int from, json&)` hooks.** Smaller (about 60 lines), but JSON-only and settings-only. MyNameChef's save files and reports don't fit, and the bitsery and raw paths get nothing.
3. **Full MyNameChef AH-06**: codec registry, checksum envelope, snapshot profiles and round-trip helpers. Covers everything asked for, but it's 500+ lines, has several API surfaces to review, and bakes in a compatibility policy the projects disagree on. Too big for one PR, and against "keep PRs small".

## Recommendation
Option 1. It solves floatinghotel's need completely and gives MyNameChef the version, migration and typed-error core. Checksums can come later as one more step in the codec (`verify` before `decode`) once a second project asks.

## Questions for Gabe
- Header format: a one-line `afterhours-persist v3` prefix (works for any payload) or a JSON wrapper `{"version":3,"data":...}` (readable, but forces JSON)? I recommend the prefix.
- `std::expected` needs C++23, and the tests build with `-std=c++20`. Use a small `Result<T>` or `std::variant<T, DecodeError>` instead?
- On `unsupported_version` (a newer file opened by an older build), should `load` refuse, or return a default and leave the file untouched? I recommend refusing and letting the caller decide, so nothing overwrites a newer save.

Rough size: option 1, about 150 src lines + about 120 test lines + an example, one PR.
