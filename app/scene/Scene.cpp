#include "Scene.h"

#include <future>
#include <iostream>
#include <thread>
#include <glm/gtc/type_ptr.inl>

#include "../objects/menus/MeshMenu.h"
#include <print>
#include <ranges>

namespace vks_engine
{
    Scene::Scene(Window &window, VulkanHandler &vkHandler): m_Vk(vkHandler),
                                                            m_Window(window),
                                                            m_Camera(window.getWindow(), glm::vec3(0.f, 0.f, 2.f), 5.f),
                                                            m_ActiveDirectionalLights(0), m_ActivePointLights(0),
                                                            m_CurrentComplexMeshCount(0), m_CurrentSimpleMeshCount(0)
    {
    }

    void Scene::init()
    {
        setInputHandlers();

        initFileExplorer();

        initMenus();

        m_ImGui.init(m_Vk, m_Window.getWindow());

        m_ColorAttachmentFormat = m_ImGui.m_ColorAttachmentFormat;

        m_DepthAttachmentFormat = m_ImGui.m_DepthAttachmentFormat;

        initUBO();

        createCommandBuffers();

        createUniformBuffers();

        /*m_Vk.createComplexMeshDescriptorSets(m_UBOvpBuffer, sizeof(UBOvp),
                                             m_UBOPointLightBuffer,
                                             sizeof(PointLight::Aligned) * SCENE_MAX_ALLOWED_POINT_LIGHT_COUNT,
                                             m_UBODirectionalLightBuffer,
                                             sizeof(DirectionalLight::Aligned) * SCENE_MAX_ALLOWED_DIRECTIONAL_LIGHT_COUNT,
                                             m_UBOCountersBuffer, sizeof(UBOcounters)
        );*/

        m_Vk.createSimpleMeshDescriptorSets(m_UBOvpBuffer, sizeof(UBOvp),
                                            m_UBOPointLightBuffer,
                                            sizeof(PointLight::Aligned) * SCENE_MAX_ALLOWED_POINT_LIGHT_COUNT,
                                            m_UBODirectionalLightBuffer,
                                            sizeof(DirectionalLight::Aligned) *
                                            SCENE_MAX_ALLOWED_DIRECTIONAL_LIGHT_COUNT,
                                            m_UBOCountersBuffer, sizeof(UBOcounters));

        m_SimpleMeshComponents.reserve(SCENE_MAX_ALLOWED_MESH_COUNT);
        m_ComplexMeshComponents.reserve(SCENE_MAX_ALLOWED_MESH_COUNT);
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

        recordMeshCommands(currentFrame);

        renderMenus();

        m_ImGui.recordCommandBuffer(currentFrame);

        //meshCommand.join();

        m_Vk.renderFrame(m_ImGui.getCommandBuffer(currentFrame),
                         m_SimpleMeshCommandBuffers[currentFrame],
                         m_ComplexMeshCommandBuffers[currentFrame]);
    }

    void Scene::waitForFences()
    {
        while (m_Vk.m_Device.waitForFences(*m_Vk.m_InFlightFences[m_Vk.m_CurrentFrame], vk::True, UINT64_MAX) ==
               vk::Result::eTimeout);
    }

    void Scene::handleSwapChainRecreation(int width, int height)
    {
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
        m_Vk.CreateCommandBuffers(m_SimpleMeshCommandBuffers, vk::CommandBufferLevel::eSecondary);

        m_Vk.CreateCommandBuffers(m_ComplexMeshCommandBuffers, vk::CommandBufferLevel::eSecondary);
    }

    void Scene::createUniformBuffers()
    {
        m_Vk.CreateUniformBuffers(m_UBOvpBuffer, sizeof(m_UBOvp));

        m_Vk.CreateUniformBuffers(m_UBOPointLightBuffer,
                                  sizeof(PointLight::Aligned) * SCENE_MAX_ALLOWED_POINT_LIGHT_COUNT);

        m_Vk.CreateUniformBuffers(m_UBODirectionalLightBuffer,
                                  sizeof(DirectionalLight::Aligned) * SCENE_MAX_ALLOWED_DIRECTIONAL_LIGHT_COUNT);

        m_Vk.CreateUniformBuffers(m_UBOCountersBuffer, sizeof(m_Counters));
    }

