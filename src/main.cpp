#include "scn/scene_context.h"
#include "scn/scene_stack.h"

#include "ibn_stats.h"

#include <bn_colors.h>
#include <bn_common.h>
#include <bn_core.h>

int main()
{
    bn::core::init(bn::colors::black);

    static BN_DATA_EWRAM jb::scn::scene_stack scene_stack;
    static BN_DATA_EWRAM jb::scn::scene_context scene_context(scene_stack);

    scene_context.config_save().load();

    scene_stack.reserve_push<jb::scn::jukebox>(scene_context);

    while (true)
    {
        scene_stack.update();
        scene_context.transitions().update();

        bn::core::update();
        IBN_STATS_UPDATE;
    }
}
