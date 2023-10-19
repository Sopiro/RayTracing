#pragma once

#include "math.h"

namespace bulbit
{

// https://www.pcg-random.org/
inline uint32 PCGHash(uint32 rng_state)
{
    uint32 state = rng_state * 747796405u + 2891336453u;
    uint32 word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
    return (word >> 22u) ^ word;
}

inline uint32 Rand(uint32& seed)
{
    seed = PCGHash(seed);
    return seed;
}

inline thread_local uint32 seed = 1234;

inline void Srand(uint32 new_seed)
{
    seed = new_seed;
}

inline Float Rand()
{
    return Rand(seed) / Float(UINT32_MAX);
}

inline Float Rand(Float min, Float max)
{
    return min + (max - min) * Rand();
}

inline Vec2 RandVec2()
{
    return Vec2(Rand(), Rand());
}

inline Vec2 RandVec2(Float min, Float max)
{
    return Vec2(Rand(min, max), Rand(min, max));
}

inline Vec3 RandVec3()
{
    return Vec3(Rand(), Rand(), Rand());
}

inline Vec3 RandVec3(Float min, Float max)
{
    return Vec3(Rand(min, max), Rand(min, max), Rand(min, max));
}

} // namespace bulbit