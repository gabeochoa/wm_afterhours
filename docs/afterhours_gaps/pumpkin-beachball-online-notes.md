# afterhours-online gaps and backlog

Last reviewed: 2026-11-05

This file records gaps found by using `afterhours-online` from Pumpkin
Beachball. It complements:

- `afterhours_gaps.md` for core Afterhours rendering, input, UI, and
  E2E asks, including audit item Q.
- `docs/afterhours-online-plan.md` for the original plan and decisions.
- `vendor/afterhours-online/docs/e2e-testing.md` for the library's
  transport-test strategy and its two current GNS `XFAIL`s.
- `vendor/afterhours-online/docs/gaps.md` for the library-owned
  version of the transport/API backlog. This file keeps the
  consumer evidence and game-integration consequences.

The checked revisions were Pumpkin Beachball `73de546`,
`vendor/afterhours-online` `57b54f5`, and `vendor/afterhours`
`d898e7c`. File and line references below describe those revisions.

## How to read this file

Call the entries below `OG-nn` when referring to them elsewhere.

- **Confirmed gap**: current behavior is missing, inconsistent, or can
  leave a consumer without a usable signal.
- **Design decision needed**: the implementation made a defensible
  choice, but it differs from an earlier stated goal and needs either
  new work or an explicit decision to keep it.
- **Future scope**: not promised for v1 and not a defect today. It is
  recorded here so the limit is visible rather than rediscovered.

## Current implementation snapshot

- `vendor/afterhours-online/src/online/` contains `core.h`,
  `serialize.h`, `local.h`, `online.h`, `testing.h`, `gns.h`,
  `gns_impl.h`, and `gns.cpp`. There is no `plugin.h`, ECS component,
  or ECS system in the library.
- `core.h` is intentionally transport-only. It exposes peer IDs,
  channels, `Connected`/`Disconnected`/`Packet` events, and a
  `Transport` interface. The game owns session state, side and
  character assignment, ECS writes, snapshot interpolation, prediction,
  send pacing, and HUD/status text.
- Standalone GNS is the only socket backend. A Steamworks backend can
  reuse `gns_impl.h` later because both expose
  `ISteamNetworkingSockets`, but no Steam backend exists yet.
- The game handshake checks two values independently:
  `afterhours::online::kProtocolVersion` is still `1`, while the game
  label hash is `protocol_hash("pumpkin-beachball-v3")`. In other words,
  "v3" is the game's wire label, not version 3 of the library protocol.
  Both values must match or the server sends `Bye("version mismatch")`
  and disconnects.
- The game wire types are `Input=1`, `Snapshot=2`, `Hello=3`,
  `Welcome=4`, `Bye=5`, `Ping=6`, and `Pong=7`.
- The game supports listen host and dedicated authoritative servers,
  two player sides, direct numeric IP targets, reconnect by starting a
  new join, 30 Hz inputs, 20 Hz snapshots, a fixed six-tick
  interpolation delay, own-player prediction, and one-second RTT
  probes after a peer is welcomed.
- The transport test matrix has 24 scenario definitions. The game
  process harness has 14 always-registered scenarios and 2 additional
  windowed scenarios under `--windowed`.

## OG-01: A GNS client can fail before `Connected` without a terminal event

Status: closed 2026-11-05 in afterhours-online `35ecb60`, plus the
game-side deadline described below. Severity was high for connection
UX.

Evidence:

- The upstream transport scenario `connect_without_server` records this
  as an `XFAIL`: an unreachable client does not receive a prompt
  terminal `Disconnected` event.
- In `gns_impl.h`, the client branch of
  `k_ESteamNetworkingConnectionState_ClosedByPeer` and
  `k_ESteamNetworkingConnectionState_ProblemDetectedLocally` only
  queues `Disconnected{kServerPeer}` when `client_connected` is true.
- Pumpkin Beachball's `unreachable_client` scenario therefore expects
  the client to remain in the Online menu with status
  `Connecting to ...`; it does not expect an automatic failure.
