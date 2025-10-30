#ifndef VKSENGINE_SCENECOMPONENTSMENU_H
#define VKSENGINE_SCENECOMPONENTSMENU_H
#include <algorithm>
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

        void setOnComponentRename(std::function<void(ComponentEntry&, std::string_view)> function);

        void setOnComponentDelete(std::function<void(ComponentEntry&)> function);

        void deleteComponentEntry(const ComponentEntry& component);

    private:
        std::vector<ComponentEntry> m_Components;

        std::function<void(ComponentEntry&)> m_OnComponentSelection;

        std::function<void(ComponentEntry&, std::string_view)> m_OnComponentRename;

        std::function<void(ComponentEntry&)> m_OnComponentDelete;
    };
}

#endif //VKSENGINE_SCENECOMPONENTSMENU_H
