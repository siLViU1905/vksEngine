#include "Scene.h"

#include <iostream>
#include <thread>
#include <glm/gtc/type_ptr.inl>

#include "../objects/menus/MeshMenu.h"

Scene::Scene(Window &window, VulkanHandler &vkHandler): m_Vk(vkHandler),
                                                        m_Window(window),
                                                        m_Camera(window.getWindow(), glm::vec3(0.f, 0.f, 2.f), 5.f),
                                                        m_ActiveDirectionalLights(0), m_ActivePointLights(0)
{
}

void Scene::init()
{
    m_ImGui.init(m_Vk, m_Window.getWindow());

    m_ColorAttachmentFormat = m_ImGui.m_ColorAttachmentFormat;

    m_DepthAttachmentFormat = m_ImGui.m_DepthAttachmentFormat;

    initUBO();

    createCommandBuffers();

    createUniformBuffers();

    m_Vk.createComplexMeshDescriptorSets(m_UBOmvpBuffer, sizeof(UBOmvp<MAX_ALLOWED_MESH_COUNT>),
                                         m_UBOPointLightBuffer,
                                         sizeof(PointLight::Aligned) * MAX_ALLOWED_POINT_LIGHT_COUNT,
                                         m_UBODirectionalLightBuffer,
                                         sizeof(DirectionalLight::Aligned) * MAX_ALLOWED_DIRECTIONAL_LIGHT_COUNT,
                                         m_UBOCountersBuffer, sizeof(UBOcounters)
    );

    m_Vk.createSimpleMeshDescriptorSets(m_UBOmvpBuffer, sizeof(UBOmvp<MAX_ALLOWED_MESH_COUNT>),
                                        m_UBOPointLightBuffer,
                                        sizeof(PointLight::Aligned) * MAX_ALLOWED_POINT_LIGHT_COUNT,
                                        m_UBODirectionalLightBuffer,
                                        sizeof(DirectionalLight::Aligned) * MAX_ALLOWED_DIRECTIONAL_LIGHT_COUNT,
                                        m_UBOCountersBuffer, sizeof(UBOcounters));

    addPointLight();
}

void Scene::run()
{
    updateScene();

    render();

    handleEvents();
}

void Scene::render()
{
    waitForFences();

    const auto &currentFrame = m_Vk.m_CurrentFrame;

    std::thread meshCommand(&Scene::recordMeshCommands, this);

    renderMenus();

    m_ImGui.recordCommandBuffer(currentFrame);

    meshCommand.join();

    m_Vk.renderFrame(m_ImGui.getCommandBuffer(currentFrame), m_MeshCommandBuffers[currentFrame]);
}

void Scene::waitForFences()
{
    while (m_Vk.m_Device.waitForFences(*m_Vk.m_InFlightFences[m_Vk.m_CurrentFrame], vk::True, UINT64_MAX) ==
           vk::Result::eTimeout);
}

void Scene::handleSwapChainRecreation()
{
    int width = 0, height = 0;
    glfwGetFramebufferSize(m_Window.getWindow(), &width, &height);
    while (width == 0 || height == 0)
    {
        glfwGetFramebufferSize(m_Window.getWindow(), &width, &height);
        glfwWaitEvents();
    }

    m_Vk.waitIdle();

    ImGui_ImplVulkan_Shutdown();

    ImGui_ImplGlfw_Shutdown();

    m_Vk.cleanupSwapChain();

    m_Vk.rebuildSwapChain();

    ImGui_ImplGlfw_InitForVulkan(m_Window.getWindow(), true);

    m_ImGui.updateSwapChain(m_Vk);
}

void Scene::createCommandBuffers()
{
    m_Vk.CreateCommandBuffers(m_MeshCommandBuffers, vk::CommandBufferLevel::eSecondary);
}

void Scene::createUniformBuffers()
{
    m_Vk.CreateUniformBuffers(m_UBOmvpBuffer, sizeof(m_UBOmvp));

    m_Vk.CreateUniformBuffers(m_UBOPointLightBuffer, sizeof(PointLight::Aligned) * MAX_ALLOWED_POINT_LIGHT_COUNT);

    m_Vk.CreateUniformBuffers(m_UBODirectionalLightBuffer,
                              sizeof(DirectionalLight::Aligned) * MAX_ALLOWED_DIRECTIONAL_LIGHT_COUNT);

    m_Vk.CreateUniformBuffers(m_UBOCountersBuffer, sizeof(m_Counters));
}

void Scene::initUBOmvp()
{
    for (uint32_t i = 0; i < MAX_ALLOWED_MESH_COUNT; ++i)
        m_UBOmvp.model[i] = glm::mat4(1.f);

    m_UBOmvp.view = m_Camera.m_View;

    m_UBOmvp.proj = m_Camera.m_Projection;

    m_UBOmvp.proj[1][1] = -m_UBOmvp.proj[1][1];

    m_UBOmvp.viewPos = m_Camera.getPosition();
}

