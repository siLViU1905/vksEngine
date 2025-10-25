#ifndef VKSENGINESCENECOMPONENTPROPERTIESMENU_H
#define VKSENGINESCENECOMPONENTPROPERTIESMENU_H

#include <functional>

#include "../../objects/lights/PointLight.h"
#include "../../objects/menus/Menu.h"
#include "../components/ComponentEntry.h"

namespace vks_engine
{
    class Mesh;
    class DirectionalLight;

    class SceneComponentPropertiesMenu : public Menu
    {
    public:
        bool render() override;

        void setActiveComponent(ComponentEntry& component);

        void handleComponentDelete(const ComponentEntry &entry);

        void setOnPointLightPropertiesChange(std::function<void(const PointLight &)> function);

        void setOnDirectionalLightPropertiesChange(std::function<void(const DirectionalLight &)> function);

    private:
        ComponentEntry m_ActiveComponent;

        std::function<void(const PointLight &)> m_OnPointLightPropertiesChange;

        std::function<void(const DirectionalLight &)> m_OnDirectionalLightPropertiesChange;
    };
}

#endif //VKSENGINESCENECOMPONENTPROPERTIESMENU_H
