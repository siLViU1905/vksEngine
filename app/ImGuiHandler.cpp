#include "ImGuiHandler.h"

#include "VulkanHandler.h"

namespace vks_engine
{
    ImGuiHandler::ImGuiHandler(): m_DescriptorPool(nullptr), m_Context(nullptr), m_CommandPool(nullptr)
    {
    }

    void ImGuiHandler::init(VulkanHandler &vkHandler, GLFWwindow *window)
    {
        createDescriptorPool(vkHandler.m_Device);

        IMGUI_CHECKVERSION();

        m_Context = ImGui::CreateContext();

        ImGuiIO &io = ImGui::GetIO();
        (void) io;

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        ImGui_ImplGlfw_InitForVulkan(window, true);

        ImGui_ImplVulkan_InitInfo initInfo{};

        initInfo.Instance = *vkHandler.m_Instance;
        initInfo.PhysicalDevice = *vkHandler.m_PhysicalDevice;
        initInfo.Device = *vkHandler.m_Device;
        initInfo.QueueFamily = vkHandler.m_GraphicsQueueFamilyIndex;
        initInfo.Queue = *vkHandler.m_GraphicsQueue;
        initInfo.DescriptorPool = *m_DescriptorPool;
        initInfo.MinImageCount = vkHandler.m_SwapChainImages.size();
        initInfo.ImageCount = vkHandler.m_SwapChainImages.size();
        initInfo.MSAASamples = static_cast<VkSampleCountFlagBits>(vkHandler.m_MsaaSamples);
        initInfo.Allocator = nullptr;
        initInfo.CheckVkResultFn = nullptr;

        initInfo.UseDynamicRendering = true;
        initInfo.RenderPass = VK_NULL_HANDLE;

        initInfo.PipelineRenderingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR;
        initInfo.PipelineRenderingCreateInfo.colorAttachmentCount = 1;
        initInfo.PipelineRenderingCreateInfo.pColorAttachmentFormats = reinterpret_cast<const VkFormat *>(&vkHandler.
            m_SwapChainImageFormat);

        vk::Format depthFormat = vkHandler.findDepthFormat();

        const auto *pDepthFormat = reinterpret_cast<const VkFormat *>(&depthFormat);

        initInfo.PipelineRenderingCreateInfo.depthAttachmentFormat = *pDepthFormat;

        if (!ImGui_ImplVulkan_Init(&initInfo))
            throw std::runtime_error("Failed to initialize ImGui for Vulkan");

        m_ColorAttachmentFormat = vkHandler.m_SwapChainImageFormat;
        m_DepthAttachmentFormat = depthFormat;
        m_MsaaSamples = vkHandler.m_MsaaSamples;

        vk::CommandPoolCreateInfo poolInfo(
            vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
            vkHandler.m_GraphicsQueueFamilyIndex
        );

        m_CommandPool = vk::raii::CommandPool(vkHandler.m_Device, poolInfo);

        vk::CommandBufferAllocateInfo allocInfo(
            *m_CommandPool,
            vk::CommandBufferLevel::eSecondary,
            VulkanHandler::MAX_FRAMES_IN_FLIGHT
        );
        m_CommandBuffers = vk::raii::CommandBuffers(vkHandler.m_Device, allocInfo);

        applyTheme();
    }

    void ImGuiHandler::beginFrame()
    {
        ImGui_ImplVulkan_NewFrame();

        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();
    }

    void ImGuiHandler::endFrame()
    {
        ImGui::Render();
    }

    void ImGuiHandler::renderDockspace()
    {
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

        const ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", nullptr, window_flags);
        ImGui::PopStyleVar();
        ImGui::PopStyleVar(2);

        ImGuiIO &io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
        ImGui::End();
    }

