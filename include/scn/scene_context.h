#pragma once

#include "sys/config_save.h"
#include "sys/text_generators.h"

#include "ibn_observer.h"
#include "ibn_transitions.h"

#include <bn_array.h>
#include <bn_bitmap_bg.h>
#include <bn_color.h>
#include <bn_span.h>

namespace jb::scn
{

class scene_stack;

class scene_context final
{
public:
    scene_context(scene_stack& st) : _scene_stack(st)
    {
    }

    scene_context(const scene_context&) = delete;
    scene_context& operator=(const scene_context&) = delete;

private:
    scene_stack& _scene_stack;

    sys::config_save _config_save;
    ibn::transitions _transitions;
    sys::text_generators _text_generators;

    bn::array<bn::color, bn::bitmap_bg::dp_direct_height() * bn::bitmap_bg::dp_direct_height()>
        _decompressed_bitmap_bg_colors;

public:
    auto stack() -> decltype((_scene_stack))
    {
        return _scene_stack;
    }

    auto stack() const -> decltype((_scene_stack))
    {
        return _scene_stack;
    }

    auto config_save() -> decltype((_config_save))
    {
        return _config_save;
    }

    auto config_save() const -> decltype((_config_save))
    {
        return _config_save;
    }

    auto transitions() -> decltype((_transitions))
    {
        return _transitions;
    }

    auto transitions() const -> decltype((_transitions))
    {
        return _transitions;
    }

    auto text_generators() -> decltype((_text_generators))
    {
        return _text_generators;
    }

    auto text_generators() const -> decltype((_text_generators))
    {
        return _text_generators;
    }

    auto decompressed_bitmap_bg_colors() -> bn::span<bn::color>
    {
        return _decompressed_bitmap_bg_colors;
    }

    auto decompressed_bitmap_bg_colors() const -> bn::span<const bn::color>
    {
        return _decompressed_bitmap_bg_colors;
    }
};

} // namespace jb::scn
