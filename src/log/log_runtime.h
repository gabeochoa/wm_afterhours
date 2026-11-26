#pragma once

#include "log_level.h"

// Local WM runtime log-level override shim (keep-as-local-plugin/shim).
// Unique semantics vs vendored afterhours logging (which has no runtime
// level override at all -- only the compile-time AFTER_HOURS_LOG_LEVEL):
//   * LOG_ALOG_ is the "use compile-time default" sentinel (default state);
//     effective_log_level() returns the runtime level only when it is at
//     least LOG_TRACE, otherwise AFTER_HOURS_LOG_LEVEL. So set(TRACE..NOTHING)
//     genuinely overrides, and reset() restores the compile-time default.
//   * Callers depend on it: src/game.cpp sets LOG_NOTHING in --quiet E2E
//     mode and LOG_WARN / reset on failure (see game.cpp) -- removal would
//     break quiet-mode output gating.
//   * Gating is split: AFTER_HOURS_LOG_AT (log_macros.h) early-outs on the
//     compile-time level, while vlog() re-checks effective_log_level() at
//     runtime. log_once_per_impl bypasses the runtime level entirely
//     (compile-time check only). Do not "simplify" these away without
//     auditing game.cpp quiet mode and the log_once_per call sites.
inline LogLevel g_runtime_log_level = LogLevel::LOG_ALOG_;

inline LogLevel effective_log_level() {
  if (static_cast<int>(g_runtime_log_level) >=
      static_cast<int>(LogLevel::LOG_TRACE)) {
    return g_runtime_log_level;
  }
  return AFTER_HOURS_LOG_LEVEL;
}

inline void set_runtime_log_level(LogLevel level) {
  g_runtime_log_level = level;
}

inline void reset_runtime_log_level() {
  g_runtime_log_level = LogLevel::LOG_ALOG_;
}
