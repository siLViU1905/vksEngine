#include "Window.h"

#include <stdexcept>

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

    glfwSetFramebufferSizeCallback(m_Window, framebufferCallback);
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

void Window::framebufferCallback(GLFWwindow *window, int width, int height)
{
    auto myWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));

    myWindow->m_FramebufferResized = true;
}

Window::~Window()
{
    glfwDestroyWindow(m_Window);

    glfwTerminate();
}