- `net_start_client` records no deadline. Once address parsing and
  `connect()` initiation succeed, only a transport event, Welcome,
  Bye, or the player's Cancel action ends the attempt.

Impact: a typo in a reachable-looking IP address, a closed UDP port, or
a host that disappears during connection can leave the game saying
"Connecting" indefinitely. Every consumer has to infer failure from
the absence of success.

Resolution:

1. The GNS client now queues one `Disconnected{kServerPeer}` when its
   connection reaches a closed/problem state before `Connected`.
   Local close paths unregister the connection first and do not
   double-report. A probe observed GNS's own unreachable timeout at
   10.012 seconds.
2. `connect_without_server` now waits up to 12 seconds and expects the
   event under `--strict-gaps`.
3. The game handles early disconnects as `Connect failed: server
   unreachable` (or `server closed connection` after Hello) and also
   has a 12-second frame-dt deadline for endpoints that accept the
   transport connection but never send Welcome. Its
   `unreachable_client` scenario now expects the offline role and a
   `Connect failed` status instead of permanent `Connecting to ...`.

## OG-02: Transport-capacity rejection is silent

Status: closed 2026-11-05 in afterhours-online `35ecb60` for the
terminal-event guarantee. Severity was medium. Distinguishing the
reason remains OG-03.

Evidence:

- The GNS server cap is four remote peers (`kMaxPeers = 4`). At
  capacity, `on_status_changed` closes the new connection without
  accepting it or queueing a rejection event for either endpoint.
- The upstream `capacity_and_extra_client` scenario records the client
  side as an `XFAIL`: the extra client is not welcomed, but it also
  receives no explicit terminal event.
- Pumpkin Beachball's normal third-player path is different. The
  transport accepts the peer because four peers fit, the game receives
  its Hello, finds both sides taken, sends `Bye("match full")`, and
  disconnects it. That game-layer path is covered; it does not fix a
  fifth simultaneous transport peer or another consumer whose game cap
  equals the transport cap.

Resolution: the rejected client now receives the same terminal
`Disconnected` event as OG-01, and
`capacity_and_extra_client` expects it under `--strict-gaps`. If the
event grows a reason field, distinguish at least unreachable, closed
by peer, local close, and capacity rejection.

## OG-03: Events do not carry disconnect reasons or connection diagnostics

Status: closed 2026-11-05. `Disconnected` now carries a portable
`DisconnectReason`, the game turns it into specific status text
(server full, local network problem, unreachable), and `Transport`
gained an optional `connection_stats()` query backed by GNS real-time
status. The remaining diagnostics depth is OG-07.

Evidence:

- Core events are only `Connected { PeerId }`,
  `Disconnected { PeerId }`, and `Packet`. There is no reason enum,
  native error code, remote address, connection state, quality, or
  debug string in the portable event surface.
- The GNS backend receives `m_eEndReason` and `m_szEndDebug` in its
  status callback, prints them to stderr, and then drops that data.
- GNS exposes richer connection state through `GetConnectionInfo` and
  `SteamNetConnectionRealTimeStatus_t` (state, ping, local/remote
  quality, byte and packet rates, pending bytes, queue time, jitter),
  but `Transport` has no `connection_info` or `stats` query.
- Pumpkin Beachball therefore implements its own application-level
  Ping/Pong only to obtain one smoothed RTT number for the HUD.

Impact: games can say "Disconnected" but usually cannot explain
whether the peer closed cleanly, the endpoint was unreachable, a local
problem occurred, or the server rejected the connection. Support logs
also depend on stderr text that is not part of the API.

Resolution: exactly that shape landed. The GNS backend maps native
end reasons onto the enum (with a dedicated app-range code for
capacity rejection), LocalTransport reports `LocalClose` to the
initiator and `ClosedByPeer` to everyone else, and the transport
matrix plus unit tests assert the reasons. Native debug strings still
go to stderr only; no portable diagnostic string was added.

