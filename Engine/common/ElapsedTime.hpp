//
// Created by turiing on 2025/9/23.
//

#ifndef ELAPSEDTIME_HPP
#define ELAPSEDTIME_HPP

#include "Common.h"
#include "Time.hpp"

USING_ENGINE_NAMESPACE_BEGIN

class ENGINE_EXPORT ElapsedTime {
public:
    explicit ElapsedTime(const Time &interval = -1s): m_startTime(Time::Now()), m_interval(interval) {}
    uint32_t GetElapsed() {
        auto now = Time::Now();
        auto elapsed = TO_U32(std::floor((now - m_startTime) / m_interval));
        if (elapsed != 0) {
            m_startTime = now;
        }
        return elapsed;
    }
    NODISCARD const Time& GetStartTime() const { return m_startTime; }
    void SetStartTime(const Time& time) { m_startTime = time; }
    NODISCARD const Time &GetInterval() const { return m_interval; }
    void SetInterval(const Time& time) { m_interval = time; }

private:
    Time m_startTime;
    Time m_interval;
};
USING_ENGINE_NAMESPACE_END
#endif //ELAPSEDTIME_HPP
