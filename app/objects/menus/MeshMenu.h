#ifndef VKSENGINEMESHMENU_H
#define VKSENGINEMESHMENU_H
#include "Menu.h"
#include "../Mesh.h"

namespace vks_engine
{
    class MeshMenu : protected Menu<Mesh>
    {
    public:
        MeshMenu();

        MeshMenu(Mesh &mesh, const std::string &title = "");

        bool render() override;

        using Menu::setComponent;
        using Menu::getComponent;
        using Menu::setTitle;
        using Menu::getTitle;
    };
}


#endif //VKSENGINEMESHMENU_H
