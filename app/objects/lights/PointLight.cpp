#include "PointLight.h"

PointLight::PointLight(): m_Position(0.f), m_Constant(1.f), m_Linear(0.2f), m_Quadratic(0.032f)
{
}

PointLight::PointLight(const glm::vec3 &position, float constant, float linear, float quadratic)
    : m_Position(position), m_Constant(constant), m_Linear(linear), m_Quadratic(quadratic)
{
}

PointLight::PointLight(const glm::vec3 &diffuse, const glm::vec3 &specular, const glm::vec3 &ambient, float intensity,
                       const glm::vec3 &position, float constant, float linear, float quadratic)
    : Light(diffuse, specular, ambient, intensity),
      m_Position(position), m_Constant(constant), m_Linear(linear), m_Quadratic(quadratic)
{
}

void PointLight::setPosition(const glm::vec3 &position)
{
    m_Position = position;
}

void PointLight::setConstant(float constant)
{
    m_Constant = constant;
}

void PointLight::setLinear(float linear)
{
    m_Linear = linear;
}

void PointLight::setQuadratic(float quadratic)
{
    m_Quadratic = quadratic;
}

PointLight::Aligned PointLight::getAligned() const
{
    Aligned aligned;

    aligned.diffuse = m_Diffuse;

    aligned.specular = m_Specular;

    aligned.ambient = m_Ambient;

    aligned.intensity = m_Intensity;

    aligned.position = m_Position;

    aligned.constant = m_Constant;

    aligned.linear = m_Linear;

    aligned.quadratic = m_Quadratic;

    return aligned;
}
