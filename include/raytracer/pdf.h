#pragma once

#include "common.h"
#include "hittable.h"
#include "onb.h"

namespace spt
{

inline double BalanceHeuristic(double pdf_f, double pdf_g)
{
    return pdf_f / (pdf_f + pdf_g);
}

inline double PowerHeuristic(double pdf_f, double pdf_g)
{
    double f2 = pdf_f * pdf_f;
    double g2 = pdf_g * pdf_g;
    return f2 / (f2 + g2);
}

// Probability distribution function
class PDF
{
public:
    virtual ~PDF() = default;

    virtual Vec3 Generate() const = 0;
    virtual double Evaluate(const Vec3& direction) const = 0;
};

class CosinePDF : public PDF
{
public:
    CosinePDF(const Vec3& w)
        : uvw{ w }
    {
    }

    virtual Vec3 Generate() const override
    {
        return uvw.GetLocal(RandomCosineDirection());
    }

    virtual double Evaluate(const Vec3& direction) const override
    {
        double cosine = Dot(direction.Normalized(), uvw.w);
        return (cosine <= 0.0) ? 0.0 : cosine / pi;
    }

public:
    ONB uvw;
};

extern double D_GGX(double NoH, double roughness);

class GGXPDF : public PDF
{
    // https://agraphicsguynotes.com/posts/sample_microfacet_brdf/
    // https://schuttejoe.github.io/post/ggximportancesamplingpart1/

public:
    GGXPDF(const Vec3& n, const Vec3& wi, double roughness, double t)
        : uvw{ n.Normalized() }
        , wi{ -wi.Normalized() }
        , alpha{ roughness }
        , t{ t }
    {
    }

    virtual Vec3 Generate() const override
    {
        if (Rand() < t)
        {
            double u = Rand();
            double theta = acos(sqrt((1.0 - u) / ((alpha * alpha - 1.0) * u + 1.0)));
            double phi = 2.0 * pi * Rand();

            double sin_thetha = sin(theta);
            double x = cos(phi) * sin_thetha;
            double y = sin(phi) * sin_thetha;
            double z = cos(theta);

            // sampled half vector
            Vec3 h{ x, y, Abs(z) };
            Vec3 wo = Reflect(-wi, uvw.GetLocal(h));
            return wo.Normalized();
        }
        else
        {
            Vec3 random_cosine = RandomCosineDirection();
            random_cosine.z = Abs(random_cosine.z);

            return uvw.GetLocal(random_cosine);
        }
    }

    virtual double Evaluate(const Vec3& wo) const override
    {
        Vec3 h = (wi + wo).Normalized();
        double NoH = Dot(uvw.w, h);
        double spec_w = D_GGX(NoH, alpha) * NoH / (4.0 * Max(Dot(wo, h), 0.00001));

        double cosine = Dot(wo.Normalized(), uvw.w);
        double diff_w = cosine / pi;

        return (1.0 - t) * diff_w + t * spec_w;
    }

public:
    ONB uvw;
    Vec3 wi;
    double alpha;
    double t;
};

class HittablePDF : public PDF
{
public:
    HittablePDF(const Hittable* target, const Vec3& origin)
        : target{ target }
        , origin{ origin }
    {
    }

    // Returns random direction vector hitting this object
    virtual Vec3 Generate() const override
    {
        return target->GetRandomDirection(origin);
    }

    virtual double Evaluate(const Vec3& direction) const override
    {
        return target->EvaluatePDF(Ray{ origin, direction });
    }

public:
    Vec3 origin;
    const Hittable* target;
};

class MixturePDF : public PDF
{
public:
    MixturePDF(PDF* pdf1, PDF* pdf2)
        : p1{ pdf1 }
        , p2{ pdf2 }
    {
    }

    virtual Vec3 Generate() const override
    {
        if (Rand() > 0.5)
        {
            return p1->Generate();
        }
        else
        {
            return p2->Generate();
        }
    }

    virtual double Evaluate(const Vec3& direction) const override
    {
        // Mixing two pdfs
        return 0.5 * p1->Evaluate(direction) + 0.5 * p2->Evaluate(direction);
    }

public:
    PDF* p1;
    PDF* p2;
};

} // namespace spt