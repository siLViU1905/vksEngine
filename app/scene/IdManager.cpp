#include "IdManager.h"

namespace vks_engine
{
    IDManager::IDManager(uint32_t freeIds)
    {
        for (uint32_t i = 0; i < freeIds; i++)
            m_FreeIDs.push_back(i);
    }

    std::optional<uint32_t> IDManager::getAvailableID()
    {
        std::lock_guard lock(m_FreeIdsMutex);

        if (m_FreeIDs.empty())
            return std::nullopt;

        auto id = m_FreeIDs.front();

        m_FreeIDs.pop_front();

        return id;
    }

    void IDManager::returnID(uint32_t id)
    {
        std::lock_guard lock(m_FreeIdsMutex);

        m_FreeIDs.push_back(id);
    }

    bool IDManager::hasFreeIDs() const
    {
        std::lock_guard lock(m_FreeIdsMutex);

        return !m_FreeIDs.empty();
    }
}
