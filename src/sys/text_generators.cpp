#include "sys/text_generators.h"

#include "sys/configs.h"

#include <bn_assert.h>
#include <bn_colors.h>

#include "galmuri11_bold_sprite_font.h"
#include "galmuri11_condensed_sprite_font.h"
#include "galmuri11_sprite_font.h"
#include "galmuri7_sprite_font.h"
#include "galmuri9_sprite_font.h"

namespace jb::sys
{

text_generators::text_generators()
    : _generators{
          ibn::sprite_text_generator(galmuri7_sprite_font),
          ibn::sprite_text_generator(galmuri9_sprite_font),
          ibn::sprite_text_generator(galmuri11_sprite_font),
          ibn::sprite_text_generator(galmuri11_bold_sprite_font),
          ibn::sprite_text_generator(galmuri11_condensed_sprite_font),
      }, _pal_colors{}
{
    for (int i = 0; i < _generators.size(); ++i)
    {
        _pal_colors[i][1] = TEXT_NORMAL_COLOR;
        _generators[i].set_palette_item(bn::sprite_palette_item(_pal_colors[i], bn::bpp_mode::BPP_4));
    }
}

auto text_generators::get(font kind) -> ibn::sprite_text_generator&
{
    BN_ASSERT(kind < font::MAX_COUNT, "Invalid font kind: ", (int)kind);

    return _generators[(int)kind];
}

auto text_generators::get(font kind) const -> const ibn::sprite_text_generator&
{
    BN_ASSERT(kind < font::MAX_COUNT, "Invalid font kind: ", (int)kind);

    return _generators[(int)kind];
}

auto text_generators::text_color(font kind) const -> bn::color
{
    BN_ASSERT(kind < font::MAX_COUNT, "Invalid font kind: ", (int)kind);

    return _pal_colors[(int)kind][1];
}

void text_generators::set_text_color(font kind, bn::color color)
{
    BN_ASSERT(kind < font::MAX_COUNT, "Invalid font kind: ", (int)kind);

    _pal_colors[(int)kind][1] = color;
}

} // namespace jb::sys
