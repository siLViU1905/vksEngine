//
// Created by Silviu on 20.09.2025.
//

#ifndef DIRECTIONALLIGHTCOMPONENT_H
#define DIRECTIONALLIGHTCOMPONENT_H
#include "../../objects/lights/DirectionalLight.h"
#include "../../objects/menus/DirectionalLightMenu.h"


struct DirectionalLightComponent {
  DirectionalLight m_Light;

  DirectionalLightMenu m_Menu;

  void bind();
};

#endif //DIRECTIONALLIGHTCOMPONENT_H
