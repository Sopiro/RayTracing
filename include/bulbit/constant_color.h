#pragma once

#include "pool.h"
#include "spectrum.h"
#include "texture.h"

namespace bulbit
{
struct ColorHash
{
    size_t operator()(const Spectrum& rgb) const
    {
        return std::hash<Float>()(rgb.r) ^ std::hash<Float>()(rgb.g) ^ std::hash<Float>()(rgb.b);
    }
};

class ConstantColor : public Texture
{
public:
    static ConstantColor* Create(const Spectrum& rgb);
    static ConstantColor* Create(Float rgb);
    static ConstantColor* Create(Float red, Float green, Float blue);

    ConstantColor(const Spectrum& rgb);

    virtual Spectrum Evaluate(const Point2& uv) const override;

protected:
    Spectrum color;

private:
    static inline Pool<Spectrum, ConstantColor, ColorHash> pool;
};

inline ConstantColor::ConstantColor(const Spectrum& rgb)
    : color{ rgb }
{
}

inline Spectrum ConstantColor::Evaluate(const Point2& uv) const
{
    return color;
}

inline ConstantColor* ConstantColor::Create(const Spectrum& rgb)
{
    return pool.Create(rgb);
}

inline ConstantColor* ConstantColor::Create(Float rgb)
{
    return Create(Spectrum(rgb));
}

inline ConstantColor* ConstantColor::Create(Float r, Float g, Float b)
{
    return Create(Spectrum(r, g, b));
}

} // namespace bulbit
