#include "Menu.h"

namespace vks_engine
{
    Menu::Menu(): m_Title("NULL")
    {
    }

    Menu::Menu(const std::string &title): m_Title(title)
    {
    }

    void Menu::setTitle(const std::string &title)
    {
        m_Title = title;
    }

    void Menu::setPosition(const ImVec2 &position)
    {
        m_Position = position;
    }

    void Menu::setSize(const ImVec2 &size)
    {
        m_Size = size;
    }

    void Menu::move(const ImVec2 &offset)
    {
        m_Position.x += offset.x;
        m_Position.y += offset.y;
    }

    void Menu::size(const ImVec2 &offset)
    {
        m_Size.x += offset.x;
        m_Size.y += offset.y;
    }

    bool Menu::render()
    {
        ImGui::Begin(m_Title.c_str());

        ImGui::Text("Render method not overloaded");

        ImGui::End();

        return false;
    }
}
