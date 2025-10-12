#ifndef VKSENGINECOMPONENTTYPE_H
#define VKSENGINECOMPONENTTYPE_H
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

#endif //VKSENGINECOMPONENTTYPE_H
