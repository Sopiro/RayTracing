#include "raytracer/triangle.h"

// Möller-Trumbore algorithm
bool Triangle::Hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const
{
    Vec3 d = ray.dir;
    double l = d.Normalize();
    Vec3 pvec = Cross(d, v0v2);

    double det = Dot(v0v1, pvec);

    bool backface = det < epsilon;
    if (one_sided && backface)
    {
        return false;
    }

    // Ray and triangle are parallel
    if (Abs(det) < epsilon)
    {
        return false;
    }

    double invDet = 1.0 / det;

    Vec3 tvec = ray.origin - v0;
    double u = Dot(tvec, pvec) * invDet;
    if (u < 0.0 || u > 1.0)
    {
        return false;
    }

    Vec3 qvec = Cross(tvec, v0v1);
    double v = Dot(d, qvec) * invDet;
    if (v < 0.0 || u + v > 1.0)
    {
        return false;
    }

    double t = Dot(v0v2, qvec) * invDet / l;
    if (t < t_min || t > t_max)
    {
        return false;
    }

    rec.mat = material;
    rec.t = t;
    rec.p = ray.At(rec.t);
    rec.SetFaceNormal(ray, normal);
    rec.uv.Set(u, v);

    return true;
}
