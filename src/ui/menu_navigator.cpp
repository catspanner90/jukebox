#include "ui/menu_navigator.h"

#include "directions.h"
#include "ui/menu_navigator_builder.h"

#include <bn_bitset.h>
#include <bn_keypad.h>
#include <bn_log.h>
#include <bn_log_level.h>
#include <bn_sound_item.h>
#include <bn_string_view.h>

#include <algorithm>

namespace jb::ui
{

namespace
{

constexpr bn::array<std::uint8_t, (int)sys::text_generators::font::MAX_COUNT> FONT_HEIGHTS = {7, 9, 11, 11, 11};

static_assert(std::ranges::all_of(FONT_HEIGHTS, [](std::uint8_t height) { return height != 0; }),
              "Invalid height found in FONT_HEIGHTS");

} // namespace

auto menu_navigator::create(const bn::fixed_point& top_left_position, sys::text_generators::font font,
                            sys::text_generators& text_gens, bn::span<const bn::string_view> menu_strings,
                            bn::ivector<bn::sprite_ptr>& output_sprites,
                            pointed_changed_callback_t pointed_changed_callback,
                            activated_callback_t activated_callback, cancelled_callback_t cancelled_callback)
    -> menu_navigator
{
    return menu_navigator_builder(font, menu_strings, output_sprites)
        .set_top_left_position(top_left_position)
        .set_pointed_changed_callback(pointed_changed_callback)
        .set_activated_callback(activated_callback)
        .set_cancelled_callback(cancelled_callback)
        .build(text_gens);
}

auto menu_navigator::create(bn::fixed top_left_x, bn::fixed top_left_y, sys::text_generators::font font,
                            sys::text_generators& text_gens, bn::span<const bn::string_view> menu_strings,
                            bn::ivector<bn::sprite_ptr>& output_sprites,
                            pointed_changed_callback_t pointed_changed_callback,
                            activated_callback_t activated_callback, cancelled_callback_t cancelled_callback)
    -> menu_navigator
{
    return menu_navigator_builder(font, menu_strings, output_sprites)
        .set_top_left_position(top_left_x, top_left_y)
        .set_pointed_changed_callback(pointed_changed_callback)
        .set_activated_callback(activated_callback)
        .set_cancelled_callback(cancelled_callback)
        .build(text_gens);
}

menu_navigator::~menu_navigator()
{
    clear_page();
}

void menu_navigator::update()
{
    if (_refresh_page_reserved)
    {
        _refresh_page_reserved = false;
        commit_refresh_page();
    }

    if (_input_enabled)
        handle_input();
}

bool menu_navigator::input_enabled() const
{
    return _input_enabled;
}

void menu_navigator::set_input_enabled(bool enabled)
{
    _input_enabled = enabled;
}

unsigned menu_navigator::pointed_index() const
{
    return _pointed_index;
}

void menu_navigator::set_pointed_index(unsigned index)
{
    BN_ASSERT(index < static_cast<unsigned>(_menu_strings.size()), "OOB index: ", index, " (max ",
              _menu_strings.size() - 1, ")");

    if (index != _pointed_index)
    {
        const auto prev_pointed_index = _pointed_index;
        const auto prev_page = get_page(prev_pointed_index);

        _pointed_index = index;
        const auto new_page = get_page(_pointed_index);

        if (new_page != prev_page)
            reserve_refresh_page();
        else
            refresh_palette();

        if (_pointed_changed_callback)
            _pointed_changed_callback(prev_page, prev_pointed_index, new_page, _pointed_index);
    }
}

unsigned menu_navigator::page() const
{
    return get_page(_pointed_index);
}

unsigned menu_navigator::total_pages() const
{
    return _total_pages;
}

menu_navigator::menu_navigator(const menu_navigator_builder& builder, sys::text_generators& text_gens)
    : _font(builder.font()), _text_gen(text_gens.get(_font)), _menu_strings(builder.menu_strings()),
      _menu_strings_2(builder.menu_strings_2()), _output_sprites(builder.output_sprites()),
      _init_output_sprites_size(_output_sprites.size()), _pointed_palette(builder.pointed_palette()),
      _unpointed_palette(builder.unpointed_palette()), _pointed_changed_callback(builder.pointed_changed_callback()),
      _activated_callback(builder.activated_callback()), _cancelled_callback(builder.cancelled_callback()),
      _pointed_changed_sfx(builder.pointed_changed_sfx()), _activated_sfx(builder.activated_sfx()),
      _activate_failed_sfx(builder.activate_failed_sfx()), _cancelled_sfx(builder.cancelled_sfx()),
      _top_left_position(builder.top_left_position()),
      _total_pages((_menu_strings.size() + builder.max_lines() - 1) / builder.max_lines()),
      _bg_priority(builder.bg_priority()), _line_margin(builder.line_margin()), _max_lines(builder.max_lines()),
      _scroll_start_delay(builder.scroll_start_delay()), _scroll_continue_delay(builder.scroll_continue_delay()),
      _refresh_page_reserved(false), _input_enabled(builder.input_enabled()), _scrolling(false),
      _scroll_delay(_scroll_start_delay), _prev_held_directions(directions::NONE),
      _pointed_index(builder.pointed_index())
{
    reserve_refresh_page();
}

void menu_navigator::handle_input()
{
    const auto prev_pointed_index = _pointed_index;
    const directions held_dirs = get_held_directions();
    if (_scrolling)
    {
        if (held_dirs == directions::NONE)
        {
            // Stop scrolling immediately.
            _scrolling = false;
        }
        // When held direction changes, or continue delay expires,
        else if (held_dirs != _prev_held_directions || --_scroll_delay == 0)
        {
            // Scroll to held directions.
            _pointed_index = get_changed_index(_pointed_index, held_dirs);

            // Repeat the continue delay.
            _scroll_delay = _scroll_continue_delay;
        }
    }
    else if (held_dirs != directions::NONE)
    {
        // When just started holding PAD,
        if (_prev_held_directions == directions::NONE)
        {
            // Scroll to pressed directions.
            _pointed_index = get_changed_index(_pointed_index, held_dirs);

            // Start the scroll delay.
            _scroll_delay = _scroll_start_delay;
        }
        // When held direction changes, or start delay expires,
        else if (held_dirs != _prev_held_directions || --_scroll_delay == 0)
        {
            // Scroll to held directions.
            _pointed_index = get_changed_index(_pointed_index, held_dirs);

            // Set the scrolling mode.
            _scrolling = true;
            _scroll_delay = _scroll_continue_delay;
        }
    }
    _prev_held_directions = held_dirs;

    if (_pointed_index != prev_pointed_index)
    {
        if (_pointed_changed_sfx)
            play_sfx(*_pointed_changed_sfx);

        const auto prev_page = get_page(prev_pointed_index);
        const auto new_page = get_page(_pointed_index);

        if (new_page != prev_page)
            reserve_refresh_page();
        else
            refresh_palette();

        if (_pointed_changed_callback)
            _pointed_changed_callback(prev_page, prev_pointed_index, new_page, _pointed_index);
    }

    if (bn::keypad::a_pressed())
    {
        if (_activated_sfx)
            play_sfx(*_activated_sfx);

        if (_activated_callback)
            _activated_callback(_pointed_index);
    }
    else if (bn::keypad::b_pressed())
    {
        if (_cancelled_sfx)
            play_sfx(*_cancelled_sfx);

        if (_cancelled_callback)
            _cancelled_callback();
    }
}

auto menu_navigator::get_held_directions() -> directions
{
    directions result = directions::NONE;

    if (bn::keypad::up_held())
        result |= directions::UP;
    if (bn::keypad::down_held())
        result |= directions::DOWN;
    if (bn::keypad::left_held())
        result |= directions::LEFT;
    if (bn::keypad::right_held())
        result |= directions::RIGHT;

    return result;
}

void menu_navigator::reserve_refresh_page()
{
    // Clear managed sprite texts.
    clear_page();

    // Reserve refreshing the page.
    _refresh_page_reserved = true;
}

void menu_navigator::commit_refresh_page()
{
    const unsigned page = this->page();

    _menu_spr_start_idxes.clear();

    // Render new sprite texts.
    bool failed = false;
    const auto prev_pal = _text_gen.palette_item();
    for (unsigned line = 0; line < get_max_lines_for_index(_pointed_index); ++line)
    {
        const unsigned idx = page * _max_lines + line;
        bn::fixed_point pos = get_line_pos(line);

        _text_gen.set_palette_item(idx == _pointed_index ? _pointed_palette : _unpointed_palette);
        _menu_spr_start_idxes.push_back(_output_sprites.size());

        auto render_texts = [&](const bn::span<const bn::string_view>& menu_strings) {
            if (!menu_strings.empty())
            {
                const bn::string_view& str = menu_strings[idx];
                failed |= !_text_gen.generate_top_left_optional(pos, str, _output_sprites);
                pos.set_x(pos.x() + _text_gen.width(str));
            }
        };

        render_texts(_menu_strings);
        render_texts(_menu_strings_2);
    }
    _text_gen.set_palette_item(prev_pal);

    _menu_spr_start_idxes.push_back(_output_sprites.size());

    // Set bg priority of new sprites.
    for (int idx = _init_output_sprites_size; idx < _output_sprites.size(); ++idx)
        _output_sprites[idx].set_bg_priority(_bg_priority);

    if (failed)
        BN_LOG_LEVEL(bn::log_level::WARN, "Failed generating text sprites for `menu_navigator`");
}

void menu_navigator::refresh_palette()
{
    if (_menu_spr_start_idxes.empty())
        return;

    for (unsigned line = 0; line < static_cast<unsigned>(_menu_spr_start_idxes.size()) - 1u; ++line)
    {
        const unsigned menu_idx = page() * _max_lines + line;

        for (auto spr_idx = _menu_spr_start_idxes[line]; spr_idx < _menu_spr_start_idxes[line + 1]; ++spr_idx)
        {
            auto& spr = _output_sprites[spr_idx];

            spr.set_palette(menu_idx == _pointed_index ? _pointed_palette : _unpointed_palette);
        }
    }
}

void menu_navigator::clear_page()
{
    while (_output_sprites.size() > _init_output_sprites_size)
        _output_sprites.pop_back();

    _menu_spr_start_idxes.clear();
}

unsigned menu_navigator::get_page(unsigned index) const
{
    return index / _max_lines;
}

unsigned menu_navigator::get_max_lines_for_page(unsigned page) const
{
    return (page != _total_pages - 1) ? _max_lines : get_line(static_cast<unsigned>(_menu_strings.size() - 1)) + 1;
}

unsigned menu_navigator::get_max_lines_for_index(unsigned index) const
{
    return get_max_lines_for_page(get_page(index));
}

unsigned menu_navigator::get_line(unsigned index) const
{
    return index % _max_lines;
}

auto menu_navigator::get_line_x() const -> bn::fixed
{
    return _top_left_position.x();
}

auto menu_navigator::get_line_y(unsigned line) const -> bn::fixed
{
    return _top_left_position.y() + line * (FONT_HEIGHTS[(int)_font] + _line_margin);
}

auto menu_navigator::get_line_pos(unsigned line) const -> bn::fixed_point
{
    return bn::fixed_point(get_line_x(), get_line_y(line));
}

auto menu_navigator::get_menu_x() const -> bn::fixed
{
    return get_line_x();
}

auto menu_navigator::get_menu_y(unsigned index) const -> bn::fixed
{
    return get_line_y(get_line(index));
}

auto menu_navigator::get_menu_pos(unsigned index) const -> bn::fixed_point
{
    return get_line_pos(get_line(index));
}

unsigned menu_navigator::get_changed_index(unsigned index, directions held_dirs) const
{
    unsigned page = get_page(index);
    unsigned line = get_line(index);

    if (!!(held_dirs & directions::LEFT))
        page = (page - 1 + _total_pages) % _total_pages;
    if (!!(held_dirs & directions::RIGHT))
        page = (page + 1) % _total_pages;

    const unsigned page_max_lines = get_max_lines_for_page(page);

    if (line >= page_max_lines)
        line = page_max_lines - 1;

    unsigned result = page * _max_lines + line;

    if (!!(held_dirs & directions::UP))
        result = (result - 1 + _menu_strings.size()) % _menu_strings.size();
    if (!!(held_dirs & directions::DOWN))
        result = (result + 1) % _menu_strings.size();

    return result;
}

void menu_navigator::play_sfx(const bn::sound_item& sfx)
{
    if (_sfx_handle.has_value())
        _sfx_handle->stop();

    _sfx_handle = sfx.play();
}

} // namespace jb::ui
