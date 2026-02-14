#pragma once

#include "ui/menu_navigator.h"

#include <bn_span.h>
#include <bn_vector.h>

namespace jb::ui
{

/// @brief Creates a `menu_navigator` instance with custom attributes.
class menu_navigator_builder final
{
public:
    using pointed_changed_callback_t = menu_navigator::pointed_changed_callback_t;
    using activated_callback_t = menu_navigator::activated_callback_t;
    using cancelled_callback_t = menu_navigator::cancelled_callback_t;

public:
    menu_navigator_builder(sys::text_generators::font font, bn::span<const bn::string_view> menu_strings,
                           bn::ivector<bn::sprite_ptr>& output_sprites);

public:
    auto build(sys::text_generators&) -> menu_navigator;

public:
    auto font() const -> sys::text_generators::font;
    auto menu_strings() const -> bn::span<const bn::string_view>;
    auto output_sprites() const -> bn::ivector<bn::sprite_ptr>&;

    auto menu_strings_2() const -> bn::span<const bn::string_view>;
    auto set_menu_strings_2(const bn::span<const bn::string_view>& strs) -> menu_navigator_builder&;

    /// @brief Gets the top-left position of the top line menu text sprites.
    auto top_left_position() const -> bn::fixed_point;

    /// @brief Sets the top-left position of the top line menu text sprites.
    auto set_top_left_position(const bn::fixed_point& top_left_position) -> menu_navigator_builder&;
    /// @brief Sets the top-left position of the top line menu text sprites.
    auto set_top_left_position(bn::fixed top_left_x, bn::fixed top_left_y) -> menu_navigator_builder&;

    unsigned bg_priority() const;
    auto set_bg_priority(unsigned priority) -> menu_navigator_builder&;

    unsigned line_margin() const;
    auto set_line_margin(unsigned margin) -> menu_navigator_builder&;

    unsigned max_lines() const;
    auto set_max_lines(unsigned lines) -> menu_navigator_builder&;

    unsigned scroll_start_delay() const;
    auto set_scroll_start_delay(unsigned delay) -> menu_navigator_builder&;

    unsigned scroll_continue_delay() const;
    auto set_scroll_continue_delay(unsigned delay) -> menu_navigator_builder&;

    bool input_enabled() const;
    auto set_input_enabled(bool enabled) -> menu_navigator_builder&;

    unsigned pointed_index() const;
    auto set_pointed_index(unsigned) -> menu_navigator_builder&;

    auto pointed_palette() const -> const bn::sprite_palette_item&;
    auto set_pointed_palette(const bn::sprite_palette_item& palette) -> menu_navigator_builder&;

    auto unpointed_palette() const -> const bn::sprite_palette_item&;
    auto set_unpointed_palette(const bn::sprite_palette_item& palette) -> menu_navigator_builder&;

    auto pointed_changed_callback() const -> pointed_changed_callback_t;
    auto set_pointed_changed_callback(pointed_changed_callback_t callback) -> menu_navigator_builder&;

    auto activated_callback() const -> activated_callback_t;
    auto set_activated_callback(activated_callback_t callback) -> menu_navigator_builder&;

    auto cancelled_callback() const -> cancelled_callback_t;
    auto set_cancelled_callback(cancelled_callback_t callback) -> menu_navigator_builder&;

    auto pointed_changed_sfx() const -> const bn::sound_item*;
    auto set_pointed_changed_sfx(const bn::sound_item* sfx) -> menu_navigator_builder&;

    auto activated_sfx() const -> const bn::sound_item*;
    auto set_activated_sfx(const bn::sound_item* sfx) -> menu_navigator_builder&;

    auto activate_failed_sfx() const -> const bn::sound_item*;
    auto set_activate_failed_sfx(const bn::sound_item* sfx) -> menu_navigator_builder&;

    auto cancelled_sfx() const -> const bn::sound_item*;
    auto set_cancelled_sfx(const bn::sound_item* sfx) -> menu_navigator_builder&;

private:
    const sys::text_generators::font _font;
    const bn::span<const bn::string_view> _menu_strings;
    bn::ivector<bn::sprite_ptr>& _output_sprites;
    bn::span<const bn::string_view> _menu_strings_2;

    bn::sprite_palette_item _pointed_palette;
    bn::sprite_palette_item _unpointed_palette;

    pointed_changed_callback_t _pointed_changed_callback;
    activated_callback_t _activated_callback;
    cancelled_callback_t _cancelled_callback;
    const bn::sound_item* _pointed_changed_sfx;
    const bn::sound_item* _activated_sfx;
    const bn::sound_item* _activate_failed_sfx;
    const bn::sound_item* _cancelled_sfx;

    bn::fixed_point _top_left_position;
    std::uint8_t _bg_priority;
    std::uint8_t _line_margin;
    std::uint8_t _max_lines;
    std::uint8_t _scroll_start_delay;
    std::uint8_t _scroll_continue_delay;

    bool _input_enabled;

    unsigned _pointed_index;
};

} // namespace jb::ui
