#pragma once

#define FMT_HEADER_ONLY
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch-enum"
#endif
#include <fmt/args.h>
#include <fmt/format.h>
#include <fmt/ostream.h>
#if FMT_VERSION >= 100000
// fmt >= 11 no longer formats std::filesystem::path by default; fmt/std.h
// provides that formatter (and is absent from the vendored fmt 8).
#include <fmt/std.h>
#endif
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
#define AFTER_HOURS_REPLACE_LOGGING
#define AFTER_HOURS_LOG_WITH_COLOR
#include "log/log.h"
