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
                for (auto it = m_Components.begin(); it != m_Components.end(); ++it)
                {
                    auto &entry = *it;

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

                    auto edit_component_popup_name = "EditComponentPopup" + std::to_string(
                                                         reinterpret_cast<uintptr_t>(&entry));

                    static ComponentEntry *entryBeingEdited = nullptr;

                    static char newName[32] = {};

                    if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
                    {
                        ImGui::OpenPopup(edit_component_popup_name.c_str());
                        entryBeingEdited = &entry;
                        strncpy_s(newName, entry.m_Name.c_str(), sizeof(newName) - 1);
                    }

                    if (ImGui::BeginPopup(edit_component_popup_name.c_str()))
                    {
                        ImGui::Text("Edit component:");
                        ImGui::Separator();

                        if (entryBeingEdited == &entry)
                        {
                            ImGui::Separator();
                            ImGui::Text("Rename:");

                            if (ImGui::InputText("##rename", newName,IM_ARRAYSIZE(newName),
                                                 ImGuiInputTextFlags_EnterReturnsTrue))
                            {
                                changed = true;
                                m_OnComponentRename(entry, newName);
                                entry.m_Name = newName;
                                entryBeingEdited = nullptr;
                                ImGui::CloseCurrentPopup();
                            }
                        }

                        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
                        if (ImGui::MenuItem("Delete"))
                        {
                            changed = true;
                            m_OnComponentDelete(entry);
                            ImGui::CloseCurrentPopup();
                        }
                        ImGui::PopStyleColor();

                        ImGui::EndPopup();
                    }

                    ImGui::PopStyleColor(3);

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

    void SceneComponentsMenu::setOnComponentRename(std::function<void(ComponentEntry &, std::string_view)> function)
    {
        m_OnComponentRename = std::move(function);
    }

    void SceneComponentsMenu::setOnComponentDelete(std::function<void(ComponentEntry &)> function)
    {
        m_OnComponentDelete = std::move(function);
    }

    void SceneComponentsMenu::deleteComponentEntry(const ComponentEntry &component)
    {
        auto it = std::find_if(m_Components.begin(), m_Components.end(), [component](const auto &comp)
        {
            return comp.p_Component == component.p_Component;
        });

        if (it != m_Components.end())
            m_Components.erase(it);
    }
}
