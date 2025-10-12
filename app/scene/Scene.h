#ifndef VKSENGINESCENE_H
#define VKSENGINESCENE_H
#include <vector>

#include "FileExplorer.h"
#include "SceneComponent.h"
#include "../handlers/ImGuiHandler.h"
#include "../Window.h"
#include "../handlers/InputHandler.h"
#include "../objects/Mesh.h"
#include "../objects/lights/DirectionalLight.h"
#include "../objects/lights/PointLight.h"
#include "../objects/timer/Clock.h"
#include "components/DirectionalLightComponent.h"
#include "components/MeshComponent.h"
#include "components/PointLightComponent.h"
#include "menus/SceneComponentPropertiesMenu.h"
#include "menus/SceneComponentsMenu.h"
#include "menus/SceneFunctionsMenu.h"
#include "ubocomponents/UBOvp.h"
#include "ubocomponents/UBOcounters.h"

namespace vks_engine
{
    class Scene
    {
    private:
        static constexpr uint32_t MAX_ALLOWED_MESH_COUNT = 10;

        static constexpr uint32_t MAX_ALLOWED_POINT_LIGHT_COUNT = 10;

        static constexpr uint32_t MAX_ALLOWED_DIRECTIONAL_LIGHT_COUNT = 10;

    public:
        Scene(Window &window, VulkanHandler &vkHandler);

        void init();

        void run();

        friend class Application;

    private:
        //======== MEMBERS ========

        Window &m_Window;

        VulkanHandler &m_Vk;

        ImGuiHandler m_ImGui;

        InputHandler m_InputHandler;

        Camera m_Camera;

        Clock m_Clock;

        FileExplorer m_FileExplorer;

        vk::Format m_ColorAttachmentFormat;

        vk::Format m_DepthAttachmentFormat;

        //=====================================MESH RELATED=====================================

        std::vector<vk::raii::CommandBuffer> m_SimpleMeshCommandBuffers;

        std::vector<vk::raii::CommandBuffer> m_ComplexMeshCommandBuffers;

        std::vector<MeshComponent> m_SimpleMeshComponents;

        std::vector<MeshComponent> m_ComplexMeshComponents;

        uint32_t m_CurrentMeshCount;

        void recordMeshCommands(uint32_t currentFrame);

        void recordSimpleMeshCommands(uint32_t currentFrame);

        void recordComplexMeshCommands(uint32_t currentFrame);

        void addMesh(const std::string &path);

        void addSphereMesh();

        //=====================================CAMERA RELATED=====================================

        //======== MVP UBO ========

        UBOvp m_UBOvp;

        UniformBuffer m_UBOvpBuffer;

        void updateUBOvpCam();

        void updateCamera(float deltaTime);

        //=====================================LIGHTS RELATED=====================================

        //======== PointLight UBO ========

        uint32_t m_ActivePointLights;

        std::array<PointLightComponent, MAX_ALLOWED_POINT_LIGHT_COUNT> m_PointLightComponents;

        UniformBuffer m_UBOPointLightBuffer;

        std::array<PointLight::Aligned, MAX_ALLOWED_POINT_LIGHT_COUNT> m_UBOpointLight;

        void initUBOPointLight();

        void updateUBOpointLight(const PointLight &pointLight);

        void addPointLight();

        //======== DirectionalLight UBO ========

        uint32_t m_ActiveDirectionalLights;

        std::array<DirectionalLightComponent, MAX_ALLOWED_DIRECTIONAL_LIGHT_COUNT> m_DirectionalLightComponents;

        UniformBuffer m_UBODirectionalLightBuffer;

        std::array<DirectionalLight::Aligned, MAX_ALLOWED_DIRECTIONAL_LIGHT_COUNT> m_UBOdirectionalLight;

        void initUBODirectionalLight();

        void updateUBOdirectionalLight(const DirectionalLight &directionalLight);

        void addDirectionalLight();

        //======== COUNTERS UBO ========

        UBOcounters m_Counters;

        UniformBuffer m_UBOCountersBuffer;

        void initUBOCounters();

        void updateUBOcounters();

        //=====================================MENU RELATED=====================================

        SceneFunctionsMenu m_SceneFunctionsMenu;

        SceneComponentsMenu m_SceneComponentsMenu;

        SceneComponentPropertiesMenu m_SceneComponentPropertiesMenu;

        void initMenus();

        void initSceneFunctionsMenu();

        void initSceneComponentsMenu();

        void initSceneComponentPropertiesMenu();

        void renderComponentMenus();

        void renderMenus();

        //======== FUNCTIONS ========

        void updateScene();

        void render();

        void waitForFences();

        void handleSwapChainRecreation(int width, int height);

        void createCommandBuffers();

        void createUniformBuffers();

        void initUBOmvp();

        void initUBO();

        void updateUBO(uint32_t frame);

        //======== Event handlers ========
        void setInputHandlers();

        void handleFramebufferResize(int width, int height);

        void handleEvents();
    };
}

#endif //VKSENGINESCENE_H
