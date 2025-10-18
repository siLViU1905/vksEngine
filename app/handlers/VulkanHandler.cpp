#include "VulkanHandler.h"

#include <chrono>
#include <fstream>
#include <print>
#include <map>
#include <stb_image.h>
#include <thread>
#include <glm/ext/matrix_transform.hpp>
#include "ImGuiHandler.h"
#include "../scene/ubocomponents/UBOvp.h"

namespace vks_engine
{
    VulkanHandler::VulkanHandler()
        : m_Instance(nullptr), m_DebugMessenger(nullptr),
          m_PhysicalDevice(nullptr), m_Device(nullptr),
          m_GraphicsQueue(nullptr), m_Surface(nullptr),
          m_SwapChain(nullptr), m_SwapChainImageFormat(vk::Format::eUndefined), m_ComplexMeshPipelineLayout(nullptr),
          m_ComplexMeshGraphicsPipeline(nullptr), m_CommandPool(nullptr),
          m_FramebufferResized(false), m_CurrentFrame(0),
          m_SemaphoreIndex(0), m_VertexBuffer(nullptr),
          m_VertexBufferMemory(nullptr), m_IndexBuffer(nullptr),
          m_IndexBufferMemory(nullptr), m_ComplexMeshDescriptorSetLayout(nullptr),
          m_ComplexMeshDescriptorPool(nullptr),
          m_DepthImage(nullptr), m_DepthImageMemory(nullptr),
          m_DepthImageView(nullptr),
          m_ColorImage(nullptr), m_ColorImageView(nullptr),
          m_ColorImageMemory(nullptr),
          m_SimpleMeshGraphicsPipeline(nullptr), m_SimpleMeshPipelineLayout(nullptr),
          m_SimpleMeshDescriptorSetLayout(nullptr), m_SimpleMeshDescriptorPool(nullptr)
    {
    }

    void VulkanHandler::init(GLFWwindow *window)
    {
        m_Window = window;

        stbi_set_flip_vertically_on_load(true);

        m_BackpackDiffuseTexture.setPath("../../app/models/backpack/diffuse.jpg");

        m_BackpackNormalTexture.setPath("../../app/models/backpack/normal.png");

        createInstance();

        setupDebugMessenger();

        createSurface(window);

        pickPhysicalDevice();

        createLogicalDevice();

        createSwapChain(window);

        createImageViews();

        createComplexMeshDescriptorSetLayout();

        createSimpleMeshDescriptorSetLayout();

        createComplexMeshGraphicsPipeline();

        createSimpleMeshGraphicsPipeline();

        createCommandPool();

        createColorResources();

        createDepthResources();

        CreateTexture(m_BackpackDiffuseTexture, m_BackpackDiffuseTexture.load());

        CreateTexture(m_BackpackNormalTexture, m_BackpackNormalTexture.load());

        /*createTextureImage(m_BackpackDiffuseTexture);

        createTextureImageView(m_BackpackDiffuseTexture);

        createTextureSampler(m_BackpackDiffuseTexture);

        createTextureImage(m_BackpackNormalTexture);

        createTextureImageView(m_BackpackNormalTexture);

        createTextureSampler(m_BackpackNormalTexture);*/

        createComplexMeshDescriptorPool();

        createSimpleMeshDescriptorPool();

        createCommandBuffers();

        createSyncObjects();
    }

    void VulkanHandler::createInstance()
    {
        constexpr vk::ApplicationInfo appInfo(
            "VulkanPP",
            VK_MAKE_VERSION(1, 0, 0),
            "vksEngine",
            VK_MAKE_VERSION(1, 0, 0),
            VK_API_VERSION_1_4
        );

        std::vector<const char *> requiredLayers;

        if (enableValidationLayers)
            requiredLayers.assign(validationLayers.begin(), validationLayers.end());

        auto layerProperties = m_Context.enumerateInstanceLayerProperties();

        for (auto const &requiredLayer: requiredLayers)

            if (std::ranges::none_of(layerProperties,
                                     [requiredLayer](auto const &layerProperty)
                                     {
                                         return strcmp(layerProperty.layerName, requiredLayer) == 0;
                                     }))
                throw std::runtime_error("Required layer not supported: " + std::string(requiredLayer));


        auto requiredExtensions = getRequiredExtensions();

        auto extensionProperties = m_Context.enumerateInstanceExtensionProperties();
        for (auto const &requiredExtension: requiredExtensions)
            if (std::ranges::none_of(extensionProperties,
                                     [requiredExtension](auto const &extensionProperty)
                                     {
                                         return strcmp(extensionProperty.extensionName, requiredExtension) == 0;
                                     }))
                throw std::runtime_error("Required extension not supported: " + std::string(requiredExtension));


        std::print("Available extensions:\n");

        for (const auto &extension: m_Context.enumerateInstanceExtensionProperties())
            std::print("\t{}\n", extension.extensionName.data());

        vk::InstanceCreateInfo createInfo{
            {},
            &appInfo,
            static_cast<uint32_t>(requiredLayers.size()),
            requiredLayers.data(),
            static_cast<uint32_t>(requiredExtensions.size()),
            requiredExtensions.data()
        };

        m_Instance = vk::raii::Instance(m_Context, createInfo);
    }

    std::vector<const char *> VulkanHandler::getRequiredExtensions()
    {
        uint32_t glfwExtensionCount = 0;

        auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        if (enableValidationLayers)
            extensions.push_back(vk::EXTDebugUtilsExtensionName);

        return extensions;
    }

    void VulkanHandler::setupDebugMessenger()
    {
        if (!enableValidationLayers)
            return;

        vk::DebugUtilsMessageSeverityFlagsEXT severityFlags(
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);

        vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags(
            vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
            vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);

        vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoEXT{
            {},
            severityFlags,
            messageTypeFlags,
            &debugCallback
        };

        m_DebugMessenger = m_Instance.createDebugUtilsMessengerEXT(debugUtilsMessengerCreateInfoEXT);
    }

