#ifndef VKSENGINE_MATERIALEDITORMENU_H
#define VKSENGINE_MATERIALEDITORMENU_H

#include "../../objects/menus/Menu.h"
#include "../../objects/mesh/Mesh.h"

namespace vks_engine
{
    class MaterialEditorMenu : public Menu
    {
    private:
        static constexpr float s_TexturePreviewSize = 64.f;

        static constexpr int s_MaterialNodeID = 1000;

        static constexpr int s_NodeIDMultiplier = 100;

        static constexpr int s_DiffuseTextureNodeID = static_cast<int>(TextureType::DIFFUSE) * s_NodeIDMultiplier;

        static constexpr int s_SpecularTextureNodeID = static_cast<int>(TextureType::SPECULAR) * s_NodeIDMultiplier;

        static constexpr int s_NormalTextureNodeID = static_cast<int>(TextureType::NORMALS) * s_NodeIDMultiplier;

        static constexpr std::array<int, 4> s_NodeIDs =
        {
            s_MaterialNodeID,
            s_DiffuseTextureNodeID,
            s_SpecularTextureNodeID,
            s_NormalTextureNodeID
        };

        static constexpr int s_TextureNodeIndent = 70;

        static constexpr float s_NodeWidth = 150.f;

    public:
        MaterialEditorMenu();

        bool render() override;

        void setMesh(Mesh &mesh);

        void open();

        void close();

        constexpr bool isOpened() const { return m_IsOpened; }

        void setOnTextureChange(std::function<void(Mesh& mesh, TextureType type)> function);

        void updateCachedDescriptor(TextureType type);

    private:
        Mesh *m_Mesh;

        bool m_IsOpened;

        std::unordered_map<TextureType, VkDescriptorSet> m_CachedDescriptors;

        std::unordered_map<int, ImVec2> m_NodePositions;

        void updateCachedDescriptors();

        void clearCachedDescriptors();

        void renderMaterialNode();

        void renderDiffuseTextureNode();

        void renderSpecularTextureNode();

        void renderNormalTextureNode();

        void renderLinks();

        void renderTexturePreview(const Texture &texture, std::string_view label);

        void saveNodePositions();

        void removeCachedDescriptor(TextureType type);

        std::function<void(Mesh& mesh, TextureType type)> m_OnTextureChange;
    };
}

#endif //VKSENGINE_MATERIALEDITORMENU_H
