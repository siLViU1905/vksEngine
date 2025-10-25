#ifndef VKSENGINEMATERIALEDITORMENU_H
#define VKSENGINEMATERIALEDITORMENU_H

#include "../../objects/menus/Menu.h"
#include "../../objects/mesh/Mesh.h"

namespace vks_engine
{
    class MaterialEditorMenu : public Menu
    {
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

        void renderMaterialNode();

        void renderDiffuseTextureNode(const ImVec2 &position);

        void renderSpecularTextureNode(const ImVec2 &position);

        void renderNormalTextureNode(const ImVec2 &position);

        void renderLinks();
    };
}

#endif //VKSENGINEMATERIALEDITORMENU_H
