#pragma once

#include "ray.h"

namespace bulbit
{

class Camera
{
public:
    virtual ~Camera() = default;

    virtual Float SampleRay(Ray* out_ray, const Point2& film_sample, const Point2& aperture_sample) const = 0;
};

} // namespace bulbit