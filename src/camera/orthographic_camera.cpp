#include "bulbit/cameras.h"
#include "bulbit/ray.h"
#include "bulbit/sampling.h"

namespace bulbit
{

OrthographicCamera::OrthographicCamera(
    const Point3& look_from,
    const Point3& look_at,
    const Vec3& up,
    const Point2& viewport_size,
    int32 resolution_x,
    const Medium* medium,
    const Filter* pixel_filter
)
    : Camera(Point2i(resolution_x, int32(resolution_x * viewport_size.y / viewport_size.x)), medium, pixel_filter)
{
    w = Normalize(look_from - look_at);
    u = Normalize(Cross(up, w));
    v = Cross(w, u);

    origin = look_from;
    horizontal = viewport_size.x * u;
    vertical = viewport_size.y * v;
    lower_left = origin - horizontal / 2 - vertical / 2;
}

Float OrthographicCamera::SampleRay(Ray* ray, const Point2i& pixel, const Point2& u0, const Point2& u1) const
{
    BulbitNotUsed(u1);

    Point2 pixel_offset = filter->Sample(u0) + Point2(Float(0.5), Float(0.5));
    Point3 pixel_center = lower_left + horizontal * (pixel.x + pixel_offset.x) / resolution.x +
                          vertical * (pixel.y + pixel_offset.y) / resolution.y;

    ray->o = pixel_center;
    ray->d = -w;

    return 1;
}

} // namespace bulbit
