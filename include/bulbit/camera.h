#pragma once

#include "ray.h"

namespace bulbit
{

class Medium;

class Camera
{
public:
    Camera(const Point2i& resolution, const Medium* medium)
        : resolution{ resolution }
        , medium{ medium }
    {
    }
    virtual ~Camera() = default;

    const Point2i& GetScreenResolution() const;
    int32 GetScreenWidth() const;
    int32 GetScreenHeight() const;

    const Medium* GetMedium() const;

    virtual Float SampleRay(Ray* out_ray, const Point2& film_sample, const Point2& aperture_sample) const = 0;

protected:
    Point2i resolution;
    const Medium* medium;
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

inline const Medium* Camera::GetMedium() const
{
    return medium;
}

} // namespace bulbit