#include "scn/jukebox.h"

#include "scn/scene_context.h"

#include <bn_colors.h>
#include <bn_display.h>
#include <bn_dmg_music.h>
#include <bn_dp_direct_bitmap_bg_builder.h>
#include <bn_fixed_point.h>
#include <bn_keypad.h>

#include <algorithm>

#include "bn_direct_bitmap_items_no_thumbnail.h"

namespace jb::scn
{

namespace
{

constexpr bn::fixed_point BG_POS(3, 29);
constexpr int BG_SIZE = bn::bitmap_bg::dp_direct_height();

constexpr bn::fixed_point TUNE_NAME_POS(1, 1);
constexpr bn::fixed_point COMPOSER_NAME_POS(15, 14);

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

    uncover();
}

jukebox::~jukebox()
{
    bn::dmg_music::stop();
}

bool jukebox::update()
{
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

void jukebox::redraw_thumbnail_bg()
{
    // Clear bg (excluding borders)
    _bg_painter->unsafe_rectangle(2, 2, BG_SIZE - 3, BG_SIZE - 3, bn::colors::black);

    // TODO: Draw thumbnail
    // TODO: Zoom thumbnail
    const bn::direct_bitmap_item& raw_bitmap_item = bn::direct_bitmap_items::no_thumbnail;
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

    // TODO
}

void jukebox::redraw_a_texts()
{
    _a_text_sprites.clear();

    auto& text_gen = context().text_generators().get(sys::text_generators::font::GALMURI_9);

    static constexpr bn::fixed_point TEXT_POS(LEFT_BTN_X, TOP_BTN_Y);
    // TODO: "Play" when selecting not currently playing tune
    const bn::string_view text = _state == state::TUNE_INFO  ? " Next"
                                 : !bn::dmg_music::playing() ? " Play"
                                 : bn::dmg_music::paused()   ? " Resume"
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
