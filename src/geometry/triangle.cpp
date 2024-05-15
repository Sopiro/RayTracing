#include "bulbit/triangle.h"

namespace bulbit
{

// Möller-Trumbore algorithm
bool Triangle::Intersect(Intersection* is, const Ray& ray, Float t_min, Float t_max) const
{
    const Point3& p0 = mesh->positions[v[0]];
    const Point3& p1 = mesh->positions[v[1]];
    const Point3& p2 = mesh->positions[v[2]];

    Vec3 e1 = p1 - p0;
    Vec3 e2 = p2 - p0;

    Vec3 d = ray.d;
    Float l = d.Normalize();
    Vec3 pvec = Cross(d, e2);

    Float det = Dot(e1, pvec);
    // bool backface = det < epsilon;

    // Ray and triangle are parallel
    if (std::fabs(det) < epsilon)
    {
        return false;
    }

    Float invDet = 1 / det;

    Vec3 tvec = ray.o - p0;
    Float u = Dot(tvec, pvec) * invDet;
    if (u < 0 || u > 1)
    {
        return false;
    }

    Vec3 qvec = Cross(tvec, e1);
    Float v = Dot(d, qvec) * invDet;
    if (v < 0 || u + v > 1)
    {
        return false;
    }

    Float t = Dot(e2, qvec) * invDet / l;
    if (t < t_min || t > t_max)
    {
        return false;
    }

    // Found intersection

    Float w = 1 - u - v;
    Point2 uv = GetTexCoord(u, v, w);

    if (mesh->material->TestAlpha(uv) == false)
    {
        return false;
    }

    is->primitive = this;
    is->t = t;
    is->point = ray.At(t);
    is->uv = uv;

    Vec3 normal = Normalize(Cross(e1, e2));
    SetFaceNormal(is, ray.d, normal);
    is->shading.normal = GetNormal(u, v, w);
    is->shading.tangent = GetTangent(u, v, w);

    return true;
}

bool Triangle::IntersectAny(const Ray& ray, Float t_min, Float t_max) const
{
    const Point3& p0 = mesh->positions[v[0]];
    const Point3& p1 = mesh->positions[v[1]];
    const Point3& p2 = mesh->positions[v[2]];

    Vec3 e1 = p1 - p0;
    Vec3 e2 = p2 - p0;

    Vec3 d = ray.d;
    Float l = d.Normalize();
    Vec3 pvec = Cross(d, e2);

    Float det = Dot(e1, pvec);
    // bool backface = det < epsilon;

    // Ray and triangle are parallel
    if (std::fabs(det) < epsilon)
    {
        return false;
    }

    Float invDet = 1 / det;

    Vec3 tvec = ray.o - p0;
    Float u = Dot(tvec, pvec) * invDet;
    if (u < 0 || u > 1)
    {
        return false;
    }

    Vec3 qvec = Cross(tvec, e1);
    Float v = Dot(d, qvec) * invDet;
    if (v < 0 || u + v > 1)
    {
        return false;
    }

    Float t = Dot(e2, qvec) * invDet / l;
    if (t < t_min || t > t_max)
    {
        return false;
    }

    Float w = 1 - u - v;
    Point2 uv = GetTexCoord(u, v, w);

    return mesh->material->TestAlpha(uv);
}

PrimitiveSample Triangle::Sample(const Point2& u0) const
{
    const Point3& p0 = mesh->positions[v[0]];
    const Point3& p1 = mesh->positions[v[1]];
    const Point3& p2 = mesh->positions[v[2]];

    Vec3 e1 = p1 - p0;
    Vec3 e2 = p2 - p0;

#if 1
    Float u = u0[0];
    Float v = u0[1];

    if (u + v > 1)
    {
        u = 1 - u;
        v = 1 - v;
    }

    PrimitiveSample sample;
    sample.normal = Cross(e1, e2);
    sample.point = p0 + e1 * u + e2 * v;

    Float area = sample.normal.Normalize() * 0.5f;
    sample.pdf = 1 / area;

    // Float w = 1 - u - v;
    // sample.uv = GetTexCoord(u, v, w);
    return sample;
#else
    Float u1 = u0[0];
    Float u2 = u0[1];

    Float s = std::sqrt(u1);
    Float u = 1.0 - s;
    Float v = u2 * s;

    PrimitiveSample sample;
    sample.normal = Cross(e1, e2);
    sample.point = p0 + e1 * u + e2 * v;

    Float area = sample.normal.Normalize() * 0.5f;
    sample.pdf = 1 / area;

    // Float w = 1 - u - v;
    // sample.uv = GetTexCoord(u, v, w);
    return sample;
#endif
}

PrimitiveSample Triangle::Sample(const Point3& ref, const Point2& u) const
{
    PrimitiveSample sample = Sample(u);

    Vec3 d = sample.point - ref;
    Float distance_squared = Dot(d, d);

    Float cosine = Dot(d, sample.normal) / std::sqrt(distance_squared);
    if (cosine < 0)
    {
        cosine = -cosine;
    }

    sample.pdf *= distance_squared / cosine; // Convert to solid angle measure

    return sample;
}

} // namespace bulbit