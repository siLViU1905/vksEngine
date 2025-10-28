#ifndef VKSENGINEMESHMENU_H
#define VKSENGINEMESHMENU_H
#include "ComponentMenu.h"
#include "../mesh/Mesh.h"

namespace vks_engine
{
    class MeshMenu : protected ComponentMenu<Mesh>
    {
    public:
        MeshMenu();

        MeshMenu(Mesh &mesh, const std::string &title = "");

        bool render() override;

        using ComponentMenu::setComponent;
        using ComponentMenu::getComponent;
        using ComponentMenu::setTitle;
        using ComponentMenu::getTitle;

        static void setOnEditMaterial(std::function<void(Mesh& mesh)> function);
    private:
        static std::function<void(Mesh& mesh)> m_OnEditMaterialPress;
    };
}


#endif //VKSENGINEMESHMENU_H
