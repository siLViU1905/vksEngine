#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace vks_engine
{
    Texture::Texture()
        : m_Image(nullptr), m_ImageSize(0),
          m_Width(0), m_Height(0),
          m_ImageView(nullptr), m_Sampler(nullptr),
          m_Memory(nullptr), m_MipLevels(1),
          m_Type(TextureType::NONE)
    {
    }

    Texture::Texture(const std::string &path)
        : m_Image(nullptr), m_ImageSize(0),
          m_Width(0), m_Height(0),
          m_ImageView(nullptr), m_Sampler(nullptr),
          m_Memory(nullptr), m_Path(path),
          m_MipLevels(1), m_Type(TextureType::NONE)
    {
    }

    Texture::Texture(Texture &&other) noexcept: m_Image(std::move(other.m_Image)),
                                                m_Memory(std::move(other.m_Memory)),
                                                m_ImageView(std::move(other.m_ImageView)),
                                                m_Sampler(std::move(other.m_Sampler)),
                                                m_ImageSize(other.m_ImageSize),
                                                m_Width(other.m_Width), m_Height(other.m_Height),
                                                m_MipLevels(other.m_MipLevels),
                                                m_Path(std::move(other.m_Path)),
                                                m_Type(other.m_Type)
    {
        other.m_Type = TextureType::NONE;

        other.m_Image = nullptr;
        other.m_ImageView = nullptr;
        other.m_Memory = nullptr;
        other.m_Sampler = nullptr;
        other.m_Pixels = nullptr;
    }

    Texture &Texture::operator=(Texture &&other) noexcept
    {
        if (this != &other)
        {
            m_Image = std::move(other.m_Image);
            m_Memory = std::move(other.m_Memory);
            m_ImageView = std::move(other.m_ImageView);
            m_Sampler = std::move(other.m_Sampler);
            m_ImageSize = other.m_ImageSize;
            m_Width = other.m_Width;
            m_Height = other.m_Height;
            m_MipLevels = other.m_MipLevels;
            m_Path = std::move(other.m_Path);
            m_Type = other.m_Type;

            other.m_Type = TextureType::NONE;

            other.m_Image = nullptr;
            other.m_ImageView = nullptr;
            other.m_Memory = nullptr;
            other.m_Sampler = nullptr;
            other.m_Pixels = nullptr;
        }

        return *this;
    }

    void Texture::setPath(const std::string &path)
    {
        m_Path = path;
    }

    void Texture::setType(TextureType type)
    {
        m_Type = type;
    }

    bool Texture::isLoaded() const
    {
        return m_ImageView != nullptr && m_Sampler != nullptr;
    }

    uint8_t *Texture::load(const std::string &path)
    {
        m_Path = path;

        int channels;

        m_Pixels = stbi_load(path.c_str(), &m_Width, &m_Height, &channels, STBI_rgb_alpha);

        m_ImageSize = m_Width * m_Height * 4;

        m_MipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(m_Width, m_Height)))) + 1;

        return m_Pixels;
    }

    uint8_t *Texture::load()
    {
        int channels;

        m_Pixels = stbi_load(m_Path.c_str(), &m_Width, &m_Height, &channels, STBI_rgb_alpha);

        m_ImageSize = m_Width * m_Height * 4;

        m_MipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(m_Width, m_Height)))) + 1;

        return m_Pixels;
    }
}
