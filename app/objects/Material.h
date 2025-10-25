#ifndef VKSENGINEMATERIAL_H
#define VKSENGINEMATERIAL_H

#include "texture/Texture.h"

namespace vks_engine
{
    class Material
    {
    public:
        Material();

        void setDiffuseTexture(const Texture &texture, bool isDefault);

        void setSpecularTexture(const Texture &texture, bool isDefault);

        void setNormalTexture(const Texture &texture, bool isDefault);

        constexpr const std::pair<const Texture *, bool> &getDiffuseTexture() const { return m_DiffuseTexture; }

        constexpr const std::pair<const Texture *, bool> &getSpecularTexture() const { return m_SpecularTexture; }

        constexpr const std::pair<const Texture *, bool> &getNormalTexture() const { return m_NormalTexture; }

        constexpr const std::pair<const Texture *, bool> &getTexture(TextureType type) const
        {
            switch (type)
            {
                case TextureType::DIFFUSE:
                    return m_DiffuseTexture;
                case TextureType::SPECULAR:
                    return m_SpecularTexture;
                case TextureType::NORMALS:
                    return m_NormalTexture;
                default:
                    return m_DiffuseTexture;
            }
        }

        friend class VulkanHandler;

    private:
        std::pair<const Texture *, bool> m_DiffuseTexture;

        std::pair<const Texture *, bool> m_SpecularTexture;

        std::pair<const Texture *, bool> m_NormalTexture;
    };
}

#endif //VKSENGINEMATERIAL_H
