#pragma once

#include "scn/scene_ptr.h"

#include "ibn_function.h"

#include <bn_generic_pool.h>
#include <bn_vector.h>

#include <concepts>
#include <utility>

namespace jb::scn
{

class scene_stack final
{
public:
    scene_stack() = default;

    scene_stack(const scene_stack&) = delete;
    scene_stack& operator=(const scene_stack&) = delete;

public:
    void update();

private:
    template <typename T>
    struct wrapped_value
    {
        T value;
    };

public:
    template <std::derived_from<scene> Scene, typename... Args>
    void reserve_push(Args&&... args)
    {
        _reserved.emplace_back([... args = wrapped_value<Args>{std::forward<Args>(args)}](scene_stack& self) mutable {
            return scene_ptr(&self._scene_pool.create<Scene>(std::forward<Args>(args.value)...),
                             scene_deleter(self._scene_pool));
        });
        _reserved_types.push_back(bn::type_id<Scene>());
    }

    void reserve_pop();
    void reserve_clear();

private:
    scene_pool_t _scene_pool;
    bn::vector<scene_ptr, MAX_SCENE_COUNT> _scenes;

    bn::vector<ibn::function<scene_ptr(scene_stack&)>, MAX_SCENE_COUNT> _reserved;
    bn::vector<bn::type_id_t, MAX_SCENE_COUNT> _reserved_types;
};

} // namespace jb::scn
