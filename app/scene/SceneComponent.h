#ifndef VKSENGINESCENECOMPONENT_H
#define VKSENGINESCENECOMPONENT_H
#include "../objects/menus/Menu.h"

namespace vks_engine
{
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
}


#endif //VKSENGINESCENECOMPONENT_H
