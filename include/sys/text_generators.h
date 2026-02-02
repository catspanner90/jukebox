#pragma once

// This supports newline character (`\n`)
#include "ibn_sprite_text_generator.h"

#include <bn_array.h>
#include <bn_color.h>

#include <cstdint>

namespace jb::sys
{

class text_generators final
{
public:
    enum class font : std::uint8_t
    {
        GALMURI_7,
        GALMURI_9,
        GALMURI_11,
        GALMURI_11_BOLD,
        GALMURI_11_CONDENSED,

        MAX_COUNT
    };

public:
    text_generators();

    text_generators(const text_generators&) = delete;
    text_generators& operator=(const text_generators&) = delete;

public:
    auto get(font) -> ibn::sprite_text_generator&;
    auto get(font) const -> const ibn::sprite_text_generator&;

public:
    auto text_color(font) const -> bn::color;
    void set_text_color(font, bn::color);

private:
    bn::array<ibn::sprite_text_generator, (int)font::MAX_COUNT> _generators;
    bn::array<bn::array<bn::color, 16>, (int)font::MAX_COUNT> _pal_colors;
};

} // namespace jb::sys
