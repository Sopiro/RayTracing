#pragma once

#include "common.h"
#include "hittable.h"
#include "material.h"

class ConstantDensityMedium : public Hittable
{
public:
    ConstantDensityMedium(std::shared_ptr<Hittable> boundary_object, double density, std::shared_ptr<Texture> albedo)
        : boundary{ boundary_object }
        , neg_inv_density{ -1.0 / density }
        , phase_function{ std::make_shared<Isotropic>(albedo) }
    {
    }

    ConstantDensityMedium(std::shared_ptr<Hittable> boundary_object, double density, Color color)
        : boundary{ boundary_object }
        , neg_inv_density{ -1.0 / density }
        , phase_function{ std::make_shared<Isotropic>(color) }
    {
    }

    virtual bool Hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const override;

public:
    std::shared_ptr<Hittable> boundary;
    std::shared_ptr<Material> phase_function;
    double neg_inv_density;
};

bool ConstantDensityMedium::Hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const
{
    HitRecord rec1;
    HitRecord rec2;

    // Find the closest hit
    if (!boundary->Hit(ray, -infinity, infinity, rec1))
    {
        return false;
    }

    // Find the farthest hit
    if (!boundary->Hit(ray, rec1.t + 0.0001, infinity, rec2))
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

    if (rec1.t < 0)
    {
        rec1.t = 0;
    }

    const auto ray_length = ray.dir.Length();
    const auto distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
    const auto hit_distance = neg_inv_density * log(Rand());

    if (hit_distance > distance_inside_boundary)
    {
        return false;
    }

    rec.t = rec1.t + hit_distance / ray_length;
    rec.p = ray.At(rec.t);
    rec.normal = Vec3{ 1, 0, 0 }; // arbitrary
    rec.front_face = true;        // also arbitrary
    rec.mat = phase_function;

    return true;
}