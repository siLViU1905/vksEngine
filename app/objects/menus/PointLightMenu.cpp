#include "PointLightMenu.h"

#include <print>
#include <glm/gtc/type_ptr.hpp>

PointLightMenu::PointLightMenu()
{
}

PointLightMenu::PointLightMenu(PointLight &pointLight, const std::string &title): Menu(pointLight, title)
{
}

bool PointLightMenu::render()
{
    ImGui::Begin(m_Title.c_str());

    bool modified = false;

    auto position = m_Component->getPosition();

    if (ImGui::SliderFloat3("Position", glm::value_ptr(position), -5.f, 5.f))
    {
        m_Component->setPosition(position);
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

    float constant = m_Component->getConstant();

    if (ImGui::SliderFloat("Constant", &constant, 0.f, 1.f))
    {
        m_Component->setConstant(constant);
        modified = true;
    }

    float linear = m_Component->getLinear();

    if (ImGui::SliderFloat("Linear", &linear, 0.f, 1.f))
    {
        m_Component->setLinear(linear);
        modified = true;
    }

    float quadratic = m_Component->getQuadratic();

    if (ImGui::SliderFloat("Quadratic", &quadratic, 0.f, 1.f))
    {
        m_Component->setQuadratic(quadratic);
        modified = true;
    }

    ImGui::End();

    return modified;
}
