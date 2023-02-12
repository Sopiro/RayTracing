#pragma once

#include "common.h"
#include "ray.h"

namespace spt
{

class Camera
{
public:
    Camera(const Vec3& look_from,
           const Vec3& look_at,
           const Vec3& up,
           double vfov, // vertical field-of-view in degrees
           double aspect_ratio,
           double aperture,
           double focus_dist)
    {
        double theta = DegToRad(vfov);
        double h = tan(theta / 2.0);
        double viewport_height = 2.0 * h;
        double viewport_width = aspect_ratio * viewport_height;

        w = (look_from - look_at).Normalized();
        u = Cross(up, w).Normalized();
        v = Cross(w, u);

        origin = look_from;
        horizontal = focus_dist * viewport_width * u;
        vertical = focus_dist * viewport_height * v;
        lower_left = origin - horizontal / 2.0 - vertical / 2.0 - focus_dist * w;

        lens_radius = aperture / 2.0;
    }

    Ray GetRay(double s, double t)
    {
        Vec3 rd = lens_radius * RandomInUnitDisk();
        Vec3 offset = u * rd.x + v * rd.y;

        return Ray{ origin + offset, lower_left + horizontal * s + vertical * t - (origin + offset) };
    }

    Vec3 origin;
    Vec3 dir;

    Vec3 horizontal;
    Vec3 vertical;
    Vec3 lower_left;
    Vec3 u, v, w;
    double lens_radius;
};

} // namespace spt