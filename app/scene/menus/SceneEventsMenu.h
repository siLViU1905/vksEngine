#ifndef VKSENGINE_SCENEEVENTSMENU_H
#define VKSENGINE_SCENEEVENTSMENU_H
#include <mutex>
#include <vector>

#include "../../objects/menus/Menu.h"

namespace vks_engine
{
    class SceneEventsMenu : public Menu
    {
    public:
        bool render() override;

        void log(const std::pair<std::string_view, ImVec4>& message_color);

        void log(std::string_view message, const ImVec4& color = ImVec4(1.f,1.f,1.f,1.f));
    private:
        std::mutex m_BufferMutex;

        std::vector<std::pair<std::string, ImVec4>> m_Buffer;
    };
}

#endif //VKSENGINE_SCENEEVENTSMENU_H
