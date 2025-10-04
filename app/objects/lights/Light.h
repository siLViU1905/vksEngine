#ifndef LIGHT_H
#define LIGHT_H
#include <glm/glm.hpp>

class Light
{
public:
    Light();

    Light(const glm::vec3 &diffuse, const glm::vec3 &specular, const glm::vec3 &ambient, float intensity);

    constexpr const glm::vec3 &getDiffuse() const { return m_Diffuse; }

    constexpr const glm::vec3 &getSpecular() const { return m_Specular; }

    constexpr const glm::vec3 &getAmbient() const { return m_Ambient; }

    constexpr float getIntensity() const { return m_Intensity; }

    constexpr uint32_t getID() const {return m_ID;}

    void setDiffuse(const glm::vec3 &diffuse);

    void setSpecular(const glm::vec3 &specular);

    void setAmbient(const glm::vec3 &ambient);

    void setIntensity(float intensity);

    void setID(uint32_t id);

    struct Aligned
    {
        glm::vec3 diffuse;

        float _pad1;

        glm::vec3 specular;

        float _pad0;

        glm::vec3 ambient;

        float intensity;
    };

    Aligned getAligned() const;

protected:
    glm::vec3 m_Diffuse;

    glm::vec3 m_Specular;

    glm::vec3 m_Ambient;

    float m_Intensity;

    uint32_t m_ID;
};


#endif //LIGHT_H
