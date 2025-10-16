#ifndef VKSENGINESCENEFUNCTIONSMENU_H
#define VKSENGINESCENEFUNCTIONSMENU_H
#include <functional>

#include "../../objects/menus/Menu.h"

namespace vks_engine
{
    class SceneFunctionsMenu : public Menu
    {
    public:
        bool render() override;

        void onAddMeshBtnClick(std::function<void()> function);

        void onAddPointLightBtnClick(std::function<void()> function);

        void onAddDirectionalLightBtnClick(std::function<void()> function);

        void onMeshTypeSelection(std::function<void()> function);

    private:
        std::function<void()> m_BtnAddMeshCallback;

        std::function<void()> m_BtnAddPointLightCallback;

        std::function<void()> m_BtnAddDirectionalLightCallback;

        std::function<void()> m_MeshTypeSelectionCallback;
    };
}

#endif //VKSENGINESCENEFUNCTIONSMENU_H
