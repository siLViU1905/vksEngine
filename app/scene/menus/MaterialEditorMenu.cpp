#include "MaterialEditorMenu.h"
#include <imnodes.h>
#include <print>

#include "imgui_impl_vulkan.h"

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
            updateCachedDescriptors();

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
        if (m_Mesh != &mesh)
        {
            clearCachedDescriptors();
            m_Mesh = &mesh;
        }
    }

    void MaterialEditorMenu::updateCachedDescriptors()
    {
        const auto &material = m_Mesh->getMaterial();

        for (auto type : {TextureType::DIFFUSE, TextureType::SPECULAR, TextureType::NORMALS})
        {
            const auto& [texture, isDefault] = material.getTexture(type);

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
        for (auto& [type, descriptor] : m_CachedDescriptors)
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

        renderTexturePreview(texture, "Diffuse");


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

        renderTexturePreview(texture, "Specular");


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

        renderTexturePreview(texture, "Normal");


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

    void MaterialEditorMenu::renderTexturePreview(const Texture &texture, std::string_view label)
    {
        ImGui::Text("%s", label.data());

        ImGui::Text("Size: %dx%d", texture.m_Width, texture.m_Height);

        ImGui::Spacing();

        float aspectRatio = static_cast<float>(texture.m_Width) / static_cast<float>(texture.m_Height);

        ImVec2 imageSize;
        if (aspectRatio > 1.f)
            imageSize = ImVec2(m_TexturePreviewSize, m_TexturePreviewSize / aspectRatio);
        else
            imageSize = ImVec2(m_TexturePreviewSize * aspectRatio, m_TexturePreviewSize);

        float offsetX = (m_TexturePreviewSize - imageSize.x) * 0.5f;

        if (offsetX > 0)
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);

        auto descriptor = m_CachedDescriptors[texture.getType()];

        if (descriptor != VK_NULL_HANDLE)
            ImGui::Image(descriptor, imageSize);
        else
            ImGui::Dummy(imageSize);

        ImGui::Spacing();
    }
}
