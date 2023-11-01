#pragma once

#include "assert.h"
#include "math.h"

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
        assert(!IsNullish());
    }

    constexpr RGBSpectrum(Float red, Float green, Float blue)
        : r{ red }
        , g{ green }
        , b{ blue }
    {
        assert(!IsNullish());
    }

    constexpr RGBSpectrum(const Vec3& rgb)
        : r{ rgb.x }
        , g{ rgb.y }
        , b{ rgb.z }
    {
        assert(!IsNullish());
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
        return r == Float(0.0) && g == Float(0.0) && b == Float(0.0);
    }

    Float Luminance() const
    {
        constexpr Vec3 coefficient(Float(0.2126), Float(0.7152), Float(0.0722));
        return r * coefficient.x + g * coefficient.y + b * coefficient.z;
    }

    bool IsNullish() const
    {
        return std::isnan(r) || std::isinf(r) || std::isnan(g) || std::isinf(g) || std::isnan(b) || std::isinf(b);
    }

    static const int32 num_spectral_samples;
    static const RGBSpectrum black;
};

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
