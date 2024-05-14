#include "bulbit/bxdfs.h"
#include "bulbit/material.h"

namespace bulbit
{

DiffuseMaterial::DiffuseMaterial(const Spectrum& color)
    : albedo{ ConstantColorTexture::Create(color) }
{
}

DiffuseMaterial::DiffuseMaterial(const SpectrumTexture* albedo)
    : albedo{ albedo }
{
}

bool DiffuseMaterial::TestAlpha(const Point2& uv) const
{
    return albedo->EvaluateAlpha(uv) > epsilon;
}

bool DiffuseMaterial::GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const
{
    *bsdf = BSDF(isect.shading.normal, alloc.new_object<DiffuseBxDF>(albedo->Evaluate(isect.uv)));
    return true;
}

} // namespace bulbit
