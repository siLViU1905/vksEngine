#include "SceneEventsMenu.h"

namespace vks_engine
{
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


            for (const auto &[msg, color]: m_Buffer)
            {
                ImGui::PushStyleColor(ImGuiCol_Text, color);
                ImGui::Text(msg.c_str());
                ImGui::PopStyleColor();
            }
        }

        ImGui::End();

        return false;
    }

    void SceneEventsMenu::log(const std::pair<std::string_view, ImVec4> &message_color)
    {
        std::lock_guard<std::mutex> lock(m_BufferMutex);

        m_Buffer.emplace_back(message_color);
    }

    void SceneEventsMenu::log(std::string_view message, const ImVec4 &color)
    {
        std::lock_guard<std::mutex> lock(m_BufferMutex);

        m_Buffer.emplace_back(message, color);
    }
}
