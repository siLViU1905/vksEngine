#ifndef VKSENGINE_SCENEINFOMENU_H
#define VKSENGINE_SCENEINFOMENU_H
#include "../../objects/menus/Menu.h"
#include <cstdint>

namespace vks_engine
{
    class SceneInfoMenu : public Menu
    {
    public:
        bool render() override;

        SceneInfoMenu(const uint32_t& simpleMesh, const uint32_t& complexMesh, const uint32_t& pl, const uint32_t& dl);

    private:
        void renderCapabilities();

        void renderCurrentState();

        const uint32_t& m_ActiveSimpleMeshCount;

        const uint32_t& m_ActiveComplexMeshCount;

        const uint32_t& m_ActivePointLightsCount;

        const uint32_t& m_ActiveDirectionalLightsCount;
    };
}

#endif //VKSENGINE_SCENEINFOMENU_H
