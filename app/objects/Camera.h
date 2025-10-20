#ifndef VKSENGINECAMERA_H
#define VKSENGINECAMERA_H
#include <chrono>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace vks_engine
{
    struct Camera
    {
        Camera() = default;

        Camera(GLFWwindow *window, const glm::vec3 &position, float speed, float sensitivity = 1.f);

        glm::mat4 m_View;

        glm::mat4 m_Projection;

        void updateMovement(float deltaTime);

        void updateLookAt(float deltaTime);

        void updateProjection(int width, int height);

        void gainFocus();

        void loseFocus();

        constexpr bool isFocused() const {return m_Focused;}

        constexpr const glm::vec3 &getPosition() const { return m_Position; }

        struct Aligned
        {
            glm::mat4 view;
            glm::mat4 projection;
            glm::vec3 position;
            float _pad0;
        };

        Aligned getAligned() const;

        void setSensitivity(float sensitivity);

        constexpr float getSensitivity() const {return m_Sensitivity;}

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

        float m_Sensitivity;

        GLFWwindow *m_Window;
    };
}


#endif //VKSENGINECAMERA_H
