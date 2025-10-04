#ifndef SCENE_H
#define SCENE_H
#include <vector>

#include "FileExplorer.h"
#include "SceneComponent.h"
#include "../ImGuiHandler.h"
#include "../Window.h"
#include "../objects/Mesh.h"
#include "../objects/lights/DirectionalLight.h"
#include "../objects/lights/PointLight.h"
#include "../objects/timer/Clock.h"
#include "components/DirectionalLightComponent.h"
#include "components/MeshComponent.h"
#include "components/PointLightComponent.h"
#include "ubocomponents/UBOmvp.h"
#include "ubocomponents/UBOcounters.h"

class Scene
{
private:
    static constexpr uint32_t MAX_ALLOWED_MESH_COUNT = 2;

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

    Camera m_Camera;

    Clock m_Clock;

    FileExplorer m_FileExplorer;

    vk::Format m_ColorAttachmentFormat;

    vk::Format m_DepthAttachmentFormat;

    std::vector<vk::raii::CommandBuffer> m_MeshCommandBuffers;

    std::vector<MeshComponent> m_MeshComponents;

    //=====================================CAMERA RELATED=====================================

    //======== MVP UBO ========

    UBOmvp<MAX_ALLOWED_MESH_COUNT> m_UBOmvp;

    UniformBuffer m_UBOmvpBuffer;

    void updateUBOmvpModel(const Mesh &mesh);

    void updateUBOmvpCam();

    void updateCamera(float deltaTime);

    //=====================================LIGHTS RELATED=====================================

    //======== PointLight UBO ========

    uint32_t m_ActivePointLights;

    std::array<PointLightComponent, MAX_ALLOWED_POINT_LIGHT_COUNT> m_PointLightComponents;

    UniformBuffer m_UBOPointLightBuffer;

    std::array<PointLight::Aligned, MAX_ALLOWED_POINT_LIGHT_COUNT> m_UBOpointLight;

    void initUBOPointLight();

    void updateUBOpointLight(const PointLight &pointLight);

    //======== DirectionalLight UBO ========

    uint32_t m_ActiveDirectionalLights;

    std::array<DirectionalLightComponent, MAX_ALLOWED_DIRECTIONAL_LIGHT_COUNT> m_DirectionalLightComponents;

    UniformBuffer m_UBODirectionalLightBuffer;

    std::array<DirectionalLight::Aligned, MAX_ALLOWED_DIRECTIONAL_LIGHT_COUNT> m_UBOdirectionalLight;

    void initUBODirectionalLight();

    void updateUBOdirectionalLight(const DirectionalLight &directionalLight);

    //======== COUNTERS UBO ========

    UBOcounters m_Counters;

    UniformBuffer m_UBOCountersBuffer;

    void initUBOCounters();

    void updateUBOcounters();

    //======== FUNCTIONS ========

    void updateScene();

    void render();

    void waitForFences();

    void handleSwapChainRecreation();

    void createCommandBuffers();

    void createUniformBuffers();

    void initUBOmvp();

    void initUBO();

    void addMesh(const std::string &path);

    void addSphereMesh();

    void addPointLight();

    void addDirectionalLight();

    void recordMeshCommands();

    void renderMenus();

    void updateUBO(uint32_t frame);

    void handleEvents();
};


#endif //SCENE_H
