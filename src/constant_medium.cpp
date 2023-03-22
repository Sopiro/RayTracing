#include "raytracer/constant_medium.h"

namespace spt
{

bool ConstantDensityMedium::Hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const
{
    HitRecord rec1;
    HitRecord rec2;

    // Find the closest hit
    if (boundary->Hit(ray, -infinity, infinity, rec1) == false)
    {
        return false;
    }

    // Find the farthest hit
    if (boundary->Hit(ray, rec1.t + ray_tolerance, infinity, rec2) == false)
    {
        return false;
    }

    if (rec1.t < t_min)
    {
        rec1.t = t_min;
    }

    if (rec2.t > t_max)
    {
        rec2.t = t_max;
    }

    if (rec1.t >= rec2.t)
    {
        return false;
    }

    if (rec1.t < 0.0)
    {
        rec1.t = 0.0;
    }

    double ray_length = ray.dir.Length();
    double distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
    double hit_distance = neg_inv_density * log(Rand());

    if (hit_distance > distance_inside_boundary)
    {
        return false;
    }

    rec.t = rec1.t + hit_distance / ray_length;
    rec.point = ray.At(rec.t);
    rec.normal = Vec3{ 1.0, 0.0, 0.0 }; // arbitrary
    rec.front_face = true;              // also arbitrary
    rec.mat = phase_function;

    return true;
}

} // namespace spt