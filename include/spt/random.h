#pragma once

#include "math.h"

namespace spt
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
    return Vec2{ Rand(), Rand() };
}

inline Vec2 RandVec2(Real min, Real max)
{
    return Vec2{ Rand(min, max), Rand(min, max) };
}

inline Vec3 RandVec3()
{
    return Vec3{ Rand(), Rand(), Rand() };
}

inline Vec3 RandVec3(Real min, Real max)
{
    return Vec3{ Rand(min, max), Rand(min, max), Rand(min, max) };
}

inline Vec3 RandomUnitVector()
{
    Real u1 = Rand();
    Real u2 = Rand();

    Real x = cos(2 * pi * u1) * 2 * sqrt(u2 * (1 - u2));
    Real y = sin(2 * pi * u1) * 2 * sqrt(u2 * (1 - u2));
    Real z = 1 - 2 * u2;

    return Vec3{ x, y, z };
}

inline Vec3 RandomToSphere(Real radius, Real distance_squared)
{
    Real u1 = Rand();
    Real u2 = Rand();

    Real z = Real(1.0) + u2 * (sqrt(1.0 - radius * radius / distance_squared) - Real(1.0));

    Real phi = two_pi * u1;

    Real sin_theta = sqrt(Real(1.0) - z * z);
    Real x = cos(phi) * sin_theta;
    Real y = sin(phi) * sin_theta;

    return Vec3{ x, y, z };
}

// z > 0
inline Vec3 RandomCosineDirection()
{
    Real u1 = Rand();
    Real u2 = Rand();

    Real z = sqrt(Real(1.0) - u2);

    Real phi = two_pi * u1;
    Real x = cos(phi) * sqrt(u2);
    Real y = sin(phi) * sqrt(u2);

    return Vec3{ x, y, z };
}

inline Vec3 RandomInUnitSphere()
{
    // Rejection sampling
    Vec3 p;
    do
    {
        p = RandVec3(Real(-1.0), Real(1.0));
    }
    while (p.Length2() >= Real(1.0));

    return p;
}

inline Vec3 RandomInUnitDiskXY()
{
    Real u1 = Rand();
    Real u2 = Rand();

    Real r = sqrt(u1);
    Real theta = two_pi * u2;
    return Vec3{ r * cos(theta), r * sin(theta), Real(0.0) };
}

} // namespace spt