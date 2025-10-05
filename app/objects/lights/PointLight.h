#ifndef VKSENGINEPOINTLIGHT_H
#define VKSENGINEPOINTLIGHT_H
#include "Light.h"

namespace vks_engine
{
    class PointLight : protected Light
    {
    public:
        PointLight();

        PointLight(const glm::vec3& position, float constant, float linear, float quadratic);

        PointLight(const glm::vec3 &diffuse, const glm::vec3 &specular, const glm::vec3 &ambient, float intensity,
            const glm::vec3& position, float constant, float linear, float quadratic);

        constexpr const glm::vec3& getPosition() const {return m_Position;}

        constexpr float getConstant() const {return m_Constant;}

        constexpr float getLinear() const {return m_Linear;}

        constexpr float getQuadratic() const {return m_Quadratic;}

        void setPosition(const glm::vec3& position);

        void setConstant(float constant);

        void setLinear(float linear);

        void setQuadratic(float quadratic);

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
            glm::vec3 position;

            float constant;

            float linear;

            float quadratic;

            float _pad0;

            float _pad1;
        };

        Aligned getAligned() const;

    private:
        glm::vec3 m_Position;

        float m_Constant;

        float m_Linear;

        float m_Quadratic;
    };
}


#endif //VKSENGINEPOINTLIGHT_H
