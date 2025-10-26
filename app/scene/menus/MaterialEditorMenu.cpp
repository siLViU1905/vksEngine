#include "MaterialEditorMenu.h"
#include <imnodes.h>
#include <print>
#include <ranges>

#include "imgui_impl_vulkan.h"

namespace vks_engine
{
    MaterialEditorMenu::MaterialEditorMenu(): m_Mesh(nullptr), m_IsOpened(false)
    {
        m_NodePositions[s_MaterialNodeID] = ImVec2(400, 150);
        m_NodePositions[s_DiffuseTextureNodeID] = ImVec2(30, 30);
        m_NodePositions[s_SpecularTextureNodeID] = ImVec2(60, 220);
        m_NodePositions[s_NormalTextureNodeID] = ImVec2(90, 410);
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
            updateCachedDescriptors();

            ImNodes::BeginNodeEditor();

            renderMaterialNode();

            renderDiffuseTextureNode();

            renderSpecularTextureNode();

            renderNormalTextureNode();

            renderLinks();

            ImNodes::MiniMap(0.2f, ImNodesMiniMapLocation_BottomRight);
            ImNodes::EndNodeEditor();

            saveNodePositions();

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
        if (m_Mesh != &mesh)
        {
            clearCachedDescriptors();
            m_Mesh = &mesh;
        }
    }

    void MaterialEditorMenu::updateCachedDescriptors()
    {
        const auto &material = m_Mesh->getMaterial();

        for (auto type: {TextureType::DIFFUSE, TextureType::SPECULAR, TextureType::NORMALS})
        {
            const auto &[texture, isDefault] = material.getTexture(type);

            if (texture->isLoaded() && m_CachedDescriptors[type] == VK_NULL_HANDLE)
            {
                m_CachedDescriptors[type] = ImGui_ImplVulkan_AddTexture(
                    *texture->m_Sampler,
                    *texture->m_ImageView,
                    VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
                );
            }
        }
    }

    void MaterialEditorMenu::clearCachedDescriptors()
    {
        for (auto &descriptor: m_CachedDescriptors | std::views::values)
        {
            if (descriptor != VK_NULL_HANDLE)
            {
                ImGui_ImplVulkan_RemoveTexture(descriptor);
                descriptor = VK_NULL_HANDLE;
            }
        }
    }

    void MaterialEditorMenu::renderMaterialNode()
    {
        auto position = m_NodePositions[s_MaterialNodeID];

        ImNodes::BeginNode(s_MaterialNodeID);

        ImNodes::BeginNodeTitleBar();

        ImGui::TextUnformatted("Material");

        ImNodes::EndNodeTitleBar();

        ImNodes::BeginInputAttribute(s_MaterialNodeID + 1);

        ImGui::Text("Diffuse");

        ImNodes::EndInputAttribute();

        ImNodes::BeginInputAttribute(s_MaterialNodeID + 2);

        ImGui::Text("Specular");

        ImNodes::EndInputAttribute();

        ImNodes::BeginInputAttribute(s_MaterialNodeID + 3);

        ImGui::Text("Normal");

        ImNodes::EndInputAttribute();

        ImNodes::EndNode();

        ImNodes::SetNodeGridSpacePos(s_MaterialNodeID, position);
    }

    void MaterialEditorMenu::renderDiffuseTextureNode()
    {
        const auto &material = m_Mesh->getMaterial();

        const auto &texture = *material.getDiffuseTexture().first;

        auto position = m_NodePositions[s_DiffuseTextureNodeID];

        ImNodes::BeginNode(s_DiffuseTextureNodeID);

        ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkDetachWithDragClick);

        ImGui::PushItemWidth(150.0f);

        ImNodes::BeginNodeTitleBar();

        ImGui::Text("Diffuse Texture");

        ImNodes::EndNodeTitleBar();

        renderTexturePreview(texture, "Diffuse");


        ImNodes::BeginOutputAttribute(s_DiffuseTextureNodeID + 1);

        ImGui::Indent(s_TextureNodeIndent);

        ImGui::Text("→");

        ImNodes::EndOutputAttribute();

