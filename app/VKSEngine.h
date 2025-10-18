#ifndef VKSENGINE_H
#define VKSENGINE_H
#include <cstdint>

namespace vks_engine
{
        static constexpr uint32_t SCENE_MAX_ALLOWED_MESH_COUNT = 10;

        static constexpr uint32_t SCENE_MAX_ALLOWED_POINT_LIGHT_COUNT = 10;

        static constexpr uint32_t SCENE_MAX_ALLOWED_DIRECTIONAL_LIGHT_COUNT = 10;

        static constexpr float SCENE_MESH_POSITION_UPPER_BOUND = 20.f;

        static constexpr float SCENE_MESH_POSITION_LOWER_BOUND = -20.f;

        static constexpr float SCENE_MESH_SCALE_UPPER_BOUND = 10.f;

        static constexpr float SCENE_MESH_SCALE_LOWER_BOUND = 0.05f;

        static constexpr float SCENE_LIGHT_POSITION_UPPER_BOUND = 10.f;

        static constexpr float SCENE_LIGHT_POSITION_LOWER_BOUND = -10.f;

        static constexpr uint32_t SUPPORTED_TEXTURE_TYPES_COUNT = 3;

        static constexpr uint32_t USED_UNIFORM_BUFFERS = 4;
}

#endif //VKSENGINE_H
