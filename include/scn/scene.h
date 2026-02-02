#pragma once

#include <bn_type_id.h>

namespace jb::scn
{

class scene_stack;
class scene_context;

class scene
{
public:
    /// @brief Virtual destructor.
    virtual ~scene() = default;

    /// @brief Constructor.
    scene(scene_context& ctx) : _ctx(ctx)
    {
    }

    scene(const scene&) = delete;
    scene& operator=(const scene&) = delete;

public:
    /// @brief Callback to be called when the scene has been covered by other scene, thus no longer top anymore.
    /// @param new_top_scene_type Type of the scene that covered this scene.
    virtual void cover([[maybe_unused]] bn::type_id_t new_top_scene_type) {};

    /// @brief Callback to be called when the upper scene has been removed, thus this scene became top once again.
    virtual void uncover() {};

public:
    /// @brief Updates the scene.
    /// @return `true` if `update()` should be called for the next scene below the stack.
    virtual bool update() = 0;

protected:
    auto context() -> scene_context&
    {
        return _ctx;
    }

    auto context() const -> const scene_context&
    {
        return _ctx;
    }

private:
    scene_context& _ctx;
};

} // namespace jb::scn
