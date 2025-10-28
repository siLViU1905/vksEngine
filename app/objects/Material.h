#ifndef VKSENGINEMATERIAL_H
#define VKSENGINEMATERIAL_H

#include "texture/Texture.h"

namespace vks_engine
{
    class Material
    {
    public:
        Material();

        void setDiffuseTexture(Texture &texture);

        void setSpecularTexture(Texture &texture);

        void setNormalTexture(Texture &texture);

        void setTexture(Texture &texture, TextureType type);

        constexpr Texture &getDiffuseTexture() { return *m_DiffuseTexture; }

        constexpr const Texture &getDiffuseTexture() const { return *m_DiffuseTexture; }

        constexpr Texture &getSpecularTexture() { return *m_SpecularTexture; }

        constexpr const Texture &getSpecularTexture() const { return *m_SpecularTexture; }

        constexpr Texture &getNormalTexture() { return *m_NormalTexture; }

        constexpr const Texture &getNormalTexture() const { return *m_NormalTexture; }

        constexpr Texture &getTexture(TextureType type)
        {
            switch (type)
            {
                case TextureType::DIFFUSE:
                    return *m_DiffuseTexture;
                case TextureType::SPECULAR:
                    return *m_SpecularTexture;
                case TextureType::NORMALS:
                    return *m_NormalTexture;
                default:
                    return *m_DiffuseTexture;
            }
        }

        constexpr const Texture &getTexture(TextureType type) const
        {
            switch (type)
            {
                case TextureType::DIFFUSE:
                    return *m_DiffuseTexture;
                case TextureType::SPECULAR:
                    return *m_SpecularTexture;
                case TextureType::NORMALS:
                    return *m_NormalTexture;
                default:
                    return *m_DiffuseTexture;
            }
        }

        friend class VulkanHandler;

    private:
        Texture *m_DiffuseTexture;

        Texture *m_SpecularTexture;

        Texture *m_NormalTexture;
    };
}

#endif //VKSENGINEMATERIAL_H
