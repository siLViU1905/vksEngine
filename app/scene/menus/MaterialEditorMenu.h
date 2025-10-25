#ifndef VKSENGINEMATERIALEDITORMENU_H
#define VKSENGINEMATERIALEDITORMENU_H

#include "../../objects/menus/Menu.h"
#include "../../objects/mesh/Mesh.h"

namespace vks_engine
{
    class MaterialEditorMenu : public Menu
    {
    private:
        static constexpr float m_TexturePreviewSize = 128.f;

    public:
        MaterialEditorMenu();

        bool render() override;

        void setMesh(const Mesh &mesh);

        void open();

        void close();

        constexpr bool isOpened() const { return m_IsOpened; }

    private:
        const Mesh *m_Mesh;

        bool m_IsOpened;

        std::unordered_map<TextureType, VkDescriptorSet> m_CachedDescriptors;

        void updateCachedDescriptors();

        void clearCachedDescriptors();

        void renderMaterialNode();

        void renderDiffuseTextureNode(const ImVec2 &position);

        void renderSpecularTextureNode(const ImVec2 &position);

        void renderNormalTextureNode(const ImVec2 &position);

        void renderLinks();

        void renderTexturePreview(const Texture& texture, std::string_view label);
    };
}

#endif //VKSENGINEMATERIALEDITORMENU_H
