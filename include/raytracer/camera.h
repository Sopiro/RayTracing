#pragma once

#include "common.h"
#include "ray.h"

class Camera
{
public:
    Camera(const Vec3& look_from,
           const Vec3& look_at,
           const Vec3& up,
           Real vfov, // vertical field-of-view in degrees
           Real aspect_ratio,
           Real aperture,
           Real focus_dist)
    {
        Real theta = DegToRad(vfov);
        Real h = tan(theta / Real(2.0));
        Real viewport_height = Real(2.0) * h;
        Real viewport_width = aspect_ratio * viewport_height;

        w = (look_from - look_at).Normalized();
        u = Cross(up, w).Normalized();
        v = Cross(w, u);

        origin = look_from;
        horizontal = focus_dist * viewport_width * u;
        vertical = focus_dist * viewport_height * v;
        lower_left = origin - horizontal / 2.0 - vertical / 2.0 - focus_dist * w;

        lens_radius = aperture / Real(2.0);
    }

    Ray GetRay(Real s, Real t)
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
    Real lens_radius;
};