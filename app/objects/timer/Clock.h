#ifndef CLOCK_H
#define CLOCK_H
#include <chrono>
#include "TimeType.h"

class Clock
{
public:
    Clock();

    template<typename Ty, TimeType timeType>
    Ty getElapsedTime() const
    {
        auto now = std::chrono::high_resolution_clock::now();

        if constexpr (timeType == TimeType::Microseconds)
            return std::chrono::duration<Ty, std::micro>(now - m_TimePoint).count();
        else if constexpr (timeType == TimeType::Milliseconds)
            return std::chrono::duration<Ty, std::milli>(now - m_TimePoint).count();
        else if constexpr (timeType == TimeType::Seconds)
            return std::chrono::duration<Ty, std::milli>(now - m_TimePoint).count() / static_cast<Ty>(1000);
        else
            return Ty{};
    }

    template<typename Ty, TimeType timeType>
    Ty restart()
    {
        Ty elapsedTime = getElapsedTime<Ty, timeType>();

        m_TimePoint = std::chrono::high_resolution_clock::now();

        return elapsedTime;
    }

    void restart();

private:
    std::chrono::time_point<std::chrono::steady_clock> m_TimePoint;
};


#endif //CLOCK_H
