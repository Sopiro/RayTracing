#pragma once

#include "bitmap.h"
#include "camera.h"
#include "post_process.h"
#include "spectrum.h"

namespace bulbit
{

class Film
{
public:
    Film(const Point2i& resolution);

    void AddSample(int32 x, int32 y, const Spectrum& L, Float weight);
    Bitmap ConvertToBitmap() const;

    const Point2i resolution;

private:
    std::unique_ptr<Spectrum[]> samples;
    std::unique_ptr<Float[]> weights;
};

inline Film::Film(const Point2i& resolution)
    : resolution{ resolution }
{
    int32 width = resolution.x;
    int32 height = resolution.y;

    samples = std::make_unique<Spectrum[]>(width * height);
    weights = std::make_unique<Float[]>(width * height);

    for (int32 i = 0; i < width * height; ++i)
    {
        samples[i] = Spectrum::black;
        weights[i] = 0.0f;
    }
}

inline void Film::AddSample(int32 x, int32 y, const Spectrum& L, Float w)
{
    samples[x + y * resolution.x] += L;
    weights[x + y * resolution.x] += w;
}

inline Bitmap Film::ConvertToBitmap() const
{
    int32 width = resolution.x;
    int32 height = resolution.y;
    Bitmap bitmap(width, height);

    for (int32 i = 0; i < width * height; ++i)
    {
        Spectrum s = samples[i] / weights[i];
        s = Tonemap_ACES(s);
        s = ToSRGB(s);

        bitmap.Set(i, s);
    }

    return bitmap;
}

} // namespace bulbit
