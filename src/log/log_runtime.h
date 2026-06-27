#pragma once

#include "log_level.h"

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
