#ifndef SCENECOMPONENT_H
#define SCENECOMPONENT_H
#include "../objects/menus/Menu.h"

template<typename Component>
class SceneComponent
{
public:
    Component m_Mesh;

    Menu<Component> m_Menu;

    void bind()
    {
        m_Menu.setComponent(m_Mesh);
    }
};


#endif //SCENECOMPONENT_H
