#pragma once

#include "material.h"

namespace bulbit
{

// Conductor
class Metal : public Material
{
public:
    Metal(const Spectrum& albedo, Float fuzziness);

    virtual bool Scatter(Interaction* out_ir, const Intersection& is, const Vec3& wi, const Point2& u) const override;

public:
    Spectrum albedo;
    Float fuzziness;
};

} // namespace bulbit
