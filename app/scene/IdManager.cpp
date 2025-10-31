#include "IdManager.h"

namespace vks_engine
{
    IDManager::IDManager(uint32_t freeIDs):m_InitFreeIDs(freeIDs)
    {
        for (uint32_t i = 0; i < freeIDs; i++)
            m_FreeIDs.push_back(i);
    }

    std::optional<uint32_t> IDManager::getAvailableID()
    {
        std::lock_guard lock(m_FreeIDsMutex);

        if (m_FreeIDs.empty())
            return std::nullopt;

        auto id = m_FreeIDs.front();

        m_FreeIDs.pop_front();

        return id;
    }

    void IDManager::returnID(uint32_t id)
    {
        std::lock_guard lock(m_FreeIDsMutex);

        m_FreeIDs.push_back(id);
    }

    bool IDManager::hasFreeIDs() const
    {
        std::lock_guard lock(m_FreeIDsMutex);

        return !m_FreeIDs.empty();
    }

    uint32_t IDManager::getUsedIDs() const
    {
        return m_InitFreeIDs - m_FreeIDs.size();
    }
}