## OG-04: The planned ECS plugin layer does not exist

Status: design decision needed. Severity: high relative to the
original project goal, low for the already-working game. Owner:
`afterhours-online` architecture.

Evidence:

- The original goal was a simple Afterhours multiplayer plugin/addon
  that works easily with the ECS. The plan named `plugin.h`,
  `ProvidesOnlineSession`, peer/input components, and poll/send/session
  systems.
- The shipped library has no `plugin.h`, ECS component, singleton, or
  system. `core.h` explicitly says it has no ECS dependency.
- Pumpkin Beachball consequently owns the whole integration in game
  code: `NetSession`, peer-to-side and peer-to-character maps, remote
  intent latches, Hello/Welcome handling, snapshot application,
  prediction, disconnect cleanup, menu state, and status text. That
  code is the de facto reference integration, but a second game would
  mostly copy it rather than register a plugin.

Impact: the transport is reusable today; the ECS session pattern is
not. Calling the library a "plugin" overstates the current surface.

Decision needed:

1. Add an optional ECS layer in a separate header (for example
   `ecs.h` or `plugin.h`) while keeping `core.h` dependency-free. It
   could own session state, peer/side bookkeeping, and transport
   polling, leaving game-specific packets and snapshot application to
   the consumer; or
2. Explicitly redefine `afterhours-online` as transport plumbing only
   and remove the ECS-plugin expectation from the plan and README.

Do not solve this by putting Slime packet types into the library. The
useful shared boundary is session and transport bookkeeping, not one
game's snapshot schema.

## OG-05: Snapshot interpolation, send pacing, and prediction are consumer-built

Status: confirmed reusable-work gap. Severity: medium. Owner:
`afterhours-online`, with game-specific prediction remaining in games.
This expands item Q in `afterhours_gaps.md`.

Evidence:

- The library ships transport, serialization, sequence tracking, and
  fault injection. It has no snapshot buffer, interpolation sampler,
  jitter/lag adjustment, send-rate gate, or connection-stats helper.
- Pumpkin Beachball keeps a `std::deque<MatchSnapshot>`, samples six
  server ticks behind the newest snapshot, lerps only positions, takes
  velocities and scores from the newest snapshot, and clears the buffer
  on serve.
- Input and snapshot pacing are also game code: 30 Hz wall-clock input
  sends, 20 Hz snapshot broadcasts, and reliable phase-change
  snapshots.
- Own-player prediction duplicates only part of server movement. It
  does not predict player-player collision; reconciliation blends
  toward the authoritative position or snaps when far away or on serve.

Limits to record, not hide:

- The interpolation delay is fixed at six ticks. It does not grow or
  shrink with jitter, packet loss, snapshot inter-arrival time, or the
  RTT now measured by Ping/Pong.
- There is no extrapolation policy when snapshots stop arriving.
- Snapshot tick order is accepted, but there is no generic stale,
  duplicate, or out-of-order accounting for game snapshots. The
  library sequence envelope exists for tests/consumers, but the game
  snapshot path does not use it.
- Input packets carry a tick, but there is no input acknowledgement,
  redundancy, or replay. A lost unreliable packet can lose a one-frame
  jump press; later packets describe the currently held state, not the
  missed edge.

Suggested fix: add transport-agnostic headers for a bounded snapshot
buffer/sampler and a wall-clock rate gate. Leave schemas, movement
prediction, reconciliation thresholds, and authority rules in the
game.

## OG-06: Join targets accept hostname-shaped text but only resolve IP literals

Status: confirmed consumer-facing mismatch. Severity: medium for LAN
usability. Owner: game target parser and/or `afterhours-online`
endpoint API.

Evidence:

- The game's `parse_join_target` accepts letters, digits, dot, hyphen,
  and underscore, and its comment calls those DNS names.
- No game or library code calls a DNS resolver. The GNS backend forms
  `host:port` and passes it to `SteamNetworkingIPAddr::ParseString`,
  which parses an IP address.
