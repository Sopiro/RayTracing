#pragma once

#include "filter.h"

namespace bulbit
{

class BoxFilter : public Filter
{
public:
    BoxFilter(Float extent)
        : extent{ extent }
    {
    }

    virtual Point2 Sample(const Point2& u) const override
    {
        // Remap [0, 1]^2 to [-extent/2, extent/2]^2
        return (2 * u - 1) * (extent / 2);
    }

private:
    Float extent;
};

class TentFilter : public Filter
{
public:
    TentFilter(Float extent)
        : extent{ extent }
    {
    }

    virtual Point2 Sample(const Point2& u) const override
    {
        Float h = extent / 2;

        Float x = u[0] < Float(0.5) ? h * (sqrt(2 * u[0]) - 1) : h * (1 - sqrt(1 - 2 * (u[0] - Float(0.5))));
        Float y = u[1] < Float(0.5) ? h * (sqrt(2 * u[1]) - 1) : h * (1 - sqrt(1 - 2 * (u[1] - Float(0.5))));

        return Point2(x, y);
    }

private:
    Float extent;
};

class GaussianFilter : public Filter
{
public:
    GaussianFilter(Float sigma)
        : sigma{ sigma }
    {
    }

    virtual Point2 Sample(const Point2& u) const override
    {
        // Box Muller transform
        Float r = sigma * std::sqrt(-2 * std::log(std::max<Float>(u[0], 1e-8)));
        return Vector2{ r * std::cos(2 * pi * u[1]), r * std::sin(2 * pi * u[1]) };
    }

private:
    Float sigma;
};

} // namespace bulbit
