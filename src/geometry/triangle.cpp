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
    // if (backface)
    // {
    //     return false;
    // }

    // Ray and triangle are parallel
    if (std::fabs(det) < epsilon)
    {
        return false;
    }

    Float invDet = 1 / det;

    Vec3 tvec = ray.o - p0;
    Float u = Dot(tvec, pvec) * invDet;
    if (u < 0.0 || u > 1.0)
    {
        return false;
    }

    Vec3 qvec = Cross(tvec, e1);
    Float v = Dot(d, qvec) * invDet;
    if (v < 0.0 || u + v > 1.0)
    {
        return false;
    }

    Float t = Dot(e2, qvec) * invDet / l;
    if (t < t_min || t > t_max)
    {
        return false;
    }

    Float w = 1 - u - v;

    // Found intersection
    is->material = GetMaterial();
    is->t = t;
    is->point = ray.At(t);

    Vec3 normal = Normalize(Cross(e1, e2));
    Vec3 shading_normal = GetNormal(u, v, w);
    Vec3 shading_tangent = GetTangent(u, v, w);
    SetFaceNormal(is, ray.d, normal);
    is->shading.normal = shading_normal;
    is->shading.tangent = shading_tangent;

    is->uv = GetTexCoord(u, v, w);

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
    // if (backface)
    // {
    //     return false;
    // }

    // Ray and triangle are parallel
    if (std::fabs(det) < epsilon)
    {
        return false;
    }

    Float invDet = 1 / det;

    Vec3 tvec = ray.o - p0;
    Float u = Dot(tvec, pvec) * invDet;
    if (u < 0.0 || u > 1.0)
    {
        return false;
    }

    Vec3 qvec = Cross(tvec, e1);
    Float v = Dot(d, qvec) * invDet;
    if (v < 0.0 || u + v > 1.0)
    {
        return false;
    }

    Float t = Dot(e2, qvec) * invDet / l;
    if (t < t_min || t > t_max)
    {
        return false;
    }

    return true;
}

void Triangle::Sample(Intersection* sample, Float* pdf, const Point2& u0) const
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

    Vec3 normal = Cross(e1, e2);
    Float area = normal.Normalize() * Float(0.5);
    Point3 point = p0 + e1 * u + e2 * v;

    sample->normal = normal;
    sample->point = point;
    *pdf = 1 / area;

    Float w = 1 - u - v;
    sample->uv = GetTexCoord(u, v, w);

#else
    Float u1 = u[0];
    Float u2 = u[1];

    Float s = std::sqrt(u1);
    Float u = 1.0 - s;
    Float v = u2 * s;

    Vec3 normal = Cross(e1, e2);
    Float area = normal.Normalize() * 0.5;
    Point3 point = p0 + e1 * u + e2 * v;

    sample->n = normal;
    sample->p = point;
    sample->pdf = 1.0 / area;
#endif
}

void Triangle::Sample(Intersection* sample, Float* pdf, Vec3* ref2p, const Point3& ref, const Point2& u) const
{
    Sample(sample, pdf, u);

    Vec3 d = sample->point - ref;
    Float distance_squared = Dot(d, d);

    Float cosine = Dot(d, sample->normal) / std::sqrt(distance_squared);
    if (cosine > 0)
    {
        sample->front_face = false;
        sample->normal.Negate();
    }
    else
    {
        sample->front_face = true;
        cosine = -cosine;
    }

    *pdf *= distance_squared / cosine; // Convert to solid angle measure
    *ref2p = d;

    sample->material = GetMaterial();
}

} // namespace bulbit