#ifndef VKSENGINE_TEXTURE_H
#define VKSENGINE_TEXTURE_H
#include <vulkan/vulkan_raii.hpp>
#include "TextureType.h"
#include "TextureOrigin.h"

namespace vks_engine
{
    class Texture
    {
    public:
        Texture();

        Texture(const std::string &path);

        Texture(Texture&& other) noexcept;

        Texture& operator=(Texture&& other) noexcept;

        constexpr bool operator==(const Texture & other) const
        {
            return m_Path == other.m_Path;
        }

        vk::raii::Image m_Image;

        vk::DeviceSize m_ImageSize;

        int m_Width;

        int m_Height;

        vk::raii::ImageView m_ImageView;

        vk::raii::Sampler m_Sampler;

        vk::raii::DeviceMemory m_Memory;

        uint32_t m_MipLevels;

        TextureType m_Type;

        uint8_t* m_Pixels;

        TextureOrigin m_Origin;

        uint8_t *load(const std::string &path);

        uint8_t *load();

        void setPath(const std::string &path);

        constexpr const std::string &getPath() const { return m_Path; }

        void setType(TextureType type);

        constexpr TextureType getType() const {return m_Type;}

        bool isLoaded() const;

        constexpr const uint8_t* getPixels() const {return m_Pixels;}

        void clear();

        ~Texture();
    private:
        std::string m_Path;
    };
}

#endif //VKSENGINE_TEXTURE_H
