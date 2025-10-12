#ifndef VKSENGINEPOINTLIGHTCOMPONENT_H
#define VKSENGINEPOINTLIGHTCOMPONENT_H
#include "ComponentType.h"
#include "../../objects/lights/PointLight.h"
#include "../../objects/menus/PointLightMenu.h"

namespace vks_engine
{
    struct PointLightComponent
    {
        PointLight m_Light;

        PointLightMenu m_Menu;

        void bind();

        constexpr ComponentType getType() const {return ComponentType::POINT_LIGHT;}
    };
}


#endif //VKSENGINEPOINTLIGHTCOMPONENT_H