    void VulkanHandler::pickPhysicalDevice()
    {
        auto devices = vk::raii::PhysicalDevices(m_Instance);

        if (devices.empty())
            throw std::runtime_error("Failed to find GPUs with Vulkan support");

        const auto devIter = std::ranges::find_if(devices,
                                                  [&](auto const &device)
                                                  {
                                                      auto queueFamilies = device.getQueueFamilyProperties();
                                                      bool isSuitable =
                                                              device.getProperties().apiVersion >= VK_API_VERSION_1_3;
                                                      const auto qfpIter = std::ranges::find_if(queueFamilies,
                                                          [](vk::QueueFamilyProperties const &qfp)
                                                          {
                                                              return (qfp.queueFlags & vk::QueueFlagBits::eGraphics) !=
                                                                     static_cast<vk::QueueFlags>(0);
                                                          });
                                                      isSuitable = isSuitable && (qfpIter != queueFamilies.end());
                                                      auto extensions = device.enumerateDeviceExtensionProperties();
                                                      bool found = true;
                                                      for (auto const &extension: deviceExtensions)
                                                      {
                                                          auto extensionIter = std::ranges::find_if(
                                                              extensions, [extension](auto const &ext)
                                                              {
                                                                  return strcmp(ext.extensionName, extension) == 0;
                                                              });
                                                          found = found && extensionIter != extensions.end();
                                                      }
                                                      isSuitable = isSuitable && found;
                                                      std::print("\n");
                                                      if (isSuitable)
                                                      {
                                                          m_PhysicalDevice = device;
                                                          m_MsaaSamples = getMaxUsableSampleCount();
                                                      }

                                                      return isSuitable;
                                                  });
        if (devIter == devices.end())
        {
            throw std::runtime_error("failed to find a suitable GPU!");
        }

        std::multimap<int, vk::raii::PhysicalDevice> candidates;

        for (const auto &device: devices)
        {
            auto deviceProperties = device.getProperties();
            auto deviceFeatures = device.getFeatures();
            uint32_t score = 0;

            if (deviceProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
                score += 1000;

            score += deviceProperties.limits.maxImageDimension2D;

            if (!deviceFeatures.geometryShader)
                continue;

            candidates.insert(std::make_pair(score, device));
        }

        if (candidates.rbegin()->first > 0)
            m_PhysicalDevice = std::move(candidates.rbegin()->second);
        else
            throw std::runtime_error("Failed to find a suitable GPU");
    }

    void VulkanHandler::createLogicalDevice()
    {
        auto queueFamilyProperties = m_PhysicalDevice.getQueueFamilyProperties();

        uint32_t queueIndex = ~0;

        for (uint32_t qfpIndex = 0; qfpIndex < queueFamilyProperties.size(); qfpIndex++)
        {
            if ((queueFamilyProperties[qfpIndex].queueFlags & vk::QueueFlagBits::eGraphics) &&
                m_PhysicalDevice.getSurfaceSupportKHR(qfpIndex, m_Surface))
            {
                queueIndex = qfpIndex;
                break;
            }
        }

        m_GraphicsQueueFamilyIndex = queueIndex;

        if (queueIndex == ~0)
            throw std::runtime_error("Could not find a queue for graphics and present -> terminating");

        vk::PhysicalDeviceFeatures2 features2{};

        features2.features.samplerAnisotropy = vk::True;

        vk::PhysicalDeviceVulkan13Features features13{};

        features13.dynamicRendering = true;
        features13.synchronization2 = true;

        vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT featuresExt{};

        featuresExt.extendedDynamicState = true;


        vk::StructureChain<
            vk::PhysicalDeviceFeatures2,
            vk::PhysicalDeviceVulkan13Features,
            vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT
        > featureChain = {
            features2,
            features13,
            featuresExt
        };

        float queuePriority = 0.f;

        vk::DeviceQueueCreateInfo deviceQueueCreateInfo{
            {},
            queueIndex,
            1,
            &queuePriority
        };

        vk::DeviceCreateInfo deviceCreateInfo{};

        deviceCreateInfo.pNext = &featureChain.get<vk::PhysicalDeviceFeatures2>();
        deviceCreateInfo.queueCreateInfoCount = 1;
        deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;
        deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();

        m_Device = vk::raii::Device(m_PhysicalDevice, deviceCreateInfo);

        m_GraphicsQueue = vk::raii::Queue(m_Device, queueIndex, 0);
    }

    void VulkanHandler::createSurface(GLFWwindow *window)
    {
        VkSurfaceKHR surface;

        if (glfwCreateWindowSurface(*m_Instance, window, nullptr, &surface))
            throw std::runtime_error("failed to create window surface");

        m_Surface = vk::raii::SurfaceKHR(m_Instance, surface);
    }

    vk::SurfaceFormatKHR VulkanHandler::chooseSwapSurfaceFormat(
        const std::vector<vk::SurfaceFormatKHR> &availableFormats)
    {
        for (const auto &format: availableFormats)
            if (format.format == vk::Format::eB8G8R8A8Srgb &&
                format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
                return format;

        return availableFormats[0];
    }

    vk::PresentModeKHR VulkanHandler::chooseSwapPresentMode(
        const std::vector<vk::PresentModeKHR> &availablePresentModes)
    {
        for (const auto &presentMode: availablePresentModes)
            if (presentMode == vk::PresentModeKHR::eMailbox)
                return presentMode;

        return vk::PresentModeKHR::eFifo;
    }

    vk::Extent2D VulkanHandler::chooseSwapExtent(const vk::SurfaceCapabilitiesKHR &capabilities, GLFWwindow *window)
    {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()
        )
            return capabilities.currentExtent;

        int width, height;

        glfwGetFramebufferSize(window, &width, &height);

        return {
            std::clamp<uint32_t>(width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
            std::clamp<uint32_t>(height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height)
        };
    }

    void VulkanHandler::createSwapChain(GLFWwindow *window)
    {
        auto surfaceCapabilities = m_PhysicalDevice.getSurfaceCapabilitiesKHR(m_Surface);

        m_SwapChainSurfaceFormat = chooseSwapSurfaceFormat(m_PhysicalDevice.getSurfaceFormatsKHR(m_Surface));

        m_SwapChainExtent = chooseSwapExtent(surfaceCapabilities, window);

        m_SwapChainImageFormat = m_SwapChainSurfaceFormat.format;

        auto minImageCount = std::max(3u, surfaceCapabilities.minImageCount);

        minImageCount = (surfaceCapabilities.maxImageCount > 0 && minImageCount > surfaceCapabilities.maxImageCount)
                            ? surfaceCapabilities.maxImageCount
                            : minImageCount;

        uint32_t imageCount = surfaceCapabilities.minImageCount + 1;

        if (surfaceCapabilities.maxImageCount > 0 && imageCount > surfaceCapabilities.maxImageCount)
            imageCount = surfaceCapabilities.maxImageCount;

        vk::SwapchainCreateInfoKHR swapChainCreateInfo{
            vk::SwapchainCreateFlagsKHR(),
            m_Surface,
            minImageCount,
            m_SwapChainSurfaceFormat.format,
            m_SwapChainSurfaceFormat.colorSpace,
            m_SwapChainExtent,
            1,
            vk::ImageUsageFlagBits::eColorAttachment,
            vk::SharingMode::eExclusive
        };

        swapChainCreateInfo.preTransform = surfaceCapabilities.currentTransform;
        swapChainCreateInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
        swapChainCreateInfo.presentMode = chooseSwapPresentMode(m_PhysicalDevice.getSurfacePresentModesKHR(m_Surface));
        swapChainCreateInfo.clipped = true;
        swapChainCreateInfo.oldSwapchain = nullptr;

        m_SwapChain = vk::raii::SwapchainKHR(m_Device, swapChainCreateInfo);

        m_SwapChainImages = m_SwapChain.getImages();
    }

    void VulkanHandler::createImageViews()
    {
        m_SwapChainImageViews.clear();

        vk::ImageViewCreateInfo imageViewCreateInfo
        {
            {},
            {},
            vk::ImageViewType::e2D,
            m_SwapChainImageFormat
        };

        imageViewCreateInfo.subresourceRange = {
            vk::ImageAspectFlagBits::eColor,
            0, 1, 0, 1
        };

        for (auto &image: m_SwapChainImages)
        {
            imageViewCreateInfo.image = image;
            m_SwapChainImageViews.emplace_back(m_Device, imageViewCreateInfo);
        }
    }

    void VulkanHandler::createComplexMeshGraphicsPipeline()
    {
        auto vertexShaderCode = readFile("shaders/complexmesh/ComplexMeshShaderVert.spv");
        auto fragmentShaderCode = readFile("shaders/complexmesh/ComplexMeshShaderFrag.spv");

        auto vertexShaderModule = createShaderModule(vertexShaderCode, m_Device);
        auto fragmentShaderModule = createShaderModule(fragmentShaderCode, m_Device);

        vk::PipelineShaderStageCreateInfo vertexShaderStageInfo{};

        vertexShaderStageInfo.stage = vk::ShaderStageFlagBits::eVertex;
        vertexShaderStageInfo.module = vertexShaderModule;
        vertexShaderStageInfo.pName = "main";

        vk::PipelineShaderStageCreateInfo fragmentShaderStageInfo{};

        fragmentShaderStageInfo.stage = vk::ShaderStageFlagBits::eFragment;
        fragmentShaderStageInfo.module = fragmentShaderModule;
        fragmentShaderStageInfo.pName = "main";

        vk::PipelineShaderStageCreateInfo shaderStages[] = {vertexShaderStageInfo, fragmentShaderStageInfo};

        std::vector<vk::DynamicState> dynamicStates =
        {
            vk::DynamicState::eViewport,
            vk::DynamicState::eScissor
        };

        vk::PipelineDynamicStateCreateInfo dynamicState{};

        dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        dynamicState.pDynamicStates = dynamicStates.data();

        auto bindingDescription = Vertex::getBindingDescription();
        auto attributeDescription = Vertex::getAttributeDescription();

        vk::PipelineVertexInputStateCreateInfo vertexInputInfo;

        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;

        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescription.size());
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescription.data();

        vk::PipelineInputAssemblyStateCreateInfo inputAssembly;

        inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;

        m_Viewport = vk::Viewport(0.f, 0.f,
                                  static_cast<float>(m_SwapChainExtent.width),
                                  static_cast<float>(m_SwapChainExtent.height),
                                  0.f, 1.f);

        m_Scissor = vk::Rect2D({0, 0},
                               m_SwapChainExtent);

        vk::PipelineViewportStateCreateInfo viewportState({}, 1, {}, 1);

        vk::PipelineRasterizationStateCreateInfo rasterizer({}, vk::False,
                                                            vk::False,
                                                            vk::PolygonMode::eFill,
                                                            vk::CullModeFlagBits::eBack,
                                                            vk::FrontFace::eCounterClockwise,
                                                            vk::False
        );

        rasterizer.depthBiasSlopeFactor = 1.f;
        rasterizer.lineWidth = 1.f;

        vk::PipelineMultisampleStateCreateInfo multisampling;

        multisampling.rasterizationSamples = m_MsaaSamples;
        multisampling.sampleShadingEnable = vk::True;

        vk::PipelineColorBlendAttachmentState colorBlendAttachment;

        colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
                                              vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
        colorBlendAttachment.blendEnable = vk::False;
        colorBlendAttachment.blendEnable = vk::True;
        colorBlendAttachment.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;
        colorBlendAttachment.dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;
        colorBlendAttachment.colorBlendOp = vk::BlendOp::eAdd;
        colorBlendAttachment.srcAlphaBlendFactor = vk::BlendFactor::eOne;
        colorBlendAttachment.dstAlphaBlendFactor = vk::BlendFactor::eZero;
        colorBlendAttachment.alphaBlendOp = vk::BlendOp::eAdd;

        vk::PipelineColorBlendStateCreateInfo colorBlending({}, vk::False,
                                                            vk::LogicOp::eCopy, 1, &colorBlendAttachment);

        vk::PipelineDepthStencilStateCreateInfo depthStencilInfo;

        depthStencilInfo.depthTestEnable = vk::True;
        depthStencilInfo.depthWriteEnable = vk::True;
        depthStencilInfo.depthCompareOp = vk::CompareOp::eLess;
        depthStencilInfo.depthBoundsTestEnable = vk::False;
        depthStencilInfo.stencilTestEnable = vk::False;

        vk::PushConstantRange modelPushConstant(
            vk::ShaderStageFlagBits::eVertex,
            0,
            sizeof(glm::mat4)
        );

        vk::PushConstantRange colorPushConstant(
            vk::ShaderStageFlagBits::eFragment,
            sizeof(glm::mat4),
            sizeof(glm::vec4)
        );

        vk::PushConstantRange pushConstants[] = {modelPushConstant, colorPushConstant};

        vk::PipelineLayoutCreateInfo pipelineLayoutInfo({},
                                                        1,
                                                        &*m_ComplexMeshDescriptorSetLayout,
                                                        2,
                                                        pushConstants
        );


        m_ComplexMeshPipelineLayout = vk::raii::PipelineLayout(m_Device, pipelineLayoutInfo);

        auto depthFormat = findDepthFormat();

        vk::PipelineRenderingCreateInfo pipelineRenderingCreateInfo(
            {},
            1,
            &m_SwapChainImageFormat,
            depthFormat
        );

        vk::GraphicsPipelineCreateInfo pipelineInfo;

        pipelineInfo.pNext = &pipelineRenderingCreateInfo;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = &dynamicState;
        pipelineInfo.pDepthStencilState = &depthStencilInfo;
        pipelineInfo.layout = m_ComplexMeshPipelineLayout;
        pipelineInfo.renderPass = nullptr;

        m_ComplexMeshGraphicsPipeline = vk::raii::Pipeline(m_Device, nullptr, pipelineInfo);
    }

    void VulkanHandler::createSimpleMeshGraphicsPipeline()
    {
        auto vertexShaderCode = readFile("shaders/simplemesh/SimpleMeshShaderVert.spv");
        auto fragmentShaderCode = readFile("shaders/simplemesh/SimpleMeshShaderFrag.spv");

        auto vertexShaderModule = createShaderModule(vertexShaderCode, m_Device);
        auto fragmentShaderModule = createShaderModule(fragmentShaderCode, m_Device);

        vk::PipelineShaderStageCreateInfo vertexShaderStageInfo{};

        vertexShaderStageInfo.stage = vk::ShaderStageFlagBits::eVertex;
        vertexShaderStageInfo.module = vertexShaderModule;
        vertexShaderStageInfo.pName = "main";

        vk::PipelineShaderStageCreateInfo fragmentShaderStageInfo{};

        fragmentShaderStageInfo.stage = vk::ShaderStageFlagBits::eFragment;
        fragmentShaderStageInfo.module = fragmentShaderModule;
        fragmentShaderStageInfo.pName = "main";

        vk::PipelineShaderStageCreateInfo shaderStages[] = {vertexShaderStageInfo, fragmentShaderStageInfo};

        std::vector<vk::DynamicState> dynamicStates =
        {
            vk::DynamicState::eViewport,
            vk::DynamicState::eScissor
        };

        vk::PipelineDynamicStateCreateInfo dynamicState{};

        dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        dynamicState.pDynamicStates = dynamicStates.data();

        auto bindingDescription = Vertex::getBindingDescription();
        auto attributeDescription = Vertex::getAttributeDescription();

        vk::PipelineVertexInputStateCreateInfo vertexInputInfo;

        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;

        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescription.size());
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescription.data();

        vk::PipelineInputAssemblyStateCreateInfo inputAssembly;

        inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;

        m_Viewport = vk::Viewport(0.f, 0.f,
                                  static_cast<float>(m_SwapChainExtent.width),
                                  static_cast<float>(m_SwapChainExtent.height),
                                  0.f, 1.f);

        m_Scissor = vk::Rect2D({0, 0},
                               m_SwapChainExtent);

        vk::PipelineViewportStateCreateInfo viewportState({}, 1, {}, 1);

        vk::PipelineRasterizationStateCreateInfo rasterizer({}, vk::False,
                                                            vk::False,
                                                            vk::PolygonMode::eFill,
                                                            vk::CullModeFlagBits::eBack,
                                                            vk::FrontFace::eCounterClockwise,
                                                            vk::False
        );

        rasterizer.depthBiasSlopeFactor = 1.f;
        rasterizer.lineWidth = 1.f;

        vk::PipelineMultisampleStateCreateInfo multisampling;

        multisampling.rasterizationSamples = m_MsaaSamples;
        multisampling.sampleShadingEnable = vk::True;

        vk::PipelineColorBlendAttachmentState colorBlendAttachment;

        colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
                                              vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
        colorBlendAttachment.blendEnable = vk::False;
        colorBlendAttachment.blendEnable = vk::True;
        colorBlendAttachment.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;
        colorBlendAttachment.dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;
        colorBlendAttachment.colorBlendOp = vk::BlendOp::eAdd;
        colorBlendAttachment.srcAlphaBlendFactor = vk::BlendFactor::eOne;
        colorBlendAttachment.dstAlphaBlendFactor = vk::BlendFactor::eZero;
        colorBlendAttachment.alphaBlendOp = vk::BlendOp::eAdd;

        vk::PipelineColorBlendStateCreateInfo colorBlending({}, vk::False,
                                                            vk::LogicOp::eCopy, 1, &colorBlendAttachment);

        vk::PipelineDepthStencilStateCreateInfo depthStencilInfo;

        depthStencilInfo.depthTestEnable = vk::True;
        depthStencilInfo.depthWriteEnable = vk::True;
        depthStencilInfo.depthCompareOp = vk::CompareOp::eLess;
        depthStencilInfo.depthBoundsTestEnable = vk::False;
        depthStencilInfo.stencilTestEnable = vk::False;

        vk::PushConstantRange modelPushConstant(
            vk::ShaderStageFlagBits::eVertex,
            0,
            sizeof(glm::mat4)
        );

        vk::PushConstantRange colorPushConstant(
            vk::ShaderStageFlagBits::eFragment,
            sizeof(glm::mat4),
            sizeof(glm::vec4)
        );

        vk::PushConstantRange pushConstants[] = {modelPushConstant, colorPushConstant};

        vk::PipelineLayoutCreateInfo pipelineLayoutInfo({},
                                                        1,
                                                        &*m_SimpleMeshDescriptorSetLayout,
                                                        2,
                                                        pushConstants
        );


        m_SimpleMeshPipelineLayout = vk::raii::PipelineLayout(m_Device, pipelineLayoutInfo);

        auto depthFormat = findDepthFormat();

        vk::PipelineRenderingCreateInfo pipelineRenderingCreateInfo(
            {},
            1,
            &m_SwapChainImageFormat,
            depthFormat
        );

        vk::GraphicsPipelineCreateInfo pipelineInfo;

        pipelineInfo.pNext = &pipelineRenderingCreateInfo;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = &dynamicState;
        pipelineInfo.pDepthStencilState = &depthStencilInfo;
        pipelineInfo.layout = m_SimpleMeshPipelineLayout;
        pipelineInfo.renderPass = nullptr;

        m_SimpleMeshGraphicsPipeline = vk::raii::Pipeline(m_Device, nullptr, pipelineInfo);
    }

