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
           float64 vfov, // vertical field-of-view in degrees
           float64 aspect_ratio,
           float64 aperture,
           float64 focus_dist);

    Ray GetRay(float64 s, float64 t) const;

    Point3 origin;
    Vec3 dir;

    Vec3 horizontal, vertical;
    Point3 lower_left;

    Vec3 u, v, w;
    float64 lens_radius;
};

inline Camera::Camera(const Point3& look_from,
                      const Point3& look_at,
                      const Vec3& up,
                      float64 vfov, // vertical field-of-view in degrees
                      float64 aspect_ratio,
                      float64 aperture,
                      float64 focus_dist)
{
    float64 theta = DegToRad(vfov);
    float64 h = tan(theta / 2.0);
    float64 viewport_height = 2.0 * h;
    float64 viewport_width = aspect_ratio * viewport_height;

    w = (look_from - look_at).Normalized();
    u = Cross(up, w).Normalized();
    v = Cross(w, u);

    origin = look_from;
    horizontal = focus_dist * viewport_width * u;
    vertical = focus_dist * viewport_height * v;
    lower_left = origin - horizontal / 2.0 - vertical / 2.0 - focus_dist * w;

    lens_radius = aperture / 2.0;
}

inline Ray Camera::GetRay(float64 s, float64 t) const
{
    Vec3 rd = lens_radius * RandomInUnitDiskXY();
    Vec3 offset = u * rd.x + v * rd.y;

    return Ray{ origin + offset, lower_left + horizontal * s + vertical * t - (origin + offset) };
}

} // namespace spt