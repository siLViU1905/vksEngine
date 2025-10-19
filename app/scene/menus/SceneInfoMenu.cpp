#include "SceneInfoMenu.h"
#include "../../VKSEngine.h"

namespace vks_engine
{
    bool SceneInfoMenu::render()
    {
        ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetIO().DisplaySize.y * 0.5f), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(200, ImGui::GetIO().DisplaySize.y * 0.5f), ImGuiCond_Always);

        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.15f, 0.15f, 0.2f, 0.95f));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.2f, 0.4f, 0.8f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.3f, 0.5f, 0.9f, 0.8f));
        ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.4f, 0.8f, 0.7f));

        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 8));
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);

        if (ImGui::Begin(m_Title.c_str(), nullptr,
                         ImGuiWindowFlags_NoMove |
                         ImGuiWindowFlags_NoCollapse))
        {
            float availableHeight = ImGui::GetContentRegionAvail().y;
            float capabilitiesHeight = availableHeight * 0.5f - 4.0f;

            if (ImGui::BeginChild("Capabilities", ImVec2(0, capabilitiesHeight), true))
                renderCapabilities();

            ImGui::EndChild();

            ImGui::Spacing();

            if (ImGui::BeginChild("Current State", ImVec2(0, 0), true))
                renderCurrentState();

            ImGui::EndChild();
        }

        ImGui::End();

        ImGui::PopStyleVar(5);
        ImGui::PopStyleColor(4);

        return false;
    }

    SceneInfoMenu::SceneInfoMenu(const uint32_t &simpleMesh, const uint32_t &complexMesh, const uint32_t &pl,
                                 const uint32_t &dl): m_ActiveSimpleMeshCount(simpleMesh),
                                                      m_ActiveComplexMeshCount(complexMesh),
                                                      m_ActivePointLightsCount(pl), m_ActiveDirectionalLightsCount(dl)
    {
    }

    void SceneInfoMenu::renderCapabilities()
    {
        ImGui::Text("Capabilities");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("Max Meshes: %u", SCENE_MAX_ALLOWED_MESH_COUNT);
        ImGui::Text("Max Point Lights: %u", SCENE_MAX_ALLOWED_POINT_LIGHT_COUNT);
        ImGui::Text("Max Dir Lights: %u", SCENE_MAX_ALLOWED_DIRECTIONAL_LIGHT_COUNT);

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("Mesh Bounds:");
        ImGui::BulletText("Position: [%.1f, %.1f]",
                          SCENE_MESH_POSITION_LOWER_BOUND,
                          SCENE_MESH_POSITION_UPPER_BOUND);
        ImGui::BulletText("Scale: [%.2f, %.1f]",
                          SCENE_MESH_SCALE_LOWER_BOUND,
                          SCENE_MESH_SCALE_UPPER_BOUND);

        ImGui::Spacing();
        ImGui::Text("Light Bounds:");
        ImGui::BulletText("Position: [%.1f, %.1f]",
                          SCENE_LIGHT_POSITION_LOWER_BOUND,
                          SCENE_LIGHT_POSITION_UPPER_BOUND);

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("Texture Types: %u", SUPPORTED_TEXTURE_TYPES_COUNT);
        ImGui::Text("Uniform Buffers: %u", USED_UNIFORM_BUFFERS);
    }

    void SceneInfoMenu::renderCurrentState()
    {
        ImGui::Text("Current state");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("Active simple meshes: %u", m_ActiveSimpleMeshCount);
        ImGui::Text("Active complex meshes: %u", m_ActiveComplexMeshCount);
        ImGui::Text("Total active meshes: %u", m_ActiveSimpleMeshCount + m_ActiveComplexMeshCount);

        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("Active point lights: %u", m_ActivePointLightsCount);
        ImGui::Text("Active directional lights: %u", m_ActiveDirectionalLightsCount);
        ImGui::Text("Total active lights: %u", m_ActivePointLightsCount + m_ActiveDirectionalLightsCount);
    }
}
