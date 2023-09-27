#include "spt/triangle.h"

namespace spt
{

// Möller-Trumbore algorithm
bool Triangle::Intersect(Intersection* is, const Ray& ray, f64 t_min, f64 t_max) const
{
    const Point3& p0 = mesh->positions[v[0]];
    const Point3& p1 = mesh->positions[v[1]];
    const Point3& p2 = mesh->positions[v[2]];

    Vec3 e1 = p1 - p0;
    Vec3 e2 = p2 - p0;

    Vec3 d = ray.d;
    f64 l = d.Normalize();
    Vec3 pvec = Cross(d, e2);

    f64 det = Dot(e1, pvec);

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

    f64 invDet = 1.0 / det;

    Vec3 tvec = ray.o - p0;
    f64 u = Dot(tvec, pvec) * invDet;
    if (u < 0.0 || u > 1.0)
    {
        return false;
    }

    Vec3 qvec = Cross(tvec, e1);
    f64 v = Dot(d, qvec) * invDet;
    if (v < 0.0 || u + v > 1.0)
    {
        return false;
    }

    f64 t = Dot(e2, qvec) * invDet / l;
    if (t < t_min || t > t_max)
    {
        return false;
    }

    f64 w = 1.0 - u - v;

    // Found intersection
    is->object = this;
    is->material = GetMaterial();
    is->t = t;
    is->point = ray.At(t);

    Vec3 normal = Normalize(Cross(e1, e2));
    Vec3 shading_normal = GetNormal(u, v, w);
    Vec3 shading_tangent = GetTangent(u, v, w);
    SetFaceNormal(is, ray.d, normal, shading_normal, shading_tangent);

    is->uv = GetTexCoord(u, v, w);

    return true;
}

bool Triangle::IntersectAny(const Ray& ray, f64 t_min, f64 t_max) const
{
    const Point3& p0 = mesh->positions[v[0]];
    const Point3& p1 = mesh->positions[v[1]];
    const Point3& p2 = mesh->positions[v[2]];

    Vec3 e1 = p1 - p0;
    Vec3 e2 = p2 - p0;

    Vec3 d = ray.d;
    f64 l = d.Normalize();
    Vec3 pvec = Cross(d, e2);

    f64 det = Dot(e1, pvec);

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

    f64 invDet = 1.0 / det;

    Vec3 tvec = ray.o - p0;
    f64 u = Dot(tvec, pvec) * invDet;
    if (u < 0.0 || u > 1.0)
    {
        return false;
    }

    Vec3 qvec = Cross(tvec, e1);
    f64 v = Dot(d, qvec) * invDet;
    if (v < 0.0 || u + v > 1.0)
    {
        return false;
    }

    f64 t = Dot(e2, qvec) * invDet / l;
    if (t < t_min || t > t_max)
    {
        return false;
    }

    return true;
}

void Triangle::Sample(Intersection* sample, f64* pdf) const
{
    const Point3& p0 = mesh->positions[v[0]];
    const Point3& p1 = mesh->positions[v[1]];
    const Point3& p2 = mesh->positions[v[2]];

    Vec3 e1 = p1 - p0;
    Vec3 e2 = p2 - p0;

#if 1
    f64 u = Rand(0.0, 1.0);
    f64 v = Rand(0.0, 1.0);

    if (u + v > 1.0)
    {
        u = 1.0 - u;
        v = 1.0 - v;
    }

    Vec3 normal = Cross(e1, e2);
    f64 area = normal.Normalize() * 0.5;
    Point3 point = p0 + e1 * u + e2 * v;

    sample->normal = normal;
    sample->point = point;
    *pdf = 1.0 / area;

    f64 w = 1.0 - u - v;
    sample->uv = GetTexCoord(u, v, w);

#else
    f64 u1 = Rand(0.0, 1.0);
    f64 u2 = Rand(0.0, 1.0);

    f64 s = std::sqrt(u1);
    f64 u = 1.0 - s;
    f64 v = u2 * s;

    Vec3 normal = Cross(e1, e2);
    f64 area = normal.Normalize() * 0.5;
    Point3 point = p0 + e1 * u + e2 * v;

    sample->n = normal;
    sample->p = point;
    sample->pdf = 1.0 / area;
#endif
}

void Triangle::Sample(Intersection* sample, f64* pdf, Vec3* ref2p, const Point3& ref) const
{
    Sample(sample, pdf);

    Vec3 d = sample->point - ref;
    f64 distance_squared = Dot(d, d);

    f64 cosine = Dot(d, sample->normal) / std::sqrt(distance_squared);
    if (cosine > 0.0)
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

    sample->object = this;
    sample->material = GetMaterial();
}

} // namespace spt