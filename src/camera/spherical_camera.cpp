#include "bulbit/cameras.h"
#include "bulbit/frame.h"

namespace bulbit
{

SphericalCamera::SphericalCamera(
    const Point3& position, const Vec2i& resolution, const Medium* medium, const Filter* pixel_filter
)
    : Camera(resolution, medium, pixel_filter)
    , origin{ position }
{
}

Float SphericalCamera::SampleRay(Ray* ray, const Point2i& pixel, const Point2& u0, const Point2& u1) const
{
    BulbitNotUsed(pixel);
    BulbitNotUsed(u1);

    Float theta = (1 - u0[1] / Float(resolution.y)) * pi;
    Float phi = u0[0] / Float(resolution.x) * two_pi;

    ray->o = origin;
    ray->d = SphericalDirection(theta, phi);

    return 1;
}

} // namespace bulbit
