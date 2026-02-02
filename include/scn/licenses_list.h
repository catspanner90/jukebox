#pragma once

#include "scn/scene.h"

#include <bn_array.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>

#include <cstdint>

#include "gen/licenses.h"

namespace jb::scn
{

class licenses_list final : public scene
{
public:
    licenses_list(int license_idx, scene_context&);

public:
    bool update() override;

private:
    void move_cursor_idx(int diff);

    void recolor_license(int license_idx);

private:
    bn::vector<bn::sprite_ptr, 128> _names_sprites;
    bn::array<std::uint8_t, gen::LICENSE_NAMES.size() + 1> _name_start_idxes;

    int _cursor_idx;
};

} // namespace jb::scn
