#include "PointLightComponent.h"

namespace vks_engine
{
    void PointLightComponent::bind()
    {
        m_Menu.setComponent(m_Light);
    }
}
