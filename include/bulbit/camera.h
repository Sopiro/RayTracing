#pragma once

#include "ray.h"

namespace bulbit
{

class Camera
{
public:
    Camera(const Point2i& resolution)
        : resolution{ resolution }
    {
    }
    virtual ~Camera() = default;

    const Point2i& GetScreenResolution() const;
    int32 GetScreenWidth() const;
    int32 GetScreenHeight() const;

    virtual Float SampleRay(Ray* out_ray, const Point2& film_sample, const Point2& aperture_sample) const = 0;

protected:
    Point2i resolution;
};

inline const Vec2i& Camera::GetScreenResolution() const
{
    return resolution;
}

inline int32 Camera::GetScreenWidth() const
{
    return resolution.x;
}

inline int32 Camera::GetScreenHeight() const
{
    return resolution.y;
}

} // namespace bulbit