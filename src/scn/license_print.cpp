#include "scn/license_print.h"

#include "gen/licenses.h"
#include "scn/scene_context.h"
#include "scn/scene_stack.h"

#include <bn_display.h>
#include <bn_keypad.h>

namespace jb::scn
{

namespace
{

constexpr auto FONT = sys::text_generators::font::GALMURI_7;

constexpr bn::fixed_point TEXT_TOP_LEFT_POS(10, 10);
constexpr bn::fixed LINE_SPACING = 10;
constexpr int MAX_LINES = 14;

} // namespace

license_print::license_print(int license_idx, scene_context& ctx)
    : scene(ctx), _typewriter(ctx.text_generators().get(FONT)), _license_idx(license_idx)
{
    auto& gens = ctx.text_generators();
    auto& gen = gens.get(FONT);

    const auto prev_color = gens.text_color(FONT);
    const auto prev_alignment = gen.alignment();
    gen.set_left_alignment();

    _typewriter.start(TEXT_TOP_LEFT_POS, gen::LICENSE_CONTENTS[_license_idx], _text_sprites, 1, nullptr,
                      (bn::display::width() - 2 * TEXT_TOP_LEFT_POS.x()).ceil_integer(), LINE_SPACING, MAX_LINES);

    gen.set_alignment(prev_alignment);
    gens.set_text_color(FONT, prev_color);
}

bool license_print::update()
{
    if (_typewriter.done())
    {
        if (bn::keypad::a_pressed() || bn::keypad::b_pressed())
        {
            auto& state_stack = context().stack();

            state_stack.reserve_pop();
            state_stack.reserve_push<licenses_list>(_license_idx, context());
        }
    }
    else
    {
        _typewriter.update();
    }

    return false;
}

} // namespace jb::scn
