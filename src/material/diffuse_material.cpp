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

bool DiffuseMaterial::Scatter(Interaction* ir, const Intersection& isect, const Vec3& wo, const Point2& u) const
{
    ir->bsdf = BSDF(isect.shading.normal, isect.shading.tangent, new (ir->mem) DiffuseBxDF(albedo->Evaluate(isect.uv)));
    return true;
}

bool DiffuseMaterial::TestAlpha(const Point2& uv) const
{
    return albedo->EvaluateAlpha(uv) > epsilon;
}

} // namespace bulbit
