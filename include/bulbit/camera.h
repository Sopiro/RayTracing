#pragma once

#include "filters.h"
#include "ray.h"

namespace bulbit
{

class Medium;

class Camera
{
public:
    Camera(const Point2i& resolution, const Medium* medium, const Filter* pixel_filter)
        : resolution{ resolution }
        , medium{ medium }
        , filter{ pixel_filter }
    {
    }
    virtual ~Camera() = default;

    const Point2i& GetScreenResolution() const;
    int32 GetScreenWidth() const;
    int32 GetScreenHeight() const;
    const Medium* GetMedium() const;
    const Filter* GetFilter() const;

    virtual Float SampleRay(Ray* out_ray, const Point2i& pixel, const Point2& u0, const Point2& u1) const = 0;

    static inline std::unique_ptr<Filter> default_filter = std::make_unique<BoxFilter>(1.0f);

protected:
    Point2i resolution;
    const Medium* medium;
    const Filter* filter;
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

inline const Filter* Camera::GetFilter() const
{
    return filter;
}

} // namespace bulbit