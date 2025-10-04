#ifndef CAMERA_H
#define CAMERA_H
#include <chrono>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

struct Camera
{
    Camera() = default;

    Camera(GLFWwindow* window, const glm::vec3& position, float speed);

    glm::mat4 m_View;

    glm::mat4 m_Projection;

    void updateMovement(float deltaTime);

    void updateLookAt(float deltaTime);

    void updateProjection();

    void gainFocus();

    void loseFocus();

    constexpr const glm::vec3& getPosition() const {return m_Position;}

    struct Aligned
    {
        glm::mat4 view;
        glm::mat4 projection;
        glm::vec3 position;
        float _pad0;
    };

    Aligned getAligned() const;

private:
    glm::vec3 m_Position;

    glm::vec3 m_Forward;

    glm::vec3 m_Right;

    glm::vec3 m_Up;

    float m_Yaw;

    float m_Pitch;

    float m_Speed;

    double m_LastX;

    double m_LastY;

    bool m_Focused;

    GLFWwindow* m_Window;
};



#endif //CAMERA_H