void Scene::initUBOPointLight()
{
    for (int i = 0; i < MAX_ALLOWED_POINT_LIGHT_COUNT; ++i)
    {
        m_PointLightComponents[i].bind();
        m_UBOpointLight[i] = m_PointLightComponents[i].m_Light.getAligned();
    }
}

void Scene::initUBODirectionalLight()
{
    for (int i = 0; i < MAX_ALLOWED_DIRECTIONAL_LIGHT_COUNT; ++i)
    {
        m_DirectionalLightComponents[i].bind();
        m_UBOdirectionalLight[i] = m_DirectionalLightComponents[i].m_Light.getAligned();
    }
}

void Scene::initUBOCounters()
{
    m_Counters.directionalLightCount = static_cast<int>(m_ActiveDirectionalLights);

    m_Counters.pointLightCount = static_cast<int>(m_ActivePointLights);
}

void Scene::initUBO()
{
    initUBOmvp();

    initUBODirectionalLight();

    initUBOPointLight();

    initUBOCounters();
}

void Scene::addMesh(const std::string &path)
{
    auto &meshComponent = m_MeshComponents.emplace_back();

    meshComponent.m_Mesh.setID(m_MeshComponents.size() - 1);

    meshComponent.m_Mesh.load(path);

    auto &mesh = meshComponent.m_Mesh;

    m_Vk.CreateVertexBuffer(mesh.getVertices(), mesh.m_VertexBuffer, mesh.m_VertexBufferMemory);

    m_Vk.CreateIndexBuffer(mesh.getIndices(), mesh.m_IndexBuffer, mesh.m_IndexBufferMemory);

    meshComponent.bind();

    meshComponent.m_Menu.setTitle("Mesh" + std::to_string(meshComponent.m_Mesh.getID()));

    m_UBOmvp.model[mesh.getID()] = mesh.getModel();
}

void Scene::addSphereMesh()
{
    Mesh sphere = Mesh::generateSphere({}, 1.f, 64, 64);

    auto &meshComponent = m_MeshComponents.emplace_back();

    meshComponent.m_Mesh = std::move(sphere);

    meshComponent.m_Mesh.setID(m_MeshComponents.size() - 1);

    auto &mesh = meshComponent.m_Mesh;

    m_Vk.CreateVertexBuffer(mesh.getVertices(), mesh.m_VertexBuffer, mesh.m_VertexBufferMemory);

    m_Vk.CreateIndexBuffer(mesh.getIndices(), mesh.m_IndexBuffer, mesh.m_IndexBufferMemory);

    meshComponent.bind();

    meshComponent.m_Menu.setTitle("Mesh" + std::to_string(meshComponent.m_Mesh.getID()));

    m_UBOmvp.model[mesh.getID()] = mesh.getModel();
}

void Scene::addPointLight()
{
    if (m_ActivePointLights == MAX_ALLOWED_POINT_LIGHT_COUNT)
        return;

    auto &component = m_PointLightComponents[m_ActivePointLights];

    component.m_Light.setID(m_ActivePointLights);

    component.m_Menu.setTitle("PointLight" + std::to_string(component.m_Light.getID()));

    component.bind();

    m_UBOpointLight[m_ActivePointLights] = component.m_Light.getAligned();

    ++m_ActivePointLights;

    updateUBOcounters();
}

void Scene::addDirectionalLight()
{
    if (m_ActiveDirectionalLights == MAX_ALLOWED_DIRECTIONAL_LIGHT_COUNT)
        return;

    auto &component = m_DirectionalLightComponents[m_ActiveDirectionalLights];

    component.m_Light.setID(m_ActiveDirectionalLights);

    component.m_Menu.setTitle("DirectionalLight" + std::to_string(component.m_Light.getID()));

    component.bind();

    m_UBOdirectionalLight[m_ActiveDirectionalLights] = component.m_Light.getAligned();

    ++m_ActiveDirectionalLights;

    updateUBOcounters();
}

