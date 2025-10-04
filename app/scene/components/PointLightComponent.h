//
// Created by Silviu on 20.09.2025.
//

#ifndef POINTLIGHTCOMPONENT_H
#define POINTLIGHTCOMPONENT_H
#include "../../objects/lights/PointLight.h"
#include "../../objects/menus/PointLightMenu.h"


struct PointLightComponent {
    PointLight m_Light;

    PointLightMenu m_Menu;

    void bind();
};



#endif //POINTLIGHTCOMPONENT_H
