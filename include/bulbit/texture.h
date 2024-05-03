#pragma once

#include "common.h"
#include "pool.h"
#include "spectrum.h"

namespace bulbit
{

class Texture
{
public:
    virtual ~Texture() = default;

    virtual Spectrum Evaluate(const Point2& uv) const = 0;

    virtual Float EvaluateAlpha(const Point2& uv) const
    {
        return 1;
    }
};

class ConstantColor : public Texture
{
public:
    static ConstantColor* Create(const Spectrum& rgb)
    {
        return pool.Create(rgb);
    }

    static ConstantColor* Create(Float rgb)
    {
        return Create(Spectrum(rgb));
    }

    static ConstantColor* Create(Float red, Float green, Float blue)
    {
        return Create(Spectrum(red, green, blue));
    }

    ConstantColor(const Spectrum& rgb)
        : color{ rgb }
    {
    }

    virtual Spectrum Evaluate(const Point2& uv) const override
    {
        return color;
    }

protected:
    Spectrum color;

private:
    struct ColorHash
    {
        size_t operator()(const Spectrum& rgb) const
        {
            return std::hash<Float>()(rgb.r) ^ std::hash<Float>()(rgb.g) ^ std::hash<Float>()(rgb.b);
        }
    };

    static inline Pool<Spectrum, ConstantColor, ColorHash> pool;
};

enum TexCoordFilter
{
    repeat,
    clamp,
};

class ImageTexture : public Texture
{
public:
    static ImageTexture* Create(const std::string& filename, bool srgb = false)
    {
        return pool.Create(filename, srgb);
    }

    ImageTexture();
    ImageTexture(const std::string& filename, bool srgb);

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

protected:
    void FilterTexCoord(int32* u, int32* v) const;

    std::unique_ptr<RGBSpectrum[]> rgb;
    std::unique_ptr<Float[]> alpha;

    int32 width, height;
    TexCoordFilter texcoord_filter;

private:
    static inline Pool<std::string, ImageTexture> pool;
};

} // namespace bulbit