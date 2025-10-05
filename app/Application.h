#ifndef VKSENGINEAPPLICATION_H
#define VKSENGINEAPPLICATION_H
#include "handlers/VulkanHandler.h"
#include "Window.h"
#include "scene/Scene.h"

namespace vks_engine
{
    class Application
    {
    public:
        Application(int windowWidth, int windowHeight, const std::string &windowTitle);

        void run();

    private:
        Window m_Window;

        VulkanHandler m_VulkanHandler;

        Scene m_Scene;
    };
}


#endif //VKSENGINEAPPLICATION_H
