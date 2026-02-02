#pragma once

#include "scn/scenes.h"

#include <bn_generic_pool.h>
#include <bn_unique_ptr.h>

namespace jb::scn
{

class scene;

inline constexpr int MAX_SCENE_COUNT = 2;

using scene_pool_t = bn::generic_pool<MAX_SCENE_SIZE, MAX_SCENE_COUNT, MAX_SCENE_ALIGN>;

class scene_deleter final
{
public:
    scene_deleter() = default;
    scene_deleter(scene_pool_t&);

    void operator()(scene*) const;

private:
    scene_pool_t* _pool = nullptr;
};

using scene_ptr = bn::unique_ptr<scene, scene_deleter>;

} // namespace jb::scn
