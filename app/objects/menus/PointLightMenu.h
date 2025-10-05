#ifndef VKSENGINEPOINTLIGHTMENU_H
#define VKSENGINEPOINTLIGHTMENU_H
#include "Menu.h"
#include "../lights/PointLight.h"

namespace vks_engine
{
    class PointLightMenu : protected Menu<PointLight>
    {
    public:
        PointLightMenu();

        PointLightMenu(PointLight &pointLight, const std::string &title = "");

        bool render() override;

        using Menu::setComponent;
        using Menu::getComponent;
        using Menu::setTitle;
        using Menu::getTitle;
    };
}


#endif //VKSENGINEPOINTLIGHTMENU_H
