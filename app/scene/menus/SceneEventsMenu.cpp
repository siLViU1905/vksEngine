#include "SceneEventsMenu.h"

namespace vks_engine {
    bool SceneEventsMenu::render()
    {
        static float windowHeight = ImGui::GetIO().DisplaySize.y * 0.3f;

        ImVec2 displaySize = ImGui::GetIO().DisplaySize;
        float fixedWidth = displaySize.x - 500;
        float fixedX = 200;
        float fixedY = displaySize.y - windowHeight;

        ImGui::SetNextWindowPos(ImVec2(fixedX, fixedY), ImGuiCond_Always);
        ImGui::SetNextWindowSizeConstraints(ImVec2(fixedWidth, 100), ImVec2(fixedWidth, displaySize.y - 100));

        if (ImGui::Begin(m_Title.c_str(), nullptr,
                         ImGuiWindowFlags_NoMove |
                         ImGuiWindowFlags_NoCollapse |
                         ImGuiWindowFlags_NoDocking))
        {
            windowHeight = ImGui::GetWindowHeight();
        }

        ImGui::End();

        return false;
    }
}
