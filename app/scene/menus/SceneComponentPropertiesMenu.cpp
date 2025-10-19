#include "SceneComponentPropertiesMenu.h"

#include "../components/DirectionalLightComponent.h"
#include "../components/MeshComponent.h"
#include "../components/PointLightComponent.h"

namespace vks_engine
{
    bool SceneComponentPropertiesMenu::render()
    {
        float displayHeight = ImGui::GetIO().DisplaySize.y;
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 300, displayHeight * 0.4f), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(300, displayHeight * 0.6f), ImGuiCond_Always);

        bool changed = false;

        if (ImGui::Begin(m_Title.c_str(), nullptr,
                         ImGuiWindowFlags_NoResize |
                         ImGuiWindowFlags_NoMove |
                         ImGuiWindowFlags_NoCollapse))
        {
            if (m_ActiveComponent.p_Component == nullptr)
            {
                ImGui::Text("Select a component to edit");
                ImGui::End();
                return changed;
            } else
            {
                ImGui::Text("Editing: %s", m_ActiveComponent.m_Name.c_str());
                ImGui::Text("Type: %s", componentType_to_string(m_ActiveComponent.m_Type).c_str());
                ImGui::Separator();
                ImGui::Spacing();

                auto &meshComponent = *static_cast<MeshComponent *>(m_ActiveComponent.p_Component);

                auto &plComponent = *static_cast<PointLightComponent *>(m_ActiveComponent.p_Component);

                auto &dlComponent = *static_cast<DirectionalLightComponent *>(m_ActiveComponent.p_Component);

                switch (m_ActiveComponent.m_Type)
                {
                    case ComponentType::MESH:
                        changed = meshComponent.m_Menu.render();
                        break;
                    case ComponentType::POINT_LIGHT:
                        changed = plComponent.m_Menu.render();
                        if (changed)
                            m_OnPointLightPropertiesChange(plComponent.m_Light);
                        break;
                    case ComponentType::DIRECTIONAL_LIGHT:
                        changed = dlComponent.m_Menu.render();
                        if (changed)
                            m_OnDirectionalLightPropertiesChange(dlComponent.m_Light);
                        break;
                }
            }
        }
        ImGui::End();

        return changed;
    }

    void SceneComponentPropertiesMenu::setActiveComponent(ComponentEntry &component)
    {
        m_ActiveComponent = component;
    }

    void SceneComponentPropertiesMenu::handleComponentDelete(const ComponentEntry &entry)
    {
        if (m_ActiveComponent.p_Component == entry.p_Component)
            m_ActiveComponent = {};
    }

    void SceneComponentPropertiesMenu::setOnPointLightPropertiesChange(std::function<void(const PointLight &)> function)
    {
        m_OnPointLightPropertiesChange = std::move(function);
    }

    void SceneComponentPropertiesMenu::setOnDirectionalLightPropertiesChange(
        std::function<void(const DirectionalLight &)> function)
    {
        m_OnDirectionalLightPropertiesChange = std::move(function);
    }
}
