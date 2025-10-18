#ifndef VKSENGINE_H
#define VKSENGINE_H
#include <cstdint>

namespace vks_engine
{
        static constexpr uint32_t SCENE_MAX_ALLOWED_MESH_COUNT = 10;

        static constexpr uint32_t SCENE_MAX_ALLOWED_POINT_LIGHT_COUNT = 10;

        static constexpr uint32_t SCENE_MAX_ALLOWED_DIRECTIONAL_LIGHT_COUNT = 10;

        static constexpr uint32_t SUPPORTED_TEXTURE_TYPES_COUNT = 3;

        static constexpr uint32_t USED_UNIFORM_BUFFERS = 4;
}

#endif //VKSENGINE_H
