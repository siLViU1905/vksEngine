#ifndef TEXTURE_H
#define TEXTURE_H
#include <vulkan/vulkan_raii.hpp>

class Texture
{
public:
    Texture();

    Texture(const std::string& path);

    vk::raii::Image m_Image;

    vk::DeviceSize m_ImageSize;

    int m_Width;

    int m_Height;

    vk::raii::ImageView m_ImageView;

    vk::raii::Sampler m_Sampler;

    vk::raii::DeviceMemory m_Memory;

    uint32_t m_MipLevels;

    uint8_t* load(const std::string& path);

    uint8_t* load();

    void setPath(const std::string& path);

    constexpr const std::string& getPath() const {return m_Path;}
private:
    std::string m_Path;
};


#endif //TEXTURE_H
