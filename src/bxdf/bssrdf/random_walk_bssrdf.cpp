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
    RNG rng(Hash(po, u0, u12));

    Vec3 n = po.front_face ? po.normal : -po.normal;

    Frame f(n);
    Vec3 wi = SampleCosineHemisphere({ rng.NextFloat(), rng.NextFloat() });
    wi = -Normalize(f.FromLocal(wi));

    Intersection pi;
    Ray ray(po.point, wi);

    Float weight = 1;
    int32 bounce = 0;
    const int32 max_bounces = 256;

    while (bounce++ < max_bounces)
    {
        Float l = SampleExponential(rng.NextFloat(), sigma_t[wavelength]);

        bool found_intersection = accel->Intersect(&pi, ray, Ray::epsilon, l);
        if (found_intersection)
        {
            if (pi.primitive->GetMaterial() == po.primitive->GetMaterial())
            {
                break;
            }
        }

        // Stochastically terminate path with russian roulette
        const Float rr = 0.995f;
        if (rng.NextFloat() > rr)
        {
            return false;
        }
        weight /= rr;

        // Random walk forward
        ray.o += l * ray.d;

        PhaseFunctionSample sample;
        if (!phase_function.Sample_p(&sample, -ray.d, { rng.NextFloat(), rng.NextFloat() }))
        {
            return false;
        }
        ray.d = sample.wi;
    }

    if (bounce >= max_bounces)
    {
        return false;
    }

    bssrdf_sample->pi = pi;
    bssrdf_sample->Sp = Spectrum(0);
    bssrdf_sample->Sp[wavelength] = weight * R[wavelength];

    bssrdf_sample->pdf = Spectrum(0);
    bssrdf_sample->pdf[wavelength] = 1;
    bssrdf_sample->p = 1;

    n = pi.front_face ? pi.shading.normal : -pi.shading.normal;
    bssrdf_sample->Sw = BSDF(n, &sw);
    bssrdf_sample->wo = n;
    return true;
}

} // namespace bulbit
