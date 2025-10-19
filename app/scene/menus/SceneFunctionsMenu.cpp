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

        ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.15f, 0.15f, 0.2f, 0.95f));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.2f, 0.4f, 0.8f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.3f, 0.5f, 0.9f, 0.8f));
        ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.4f, 0.8f, 0.7f));

        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
        ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 5.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, 1.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 8));

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


            ImGui::Button("Add Mesh", ImVec2(-1, 40));

            std::string_view mesh_popup_name = "Mesh options";

            if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
            {
                ImGui::OpenPopup(mesh_popup_name.data());
            }


            if (ImGui::BeginPopup(mesh_popup_name.data()))
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


            ImGui::Button("Add Light", ImVec2(-1, 40));

            std::string_view light_popup_name = "Light Options";

            if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
            {
                ImGui::OpenPopup(light_popup_name.data());
            }


            if (ImGui::BeginPopup(light_popup_name.data()))
            {
                ImGui::Text("Light Type:");
                ImGui::Separator();

                if (ImGui::MenuItem("Point"))
                {
                    changed = true;
                    ImGui::CloseCurrentPopup();
                    m_BtnAddPointLightCallback();
                }
                if (ImGui::MenuItem("Directional"))
                {
                    changed = true;
                    ImGui::CloseCurrentPopup();
                    m_BtnAddDirectionalLightCallback();
                }

                ImGui::EndPopup();
            }
        }
        ImGui::End();

        ImGui::PopStyleVar(5);
        ImGui::PopStyleColor(7);

        return changed;
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
