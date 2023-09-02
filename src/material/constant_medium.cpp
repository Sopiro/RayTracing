#include "spt/constant_medium.h"

namespace spt
{

bool ConstantDensityMedium::Intersect(const Ray& ray, f64 t_min, f64 t_max, Intersection& is) const
{
    Intersection itst1, itst2;

    // Find the closest hit
    if (boundary->Intersect(ray, -infinity, infinity, itst1) == false)
    {
        return false;
    }

    // Find the farthest hit
    if (boundary->Intersect(ray, itst1.t + ray_offset, infinity, itst2) == false)
    {
        return false;
    }

    if (itst1.t < t_min)
    {
        itst1.t = t_min;
    }

    if (itst2.t > t_max)
    {
        itst2.t = t_max;
    }

    if (itst1.t >= itst2.t)
    {
        return false;
    }

    if (itst1.t < 0.0)
    {
        itst1.t = 0.0;
    }

    f64 ray_length = ray.dir.Length();
    f64 distance_inside_boundary = (itst2.t - itst1.t) * ray_length;
    f64 hit_distance = neg_inv_density * log(Rand());

    if (hit_distance > distance_inside_boundary)
    {
        return false;
    }

    is.object = this;
    is.mat = phase_function.get();
    is.t = itst1.t + hit_distance / ray_length;
    is.point = ray.At(is.t);
    is.normal = Vec3{ 1.0, 0.0, 0.0 }; // arbitrary
    is.front_face = true;              // also arbitrary

    return true;
}

} // namespace spt