#include "bulbit/camera.h"
#include "bulbit/ray.h"
#include "bulbit/sampling.h"

namespace bulbit
{

OrthographicCamera::OrthographicCamera(
    const Point3& look_from,
    const Point3& look_at,
    const Vec3& up,
    Float viewport_width,
    Float viewport_height,
    int32 screen_width
)
    : Camera(screen_width, int32(screen_width * viewport_height / viewport_width))
{
    w = Normalize(look_from - look_at);
    u = Normalize(Cross(up, w));
    v = Cross(w, u);

    origin = look_from;
    horizontal = viewport_width * u;
    vertical = viewport_height * v;
    lower_left = origin - horizontal / 2 - vertical / 2;
}

Float OrthographicCamera::SampleRay(Ray* ray, const Point2& film_sample, const Point2& aperture_sample) const
{
    Vec3 pixel_center = lower_left + horizontal * (film_sample.x / width) + vertical * (film_sample.y / height);

    ray->o = pixel_center;
    ray->d = -w;

    return 1;
}

} // namespace bulbit
