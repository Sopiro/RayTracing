#pragma once

#include "frame.h"
#include "intersectable.h"

namespace bulbit
{

// Directional PDF
class PDF
{
public:
    virtual ~PDF() = default;

    // Given an outgoing direction wo, importance sample an incident direction
    virtual Vec3 Sample(const Point2& u) const = 0;

    // Evaluate PDF with given direction
    virtual Float Evaluate(const Vec3& wi) const = 0;
};

// Cosine weighted sampling
class LambertianReflection : public PDF
{
public:
    LambertianReflection(const Vec3& n);

    virtual Vec3 Sample(const Point2& u) const override;
    virtual Float Evaluate(const Vec3& wi) const override;

private:
    Frame frame;
};

// GGX importance sampling
class MicrofacetGGX : public PDF
{
public:
    MicrofacetGGX(const Vec3& n, const Vec3& wo, Float a, Float t);

    virtual Vec3 Sample(const Point2& u) const override;
    virtual Float Evaluate(const Vec3& wi) const override;

private:
    Frame frame;
    Vec3 wo;
    Float alpha2;
    Float t;
};

// GGX VNDF importance sampling
class MicrofacetGGXVNDF : public PDF
{
public:
    MicrofacetGGXVNDF(const Vec3& n, const Vec3& wo, Float a, Float t);

    virtual Vec3 Sample(const Point2& u) const override;
    virtual Float Evaluate(const Vec3& wi) const override;

private:
    Frame frame;
    Vec3 wo;
    Float alpha;
    Float t;
};

} // namespace bulbit