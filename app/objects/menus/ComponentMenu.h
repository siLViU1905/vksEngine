#ifndef VKSENGINECOMPONENTMENU_H
#define VKSENGINECOMPONENTMENU_H

#include "Menu.h"
#include <imgui.h>

namespace vks_engine
{
    template<typename Component>
    class ComponentMenu : public Menu
    {
    public:
        ComponentMenu():m_Component(nullptr)
        {}

        ComponentMenu(Component& component, const std::string& title = "NULL"):Menu(title), m_Component(&component)
        {}

        void setComponent(Component& component)
        {
            m_Component = &component;
        }

        Component& getComponent() {return *m_Component;}

    protected:
        Component* m_Component;
    };
}

#endif //VKSENGINECOMPONENTMENU_H
