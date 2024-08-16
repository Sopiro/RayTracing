#pragma once

#include "vectors.h"

namespace bulbit
{

// https://www.pcg-random.org/
class RNG
{
public:
    RNG()
        : state{ pcg32_default_state }
        , inc{ pcg32_default_stream }
    {
    }

    RNG(uint64 initstate, uint64 initseq = 1)
    {
        Seed(initstate, initseq);
    }

    // pcg32_srandom(initstate, initseq)
    // pcg32_srandom_r(rng, initstate, initseq):
    //     Seed the rng.  Specified in two parts, state initializer and a
    //     sequence selection constant (a.k.a. stream id)
    void Seed(uint64 initstate, uint64 initseq = 1)
    {
        state = 0U;
        inc = (initseq << 1u) | 1u;
        NextUint();
        state += initstate;
        NextUint();
    }

    // pcg32_random()
    // pcg32_random_r(rng)
    //     Generate a uniformly distributed 32-bit random number
    uint32 NextUint()
    {
        uint64 oldstate = state;
        state = oldstate * pcg32_mult + inc;
        uint32 xorshifted = (uint32)(((oldstate >> 18u) ^ oldstate) >> 27u);
        uint32 rot = (uint32)(oldstate >> 59u);
        return (xorshifted >> rot) | (xorshifted << ((~rot + 1u) & 31));
    }

    // pcg32_boundedrand(bound):
    // pcg32_boundedrand_r(rng, bound):
    //     Generate a uniformly distributed number, r, where 0 <= r < bound
    uint32 NextUint(uint32 bound)
    {
        // To avoid bias, we need to make the range of the RNG a multiple of
        // bound, which we do by dropping output less than a threshold.
        // A naive scheme to calculate the threshold would be to do
        //
        //     uint32_t threshold = 0x100000000ull % bound;
        //
        // but 64-bit div/mod is slower than 32-bit div/mod (especially on
        // 32-bit platforms).  In essence, we do
        //
        //     uint32_t threshold = (0x100000000ull-bound) % bound;
        //
        // because this version will calculate the same modulus, but the LHS
        // value is less than 2^32.

        uint32 threshold = (~bound + 1u) % bound;

        // Uniformity guarantees that this loop will terminate.  In practice, it
        // should usually terminate quickly; on average (assuming all bounds are
        // equally likely), 82.25% of the time, we can expect it to require just
        // one iteration.  In the worst case, someone passes a bound of 2^31 + 1
        // (i.e., 2147483649), which invalidates almost 50% of the range.  In
        // practice, bounds are typically small and only a tiny amount of the range
        // is eliminated.
        for (;;)
        {
            uint32 r = NextUint();
            if (r >= threshold)
            {
                return r % bound;
            }
        }
    }

    // [0, 1)
    Float NextFloat()
    {
        return std::fmin(1 - epsilon, Float(NextUint() * 0x1p-32f));
    }

    void Advance(int64_t idelta)
    {
        uint64_t delta = (uint64_t)idelta;

        uint64_t curMult = pcg32_mult;
        uint64_t curPlus = inc;

        uint64_t accMult = 1u;
        uint64_t accPlus = 0u;

        while (delta > 0)
        {
            if (delta & 1)
            {
                accMult *= curMult;
                accPlus = accPlus * curMult + curPlus;
            }
            curPlus = (curMult + 1) * curPlus;
            curMult *= curMult;
            delta /= 2;
        }
        state = accMult * state + accPlus;
    }

private:
    static inline uint64 pcg32_default_state = 0x853c49e6748fea9bULL;
    static inline uint64 pcg32_default_stream = 0xda3e39cb94b95bdbULL;
    static inline uint64 pcg32_mult = 0x5851f42d4c957f2dULL;

    uint64 state; // RNG state.  All values are possible.
    uint64 inc;   // Controls which RNG sequence (stream) is
                  // selected. Must *always* be odd.
};

// Global thread-local RNG
inline thread_local RNG g_rng;

inline void Srand(uint32 new_seed)
{
    g_rng.Seed(new_seed);
}

inline Float Rand()
{
    return g_rng.NextFloat();
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