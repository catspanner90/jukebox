#pragma once

#include "scn/scene.h"

namespace jb::scn
{

class jukebox final : public scene
{
public:
    jukebox(scene_context&);

public:
    bool update() override;
};

} // namespace jb::scn