    void VulkanHandler::createCommandPool()
    {
        vk::CommandPoolCreateInfo poolInfo(vk::CommandPoolCreateFlagBits::eResetCommandBuffer);

        m_CommandPool = vk::raii::CommandPool(m_Device, poolInfo);
    }

    void VulkanHandler::recordCommandBuffer(uint32_t imageIndex, vk::raii::CommandBuffer &imguiCMDBuffer,
                                            vk::raii::CommandBuffer &simpleMeshCMDBuffer,
                                            vk::raii::CommandBuffer &complexMeshCMDBuffer)
    {
        m_CommandBuffers[m_CurrentFrame].begin({});

        transitionImageLayout(
            imageIndex,
            vk::ImageLayout::eUndefined,
            vk::ImageLayout::eColorAttachmentOptimal,
            {},
            vk::AccessFlagBits2::eColorAttachmentWrite,
            vk::PipelineStageFlagBits2::eTopOfPipe,
            vk::PipelineStageFlagBits2::eColorAttachmentOutput
        );

        vk::ClearValue clearColor = vk::ClearColorValue(0.0f, 0.0f, 0.0f, 1.0f);

        vk::ClearValue clearDepth = vk::ClearDepthStencilValue(1.f, 0.f);

        vk::RenderingAttachmentInfo colorAttachmentInfo(
            m_ColorImageView,
            vk::ImageLayout::eColorAttachmentOptimal,
            vk::ResolveModeFlagBits::eAverage,
            m_SwapChainImageViews[imageIndex],
            vk::ImageLayout::eColorAttachmentOptimal
        );

        colorAttachmentInfo.loadOp = vk::AttachmentLoadOp::eClear;
        colorAttachmentInfo.storeOp = vk::AttachmentStoreOp::eStore;
        colorAttachmentInfo.clearValue = clearColor;

        vk::RenderingAttachmentInfo depthAttachmentInfo(
            m_DepthImageView,
            vk::ImageLayout::eDepthStencilAttachmentOptimal
        );

        depthAttachmentInfo.loadOp = vk::AttachmentLoadOp::eClear;
        depthAttachmentInfo.storeOp = vk::AttachmentStoreOp::eDontCare;
        depthAttachmentInfo.clearValue = clearDepth;

        vk::RenderingInfoKHR renderingInfo(
            vk::RenderingFlagBitsKHR::eContentsSecondaryCommandBuffers,
            m_Scissor,
            1, {}, 1,
            &colorAttachmentInfo,
            &depthAttachmentInfo
        );

        m_CommandBuffers[m_CurrentFrame].beginRendering(renderingInfo);

        vk::CommandBuffer secCMDBuffers[] = {*simpleMeshCMDBuffer, *complexMeshCMDBuffer, *imguiCMDBuffer};

        m_CommandBuffers[m_CurrentFrame].executeCommands(secCMDBuffers);

        m_CommandBuffers[m_CurrentFrame].endRendering();

        transitionImageLayout(
            imageIndex,
            vk::ImageLayout::eColorAttachmentOptimal,
            vk::ImageLayout::ePresentSrcKHR,
            vk::AccessFlagBits2::eColorAttachmentWrite,
            {},
            vk::PipelineStageFlagBits2::eColorAttachmentOutput,
            vk::PipelineStageFlagBits2::eBottomOfPipe
        );

        m_CommandBuffers[m_CurrentFrame].end();
    }

