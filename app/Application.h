#ifndef APPLICATION_H
#define APPLICATION_H
#include "VulkanHandler.h"
#include "Window.h"
#include "scene/Scene.h"


class Application {

public:
    Application(int windowWidth, int windowHeight, const std::string& windowTitle);

    void run();
private:
    Window m_Window;

    VulkanHandler m_VulkanHandler;

    Scene m_Scene;
};



#endif //APPLICATION_H
