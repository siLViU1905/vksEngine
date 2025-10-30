#ifndef VKSENGINE_IDMANAGER_H
#define VKSENGINE_IDMANAGER_H
#include <cstdint>
#include <deque>
#include <mutex>
#include <optional>

namespace vks_engine
{
    class IDManager
    {
    public:
        IDManager(uint32_t freeIDs);

        std::optional<uint32_t> getAvailableID();

        void returnID(uint32_t id);

        bool hasFreeIDs() const;

        uint32_t getUsedIDs();
    private:
        std::deque<uint32_t> m_FreeIDs;

        mutable std::mutex m_FreeIDsMutex;

        uint32_t m_InitFreeIDs;
    };
}

#endif //VKSENGINE_IDMANAGER_H
