#ifndef VKSENGINE_MESHCOMPONENT_H
#define VKSENGINE_MESHCOMPONENT_H
#include "ComponentType.h"
#include "../../objects/mesh/Mesh.h"
#include "../../objects/menus/MeshMenu.h"

namespace vks_engine
{
    struct MeshComponent
    {
        Mesh m_Mesh;

        MeshMenu m_Menu;

        void bind();

        constexpr ComponentType getType() const {return ComponentType::MESH;}
    };
}

#endif //VKSENGINE_MESHCOMPONENT_H
