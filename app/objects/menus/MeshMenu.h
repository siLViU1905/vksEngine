#ifndef MESHMENU_H
#define MESHMENU_H
#include "Menu.h"
#include "../Mesh.h"


class MeshMenu : protected Menu<Mesh>{
public:
    MeshMenu();

    MeshMenu(Mesh& mesh, const std::string& title = "");

    bool render() override;

    using Menu::setComponent;
    using Menu::getComponent;
    using Menu::setTitle;
    using Menu::getTitle;
};



#endif //MESHMENU_H
