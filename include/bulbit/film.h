#pragma once

#include "camera.h"
#include "image.h"
#include "spectrum.h"

namespace bulbit
{

class Film
{
public:
    Film(const Point2i& resolution);

    void AddSample(const Point2i& pixel, const Spectrum& L, Float weight);
    Image3 ConvertToImage() const;

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

inline void Film::AddSample(const Point2i& pixel, const Spectrum& L, Float w)
{
    samples[pixel.x + pixel.y * resolution.x] += L;
    weights[pixel.x + pixel.y * resolution.x] += w;
}

inline Image3 Film::ConvertToImage() const
{
    int32 width = resolution.x;
    int32 height = resolution.y;
    Image3 image(width, height);

    for (int32 i = 0; i < width * height; ++i)
    {
        Spectrum s = samples[i] / weights[i];
        image[i] = Vec3(s.r, s.g, s.b);
    }

    return image;
}

} // namespace bulbit