    void Scene::initUBOmvp()
    {
        m_UBOvp.view = m_Camera.m_View;

        m_UBOvp.proj = m_Camera.m_Projection;

        m_UBOvp.proj[1][1] = -m_UBOvp.proj[1][1];

        m_UBOvp.viewPos = m_Camera.getPosition();
    }

    void Scene::initUBOPointLight()
    {
        for (int i = 0; i < SCENE_MAX_ALLOWED_POINT_LIGHT_COUNT; ++i)
        {
            m_PointLightComponents[i].bind();
            m_UBOpointLight[i] = m_PointLightComponents[i].m_Light.getAligned();
        }
    }

    void Scene::initUBODirectionalLight()
    {
        for (int i = 0; i < SCENE_MAX_ALLOWED_DIRECTIONAL_LIGHT_COUNT; ++i)
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

    void Scene::loadModelWorker(std::string_view path)
    {
        MeshComponent component;

        uint32_t newID; {
            std::lock_guard<std::mutex> lock(m_ComplexMeshCountMutex);
            if (getTotalMeshCount() == SCENE_MAX_ALLOWED_MESH_COUNT)
                return;

            newID = m_CurrentComplexMeshCount;
        }

        auto &mesh = component.m_Mesh;

        mesh.setID(newID);

        mesh.load(path.data());

        mesh.loadTextures();

        mesh.setType(MeshType::MODEL);

        component.bind();

        component.m_Menu.setTitle("Mesh" + std::to_string(getTotalMeshCount())); {
            std::lock_guard<std::mutex> lock(m_LoadedMeshMutex);
            m_LoadedMeshQueue.push_back(std::move(component));
        }

        ++m_CurrentComplexMeshCount;
    }

    void Scene::addSphereMesh()
    {
        if (getTotalMeshCount() == SCENE_MAX_ALLOWED_MESH_COUNT)
            return;

        Mesh sphere = Mesh::generateSphere({}, 1.f, 64, 64);

        auto &component = m_SimpleMeshComponents.emplace_back();

        component.m_Mesh = std::move(sphere);

        component.m_Mesh.setID(m_CurrentSimpleMeshCount);

        auto &mesh = component.m_Mesh;

        m_Vk.CreateVertexBuffer(mesh.getVertices(), mesh.m_VertexBuffer, mesh.m_VertexBufferMemory);

        m_Vk.CreateIndexBuffer(mesh.getIndices(), mesh.m_IndexBuffer, mesh.m_IndexBufferMemory);

        component.bind();

        component.m_Menu.setTitle("Mesh" + std::to_string(getTotalMeshCount()));

        ++m_CurrentSimpleMeshCount;

        m_SceneComponentsMenu.addComponent(ComponentType::MESH, component);
    }

    void Scene::addCubeMesh()
    {
        if (getTotalMeshCount() == SCENE_MAX_ALLOWED_MESH_COUNT)
            return;

        Mesh cube = Mesh::generateCube({}, 1.f);

        auto &component = m_SimpleMeshComponents.emplace_back();

        component.m_Mesh = std::move(cube);

        component.m_Mesh.setID(m_CurrentSimpleMeshCount);

        auto &mesh = component.m_Mesh;

        m_Vk.CreateVertexBuffer(mesh.getVertices(), mesh.m_VertexBuffer, mesh.m_VertexBufferMemory);

        m_Vk.CreateIndexBuffer(mesh.getIndices(), mesh.m_IndexBuffer, mesh.m_IndexBufferMemory);

        component.bind();

        component.m_Menu.setTitle("Mesh" + std::to_string(getTotalMeshCount()));

        ++m_CurrentSimpleMeshCount;

        m_SceneComponentsMenu.addComponent(ComponentType::MESH, component);
    }

    void Scene::addMesh(MeshType type)
    {
        switch (type)
        {
            case MeshType::CUBE:
                addCubeMesh();
                break;

            case MeshType::SPHERE:
                addSphereMesh();
                break;

            case MeshType::MODEL:
                m_FileExplorer.open();
                break;
        }
    }

    void Scene::handleFileSelected(const std::string &path)
    {
        std::lock_guard<std::mutex> lock(m_PendingModelsMutex);

        m_PendingModelPaths.push_back(path);
    }

    void Scene::handleLoadedModel(MeshComponent &component)
    {
        m_ComplexMeshComponents.push_back(std::move(component));

        auto &addedComponent = m_ComplexMeshComponents.back();

        addedComponent.bind();

        auto &mesh = addedComponent.m_Mesh;

        m_Vk.CreateVertexBuffer(mesh.getVertices(), mesh.m_VertexBuffer, mesh.m_VertexBufferMemory);

        m_Vk.CreateIndexBuffer(mesh.getIndices(), mesh.m_IndexBuffer, mesh.m_IndexBufferMemory);

        for (auto &texture: mesh.m_Textures | std::views::values)
            m_Vk.CreateTexture(texture);

        m_Vk.CreateMeshDescriptorSets(mesh,
                                      m_UBOvpBuffer, sizeof(UBOvp),
                                      m_UBOPointLightBuffer,
                                      sizeof(PointLight::Aligned) * SCENE_MAX_ALLOWED_POINT_LIGHT_COUNT,
                                      m_UBODirectionalLightBuffer,
                                      sizeof(DirectionalLight::Aligned) * SCENE_MAX_ALLOWED_DIRECTIONAL_LIGHT_COUNT,
                                      m_UBOCountersBuffer, sizeof(UBOcounters)
        );
    }

    void Scene::procesPendingActions()
    {
        std::lock_guard<std::mutex> lock(m_PendingModelsMutex);

        while (!m_PendingModelPaths.empty())
        {
            auto path = m_PendingModelPaths.front();

            m_PendingModelPaths.pop_front();

            std::thread(&Scene::loadModelWorker, this, path).detach();
        }
    }

    void Scene::processLoadedModels()
    {
        std::lock_guard<std::mutex> lock(m_LoadedMeshMutex);

        while (!m_LoadedMeshQueue.empty())
        {
            MeshComponent &componentToProcess = m_LoadedMeshQueue.front();

            handleLoadedModel(componentToProcess);

            m_SceneComponentsMenu.addComponent(ComponentType::MESH, m_ComplexMeshComponents.back());

            m_LoadedMeshQueue.pop_front();
        }
    }

    void Scene::initFileExplorer()
    {
        m_FileExplorer.onFileSelected([this](const auto &path) { this->handleFileSelected(path); });
    }

    void Scene::addPointLight()
    {
        if (m_ActivePointLights == SCENE_MAX_ALLOWED_POINT_LIGHT_COUNT)
            return;

        auto &component = m_PointLightComponents[m_ActivePointLights];

        component.m_Light.setID(m_ActivePointLights);

        component.m_Menu.setTitle("PointLight" + std::to_string(component.m_Light.getID()));

        component.bind();

        m_UBOpointLight[m_ActivePointLights] = component.m_Light.getAligned();

        ++m_ActivePointLights;

        updateUBOcounters();

        m_SceneComponentsMenu.addComponent(ComponentType::POINT_LIGHT, component);
    }

    void Scene::addDirectionalLight()
    {
        if (m_ActiveDirectionalLights == SCENE_MAX_ALLOWED_DIRECTIONAL_LIGHT_COUNT)
            return;

        auto &component = m_DirectionalLightComponents[m_ActiveDirectionalLights];

        component.m_Light.setID(m_ActiveDirectionalLights);

        component.m_Menu.setTitle("DirectionalLight" + std::to_string(component.m_Light.getID()));

        component.bind();

        m_UBOdirectionalLight[m_ActiveDirectionalLights] = component.m_Light.getAligned();

        ++m_ActiveDirectionalLights;

        updateUBOcounters();

        m_SceneComponentsMenu.addComponent(ComponentType::DIRECTIONAL_LIGHT, component);
    }

    void Scene::recordMeshCommands(uint32_t currentFrame)
    {
        /*std::thread simpleMeshThread(&Scene::recordSimpleMeshCommands, this, currentFrame);

        std::thread complexMeshThread(&Scene::recordComplexMeshCommands, this, currentFrame);

        simpleMeshThread.join();
        complexMeshThread.join();*/

        recordSimpleMeshCommands(currentFrame);

        recordComplexMeshCommands(currentFrame);
    }

    void Scene::recordSimpleMeshCommands(uint32_t currentFrame)
    {
        const auto &commandBuffer = m_SimpleMeshCommandBuffers[currentFrame];

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

        commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, m_Vk.m_SimpleMeshGraphicsPipeline);

        commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
                                         m_Vk.m_SimpleMeshPipelineLayout,
                                         0,
                                         *m_Vk.m_SimpleMeshDescriptorSets[currentFrame],
                                         nullptr
        );

