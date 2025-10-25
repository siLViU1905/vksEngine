#include "MaterialEditorMenu.h"
#include <imnodes.h>

namespace vks_engine
{
    MaterialEditorMenu::MaterialEditorMenu(): m_Mesh(nullptr), m_IsOpened(false)
    {
    }

    void MaterialEditorMenu::open()
    {
        m_IsOpened = true;
    }

    void MaterialEditorMenu::close()
    {
        m_IsOpened = false;
    }

    bool MaterialEditorMenu::render()
    {
        if (!m_IsOpened)
            return false;

        bool changed = false;

        bool isOpen = true;

        if (ImGui::Begin(m_Title.c_str(), &isOpen, ImGuiWindowFlags_NoCollapse))
        {
            ImNodes::BeginNodeEditor();

            renderMaterialNode();

            renderDiffuseTextureNode(ImVec2(50, 50));

            renderSpecularTextureNode(ImVec2(50, 250));

            renderNormalTextureNode(ImVec2(50, 450));

            renderLinks();

            ImNodes::MiniMap(0.2f, ImNodesMiniMapLocation_BottomRight);
            ImNodes::EndNodeEditor();


            int start_attr, end_attr;
            if (ImNodes::IsLinkCreated(&start_attr, &end_attr))
            {
            }

            int link_id;
            if (ImNodes::IsLinkDestroyed(&link_id))
            {
            }
        }
        ImGui::End();

        if (!isOpen)
            close();

        return changed;
    }

    void MaterialEditorMenu::setMesh(const Mesh &mesh)
    {
        m_Mesh = &mesh;
    }

    void MaterialEditorMenu::renderMaterialNode()
    {
        constexpr int materialNodeId = 1000;

        ImNodes::BeginNode(materialNodeId);

        ImNodes::BeginNodeTitleBar();

        ImGui::TextUnformatted("Material");

        ImNodes::EndNodeTitleBar();

        ImNodes::BeginInputAttribute(materialNodeId + 1);

        ImGui::Text("Diffuse");

        ImNodes::EndInputAttribute();

        ImNodes::BeginInputAttribute(materialNodeId + 2);

        ImGui::Text("Specular");

        ImNodes::EndInputAttribute();

        ImNodes::BeginInputAttribute(materialNodeId + 3);

        ImGui::Text("Normal");

        ImNodes::EndInputAttribute();

        ImNodes::EndNode();

        ImNodes::SetNodeGridSpacePos(materialNodeId, ImVec2(600, 250));
    }

    void MaterialEditorMenu::renderDiffuseTextureNode(const ImVec2 &position)
    {
        constexpr int baseId = static_cast<int>(TextureType::DIFFUSE) * 100;

        const auto &material = m_Mesh->getMaterial();

        const auto &texture = *material.getDiffuseTexture().first;

        ImNodes::BeginNode(baseId);


        ImNodes::BeginNodeTitleBar();

        ImGui::Text("Diffuse Texture");

        ImNodes::EndNodeTitleBar();

        ImGui::Spacing();

        ImGui::Text("Size: %dx%d", texture.m_Width, texture.m_Height);

        ImGui::Spacing();


        ImNodes::BeginOutputAttribute(baseId + 1);

        ImGui::Indent(150);

        ImGui::Text("→");

        ImNodes::EndOutputAttribute();

        ImNodes::EndNode();

        ImNodes::SetNodeGridSpacePos(baseId, position);
    }

    void MaterialEditorMenu::renderSpecularTextureNode(const ImVec2 &position)
    {
        constexpr int baseId = static_cast<int>(TextureType::SPECULAR) * 100;

        const auto &material = m_Mesh->getMaterial();

        const auto &texture = *material.getSpecularTexture().first;

        ImNodes::BeginNode(baseId);


        ImNodes::BeginNodeTitleBar();

        ImGui::Text("Specular Texture");

        ImNodes::EndNodeTitleBar();

        ImGui::Spacing();

        ImGui::Text("Size: %dx%d", texture.m_Width, texture.m_Height);

        ImGui::Spacing();


        ImNodes::BeginOutputAttribute(baseId + 1);

        ImGui::Indent(150);

        ImGui::Text("→");

        ImNodes::EndOutputAttribute();

        ImNodes::EndNode();

        ImNodes::SetNodeGridSpacePos(baseId, position);
    }

    void MaterialEditorMenu::renderNormalTextureNode(const ImVec2 &position)
    {
        constexpr int baseId = static_cast<int>(TextureType::NORMALS) * 100;

        const auto &material = m_Mesh->getMaterial();

        const auto &texture = *material.getNormalTexture().first;

        ImNodes::BeginNode(baseId);


        ImNodes::BeginNodeTitleBar();

        ImGui::Text("Normal Texture");

        ImNodes::EndNodeTitleBar();

        ImGui::Spacing();

        ImGui::Text("Size: %dx%d", texture.m_Width, texture.m_Height);

        ImGui::Spacing();


        ImNodes::BeginOutputAttribute(baseId + 1);

        ImGui::Indent(150);

        ImGui::Text("→");

        ImNodes::EndOutputAttribute();

        ImNodes::EndNode();

        ImNodes::SetNodeGridSpacePos(baseId, position);
    }

    void MaterialEditorMenu::renderLinks()
    {
        constexpr int materialNodeId = 1000;

        auto &material = m_Mesh->getMaterial();

        int linkId = 1;
        int startAttr = static_cast<int>(TextureType::DIFFUSE) * 100 + 1;
        int endAttr = materialNodeId + 1;
        ImNodes::Link(linkId, startAttr, endAttr);


        linkId = 2;
        startAttr = static_cast<int>(TextureType::SPECULAR) * 100 + 1;
        endAttr = materialNodeId + 2;
        ImNodes::Link(linkId, startAttr, endAttr);


        linkId = 3;
        startAttr = static_cast<int>(TextureType::NORMALS) * 100 + 1;
        endAttr = materialNodeId + 3;
        ImNodes::Link(linkId, startAttr, endAttr);
    }
}
