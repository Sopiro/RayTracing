#pragma once

#include "math.h"

namespace bulbit
{

// Pixel reconstruction filter
class Filter
{
public:
    Filter() = default;
    virtual ~Filter() = default;

    virtual Point2 Sample(const Point2& u) const = 0;
};

} // namespace bulbit
