#include "ComponentType.h"

#include <unordered_map>

namespace vks_engine
{
    std::string componentType_to_string(ComponentType type)
    {
        static const std::unordered_map<int, std::string> typeToStrMap =
        {
            {static_cast<int>(ComponentType::NaN), "Nan"},
            {static_cast<int>(ComponentType::MESH), "Mesh"},
            {static_cast<int>(ComponentType::POINT_LIGHT), "PointLight"},
            {static_cast<int>(ComponentType::DIRECTIONAL_LIGHT), "DirectionalLight"}
        };

        return typeToStrMap.at(static_cast<int>(type));
    }
}
