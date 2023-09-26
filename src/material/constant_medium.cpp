#include "spt/constant_medium.h"

namespace spt
{

ConstantDensityMedium::ConstantDensityMedium(const Ref<Intersectable> boundary_object, f64 density, const Ref<Texture> albedo)
    : boundary{ std::move(boundary_object) }
    , neg_inv_density{ -1.0 / density }
    , phase_function{ CreateSharedRef<Isotropic>(albedo) }
{
}

ConstantDensityMedium::ConstantDensityMedium(const Ref<Intersectable> boundary_object, f64 density, Color color)
    : boundary{ boundary_object }
    , neg_inv_density{ -1.0 / density }
    , phase_function{ CreateSharedRef<Isotropic>(color) }
{
}

bool ConstantDensityMedium::Intersect(Intersection* is, const Ray& ray, f64 t_min, f64 t_max) const
{
    Intersection is1, is2;

    // Find the closest hit
    if (boundary->Intersect(&is1, ray, t_min, infinity) == false)
    {
        return false;
    }

    if (is1.front_face)
    {
        // Find the closest boundary
        if (boundary->Intersect(&is2, ray, is1.t + Ray::epsilon, infinity) == false)
        {
            return false;
        }

        if (is2.t > t_max)
        {
            is2.t = t_max;
        }
    }
    else
    {
        // Ray origin is inside the medium
        is2.t = is1.t;
        is1.t = t_min;
    }

    f64 ray_length = ray.d.Length();
    f64 distance_inside_boundary = (is2.t - is1.t) * ray_length;
    f64 hit_distance = neg_inv_density * log(Rand());

    if (hit_distance > distance_inside_boundary)
    {
        return false;
    }

    is->object = this;
    is->material = phase_function.get();
    is->t = is1.t + hit_distance / ray_length;
    is->point = ray.At(is->t);
    is->normal = y_axis;   // arbitrary
    is->front_face = true; // also arbitrary

    return true;
}

} // namespace spt