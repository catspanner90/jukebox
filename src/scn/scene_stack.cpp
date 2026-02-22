#include "scn/scene_stack.h"

#include "ibn_stats.h"

#include <bn_core.h>

namespace jb::scn
{

void scene_stack::update()
{
    // Update scenes
    for (auto iter = _scenes.rbegin(); iter != _scenes.rend(); ++iter)
    {
        // Break if the upper scene don't want to update the scene below
        if (!(*iter)->update())
            break;
    }

    // Apply reserved changes
    for (auto& reserved : _reserved_changes)
    {
        switch (reserved.change_kind)
        {
        case reserved_change::kind::PUSH:
            // Previous top scene is `cover()`ed first
            if (!_scenes.empty())
                _scenes.back()->cover(reserved.new_scene_type);

            if (reserved.delay_frame)
            {
                bn::core::update();
                IBN_STATS_UPDATE;
            }

            _scenes.push_back(reserved.new_scene_factory(*this));
            break;

        case reserved_change::kind::POP:
            _scenes.pop_back();

            if (reserved.delay_frame)
            {
                bn::core::update();
                IBN_STATS_UPDATE;
            }

            // Next top scene is `uncover()`ed last
            if (!_scenes.empty())
                _scenes.back()->uncover();
            break;

        case reserved_change::kind::REPLACE_TOP:
            // Avoids `uncover()` & `cover()` overhead
            if (!_scenes.empty())
                _scenes.pop_back();

            if (reserved.delay_frame)
            {
                bn::core::update();
                IBN_STATS_UPDATE;
            }

            _scenes.push_back(reserved.new_scene_factory(*this));
            break;

        case reserved_change::kind::CLEAR:
            // Avoids `uncover()` overhead
            while (!_scenes.empty())
                _scenes.pop_back();
            break;

        default:
            BN_ERROR("Invalid reserved_change::kind : ", (int)reserved.change_kind);
        }
    }
    _reserved_changes.clear();
}

void scene_stack::reserve_pop()
{
    _reserved_changes.emplace_back(reserved_change::kind::POP, false, bn::type_id_t{},
                                   reserved_change::new_scene_factory_t{});
}

void scene_stack::reserve_pop_with_delay()
{
    _reserved_changes.emplace_back(reserved_change::kind::POP, true, bn::type_id_t{},
                                   reserved_change::new_scene_factory_t{});
}

void scene_stack::reserve_clear()
{
    _reserved_changes.emplace_back(reserved_change::kind::CLEAR, false, bn::type_id_t{},
                                   reserved_change::new_scene_factory_t{});
}

} // namespace jb::scn
