#include "bulbit/camera.h"
#include "bulbit/sampling.h"

namespace bulbit
{

Camera::Camera(const Point3& look_from,
               const Point3& look_at,
               const Vec3& up,
               Float vfov, // vertical field-of-view in degrees
               Float aspect_ratio,
               Float aperture,
               Float focus_dist)
{
    Float theta = DegToRad(vfov);
    Float h = std::tan(theta / 2);
    Float viewport_height = 2 * h;
    Float viewport_width = aspect_ratio * viewport_height;

    w = Normalize(look_from - look_at);
    u = Normalize(Cross(up, w));
    v = Cross(w, u);

    origin = look_from;
    horizontal = focus_dist * viewport_width * u;
    vertical = focus_dist * viewport_height * v;
    lower_left = origin - horizontal / 2 - vertical / 2 - focus_dist * w;

    lens_radius = aperture / 2;
}

Ray Camera::GenerateRay(Float s, Float t) const
{
    Vec3 rd = lens_radius * UniformSampleUnitDiskXY();
    Vec3 offset = u * rd.x + v * rd.y;

    Vec3 camera_center = origin + offset;
    Vec3 pixel_center = lower_left + horizontal * s + vertical * t;

    return Ray{ camera_center, Normalize(pixel_center - camera_center) };
}

} // namespace bulbit
