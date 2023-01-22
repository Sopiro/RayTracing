#pragma once

#include "common.h"

#define COLOR_CHANNELS (3)

class Bitmap
{
public:
    Bitmap(int32 width, int32 height)
        : width{ width }
        , height{ height }
    {
        pixels = new uint8[width * height * COLOR_CHANNELS];
    }

    void Set(int32 x, int32 y, const Color& color)
    {
        pixels[(x + (height - y - 1) * width) * COLOR_CHANNELS + 0] = static_cast<int32>(Clamp(color.x, 0.0, 0.999) * 256.0);
        pixels[(x + (height - y - 1) * width) * COLOR_CHANNELS + 1] = static_cast<int32>(Clamp(color.y, 0.0, 0.999) * 256.0);
        pixels[(x + (height - y - 1) * width) * COLOR_CHANNELS + 2] = static_cast<int32>(Clamp(color.z, 0.0, 0.999) * 256.0);
    }

    void WriteToFile(char const* filename) const
    {
        // stbi_write_png(filename, width, height, COLOR_CHANNELS, pixels, width * COLOR_CHANNELS);
        stbi_write_jpg(filename, width, height, COLOR_CHANNELS, pixels, 100);
    }

    ~Bitmap()
    {
        delete[] pixels;
    }

private:
    int32 width;
    int32 height;

    uint8* pixels;
};