#ifndef VKSENGINEAPPWINDOW_H
#define VKSENGINEAPPWINDOW_H
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

        ~Window();

    private:
        GLFWwindow *m_Window;

        int m_Width;

        int m_Height;

        bool m_FramebufferResized;

        static void framebufferCallback(GLFWwindow *window, int width, int height);
    };
}

#endif //VKSENGINEAPPWINDOW_H
