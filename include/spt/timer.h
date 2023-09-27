#pragma once

#include <chrono>

namespace spt
{

class Timer
{
    using clock = std::chrono::steady_clock;

public:
    Timer();

    void Mark();
    void Reset();

    Float Get();

private:
    std::vector<clock::time_point> time_points;

    size_t ptr;
};

inline Timer::Timer()
    : ptr{ 0 }
{
    Mark();
}

inline void Timer::Mark()
{
    time_points.push_back(clock::now());
}

inline void Timer::Reset()
{
    time_points.clear();
    time_points.push_back(clock::now());
    ptr = 0;
}

inline Float Timer::Get()
{
    if (ptr < time_points.size() - 1)
    {
        std::chrono::duration<Float> dt = time_points[ptr + 1] - time_points[ptr];
        ++ptr;

        return dt.count();
    }
    else
    {
        return 0.0;
    }
}

} // namespace spt
