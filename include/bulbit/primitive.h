#pragma once

#include "intersectable.h"
#include "material.h"
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

    if (material->TestAlpha(isect->uv))
    {
        isect->primitive = this;
        return true;
    }
    else
    {
        return false;
    }
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
