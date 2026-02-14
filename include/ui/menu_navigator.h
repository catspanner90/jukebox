#pragma once

#include "sys/text_generators.h"

#include "ibn_function.h"

#include <bn_fixed_point.h>
#include <bn_optional.h>
#include <bn_sound_handle.h>
#include <bn_sprite_actions.h>
#include <bn_sprite_item.h>
#include <bn_string_view.h>
#include <bn_vector.h>

#include <cstdint>

namespace bn
{
class ibitset;
class sound_item;
} // namespace bn

namespace jb
{
enum class directions : std::uint8_t;
}

namespace jb::ui
{

class menu_navigator_builder;

/// @brief Helper to navigate the menu.
///
/// * Renders the menu option texts.
/// * Navigate the menu options with Up/Down key.
///   * If the number of menu options exceeds the max lines,
///     it also allows navigating between pages with Left/Right key.
/// * Activate the pointed menu option with A key.
/// * Cancel the navigating with B key.
class menu_navigator final
{
public:
    static constexpr int MAX_MENUS_COUNT = 9;

public:
    /// @brief Callback that fires when pointed index has been changed.
    /// @param prev_page Previous page.
    /// @param prev_pointed_index Previously pointed menu index.
    /// @param new_page New page.
    /// @param new_pointed_index Newly pointed menu index.
    using pointed_changed_callback_t = ibn::function<void(unsigned, unsigned, unsigned, unsigned)>;

    /// @brief Callback that fires when pointed menu is activated.
    /// @param menu_index Activated menu index.
    using activated_callback_t = ibn::function<void(unsigned)>;

    /// @brief Callback that fires when cancelled.
    using cancelled_callback_t = ibn::function<void()>;

public:
    static auto create(const bn::fixed_point& top_left_position, sys::text_generators::font font, sys::text_generators&,
                       bn::span<const bn::string_view> menu_strings, bn::ivector<bn::sprite_ptr>& output_sprites,
                       pointed_changed_callback_t pointed_changed_callback, activated_callback_t activated_callback,
                       cancelled_callback_t cancelled_callback) -> menu_navigator;

    static auto create(bn::fixed top_left_x, bn::fixed top_left_y, sys::text_generators::font font,
                       sys::text_generators&, bn::span<const bn::string_view> menu_strings,
                       bn::ivector<bn::sprite_ptr>& output_sprites, pointed_changed_callback_t pointed_changed_callback,
                       activated_callback_t activated_callback, cancelled_callback_t cancelled_callback)
        -> menu_navigator;

public:
    ~menu_navigator();

    menu_navigator(const menu_navigator&) = delete;
    menu_navigator& operator=(const menu_navigator&) = delete;

public:
    void update();

    void reserve_refresh_page();
    void clear_page();

public:
    bool input_enabled() const;
    void set_input_enabled(bool enabled);

    unsigned pointed_index() const;
    void set_pointed_index(unsigned);

    unsigned page() const;
    unsigned total_pages() const;

private:
    friend class menu_navigator_builder;

    explicit menu_navigator(const menu_navigator_builder&, sys::text_generators&);

private:
    void handle_input();

    auto get_held_directions() -> directions;

private:
    void commit_refresh_page();

    void refresh_palette();

private:
    unsigned get_page(unsigned index) const;
    unsigned get_max_lines_for_page(unsigned page) const;
    unsigned get_max_lines_for_index(unsigned index) const;

    unsigned get_line(unsigned index) const;

    auto get_line_x() const -> bn::fixed;
    auto get_line_y(unsigned line) const -> bn::fixed;
    auto get_line_pos(unsigned line) const -> bn::fixed_point;

    auto get_menu_x() const -> bn::fixed;
    auto get_menu_y(unsigned index) const -> bn::fixed;
    auto get_menu_pos(unsigned index) const -> bn::fixed_point;

private:
    unsigned get_changed_index(unsigned index, directions held_dirs) const;

private:
    void play_sfx(const bn::sound_item& sfx);

private:
    const sys::text_generators::font _font;
    ibn::sprite_text_generator& _text_gen;
    const bn::span<const bn::string_view> _menu_strings;
    const bn::span<const bn::string_view> _menu_strings_2;
    bn::ivector<bn::sprite_ptr>& _output_sprites;

    const int _init_output_sprites_size;

    const bn::sprite_palette_item _pointed_palette;
    const bn::sprite_palette_item _unpointed_palette;

    pointed_changed_callback_t _pointed_changed_callback;
    activated_callback_t _activated_callback;
    cancelled_callback_t _cancelled_callback;
    const bn::sound_item* const _pointed_changed_sfx;
    const bn::sound_item* const _activated_sfx;
    const bn::sound_item* const _activate_failed_sfx;
    const bn::sound_item* const _cancelled_sfx;

    const bn::fixed_point _top_left_position;
    const unsigned _total_pages;
    const std::uint8_t _bg_priority;
    const std::uint8_t _line_margin;
    const std::uint8_t _max_lines;
    const std::uint8_t _scroll_start_delay;
    const std::uint8_t _scroll_continue_delay;

    bool _refresh_page_reserved;

    bool _input_enabled;

    bool _scrolling;
    std::uint8_t _scroll_delay;
    directions _prev_held_directions;

    unsigned _pointed_index;

    bn::optional<bn::sound_handle> _sfx_handle;

    bn::vector<std::uint8_t, MAX_MENUS_COUNT + 1> _menu_spr_start_idxes;
};

} // namespace jb::ui
