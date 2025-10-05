#include "DirectionalLightComponent.h"

namespace vks_engine
{
    void DirectionalLightComponent::bind()
    {
        m_Menu.setComponent(m_Light);
    }
}
