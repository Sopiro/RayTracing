#pragma once

#include "common.h"
#include "pool.h"
#include "spectrum.h"

namespace bulbit
{

template <typename T>
class Texture
{
public:
    virtual ~Texture() = default;

    virtual T Evaluate(const Point2& uv) const = 0;
};

class FloatTexture : public Texture<Float>
{
public:
    virtual ~FloatTexture() = default;
};

class SpectrumTexture : public Texture<Spectrum>
{
public:
    virtual ~SpectrumTexture() = default;

    virtual Float EvaluateAlpha(const Point2& uv) const
    {
        return 1;
    };
};

class ConstantColorTexture : public SpectrumTexture
{
public:
    static ConstantColorTexture* Create(const Spectrum& rgb)
    {
        return pool.Create(rgb);
    }

    static ConstantColorTexture* Create(Float rgb)
    {
        return Create(Spectrum(rgb));
    }

    static ConstantColorTexture* Create(Float red, Float green, Float blue)
    {
        return Create(Spectrum(red, green, blue));
    }

    ConstantColorTexture(const Spectrum& rgb)
        : color{ rgb }
    {
    }

    virtual Spectrum Evaluate(const Point2& uv) const override
    {
        return color;
    }

private:
    Spectrum color;

    struct ColorHash
    {
        size_t operator()(const Spectrum& rgb) const
        {
            return std::hash<Float>()(rgb.r) ^ std::hash<Float>()(rgb.g) ^ std::hash<Float>()(rgb.b);
        }
    };

    static inline Pool<Spectrum, ConstantColorTexture, ColorHash> pool;
};

class ConstantFloatTexture : public FloatTexture
{
public:
    static ConstantFloatTexture* Create(Float value)
    {
        return pool.Create(value);
    }

    ConstantFloatTexture(Float value)
        : value{ value }
    {
    }

    virtual Float Evaluate(const Point2& uv) const override
    {
        return value;
    }

private:
    Float value;

    static inline Pool<Float, ConstantFloatTexture> pool;
};

enum TexCoordFilter
{
    repeat,
    clamp,
};

inline void FilterTexCoord(int32* u, int32* v, int32 width, int32 height, TexCoordFilter texcoord_filter)
{
    switch (texcoord_filter)
    {
    case TexCoordFilter::repeat:
    {
        *u = *u >= 0 ? *u % width : width - (-(*u) % width) - 1;
        *v = *v >= 0 ? *v % height : height - (-(*v) % height) - 1;
    }
    break;
    case TexCoordFilter::clamp:
    {
        *u = Clamp(*u, 0, width - 1);
        *v = Clamp(*v, 0, height - 1);
    }
    break;
    default:
        assert(false);
        break;
    }
}

class FloatImageTexture : public FloatTexture
{
public:
    static FloatImageTexture* Create(const std::string& filename, int32 channel, bool srgb = false)
    {
        return pool.Create({ filename, channel }, srgb);
    }

    FloatImageTexture();
    FloatImageTexture(const std::string& filename, int32 channel, bool srgb);
    FloatImageTexture(const std::pair<std::string, int32>& filename_channel, bool srgb);

    virtual Float Evaluate(const Point2& uv) const override;

    int32 GetWidth() const
    {
        return width;
    }

    int32 GetHeight() const
    {
        return height;
    }

private:
    std::unique_ptr<Float[]> floats;

    int32 channel;
    int32 width, height;
    TexCoordFilter texcoord_filter;

    struct StringIntHash
    {
        size_t operator()(const std::pair<std::string, int32>& string_float) const
        {
            return std::hash<std::string>()(string_float.first) ^ std::hash<int32>()(string_float.second);
        }
    };

    static inline Pool<std::pair<std::string, int32>, FloatImageTexture, StringIntHash> pool;
};

class ColorImageTexture : public SpectrumTexture
{
public:
    static ColorImageTexture* Create(const std::string& filename, bool srgb = false)
    {
        return pool.Create(filename, srgb);
    }

    ColorImageTexture();
    ColorImageTexture(const std::string& filename, bool srgb);

    virtual Spectrum Evaluate(const Point2& uv) const override;
    virtual Float EvaluateAlpha(const Point2& uv) const override;

    int32 GetWidth() const
    {
        return width;
    }

    int32 GetHeight() const
    {
        return height;
    }

private:
    std::unique_ptr<RGBSpectrum[]> rgb;
    std::unique_ptr<Float[]> alpha;

    int32 width, height;
    TexCoordFilter texcoord_filter;

    static inline Pool<std::string, ColorImageTexture> pool;
};

} // namespace bulbit