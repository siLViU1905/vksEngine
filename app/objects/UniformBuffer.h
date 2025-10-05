#ifndef VKSENGINEUNIFORMBUFFER_H
#define VKSENGINEUNIFORMBUFFER_H
#include <vulkan/vulkan_raii.hpp>

namespace vks_engine
{
    class UniformBuffer
    {
    public:
        template<typename Ty>
        void update(const Ty &data, uint32_t frame)
        {
            memcpy(m_MappedMemories[frame], &data, sizeof(data));
        }

        template<typename Ty>
        void update(const Ty *data, uint32_t count, uint32_t frame)
        {
            memcpy(m_MappedMemories[frame], data, sizeof(Ty) * count);
        }

        void cleanUp();

        friend class VulkanHandler;

    private:
        std::vector<vk::raii::Buffer> m_Buffers;

        std::vector<vk::raii::DeviceMemory> m_Memories;

        std::vector<void *> m_MappedMemories;
    };
}

#endif //VKSENGINEUNIFORMBUFFER_H
