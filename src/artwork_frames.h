#pragma once

#include "external.h"
#include <array>

namespace artwork {
inline constexpr std::array<raylib::Rectangle, 5> cafe_frames{{
    {2.f, 2.f, 400.f, 400.f},
    {2.f, 406.f, 400.f, 400.f},
    {2.f, 810.f, 400.f, 400.f},
    {2.f, 1214.f, 400.f, 400.f},
    {2.f, 1618.f, 400.f, 400.f},
}};
inline constexpr std::array<raylib::Rectangle, 5> icons_frames{{
    {2.f, 2.f, 100.f, 100.f},
    {2.f, 106.f, 100.f, 100.f},
    {2.f, 210.f, 100.f, 100.f},
    {2.f, 314.f, 100.f, 100.f},
    {2.f, 418.f, 100.f, 100.f},
}};
}
