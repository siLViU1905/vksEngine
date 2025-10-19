#ifndef VKSENGINESCENEEVENTSMENU_H
#define VKSENGINESCENEEVENTSMENU_H
#include <mutex>
#include <vector>

#include "../../objects/menus/Menu.h"

namespace vks_engine
{
    class SceneEventsMenu : public Menu
    {
    public:
        bool render() override;

        void log(std::string_view message);
    private:
        std::mutex m_BufferMutex;

        std::vector<std::string> m_Buffer;
    };
}

#endif //VKSENGINESCENEEVENTSMENU_H
