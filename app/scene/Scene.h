#ifndef vks_engineSCENE_H
#define vks_engineSCENE_H

#include <deque>
#include <mutex>
#include <vector>
#include "FileExplorer.h"
#include "SceneComponent.h"
#include "../handlers/ImGuiHandler.h"
#include "../Window.h"
#include "../handlers/ImNodesHandler.h"
#include "../handlers/InputHandler.h"
#include "../objects/mesh/Mesh.h"
#include "../objects/lights/DirectionalLight.h"
#include "../objects/lights/PointLight.h"
#include "../objects/timer/Clock.h"
#include "components/DirectionalLightComponent.h"
#include "components/MeshComponent.h"
#include "components/PointLightComponent.h"
#include "menus/MaterialEditorMenu.h"
#include "menus/SceneComponentPropertiesMenu.h"
#include "menus/SceneComponentsMenu.h"
#include "menus/SceneEventsMenu.h"
#include "menus/SceneFunctionsMenu.h"
#include "menus/SceneInfoMenu.h"
#include "ubocomponents/UBOvp.h"
#include "ubocomponents/UBOcounters.h"

namespace vks_engine
{
    class Scene
    {
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

        ImNodesHandler m_ImNodes;

        InputHandler m_InputHandler;

        Camera m_Camera;

        Clock m_Clock;

        FileExplorer m_FileExplorer;

        vk::Format m_ColorAttachmentFormat;

        vk::Format m_DepthAttachmentFormat;

        //=====================================MESH RELATED=====================================

        uint32_t m_CurrentSimpleMeshCount;

        uint32_t m_CurrentComplexMeshCount;

        std::vector<vk::raii::CommandBuffer> m_SimpleMeshCommandBuffers;

        std::vector<vk::raii::CommandBuffer> m_ComplexMeshCommandBuffers;

        std::vector<MeshComponent> m_SimpleMeshComponents;

        std::vector<MeshComponent> m_ComplexMeshComponents;

        constexpr uint32_t getTotalMeshCount() const
        {
            return m_CurrentSimpleMeshCount + m_CurrentComplexMeshCount;
        }

        std::mutex m_ComplexMeshCountMutex;

        void recordMeshCommands(uint32_t currentFrame);

        void recordSimpleMeshCommands(uint32_t currentFrame);

        void recordComplexMeshCommands(uint32_t currentFrame);

        void loadModelWorker(std::string_view path);

        void addSphereMesh();

        void addCubeMesh();

        void addMesh(MeshType type);

        //=====================================MODEL LOADING RELATED=====================================

        std::mutex m_PendingModelsMutex;

        std::deque<std::string> m_PendingModelPaths;

        std::mutex m_LoadedMeshMutex;

        std::deque<MeshComponent> m_LoadedMeshQueue;

        void handleFileSelected(const std::vector<std::string>& paths);

        void handleLoadedModel(MeshComponent &component);

        void procesPendingActions();

        void processLoadedModels();

        void initFileExplorer();

        //=====================================CAMERA RELATED=====================================

        //======== MVP UBO ========

        UBOvp m_UBOvp;

        UniformBuffer m_UBOvpBuffer;

        void updateUBOvpCam();

        void updateCamera(float deltaTime);

        //=====================================LIGHTS RELATED=====================================

        constexpr uint32_t getActiveLightsCount() const {return m_ActivePointLights + m_ActiveDirectionalLights;}

        //======== PointLight UBO ========

        uint32_t m_ActivePointLights;

        std::array<PointLightComponent, SCENE_MAX_ALLOWED_POINT_LIGHT_COUNT> m_PointLightComponents;

        UniformBuffer m_UBOPointLightBuffer;

        std::array<PointLight::Aligned, SCENE_MAX_ALLOWED_POINT_LIGHT_COUNT> m_UBOpointLight;

        void initUBOPointLight();

        void updateUBOpointLight(const PointLight &pointLight);

        void addPointLight();

        //======== DirectionalLight UBO ========

        uint32_t m_ActiveDirectionalLights;

        std::array<DirectionalLightComponent, SCENE_MAX_ALLOWED_DIRECTIONAL_LIGHT_COUNT>
        m_DirectionalLightComponents;

        UniformBuffer m_UBODirectionalLightBuffer;

        std::array<DirectionalLight::Aligned, SCENE_MAX_ALLOWED_DIRECTIONAL_LIGHT_COUNT>
        m_UBOdirectionalLight;

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

        SceneInfoMenu m_SceneInfoMenu;

        SceneEventsMenu m_SceneEventsMenu;

        MaterialEditorMenu m_MaterialEditor;

        void initMenus();

        void fixMenus();

        void initSceneFunctionsMenu();

        void initSceneComponentsMenu();

        void initSceneComponentPropertiesMenu();

        void initSceneInfoMenu();

        void initSceneEventsMenu();

        void initMaterialEditorMenu();

        void renderMenus();

        void handleComponentRename(ComponentEntry &entry, std::string_view newName);

        void renameMesh(const Mesh &mesh, std::string_view newName);

        void renamePointLight(const PointLight &pl, std::string_view newName);

        void renameDirectionalLight(const DirectionalLight &dl, std::string_view newName);

        void handleComponentDelete(ComponentEntry &entry);

        void deleteMesh(const Mesh &mesh);

        void deletePointLight(const PointLight& pl);

        void deleteDirectionalLight(const DirectionalLight& dl);

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

        void setupKeyHandlers();

        void setupButtonHandlers();

        void handleFramebufferResize(int width, int height);

        void handleEvents();
    };
}

#endif //vks_engineSCENE_H
