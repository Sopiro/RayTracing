#pragma once

#include "camera.h"

namespace bulbit
{

class OrthographicCamera : public Camera
{
public:
    OrthographicCamera(
        const Vec2& viewport_size, int32 resolution_x, const Point3& look_from, const Point3& look_at, const Vec3& up
    );

    virtual Float SampleRay(Ray* out_ray, const Point2& film_sample, const Point2& aperture_sample) const override;

private:
    Point3 origin;
    Point3 lower_left;
    Vec3 horizontal, vertical;

    // Local coordinate frame
    Vec3 u, v, w;
};

class PerspectiveCamera : public Camera
{
public:
    PerspectiveCamera(
        const Vec2i& resolution,
        const Point3& look_from,
        const Point3& look_at,
        const Vec3& up,
        Float vfov, // vertical field-of-view. in degrees.
        Float aperture,
        Float focus_dist
    );

    virtual Float SampleRay(Ray* out_ray, const Point2& film_sample, const Point2& aperture_sample) const override;

private:
    Point3 origin;
    Point3 lower_left;
    Vec3 horizontal, vertical;

    Float lens_radius;

    // Local coordinate frame
    Vec3 u, v, w;
};

class SphericalCamera : public Camera
{
public:
    SphericalCamera(const Vec2i& resolution, const Point3& position);

    virtual Float SampleRay(Ray* out_ray, const Point2& film_sample, const Point2& aperture_sample) const override;

private:
    Point3 origin;
};

} // namespace bulbit
