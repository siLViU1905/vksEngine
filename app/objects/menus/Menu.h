#ifndef VKSENGINEMENU_H
#define VKSENGINEMENU_H
#include <string>

#include "imgui/imgui.h"

namespace vks_engine
{
    template<typename Component>
    class Menu
    {
    public:
        Menu():m_Component(nullptr), m_Title("NULL")
        {}

        Menu(Component& component, const std::string& title = "NULL"):m_Component(&component), m_Title(title)
        {}

        void setComponent(Component& component)
        {
            m_Component = &component;
        }

        Component& getComponent() {return *m_Component;}

        void setTitle(const std::string& title)
        {
            m_Title = title;
        }

        constexpr const std::string& getTitle() const {return m_Title;}

        virtual bool render()
        {
            ImGui::Begin(m_Title.c_str());

            ImGui::Text("Render method not overloaded");

            ImGui::End();

            return false;
        }

        virtual ~Menu() = default;

    protected:
        Component* m_Component;

        std::string m_Title;
    };
}

#endif //VKSENGINEMENU_H
