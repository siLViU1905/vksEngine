#include "Window.h"

#include <stdexcept>

namespace vks_engine
{
    Window::Window(int width, int height, const std::string &title): m_Width(width), m_Height(height),
                                                                     m_FramebufferResized(false)
    {
        if (!glfwInit())
            throw std::runtime_error("Failed to init GLFW");

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        m_Window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

        if (!m_Window)
            throw std::runtime_error("Failed to create GLFW Window");

        glfwSetWindowUserPointer(m_Window, this);

        glfwSetFramebufferSizeCallback(m_Window, glfw_framebufferCallback);

        glfwSetKeyCallback(m_Window, glfw_keyboardCallback);

        glfwSetMouseButtonCallback(m_Window, glfw_mouseCallback);
    }

    bool Window::isOpen() const
    {
        return !glfwWindowShouldClose(m_Window);
    }

    void Window::close()
    {
        glfwSetWindowShouldClose(m_Window, true);
    }

    void Window::pollEvents()
    {
        glfwPollEvents();
    }

    bool Window::wasResized()
    {
        bool temp = m_FramebufferResized;

        m_FramebufferResized = false;

        return temp;
    }

    void Window::setFramebufferResizeCallback(std::function<void(int, int)> function)
    {
        m_FramebufferResizeCallbackFunction = std::move(function);
    }

    void Window::setKeyCallback(std::function<void(int, int, int, int)> function)
    {
        m_KeyCallbackFunction = std::move(function);
    }

    void Window::setButtonCallback(std::function<void(int, int, int)> function)
    {
        m_ButtonCallbackFunction = std::move(function);
    }

    void Window::glfw_framebufferCallback(GLFWwindow *window, int width, int height)
    {
        auto *self = static_cast<Window *>(glfwGetWindowUserPointer(window));

        self->m_FramebufferResized = true;

        self->m_Width = width;

        self->m_Height = height;
    }

    void Window::glfw_keyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        auto *self = static_cast<Window *>(glfwGetWindowUserPointer(window));

        self->m_KeyCallbackFunction(key, scancode, action, mods);
    }

    void Window::glfw_mouseCallback(GLFWwindow *window, int button, int action, int mods)
    {
        auto *self = static_cast<Window *>(glfwGetWindowUserPointer(window));

        self->m_ButtonCallbackFunction(button, action, mods);
    }

    Window::~Window()
    {
        glfwDestroyWindow(m_Window);

        glfwTerminate();
    }
}
