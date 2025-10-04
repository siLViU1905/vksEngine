#ifndef MESHCOMPONENT_H
#define MESHCOMPONENT_H
#include "../../objects/Mesh.h"
#include "../../objects/menus/MeshMenu.h"

struct MeshComponent
{
    Mesh m_Mesh;

    MeshMenu m_Menu;

    void bind();
};

#endif //MESHCOMPONENT_H
