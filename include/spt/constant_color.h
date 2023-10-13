#pragma once

#include "texture.h"

namespace spt
{

class ConstantColor : public Texture
{
public:
    struct ColorHash
    {
        size_t operator()(const Spectrum& v) const
        {
            return std::hash<Float>()(v.r) ^ std::hash<Float>()(v.g) ^ std::hash<Float>()(v.b);
        }
    };

    ConstantColor() = default;
    ConstantColor(const Spectrum& color);

    inline static int32 color_count = 0;
    inline static std::unordered_map<Spectrum, Ref<ConstantColor>, ColorHash> loaded_colors;

    static Ref<ConstantColor> Create(const Spectrum& color);
    static Ref<ConstantColor> Create(Float rgb);
    static Ref<ConstantColor> Create(Float red, Float green, Float blue);

    virtual Spectrum Evaluate(const Point2& uv) const override;

protected:
    Spectrum color;
};

inline ConstantColor::ConstantColor(const Spectrum& _color)
    : color{ _color }
{
}

inline Spectrum ConstantColor::Evaluate(const Point2& uv) const
{
    return color;
}

inline Ref<ConstantColor> ConstantColor::Create(const Spectrum& color)
{
    auto loaded = loaded_colors.find(color);
    if (loaded != loaded_colors.end())
    {
        return loaded->second;
    }

    Ref<ConstantColor> ptr{ new ConstantColor(color) };
    loaded_colors.emplace(color, ptr);
    ++color_count;

    return ptr;
}

inline Ref<ConstantColor> ConstantColor::Create(Float rgb)
{
    return Create(Spectrum(rgb));
}

inline Ref<ConstantColor> ConstantColor::Create(Float r, Float g, Float b)
{
    return Create(Spectrum(r, g, b));
}

} // namespace spt
