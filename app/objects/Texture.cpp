#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace vks_engine
{
    Texture::Texture()
        : m_Image(nullptr), m_ImageSize(0),
          m_Width(0), m_Height(0),
          m_ImageView(nullptr), m_Sampler(nullptr),
          m_Memory(nullptr), m_MipLevels(1)
    {
    }

    Texture::Texture(const std::string &path)
        : m_Image(nullptr), m_ImageSize(0),
          m_Width(0), m_Height(0),
          m_ImageView(nullptr), m_Sampler(nullptr),
          m_Memory(nullptr), m_Path(path),
          m_MipLevels(1)
    {
    }

    void Texture::setPath(const std::string &path)
    {
        m_Path = path;
    }

    uint8_t *Texture::load(const std::string &path)
    {
        m_Path = path;

        int channels;

        uint8_t *pixels = stbi_load(path.c_str(), &m_Width, &m_Height, &channels, STBI_rgb_alpha);

        if (!pixels)
            throw std::runtime_error("File not found");

        m_ImageSize = m_Width * m_Height * 4;

        m_MipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(m_Width, m_Height)))) + 1;

        return pixels;
    }

    uint8_t *Texture::load()
    {
        int channels;

        uint8_t *pixels = stbi_load(m_Path.c_str(), &m_Width, &m_Height, &channels, STBI_rgb_alpha);

        if (!pixels)
            throw std::runtime_error("File not found");

        m_ImageSize = m_Width * m_Height * 4;

        m_MipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(m_Width, m_Height)))) + 1;

        return pixels;
    }
}
