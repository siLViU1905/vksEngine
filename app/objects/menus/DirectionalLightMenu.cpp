#include "DirectionalLightMenu.h"

#include <glm/gtc/type_ptr.hpp>

namespace vks_engine
{
    DirectionalLightMenu::DirectionalLightMenu()
    {
    }

    DirectionalLightMenu::DirectionalLightMenu(DirectionalLight &directionalLight, const std::string &title): Menu(
        directionalLight, title)
    {
    }

    bool DirectionalLightMenu::render()
    {
        ImGui::Begin(m_Title.c_str());

        bool modified = false;

        auto direction = m_Component->getNonNormalizedDirection();

        if (ImGui::SliderFloat3("Direction", glm::value_ptr(direction), -1.f, 1.f))
        {
            m_Component->setDirection(direction);
            modified = true;
        }

        auto diffuse = m_Component->getDiffuse();

        if (ImGui::ColorPicker3("Diffuse", glm::value_ptr(diffuse)))
        {
            m_Component->setDiffuse(diffuse);
            modified = true;
        }

        auto specular = m_Component->getSpecular();

        if (ImGui::ColorPicker3("Specular", glm::value_ptr(specular)))
        {
            m_Component->setSpecular(specular);
            modified = true;
        }

        auto ambient = m_Component->getAmbient();

        if (ImGui::ColorPicker3("Ambient", glm::value_ptr(ambient)))
        {
            m_Component->setAmbient(ambient);
            modified = true;
        }

        float intensity = m_Component->getIntensity();

        if (ImGui::SliderFloat("Intensity", &intensity, 0.f, 1.f))
        {
            m_Component->setIntensity(intensity);
            modified = true;
        }

        ImGui::End();

        return modified;
    }
}
