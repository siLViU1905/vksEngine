#include "SceneFunctionsMenu.h"

namespace vks_engine
{
    bool SceneFunctionsMenu::render()
    {
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(200, ImGui::GetIO().DisplaySize.y * 0.7f), ImGuiCond_Always);


        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.4f, 0.8f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.5f, 0.9f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.3f, 0.7f, 1.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));

        bool changed = false;

        if (ImGui::Begin(m_Title.c_str(), nullptr,
                         ImGuiWindowFlags_NoResize |
                         ImGuiWindowFlags_NoMove |
                         ImGuiWindowFlags_NoCollapse |
                         ImGuiWindowFlags_NoDocking))
        {
            ImGui::Text("Scene Controls");
            ImGui::Separator();
            ImGui::Spacing();


            if (ImGui::Button("Add Mesh", ImVec2(-1, 40)))
            {
                m_BtnAddMeshCallback();
                changed = true;
            }

            std::string_view popup_name = "AddMeshPopup";

            if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
            {
                ImGui::OpenPopup(popup_name.data());
            }


            if (ImGui::BeginPopup(popup_name.data()))
            {
                ImGui::Text("Mesh Type:");
                ImGui::Separator();

                if (ImGui::MenuItem("Cube"))
                {
                    changed = true;
                    ImGui::CloseCurrentPopup();
                    m_MeshTypeSelectionCallback(MeshType::CUBE);
                }
                if (ImGui::MenuItem("Sphere"))
                {
                    changed = true;
                    ImGui::CloseCurrentPopup();
                    m_MeshTypeSelectionCallback(MeshType::SPHERE);
                }
                if (ImGui::MenuItem("Custom Model..."))
                {
                    changed = true;
                    ImGui::CloseCurrentPopup();
                    m_MeshTypeSelectionCallback(MeshType::MODEL);
                }

                ImGui::EndPopup();
            }

            ImGui::Spacing();


            if (ImGui::Button("Add Point Light", ImVec2(-1, 40)))
            {
                m_BtnAddPointLightCallback();
                changed = true;
            }

            ImGui::Spacing();


            if (ImGui::Button("Add Directional Light", ImVec2(-1, 40)))
            {
                m_BtnAddDirectionalLightCallback();
                changed = true;
            }
        }
        ImGui::End();

        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(3);

        return changed;
    }

    void SceneFunctionsMenu::onAddMeshBtnClick(std::function<void()> function)
    {
        m_BtnAddMeshCallback = std::move(function);
    }

    void SceneFunctionsMenu::onAddPointLightBtnClick(std::function<void()> function)
    {
        m_BtnAddPointLightCallback = std::move(function);
    }

    void SceneFunctionsMenu::onAddDirectionalLightBtnClick(std::function<void()> function)
    {
        m_BtnAddDirectionalLightCallback = std::move(function);
    }

    void SceneFunctionsMenu::onMeshTypeSelection(std::function<void(MeshType)> function)
    {
        m_MeshTypeSelectionCallback = std::move(function);
    }
}
