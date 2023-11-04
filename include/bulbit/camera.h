#pragma once

#include "ray.h"

namespace bulbit
{

class Camera
{
public:
    Camera(int32 screen_width, int32 screen_height);
    virtual ~Camera() = default;

    virtual Float SampleRay(Ray* out_ray, const Point2& film_sample, const Point2& aperture_sample) const = 0;

    int32 GetScreenWidth() const;
    int32 GetScreenHeight() const;

protected:
    int32 width, height;
};

inline Camera::Camera(int32 screen_width, int32 screen_height)
    : width{ screen_width }
    , height{ screen_height }
{
}

inline int32 Camera::GetScreenWidth() const
{
    return width;
}
inline int32 Camera::GetScreenHeight() const
{
    return height;
}

} // namespace bulbit