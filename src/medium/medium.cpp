#include "bulbit/media.h"

namespace bulbit
{

bool Medium::IsEmissive() const
{
    return Dispatch([](auto medium) { return medium->IsEmissive(); });
}

MediumSample Medium::SamplePoint(Point3 p) const
{
    return Dispatch([&](auto medium) { return medium->SamplePoint(p); });
}

RayMajorantIterator* Medium::SampleRay(Ray ray, Float t_max, Allocator& alloc) const
{
    return Dispatch([&](auto medium) { return medium->SampleRay(ray, t_max, alloc); });
}

} // namespace bulbit