    void VulkanHandler::transitionImageLayout(uint32_t imageIndex, vk::ImageLayout oldLayout, vk::ImageLayout newLayout,
                                              vk::AccessFlags2 srcAccessMask, vk::AccessFlags2 dstAccessMask,
                                              vk::PipelineStageFlags2 srcStageMask,
                                              vk::PipelineStageFlags2 dstStageMask)
    {
        vk::ImageMemoryBarrier2 barrier(
            srcStageMask, srcAccessMask,
            dstStageMask, dstAccessMask,
            oldLayout, newLayout,
            VK_QUEUE_FAMILY_IGNORED,
            VK_QUEUE_FAMILY_IGNORED,
            m_SwapChainImages[imageIndex],
            {
                vk::ImageAspectFlagBits::eColor,
                0, 1, 0, 1
            }
        );

        vk::DependencyInfo dependencyInfo;

        dependencyInfo.imageMemoryBarrierCount = 1;
        dependencyInfo.pImageMemoryBarriers = &barrier;

        m_CommandBuffers[m_CurrentFrame].pipelineBarrier2(dependencyInfo);
    }

    void VulkanHandler::transitionImageLayout(const vk::raii::Image &image,
                                              vk::ImageLayout oldLayout,
                                              vk::ImageLayout newLayout,
                                              uint32_t mipLevels
    )
    {
        auto commandBuffer = beginSingleTimeCommands();

        vk::ImageMemoryBarrier barrier;

        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.image = image;
        barrier.subresourceRange = {
            vk::ImageAspectFlagBits::eColor,
            0, mipLevels, 0, 1
        };

        vk::PipelineStageFlags sourceStage;

        vk::PipelineStageFlags destinationStage;

        if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eTransferDstOptimal)
        {
            barrier.srcAccessMask = {};
            barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

            sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
            destinationStage = vk::PipelineStageFlagBits::eTransfer;
        } else if (oldLayout == vk::ImageLayout::eTransferDstOptimal && newLayout ==
                   vk::ImageLayout::eShaderReadOnlyOptimal)
        {
            barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
            barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

            sourceStage = vk::PipelineStageFlagBits::eTransfer;
            destinationStage = vk::PipelineStageFlagBits::eFragmentShader;
        } else
            throw std::invalid_argument("Unsupported layout transition");

