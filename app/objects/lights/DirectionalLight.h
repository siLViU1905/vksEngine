#ifndef VKSENGINE_DIRECTIONALLIGHT_H
#define VKSENGINE_DIRECTIONALLIGHT_H
#include "Light.h"

namespace vks_engine
{
    class DirectionalLight : protected Light
    {
    public:
        DirectionalLight();

        DirectionalLight(const glm::vec3 &direction);

        DirectionalLight(const glm::vec3 &diffuse, const glm::vec3 &specular, const glm::vec3 &ambient, float intensity,
                         const glm::vec3 &direction);

        constexpr const glm::vec3 &getDirection() const { return m_Direction; }

        constexpr const glm::vec3 &getNonNormalizedDirection() const { return m_NonNormalizedDirection; }

        void setDirection(const glm::vec3 &direction);

        using Light::setDiffuse;
        using Light::setSpecular;
        using Light::setAmbient;
        using Light::setIntensity;
        using Light::setID;
        using Light::getDiffuse;
        using Light::getSpecular;
        using Light::getAmbient;
        using Light::getIntensity;
        using Light::getID;

        struct Aligned : Light::Aligned
        {
            glm::vec3 direction;

            float _pad0;
        };

        Aligned getAligned() const;

    private:
        glm::vec3 m_Direction;

        glm::vec3 m_NonNormalizedDirection;
    };
}

#endif //VKSENGINE_DIRECTIONALLIGHT_H
