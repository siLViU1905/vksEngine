#ifndef VKSENGINEMATERIAL_H
#define VKSENGINEMATERIAL_HMATERIAL_H

#include "texture/Texture.h"

namespace vks_engine
{
    class Material
    {
    public:
        void setDiffuseTexture(Texture &texture);

        void setSpecularTexture(Texture &texture);

        void setNormalTexture(Texture &texture);

        constexpr const Texture &getDiffuseTexture() const { return m_DiffuseTexture; }

        constexpr const Texture &getSpecularTexture() const { return m_SpecularTexture; }

        constexpr const Texture &getNormalTexture() const { return m_NormalTexture; }

    private:
        Texture m_DiffuseTexture;

        Texture m_SpecularTexture;

        Texture m_NormalTexture;
    };
}

#endif //VKSENGINEMATERIAL_HMATERIAL_H
