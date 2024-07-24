#include "bulbit/cameras.h"
#include "bulbit/ray.h"
#include "bulbit/sampling.h"

namespace bulbit
{

OrthographicCamera::OrthographicCamera(
    const Vec2& viewport_size, int32 resolution_x, const Point3& look_from, const Point3& look_at, const Vec3& up
)
    : Camera(Vec2i(resolution_x, int32(resolution_x * viewport_size.y / viewport_size.x)))
{
    w = Normalize(look_from - look_at);
    u = Normalize(Cross(up, w));
    v = Cross(w, u);

    origin = look_from;
    horizontal = viewport_size.x * u;
    vertical = viewport_size.y * v;
    lower_left = origin - horizontal / 2 - vertical / 2;
}

Float OrthographicCamera::SampleRay(Ray* ray, const Point2& film_sample, const Point2& aperture_sample) const
{
    Vec3 pixel_center = lower_left + horizontal * (film_sample.x / resolution.x) + vertical * (film_sample.y / resolution.y);

    ray->o = pixel_center;
    ray->d = -w;

    return 1;
}

} // namespace bulbit
