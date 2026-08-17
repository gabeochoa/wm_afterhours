#pragma once

// Each wrapped in do/while so they are a single statement. Without it
// `if (x) log_error(...);` put the assert outside the if and crashed
// unconditionally, and the others silently swallowed a trailing `else`.
#define AFTER_HOURS_LOG_AT(level, ...)                                         \
  do {                                                                         \
    if (static_cast<int>(level) >= static_cast<int>(AFTER_HOURS_LOG_LEVEL))    \
      log_me(level, __FILE__, __LINE__, __VA_ARGS__);                          \
  } while (0)

#define log_trace(...) AFTER_HOURS_LOG_AT(LogLevel::LOG_TRACE, __VA_ARGS__)
#define log_info(...) AFTER_HOURS_LOG_AT(LogLevel::LOG_INFO, __VA_ARGS__)
#define log_warn(...) AFTER_HOURS_LOG_AT(LogLevel::LOG_WARN, __VA_ARGS__)
#define log_error(...)                                                         \
  do {                                                                         \
    AFTER_HOURS_LOG_AT(LogLevel::LOG_ERROR, __VA_ARGS__);                      \
    assert(false);                                                             \
  } while (0)

#define log_clean(level, ...)                                                  \
  if (static_cast<int>(level) >= static_cast<int>(AFTER_HOURS_LOG_LEVEL))      \
    log_me(level, "", -1, __VA_ARGS__);

#define log_if(x, ...)                                                         \
  {                                                                            \
    if (x)                                                                     \
      log_me(LogLevel::LOG_IF, __FILE__, __LINE__, __VA_ARGS__);               \
  }

#define log_ifx(x, level, ...)                                                 \
  {                                                                            \
    if (x)                                                                     \
      log_me(level, __FILE__, __LINE__, __VA_ARGS__);                          \
  }

#define log_once_per(interval, level, ...)                                     \
  ::log_once_per_impl(interval, level, __FILE__, __LINE__, __VA_ARGS__)
