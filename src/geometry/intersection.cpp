#include "bulbit/bsdf.h"
#include "bulbit/material.h"
#include "bulbit/primitive.h"

namespace bulbit
{

Spectrum Intersection::Le(const Vec3& wo) const
{
    return primitive->GetMaterial()->Le(*this, wo);
}

bool Intersection::GetBSDF(BSDF* bsdf, const Vec3& wo, Allocator& alloc) const
{
    // todo: handle normal mapping here
    return primitive->GetMaterial()->GetBSDF(bsdf, *this, wo, alloc);
}

} // namespace  bulbit
