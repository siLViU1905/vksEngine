#include "IdManager.h"

namespace vks_engine
{
    IDManager::IDManager(uint32_t freeIds)
    {
        for (uint32_t i = 0; i < freeIds; i++)
            m_FreeIDs.push_back(i);
    }

    uint32_t IDManager::getAvailableID()
    {
        auto id = m_FreeIDs.front();

        m_FreeIDs.pop_front();

        return id;
    }

    void IDManager::returnID(uint32_t id)
    {
        m_FreeIDs.push_back(id);
    }

    bool IDManager::hasFreeIDs() const
    {
        return !m_FreeIDs.empty();
    }
}
