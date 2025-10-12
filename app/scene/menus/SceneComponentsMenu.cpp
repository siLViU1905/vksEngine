#include "SceneComponentsMenu.h"

namespace vks_engine
{
    bool SceneComponentsMenu::render()
    {
        float displayHeight = ImGui::GetIO().DisplaySize.y;
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 300, 0), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(300, displayHeight * 0.4f), ImGuiCond_Always);

        bool changed = false;

        if (ImGui::Begin(m_Title.c_str(), nullptr,
                         ImGuiWindowFlags_NoResize |
                         ImGuiWindowFlags_NoMove |
                         ImGuiWindowFlags_NoCollapse |
                         ImGuiWindowFlags_NoDocking))
        {
            ImGui::Separator();
            ImGui::Spacing();

            if (m_Components.empty())
                ImGui::Text("No components added");
            else
            {
                for (auto &entry: m_Components)
                {
                    ImVec4 color;
                    switch (entry.m_Type)
                    {
                        case ComponentType::MESH:
                            color = ImVec4(0.2f, 0.8f, 0.2f, 1.0f);
                            break;
                        case ComponentType::POINT_LIGHT:
                            color = ImVec4(0.8f, 0.8f, 0.2f, 1.0f);
                            break;
                        case ComponentType::DIRECTIONAL_LIGHT:
                            color = ImVec4(0.8f, 0.2f, 0.2f, 1.0f);
                            break;
                    }

                    ImGui::PushStyleColor(ImGuiCol_Button, color);
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                                          ImVec4(color.x * 1.2f, color.y * 1.2f, color.z * 1.2f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                                          ImVec4(color.x * 0.8f, color.y * 0.8f, color.z * 0.8f, 1.0f));

                    const auto &buttonLabel = entry.m_Name;

                    if (ImGui::Button(buttonLabel.c_str(), ImVec2(-1, 30)))
                    {
                        m_OnComponentSelection(entry);
                        changed = true;
                    }

                    ImGui::PopStyleColor(3);

                    /*if (entry.name == m_SelectedComponent)
                    {
                        ImGui::GetWindowDrawList()->AddRect(
                            ImGui::GetItemRectMin(), ImGui::GetItemRectMax(),
                            IM_COL32(255, 255, 255, 255), 3.0f, 0, 2.0f
                        );
                    }*/

                    ImGui::Spacing();
                }
            }
        }
        ImGui::End();

        return changed;
    }

    void SceneComponentsMenu::setOnComponentSelected(std::function<void(ComponentEntry &)> function)
    {
        m_OnComponentSelection = std::move(function);
    }
}
