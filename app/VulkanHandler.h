#ifndef APPVULKANHANDLER_HPP
#define APPVULKANHANDLER_HPP

#define NOMINMAX

#include <vulkan/vulkan_raii.hpp>

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "objects/Camera.h"
#include "objects/lights/DirectionalLight.h"
#include "objects/Mesh.h"
#include "objects/Texture.h"
#include "objects/Vertex.h"
#include "objects/UniformBuffer.h"
#include "objects/lights/PointLight.h"

class ImGuiHandler;

class VulkanHandler
{
public:
    VulkanHandler();

    void init(GLFWwindow *window);

    void renderFrame(vk::raii::CommandBuffer &imguiCMDBuffer, vk::raii::CommandBuffer &meshCMDBuffer);

    void notifyFramebufferResized();

    void CreateUniformBuffers(UniformBuffer &buffer, vk::DeviceSize size);

    void CreateCommandBuffers(std::vector<vk::raii::CommandBuffer> &commandBuffers, vk::CommandBufferLevel level);

    void CreateVertexBuffer(const std::vector<Vertex> &vertices, vk::raii::Buffer &buffer,
                            vk::raii::DeviceMemory &bufferMemory);

    void CreateIndexBuffer(const std::vector<uint32_t> &indices, vk::raii::Buffer &buffer,
                           vk::raii::DeviceMemory &bufferMemory);

    void GenerateMipMaps(Texture &texture, vk::Format imageFormat);

    void CreateTexture(Texture &texture);

    void waitIdle() const;

    ~VulkanHandler();

    friend class ImGuiHandler;

    friend class Scene;

private:
    void createInstance();

    std::vector<const char *> getRequiredExtensions();

    void setupDebugMessenger();

    void pickPhysicalDevice();

    void createLogicalDevice();

    void createSurface(GLFWwindow *window);

    vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &availableFormats);

    vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR> &availablePresentModes);

    vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR &capabilities, GLFWwindow *window);

    void createSwapChain(GLFWwindow *window);

    void createImageViews();

    void createComplexMeshGraphicsPipeline();

    void createSimpleMeshGraphicsPipeline();

    void createCommandPool();

    void createCommandBuffers();

    void createSyncObjects();

    void createVertexBuffer(const std::vector<Vertex> &vertices);

    void createIndexBuffer(const std::vector<uint32_t> &indices);

    void createComplexMeshDescriptorSetLayout();

    void createSimpleMeshDescriptorSetLayout();

    void createComplexMeshDescriptorPool();

    void createSimpleMeshDescriptorPool();

    void createComplexMeshDescriptorSets(UniformBuffer &mvpUBO, vk::DeviceSize mvpSize,
                                         UniformBuffer &pointLightUBO, vk::DeviceSize plSize,
                                         UniformBuffer &directionalLightUBO, vk::DeviceSize dlSize,
                                         UniformBuffer &countersUBO, vk::DeviceSize ctSize);

    void createSimpleMeshDescriptorSets(UniformBuffer &mvpUBO, vk::DeviceSize mvpSize,
                                        UniformBuffer &pointLightUBO, vk::DeviceSize plSize,
                                        UniformBuffer &directionalLightUBO, vk::DeviceSize dlSize,
                                        UniformBuffer &countersUBO, vk::DeviceSize ctSize);

    void createTextureImage(Texture &texture);

    void createTextureImageView(Texture &texture);

    void createTextureSampler(Texture &texture);

    void createDepthResources();

    vk::Format findSupportedFormat(const std::vector<vk::Format> &candidates,
                                   vk::ImageTiling tiling,
                                   vk::FormatFeatureFlags features);

    vk::Format findDepthFormat();

    bool hasStencilComponent(vk::Format format);

    void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, vk::Format format,
                     vk::SampleCountFlagBits numSamples, vk::ImageTiling tiling,
                     vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties, vk::raii::Image &image,
                     vk::raii::DeviceMemory &imageMemory);

    vk::raii::ImageView createImageView(vk::raii::Image &image,
                                        vk::Format format,
                                        vk::ImageAspectFlags aspectFlags,
                                        uint32_t mipLevels
    );


    void createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties,
                      vk::raii::Buffer &buffer, vk::raii::DeviceMemory &bufferMemory);

    void copyBuffer(vk::raii::Buffer &srcBuffer, vk::raii::Buffer &dstBuffer, vk::DeviceSize size);

    void recordCommandBuffer(uint32_t imageIndex, vk::raii::CommandBuffer &imguiCMDBuffer,
                             vk::raii::CommandBuffer &meshCMDBuffer);

    void transitionImageLayout(
        uint32_t imageIndex,
        vk::ImageLayout oldLayout,
        vk::ImageLayout newLayout,
        vk::AccessFlags2 srcAccessMask,
        vk::AccessFlags2 dstAccessMask,
        vk::PipelineStageFlags2 srcStageMask,
        vk::PipelineStageFlags2 dstStageMask
    );

    void transitionImageLayout(const vk::raii::Image &image,
                               vk::ImageLayout oldLayout,
                               vk::ImageLayout newLayout,
                               uint32_t mipLevels
    );

    void copyBufferToImage(const vk::raii::Buffer &buffer,
                           vk::raii::Image &image,
                           uint32_t width, uint32_t height);

    std::unique_ptr<vk::raii::CommandBuffer> beginSingleTimeCommands();

    void endSingleTimeCommands(vk::raii::CommandBuffer &commandBuffer);

    void cleanupSwapChain();

    void rebuildSwapChain();

    vk::SampleCountFlagBits getMaxUsableSampleCount();

    void createColorResources();

    uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties);

    static VKAPI_ATTR vk::Bool32 VKAPI_CALL debugCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
                                                          vk::DebugUtilsMessageTypeFlagsEXT type,
                                                          const vk::DebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                                          void *pUser);

    static std::vector<char> readFile(const std::string &path);

    static vk::raii::ShaderModule createShaderModule(const std::vector<char> &code, const vk::raii::Device &device);

    vk::raii::Context m_Context;

    vk::raii::Instance m_Instance;

    vk::raii::DebugUtilsMessengerEXT m_DebugMessenger;

    vk::raii::PhysicalDevice m_PhysicalDevice;

    vk::raii::Device m_Device;

    vk::raii::Queue m_GraphicsQueue;

    vk::raii::SurfaceKHR m_Surface;

    vk::SurfaceFormatKHR m_SwapChainSurfaceFormat;

    vk::Extent2D m_SwapChainExtent;

    vk::raii::SwapchainKHR m_SwapChain;

    std::vector<vk::Image> m_SwapChainImages;

    vk::Format m_SwapChainImageFormat;

    std::vector<vk::raii::ImageView> m_SwapChainImageViews;

    vk::Viewport m_Viewport;

    vk::Rect2D m_Scissor;

    vk::raii::DescriptorSetLayout m_ComplexMeshDescriptorSetLayout;

    vk::raii::PipelineLayout m_ComplexMeshPipelineLayout;

    vk::raii::Pipeline m_ComplexMeshGraphicsPipeline;

    vk::raii::DescriptorSetLayout m_SimpleMeshDescriptorSetLayout;

    vk::raii::PipelineLayout m_SimpleMeshPipelineLayout;

    vk::raii::Pipeline m_SimpleMeshGraphicsPipeline;

    vk::raii::CommandPool m_CommandPool;

    std::vector<vk::raii::CommandBuffer> m_CommandBuffers;

    std::vector<vk::raii::Semaphore> m_PresentCompleteSemaphores;

    std::vector<vk::raii::Semaphore> m_RenderFinishedSemaphores;

    std::vector<vk::raii::Fence> m_InFlightFences;

    vk::raii::Buffer m_VertexBuffer;

    vk::raii::DeviceMemory m_VertexBufferMemory;

    vk::raii::Buffer m_IndexBuffer;

    vk::raii::DeviceMemory m_IndexBufferMemory;

    uint32_t m_CurrentFrame;

    uint32_t m_SemaphoreIndex;

    uint32_t m_GraphicsQueueFamilyIndex;

    vk::raii::DescriptorPool m_ComplexMeshDescriptorPool;

    vk::raii::DescriptorPool m_SimpleMeshDescriptorPool;

    std::vector<vk::raii::DescriptorSet> m_ComplexMeshDescriptorSets;

    std::vector<vk::raii::DescriptorSet> m_SimpleMeshDescriptorSets;

    Texture m_BackpackDiffuseTexture;

    Texture m_BackpackNormalTexture;

    vk::raii::Image m_DepthImage;

    vk::raii::DeviceMemory m_DepthImageMemory;

    vk::raii::ImageView m_DepthImageView;

    vk::raii::Image m_ColorImage;

    vk::raii::DeviceMemory m_ColorImageMemory;

    vk::raii::ImageView m_ColorImageView;

    vk::SampleCountFlagBits m_MsaaSamples;

    static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

    const std::vector<Vertex> vertices = {
        {{-0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}, {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},

        {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},

        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},


        {{0.5f, -0.5f, 0.5f}, {1.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
        {{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},


        {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
        {{0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},


        {{-0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {1.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, -0.5f}, {1.0f, 0.0f, 1.0f}, {0.0f, 1.0f}}
    };

    const std::vector<uint32_t> indices = {
        0, 1, 2, 2, 3, 0,

        4, 5, 6, 6, 7, 4,

        8, 9, 10, 10, 11, 8,

        12, 13, 14, 14, 15, 12,

        16, 17, 18, 18, 19, 16,

        20, 21, 22, 22, 23, 20

    };

    const std::vector<const char *> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    const std::vector<const char *> deviceExtensions = {
        vk::KHRSwapchainExtensionName,
        vk::KHRSpirv14ExtensionName,
        vk::KHRSynchronization2ExtensionName,
        vk::KHRCreateRenderpass2ExtensionName
    };

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

    GLFWwindow *m_Window;

    bool m_FramebufferResized;
};
#endif //APPVULKANHANDLER_HPP
