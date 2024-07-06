#pragma once

#include "types.h"

#ifdef BULBIT_DOUBLE_PRECISION
typedef double Float;
#else
typedef float Float;
#endif

namespace bulbit
{

constexpr Float pi = Float(3.14159265358979323846);
constexpr Float two_pi = Float(2 * pi);
constexpr Float four_pi = Float(4 * pi);
constexpr Float inv_pi = Float(1 / pi);
constexpr Float inv_two_pi = Float(1 / (2 * pi));
constexpr Float inv_four_pi = Float(1 / (4 * pi));
constexpr Float epsilon = std::numeric_limits<Float>::epsilon();
constexpr Float infinity = std::numeric_limits<Float>::infinity();
constexpr Float max_value = std::numeric_limits<Float>::max();

inline bool IsNullish(int32 v)
{
    return false;
}

inline bool IsNullish(Float v)
{
    return std::isnan(v) || std::isinf(v);
}

template <typename T>
inline bool IsNullish(const T& v)
{
    return v.IsNullish();
}

#define CheckNull(v)                                                                                                             \
    if (IsNullish(v))                                                                                                            \
    {                                                                                                                            \
        std::cout << #v;                                                                                                         \
        std::cout << " null" << std::endl;                                                                                       \
        assert(false);                                                                                                           \
    }

} // namespace bulbit
