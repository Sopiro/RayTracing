#pragma once

#include "vectors.h"

#include <format>

namespace bulbit
{

using Spectrum = struct RGBSpectrum;

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

    Float MinComponent() const
    {
        return std::min({ r, g, b });
    }

    Float MaxComponent() const
    {
        return std::max({ r, g, b });
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

inline RGBSpectrum operator-(const RGBSpectrum& sp)
{
    return RGBSpectrum(-sp.r, -sp.g, -sp.b);
}

inline RGBSpectrum operator+(const RGBSpectrum& sp1, const RGBSpectrum& sp2)
{
    return RGBSpectrum(sp1.r + sp2.r, sp1.g + sp2.g, sp1.b + sp2.b);
}

inline RGBSpectrum operator-(const RGBSpectrum& sp1, const RGBSpectrum& sp2)
{
    return RGBSpectrum(sp1.r - sp2.r, sp1.g - sp2.g, sp1.b - sp2.b);
}

template <typename T>
inline RGBSpectrum operator*(const RGBSpectrum& sp, T s)
{
    return RGBSpectrum(sp.r * Float(s), sp.g * Float(s), sp.b * Float(s));
}

template <typename T>
inline RGBSpectrum operator*(T s, const RGBSpectrum& sp)
{
    return RGBSpectrum(sp.r * s, sp.g * s, sp.b * s);
}

inline RGBSpectrum operator*(const RGBSpectrum& sp1, const RGBSpectrum& sp2)
{
    return RGBSpectrum(sp1.r * sp2.r, sp1.g * sp2.g, sp1.b * sp2.b);
}

template <typename T>
inline RGBSpectrum operator/(const RGBSpectrum& sp, T s)
{
    return RGBSpectrum(sp.r / s, sp.g / s, sp.b / s);
}

template <typename T>
inline RGBSpectrum operator/(T s, const RGBSpectrum& sp)
{
    return RGBSpectrum(s / sp.r, s / sp.g, s / sp.b);
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

template <typename T>
inline RGBSpectrum operator+=(RGBSpectrum& sp1, T s)
{
    sp1.r += s;
    sp1.g += s;
    sp1.b += s;
    return sp1;
}

template <typename T>
inline RGBSpectrum operator-=(RGBSpectrum& sp1, T s)
{
    sp1.r -= s;
    sp1.g -= s;
    sp1.b -= s;
    return sp1;
}

template <typename T>
inline RGBSpectrum operator*=(RGBSpectrum& sp1, T s)
{
    sp1.r *= s;
    sp1.g *= s;
    sp1.b *= s;
    return sp1;
}

template <typename T>
inline RGBSpectrum operator/=(RGBSpectrum& sp1, T s)
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

template <typename T>
inline RGBSpectrum Lerp(const RGBSpectrum& sp1, const RGBSpectrum& sp2, T t)
{
    return (1 - t) * sp1 + t * sp2;
}

inline RGBSpectrum Sqrt(const RGBSpectrum& sp)
{
    return RGBSpectrum(std::sqrt(sp.r), std::sqrt(sp.g), std::sqrt(sp.b));
}

inline RGBSpectrum Exp(const RGBSpectrum& sp)
{
    return RGBSpectrum(std::exp(sp.r), std::exp(sp.g), std::exp(sp.b));
}

template <typename T>
inline RGBSpectrum Min(const RGBSpectrum& sp, T val)
{
    return RGBSpectrum(std::min(Float(val), sp.r), std::min(Float(val), sp.g), std::min(Float(val), sp.b));
}

template <typename T>
inline RGBSpectrum Max(const RGBSpectrum& sp, T val)
{
    return RGBSpectrum(std::max(Float(val), sp.r), std::max(Float(val), sp.g), std::max(Float(val), sp.b));
}

template <typename U, typename V>
inline RGBSpectrum Clamp(const RGBSpectrum& sp, U left, V right)
{
    return RGBSpectrum(Clamp(sp.r, left, right), Clamp(sp.g, left, right), Clamp(sp.b, left, right));
}

} // namespace bulbit
