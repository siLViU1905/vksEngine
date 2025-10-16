#include "Application.h"

namespace vks_engine
{
    Application::Application(int windowWidth, int windowHeight, const std::string &windowTitle)
        : m_Window(
              windowWidth, windowHeight, windowTitle), m_Scene(m_Window, m_VulkanHandler)
    {
        m_VulkanHandler.init(m_Window.getWindow());

        m_Scene.init();

        //m_Scene.addModel("../../app/models/backpack/backpack.obj");
        m_Scene.addPointLight();
    }

    void Application::run()
    {
        while (m_Window.isOpen())
        {
            m_Scene.run();
        }

        m_VulkanHandler.waitIdle();
    }
}
