#pragma once

#include "ray.h"

namespace bulbit
{

class Camera
{
public:
    Camera(int32 screen_width, int32 screen_height)
        : width{ screen_width }
        , height{ screen_height }
    {
    }

    virtual ~Camera() = default;

    virtual Float SampleRay(Ray* out_ray, const Point2& film_sample, const Point2& aperture_sample) const = 0;

    int32 GetScreenWidth() const
    {
        return width;
    }

    int32 GetScreenHeight() const
    {
        return height;
    }

protected:
    int32 width, height;
};

class OrthographicCamera : public Camera
{
public:
    OrthographicCamera(
        const Point3& look_from,
        const Point3& look_at,
        const Vec3& up,
        Float viewport_width,
        Float viewport_height,
        int32 screen_width
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
        const Point3& look_from,
        const Point3& look_at,
        const Vec3& up,
        int32 screen_width,
        int32 screen_height,
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
    SphericalCamera(const Point3& position, int32 screen_width, int32 screen_height);

    virtual Float SampleRay(Ray* out_ray, const Point2& film_sample, const Point2& aperture_sample) const override;

private:
    Point3 origin;
};

} // namespace bulbit