        commandBuffer->pipelineBarrier(sourceStage, destinationStage, {}, {}, nullptr, barrier);
        endSingleTimeCommands(*commandBuffer);
    }

    void VulkanHandler::copyBufferToImage(const vk::raii::Buffer &buffer, vk::raii::Image &image, uint32_t width,
                                          uint32_t height)
    {
        auto commandBuffer = beginSingleTimeCommands();

        vk::BufferImageCopy region(
            0, 0, 0,
            {vk::ImageAspectFlagBits::eColor, 0, 0, 1},
            {0, 0, 0},
            {width, height, 1}
        );

        commandBuffer->copyBufferToImage(buffer, image, vk::ImageLayout::eTransferDstOptimal, {region});

        endSingleTimeCommands(*commandBuffer);
    }

    std::unique_ptr<vk::raii::CommandBuffer> VulkanHandler::beginSingleTimeCommands()
    {
        vk::CommandBufferAllocateInfo allocInfo(
            m_CommandPool,
            vk::CommandBufferLevel::ePrimary,
            1
        );

        auto commandBuffer = std::make_unique<vk::raii::CommandBuffer>
        (
            std::move(vk::raii::CommandBuffers(m_Device, allocInfo).front())
        );

        commandBuffer->begin(vk::CommandBufferBeginInfo(vk::CommandBufferUsageFlagBits::eOneTimeSubmit));

        return commandBuffer;
    }

    void VulkanHandler::endSingleTimeCommands(vk::raii::CommandBuffer &commandBuffer)
    {
        commandBuffer.end();

        vk::SubmitInfo submitInfo;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &*commandBuffer;

        m_GraphicsQueue.submit(submitInfo);

        m_GraphicsQueue.waitIdle();
    }

    void VulkanHandler::cleanupSwapChain()
    {
        m_ColorImageView = nullptr;
        m_ColorImage = nullptr;
        m_ColorImageMemory = nullptr;

        m_DepthImageView = nullptr;
        m_DepthImage = nullptr;
        m_DepthImageMemory = nullptr;

        m_SwapChainImageViews.clear();
        m_SwapChain = nullptr;
    }

    void VulkanHandler::rebuildSwapChain()
    {
        createSwapChain(m_Window);

        createImageViews();

        createColorResources();

        createDepthResources();

        m_Viewport.width = static_cast<float>(m_SwapChainExtent.width);

        m_Viewport.height = static_cast<float>(m_SwapChainExtent.height);

        m_Scissor.extent = m_SwapChainExtent;
    }

    vk::SampleCountFlagBits VulkanHandler::getMaxUsableSampleCount()
    {
        vk::PhysicalDeviceProperties physicalDeviceProperties = m_PhysicalDevice.getProperties();

        vk::SampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts &
                                      physicalDeviceProperties.limits.framebufferDepthSampleCounts;

        if (counts & vk::SampleCountFlagBits::e64) { return vk::SampleCountFlagBits::e64; }
        if (counts & vk::SampleCountFlagBits::e32) { return vk::SampleCountFlagBits::e32; }
        if (counts & vk::SampleCountFlagBits::e16) { return vk::SampleCountFlagBits::e16; }
        if (counts & vk::SampleCountFlagBits::e8) { return vk::SampleCountFlagBits::e8; }
        if (counts & vk::SampleCountFlagBits::e4) { return vk::SampleCountFlagBits::e4; }
        if (counts & vk::SampleCountFlagBits::e2) { return vk::SampleCountFlagBits::e2; }

        return vk::SampleCountFlagBits::e1;
    }

    void VulkanHandler::createColorResources()
    {
        createImage(m_SwapChainExtent.width, m_SwapChainExtent.height, 1,
                    m_SwapChainImageFormat,
                    m_MsaaSamples,
                    vk::ImageTiling::eOptimal,
                    vk::ImageUsageFlagBits::eTransientAttachment | vk::ImageUsageFlagBits::eColorAttachment,
                    vk::MemoryPropertyFlagBits::eDeviceLocal,
                    m_ColorImage,
                    m_ColorImageMemory
        );

        m_ColorImageView = createImageView(m_ColorImage, m_SwapChainImageFormat, vk::ImageAspectFlagBits::eColor, 1);
    }

    uint32_t VulkanHandler::findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties)
    {
        auto memProperties = m_PhysicalDevice.getMemoryProperties();

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i)
            if ((typeFilter & (1 << i)) &&
                (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
                return i;

        throw std::runtime_error("Failed to find suitable memory type");
    }

    void VulkanHandler::notifyFramebufferResized()
    {
        m_FramebufferResized = true;
    }

    void VulkanHandler::CreateUniformBuffers(UniformBuffer &buffer, vk::DeviceSize size)
    {
        buffer.m_Buffers.clear();
        buffer.m_Memories.clear();
        buffer.m_MappedMemories.clear();

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            vk::raii::Buffer tempBuffer({});

            vk::raii::DeviceMemory tempBufferMem({});

            createBuffer(size,
                         vk::BufferUsageFlagBits::eUniformBuffer,
                         vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                         tempBuffer,
                         tempBufferMem);

            buffer.m_Buffers.emplace_back(std::move(tempBuffer));

            buffer.m_Memories.emplace_back(std::move(tempBufferMem));

            buffer.m_MappedMemories.emplace_back(buffer.m_Memories[i].mapMemory(0, size));
        }
    }

    void VulkanHandler::CreateCommandBuffers(std::vector<vk::raii::CommandBuffer> &commandBuffers,
                                             vk::CommandBufferLevel level)
    {
        commandBuffers.clear();

        vk::CommandBufferAllocateInfo allocInfo{
            m_CommandPool,
            level,
            MAX_FRAMES_IN_FLIGHT
        };

        commandBuffers = m_Device.allocateCommandBuffers(allocInfo);
    }

    void VulkanHandler::CreateVertexBuffer(const std::vector<Vertex> &vertices, vk::raii::Buffer &buffer,
                                           vk::raii::DeviceMemory &bufferMemory)
    {
        vk::DeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

        vk::raii::Buffer stagingBuffer({});

        vk::raii::DeviceMemory stagingBufferMemory({});

        createBuffer(bufferSize,
                     vk::BufferUsageFlagBits::eTransferSrc,
                     vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                     stagingBuffer,
                     stagingBufferMemory);

        void *dataStaging = stagingBufferMemory.mapMemory(0, bufferSize);

        memcpy(dataStaging, vertices.data(), bufferSize);

        stagingBufferMemory.unmapMemory();

        createBuffer(bufferSize,
                     vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer,
                     vk::MemoryPropertyFlagBits::eDeviceLocal,
                     buffer,
                     bufferMemory);

        copyBuffer(stagingBuffer, buffer, bufferSize);
    }

    void VulkanHandler::CreateIndexBuffer(const std::vector<uint32_t> &indices, vk::raii::Buffer &buffer,
                                          vk::raii::DeviceMemory &bufferMemory)
    {
        vk::DeviceSize bufferSize = sizeof(indices[0]) * indices.size();

        vk::raii::Buffer stagingBuffer({});

        vk::raii::DeviceMemory stagingBufferMemory({});

        createBuffer(bufferSize,
                     vk::BufferUsageFlagBits::eTransferSrc,
                     vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                     stagingBuffer,
                     stagingBufferMemory);

        void *dataStaging = stagingBufferMemory.mapMemory(0, bufferSize);

        memcpy(dataStaging, indices.data(), bufferSize);

        stagingBufferMemory.unmapMemory();

        createBuffer(bufferSize,
                     vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer,
                     vk::MemoryPropertyFlagBits::eDeviceLocal,
                     buffer,
                     bufferMemory);

        copyBuffer(stagingBuffer, buffer, bufferSize);
    }

    void VulkanHandler::GenerateMipMaps(Texture &texture, vk::Format imageFormat)
    {
        vk::FormatProperties formatProperties = m_PhysicalDevice.getFormatProperties(imageFormat);

        if (!(formatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eSampledImageFilterLinear))
            throw std::runtime_error("Texture image format does not support linear blitting");


        auto commandBuffer = beginSingleTimeCommands();

        vk::ImageMemoryBarrier barrier(
            vk::AccessFlagBits::eTransferWrite,
            vk::AccessFlagBits::eTransferRead,
            vk::ImageLayout::eTransferDstOptimal,
            vk::ImageLayout::eTransferSrcOptimal,
            vk::QueueFamilyIgnored,
            vk::QueueFamilyIgnored,
            texture.m_Image
        );

        barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;
        barrier.subresourceRange.levelCount = 1;

        int mipWidth = texture.m_Width;
        int mipHeight = texture.m_Height;

        for (uint32_t i = 1; i < texture.m_MipLevels; i++)
        {
            barrier.subresourceRange.baseMipLevel = i - 1;
            barrier.oldLayout = vk::ImageLayout::eTransferDstOptimal;
            barrier.newLayout = vk::ImageLayout::eTransferSrcOptimal;
            barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
            barrier.dstAccessMask = vk::AccessFlagBits::eTransferRead;

            commandBuffer->pipelineBarrier(
                vk::PipelineStageFlagBits::eTransfer,
                vk::PipelineStageFlagBits::eTransfer,
                {}, {}, {},
                barrier);

            vk::ArrayWrapper1D<vk::Offset3D, 2> offsets, dstOffsets;
            offsets[0] = vk::Offset3D(0, 0, 0);
            offsets[1] = vk::Offset3D(mipWidth, mipHeight, 1);
            dstOffsets[0] = vk::Offset3D(0, 0, 0);
            dstOffsets[1] = vk::Offset3D(mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1);
            vk::ImageBlit blit(
                {}, offsets,
                {}, dstOffsets
            );
            blit.srcSubresource = vk::ImageSubresourceLayers(vk::ImageAspectFlagBits::eColor, i - 1, 0, 1);
            blit.dstSubresource = vk::ImageSubresourceLayers(vk::ImageAspectFlagBits::eColor, i, 0, 1);

            commandBuffer->blitImage(
                texture.m_Image,
                vk::ImageLayout::eTransferSrcOptimal,
                texture.m_Image,
                vk::ImageLayout::eTransferDstOptimal,
                {blit},
                vk::Filter::eLinear
            );

            barrier.oldLayout = vk::ImageLayout::eTransferSrcOptimal;
            barrier.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
            barrier.srcAccessMask = vk::AccessFlagBits::eTransferRead;
            barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

            commandBuffer->pipelineBarrier(
                vk::PipelineStageFlagBits::eTransfer,
                vk::PipelineStageFlagBits::eFragmentShader,
                {}, {}, {},
                barrier
            );

            if (mipWidth > 1)
                mipWidth /= 2;

            if (mipHeight > 1)
                mipHeight /= 2;
        }

        barrier.subresourceRange.baseMipLevel = texture.m_MipLevels - 1;
        barrier.oldLayout = vk::ImageLayout::eTransferDstOptimal;
        barrier.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
        barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
        barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

        commandBuffer->pipelineBarrier(
            vk::PipelineStageFlagBits::eTransfer,
            vk::PipelineStageFlagBits::eFragmentShader,
            {}, {}, {},
            barrier
        );

        endSingleTimeCommands(*commandBuffer);
    }

    void VulkanHandler::CreateTexture(Texture &texture, uint8_t *pixels)
    {
        createTextureImage(texture, pixels);

        createTextureImageView(texture);

        createTextureSampler(texture);
    }

    void VulkanHandler::CreateMeshDescriptorSets(Mesh &mesh,
                                                 UniformBuffer &mvpUBO, vk::DeviceSize vpSize,
                                                 UniformBuffer &pointLightUBO, vk::DeviceSize plSize,
                                                 UniformBuffer &directionalLightUBO, vk::DeviceSize dlSize,
                                                 UniformBuffer &countersUBO, vk::DeviceSize ctSize)
    {
    }

    void VulkanHandler::waitIdle() const
    {
        m_Device.waitIdle();
    }

    void VulkanHandler::createCommandBuffers()
    {
        m_CommandBuffers.clear();

        vk::CommandBufferAllocateInfo allocInfo(
            m_CommandPool,
            vk::CommandBufferLevel::ePrimary,
            MAX_FRAMES_IN_FLIGHT
        );

        m_CommandBuffers = vk::raii::CommandBuffers(m_Device, allocInfo);
    }

    void VulkanHandler::createSyncObjects()
    {
        m_PresentCompleteSemaphores.clear();

        m_RenderFinishedSemaphores.clear();

        m_InFlightFences.clear();

        for (size_t i = 0; i < m_SwapChainImages.size(); ++i)
        {
            m_PresentCompleteSemaphores.emplace_back(m_Device, vk::SemaphoreCreateInfo());
            m_RenderFinishedSemaphores.emplace_back(m_Device, vk::SemaphoreCreateInfo());
        }

        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
            m_InFlightFences.emplace_back(m_Device, vk::FenceCreateInfo(vk::FenceCreateFlagBits::eSignaled));
    }

    void VulkanHandler::createVertexBuffer(const std::vector<Vertex> &vertices)
    {
        vk::DeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

        vk::raii::Buffer stagingBuffer({});

        vk::raii::DeviceMemory stagingBufferMemory({});

        createBuffer(bufferSize,
                     vk::BufferUsageFlagBits::eTransferSrc,
                     vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                     stagingBuffer,
                     stagingBufferMemory);

        void *dataStaging = stagingBufferMemory.mapMemory(0, bufferSize);

        memcpy(dataStaging, vertices.data(), bufferSize);

        stagingBufferMemory.unmapMemory();

        createBuffer(bufferSize,
                     vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer,
                     vk::MemoryPropertyFlagBits::eDeviceLocal,
                     m_VertexBuffer,
                     m_VertexBufferMemory);

        copyBuffer(stagingBuffer, m_VertexBuffer, bufferSize);
    }

    void VulkanHandler::createIndexBuffer(const std::vector<uint32_t> &indices)
    {
        vk::DeviceSize bufferSize = sizeof(indices[0]) * indices.size();

        vk::raii::Buffer stagingBuffer({});

        vk::raii::DeviceMemory stagingBufferMemory({});

        createBuffer(bufferSize,
                     vk::BufferUsageFlagBits::eTransferSrc,
                     vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                     stagingBuffer,
                     stagingBufferMemory);

        void *dataStaging = stagingBufferMemory.mapMemory(0, bufferSize);

        memcpy(dataStaging, indices.data(), bufferSize);

        stagingBufferMemory.unmapMemory();

        createBuffer(bufferSize,
                     vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer,
                     vk::MemoryPropertyFlagBits::eDeviceLocal,
                     m_IndexBuffer,
                     m_IndexBufferMemory);

        copyBuffer(stagingBuffer, m_IndexBuffer, bufferSize);
    }

    void VulkanHandler::createComplexMeshDescriptorSetLayout()
    {
        std::array<vk::DescriptorSetLayoutBinding, 5> bindings = {
            vk::DescriptorSetLayoutBinding(
                0,
                vk::DescriptorType::eUniformBuffer,
                1,
                vk::ShaderStageFlagBits::eVertex
            ),

            vk::DescriptorSetLayoutBinding(
                1,
                vk::DescriptorType::eUniformBuffer,
                1,
                vk::ShaderStageFlagBits::eFragment
            ),

            vk::DescriptorSetLayoutBinding(
                2,
                vk::DescriptorType::eUniformBuffer,
                1,
                vk::ShaderStageFlagBits::eFragment
            ),

            vk::DescriptorSetLayoutBinding(
                3,
                vk::DescriptorType::eUniformBuffer,
                1,
                vk::ShaderStageFlagBits::eFragment
            ),

            vk::DescriptorSetLayoutBinding(
                4,
                vk::DescriptorType::eCombinedImageSampler,
                2,
                vk::ShaderStageFlagBits::eFragment
            )
        };

        vk::DescriptorSetLayoutCreateInfo layoutInfo(
            {},
            bindings.size(),
            bindings.data()
        );

        m_ComplexMeshDescriptorSetLayout = vk::raii::DescriptorSetLayout(m_Device, layoutInfo);
    }

    void VulkanHandler::createSimpleMeshDescriptorSetLayout()
    {
        std::array<vk::DescriptorSetLayoutBinding, 4> bindings = {
            vk::DescriptorSetLayoutBinding(
                0,
                vk::DescriptorType::eUniformBuffer,
                1,
                vk::ShaderStageFlagBits::eVertex
            ),

            vk::DescriptorSetLayoutBinding(
                1,
                vk::DescriptorType::eUniformBuffer,
                1,
                vk::ShaderStageFlagBits::eFragment
            ),

            vk::DescriptorSetLayoutBinding(
                2,
                vk::DescriptorType::eUniformBuffer,
                1,
                vk::ShaderStageFlagBits::eFragment
            ),

            vk::DescriptorSetLayoutBinding(
                3,
                vk::DescriptorType::eUniformBuffer,
                1,
                vk::ShaderStageFlagBits::eFragment
            )
        };

        vk::DescriptorSetLayoutCreateInfo layoutInfo(
            {},
            bindings.size(),
            bindings.data()
        );

        m_SimpleMeshDescriptorSetLayout = vk::raii::DescriptorSetLayout(m_Device, layoutInfo);
    }


    void VulkanHandler::createComplexMeshDescriptorPool()
    {
        std::array<vk::DescriptorPoolSize, 5> poolSize = {
            vk::DescriptorPoolSize(vk::DescriptorType::eUniformBuffer, MAX_FRAMES_IN_FLIGHT),
            vk::DescriptorPoolSize(vk::DescriptorType::eUniformBuffer, MAX_FRAMES_IN_FLIGHT),
            vk::DescriptorPoolSize(vk::DescriptorType::eUniformBuffer, MAX_FRAMES_IN_FLIGHT),
            vk::DescriptorPoolSize(vk::DescriptorType::eUniformBuffer, MAX_FRAMES_IN_FLIGHT),
            vk::DescriptorPoolSize(vk::DescriptorType::eCombinedImageSampler,
                                   MAX_FRAMES_IN_FLIGHT * vksEngine::SUPPORTED_TEXTURE_TYPES_COUNT)
        };

        vk::DescriptorPoolCreateInfo poolInfo(
            vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet,
            MAX_FRAMES_IN_FLIGHT,
            poolSize
        );

        m_ComplexMeshDescriptorPool = vk::raii::DescriptorPool(m_Device, poolInfo);
    }

    void VulkanHandler::createSimpleMeshDescriptorPool()
    {
        std::array<vk::DescriptorPoolSize, 4> poolSize = {
            vk::DescriptorPoolSize(vk::DescriptorType::eUniformBuffer, MAX_FRAMES_IN_FLIGHT),
            vk::DescriptorPoolSize(vk::DescriptorType::eUniformBuffer, MAX_FRAMES_IN_FLIGHT),
            vk::DescriptorPoolSize(vk::DescriptorType::eUniformBuffer, MAX_FRAMES_IN_FLIGHT),
            vk::DescriptorPoolSize(vk::DescriptorType::eUniformBuffer, MAX_FRAMES_IN_FLIGHT)
        };

        vk::DescriptorPoolCreateInfo poolInfo(
            vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet,
            MAX_FRAMES_IN_FLIGHT,
            poolSize
        );

        m_SimpleMeshDescriptorPool = vk::raii::DescriptorPool(m_Device, poolInfo);
    }

    void VulkanHandler::createComplexMeshDescriptorSets(UniformBuffer &mvpUBO, vk::DeviceSize vpSize,
                                                        UniformBuffer &pointLightUBO, vk::DeviceSize plSize,
                                                        UniformBuffer &directionalLightUBO, vk::DeviceSize dlSize,
                                                        UniformBuffer &countersUBO, vk::DeviceSize ctSize)
    {
        std::vector<vk::DescriptorSetLayout> layouts(
            MAX_FRAMES_IN_FLIGHT,
            *m_ComplexMeshDescriptorSetLayout
        );

        vk::DescriptorSetAllocateInfo allocInfo(
            m_ComplexMeshDescriptorPool,
            layouts.size(),
            layouts.data()
        );

        m_ComplexMeshDescriptorSets.clear();

        m_ComplexMeshDescriptorSets = m_Device.allocateDescriptorSets(allocInfo);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            vk::DescriptorBufferInfo mvpBufferInfo(
                mvpUBO.m_Buffers[i],
                0,
                vpSize
            );

            vk::DescriptorBufferInfo directionalLightBuffer(
                directionalLightUBO.m_Buffers[i],
                0,
                dlSize
            );

            vk::DescriptorBufferInfo pointLightBuffer(
                pointLightUBO.m_Buffers[i],
                0,
                plSize
            );

            vk::DescriptorBufferInfo countersBuffer(
                countersUBO.m_Buffers[i],
                0,
                ctSize
            );


            vk::DescriptorImageInfo diffuseImageInfo(
                m_BackpackDiffuseTexture.m_Sampler,
                m_BackpackDiffuseTexture.m_ImageView,
                vk::ImageLayout::eShaderReadOnlyOptimal
            );

            vk::DescriptorImageInfo specularImageInfo(
                m_BackpackDiffuseTexture.m_Sampler,
                m_BackpackDiffuseTexture.m_ImageView,
                vk::ImageLayout::eShaderReadOnlyOptimal
            );

            vk::DescriptorImageInfo normalImageInfo(
                m_BackpackNormalTexture.m_Sampler,
                m_BackpackNormalTexture.m_ImageView,
                vk::ImageLayout::eShaderReadOnlyOptimal
            );

            std::array<vk::DescriptorImageInfo, 2> imageInfos = {diffuseImageInfo, normalImageInfo};


            std::array<vk::WriteDescriptorSet, 5> descriptorWrites{};

            descriptorWrites[0].dstSet = m_ComplexMeshDescriptorSets[i];
            descriptorWrites[0].dstBinding = 0;
            descriptorWrites[0].dstArrayElement = 0;
            descriptorWrites[0].descriptorType = vk::DescriptorType::eUniformBuffer;
            descriptorWrites[0].descriptorCount = 1;
            descriptorWrites[0].pBufferInfo = &mvpBufferInfo;

            descriptorWrites[1].dstSet = m_ComplexMeshDescriptorSets[i];
            descriptorWrites[1].dstBinding = 1;
            descriptorWrites[1].dstArrayElement = 0;
            descriptorWrites[1].descriptorType = vk::DescriptorType::eUniformBuffer;
            descriptorWrites[1].descriptorCount = 1;
            descriptorWrites[1].pBufferInfo = &directionalLightBuffer;

            descriptorWrites[2].dstSet = m_ComplexMeshDescriptorSets[i];
            descriptorWrites[2].dstBinding = 2;
            descriptorWrites[2].dstArrayElement = 0;
            descriptorWrites[2].descriptorType = vk::DescriptorType::eUniformBuffer;
            descriptorWrites[2].descriptorCount = 1;
            descriptorWrites[2].pBufferInfo = &pointLightBuffer;

            descriptorWrites[3].dstSet = m_ComplexMeshDescriptorSets[i];
            descriptorWrites[3].dstBinding = 3;
            descriptorWrites[3].dstArrayElement = 0;
            descriptorWrites[3].descriptorType = vk::DescriptorType::eUniformBuffer;
            descriptorWrites[3].descriptorCount = 1;
            descriptorWrites[3].pBufferInfo = &countersBuffer;

            descriptorWrites[4].dstSet = m_ComplexMeshDescriptorSets[i];
            descriptorWrites[4].dstBinding = 4;
            descriptorWrites[4].dstArrayElement = 0;
            descriptorWrites[4].descriptorType = vk::DescriptorType::eCombinedImageSampler;
            descriptorWrites[4].descriptorCount = 2;
            descriptorWrites[4].pImageInfo = imageInfos.data();

            m_Device.updateDescriptorSets(descriptorWrites, {});
        }
    }

    void VulkanHandler::createSimpleMeshDescriptorSets(UniformBuffer &mvpUBO, vk::DeviceSize vpSize,
                                                       UniformBuffer &pointLightUBO, vk::DeviceSize plSize,
                                                       UniformBuffer &directionalLightUBO, vk::DeviceSize dlSize,
                                                       UniformBuffer &countersUBO, vk::DeviceSize ctSize)
    {
        std::vector<vk::DescriptorSetLayout> layouts(
            MAX_FRAMES_IN_FLIGHT,
            *m_SimpleMeshDescriptorSetLayout
        );

        vk::DescriptorSetAllocateInfo allocInfo(
            m_SimpleMeshDescriptorPool,
            layouts.size(),
            layouts.data()
        );

        m_SimpleMeshDescriptorSets.clear();

        m_SimpleMeshDescriptorSets = m_Device.allocateDescriptorSets(allocInfo);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            vk::DescriptorBufferInfo mvpBufferInfo(
                mvpUBO.m_Buffers[i],
                0,
                vpSize
            );

            vk::DescriptorBufferInfo directionalLightBuffer(
                directionalLightUBO.m_Buffers[i],
                0,
                dlSize
            );

            vk::DescriptorBufferInfo pointLightBuffer(
                pointLightUBO.m_Buffers[i],
                0,
                plSize
            );

            vk::DescriptorBufferInfo countersBuffer(
                countersUBO.m_Buffers[i],
                0,
                ctSize
            );


            std::array<vk::WriteDescriptorSet, 4> descriptorWrites{};

            descriptorWrites[0].dstSet = m_SimpleMeshDescriptorSets[i];
            descriptorWrites[0].dstBinding = 0;
            descriptorWrites[0].dstArrayElement = 0;
            descriptorWrites[0].descriptorType = vk::DescriptorType::eUniformBuffer;
            descriptorWrites[0].descriptorCount = 1;
            descriptorWrites[0].pBufferInfo = &mvpBufferInfo;

            descriptorWrites[1].dstSet = m_SimpleMeshDescriptorSets[i];
            descriptorWrites[1].dstBinding = 1;
            descriptorWrites[1].dstArrayElement = 0;
            descriptorWrites[1].descriptorType = vk::DescriptorType::eUniformBuffer;
            descriptorWrites[1].descriptorCount = 1;
            descriptorWrites[1].pBufferInfo = &directionalLightBuffer;

            descriptorWrites[2].dstSet = m_SimpleMeshDescriptorSets[i];
            descriptorWrites[2].dstBinding = 2;
            descriptorWrites[2].dstArrayElement = 0;
            descriptorWrites[2].descriptorType = vk::DescriptorType::eUniformBuffer;
            descriptorWrites[2].descriptorCount = 1;
            descriptorWrites[2].pBufferInfo = &pointLightBuffer;

            descriptorWrites[3].dstSet = m_SimpleMeshDescriptorSets[i];
            descriptorWrites[3].dstBinding = 3;
            descriptorWrites[3].dstArrayElement = 0;
            descriptorWrites[3].descriptorType = vk::DescriptorType::eUniformBuffer;
            descriptorWrites[3].descriptorCount = 1;
            descriptorWrites[3].pBufferInfo = &countersBuffer;

            m_Device.updateDescriptorSets(descriptorWrites, {});
        }
    }

    void VulkanHandler::createTextureImage(Texture &texture, uint8_t *pixels)
    {
        vk::raii::Buffer stagingBuffer({});

        vk::raii::DeviceMemory stagingBufferMemory({});

        createBuffer(texture.m_ImageSize,
                     vk::BufferUsageFlagBits::eTransferSrc,
                     vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                     stagingBuffer,
                     stagingBufferMemory
        );

        void *data = stagingBufferMemory.mapMemory(0, texture.m_ImageSize);

        memcpy(data, pixels, texture.m_ImageSize);

        stagingBufferMemory.unmapMemory();

        stbi_image_free(pixels);

        createImage(texture.m_Width, texture.m_Height, texture.m_MipLevels,
                    vk::Format::eR8G8B8A8Srgb,
                    vk::SampleCountFlagBits::e1,
                    vk::ImageTiling::eOptimal,
                    vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
                    vk::MemoryPropertyFlagBits::eDeviceLocal,
                    texture.m_Image,
                    texture.m_Memory
        );

        transitionImageLayout(texture.m_Image,
                              vk::ImageLayout::eUndefined,
                              vk::ImageLayout::eTransferDstOptimal,
                              texture.m_MipLevels
        );

        GenerateMipMaps(texture, vk::Format::eR8G8B8A8Srgb);

        copyBufferToImage(stagingBuffer, texture.m_Image, texture.m_Width, texture.m_Height);

        transitionImageLayout(texture.m_Image,
                              vk::ImageLayout::eTransferDstOptimal,
                              vk::ImageLayout::eShaderReadOnlyOptimal,
                              texture.m_MipLevels
        );
    }

    void VulkanHandler::createTextureImageView(Texture &texture)
    {
        texture.m_ImageView = createImageView(texture.m_Image, vk::Format::eR8G8B8A8Srgb,
                                              vk::ImageAspectFlagBits::eColor,
                                              texture.m_MipLevels);
    }

    void VulkanHandler::createTextureSampler(Texture &texture)
    {
        auto properties = m_PhysicalDevice.getProperties();

        vk::SamplerCreateInfo samplerInfo({},
                                          vk::Filter::eLinear,
                                          vk::Filter::eLinear,
                                          vk::SamplerMipmapMode::eLinear,
                                          vk::SamplerAddressMode::eRepeat,
                                          vk::SamplerAddressMode::eRepeat,
                                          vk::SamplerAddressMode::eRepeat,
                                          0, 1,
                                          properties.limits.maxSamplerAnisotropy,
                                          vk::False,
                                          vk::CompareOp::eAlways
        );

        samplerInfo.borderColor = vk::BorderColor::eIntOpaqueBlack;
        samplerInfo.unnormalizedCoordinates = vk::False;
        samplerInfo.compareEnable = vk::False;
        samplerInfo.compareOp = vk::CompareOp::eAlways;
        samplerInfo.mipmapMode = vk::SamplerMipmapMode::eLinear;
        samplerInfo.mipLodBias = 0.0f;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = 0.0f;

        texture.m_Sampler = vk::raii::Sampler(m_Device, samplerInfo);
    }

    void VulkanHandler::createDepthResources()
    {
        auto depthFormat = findDepthFormat();

        createImage(m_SwapChainExtent.width, m_SwapChainExtent.height, 1,
                    depthFormat,
                    m_MsaaSamples,
                    vk::ImageTiling::eOptimal,
                    vk::ImageUsageFlagBits::eDepthStencilAttachment,
                    vk::MemoryPropertyFlagBits::eDeviceLocal,
                    m_DepthImage,
                    m_DepthImageMemory
        );

        m_DepthImageView = createImageView(m_DepthImage, depthFormat, vk::ImageAspectFlagBits::eDepth, 1);
    }

    vk::Format VulkanHandler::findSupportedFormat(const std::vector<vk::Format> &candidates, vk::ImageTiling tiling,
                                                  vk::FormatFeatureFlags features)
    {
        for (const auto &format: candidates)
        {
            vk::FormatProperties props = m_PhysicalDevice.getFormatProperties(format);

            if (tiling == vk::ImageTiling::eLinear &&
                (props.linearTilingFeatures & features) == features)
                return format;

            if (tiling == vk::ImageTiling::eOptimal &&
                (props.optimalTilingFeatures & features) == features)
                return format;
        }

        throw std::runtime_error("Failed to find suitable format");
    }

    vk::Format VulkanHandler::findDepthFormat()
    {
        return findSupportedFormat(
            {vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint},
            vk::ImageTiling::eOptimal,
            vk::FormatFeatureFlagBits::eDepthStencilAttachment
        );
    }

    bool VulkanHandler::hasStencilComponent(vk::Format format)
    {
        return format == vk::Format::eD32SfloatS8Uint ||
               format == vk::Format::eD24UnormS8Uint;
    }

    void VulkanHandler::createImage(uint32_t width, uint32_t height, uint32_t mipLevels, vk::Format format,
                                    vk::SampleCountFlagBits numSamples,
                                    vk::ImageTiling tiling,
                                    vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties,
                                    vk::raii::Image &image,
                                    vk::raii::DeviceMemory &imageMemory)
    {
        vk::ImageCreateInfo imageInfo(
            {},
            vk::ImageType::e2D,
            format,
            {width, height, 1},
            mipLevels
            , 1,
            numSamples,
            tiling,
            usage,
            vk::SharingMode::eExclusive
        );

        image = vk::raii::Image(m_Device, imageInfo);

        auto memReq = image.getMemoryRequirements();

        vk::MemoryAllocateInfo allocInfo(
            memReq.size,
            findMemoryType(memReq.memoryTypeBits, properties)
        );

        imageMemory = vk::raii::DeviceMemory(m_Device, allocInfo);

        image.bindMemory(imageMemory, 0);
    }

    vk::raii::ImageView VulkanHandler::createImageView(vk::raii::Image &image, vk::Format format,
                                                       vk::ImageAspectFlags aspectFlags,
                                                       uint32_t mipLevels
    )
    {
        vk::ImageViewCreateInfo viewInfo(
            {},
            image,
            vk::ImageViewType::e2D,
            format,
            {},
            {vk::ImageAspectFlagBits::eColor, 0, mipLevels, 0, 1}
        );

        viewInfo.subresourceRange.aspectMask = aspectFlags;

        return vk::raii::ImageView(m_Device, viewInfo);
    }

    void VulkanHandler::createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage,
                                     vk::MemoryPropertyFlags properties,
                                     vk::raii::Buffer &buffer, vk::raii::DeviceMemory &bufferMemory)
    {
        vk::BufferCreateInfo bufferInfo({},
                                        size, usage,
                                        vk::SharingMode::eExclusive
        );

        buffer = vk::raii::Buffer(m_Device, bufferInfo);

        vk::MemoryRequirements memRequirements = buffer.getMemoryRequirements();

        vk::MemoryAllocateInfo allocInfo(memRequirements.size,
                                         findMemoryType(memRequirements.memoryTypeBits, properties)
        );

        bufferMemory = vk::raii::DeviceMemory(m_Device, allocInfo);

        buffer.bindMemory(*bufferMemory, 0);
    }

    void VulkanHandler::copyBuffer(vk::raii::Buffer &srcBuffer, vk::raii::Buffer &dstBuffer, vk::DeviceSize size)
    {
        vk::CommandBufferAllocateInfo allocInfo{
            m_CommandPool,
            vk::CommandBufferLevel::ePrimary,
            1
        };

        vk::raii::CommandBuffer commandCopyBuffer = std::move(m_Device.allocateCommandBuffers(allocInfo).front());

        commandCopyBuffer.begin(vk::CommandBufferBeginInfo(vk::CommandBufferUsageFlagBits::eOneTimeSubmit));

        commandCopyBuffer.copyBuffer(srcBuffer,
                                     dstBuffer,
                                     vk::BufferCopy(0, 0, size)
        );

        commandCopyBuffer.end();

        vk::SubmitInfo submitInfo;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &*commandCopyBuffer;

        m_GraphicsQueue.submit(submitInfo);

        m_GraphicsQueue.waitIdle();
    }

    void VulkanHandler::renderFrame(vk::raii::CommandBuffer &imguiCMDBuffer,
                                    vk::raii::CommandBuffer &simpleMeshCMDBuffer,
                                    vk::raii::CommandBuffer &complexMeshCMDBuffer)
    {
        m_FramebufferResized = false;

        auto [result, imageIndex] = m_SwapChain.acquireNextImage(
            UINT64_MAX, m_PresentCompleteSemaphores[m_SemaphoreIndex]);

        if (result == vk::Result::eErrorOutOfDateKHR)
        {
            m_FramebufferResized = true;
            return;
        }

        if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR)
            throw std::runtime_error("Failed to acquire swap chain image");

        m_Device.resetFences(*m_InFlightFences[m_CurrentFrame]);


        m_CommandBuffers[m_CurrentFrame].reset();

        recordCommandBuffer(imageIndex, imguiCMDBuffer, simpleMeshCMDBuffer, complexMeshCMDBuffer);

        vk::PipelineStageFlags waitDestinationStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);

        const vk::SubmitInfo submitInfo(1, &*m_PresentCompleteSemaphores[m_SemaphoreIndex],
                                        &waitDestinationStageMask,
                                        1, &*m_CommandBuffers[m_CurrentFrame],
                                        1, &*m_RenderFinishedSemaphores[m_SemaphoreIndex]
        );

        m_GraphicsQueue.submit(submitInfo, *m_InFlightFences[m_CurrentFrame]);

        const vk::PresentInfoKHR presentInfo(1, &*m_RenderFinishedSemaphores[m_SemaphoreIndex],
                                             1, &*m_SwapChain,
                                             &imageIndex
        );

        try
        {
            result = m_GraphicsQueue.presentKHR(presentInfo);
        } catch (const vk::OutOfDateKHRError &e)
        {
            result = vk::Result::eErrorOutOfDateKHR;
        }

        if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR || m_FramebufferResized)
            m_FramebufferResized = true;
        else if (result != vk::Result::eSuccess)
            throw std::runtime_error("Failed to present swap chain image");

        m_SemaphoreIndex = (m_SemaphoreIndex + 1) % m_PresentCompleteSemaphores.size();

        m_CurrentFrame = (m_CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    VulkanHandler::~VulkanHandler()
    {
        m_Device.waitIdle();

        cleanupSwapChain();
    }

    vk::Bool32 VulkanHandler::debugCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
                                            vk::DebugUtilsMessageTypeFlagsEXT type,
                                            const vk::DebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUser)
    {
        std::print("Validation layer: type {} msg {}\n", to_string(type), pCallbackData->pMessage);

        return VK_FALSE;
    }

    std::vector<char> VulkanHandler::readFile(const std::string &path)
    {
        std::ifstream file(path, std::ios::ate | std::ios::binary);

        if (!file.is_open())
            throw std::runtime_error("Failed to open shader file");

        size_t fileSize = file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;
    }

    vk::raii::ShaderModule VulkanHandler::createShaderModule(const std::vector<char> &code,
                                                             const vk::raii::Device &device)
    {
        vk::ShaderModuleCreateInfo createInfo{};

        createInfo.codeSize = code.size() * sizeof(char);
        createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

        vk::raii::ShaderModule shaderModule{device, createInfo};

        return shaderModule;
    }
}
