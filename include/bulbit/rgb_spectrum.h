#pragma once

#include "assert.h"
#include "math.h"

#include <format>

namespace bulbit
{

struct RGBSpectrum
{
    Float r, g, b;

    explicit constexpr RGBSpectrum(Float s = 0)
        : r{ s }
        , g{ s }
        , b{ s }
    {
    }

    constexpr RGBSpectrum(Float red, Float green, Float blue)
        : r{ red }
        , g{ green }
        , b{ blue }
    {
    }

    constexpr RGBSpectrum(const Vec3& rgb)
        : r{ rgb.x }
        , g{ rgb.y }
        , b{ rgb.z }
    {
    }

    Float operator[](int32 i) const
    {
        return (&r)[i];
    }

    Float& operator[](int32 i)
    {
        return (&r)[i];
    }

    bool IsBlack() const
    {
        return r == 0.0f && g == 0.0f && b == 0.0f;
    }

    Float Luminance() const
    {
        constexpr RGBSpectrum coefficient(0.2126f, 0.7152f, 0.0722f);
        return r * coefficient.r + g * coefficient.g + b * coefficient.b;
    }

    Float Average() const
    {
#if 0
        Float sp = 0;
        for (int32 i = 0; i < num_spectral_samples; ++i)
        {
            sp += (*this)[i];
        }
        return sp / num_spectral_samples;
#else
        return (r + g + b) / 3;
#endif
    }

    bool IsNullish() const
    {
        return std::isnan(r) || std::isinf(r) || std::isnan(g) || std::isinf(g) || std::isnan(b) || std::isinf(b);
    }

    std::string ToString() const
    {
        return std::format("{:.4f}\t{:.4f}\t{:.4f}", r, g, b);
    }

    static const int32 num_spectral_samples;
    static const RGBSpectrum black;
};

constexpr inline int32 RGBSpectrum::num_spectral_samples(3);
constexpr inline RGBSpectrum RGBSpectrum::black(0);

inline RGBSpectrum operator+(const RGBSpectrum& sp1, const RGBSpectrum& sp2)
{
    return RGBSpectrum(sp1.r + sp2.r, sp1.g + sp2.g, sp1.b + sp2.b);
}

inline RGBSpectrum operator-(const RGBSpectrum& sp1, const RGBSpectrum& sp2)
{
    return RGBSpectrum(sp1.r - sp2.r, sp1.g - sp2.g, sp1.b - sp2.b);
}

inline RGBSpectrum operator*(const RGBSpectrum& sp, Float s)
{
    return RGBSpectrum(sp.r * s, sp.g * s, sp.b * s);
}

inline RGBSpectrum operator*(Float s, const RGBSpectrum& sp)
{
    return operator*(sp, s);
}

inline RGBSpectrum operator*(const RGBSpectrum& sp1, const RGBSpectrum& sp2)
{
    return RGBSpectrum(sp1.r * sp2.r, sp1.g * sp2.g, sp1.b * sp2.b);
}

inline RGBSpectrum operator/(const RGBSpectrum& sp, Float s)
{
    return sp * (1 / s);
}

inline RGBSpectrum operator/(Float s, const RGBSpectrum& sp)
{
    return (1 / s) * sp;
}

inline RGBSpectrum operator/(const RGBSpectrum& sp1, const RGBSpectrum& sp2)
{
    return RGBSpectrum(sp1.r / sp2.r, sp1.g / sp2.g, sp1.b / sp2.b);
}

inline bool operator==(const RGBSpectrum& sp1, const RGBSpectrum& sp2)
{
    return sp1.r == sp2.r && sp1.g == sp2.g && sp1.b == sp2.b;
}

inline bool operator!=(const RGBSpectrum& sp1, const RGBSpectrum& sp2)
{
    return sp1.r != sp2.r || sp1.g != sp2.g || sp1.b != sp2.b;
}

inline RGBSpectrum operator+=(RGBSpectrum& sp1, Float s)
{
    sp1.r += s;
    sp1.g += s;
    sp1.b += s;
    return sp1;
}

inline RGBSpectrum operator-=(RGBSpectrum& sp1, Float s)
{
    sp1.r -= s;
    sp1.g -= s;
    sp1.b -= s;
    return sp1;
}

inline RGBSpectrum operator*=(RGBSpectrum& sp1, Float s)
{
    sp1.r *= s;
    sp1.g *= s;
    sp1.b *= s;
    return sp1;
}

inline RGBSpectrum operator/=(RGBSpectrum& sp1, Float s)
{
    sp1.r /= s;
    sp1.g /= s;
    sp1.b /= s;
    return sp1;
}

inline RGBSpectrum operator+=(RGBSpectrum& sp1, const RGBSpectrum& sp2)
{
    sp1.r += sp2.r;
    sp1.g += sp2.g;
    sp1.b += sp2.b;
    return sp1;
}

inline RGBSpectrum operator-=(RGBSpectrum& sp1, const RGBSpectrum& sp2)
{
    sp1.r -= sp2.r;
    sp1.g -= sp2.g;
    sp1.b -= sp2.b;
    return sp1;
}

inline RGBSpectrum operator*=(RGBSpectrum& sp1, const RGBSpectrum& sp2)
{
    sp1.r *= sp2.r;
    sp1.g *= sp2.g;
    sp1.b *= sp2.b;
    return sp1;
}

inline RGBSpectrum operator/=(RGBSpectrum& sp1, const RGBSpectrum& sp2)
{
    sp1.r /= sp2.r;
    sp1.g /= sp2.g;
    sp1.b /= sp2.b;
    return sp1;
}

inline RGBSpectrum Lerp(const RGBSpectrum& sp1, const RGBSpectrum& sp2, Float t)
{
    return (1 - t) * sp1 + t * sp2;
}

inline Vec3 ToVector(const RGBSpectrum& sp)
{
    return Vec3(sp.r, sp.g, sp.b);
}

} // namespace bulbit
