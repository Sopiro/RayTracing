#include "bulbit/bssrdf.h"
#include "bulbit/bxdfs.h"
#include "bulbit/hash.h"
#include "bulbit/primitive.h"
#include "bulbit/sampling.h"
#include "bulbit/scattering.h"

namespace bulbit
{

Spectrum SeparableBSSRDF::S(const Intersection& pi, const Vec3& wi) const
{
    // Fresnel transmittance when the ray exits a material into direction w_o
    Float F_t = 1 - FresnelDielectric(Dot(wo, po.shading.normal), eta);
    return F_t * Sp(pi) * Sw(pi, wi);
}

Spectrum SeparableBSSRDF::Sw(const Intersection& pi, const Vec3& wi) const
{
    // S_w accounts for the influence of the boundary
    // on the directional distribution of light entering the object from direction w_i
    Frame frame(pi.shading.normal);
    // We don't need w_o
    return sw.f(z_axis, frame.ToLocal(wi));
}

Spectrum SeparableBSSRDF::Sp(const Intersection& pi) const
{
    return Sr(Dist(po.point, pi.point));
}

bool SeparableBSSRDF::Sample_S(
    BSSRDFSample* bssrdf_sample, const Intersectable* accel, int32 wavelength, Float u0, const Point2& u12, Allocator& alloc
)
{
    // Choose projection axis
    Frame f;
    if (u0 < axis_sampling_probabilities[0])
    {
        f = Frame::FromX(po.shading.normal);
    }
    else if (u0 < axis_sampling_probabilities[0] + axis_sampling_probabilities[1])
    {
        f = Frame::FromY(po.shading.normal);
    }
    else
    {
        f = Frame::FromZ(po.shading.normal);
    }

    // Sample scattering distance
    Float r = Sample_Sr(wavelength, u12[0]);
    if (r < 0)
    {
        return false;
    }

    // Uniform in azimuth
    Float phi = two_pi * u12[1];

    Float r_max = MaxSr(wavelength);
    if (r > r_max)
    {
        return false;
    }

    // Compute BSSRDF sampling segment length
    Float l = 2 * std::sqrt(Sqr(r_max) - Sqr(r));

    // Prepare ray for BSSRDF sampling
    Point3 start = po.point + r * (f.x * std::cos(phi) + f.y * std::sin(phi)) - l * f.z / 2;
    Point3 end = start + l * f.z;

    Ray ray(start, Normalize(end - start));

    WeightedReservoirSampler<Intersection> wrs(Hash(start, end));

    Intersection isect;
    while (l > 0 && accel->Intersect(&isect, ray, Ray::epsilon, l))
    {
        if (isect.primitive->GetMaterial() == po.primitive->GetMaterial())
        {
            wrs.Add(isect, 1);
        }

        ray.o = isect.point;
        l -= isect.t;
    }

    if (!wrs.HasSample())
    {
        return false;
    }

    const Intersection& pi = wrs.GetSample();

    bssrdf_sample->pi = pi;
    bssrdf_sample->Sp = Sp(pi);
    bssrdf_sample->pdf = PDF_Sp(pi);
    bssrdf_sample->p = wrs.GetSampleProbability();

    Vec3 n = pi.front_face ? pi.shading.normal : -pi.shading.normal;
    bssrdf_sample->Sw = BSDF(n, &sw);
    bssrdf_sample->wo = n;

    return true;
}

Spectrum SeparableBSSRDF::PDF_Sp(const Intersection& pi) const
{
    Frame f(po.shading.normal);

    Vec3 d = pi.point - po.point;
    Vec3 d_local = f.ToLocal(d);
    Vec3 n_local = f.ToLocal(pi.normal);

    // Radius projected on each sampling axis
    Float projected_r[3] = { std::sqrt(Sqr(d_local.y) + Sqr(d_local.z)), std::sqrt(Sqr(d_local.z) + Sqr(d_local.x)),
                             std::sqrt(Sqr(d_local.x) + Sqr(d_local.y)) };

    // Combine all wavelength dependent sampling probabilities
    Spectrum pdf(0, 0, 0);
    for (int32 axis = 0; axis < 3; ++axis)
    {
        // Sum up Sr(r) * ds cos\theta * p[axis]
        pdf += PDF_Sr(projected_r[axis]) * std::abs(n_local[axis]) * axis_sampling_probabilities[axis];
    }

    return pdf;
}

} // namespace bulbit
