#include "Light.h"

namespace vks_engine
{
    Light::Light()
        : m_Diffuse(0.8f),
          m_Specular(0.8f),
          m_Ambient(0.8f),
          m_Intensity(0.5f),
          m_ID(std::numeric_limits<uint32_t>::max())
    {
    }

    Light::Light(const glm::vec3 &diffuse, const glm::vec3 &specular, const glm::vec3 &ambient, float intensity)
        : m_Diffuse(diffuse),
          m_Specular(specular),
          m_Ambient(ambient),
          m_Intensity(intensity),
          m_ID(std::numeric_limits<uint32_t>::max())
    {
    }

    void Light::setDiffuse(const glm::vec3 &diffuse)
    {
        m_Diffuse = diffuse;
    }

    void Light::setSpecular(const glm::vec3 &specular)
    {
        m_Specular = specular;
    }

    void Light::setAmbient(const glm::vec3 &ambient)
    {
        m_Ambient = ambient;
    }

    void Light::setIntensity(float intensity)
    {
        m_Intensity = intensity;
    }

    void Light::setID(uint32_t id)
    {
        m_ID = id;
    }

    Light::Aligned Light::getAligned() const
    {
        Aligned aligned;

        aligned.diffuse = m_Diffuse;

        aligned.specular = m_Specular;

        aligned.ambient = m_Ambient;

        aligned.intensity = m_Intensity;

        return aligned;
    }
}
