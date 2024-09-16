#include "bulbit/cameras.h"
#include "bulbit/ray.h"
#include "bulbit/sampling.h"

namespace bulbit
{

PerspectiveCamera::PerspectiveCamera(
    const Point3& look_from,
    const Point3& look_at,
    const Vec3& up,
    Float vfov,
    Float aperture,
    Float focus_dist,
    const Point2i& resolution,
    const Medium* medium,
    const Filter* pixel_filter
)
    : Camera(resolution, medium, pixel_filter)
{
    Float theta = DegToRad(vfov);
    Float h = std::tan(theta / 2);
    Float aspect_ratio = (Float)resolution.x / (Float)resolution.y;
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

Float PerspectiveCamera::SampleRay(Ray* ray, const Point2i& pixel, const Point2& u0, const Point2& u1) const
{
    Point2 pixel_offset = filter->Sample(u0) + Point2(Float(0.5), Float(0.5));
    Point3 pixel_center = lower_left + horizontal * (pixel.x + pixel_offset.x) / resolution.x +
                          vertical * (pixel.y + pixel_offset.y) / resolution.y;

    Point3 aperture_sample = lens_radius * SampleUniformUnitDiskXY(u1);
    Point3 camera_offset = u * aperture_sample.x + v * aperture_sample.y;
    Point3 camera_center = origin + camera_offset;

    ray->o = camera_center;
    ray->d = Normalize(pixel_center - camera_center);

    return 1;
}

} // namespace bulbit
