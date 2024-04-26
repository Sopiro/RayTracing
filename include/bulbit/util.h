#pragma once

#include "transform.h"

namespace bulbit
{

inline bool IsNullish(Float v)
{
    return std::isnan(v) || std::isinf(v);
}

template <typename T>
inline bool IsNullish(const T& v)
{
    return v.IsNullish();
}

#define checkNull(v)                                                                                                             \
    if (IsNullish(v))                                                                                                            \
    {                                                                                                                            \
        std::cout << #v;                                                                                                         \
        std::cout << " null" << std::endl;                                                                                       \
    }

} // namespace bulbit
