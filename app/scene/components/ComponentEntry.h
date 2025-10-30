#ifndef VKSENGINE_COMPONENTENTRY_H
#define VKSENGINE_COMPONENTENTRY_H
#include <string>

#include "ComponentType.h"

namespace vks_engine
{
    struct ComponentEntry
    {
        ComponentType m_Type;

        std::string m_Name;

        void *p_Component;

        constexpr ComponentEntry(): m_Type(ComponentType::NaN), m_Name("NULL"), p_Component(nullptr)
        {
        }
    };
}

#endif //VKSENGINE_COMPONENTENTRY_H
