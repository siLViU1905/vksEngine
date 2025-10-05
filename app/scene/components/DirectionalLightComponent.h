#ifndef VKSENGINEDIRECTIONALLIGHTCOMPONENT_H
#define VKSENGINEDIRECTIONALLIGHTCOMPONENT_H
#include "../../objects/lights/DirectionalLight.h"
#include "../../objects/menus/DirectionalLightMenu.h"

namespace vks_engine
{
    struct DirectionalLightComponent
    {
        DirectionalLight m_Light;

        DirectionalLightMenu m_Menu;

        void bind();
    };
}

#endif //VKSENGINEDIRECTIONALLIGHTCOMPONENT_H
