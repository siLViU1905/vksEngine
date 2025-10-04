#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(): m_Direction(0.f, -1.f, 0.f), m_NonNormalizedDirection(0.f, -1.f, 0.f)
{
    m_Direction = glm::normalize(-m_Direction);
}

DirectionalLight::DirectionalLight(const glm::vec3 &direction): Light()
{
    m_Direction = glm::normalize(-direction);

    m_NonNormalizedDirection = direction;
}

DirectionalLight::DirectionalLight(const glm::vec3 &diffuse, const glm::vec3 &specular, const glm::vec3 &ambient,
                                   float intensity, const glm::vec3 &direction): Light(
    diffuse, specular, ambient, intensity)
{
    m_Direction = glm::normalize(-direction);

    m_NonNormalizedDirection = direction;
}

void DirectionalLight::setDirection(const glm::vec3 &direction)
{
    m_Direction = glm::normalize(-direction);

    m_NonNormalizedDirection = direction;
}

DirectionalLight::Aligned DirectionalLight::getAligned() const
{
    Aligned aligned;

    aligned.diffuse = m_Diffuse;

    aligned.specular = m_Specular;

    aligned.ambient = m_Ambient;

    aligned.intensity = m_Intensity;

    aligned.direction = m_Direction;

    return aligned;
}
