#include "scn/jukebox.h"

#include "scn/scene_context.h"
#include "tune_info.h"

#include <bn_colors.h>
#include <bn_display.h>
#include <bn_dmg_music.h>
#include <bn_dmg_music_item.h>
#include <bn_dp_direct_bitmap_bg_builder.h>
#include <bn_fixed_point.h>
#include <bn_keypad.h>
#include <bn_sstream.h>
#include <bn_string.h>

#include <algorithm>

#include "bn_direct_bitmap_items_no_thumbnail.h"

namespace jb::scn
{

namespace
{

constexpr bn::color LIGHT_GRAY(0x5294);

constexpr bn::fixed_point BG_POS(3, 29);
constexpr int BG_SIZE = bn::bitmap_bg::dp_direct_height();

constexpr bn::fixed LEFT_BTN_X = 137;
constexpr bn::fixed RIGHT_BTN_X = 197;
constexpr bn::fixed TOP_BTN_Y = 133;
constexpr bn::fixed BOTTOM_BTN_Y = 148;

auto create_bg_painter() -> bn::dp_direct_bitmap_bg_painter
{
    return bn::dp_direct_bitmap_bg_painter(
        bn::dp_direct_bitmap_bg_builder{}.set_top_left_position(BG_POS).release_build());
}

} // namespace

jukebox::jukebox(scene_context& ctx) : scene(ctx)
{
    bn::dmg_music::set_master_volume(bn::dmg_music_master_volume::FULL);

    // Fix out-of-bound tune index
    if (cursor_index() >= static_cast<unsigned>(tune_info::tunes_list().size()))
        set_cursor_index(0);

    play_at_cursor();

    uncover();
}

jukebox::~jukebox()
{
    bn::dmg_music::stop();
}

bool jukebox::update()
{
    if (_playing_index.has_value() && !bn::dmg_music::playing())
    {
        _playing_index.reset();
        redraw_tune_head_texts();
        redraw_a_texts();
    }

    switch (_state)
    {
    case state::TUNE_LIST:
        if (bn::keypad::b_pressed())
            stop();

        if (bn::keypad::a_pressed())
        {
            if (!_playing_index.has_value() || _playing_index.value() != cursor_index())
                play_at_cursor();
            else
                pause_or_resume();
        }
        break;

    case state::TUNE_INFO:
        break;

    default:
        BN_ERROR("Invalid state: ", (int)_state);
    }

    return false;
}

void jukebox::cover(bn::type_id_t)
{
    _bg_painter.reset();

    _tune_head_text_sprites.clear();
    _a_text_sprites.clear();
    _b_text_sprites.clear();
    _lr_text_sprites.clear();
    _start_text_sprites.clear();
    _list_text_sprites.clear();
}

void jukebox::uncover()
{
    if (!_bg_painter.has_value())
        _bg_painter = create_bg_painter();

    redraw_thumbnail_bg();

    redraw_tune_head_texts();
    redraw_a_texts();
    redraw_b_texts();
    redraw_lr_texts();
    redraw_start_texts();

    redraw_tune_list_texts();
}

void jukebox::play_at_cursor()
{
    const tune_info& info = tune_info::tunes_list()[cursor_index()];
    info.tune().play(1, info.loop());

    _playing_index = cursor_index();

    redraw_tune_head_texts();
    redraw_a_texts();
}

void jukebox::pause_or_resume()
{
    if (bn::dmg_music::paused())
        bn::dmg_music::resume();
    else
        bn::dmg_music::pause();

    redraw_a_texts();
}

void jukebox::stop()
{
    if (_playing_index.has_value())
    {
        bn::dmg_music::stop();
        _playing_index.reset();

        redraw_tune_head_texts();
        redraw_a_texts();
    }
}

unsigned jukebox::cursor_index()
{
    return context().config_save().tune_index();
}

void jukebox::set_cursor_index(unsigned index)
{
    auto& config_save = context().config_save();

    config_save.set_tune_index(index);
    config_save.save();
}

void jukebox::redraw_thumbnail_bg()
{
    // Clear bg (excluding borders)
    _bg_painter->unsafe_rectangle(2, 2, BG_SIZE - 3, BG_SIZE - 3, bn::colors::black);

    // Draw thumbnail
    // TODO: Zoom thumbnail
    const tune_info& info = tune_info::tunes_list()[cursor_index()];
    const bn::direct_bitmap_item& raw_bitmap_item =
        info.thumbnail() ? *info.thumbnail() : bn::direct_bitmap_items::no_thumbnail;
    const bn::direct_bitmap_item bitmap_item = raw_bitmap_item.decompress(context().decompressed_bitmap_bg_colors());

    const int roi_width = std::min(BG_SIZE, bitmap_item.dimensions().width());
    const int roi_height = std::min(BG_SIZE, bitmap_item.dimensions().height());
    const int roi_x = (bitmap_item.dimensions().width() - roi_width) / 2;
    const int roi_y = (bitmap_item.dimensions().height() - roi_height) / 2;
    const bn::direct_bitmap_roi roi(bitmap_item, roi_x, roi_y, roi_width, roi_height);

    const int x = (BG_SIZE - roi_width) / 2;
    const int y = (BG_SIZE - roi_height) / 2;
    _bg_painter->unsafe_blit(x, y, roi);

    // Draw inner borders
    _bg_painter->unsafe_horizontal_line(1, BG_SIZE - 2, 1, bn::colors::black);
    _bg_painter->unsafe_vertical_line(1, 1, BG_SIZE - 2, bn::colors::black);
    _bg_painter->unsafe_vertical_line(BG_SIZE - 2, 1, BG_SIZE - 2, bn::colors::black);
    _bg_painter->unsafe_horizontal_line(1, BG_SIZE - 2, BG_SIZE - 2, bn::colors::black);

    // Draw outer borders
    _bg_painter->unsafe_horizontal_line(0, BG_SIZE - 1, 0, bn::colors::gray);
    _bg_painter->unsafe_vertical_line(0, 0, BG_SIZE - 1, bn::colors::gray);
    _bg_painter->unsafe_vertical_line(BG_SIZE - 1, 0, BG_SIZE - 1, bn::colors::gray);
    _bg_painter->unsafe_horizontal_line(0, BG_SIZE - 1, BG_SIZE - 1, bn::colors::gray);

    // Apply changes
    _bg_painter->flip_page_later();
}

void jukebox::redraw_tune_head_texts()
{
    _tune_head_text_sprites.clear();

    if (!_playing_index.has_value())
        return;

    auto& ctx = context();
    auto& text_gens = ctx.text_generators();

    static constexpr auto BIG_FONT = sys::text_generators::font::GALMURI_9;
    static constexpr auto SMALL_FONT = sys::text_generators::font::GALMURI_7;
    auto& big_text_gen = text_gens.get(BIG_FONT);
    auto& small_text_gen = text_gens.get(SMALL_FONT);

    const auto prev_small_palette = small_text_gen.palette_item();
    text_gens.set_text_color(SMALL_FONT, LIGHT_GRAY);

    static constexpr bn::fixed_point TUNE_NAME_POS(1, 1);
    static constexpr bn::fixed_point COMPOSER_NAME_POS(15, 14);

    const tune_info& info = tune_info::tunes_list()[_playing_index.value()];

    [[maybe_unused]] bool generated;
    bn::fixed_point text_pos(TUNE_NAME_POS);

    // "♪ 01 "
    {
        bn::string<6> str;
        bn::ostringstream oss(str);
        oss << "♪ ";
        if (_playing_index.value() < 10)
            oss << '0';
        oss << _playing_index.value();

        generated = big_text_gen.generate_top_left_optional(text_pos, str, _tune_head_text_sprites);
        text_pos.set_x(text_pos.x() + big_text_gen.width(str) + big_text_gen.width(" "));
    }

    // "Playing tune name"
    generated = big_text_gen.generate_top_left_optional(text_pos, info.tune_name(), _tune_head_text_sprites);

    // "☻ "
    text_pos = COMPOSER_NAME_POS;
    generated = small_text_gen.generate_top_left_optional(text_pos, "☻ ", _tune_head_text_sprites);
    text_pos.set_x(text_pos.x() + small_text_gen.width("☻ "));

    // "Composer name"
    generated = small_text_gen.generate_top_left_optional(text_pos, info.composer_name(), _tune_head_text_sprites);
    text_pos.set_x(text_pos.x() + small_text_gen.width(info.composer_name()));

    if (!info.remixer_name().empty())
    {
        // " (Remixed by "
        generated = small_text_gen.generate_top_left_optional(text_pos, " (Remixed by ", _tune_head_text_sprites);
        text_pos.set_x(text_pos.x() + small_text_gen.width(" (Remixed by "));

        // "remixer name"
        generated = small_text_gen.generate_top_left_optional(text_pos, info.remixer_name(), _tune_head_text_sprites);
        text_pos.set_x(text_pos.x() + small_text_gen.width(info.remixer_name()));

        // ")"
        generated = small_text_gen.generate_top_left_optional(text_pos, ")", _tune_head_text_sprites);
        text_pos.set_x(text_pos.x() + small_text_gen.width(")"));
    }

    small_text_gen.set_palette_item(prev_small_palette);
}

void jukebox::redraw_a_texts()
{
    _a_text_sprites.clear();

    auto& text_gen = context().text_generators().get(sys::text_generators::font::GALMURI_9);

    static constexpr bn::fixed_point TEXT_POS(LEFT_BTN_X, TOP_BTN_Y);
    const bn::string_view text = _state == state::TUNE_INFO ? " Next"
                                 : (!_playing_index.has_value() || _playing_index.value() != cursor_index())
                                     ? " Play"
                                 : bn::dmg_music::paused() ? " Resume"
                                                           : " Pause";

    [[maybe_unused]] bool generated = text_gen.generate_top_left_optional(TEXT_POS, text, _a_text_sprites);
}

void jukebox::redraw_b_texts()
{
    _b_text_sprites.clear();

    auto& text_gen = context().text_generators().get(sys::text_generators::font::GALMURI_9);

    static constexpr bn::fixed_point TEXT_POS(RIGHT_BTN_X, TOP_BTN_Y);
    const bn::string_view text = _state == state::TUNE_INFO ? " Skip" : " Stop";

    [[maybe_unused]] bool generated = text_gen.generate_top_left_optional(TEXT_POS, text, _b_text_sprites);
}

void jukebox::redraw_lr_texts()
{
    _lr_text_sprites.clear();

    auto& text_gen = context().text_generators().get(sys::text_generators::font::GALMURI_7);

    static constexpr bn::fixed_point TEXT_POS(LEFT_BTN_X, BOTTOM_BTN_Y);
    static constexpr bn::string_view TEXT = " Zoom";

    [[maybe_unused]] bool generated = text_gen.generate_top_left_optional(TEXT_POS, TEXT, _lr_text_sprites);
}

void jukebox::redraw_start_texts()
{
    _start_text_sprites.clear();

    if (_state != state::TUNE_INFO)
    {
        auto& text_gen = context().text_generators().get(sys::text_generators::font::GALMURI_7);

        static constexpr bn::fixed_point TEXT_POS(RIGHT_BTN_X, BOTTOM_BTN_Y);
        static constexpr bn::string_view TEXT = " Info";

        [[maybe_unused]] bool generated = text_gen.generate_top_left_optional(TEXT_POS, TEXT, _start_text_sprites);
    }
}

void jukebox::redraw_tune_list_texts()
{
    // TODO
}

} // namespace jb::scn
