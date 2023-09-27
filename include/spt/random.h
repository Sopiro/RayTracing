#pragma once

#include "math.h"

namespace spt
{

// https://www.pcg-random.org/
inline u32 PCGHash(u32 rng_state)
{
    u32 state = rng_state * 747796405u + 2891336453u;
    u32 word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
    return (word >> 22u) ^ word;
}

inline u32 Rand(u32& seed)
{
    seed = PCGHash(seed);
    return seed;
}

inline thread_local u32 seed = 1234;

inline void Srand(u32 new_seed)
{
    seed = new_seed;
}

inline Real Rand()
{
    return Rand(seed) / Real(UINT32_MAX);
}

inline Real Rand(Real min, Real max)
{
    return min + (max - min) * Rand();
}

inline Vec2 RandVec2()
{
    return Vec2(Rand(), Rand());
}

inline Vec2 RandVec2(Real min, Real max)
{
    return Vec2(Rand(min, max), Rand(min, max));
}

inline Vec3 RandVec3()
{
    return Vec3(Rand(), Rand(), Rand());
}

inline Vec3 RandVec3(Real min, Real max)
{
    return Vec3(Rand(min, max), Rand(min, max), Rand(min, max));
}

} // namespace spt