- `bad_address` proves `not-an-ip` is rejected at connect time. A real
  hostname can pass the game's text validation and then fail with the
  generic "Connect failed: bad address" status.
- IPv6 is also split across layers: GNS can parse bracketed IPv6
  endpoints, but the game parser rejects more than one colon and does
  not accept brackets.

Impact: players can type a plausible LAN hostname that the field
appears to accept, then get a misleading connection failure. IPv6
literals are unavailable from the UI even though the backend address
type supports them.

Suggested fix: pick one contract and make every layer agree. Either
resolve DNS before `connect()` (with clear resolution errors and an
IPv4/IPv6 choice policy), or label and validate the field as a numeric
IP endpoint and add bracketed IPv6 parsing. Add parser and game E2E
cases for a hostname, an IPv4 literal, and an IPv6 literal.

## OG-07: RTT is one aggregate value and never goes stale

Status: confirmed limitation of the new Ping/Pong implementation.
Severity: low for a two-player listen host or client, medium for a
dedicated server or future diagnostics. Owner: game today;
`afterhours-online` if OG-03 adds connection stats.

Evidence:

- The server sends the same sequence number to every welcomed peer and
  stores one send timestamp in `ping_sent[seq]`.
- The first matching Pong erases that entry and updates the single
  `rtt_ms`; a second peer's Pong for the same sequence is ignored.
  The dedicated server therefore measures the fastest responder that
  second, not a per-peer round trip.
- `rtt_ms` is a 75/25 smoothed value with no sample age. It is cleared
  when the session stops, but it does not expire while a session stays
  up and Pongs stop arriving.
- No loss, jitter, last-seen-Pong, bytes/sec, or queue-depth value is
  exposed to the HUD or logs.

Impact: the current HUD number is useful for a client and for a listen
host with one guest. It is not a per-connection health signal for a
dedicated server, future spectators, or more peers.

Suggested fix: key outstanding probes by `(peer, seq)`, keep RTT and
last-sample time per peer, expire samples after a few missed probes,
and prefer native transport stats from OG-03 when that API exists.
Keep the application Ping/Pong as an end-to-end game-loop check: it
proves packet dispatch and game polling work, which transport ping
alone does not.

## OG-08: Distribution still depends on a local, macOS/Homebrew GNS build

Status: confirmed release gap. Severity: high for the original
"clone and play" goal, low for development on the current machine.
Owner: `afterhours-online` release pipeline.

Evidence:

- The only GNS acquisition script is `scripts/build-gns.sh`. It clones
  Valve's repository at `v1.6.1`, assumes CMake plus Homebrew
  protobuf/Abseil/OpenSSL locations, and builds locally for one or two
  minutes.
- There is no `fetch-gns.sh`, checksum file, CI workflow, release tag,
  or published per-platform artifact in the checked-out library.
- The GNS static archive is static, but its Homebrew protobuf and
  Abseil dependencies remain dynamic on this dev machine. OpenSSL is
  linked statically. Ship builds therefore still need dependency
  inspection (`otool -L` on macOS, `ldd` on Linux) instead of assuming
  the archive is self-contained.
- The library repository has no top-level `LICENSE` and no
  compatibility matrix stating which afterhours revision, compiler,
  operating systems, and architectures a given online revision supports.
  The license file under the generated `vendor/gns/` tree is the GNS
  license, not a license for `afterhours-online` itself.
- The library README still says the folder is kept self-contained so
  it "can be split into its own repository" even though this checkout
  is that separate repository, vendored back into the game as a
  submodule.

Impact: the original success criterion, no local GNS/protobuf build on
clone, is not met yet. A fresh clone needs network access, Git, CMake,
and compatible Homebrew packages, and only the current macOS path is
exercised.

Suggested fix:

1. Build fully static GNS artifacts in CI for the supported platforms.
2. Publish versioned assets with SHA256 sums and add the planned
   verified fetch script.
