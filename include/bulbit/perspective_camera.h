#pragma once

#include "camera.h"

namespace bulbit
{

class PerspectiveCamera : public Camera
{
public:
    PerspectiveCamera(const Point3& look_from,
                      const Point3& look_at,
                      const Vec3& up,
                      Float vfov, // vertical field-of-view. in degrees.
                      Float aspect_ratio,
                      Float aperture,
                      Float focus_dist);

    virtual Float SampleRay(Ray* out_ray, const Point2& film_sample, const Point2& aperture_sample) const override;

    Point3 origin;
    Vec3 dir;

private:
    Point3 lower_left;
    Vec3 horizontal, vertical;

    Float lens_radius;

    // Local coordinate frame
    Vec3 u, v, w;
};

} // namespace bulbit