        for (const auto &meshComponent: m_SimpleMeshComponents)
        {
            const auto &mesh = meshComponent.m_Mesh;

            const auto &model = mesh.getModel();

            auto color = glm::vec4(mesh.getColor(), 1.0);

            vk::ArrayProxy<const glm::mat4> modelProxy = model;

            vk::ArrayProxy<const glm::vec4> colorProxy = color;

            vk::DeviceSize offset = 0;
            commandBuffer.bindVertexBuffers(0, *mesh.m_VertexBuffer, offset);

            commandBuffer.bindIndexBuffer(mesh.m_IndexBuffer, 0, vk::IndexType::eUint32);

            commandBuffer.pushConstants(*m_Vk.m_SimpleMeshPipelineLayout, vk::ShaderStageFlagBits::eVertex, 0,
                                        modelProxy);

            commandBuffer.pushConstants(*m_Vk.m_SimpleMeshPipelineLayout, vk::ShaderStageFlagBits::eFragment,
                                        sizeof(glm::mat4), colorProxy);

            commandBuffer.drawIndexed(
                static_cast<uint32_t>(mesh.getIndices().size()),
                mesh.getInstances(),
                0, 0, 0);
        }

        commandBuffer.end();
    }

    void Scene::recordComplexMeshCommands(uint32_t currentFrame)
    {
        const auto &commandBuffer = m_ComplexMeshCommandBuffers[currentFrame];

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

        commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, m_Vk.m_ComplexMeshGraphicsPipeline);

        for (const auto &meshComponent: m_ComplexMeshComponents)
        {
            const auto &mesh = meshComponent.m_Mesh;

            commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
                                             m_Vk.m_ComplexMeshPipelineLayout,
                                             0,
                                             *mesh.m_DescriptorSets[currentFrame],
                                             nullptr
            );

            const auto &model = mesh.getModel();

            auto color = glm::vec4(mesh.getColor(), 1.0);

            vk::ArrayProxy<const glm::mat4> modelProxy = model;

            vk::ArrayProxy<const glm::vec4> colorProxy = color;

            vk::DeviceSize offset = 0;
            commandBuffer.bindVertexBuffers(0, *mesh.m_VertexBuffer, offset);

            commandBuffer.bindIndexBuffer(mesh.m_IndexBuffer, 0, vk::IndexType::eUint32);

            commandBuffer.pushConstants(*m_Vk.m_ComplexMeshPipelineLayout, vk::ShaderStageFlagBits::eVertex, 0,
                                        modelProxy);

            commandBuffer.pushConstants(*m_Vk.m_ComplexMeshPipelineLayout, vk::ShaderStageFlagBits::eFragment,
                                        sizeof(glm::mat4), colorProxy);

            commandBuffer.drawIndexed(
                static_cast<uint32_t>(mesh.getIndices().size()),
                mesh.getInstances(),
                0, 0, 0);
        }

        commandBuffer.end();
    }

    void Scene::updateUBOvpCam()
    {
        m_UBOvp.view = m_Camera.m_View;

        m_UBOvp.viewPos = m_Camera.getPosition();
    }

    void Scene::updateCamera(float deltaTime)
    {
        m_Camera.updateMovement(deltaTime);

        m_Camera.updateLookAt(deltaTime);

        updateUBOvpCam();
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

    void Scene::initMenus()
    {
        initSceneFunctionsMenu();

        initSceneComponentsMenu();

        initSceneComponentPropertiesMenu();
    }

    void Scene::initSceneFunctionsMenu()
    {
        m_SceneFunctionsMenu.setTitle("Functions");

        m_SceneFunctionsMenu.onAddPointLightBtnClick([this]() { this->addPointLight(); });

        m_SceneFunctionsMenu.onAddDirectionalLightBtnClick([this]() { this->addDirectionalLight(); });

        m_SceneFunctionsMenu.onMeshTypeSelection([this](MeshType type) { this->addMesh(type); });
    }

    void Scene::initSceneComponentsMenu()
    {
        m_SceneComponentsMenu.setTitle("Scene Components");

        m_SceneComponentsMenu.setOnComponentSelected([this](ComponentEntry &entry)
        {
            this->m_SceneComponentPropertiesMenu.setActiveComponent(entry);
        });

        m_SceneComponentsMenu.setOnComponentRename([this](ComponentEntry &entry, std::string_view newName)
        {
            this->handleComponentRename(entry, newName);
        });

        m_SceneComponentsMenu.setOnComponentDelete([this](ComponentEntry &entry)
        {
            this->m_SceneComponentPropertiesMenu.handleComponentDelete(entry);
            this->handleComponentDelete(entry);
        });
    }

    void Scene::initSceneComponentPropertiesMenu()
    {
        m_SceneComponentPropertiesMenu.setTitle("Component Properties");

        m_SceneComponentPropertiesMenu.setOnPointLightPropertiesChange([this](const PointLight &pl)
        {
            this->updateUBOpointLight(pl);
        });

        m_SceneComponentPropertiesMenu.setOnDirectionalLightPropertiesChange([this](const DirectionalLight &dl)
        {
            this->updateUBOdirectionalLight(dl);
        });
    }

    void Scene::renderMenus()
    {
        m_ImGui.beginFrame();

        m_SceneFunctionsMenu.render();

        m_SceneComponentsMenu.render();

        m_SceneComponentPropertiesMenu.render();

        m_ImGui.endFrame();
    }

    void Scene::handleComponentRename(ComponentEntry &entry, std::string_view newName)
    {
        auto &meshComponent = *static_cast<MeshComponent *>(entry.p_Component);

        auto &plComponent = *static_cast<PointLightComponent *>(entry.p_Component);

        auto &dlComponent = *static_cast<DirectionalLightComponent *>(entry.p_Component);

        switch (entry.m_Type)
        {
            case ComponentType::MESH:
                renameMesh(meshComponent.m_Mesh, newName);
                break;
            case ComponentType::POINT_LIGHT:
                renamePointLight(plComponent.m_Light, newName);
                break;
            case ComponentType::DIRECTIONAL_LIGHT:
                renameDirectionalLight(dlComponent.m_Light, newName);
                break;
        }
    }

    void Scene::renameMesh(const Mesh &mesh, std::string_view newName)
    {
        if (mesh.getType() == MeshType::MODEL)
            m_ComplexMeshComponents[mesh.getID()].m_Menu.setTitle(newName.data());
        else
            m_SimpleMeshComponents[mesh.getID()].m_Menu.setTitle(newName.data());
    }

    void Scene::renamePointLight(const PointLight &pl, std::string_view newName)
    {
        m_PointLightComponents[pl.getID()].m_Menu.setTitle(newName.data());
    }

    void Scene::renameDirectionalLight(const DirectionalLight &dl, std::string_view newName)
    {
        m_DirectionalLightComponents[dl.getID()].m_Menu.setTitle(newName.data());
    }

    void Scene::handleComponentDelete(ComponentEntry &entry)
    {
        auto &meshComponent = *static_cast<MeshComponent *>(entry.p_Component);

        auto &plComponent = *static_cast<PointLightComponent *>(entry.p_Component);

        auto &dlComponent = *static_cast<DirectionalLightComponent *>(entry.p_Component);


        switch (entry.m_Type)
        {
            case ComponentType::MESH:
                deleteMesh(meshComponent.m_Mesh);
                break;
            case ComponentType::POINT_LIGHT:
                deletePointLight(plComponent.m_Light);
                break;
            case ComponentType::DIRECTIONAL_LIGHT:
                deleteDirectionalLight(dlComponent.m_Light);
                break;
        }
    }

    void Scene::deleteMesh(const Mesh &mesh)
    {
        m_Vk.waitIdle();

        if (mesh.getType() == MeshType::MODEL)
        {
            auto erase_it = m_ComplexMeshComponents.begin() + mesh.getID();

            for (auto it = erase_it + 1; it != m_ComplexMeshComponents.end(); ++it)
                it->m_Mesh.setID(it->m_Mesh.getID() - 1);

            for (auto &commandBuffer: m_ComplexMeshCommandBuffers)
                commandBuffer.reset();

            m_ComplexMeshComponents.erase(erase_it);
            --m_CurrentComplexMeshCount;
        } else
        {
            auto erase_it = m_SimpleMeshComponents.begin() + mesh.getID();

            for (auto it = erase_it + 1; it != m_SimpleMeshComponents.end(); ++it)
                it->m_Mesh.setID(it->m_Mesh.getID() - 1);

            for (auto &commandBuffer: m_SimpleMeshCommandBuffers)
                commandBuffer.reset();

            m_SimpleMeshComponents.erase(erase_it);
            --m_CurrentSimpleMeshCount;
        }
    }

    void Scene::deletePointLight(const PointLight &pl)
    {
        std::move(m_UBOpointLight.begin() + pl.getID() + 1, m_UBOpointLight.end(), m_UBOpointLight.begin() + pl.getID());

        std::move(m_PointLightComponents.begin() + pl.getID() + 1, m_PointLightComponents.end(), m_PointLightComponents.begin() + pl.getID());

        --m_ActivePointLights;

        updateUBOcounters();
    }

    void Scene::deleteDirectionalLight(const DirectionalLight &dl)
    {
        std::move(m_UBOdirectionalLight.begin() + dl.getID() + 1, m_UBOdirectionalLight.end(), m_UBOdirectionalLight.begin() + dl.getID());

        std::move(m_DirectionalLightComponents.begin() + dl.getID() + 1, m_DirectionalLightComponents.end(), m_DirectionalLightComponents.begin() + dl.getID());

        --m_ActiveDirectionalLights;

        updateUBOcounters();
    }

    void Scene::updateScene()
    {
        procesPendingActions();

        processLoadedModels();

        const auto &currentFrame = m_Vk.m_CurrentFrame;

        auto uboFuture = std::async(std::launch::async, &Scene::updateUBO, this, currentFrame);

        auto cameraFuture = std::async(std::launch::async, &Scene::updateCamera, this,
                                       m_Clock.getElapsedTime<float, TimeType::Seconds>());

        m_Clock.restart();

        uboFuture.wait();

        cameraFuture.wait();
    }

    void Scene::updateUBO(uint32_t frame)
    {
        m_UBOvpBuffer.update(m_UBOvp, frame);

        m_UBOCountersBuffer.update(m_Counters, frame);

        m_UBOPointLightBuffer.update(m_UBOpointLight.data(), m_ActivePointLights, frame);

        m_UBODirectionalLightBuffer.update(m_UBOdirectionalLight.data(), m_ActiveDirectionalLights, frame);
    }

    void Scene::setInputHandlers()
    {
        setupKeyHandlers();

        setupButtonHandlers();
    }

    void Scene::setupKeyHandlers()
    {
        m_Window.setKeyCallback([this](int key, int scancode, int action, int mods)
        {
            this->m_InputHandler.handleKeyboard(key, scancode, action, mods);
        });

        m_InputHandler.setKeyCallbackFunction(Key(Key::KEY_ESCAPE, Key::KEY_ACTION_PRESS, Key::KEY_MOD_NONE),
                                              [this] { this->m_Window.close(); });

        m_InputHandler.setKeyCallbackFunction(Key(Key::KEY_ENTER, Key::KEY_ACTION_PRESS, Key::KEY_MOD_ALT),
                                              [this]
                                              {
                                                  if (!this->m_Window.isMaximized())
                                                      this->m_Window.maximize();
                                                  else
                                                      this->m_Window.restore();
                                              });
    }

    void Scene::setupButtonHandlers()
    {
        m_Window.setButtonCallback([this](int button, int action, int mods)
        {
            this->m_InputHandler.handleMouse(button, action, mods);
        });

        m_InputHandler.setButtonCallbackFunction(
            Button(Button::BUTTON_MIDDLE, Button::BUTTON_ACTION_PRESS, Button::BUTTON_MOD_NONE),
            [this]()
            {
                if (this->m_Camera.isFocused())
                    this->m_Camera.loseFocus();
                else
                    this->m_Camera.gainFocus();
            });
    }

    void Scene::handleFramebufferResize(int width, int height)
    {
        handleSwapChainRecreation(width, height);

        m_Camera.updateProjection(width, height);
    }

    void Scene::handleEvents()
    {
        m_Window.pollEvents();

        if (m_Window.wasResized())
        {
            int w, h;
            glfwGetFramebufferSize(m_Window.getWindow(), &w, &h);

            if (w != 0 && h != 0)
            {
                handleSwapChainRecreation(w, h);
                m_Camera.updateProjection(w, h);
            }
        }
    }
}
