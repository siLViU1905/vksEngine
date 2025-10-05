#ifndef VKSENGINEDIRECTIONALLIGHTMENU_H
#define VKSENGINEDIRECTIONALLIGHTMENU_H
#include "Menu.h"
#include "../lights/DirectionalLight.h"

namespace vks_engine
{
    class DirectionalLightMenu : protected Menu<DirectionalLight>
    {
    public:
        DirectionalLightMenu();

        DirectionalLightMenu(DirectionalLight &directionalLight, const std::string &title = "");

        bool render() override;

        using Menu::setComponent;
        using Menu::getComponent;
        using Menu::setTitle;
        using Menu::getTitle;
    };
}


#endif //VKSENGINEDIRECTIONALLIGHTMENU_H
