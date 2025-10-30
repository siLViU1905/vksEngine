#ifndef VKSENGINE_COMPONENTTYPE_H
#define VKSENGINE_COMPONENTTYPE_H
#include <string>

namespace vks_engine
{
    enum class ComponentType
    {
        NaN,
        MESH,
        POINT_LIGHT,
        DIRECTIONAL_LIGHT
    };

    std::string componentType_to_string(ComponentType type);
}

#endif //VKSENGINE_COMPONENTTYPE_H
