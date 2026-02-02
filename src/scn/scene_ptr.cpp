#include "scn/scene_ptr.h"

namespace jb::scn
{

scene_deleter::scene_deleter(scene_pool_t& pool) : _pool(&pool)
{
}

void scene_deleter::operator()(scene* scene_) const
{
    _pool->destroy(*scene_);
}

} // namespace jb::scn
