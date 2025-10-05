#ifndef VKSENGINEMESHCOMPONENT_H
#define VKSENGINEMESHCOMPONENT_H
#include "../../objects/Mesh.h"
#include "../../objects/menus/MeshMenu.h"

namespace vks_engine
{
    struct MeshComponent
    {
        Mesh m_Mesh;

        MeshMenu m_Menu;

        void bind();
    };
}

#endif //VKSENGINEMESHCOMPONENT_H
