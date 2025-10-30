#ifndef VKSENGINE_SCENEFUNCTIONSMENU_H
#define VKSENGINE_SCENEFUNCTIONSMENU_H
#include <functional>

#include "../../objects/menus/Menu.h"
#include "../../objects/mesh/MeshType.h"

namespace vks_engine
{
    class SceneFunctionsMenu : public Menu
    {
    public:
        bool render() override;

        void onAddPointLightBtnClick(std::function<void()> function);

        void onAddDirectionalLightBtnClick(std::function<void()> function);

        void onMeshTypeSelection(std::function<void(MeshType)> function);

    private:
        std::function<void()> m_BtnAddPointLightCallback;

        std::function<void()> m_BtnAddDirectionalLightCallback;

        std::function<void(MeshType)> m_MeshTypeSelectionCallback;
    };
}

#endif //VKSENGINE_SCENEFUNCTIONSMENU_H
