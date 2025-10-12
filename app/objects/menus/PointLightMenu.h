#ifndef VKSENGINEPOINTLIGHTMENU_H
#define VKSENGINEPOINTLIGHTMENU_H
#include "ComponentMenu.h"
#include "../lights/PointLight.h"

namespace vks_engine
{
    class PointLightMenu : protected ComponentMenu<PointLight>
    {
    public:
        PointLightMenu();

        PointLightMenu(PointLight &pointLight, const std::string &title = "");

        bool render() override;

        using ComponentMenu::setComponent;
        using ComponentMenu::getComponent;
        using ComponentMenu::setTitle;
        using ComponentMenu::getTitle;
    };
}


#endif //VKSENGINEPOINTLIGHTMENU_H
