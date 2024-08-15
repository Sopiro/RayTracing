#pragma once

#include "hash.h"
#include "pool.h"
#include "texture.h"

namespace bulbit
{

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
        BulbitNotUsed(uv);
        return color;
    }

private:
    Spectrum color;

    struct ColorHash
    {
        size_t operator()(const Spectrum& rgb) const
        {
            return Hash(rgb);
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
        BulbitNotUsed(uv);
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
        BulbitAssert(false);
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

    int32 width, height;
    TexCoordFilter texcoord_filter;

    struct StringIntHash
    {
        size_t operator()(const std::pair<std::string, int32>& string_float) const
        {
            return Hash(string_float.first, string_float.second);
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

class CheckerTexture : public SpectrumTexture
{
public:
    static CheckerTexture* Create(const SpectrumTexture* a, const SpectrumTexture* b, const Point2& resolution)
    {
        return pool.Create({ a, b }, resolution);
    }

    CheckerTexture(const SpectrumTexture* a, const SpectrumTexture* b, const Point2& resolution);
    CheckerTexture(std::pair<const SpectrumTexture*, const SpectrumTexture*> checker, const Point2& resolution);

    virtual Spectrum Evaluate(const Point2& uv) const override;
    virtual Float EvaluateAlpha(const Point2& uv) const override;

private:
    const SpectrumTexture* a;
    const SpectrumTexture* b;
    Point2 resolution;

    struct CheckerHash
    {
        size_t operator()(const std::pair<const SpectrumTexture*, const SpectrumTexture*>& checker) const
        {
            return Hash(checker.first, checker.second);
        }
    };

    static inline Pool<std::pair<const SpectrumTexture*, const SpectrumTexture*>, CheckerTexture, CheckerHash> pool;
};

} // namespace bulbit
