#include "spt/triangle.h"

namespace spt
{

// Möller-Trumbore algorithm
bool Triangle::Hit(const Ray& ray, float64 t_min, float64 t_max, HitRecord& rec) const
{
    Vec3 d = ray.dir;
    float64 l = d.Normalize();
    Vec3 pvec = Cross(d, e2);

    float64 det = Dot(e1, pvec);

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

    float64 invDet = 1.0 / det;

    Vec3 tvec = ray.origin - v0.position;
    float64 u = Dot(tvec, pvec) * invDet;
    if (u < 0.0 || u > 1.0)
    {
        return false;
    }

    Vec3 qvec = Cross(tvec, e1);
    float64 v = Dot(d, qvec) * invDet;
    if (v < 0.0 || u + v > 1.0)
    {
        return false;
    }

    float64 t = Dot(e2, qvec) * invDet / l;
    if (t < t_min || t > t_max)
    {
        return false;
    }

    float64 w = 1.0 - u - v;

    rec.object = this;
    rec.mat = material.get();
    rec.t = t;
    rec.point = ray.At(rec.t);

    Vec3 normal = GetNormal(u, v, w);
    Vec3 tangent = GetTangent(u, v, w);
    rec.SetFaceNormal(ray, normal, tangent);

    UV tex = GetTexCoord(u, v, w);
    rec.uv = tex;

    return true;
}

} // namespace spt