void Scene::recordMeshCommands()
{
    const auto &currentFrame = m_Vk.m_CurrentFrame;
    const auto &commandBuffer = m_MeshCommandBuffers[currentFrame];

    commandBuffer.reset();

    vk::CommandBufferInheritanceRenderingInfoKHR inheritanceRenderingInfo{};

    inheritanceRenderingInfo.colorAttachmentCount = 1;
    inheritanceRenderingInfo.pColorAttachmentFormats = &m_ColorAttachmentFormat;
    inheritanceRenderingInfo.depthAttachmentFormat = m_DepthAttachmentFormat;
    inheritanceRenderingInfo.flags = vk::RenderingFlagBitsKHR::eContentsSecondaryCommandBuffers;
    inheritanceRenderingInfo.rasterizationSamples = m_Vk.m_MsaaSamples;

    vk::CommandBufferInheritanceInfo inheritanceInfo;
    inheritanceInfo.pNext = &inheritanceRenderingInfo;
    inheritanceInfo.renderPass = VK_NULL_HANDLE;

    vk::CommandBufferBeginInfo beginInfo(
        vk::CommandBufferUsageFlagBits::eRenderPassContinue | vk::CommandBufferUsageFlagBits::eOneTimeSubmit,
        &inheritanceInfo
    );

    commandBuffer.begin(beginInfo);

    commandBuffer.setViewport(0, m_Vk.m_Viewport);

    commandBuffer.setScissor(0, m_Vk.m_Scissor);

    // commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, m_Vk.m_ComplexMeshGraphicsPipeline);
    //
    // commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
    //                                  m_Vk.m_ComplexMeshPipelineLayout,
    //                                  0,
    //                                  *m_Vk.m_ComplexMeshDescriptorSets[currentFrame],
    //                                  nullptr
    // );

    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, m_Vk.m_SimpleMeshGraphicsPipeline);
    commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
                                     m_Vk.m_SimpleMeshPipelineLayout,
                                     0,
                                     *m_Vk.m_SimpleMeshDescriptorSets[currentFrame],
                                     nullptr
    );

    for (const auto &meshComponent: m_MeshComponents)
    {
        const auto &mesh = meshComponent.m_Mesh;

        auto color = glm::vec4(mesh.getColor(), 1.0);

        vk::ArrayProxy<const glm::vec4> arr = color;

        vk::DeviceSize offset = 0;
        commandBuffer.bindVertexBuffers(0, *mesh.m_VertexBuffer, offset);

        commandBuffer.bindIndexBuffer(mesh.m_IndexBuffer, 0, vk::IndexType::eUint32);

        commandBuffer.pushConstants(*m_Vk.m_SimpleMeshPipelineLayout, vk::ShaderStageFlagBits::eFragment, 0, arr);

        commandBuffer.drawIndexed(
            static_cast<uint32_t>(mesh.getIndices().size()),
            mesh.getInstances(),
            0, 0, 0);
    }

    commandBuffer.end();
}

void Scene::renderMenus()
{
    m_ImGui.beginFrame();

    for (auto &component: m_MeshComponents)
    {
        auto &menu = component.m_Menu;

        if (menu.render())
            updateUBOmvpModel(component.m_Mesh);
    }

    for (uint32_t i = 0; i < m_ActivePointLights; ++i)
    {
        auto &menu = m_PointLightComponents[i].m_Menu;

        if (menu.render())
            updateUBOpointLight(m_PointLightComponents[i].m_Light);
    }

    for (uint32_t i = 0; i < m_ActiveDirectionalLights; ++i)
    {
        auto &menu = m_DirectionalLightComponents[i].m_Menu;

        if (menu.render())
            updateUBOdirectionalLight(m_DirectionalLightComponents[i].m_Light);
    }

    m_ImGui.endFrame();
}

void Scene::updateUBOmvpModel(const Mesh &mesh)
{
    m_UBOmvp.model[mesh.getID()] = mesh.getModel();
}

void Scene::updateUBOmvpCam()
{
    m_UBOmvp.view = m_Camera.m_View;

    m_UBOmvp.viewPos = m_Camera.getPosition();
}

void Scene::updateCamera(float deltaTime)
{
    m_Camera.updateMovement(deltaTime);

    m_Camera.updateLookAt(deltaTime);

    updateUBOmvpCam();
}

void Scene::updateUBOpointLight(const PointLight &pointLight)
{
    m_UBOpointLight[pointLight.getID()] = pointLight.getAligned();
}

void Scene::updateUBOdirectionalLight(const DirectionalLight &directionalLight)
{
    m_UBOdirectionalLight[directionalLight.getID()] = directionalLight.getAligned();
}

void Scene::updateUBOcounters()
{
    m_Counters.directionalLightCount = static_cast<int>(m_ActiveDirectionalLights);

    m_Counters.pointLightCount = static_cast<int>(m_ActivePointLights);
}

void Scene::updateScene()
{
    const auto& currentFrame = m_Vk.m_CurrentFrame;

    std::thread uboUpdate(&Scene::updateUBO, this, currentFrame);

    std::thread cameraUpdate(&Scene::updateCamera, this, m_Clock.getElapsedTime<float, TimeType::Seconds>());

    m_Clock.restart();

    uboUpdate.join();
    cameraUpdate.join();
}

void Scene::updateUBO(uint32_t frame)
{
    m_UBOmvp.viewPos = m_Camera.getPosition();

    m_UBOmvpBuffer.update(m_UBOmvp, frame);

    m_UBOCountersBuffer.update(m_Counters, frame);

    m_UBOPointLightBuffer.update(m_UBOpointLight.data(), m_ActivePointLights, frame);

    m_UBODirectionalLightBuffer.update(m_UBOdirectionalLight.data(), m_ActiveDirectionalLights, frame);
}

void Scene::handleEvents()
{
    m_Window.pollEvents();

    if (m_Vk.m_FramebufferResized)
    {
        handleSwapChainRecreation();
        m_Camera.updateProjection();
    }
}
