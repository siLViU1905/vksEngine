#include "Clock.h"

namespace vks_engine
{
    Clock::Clock()
    {
        m_TimePoint = std::chrono::high_resolution_clock::now();
    }

    void Clock::restart()
    {
        m_TimePoint = std::chrono::high_resolution_clock::now();
    }
}
