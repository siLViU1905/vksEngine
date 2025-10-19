#include "app/Application.h"
#include <print>

int main()
{
    try
    {
        vks_engine::Application app(800, 600, "VKSEngine");

        app.run();
    } catch (const std::exception &e)
    {
        std::println("==============  ERROR: {}  ==============", e.what());

        return 1;
    }
}
