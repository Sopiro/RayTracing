#pragma once

#include "bitmap.h"
#include "postprocess.h"
#include "spectrum.h"

namespace bulbit
{

class Film
{
public:
    Film(const Camera* camera);

    void Set(int32 x, int32 y, const Spectrum& color);
    Bitmap ConvertToBitmap() const;

    const int32 width, height;

private:
    std::unique_ptr<Spectrum[]> data;
};

inline Film::Film(const Camera* camera)
    : width{ camera->GetScreenWidth() }
    , height{ camera->GetScreenHeight() }
{
    data = std::make_unique<Spectrum[]>(width * height);
}

inline void Film::Set(int32 x, int32 y, const Spectrum& color)
{
    data[x + y * width] = color;
}

inline Bitmap Film::ConvertToBitmap() const
{
    Bitmap bitmap(width, height);

    for (int32 y = 0; y < height; ++y)
    {
        for (int32 x = 0; x < width; ++x)
        {
            Spectrum color = data[x + y * width];
            color = Tonemap_ACES(color);
            color = ToSRGB(color);
            bitmap.Set(x, y, color);
        }
    }

    return bitmap;
}

} // namespace bulbit
