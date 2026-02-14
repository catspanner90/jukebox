#include "ui/menu_navigator_builder.h"

#include "sys/configs.h"
#include "ui/ui_configs.h"

#include <bn_array.h>
#include <bn_bitset.h>

#include <limits>

namespace jb::ui
{

namespace
{

constexpr bn::array<std::uint8_t, (int)sys::text_generators::font::MAX_COUNT> DEFAULT_MARGINS = {3, 3, 4, 4, 4};

constexpr bn::array<bn::color, 16> DEFAULT_POINTED_COLORS = {bn::colors::black, bn::colors::white};
constexpr bn::array<bn::color, 16> DEFAULT_UNPOINTED_COLORS = {bn::colors::black, bn::colors::gray};

constexpr bn::sprite_palette_item DEFAULT_POINTED_PALETTE(DEFAULT_POINTED_COLORS, bn::bpp_mode::BPP_4);
constexpr bn::sprite_palette_item DEFAULT_UNPOINTED_PALETTE(DEFAULT_UNPOINTED_COLORS, bn::bpp_mode::BPP_4);

} // namespace

menu_navigator_builder::menu_navigator_builder(sys::text_generators::font font,
                                               bn::span<const bn::string_view> menu_strings,
                                               bn::ivector<bn::sprite_ptr>& output_sprites)
    : _font(font), _menu_strings(menu_strings), _output_sprites(output_sprites),
      _pointed_palette(DEFAULT_POINTED_PALETTE), _unpointed_palette(DEFAULT_UNPOINTED_PALETTE),
      _pointed_changed_sfx(nullptr), _activated_sfx(nullptr), _activate_failed_sfx(nullptr), _cancelled_sfx(nullptr),
      _bg_priority(BG_PRIORITY), _line_margin(DEFAULT_MARGINS[(int)font]), _max_lines(menu_strings.size()),
      _scroll_start_delay(30), _scroll_continue_delay(6), _input_enabled(true), _pointed_index(0)
{
    BN_ASSERT(menu_strings.size() != 0, "At least one menu should exist");
}

auto menu_navigator_builder::build(sys::text_generators& text_gens) -> menu_navigator
{
    if (!_menu_strings_2.empty())
        BN_ASSERT(_menu_strings_2.size() == _menu_strings.size(), "strings_2 size mismatch: ", _menu_strings_2.size(),
                  " - ", _menu_strings.size());

    return menu_navigator(*this, text_gens);
}

auto menu_navigator_builder::font() const -> sys::text_generators::font
{
    return _font;
}

auto menu_navigator_builder::menu_strings() const -> bn::span<const bn::string_view>
{
    return _menu_strings;
}

auto menu_navigator_builder::output_sprites() const -> bn::ivector<bn::sprite_ptr>&
{
    return _output_sprites;
}

auto menu_navigator_builder::menu_strings_2() const -> bn::span<const bn::string_view>
{
    return _menu_strings_2;
}

auto menu_navigator_builder::set_menu_strings_2(const bn::span<const bn::string_view>& strs) -> menu_navigator_builder&
{
    _menu_strings_2 = strs;
    return *this;
}

auto menu_navigator_builder::top_left_position() const -> bn::fixed_point
{
    return _top_left_position;
}

auto menu_navigator_builder::set_top_left_position(const bn::fixed_point& top_left_position) -> menu_navigator_builder&
{
    _top_left_position = top_left_position;
    return *this;
}

auto menu_navigator_builder::set_top_left_position(bn::fixed top_left_x, bn::fixed top_left_y)
    -> menu_navigator_builder&
{
    _top_left_position = {top_left_x, top_left_y};
    return *this;
}

unsigned menu_navigator_builder::bg_priority() const
{
    return _bg_priority;
}

auto menu_navigator_builder::set_bg_priority(unsigned priority) -> menu_navigator_builder&
{
    static constexpr std::uint8_t MAX = 3;
    BN_ASSERT(priority <= MAX, "Too big priority: ", priority, " (max ", (unsigned)MAX, ")");

    _bg_priority = priority;
    return *this;
}

unsigned menu_navigator_builder::line_margin() const
{
    return _line_margin;
}

auto menu_navigator_builder::set_line_margin(unsigned margin) -> menu_navigator_builder&
{
    static constexpr auto MAX = std::numeric_limits<decltype(_line_margin)>::max();
    BN_ASSERT(margin <= MAX, "Too big margin: ", margin, " (max ", (unsigned)MAX, ")");

    _line_margin = margin;
    return *this;
}

unsigned menu_navigator_builder::max_lines() const
{
    return _max_lines;
}

auto menu_navigator_builder::set_max_lines(unsigned lines) -> menu_navigator_builder&
{
    static constexpr auto MAX = std::numeric_limits<decltype(_max_lines)>::max();
    BN_ASSERT(lines <= MAX, "Too many max lines: ", lines, " (max ", (unsigned)MAX, ")");
    BN_ASSERT(lines != 0, "Max lines can't be zero");

    _max_lines = lines;
    return *this;
}

unsigned menu_navigator_builder::scroll_start_delay() const
{
    return _scroll_start_delay;
}

auto menu_navigator_builder::set_scroll_start_delay(unsigned delay) -> menu_navigator_builder&
{
    static constexpr auto MAX = std::numeric_limits<decltype(_scroll_start_delay)>::max();
    BN_ASSERT(delay <= MAX, "Too much delay: ", delay, " (max ", (unsigned)MAX, ")");

    _scroll_start_delay = delay;
    return *this;
}

unsigned menu_navigator_builder::scroll_continue_delay() const
{
    return _scroll_continue_delay;
}

auto menu_navigator_builder::set_scroll_continue_delay(unsigned delay) -> menu_navigator_builder&
{
    static constexpr auto MAX = std::numeric_limits<decltype(_scroll_continue_delay)>::max();
    BN_ASSERT(delay <= MAX, "Too much delay: ", delay, " (max ", (unsigned)MAX, ")");

    _scroll_continue_delay = delay;
    return *this;
}

bool menu_navigator_builder::input_enabled() const
{
    return _input_enabled;
}

auto menu_navigator_builder::set_input_enabled(bool enabled) -> menu_navigator_builder&
{
    _input_enabled = enabled;
    return *this;
}

unsigned menu_navigator_builder::pointed_index() const
{
    return _pointed_index;
}

auto menu_navigator_builder::set_pointed_index(unsigned index) -> menu_navigator_builder&
{
    _pointed_index = index;
    return *this;
}

auto menu_navigator_builder::pointed_palette() const -> const bn::sprite_palette_item&
{
    return _pointed_palette;
}

auto menu_navigator_builder::set_pointed_palette(const bn::sprite_palette_item& palette) -> menu_navigator_builder&
{
    _pointed_palette = palette;
    return *this;
}

auto menu_navigator_builder::unpointed_palette() const -> const bn::sprite_palette_item&
{
    return _unpointed_palette;
}

auto menu_navigator_builder::set_unpointed_palette(const bn::sprite_palette_item& palette) -> menu_navigator_builder&
{
    _unpointed_palette = palette;
    return *this;
}

auto menu_navigator_builder::pointed_changed_callback() const -> pointed_changed_callback_t
{
    return _pointed_changed_callback;
}

auto menu_navigator_builder::set_pointed_changed_callback(pointed_changed_callback_t callback)
    -> menu_navigator_builder&
{
    _pointed_changed_callback = callback;
    return *this;
}

auto menu_navigator_builder::activated_callback() const -> activated_callback_t
{
    return _activated_callback;
}

auto menu_navigator_builder::set_activated_callback(activated_callback_t callback) -> menu_navigator_builder&
{
    _activated_callback = callback;
    return *this;
}

auto menu_navigator_builder::cancelled_callback() const -> cancelled_callback_t
{
    return _cancelled_callback;
}

auto menu_navigator_builder::set_cancelled_callback(cancelled_callback_t callback) -> menu_navigator_builder&
{
    _cancelled_callback = callback;
    return *this;
}

auto menu_navigator_builder::pointed_changed_sfx() const -> const bn::sound_item*
{
    return _pointed_changed_sfx;
}

auto menu_navigator_builder::set_pointed_changed_sfx(const bn::sound_item* sfx) -> menu_navigator_builder&
{
    _pointed_changed_sfx = sfx;
    return *this;
}

auto menu_navigator_builder::activated_sfx() const -> const bn::sound_item*
{
    return _activated_sfx;
}

auto menu_navigator_builder::set_activated_sfx(const bn::sound_item* sfx) -> menu_navigator_builder&
{
    _activated_sfx = sfx;
    return *this;
}

auto menu_navigator_builder::activate_failed_sfx() const -> const bn::sound_item*
{
    return _activate_failed_sfx;
}

auto menu_navigator_builder::set_activate_failed_sfx(const bn::sound_item* sfx) -> menu_navigator_builder&
{
    _activate_failed_sfx = sfx;
    return *this;
}

auto menu_navigator_builder::cancelled_sfx() const -> const bn::sound_item*
{
    return _cancelled_sfx;
}

auto menu_navigator_builder::set_cancelled_sfx(const bn::sound_item* sfx) -> menu_navigator_builder&
{
    _cancelled_sfx = sfx;
    return *this;
}

} // namespace jb::ui
