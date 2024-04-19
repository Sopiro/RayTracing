#pragma once

#include "spectrum.h"
#include "texture.h"

namespace bulbit
{
struct ColorHash
{
    size_t operator()(const Spectrum& v) const
    {
        return std::hash<Float>()(v.r) ^ std::hash<Float>()(v.g) ^ std::hash<Float>()(v.b);
    }
};

class ConstantColor : public Texture
{
public:
    ConstantColor(const Spectrum& rgb);

    virtual Spectrum Evaluate(const Point2& uv) const override;

    inline static int32 color_count = 0;
    inline static std::unordered_map<Spectrum, std::unique_ptr<ConstantColor>, ColorHash> loaded_colors;

    static ConstantColor* Create(const Spectrum& rgb);
    static ConstantColor* Create(Float rgb);
    static ConstantColor* Create(Float red, Float green, Float blue);

protected:
    Spectrum color;
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
    auto loaded = loaded_colors.find(rgb);
    if (loaded != loaded_colors.end())
    {
        return loaded->second.get();
    }

    std::unique_ptr<ConstantColor> color = std::make_unique<ConstantColor>(rgb);
    ConstantColor* ptr = color.get();

    loaded_colors.emplace(rgb, std::move(color));
    ++color_count;

    return ptr;
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