3. Add the library license, a compatibility table (online revision,
   afterhours revision, GNS/interface version, OS/arch, compiler), and
   a fresh-clone CI job that uses the fetched artifact.
4. Make the packaging check fail if the shipped binary gains an
   unexpected dynamic protobuf or Abseil dependency.

## OG-09: The Steam backend is only an implementation seam, not a backend

Status: future scope with one current documentation mismatch.
Severity: low for standalone GNS v1. Owner: `afterhours-online`.

Evidence:

- There is no `steam.cpp`, Steamworks initialization, SDR listen or
  connect path, lobby integration, or AppID handling.
- `gns_impl.h` says the including TU hands it an
  `ISteamNetworkingSockets*`. The current implementation does not
  accept that pointer from the TU: `gns_acquire()` itself calls
  `GameNetworkingSockets_Init`, `SteamNetworkingSockets_LibV12()`,
  `SteamNetworkingUtils_LibV4()`, and `GameNetworkingSockets_Kill()`.
  The connection, poll-group, send, receive, and callback logic should
  be reusable, but bootstrap and lifetime ownership need refactoring
  before a Steam TU can share it without macro or symbol collisions.

Impact: adding Steam later remains plausible, but it is not just
"write `steam.cpp`, define the other macro, and include the same
header" yet.

Suggested fix: split backend acquisition from the shared transport
implementation. Standalone and Steam TUs should each own init,
shutdown, utils access, and accessor choice, then pass the interface
into the shared implementation. Add a compile-only Steam-shaped fake
or SDK-gated build job so the seam does not rot.

## OG-10: No authenticated identity, password, spectator role, or resume token

Status: future scope for trusted direct-IP play; a security and abuse
limit if a server is exposed publicly. Severity: context-dependent.
Owner: game protocol first, library helpers only if the pattern should
be shared.

Evidence:

- Hello carries only library protocol version, game label hash, and a
  character byte. It has no display name, password/token, requested
  role, spectator flag, or reconnect credential.
- The first valid Hellos occupy Left and Right. Any compatible client
  that can reach the UDP port can take a free player slot. Later
  compatible clients receive `Bye("match full")`.
- Disconnect cleanup frees the side. A later process can join as a
  replacement, and the same process can start a new join, but neither
  proves it is the original player. There is no match/session ID or
  resume token in Hello or Welcome.
- The transport can carry up to four remote peers, but the game has no
  spectator role. Extra peers are not given a read-only snapshot view.

Current accepted boundary: friends/LAN/direct-IP sessions where
reaching the port is the admission control. Do not describe the
version hash as authentication; the library source explicitly calls it
a compatibility guard, not a security mechanism.

Suggested fix if public servers matter: add an optional server
password or join token to Hello, return distinct Bye reasons for
version, password, and capacity failures, and add an explicit
player/spectator role. Resume needs a server-issued opaque token and a
grace window in which the side stays reserved.

## OG-11: Wire evolution is strict and mostly undocumented

Status: accepted v1 policy with a diagnostics gap. Severity: low while
all peers ship together, medium once independently versioned builds
meet. Owner: game protocol and library versioning policy.

Evidence:

- Every game decoder requires the exact expected type and an exhausted
  reader. Adding a field to Hello, Input, Snapshot, Ping, or Pong is
  therefore a breaking wire change, not a backward-compatible append.
- Ping/Pong correctly forced the game label from
  `pumpkin-beachball-v2` to `pumpkin-beachball-v3`, but the library's
  numeric `kProtocolVersion` stayed `1`. That separation is defensible
  (transport contract versus game schema), but it is only implied by
  the code today.
- `hello_matches` collapses both checks into one boolean. The server
  logs and returns the same `version mismatch` text when the library
  protocol differs, when only the game hash differs, and when the
  Hello is otherwise incompatible after decoding.
- Snapshot `phase` is a raw byte whose meaning depends on the game
  enum order noted in a comment. Inserting a phase in the middle of
  that enum would silently change the wire meaning unless the game
  label is bumped.
