#pragma once

#include "camera.h"

namespace bulbit
{

class OrthographicCamera : public Camera
{
public:
    OrthographicCamera(
        const Point3& look_from, const Point3& look_at, const Vec3& up, Float width, Float height, int32 screen_width);

    virtual Float SampleRay(Ray* out_ray, const Point2& film_sample, const Point2& aperture_sample) const override;

private:
    Point3 origin;
    Point3 lower_left;
    Vec3 horizontal, vertical;

    // Local coordinate frame
    Vec3 u, v, w;
};

} // namespace bulbit
