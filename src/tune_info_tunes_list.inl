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

#include "bn_dmg_music_items_arachno_a_sad_touch.h"
#include "bn_dmg_music_items_chavez_monkeys_on_mars.h"
#include "bn_dmg_music_items_coffee_bat_blue_ocean.h"
#include "bn_dmg_music_items_coffee_bat_pilgrims_peril_menu.h"
#include "bn_dmg_music_items_coffee_bat_wyrmhole.h"
#include "bn_dmg_music_items_cognition.h"
#include "bn_dmg_music_items_dancing_to_the_death.h"
#include "bn_dmg_music_items_dtect.h"
#include "bn_dmg_music_items_fade_microplastics_in_the_air.h"
#include "bn_dmg_music_items_fade_strap_in_and_suit_up.h"
#include "bn_dmg_music_items_final_soldier_stage_1.h"
#include "bn_dmg_music_items_finger.h"
#include "bn_dmg_music_items_freedom.h"
#include "bn_dmg_music_items_galactic_quest_mus_theme_c.h"
#include "bn_dmg_music_items_gradius_mechanical_globule.h"
#include "bn_dmg_music_items_ice_3_advert.h"
#include "bn_dmg_music_items_jester_fishnchips.h"
#include "bn_dmg_music_items_lagtrain.h"
#include "bn_dmg_music_items_maktone_softworld.h"
#include "bn_dmg_music_items_melonadem_boatship_for_rent.h"
#include "bn_dmg_music_items_minos.h"
#include "bn_dmg_music_items_on_hold.h"
#include "bn_dmg_music_items_pleasure_of_tension.h"
#include "bn_dmg_music_items_pokemon_center.h"
#include "bn_dmg_music_items_puggs_in_space.h"
#include "bn_dmg_music_items_quazar_funky_stars_excerpt.h"
#include "bn_dmg_music_items_raphaelgoulart_the_murderous_funk_machine_intro.h"
#include "bn_dmg_music_items_razzberry_cupcake.h"
#include "bn_dmg_music_items_reed_funkabeers_revenge.h"
#include "bn_dmg_music_items_snowdin.h"
#include "bn_dmg_music_items_soft_maniac_ryukenden.h"
#include "bn_dmg_music_items_spreadtro.h"
#include "bn_dmg_music_items_svl_uwu.h"
#include "bn_dmg_music_items_svl_yyna.h"
#include "bn_dmg_music_items_waitforme.h"
#include "bn_dmg_music_items_wild_pokemon_appear.h"
#include "bn_dmg_music_items_youre_doing_well.h"

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
    tune_info(&bn::dmg_music_items::galactic_quest_mus_theme_c, &bn::music_items::galactic_quest_mus_theme_c,
              tune_info::category::ORIGINAL, true, nullptr, "Galactic Quest - Theme C", "potatoTeto", {}, {}),
    tune_info(&bn::dmg_music_items::razzberry_cupcake, nullptr, tune_info::category::ORIGINAL, true, nullptr,
              "Razzberry Cupcake", "AceMan / Agenda", {}, {}),
    tune_info(&bn::dmg_music_items::arachno_a_sad_touch, nullptr, tune_info::category::ORIGINAL, true, nullptr,
              "A Sad Touch", "Arachno", {}, {}),
    tune_info(&bn::dmg_music_items::chavez_monkeys_on_mars, nullptr, tune_info::category::ORIGINAL, true, nullptr,
              "monkeys on mars", "chavez", {}, {}),
    tune_info(&bn::dmg_music_items::coffee_bat_blue_ocean, nullptr, tune_info::category::ORIGINAL, true, nullptr,
              "G-ZERO - Blue Ocean theme", R"(Coffee "Valen" Bat)", {}, {}),
    tune_info(&bn::dmg_music_items::coffee_bat_pilgrims_peril_menu, nullptr, tune_info::category::ORIGINAL, true,
              nullptr, "Pilgrim's Peril - Menu theme", R"(Coffee "Valen" Bat)", {}, {}),
    tune_info(&bn::dmg_music_items::coffee_bat_wyrmhole, nullptr, tune_info::category::ORIGINAL, true, nullptr,
              "Wyrmhole - Ingame theme", R"(Coffee "Valen" Bat)", {}, {}),
    tune_info(&bn::dmg_music_items::cognition, nullptr, tune_info::category::ORIGINAL, true, nullptr, "Cognition",
              "Unknown Artist", {}, {}),
    tune_info(&bn::dmg_music_items::dancing_to_the_death, nullptr, tune_info::category::ORIGINAL, true, nullptr,
              "Dancing To The Death", "Weeppiko", {}, {}),
    tune_info(&bn::dmg_music_items::dtect, nullptr, tune_info::category::COVER, true, nullptr, "dtect", "Unknown",
              "tildearrow", {}),
    tune_info(&bn::dmg_music_items::fade_microplastics_in_the_air, nullptr, tune_info::category::ORIGINAL, true,
              nullptr, "Microplastics in the Air", R"(F/\DE)", {}, {}),
    tune_info(&bn::dmg_music_items::fade_strap_in_and_suit_up, nullptr, tune_info::category::ORIGINAL, true, nullptr,
              "Strap In and Suit Up", R"(F/\DE)", {}, {}),
    tune_info(&bn::dmg_music_items::final_soldier_stage_1, nullptr, tune_info::category::COVER, true, nullptr,
              "Final Soldier - Stage 1", "T S Kiyoshi", "Michirin", {}),
    tune_info(&bn::dmg_music_items::finger, nullptr, tune_info::category::ORIGINAL, true, nullptr,
              "My Fabulous Finger Oil of Doom", "dumbut", {}, {}),
    tune_info(&bn::dmg_music_items::freedom, nullptr, tune_info::category::COVER, true, nullptr, "freedom.grim", "grim",
              "tildearrow", {}),
    tune_info(&bn::dmg_music_items::gradius_mechanical_globule, nullptr, tune_info::category::COVER, true, nullptr,
              "Gradius - Mechanical Globule", "Miki Higashino", "MelonadeM", {}),
    tune_info(&bn::dmg_music_items::ice_3_advert, nullptr, tune_info::category::COVER, true, nullptr, "ice 3 advert",
              "EC-Rider", "tildearrow", {}),
    tune_info(&bn::dmg_music_items::jester_fishnchips, nullptr, tune_info::category::ORIGINAL, true, nullptr,
              "Fish 'n Chips", "Jester", {}, {}),
    tune_info(&bn::dmg_music_items::lagtrain, nullptr, tune_info::category::COVER, true, nullptr, "Lagtrain",
              "inabakumori", R"(David "PichuMario" Suarez)", {}),
    tune_info(&bn::dmg_music_items::maktone_softworld, nullptr, tune_info::category::ORIGINAL, true, nullptr,
              "Softworld", "Maktone", {}, {}),
    tune_info(&bn::dmg_music_items::melonadem_boatship_for_rent, nullptr, tune_info::category::ORIGINAL, true, nullptr,
              "Boatship for Rent", "MelonadeM", {}, {}),
    tune_info(&bn::dmg_music_items::minos, nullptr, tune_info::category::COVER, true, nullptr, "minos", "Arne Urbaniak",
              "tildearrow", {}),
    tune_info(&bn::dmg_music_items::on_hold, nullptr, tune_info::category::ORIGINAL, true, nullptr, "On Hold",
              "TheDuccinator", {}, {}),
    tune_info(&bn::dmg_music_items::pleasure_of_tension, nullptr, tune_info::category::ORIGINAL, true, nullptr,
              "Pleasure of Tension", "ifrit05, Masahiro Ikariko", {}, {}),
    tune_info(&bn::dmg_music_items::pokemon_center, nullptr, tune_info::category::COVER, true, nullptr,
              "Pokemon Center", "Junichi Masuda", "Yomaru Kasuga", {}),
    tune_info(&bn::dmg_music_items::puggs_in_space, nullptr, tune_info::category::COVER, true, nullptr,
              "Puggs in Space", "CoLD SToRAGE", "DevEd", {}),
    tune_info(&bn::dmg_music_items::quazar_funky_stars_excerpt, nullptr, tune_info::category::ORIGINAL, true, nullptr,
              "Funky Stars (excerpt)", "Quazar of Sanxion", {}, {}),
    tune_info(&bn::dmg_music_items::raphaelgoulart_the_murderous_funk_machine_intro, nullptr,
              tune_info::category::ORIGINAL, true, nullptr, "The Murderous Funk Machine (intro)", "Raphael Goulart", {},
              {}),
    tune_info(&bn::dmg_music_items::reed_funkabeers_revenge, nullptr, tune_info::category::ORIGINAL, true, nullptr,
              "Funkabeer's Revenge", "Reed Richards", {}, {}),
    tune_info(&bn::dmg_music_items::snowdin, nullptr, tune_info::category::COVER, true, nullptr,
              "Snowdin Town (G/S/C Style Arrange)", "Toby Fox", "Jimmy-DS", {}),
    tune_info(&bn::dmg_music_items::soft_maniac_ryukenden, nullptr, tune_info::category::ORIGINAL, true, nullptr,
              "Ryukenden", "Soft Maniac", {}, {}),
    tune_info(&bn::dmg_music_items::spreadtro, nullptr, tune_info::category::COVER, true, nullptr, "euphoria-spreadtro",
              "Mac/Euphoria", "tildearrow", {}),
    tune_info(&bn::dmg_music_items::svl_uwu, nullptr, tune_info::category::ORIGINAL, true, nullptr, "UwU", "SVL", {},
              {}),
    tune_info(&bn::dmg_music_items::svl_yyna, nullptr, tune_info::category::ORIGINAL, true, nullptr, R"("Yyna")", "SVL",
              {}, {}),
    tune_info(&bn::dmg_music_items::waitforme, nullptr, tune_info::category::ORIGINAL, true, nullptr,
              "Wait for me! (I can't keep up!)", "Wegfrei", {}, {}),
    tune_info(&bn::dmg_music_items::wild_pokemon_appear, nullptr, tune_info::category::ORIGINAL, true, nullptr,
              "Wild Pokemon Appear", "Junichi Masuda", {}, {}),
    tune_info(&bn::dmg_music_items::youre_doing_well, nullptr, tune_info::category::ORIGINAL, true, nullptr,
              "You're Doing Well!", "TheDuccinator", {}, {}),
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
