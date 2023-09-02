#include "spt/triangle.h"

namespace spt
{

// Möller-Trumbore algorithm
bool Triangle::Intersect(const Ray& ray, f64 t_min, f64 t_max, Intersection& is) const
{
    Vec3 d = ray.dir;
    f64 l = d.Normalize();
    Vec3 pvec = Cross(d, e2);

    f64 det = Dot(e1, pvec);

    bool backface = det < 0.0;
    if (backface == true && two_sided == false)
    {
        return false;
    }

    // Ray and triangle are parallel
    if (Abs(det) < 0.0)
    {
        return false;
    }

    f64 invDet = 1.0 / det;

    Vec3 tvec = ray.origin - v0.position;
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

    is.object = this;
    is.mat = material.get();
    is.t = t;
    is.point = ray.At(is.t);

    Vec3 normal = GetNormal(u, v, w);
    Vec3 tangent = GetTangent(u, v, w);
    is.SetFaceNormal(ray, normal, tangent);

    UV tex = GetTexCoord(u, v, w);
    is.uv = tex;

    return true;
}

} // namespace spt