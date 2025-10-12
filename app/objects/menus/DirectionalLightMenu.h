#ifndef VKSENGINEDIRECTIONALLIGHTMENU_H
#define VKSENGINEDIRECTIONALLIGHTMENU_H
#include "ComponentMenu.h"
#include "../lights/DirectionalLight.h"

namespace vks_engine
{
    class DirectionalLightMenu : protected ComponentMenu<DirectionalLight>
    {
    public:
        DirectionalLightMenu();

        DirectionalLightMenu(DirectionalLight &directionalLight, const std::string &title = "");

        bool render() override;

        using ComponentMenu::setComponent;
        using ComponentMenu::getComponent;
        using ComponentMenu::setTitle;
        using ComponentMenu::getTitle;
    };
}


#endif //VKSENGINEDIRECTIONALLIGHTMENU_H
