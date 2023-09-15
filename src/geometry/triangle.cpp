#include "spt/triangle.h"

namespace spt
{

// Möller-Trumbore algorithm
bool Triangle::Intersect(Intersection* is, const Ray& ray, f64 t_min, f64 t_max) const
{
    const Vec3& p0 = mesh->vertices[v[0]].position;
    const Vec3& p1 = mesh->vertices[v[1]].position;
    const Vec3& p2 = mesh->vertices[v[2]].position;

    Vec3 e1 = p1 - p0;
    Vec3 e2 = p2 - p0;

    Vec3 d = ray.dir;
    f64 l = d.Normalize();
    Vec3 pvec = Cross(d, e2);

    f64 det = Dot(e1, pvec);

    bool backface = det < epsilon;
    if (backface == true && two_sided == false)
    {
        return false;
    }

    // Ray and triangle are parallel
    if (Abs(det) < epsilon)
    {
        return false;
    }

    f64 invDet = 1.0 / det;

    Vec3 tvec = ray.origin - p0;
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

    Vec3 normal = GetShadingNormal(u, v, w);
    Vec3 tangent = GetShadingTangent(u, v, w);
    SetFaceNormal(is, ray, normal, tangent);

    UV tex = GetTexCoord(u, v, w);
    is->uv = tex;

    return true;
}

bool Triangle::IntersectAny(const Ray& ray, f64 t_min, f64 t_max) const
{
    const Vec3& p0 = mesh->vertices[v[0]].position;
    const Vec3& p1 = mesh->vertices[v[1]].position;
    const Vec3& p2 = mesh->vertices[v[2]].position;

    Vec3 e1 = p1 - p0;
    Vec3 e2 = p2 - p0;

    Vec3 d = ray.dir;
    f64 l = d.Normalize();
    Vec3 pvec = Cross(d, e2);

    f64 det = Dot(e1, pvec);

    bool backface = det < epsilon;
    if (backface == true && two_sided == false)
    {
        return false;
    }

    // Ray and triangle are parallel
    if (Abs(det) < epsilon)
    {
        return false;
    }

    f64 invDet = 1.0 / det;

    Vec3 tvec = ray.origin - p0;
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

} // namespace spt