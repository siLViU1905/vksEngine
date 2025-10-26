#ifndef VKSENGINEAPPWINDOW_H
#define VKSENGINEAPPWINDOW_H
#include <functional>
#include <string>

#include <GLFW/glfw3.h>

namespace vks_engine
{
    class Window
    {
    public:
        Window(int width, int height, const std::string &title);

        bool isOpen() const;

        void close();

        void pollEvents();

        constexpr GLFWwindow *getWindow() { return m_Window; }

        bool wasResized();

        void maximize();

        bool isMaximized() const;

        void restore();

        void resize(int width, int height);

        constexpr int getHeight() const {return m_Height;}

        constexpr int getWidth() const {return m_Width;}

        void getFramebufferSize(int& width, int& height);

        void waitForEvents();

        void setFramebufferResizeCallback(std::function<void(int, int)> function);

        void setKeyCallback(std::function<void(int, int, int, int)> function);

        void setButtonCallback(std::function<void(int, int, int)> function);

        ~Window();

    private:
        GLFWwindow *m_Window;

        int m_Width;

        int m_Height;

        bool m_FramebufferResized;

        std::function<void(int, int)> m_FramebufferResizeCallbackFunction;

        std::function<void(int, int, int, int)> m_KeyCallbackFunction;

        std::function<void(int, int, int)> m_ButtonCallbackFunction;

        static void glfw_framebufferCallback(GLFWwindow *window, int width, int height);

        static void glfw_keyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

        static void glfw_mouseCallback(GLFWwindow *window, int button, int action, int mods);
    };
}

#endif //VKSENGINEAPPWINDOW_H
