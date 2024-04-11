#pragma once

#include "bitmap.h"
#include "camera.h"
#include "postprocess.h"
#include "spectrum.h"

namespace bulbit
{

class Film
{
public:
    Film(const Camera* camera);

    void AddSample(int32 x, int32 y, const Spectrum& L, Float weight);
    Bitmap ConvertToBitmap() const;

    const int32 width, height;

private:
    std::unique_ptr<Spectrum[]> samples;
    std::unique_ptr<Float[]> weights;
};

inline Film::Film(const Camera* camera)
    : width{ camera->GetScreenWidth() }
    , height{ camera->GetScreenHeight() }
{
    samples = std::make_unique<Spectrum[]>(width * height);
    weights = std::make_unique<Float[]>(width * height);

    for (int32 i = 0; i < width * height; ++i)
    {
        samples[i] = Spectrum::black;
        weights[i] = Float(0);
    }
}

inline void Film::AddSample(int32 x, int32 y, const Spectrum& L, Float w)
{
    samples[x + y * width] += L;
    weights[x + y * width] += w;
}

inline Bitmap Film::ConvertToBitmap() const
{
    Bitmap bitmap(width, height);

    for (int32 i = 0; i < width * height; ++i)
    {
        Spectrum s = samples[i] / weights[i];
        s = Tonemap_ACES(s);
        s = ToSRGB(s);

        // int32 x = i % width;
        // int32 y = i / width;
        bitmap.Set(i, s);
    }

    return bitmap;
}

} // namespace bulbit
