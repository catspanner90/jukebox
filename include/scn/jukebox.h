#pragma once

#include "scn/scene.h"

#include "ui/menu_navigator.h"

#include <bn_dp_direct_bitmap_bg_painter.h>
#include <bn_optional.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>

namespace jb::scn
{

class jukebox final : public scene
{
public:
    jukebox(scene_context&);
    ~jukebox();

public:
    bool update() override;

    void cover([[maybe_unused]] bn::type_id_t new_top_scene_type = bn::type_id_t{}) override;
    void uncover() override;

private:
    enum class state
    {
        TUNE_LIST,
        TUNE_INFO,
    };

private:
    void play_at_cursor();
    void pause_or_resume();
    void stop();

private:
    unsigned cursor_index();
    void set_cursor_index(unsigned index);

private:
    void redraw_thumbnail_bg();

    void redraw_tune_head_texts();
    void redraw_a_texts();
    void redraw_b_texts();
    void redraw_start_texts();
    void redraw_select_texts();

    void redraw_tune_list_texts();

private:
    void on_tunes_navigator_pointed_changed(unsigned prev_page, unsigned prev_pointed_index, unsigned new_page, unsigned new_pointed_index);
    void on_tunes_navigator_activated(unsigned menu_index);
    void on_tunes_navigator_cancelled();

private:
    auto init_tunes_navigator() -> ui::menu_navigator;

private:
    state _state = state::TUNE_LIST;

    bn::optional<unsigned> _playing_index;

    bn::optional<bn::dp_direct_bitmap_bg_painter> _bg_painter;

    bn::vector<bn::sprite_ptr, 24> _tune_head_text_sprites;
    bn::vector<bn::sprite_ptr, 2> _a_text_sprites;
    bn::vector<bn::sprite_ptr, 2> _b_text_sprites;
    bn::vector<bn::sprite_ptr, 2> _start_text_sprites;
    bn::vector<bn::sprite_ptr, 2> _select_text_sprites;

    bn::vector<bn::sprite_ptr, 96> _list_text_sprites;

    ui::menu_navigator _tunes_navigator;
};

} // namespace jb::scn
