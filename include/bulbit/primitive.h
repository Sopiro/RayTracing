#pragma once

#include "hash.h"
#include "intersectable.h"
#include "materials.h"
#include "medium.h"
#include "shape.h"

namespace bulbit
{

class Primitive : public Intersectable
{
public:
    Primitive(const Shape* shape, const Material* material, const MediumInterface& medium_interface);

    virtual AABB GetAABB() const override;
    virtual bool Intersect(Intersection* out_isect, const Ray& ray, Float t_min, Float t_max) const override;
    virtual bool IntersectAny(const Ray& ray, Float t_min, Float t_max) const override;

    const Shape* GetShape() const;
    const Material* GetMaterial() const;
    const MediumInterface* GetMediumInterface() const;

private:
    const Shape* shape;
    const Material* material;
    MediumInterface medium_interface;
};

inline Primitive::Primitive(const Shape* shape, const Material* material, const MediumInterface& medium_interface)
    : shape{ shape }
    , material{ material }
    , medium_interface{ medium_interface }
{
}

inline AABB Primitive::GetAABB() const
{
    return shape->GetAABB();
}

inline bool Primitive::Intersect(Intersection* isect, const Ray& ray, Float t_min, Float t_max) const
{
    if (!shape->Intersect(isect, ray, t_min, t_max))
    {
        return false;
    }

    // Intersects medium boundary
    if (!material)
    {
        isect->primitive = this;
        return true;
    }

    // Alpha testing
    Float alpha = material->GetAlpha(*isect);
    if (alpha < 1)
    {
        // Avoid hash call as far as possible
        Float p = alpha <= 0 ? 1 : HashFloat(ray, isect->point);
        if (p > alpha)
        {
            // Recursively handle the case of sphere hit
            // Somehow, normalization is needed due to floating point error..
            Ray new_ray(isect->point, Normalize(ray.d));
            return Intersect(isect, new_ray, Ray::epsilon, t_max - isect->t);
        }
    }

    isect->primitive = this;
    return true;
}

inline bool Primitive::IntersectAny(const Ray& ray, Float t_min, Float t_max) const
{
    Intersection isect;
    return Intersect(&isect, ray, t_min, t_max);
}

inline const Shape* Primitive::GetShape() const
{
    return shape;
}

inline const Material* Primitive::GetMaterial() const
{
    return material;
}

inline const MediumInterface* Primitive::GetMediumInterface() const
{
    return &medium_interface;
}

} // namespace bulbit