    void ImGuiHandler::recordCommandBuffer(uint32_t currentFrame)
    {
        m_CommandBuffers[currentFrame].reset();

        vk::CommandBufferInheritanceRenderingInfoKHR inheritanceRenderingInfo{};

        inheritanceRenderingInfo.colorAttachmentCount = 1;
        inheritanceRenderingInfo.pColorAttachmentFormats = &m_ColorAttachmentFormat;
        inheritanceRenderingInfo.depthAttachmentFormat = m_DepthAttachmentFormat;
        inheritanceRenderingInfo.rasterizationSamples = m_MsaaSamples;

        vk::CommandBufferInheritanceInfo inheritanceInfo;

        inheritanceInfo.pNext = &inheritanceRenderingInfo;
        inheritanceInfo.renderPass = VK_NULL_HANDLE;

        vk::CommandBufferBeginInfo beginInfo(
            vk::CommandBufferUsageFlagBits::eRenderPassContinue | vk::CommandBufferUsageFlagBits::eOneTimeSubmit,
            &inheritanceInfo
        );

        m_CommandBuffers[currentFrame].begin(beginInfo);

        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), *m_CommandBuffers[currentFrame]);

        m_CommandBuffers[currentFrame].end();
    }

    void ImGuiHandler::updateSwapChain(VulkanHandler &vkHandler)
    {
        ImGui_ImplVulkan_InitInfo initInfo{};

        initInfo.Instance = *vkHandler.m_Instance;
        initInfo.PhysicalDevice = *vkHandler.m_PhysicalDevice;
        initInfo.Device = *vkHandler.m_Device;
        initInfo.QueueFamily = vkHandler.m_GraphicsQueueFamilyIndex;
        initInfo.Queue = *vkHandler.m_GraphicsQueue;
        initInfo.DescriptorPool = *m_DescriptorPool;
        initInfo.MinImageCount = vkHandler.m_SwapChainImages.size();
        initInfo.ImageCount = vkHandler.m_SwapChainImages.size();
        initInfo.MSAASamples = static_cast<VkSampleCountFlagBits>(vkHandler.m_MsaaSamples);
        initInfo.Allocator = nullptr;
        initInfo.CheckVkResultFn = nullptr;

        initInfo.UseDynamicRendering = true;
        initInfo.RenderPass = VK_NULL_HANDLE;

        initInfo.PipelineRenderingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR;
        initInfo.PipelineRenderingCreateInfo.colorAttachmentCount = 1;
        initInfo.PipelineRenderingCreateInfo.pColorAttachmentFormats = reinterpret_cast<const VkFormat *>(&vkHandler.
            m_SwapChainImageFormat);

        vk::Format depthFormat = vkHandler.findDepthFormat();

        const auto *pDepthFormat = reinterpret_cast<const VkFormat *>(&depthFormat);

        initInfo.PipelineRenderingCreateInfo.depthAttachmentFormat = *pDepthFormat;

        if (!ImGui_ImplVulkan_Init(&initInfo))
            throw std::runtime_error("Failed to update ImGui swapchian");


        m_ColorAttachmentFormat = vkHandler.m_SwapChainImageFormat;
        m_DepthAttachmentFormat = depthFormat;
        m_MsaaSamples = vkHandler.m_MsaaSamples;

        ImGui::NewFrame();
        ImGui::EndFrame();
    }

    ImGuiHandler::~ImGuiHandler()
    {
        ImGui_ImplVulkan_Shutdown();

        ImGui_ImplGlfw_Shutdown();

        ImGui::DestroyContext(m_Context);
    }

    void ImGuiHandler::createDescriptorPool(const vk::raii::Device &device)
    {
        std::array<vk::DescriptorPoolSize, 11> poolSizes = {
            vk::DescriptorPoolSize(vk::DescriptorType::eSampler, 1000),
            vk::DescriptorPoolSize(vk::DescriptorType::eCombinedImageSampler, 1000),
            vk::DescriptorPoolSize(vk::DescriptorType::eSampledImage, 1000),
            vk::DescriptorPoolSize(vk::DescriptorType::eStorageImage, 1000),
            vk::DescriptorPoolSize(vk::DescriptorType::eUniformTexelBuffer, 1000),
            vk::DescriptorPoolSize(vk::DescriptorType::eStorageTexelBuffer, 1000),
            vk::DescriptorPoolSize(vk::DescriptorType::eUniformBuffer, 1000),
            vk::DescriptorPoolSize(vk::DescriptorType::eStorageBuffer, 1000),
            vk::DescriptorPoolSize(vk::DescriptorType::eUniformBufferDynamic, 1000),
            vk::DescriptorPoolSize(vk::DescriptorType::eStorageBufferDynamic, 1000),
            vk::DescriptorPoolSize(vk::DescriptorType::eInputAttachment, 1000)
        };

        vk::DescriptorPoolCreateInfo poolInfo(
            vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet,
            1000 * IM_ARRAYSIZE(poolSizes.data()),
            IM_ARRAYSIZE(poolSizes.data()),
            poolSizes.data()
        );

        m_DescriptorPool = vk::raii::DescriptorPool(device, poolInfo);
    }

    void ImGuiHandler::applyTheme()
    {
        ImGuiStyle &style = ImGui::GetStyle();
        ImVec4 *colors = style.Colors;

        // Corners
        style.WindowRounding = 8.0f;
        style.ChildRounding = 8.0f;
        style.FrameRounding = 6.0f;
        style.PopupRounding = 6.0f;
        style.ScrollbarRounding = 6.0f;
        style.GrabRounding = 6.0f;
        style.TabRounding = 6.0f;

        // Colors
        colors[ImGuiCol_Text] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.36f, 0.42f, 0.47f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
        colors[ImGuiCol_Border] = ImVec4(0.43f, 0.50f, 0.56f, 0.50f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.20f, 0.28f, 1.00f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.09f, 0.12f, 0.14f, 1.00f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.12f, 0.14f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.12f, 0.20f, 0.28f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
        colors[ImGuiCol_Button] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.20f, 0.25f, 0.29f, 0.55f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.50f, 0.56f, 0.50f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
        colors[ImGuiCol_Tab] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.28f, 0.56f, 1.00f, 0.80f);
        colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.22f, 0.36f, 1.00f);
        colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 0.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
    }
}
