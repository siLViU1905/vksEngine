#ifndef VKSENGINESCENECOMPONENTSMENU_H
#define VKSENGINESCENECOMPONENTSMENU_H
#include <functional>
#include <vector>

#include "../../objects/menus/Menu.h"
#include "../components/ComponentType.h"
#include "../components/ComponentEntry.h"

namespace vks_engine
{
    class SceneComponentsMenu : public Menu
    {
    public:
        bool render() override;

        template<typename Component>
        void addComponent(ComponentType type, Component &component)
        {
            auto &newComponent = m_Components.emplace_back();

            newComponent.m_Type = type;

            newComponent.p_Component = &component;

            newComponent.m_Name = component.m_Menu.getTitle();
        }

        void setOnComponentSelected(std::function<void(ComponentEntry&)> function);

    private:
        std::vector<ComponentEntry> m_Components;

        std::function<void(ComponentEntry&)> m_OnComponentSelection;
    };
}

#endif //VKSENGINESCENECOMPONENTSMENU_H
