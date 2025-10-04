#ifndef POINTLIGHTMENU_H
#define POINTLIGHTMENU_H
#include "Menu.h"
#include "../lights/PointLight.h"


class PointLightMenu: protected Menu<PointLight>{
public:
    PointLightMenu();

    PointLightMenu(PointLight& pointLight, const std::string& title = "");

    bool render() override;

    using Menu::setComponent;
    using Menu::getComponent;
    using Menu::setTitle;
    using Menu::getTitle;
};



#endif //POINTLIGHTMENU_H
