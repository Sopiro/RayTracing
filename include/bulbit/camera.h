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
           Float vfov, // vertical field-of-view. in degrees.
           Float aspect_ratio,
           Float aperture,
           Float focus_dist);

    Ray GenerateRay(Float s, Float t) const;

    Point3 origin;
    Vec3 dir;

private:
    Point3 lower_left;
    Vec3 horizontal, vertical;

    Float lens_radius;

    // Local coordinate frame
    Vec3 u, v, w;
};

} // namespace spt