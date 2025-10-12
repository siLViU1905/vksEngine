#ifndef VKSENGINEMENU_H
#define VKSENGINEMENU_H
#include <string>
#include <imgui.h>

namespace vks_engine
{
    class Menu
    {
    public:
        Menu();

        Menu(const std::string &title);

        void setTitle(const std::string &title);

        void setPosition(const ImVec2 &position);

        void setSize(const ImVec2 &size);

        void move(const ImVec2 &offset);

        void size(const ImVec2 &offset);

        constexpr const std::string &getTitle() const { return m_Title; }

        constexpr const ImVec2 &getPosition() const { return m_Position; }

        constexpr const ImVec2 &getSize() const { return m_Size; }

        virtual bool render();

        virtual ~Menu() = default;

    protected:
        std::string m_Title;

        ImVec2 m_Position;

        ImVec2 m_Size;
    };
}

#endif //VKSENGINEMENU_H
