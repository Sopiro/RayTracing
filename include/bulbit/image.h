#pragma once

#include "math.h"
#include "spectrum.h"

#include <filesystem>

namespace bulbit
{

template <typename T>
struct Image
{
    Image()
        : width{ 0 }
        , height{ 0 }
        , data{ nullptr }
    {
    }

    Image(int32 width, int32 height)
        : width{ width }
        , height{ height }
    {
        data = std::make_unique<T[]>(width * height);
    }

    operator bool() const
    {
        return bool(data);
    }

    T& operator[](int32 i)
    {
        return data[i];
    }

    const T& operator[](int32 i) const
    {
        return data[i];
    }

    T& operator()(int32 x)
    {
        return data[x];
    }

    const T& operator()(int32 x) const
    {
        return data[x];
    }

    T& operator()(int32 x, int32 y)
    {
        return data[y * width + x];
    }

    const T& operator()(int32 x, int32 y) const
    {
        return data[y * width + x];
    }

    const int32 width, height;

private:
    std::unique_ptr<T[]> data;
};

using Image1f = Image<Float>;
using Image3f = Image<RGBSpectrum>;
using Image4f = Image<Vec4f>;
using Image1 = Image1f;
using Image3 = Image3f;
using Image4 = Image4f;

Image1 ReadImage1(const std::filesystem::path& filename, int32 channel, bool non_color = false);
Image3 ReadImage3(const std::filesystem::path& filename, bool non_color = false);
Image4 ReadImage4(const std::filesystem::path& filename, bool non_color = false);

void WriteImage(const Image3& image, const std::filesystem::path& filename);

} // namespace bulbit
