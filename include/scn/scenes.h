#pragma once

#include "scn/jukebox.h"
#include "scn/license_print.h"
#include "scn/licenses_list.h"

#include <algorithm>

namespace jb::scn
{

inline constexpr int MAX_SCENE_SIZE = std::max({
    sizeof(jukebox),
    sizeof(licenses_list),
    sizeof(license_print),
});

inline constexpr int MAX_SCENE_ALIGN = std::max({
    alignof(jukebox),
    alignof(licenses_list),
    alignof(license_print),
});

} // namespace jb::scn
