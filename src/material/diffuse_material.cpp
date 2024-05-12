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

bool DiffuseMaterial::Scatter(Interaction* ir, const Intersection& is, const Vec3& wo, const Point2& u) const
{
    ir->bsdf = BSDF(is.shading.normal, is.shading.tangent, new (ir->mem) DiffuseBxDF(albedo->Evaluate(is.uv)));
    return true;
}

bool DiffuseMaterial::TestAlpha(const Point2& uv) const
{
    return albedo->EvaluateAlpha(uv) > epsilon;
}

} // namespace bulbit