- Online Powers would need another breaking change: Hello or Welcome
  must negotiate the ruleset/physics identity, `InputPacket` needs the
  special edge, and snapshots need ability timers/armed state plus any
  effective-shape changes that the client cannot derive locally.

Suggested policy to write down before the next wire change:

1. Bump the game label for any message layout, enum-order, or semantic
   change. Never reuse a message type value.
2. Keep the library protocol version for transport-contract changes,
   not game schema changes, and say so in both READMEs.
3. Put the distinct mismatch reason in Bye and in the server log:
   unsupported library protocol, unsupported game build, malformed
   Hello, password failure, or match full.
4. Decide whether new trailing fields are ever allowed. If they are,
   decoders and tests must stop requiring exhaustion for those
   messages; if not, keep the current all-peers-rebuild rule explicit.

## OG-12: Test evidence is strong locally, still narrow geographically

Status: verification gap, not a known product defect. Severity:
medium before calling internet play broadly supported. Owner: test
plan across `afterhours-online` and Pumpkin Beachball.

What is covered now:

- Header-only codec/local tests and a 24-scenario transport contract
  matrix over LocalTransport and real GNS loopback, including fault
  injection above the transport.
- Fourteen always-registered game-process scenarios: dedicated and
  listen-host joins, reverse/simultaneous joins, third client,
  unreachable/bad address, occupied port, server exit, two rejoin
  shapes, raw-probe handshake attacks, and a full menu-driven match.
- Two opt-in windowed scenarios with real raylib clients and
  per-process screenshot files.
- The windowed screenshots proved a live rally renders on both
  clients. The earlier "online rally rendering not verified" note in
  the original rendering review is therefore stale.

What is not covered:

- A second physical machine, a real LAN with another host, public
  internet routing, NAT/firewall behavior, IPv6, or DNS (see OG-06).
- Windows or Linux builds and their GNS artifacts.
- UDP loss below GNS. `FaultTransport` drops application sends above
  the transport; it does not shape datagrams under GNS retransmission.
  The upstream testing doc already calls for a UDP proxy or OS traffic
  shaping at that layer.
- Physical gamepads. Online client intent includes gamepad 0, but no
  controller was attached during verification and E2E cannot inject
  gamepad input yet (afterhours gap O).
- Long-running soak, many sequential matches on one dedicated server,
  and resource growth over time. Existing scenarios cover restart and
  replacement joins, not hours of uptime.
- The final full `--all` game-process run after the last timing-floor
  change had not been repeated when this file was written. The gates
  run at the ping commit were unit tests, both game E2E backends,
  `make online-test`, `dedicated_happy` repeated twice, and
  `dedicated_windowed`; one earlier `--all` attempt was 13/14 with the
  `dedicated_happy` timing flake that prompted the timeout floors.
  Record a fresh `--all` result here when it is next run instead of
  upgrading this paragraph by implication.

Suggested fix: keep loopback as the fast gate, add one documented LAN
smoke checklist for releases, and treat any public-internet claim as
untested until NAT traversal or relay work exists.

## OG-13: Serialization strategy is a decision, not a default

Status: adopted 2026-11-05 at the user's direction, after the spike
below; the hand codec remains in the library for primitives and the
testing envelope. The library-owned detail is item 10 in
`vendor/afterhours-online/docs/gaps.md`.

`serialize.h` exists because the transport contract is opaque bytes and
v1 needed a tiny, dependency-free, explicit little-endian codec whose
readers fail closed on truncated input. It is not a schema system.
Pumpkin Beachball's seven fixed-layout messages fit it well today.

zpp::bits remains attractive for nested snapshot/component data (the
Pharmasea shape), but it is a roughly 5,700-line vendored header that
needs a `-Wshadow` suppression wrapper and a conditional
native/swapped alias (plus an option instance per archive) to pin
little-endian output. The spike proved Hello, Input, Snapshot, and
Bye can all stay byte-identical, so a migration would not have to
break the wire; it still is not worth it today for seven fixed-layout
messages. The game packets were then migrated anyway: zpp::bits is
vendored in the library behind `zpp_serialize.h` (little-endian
archives only), `slime_network.h` archives its packet structs through
it, and golden-byte unit tests freeze all seven layouts so the wire
and game hash did not change.

