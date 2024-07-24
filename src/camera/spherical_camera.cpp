#include "bulbit/cameras.h"
#include "bulbit/frame.h"

namespace bulbit
{

SphericalCamera::SphericalCamera(const Vec2i& resolution, const Point3& position)
    : Camera(resolution)
    , origin{ position }
{
}

Float SphericalCamera::SampleRay(Ray* ray, const Point2& film_sample, const Point2& aperture_sample) const
{
    Float theta = (1 - film_sample[1] / Float(resolution.y)) * pi;
    Float phi = film_sample[0] / Float(resolution.x) * two_pi;

    ray->o = origin;
    ray->d = SphericalDirection(theta, phi);

    return 1;
}

} // namespace bulbit
