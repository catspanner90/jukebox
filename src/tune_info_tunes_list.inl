#include "tune_info.h"

#include <bn_array.h>
#include <bn_bitmap_bg.h>
#include <bn_direct_bitmap_item.h>
#include <bn_dmg_music_item.h>
#include <bn_music_item.h>

#include <algorithm>

#include "bn_direct_bitmap_items_romantic_cat.h"

#include "bn_dmg_music_items_hell_owo_rld.h"
#include "bn_dmg_music_items_puku_7.h"
#include "bn_dmg_music_items_romantic_cat.h"
#include "bn_dmg_music_items_safer_with_you.h"
#include "bn_dmg_music_items_spooky_birthday.h"
#include "bn_music_items.h"

namespace jb
{

namespace
{

constexpr tune_info TUNES_LIST_RAW[] = {
    tune_info(&bn::dmg_music_items::hell_owo_rld, nullptr, tune_info::category::ORIGINAL, true, nullptr, "hellOWOrld",
              "copyrat90", {},
              R"(First loop I wrote in FamiTracker years ago, later converted into hUGETracker format.

Mostly inspired by Kitsune^2 - Naradno, Pachelbel - Canon in D and few other songs.)"),
    tune_info(&bn::dmg_music_items::puku_7, nullptr, tune_info::category::TRANSCRIBE, true, nullptr,
              "ぷくぷく天然かいらんばん - BGM #07", "さかもと ひでき", "copyrat90",
              R"(Ported a song from ぷくぷく天然かいらんばん just to practice using Furnace Tracker.

Original song also has PCM channels, but unfortunately, they're missing in this port.)"),
    tune_info(&bn::dmg_music_items::spooky_birthday, nullptr, tune_info::category::ORIGINAL, false, nullptr,
              "spooky birthday", "copyrat90", {},
              R"(Spooky birthday jingle for my GBA Microjam '23 entry:
Light the candles on the halloween cake!
https://github.com/gbadev-org/microjam23)"),
    tune_info(&bn::dmg_music_items::safer_with_you, nullptr, tune_info::category::TRANSCRIBE, true, nullptr,
              "Safer with You", "valfrey", "copyrat90", R"(I wonder what happened to this game and the composer...)"),
    tune_info(&bn::dmg_music_items::romantic_cat, &bn::music_items::romantic_cat, tune_info::category::COVER, false,
              &bn::direct_bitmap_items::romantic_cat, "낭만고양이(Romantic Cat) GBA chiptune cover", "Cherry Filter",
              "copyrat90", R"(Youtube: youtu.be/dZKVu2qoG98
GitHub: github.com/copyrat90/romantic-cat-gba
SoundCloud: https://soundcloud.com/copyrat90/romantic-cat-gba

This is my chiptune cover of 낭만고양이(Romantic Cat) for the Game Boy Advance.
It was recorded with GBA SP.

The original song was produced by Cherry Filter in 2002.
So, I thought it would be a good fit for the 2001 handheld to play it.

This was actually a test to synchronize advgm & Maxmod sound drivers.
You can download the GBA ROM on the GitHub link above.)"),
};

constexpr bn::span<const tune_info> TUNES_LIST(TUNES_LIST_RAW);

constexpr bn::array<bn::string_view, TUNES_LIST.size()> TUNES_NAMES_LIST = [] {
    bn::array<bn::string_view, TUNES_LIST.size()> result;
    for (int i = 0; i < TUNES_LIST.size(); ++i)
        result[i] = TUNES_LIST[i].tune_name();
    return result;
}();

static_assert(std::ranges::all_of(TUNES_LIST,
                                  [](const tune_info& info) {
                                      if (info.thumbnail() != nullptr)
                                      {
                                          const bn::size dimensions = info.thumbnail()->dimensions();
                                          if (dimensions.width() > bn::bitmap_bg::dp_direct_height())
                                              return false;
                                          if (dimensions.height() > bn::bitmap_bg::dp_direct_height())
                                              return false;
                                      }

                                      return true;
                                  }),
              "Thumbnail too big");

static_assert(
    [] {
        for (int l = 0; l < TUNES_LIST.size() - 1; ++l)
            for (int r = l + 1; r < TUNES_LIST.size(); ++r)
                if (&TUNES_LIST[l] == &TUNES_LIST[r])
                    return false;
        return true;
    }(),
    "Duplicated tune");

} // namespace

} // namespace jb
