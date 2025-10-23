#include "Material.h"

namespace vks_engine
{
    Material::Material()
        : m_DiffuseTexture(nullptr, true),
          m_SpecularTexture(nullptr, true),
          m_NormalTexture(nullptr, true)
    {}

    void Material::setDiffuseTexture(const Texture &texture, bool isDefault)
    {
        m_DiffuseTexture.first = &texture;
        m_DiffuseTexture.second = isDefault;
    }

    void Material::setSpecularTexture(const Texture &texture, bool isDefault)
    {
        m_SpecularTexture.first = &texture;
        m_SpecularTexture.second = isDefault;
    }

    void Material::setNormalTexture(const Texture &texture, bool isDefault)
    {
        m_NormalTexture.first = &texture;
        m_NormalTexture.second = isDefault;
    }
}
