#ifndef VKSENGINE_CLOCK_H
#define VKSENGINE_CLOCK_H
#include <chrono>
#include "TimeType.h"

namespace vks_engine
{
    class Clock
    {
    public:
        Clock();

        template<typename Ty, TimeType timeType>
        Ty getElapsedTime() const
        {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = now - m_TimePoint;


            if constexpr (std::is_floating_point_v<Ty>)
            {
                if constexpr (timeType == TimeType::Microseconds)
                    return std::chrono::duration<Ty, std::micro>(elapsed).count();
                else if constexpr (timeType == TimeType::Milliseconds)
                    return std::chrono::duration<Ty, std::milli>(elapsed).count();
                else if constexpr (timeType == TimeType::Seconds)
                    return std::chrono::duration<Ty, std::ratio<1, 1>>(elapsed).count();
                else
                    return Ty{};
            }
            else if constexpr (std::is_integral_v<Ty>)
            {
                if constexpr (timeType == TimeType::Microseconds)
                    return std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
                else if constexpr (timeType == TimeType::Milliseconds)
                    return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
                else if constexpr (timeType == TimeType::Seconds)
                    return std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();
                else
                    return Ty{};
            }
            else
                return Ty{};

        }

        template<typename Ty, TimeType timeType>
        Ty restart()
        {
            Ty elapsedTime = getElapsedTime<Ty, timeType>();

            m_TimePoint = std::chrono::steady_clock::now();

            return elapsedTime;
        }

        void restart();

    private:
        std::chrono::time_point<std::chrono::steady_clock> m_TimePoint;
    };
}


#endif //VKSENGINE_CLOCK_H
