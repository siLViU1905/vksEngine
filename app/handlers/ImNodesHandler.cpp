#include "ImNodesHandler.h"

namespace vks_engine
{
    ImNodesHandler::ImNodesHandler(): m_Context(nullptr)
    {
    }

    void ImNodesHandler::init()
    {
        m_Context = ImNodes::CreateContext();
        ImNodes::StyleColorsDark();


        ImNodesStyle &style = ImNodes::GetStyle();
        style.NodeCornerRounding = 5.0f;
        style.NodePadding = ImVec2(8.0f, 8.0f);
    }

    ImNodesHandler::~ImNodesHandler()
    {
        if (m_Context)
            ImNodes::DestroyContext(m_Context);
    }
}
