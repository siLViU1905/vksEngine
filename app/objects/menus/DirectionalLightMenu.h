#ifndef VKSENGINE_DIRECTIONALLIGHTMENU_H
#define VKSENGINE_DIRECTIONALLIGHTMENU_H
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


#endif //VKSENGINE_DIRECTIONALLIGHTMENU_H
