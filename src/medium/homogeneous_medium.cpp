#include "bulbit/media.h"

namespace bulbit
{

HomogeneousMedium::HomogeneousMedium(Spectrum sigma_a, Spectrum sigma_s, Spectrum Le, Float g)
    : sigma_a{ sigma_a }
    , sigma_s{ sigma_s }
    , Le{ Le }
    , phase{ g }
{
}

bool HomogeneousMedium::IsEmissive() const
{
    return !Le.IsBlack();
}

MediumSample HomogeneousMedium::SamplePoint(Point3 p) const
{
    return MediumSample{ sigma_a, sigma_s, Le, &phase };
}

RayMajorantIterator* HomogeneousMedium::SampleRay(Ray ray, Float t_max, Allocator& alloc) const
{
    return alloc.new_object<HomogeneousMajorantIterator>(Float(0), t_max, sigma_a + sigma_s);
}

} // namespace bulbit
