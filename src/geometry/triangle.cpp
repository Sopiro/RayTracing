#include "raytracer/triangle.h"

namespace spt
{

// Möller-Trumbore algorithm
bool Triangle::Hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const
{
    Vec3 d = ray.dir;
    double l = d.Normalize();
    Vec3 pvec = Cross(d, e2);

    double det = Dot(e1, pvec);

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

    double invDet = 1.0 / det;

    Vec3 tvec = ray.origin - v0.position;
    double u = Dot(tvec, pvec) * invDet;
    if (u < 0.0 || u > 1.0)
    {
        return false;
    }

    Vec3 qvec = Cross(tvec, e1);
    double v = Dot(d, qvec) * invDet;
    if (v < 0.0 || u + v > 1.0)
    {
        return false;
    }

    double t = Dot(e2, qvec) * invDet / l;
    if (t < t_min || t > t_max)
    {
        return false;
    }

    double w = 1.0 - u - v;

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