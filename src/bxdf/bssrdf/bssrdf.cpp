#include "bulbit/bssrdf.h"
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
    BSSRDFSample* bssrdf_sample, const Intersectable* accel, Float u1, const Point2& u2, Allocator& alloc
) const
{
    return false;
}

bool SeparableBSSRDF::Sample_Sp(
    BSSRDFSample* bssrdf_sample, const Intersectable* accel, Float u1, const Point2& u2, Allocator& alloc
) const
{
    return false;
}

Float SeparableBSSRDF::PDF_Sp(const Intersection& pi) const
{
    return 0;
}

} // namespace bulbit
