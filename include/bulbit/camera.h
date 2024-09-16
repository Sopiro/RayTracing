#pragma once

#include "filters.h"
#include "ray.h"

namespace bulbit
{

class Medium;

class Camera
{
public:
    static inline std::unique_ptr<Filter> default_filter = std::make_unique<GaussianFilter>(0.5f);

    Camera(const Point2i& resolution, const Medium* medium, const Filter* pixel_filter)
        : resolution{ resolution }
        , medium{ medium }
        , filter{ pixel_filter }
    {
    }
    virtual ~Camera() = default;

    virtual Float SampleRay(Ray* out_ray, const Point2i& pixel, const Point2& u0, const Point2& u1) const = 0;

    const Point2i& GetScreenResolution() const;
    const Medium* GetMedium() const;
    const Filter* GetFilter() const;

protected:
    Point2i resolution;
    const Medium* medium;
    const Filter* filter;
};

inline const Point2i& Camera::GetScreenResolution() const
{
    return resolution;
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