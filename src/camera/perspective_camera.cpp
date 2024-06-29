#include "bulbit/camera.h"
#include "bulbit/ray.h"
#include "bulbit/sampling.h"

namespace bulbit
{

PerspectiveCamera::PerspectiveCamera(const Point3& look_from,
                                     const Point3& look_at,
                                     const Vec3& up,
                                     int32 screen_width,
                                     int32 screen_height,
                                     Float vfov,
                                     Float aperture,
                                     Float focus_dist)
    : Camera(screen_width, screen_height)
{
    Float theta = DegToRad(vfov);
    Float h = std::tan(theta / 2);
    Float aspect_ratio = (Float)width / (Float)height;
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

Float PerspectiveCamera::SampleRay(Ray* out_ray, const Point2& film_sample, const Point2& aperture_sample) const
{
    Vec3 rd = lens_radius * SampleUniformUnitDiskXY(aperture_sample);
    Vec3 offset = u * rd.x + v * rd.y;

    Vec3 camera_center = origin + offset;
    Vec3 pixel_center = lower_left + horizontal * (film_sample.x / width) + vertical * (film_sample.y / height);

    out_ray->o = camera_center;
    out_ray->d = Normalize(pixel_center - camera_center);

    return 1;
}

} // namespace bulbit
