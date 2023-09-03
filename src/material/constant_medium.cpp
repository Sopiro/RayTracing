#include "spt/constant_medium.h"

namespace spt
{

bool ConstantDensityMedium::Intersect(const Ray& ray, f64 t_min, f64 t_max, Intersection& is) const
{
    Intersection is1, is2;

    // Find the closest hit
    if (boundary->Intersect(ray, -infinity, infinity, is1) == false)
    {
        return false;
    }

    // Find the farthest hit
    if (boundary->Intersect(ray, is1.t + ray_offset, infinity, is2) == false)
    {
        return false;
    }

    if (is1.t < t_min)
    {
        is1.t = t_min;
    }

    if (is2.t > t_max)
    {
        is2.t = t_max;
    }

    if (is1.t >= is2.t)
    {
        return false;
    }

    if (is1.t < 0.0)
    {
        is1.t = 0.0;
    }

    f64 ray_length = ray.dir.Length();
    f64 distance_inside_boundary = (is2.t - is1.t) * ray_length;
    f64 hit_distance = neg_inv_density * log(Rand());

    if (hit_distance > distance_inside_boundary)
    {
        return false;
    }

    is.object = this;
    is.t = is1.t + hit_distance / ray_length;
    is.point = ray.At(is.t);
    is.normal = Vec3{ 1.0, 0.0, 0.0 }; // arbitrary
    is.front_face = true;              // also arbitrary

    return true;
}

} // namespace spt