#ifndef IMGUIHANDER_H
#define IMGUIHANDER_H

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_vulkan.h>

#include "VulkanHandler.h"


class ImGuiHandler
{
public:
    ImGuiHandler();

    void init(VulkanHandler& vkHandler, GLFWwindow* window);

    void beginFrame();

    void endFrame();

    void renderDockspace();

    void recordCommandBuffer(uint32_t currentFrame);

    constexpr vk::raii::CommandBuffer &getCommandBuffer(uint32_t currentFrame) {return m_CommandBuffers[currentFrame];}

    void updateSwapChain(VulkanHandler& vkHandler);

    ~ImGuiHandler();

    friend class Scene;
private:
    vk::raii::DescriptorPool m_DescriptorPool;

    ImGuiContext *m_Context;

    vk::raii::CommandPool m_CommandPool;

    std::vector<vk::raii::CommandBuffer> m_CommandBuffers;

    vk::Format m_ColorAttachmentFormat;

    vk::Format m_DepthAttachmentFormat;

    vk::SampleCountFlagBits m_MsaaSamples;

    void createDescriptorPool(const vk::raii::Device &device);

    void applyTheme();
};


#endif //IMGUIHANDER_H
