#include "MeshComponent.h"

namespace vks_engine
{
    void MeshComponent::bind()
    {
        m_Menu.setComponent(m_Mesh);
    }
}
