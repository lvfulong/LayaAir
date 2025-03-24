#ifndef __TIME_H__
#define __TIME_H__

#include <chrono>

namespace laya
{
class Time
{
  public:
    Time() = default;
    ~Time() = default;

  private:
    std::chrono::steady_clock::time_point m_time;
};

using nano_seconds_t = int64_t;

struct MonotonicClock
{
    static nano_seconds_t now()
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch())
            .count();
    }
};
} // namespace laya
#endif