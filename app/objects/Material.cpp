#include "Material.h"

namespace vks_engine
{
    Material::Material()
        : m_DiffuseTexture(nullptr),
          m_SpecularTexture(nullptr),
          m_NormalTexture(nullptr)
    {
    }

    void Material::setDiffuseTexture(Texture &texture)
    {
        m_DiffuseTexture = &texture;
    }

    void Material::setSpecularTexture(Texture &texture)
    {
        m_SpecularTexture = &texture;
    }

    void Material::setNormalTexture(Texture &texture)
    {
        m_NormalTexture = &texture;
    }

    void Material::setTexture(Texture &texture, TextureType type)
    {
        switch (type)
        {
            case TextureType::DIFFUSE:
                m_DiffuseTexture = &texture;
                break;
            case TextureType::SPECULAR:
                m_SpecularTexture = &texture;
                break;
            case TextureType::NORMALS:
                m_NormalTexture = &texture;
                break;
            default: ;
        }
    }

    void Material::clear()
    {
        m_DiffuseTexture->clear();

        m_SpecularTexture->clear();

        m_NormalTexture->clear();
    }
}
