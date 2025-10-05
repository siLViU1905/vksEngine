#include "MeshMenu.h"

#include <glm/gtc/type_ptr.inl>

namespace vks_engine
{
    MeshMenu::MeshMenu()
    {
    }

    MeshMenu::MeshMenu(Mesh &mesh, const std::string &title): Menu(mesh, title)
    {
    }

    bool MeshMenu::render()
    {
        ImGui::Begin(m_Title.c_str());

        bool modified = false;

        auto position = m_Component->getPosition();

        if (ImGui::SliderFloat3("Position", glm::value_ptr(position), -5.f, 5.f))
        {
            m_Component->setPosition(position);
            modified = true;
        }

        auto scale = m_Component->getScale();

        if (ImGui::SliderFloat3("Scale", glm::value_ptr(scale), 0.1f, 5.f))
        {
            m_Component->setScale(scale);
            modified = true;
        }

        auto rotation = m_Component->getRotation();

        if (ImGui::SliderFloat3("Rotation", glm::value_ptr(rotation), 0.f, 360.f))
        {
            m_Component->setRotation(rotation);
            modified = true;
        }

        auto color = m_Component->getColor();

        if (ImGui::ColorPicker3("Color", glm::value_ptr(color)))
        {
            m_Component->setColor(color);
            modified = true;
        }

        ImGui::End();

        return modified;
    }
}