## OG-14: `online/core.h` does not read like an Afterhours plugin header

Status: design/style decision recorded 2026-11-05. The library-owned
detail is item 11 in `vendor/afterhours-online/docs/gaps.md`; this is
the consumer-facing consequence.

The mismatch is structural. Afterhours features are usually lowercase
plugin structs with nested components and systems; `online/core.h` is
a conventional abstract `Transport` with standard-library event types
and no ECS types at all. Reformatting it would not make it feel like
Afterhours. The recommended split is to keep `core.h` as the neutral
transport contract and make any future ECS adapter (OG-04) the
Afterhours-native surface. If that adapter is rejected, the plan and
README should stop calling the library an ECS plugin.

For reading the backend itself, start with
`vendor/afterhours-online/docs/gns-internals.md`; `gns_impl.h` now
opens with the same reading order (lifetime/registries, listen/connect,
poll/drain, status translation, close paths).

## OG-15: `gns_impl.h` needs a maintainer map, not just more comments

Status: partially mitigated 2026-11-05. Library-owned detail is item
12 in `vendor/afterhours-online/docs/gaps.md`.

The backend implementation interleaves process-wide GNS lifetime,
global callback registries, server and client state, native-handle
translation, message copying, reason mapping, stats, and lifecycle
handling in about 510 lines. The new upstream guide
(`vendor/afterhours-online/docs/gns-internals.md`) explains it in
runtime order, and the header now labels the same landmarks in place.

Remaining work is not a cosmetic split. When the Steam bootstrap work
in OG-09 happens, use that refactor to test whether acquisition,
callback routing, and endpoint logic separate cleanly; only then
decide whether the file needs more than its guide and landmarks.

## Future scope, not current defects

These were explicit v1 cuts or later ideas. They should stay visible
so "not built" is not confused with "broken":

- Room codes and a rendezvous service.
- NAT hole punching or relay; today internet play needs a reachable
  UDP port or a self-hosted dedicated server.
- Steamworks/SDR, Steam lobbies/friends/identity, and matchmaking.
- Rollback or lockstep. The current model is server-authoritative
  snapshots plus client-side own-player prediction.
- More than two game sides, persistent identities, and public-server
  moderation/rate-limit policy beyond transport/game slot checks.
- Pharmasea as a second consumer. Until another game adopts the
  library, its API has only been proven against Pumpkin Beachball's
  two-player, full-snapshot shape.
- Dedicated-server operations packaging: config file, signal-aware
  graceful shutdown handler, log rotation, and a documented
  systemd/process-manager unit. The binary logs to stdout and supports
  `--dedicated-seconds` mainly for tests.

## Checked during this audit: not gaps

- GNS channels are honored on the game paths: reliable handshake/Bye
  and phase-change snapshots, unreliable snapshots, and
  unreliable-no-delay inputs and pings.
- Malformed packets are dropped by exact decoders rather than crashing
  the server or client; the raw probe covers hostile Hello variants.
- The server, not the transport, assigns peer IDs and game sides.
- The current third-player behavior is explicit at the game layer:
  `Bye("match full")`, followed by disconnect.
- The core headers can be included without GNS. The game and test
  binaries prove the opt-in `gns.cpp` model in both raylib and NONE
  builds.
- Ping/Pong itself is built. The remaining ping work is OG-03/OG-07
  diagnostics depth, not basic RTT measurement.

## Maintenance rule for this file

When a gap closes, do not delete its history silently. Change its
status, name the commit or PR that closed it, and state the test that
proves the new behavior. When behavior changes without closing a gap,
update the evidence and date. If an entry turns out to be an accepted
design boundary, move it to "Future scope" or record the decision in
place.



