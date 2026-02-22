#pragma once

#include "scn/scene_ptr.h"

#include "ibn_function.h"

#include <bn_generic_pool.h>
#include <bn_vector.h>

#include <concepts>
#include <cstdint>
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
    /// @brief Updates the scene stack.
    /// This should be called each frame.
    void update();

private:
    template <typename T>
    struct wrapped_value final
    {
        T value;
    };

    struct reserved_change final
    {
        enum class kind : std::uint8_t
        {
            PUSH,
            POP,
            REPLACE_TOP,
            CLEAR,
        };

        using new_scene_factory_t = ibn::function<scene_ptr(scene_stack&)>;

        kind change_kind;
        bool delay_frame;
        bn::type_id_t new_scene_type;
        new_scene_factory_t new_scene_factory;
    };

    template <std::derived_from<scene> Scene, typename... Args>
    void reserve_add_impl(reserved_change::kind change_kind, bool delay_frame, Args&&... args)
    {
        _reserved_changes.emplace_back(
            change_kind, delay_frame, bn::type_id<Scene>(),
            [... args = wrapped_value<Args>{std::forward<Args>(args)}](scene_stack& self) mutable {
                return scene_ptr(&self._scene_pool.create<Scene>(std::forward<Args>(args.value)...),
                                 scene_deleter(self._scene_pool));
            });
    }

public:
    /// @brief Reserves push of a new scene, with given constructor arguments.
    ///
    /// Calls `scene::cover()` first for the previous top scene, and constructs a new scene afterwards.
    /// @note If you want to delay a frame between `scene::cover()` and scene construction,
    /// use `reserve_push_with_delay()` instead.
    template <std::derived_from<scene> Scene, typename... Args>
    void reserve_push(Args&&... args)
    {
        reserve_add_impl<Scene>(reserved_change::kind::PUSH, false, std::forward<Args>(args)...);
    }

    /// @brief Reserves push of a new scene, with given constructor arguments.
    ///
    /// Calls `scene::cover()` first for the previous top scene, delays a frame, and constructs a new scene afterwards.
    /// @note If you don't want to delay a frame between `scene::cover()` and scene construction,
    /// use `reserve_push()` instead.
    template <std::derived_from<scene> Scene, typename... Args>
    void reserve_push_with_delay(Args&&... args)
    {
        reserve_add_impl<Scene>(reserved_change::kind::PUSH, true, std::forward<Args>(args)...);
    }

    /// @brief Reserves replace of the top scene with a new scene, with given constructor arguments.
    ///
    /// Destroys previous top scene first, and constructs a new scene afterwards.
    /// @note If you want to delay a frame between scene destruction and construction,
    /// use `reserve_replace_top_with_delay()` instead.
    template <std::derived_from<scene> Scene, typename... Args>
    void reserve_replace_top(Args&&... args)
    {
        reserve_add_impl<Scene>(reserved_change::kind::REPLACE_TOP, false, std::forward<Args>(args)...);
    }

    /// @brief Reserves replace of the top scene with a new scene, with given constructor arguments.
    ///
    /// Destroys previous top scene first, delays a frame, and constructs a new scene afterwards.
    /// @note If you don't want to delay a frame between scene destruction and construction,
    /// use `reserve_replace_top()` instead.
    template <std::derived_from<scene> Scene, typename... Args>
    void reserve_replace_top_with_delay(Args&&... args)
    {
        reserve_add_impl<Scene>(reserved_change::kind::REPLACE_TOP, true, std::forward<Args>(args)...);
    }

    /// @brief Reserves pop of the top scene.
    ///
    /// Destroys previous top scene first, and calls `scene::uncover()` for the next top scene afterwards.
    /// @note If you want to delay a frame between scene destruction and `scene::uncover()`,
    /// use `reserve_pop_with_delay()` instead.
    void reserve_pop();

    /// @brief Reserves pop of the top scene.
    ///
    /// Destroys previous top scene first, delays a frame,
    /// and calls `scene::uncover()` for the next top scene afterwards.
    /// @note If you don't want to delay a frame between scene destruction and `scene::uncover()`,
    /// use `reserve_pop()` instead.
    void reserve_pop_with_delay();

    /// @brief Reserves clearing all the scenes in the stack.
    void reserve_clear();

private:
    scene_pool_t _scene_pool;
    bn::vector<scene_ptr, MAX_SCENE_COUNT> _scenes;

    bn::vector<reserved_change, MAX_SCENE_COUNT> _reserved_changes;
};

} // namespace jb::scn
