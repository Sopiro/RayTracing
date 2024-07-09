#include "bulbit/camera.h"
#include "bulbit/frame.h"

namespace bulbit
{

SphericalCamera::SphericalCamera(const Point3& position, int32 screen_width, int32 screen_height)
    : Camera(screen_width, screen_height)
    , origin{ position }
{
}

Float SphericalCamera::SampleRay(Ray* ray, const Point2& film_sample, const Point2& aperture_sample) const
{
    Float theta = (1 - film_sample[1] / Float(height)) * pi;
    Float phi = film_sample[0] / Float(width) * two_pi;

    ray->o = origin;
    ray->d = SphericalDirection(theta, phi);

    return 1;
}

} // namespace bulbit
