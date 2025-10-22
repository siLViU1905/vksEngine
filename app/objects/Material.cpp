#include "Material.h"

namespace vks_engine
{
    void Material::setDiffuseTexture(Texture &texture)
    {
        m_DiffuseTexture = std::move(texture);
    }

    void Material::setSpecularTexture(Texture &texture)
    {
        m_SpecularTexture = std::move(texture);
    }

    void Material::setNormalTexture(Texture &texture)
    {
        m_NormalTexture = std::move(texture);
    }
}
