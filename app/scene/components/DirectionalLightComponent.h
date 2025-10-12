#ifndef VKSENGINEDIRECTIONALLIGHTCOMPONENT_H
#define VKSENGINEDIRECTIONALLIGHTCOMPONENT_H
#include "ComponentType.h"
#include "../../objects/lights/DirectionalLight.h"
#include "../../objects/menus/DirectionalLightMenu.h"

namespace vks_engine
{
    struct DirectionalLightComponent
    {
        DirectionalLight m_Light;

        DirectionalLightMenu m_Menu;

        void bind();

        constexpr ComponentType getType() const {return ComponentType::DIRECTIONAL_LIGHT;}
    };
}

#endif //VKSENGINEDIRECTIONALLIGHTCOMPONENT_H
