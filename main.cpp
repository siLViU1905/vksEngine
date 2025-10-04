#include <iostream>

#include "app/Application.h"
#include <print>

int main()
{
    try
    {
        Application app(800, 600, "VulkanPP");

        app.run();
    }
    catch (const std::exception &e)
    {
        std::print("==============  ERROR: {}  ==============", e.what());

        return 1;
    }
}
