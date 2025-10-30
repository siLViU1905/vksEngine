#ifndef VKSENGINE_IDMANAGER_H
#define VKSENGINE_IDMANAGER_H
#include <cstdint>
#include <deque>
#include <optional>

namespace vks_engine
{
    class IDManager
    {
    public:
        IDManager(uint32_t freeIds);

        uint32_t getAvailableID();

        void returnID(uint32_t id);

        bool hasFreeIDs() const;
    private:
        std::deque<uint32_t> m_FreeIDs;
    };
}

#endif //VKSENGINE_IDMANAGER_H
