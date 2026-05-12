#include "tune_info.h"

#include <bn_dmg_music.h>
#include <bn_music.h>
#include <bn_synced_music.h>

#include "tune_info_tunes_list.inl"

namespace jb
{

auto tune_info::tunes_list() -> bn::span<const tune_info>
{
    return TUNES_LIST;
}

auto tune_info::tunes_names_list() -> bn::span<const bn::string_view>
{
    return TUNES_NAMES_LIST;
}

bool tune_info::playing()
{
    return bn::synced_music::playing() || bn::dmg_music::playing() || bn::music::playing();
}

bool tune_info::paused()
{
    return bn::synced_music::paused() || bn::dmg_music::paused() || bn::music::paused();
}

void tune_info::play() const
{
    tune_info::stop();

    if (_dmg_mus && _mus)
        bn::synced_music::play(*_mus, *_dmg_mus, 1, _loop);
    else if (_dmg_mus)
        bn::dmg_music::play(*_dmg_mus, 1, _loop);
    else if (_mus)
        bn::music::play(*_mus, 1, _loop);
}

void tune_info::stop()
{
    if (bn::synced_music::playing())
        bn::synced_music::stop();
    else if (bn::dmg_music::playing())
        bn::dmg_music::stop();
    else if (bn::music::playing())
        bn::music::stop();
}

void tune_info::pause()
{
    BN_BASIC_ASSERT(tune_info::playing(), "There's no tune playing");
    BN_BASIC_ASSERT(!tune_info::paused(), "Tune is already paused");

    if (bn::synced_music::playing())
        bn::synced_music::pause();
    else if (bn::dmg_music::playing())
        bn::dmg_music::pause();
    else if (bn::music::playing())
        bn::music::pause();
}

void tune_info::resume()
{
    BN_BASIC_ASSERT(tune_info::paused(), "Tune is not paused");

    if (bn::synced_music::paused())
        bn::synced_music::resume();
    else if (bn::dmg_music::paused())
        bn::dmg_music::resume();
    else if (bn::music::paused())
        bn::music::resume();
}

} // namespace jb
