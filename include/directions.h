#pragma once

#include "ibn_enum_as_flags.h"

#include <cstdint>

namespace jb
{

enum class direction : std::uint8_t
{
    UP,
    UP_RIGHT,
    RIGHT,
    DOWN_RIGHT,
    DOWN,
    DOWN_LEFT,
    LEFT,
    UP_LEFT,
};

enum class directions : std::uint8_t
{
    NONE = 0,

    UP = (1u << (unsigned)direction::UP),
    UP_RIGHT = (1u << (unsigned)direction::UP_RIGHT),
    RIGHT = (1u << (unsigned)direction::RIGHT),
    DOWN_RIGHT = (1u << (unsigned)direction::DOWN_RIGHT),
    DOWN = (1u << (unsigned)direction::DOWN),
    DOWN_LEFT = (1u << (unsigned)direction::DOWN_LEFT),
    LEFT = (1u << (unsigned)direction::LEFT),
    UP_LEFT = (1u << (unsigned)direction::UP_LEFT),

    ALL = UP | UP_RIGHT | RIGHT | DOWN_RIGHT | DOWN | DOWN_LEFT | LEFT | UP_LEFT
};

IBN_ENUM_AS_FLAGS(directions);

} // namespace jb
