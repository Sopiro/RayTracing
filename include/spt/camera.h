#pragma once

#include "common.h"
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
           f64 vfov, // vertical field-of-view in degrees
           f64 aspect_ratio,
           f64 aperture,
           f64 focus_dist);

    Ray GetRay(f64 s, f64 t) const;

    Point3 origin;
    Vec3 dir;

    Vec3 horizontal, vertical;
    Point3 lower_left;

    Vec3 u, v, w;
    f64 lens_radius;
};

inline Camera::Camera(const Point3& look_from,
                      const Point3& look_at,
                      const Vec3& up,
                      f64 vfov, // vertical field-of-view in degrees
                      f64 aspect_ratio,
                      f64 aperture,
                      f64 focus_dist)
{
    f64 theta = DegToRad(vfov);
    f64 h = tan(theta / 2.0);
    f64 viewport_height = 2.0 * h;
    f64 viewport_width = aspect_ratio * viewport_height;

    w = (look_from - look_at).Normalized();
    u = Cross(up, w).Normalized();
    v = Cross(w, u);

    origin = look_from;
    horizontal = focus_dist * viewport_width * u;
    vertical = focus_dist * viewport_height * v;
    lower_left = origin - horizontal / 2.0 - vertical / 2.0 - focus_dist * w;

    lens_radius = aperture / 2.0;
}

inline Ray Camera::GetRay(f64 s, f64 t) const
{
    Vec3 rd = lens_radius * RandomInUnitDiskXY();
    Vec3 offset = u * rd.x + v * rd.y;

    Vec3 camera_center = origin + offset;
    Vec3 pixel_center = lower_left + horizontal * s + vertical * t;

    return Ray{ camera_center, pixel_center - camera_center };
}

} // namespace spt