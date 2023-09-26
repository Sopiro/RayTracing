#pragma once

#include "ray.h"

namespace spt
{

class Camera
{
public:
    Camera() = default;

    Camera(const Point3& look_from,
           const Point3& look_at,
           const Vec3& up,
           f64 vfov, // vertical field-of-view. in degrees.
           f64 aspect_ratio,
           f64 aperture,
           f64 focus_dist);

    Ray GetRay(f64 s, f64 t) const;

    Point3 origin;
    Vec3 dir;

private:
    Point3 lower_left;
    Vec3 horizontal, vertical;

    f64 lens_radius;

    // Local coordinate frame
    Vec3 u, v, w;
};

} // namespace spt