        ImNodes::EndNode();

        ImNodes::SetNodeGridSpacePos(s_DiffuseTextureNodeID, position);
    }

    void MaterialEditorMenu::renderSpecularTextureNode()
    {
        const auto &material = m_Mesh->getMaterial();

        const auto &texture = *material.getSpecularTexture().first;

        auto position = m_NodePositions[s_SpecularTextureNodeID];

        ImNodes::BeginNode(s_SpecularTextureNodeID);

        ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkDetachWithDragClick);

        ImGui::PushItemWidth(150.0f);

        ImNodes::BeginNodeTitleBar();

        ImGui::Text("Specular Texture");

        ImNodes::EndNodeTitleBar();

        renderTexturePreview(texture, "Specular");


        ImNodes::BeginOutputAttribute(s_SpecularTextureNodeID + 1);

        ImGui::Indent(s_TextureNodeIndent);

        ImGui::Text("→");

        ImNodes::EndOutputAttribute();

        ImNodes::EndNode();

        ImNodes::SetNodeGridSpacePos(s_SpecularTextureNodeID, position);
    }

    void MaterialEditorMenu::renderNormalTextureNode()
    {
        const auto &material = m_Mesh->getMaterial();

        const auto &texture = *material.getNormalTexture().first;

        auto position = m_NodePositions[s_NormalTextureNodeID];

        ImNodes::BeginNode(s_NormalTextureNodeID);

        ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkDetachWithDragClick);

        ImGui::PushItemWidth(150.0f);


        ImNodes::BeginNodeTitleBar();

        ImGui::Text("Normal Texture");

        ImNodes::EndNodeTitleBar();

        renderTexturePreview(texture, "Normal");


        ImNodes::BeginOutputAttribute(s_NormalTextureNodeID + 1);

        ImGui::Indent(s_TextureNodeIndent);

        ImGui::Text("→");

        ImNodes::EndOutputAttribute();

        ImNodes::EndNode();

        ImNodes::SetNodeGridSpacePos(s_NormalTextureNodeID, position);
    }

    void MaterialEditorMenu::renderLinks()
    {
        auto &material = m_Mesh->getMaterial();

        int linkId = 1;
        int startAttr = s_DiffuseTextureNodeID + 1;
        int endAttr = s_MaterialNodeID + 1;
        ImNodes::Link(linkId, startAttr, endAttr);


        linkId = 2;
        startAttr = s_SpecularTextureNodeID + 1;
        endAttr = s_MaterialNodeID + 2;
        ImNodes::Link(linkId, startAttr, endAttr);


        linkId = 3;
        startAttr = s_NormalTextureNodeID + 1;
        endAttr = s_MaterialNodeID + 3;
        ImNodes::Link(linkId, startAttr, endAttr);
    }

    void MaterialEditorMenu::renderTexturePreview(const Texture &texture, std::string_view label)
    {
        ImGui::Text("%s", label.data());

        ImGui::Text("Size: %dx%d", texture.m_Width, texture.m_Height);

        ImGui::Spacing();

        float aspectRatio = static_cast<float>(texture.m_Width) / static_cast<float>(texture.m_Height);

        ImVec2 imageSize;
        if (aspectRatio > 1.f)
            imageSize = ImVec2(s_TexturePreviewSize, s_TexturePreviewSize / aspectRatio);
        else
            imageSize = ImVec2(s_TexturePreviewSize * aspectRatio, s_TexturePreviewSize);

        float offsetX = (s_TexturePreviewSize - imageSize.x) * 0.5f;

        if (offsetX > 0)
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);

        auto descriptor = m_CachedDescriptors[texture.getType()];

        if (descriptor != VK_NULL_HANDLE)
            ImGui::Image(descriptor, imageSize);
        else
            ImGui::Dummy(imageSize);


        ImGui::Spacing();
    }

    void MaterialEditorMenu::saveNodePositions()
    {
        for (auto nodeID: s_NodeIDs)
        {
            auto position = ImNodes::GetNodeGridSpacePos(nodeID);
            m_NodePositions[nodeID] = position;
        }
    }
}
