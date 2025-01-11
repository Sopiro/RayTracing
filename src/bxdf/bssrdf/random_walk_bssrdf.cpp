#include "bulbit/bssrdf.h"
#include "bulbit/hash.h"
#include "bulbit/primitive.h"

namespace bulbit
{

Spectrum RandomWalkBSSRDF::S(const Intersection& pi, const Vec3& wi) const
{
    BulbitNotUsed(pi);
    BulbitNotUsed(wi);
    BulbitAssert(false);

    return Spectrum::black;
}
bool RandomWalkBSSRDF::Sample_S(
    BSSRDFSample* bssrdf_sample, const Intersectable* accel, int32 wavelength, Float u0, const Point2& u12
)
{
    BulbitNotUsed(wavelength);

    RNG rng(Hash(po, u0, u12));

    Point3 p = po.point;

    Frame f(po.normal);
    Vec3 wo = SampleCosineHemisphere({ rng.NextFloat(), rng.NextFloat() });
    wo = -Normalize(f.FromLocal(wo));

    Intersection isect;

    int32 bounce = 0;
    const int32 max_bounces = 64;

    while (bounce++ < max_bounces)
    {
        Float l = SampleExponential(rng.NextFloat(), sigma_t[0]);

        bool found_intersection = accel->Intersect(&isect, Ray{ p, wo }, Ray::epsilon, l);
        if (found_intersection)
        {
            if (isect.primitive->GetMaterial() == po.primitive->GetMaterial())
            {
                break;
            }

            return false;
        }

        p += l * wo;

        wo = SampleUniformSphere({ rng.NextFloat(), rng.NextFloat() });
    }

    if (bounce >= max_bounces)
    {
        return false;
    }

    bssrdf_sample->pi = isect;
    bssrdf_sample->Sp = Sp;
    bssrdf_sample->pdf = Spectrum(1);
    bssrdf_sample->p = 1;

    Vec3 n = -isect.shading.normal;
    bssrdf_sample->Sw = BSDF(n, &sw);
    bssrdf_sample->wo = n;
    return true;
}

} // namespace bulbit
