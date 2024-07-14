#pragma once

#include "matrix.h"
#include "vectors.h"

namespace bulbit
{

struct Quat
{
    Quat() = default;

    Quat(Identity)
        : Quat(1)
    {
    }

    Quat(Float x, Float y, Float z, Float w)
        : x{ x }
        , y{ y }
        , z{ z }
        , w{ w }
    {
    }

    explicit Quat(Float w)
        : x{ 0 }
        , y{ 0 }
        , z{ 0 }
        , w{ w }
    {
    }

    Quat(const Mat3& m);

    Quat(const Vec3& front, const Vec3& up);

    // Axis must be normalized
    Quat(Float angle, const Vec3& unitAxis)
    {
        Float halfAngle = angle * 0.5f;

        Float s = std::sin(halfAngle);
        x = unitAxis.x * s;
        y = unitAxis.y * s;
        z = unitAxis.z * s;
        w = std::cos(halfAngle);
    }

    Quat operator-()
    {
        return Quat(-x, -y, -z, -w);
    }

    Quat operator*(Float s) const
    {
        return Quat(x * s, y * s, z * s, w * s);
    }

    bool IsIdentity() const
    {
        return x == 0 && y == 0 && z == 0 && w == 1;
    }

    // Magnitude
    Float Length() const
    {
        return std::sqrt(x * x + y * y + z * z + w * w);
    }

    Float Length2() const
    {
        return x * x + y * y + z * z + w * w;
    }

    Float Normalize()
    {
        Float length = Length();
        if (length < epsilon)
        {
            return 0;
        }

        Float invLength = 1 / length;
        x *= invLength;
        y *= invLength;
        z *= invLength;
        w *= invLength;

        return length;
    }

    Quat GetConjugate() const
    {
        return Quat(-x, -y, -z, w);
    }

    Vec3 GetImaginaryPart() const
    {
        return Vec3(x, y, z);
    }

    // Optimized qvq'
    Vec3 Rotate(const Vec3& v) const
    {
        Float vx = 2 * v.x;
        Float vy = 2 * v.y;
        Float vz = 2 * v.z;
        Float w2 = w * w - 0.5f;

        Float dot2 = (x * vx + y * vy + z * vz);

        return Vec3(
            (vx * w2 + (y * vz - z * vy) * w + x * dot2), (vy * w2 + (z * vx - x * vz) * w + y * dot2),
            (vz * w2 + (x * vy - y * vx) * w + z * dot2)
        );
    }

    Vec3 RotateInv(const Vec3& v) const
    {
        Float vx = 2 * v.x;
        Float vy = 2 * v.y;
        Float vz = 2 * v.z;
        Float w2 = w * w - 0.5f;

        Float dot2 = (x * vx + y * vy + z * vz);

        return Vec3(
            (vx * w2 - (y * vz - z * vy) * w + x * dot2), (vy * w2 - (z * vx - x * vz) * w + y * dot2),
            (vz * w2 - (x * vy - y * vx) * w + z * dot2)
        );
    }

    void SetIdentity()
    {
        x = 0;
        y = 0;
        z = 0;
        w = 1;
    }

    // Computes rotation of x-axis
    Vec3 GetBasisX() const
    {
        Float x2 = x * 2;
        Float w2 = w * 2;

        return Vec3((w * w2) - 1 + x * x2, (z * w2) + y * x2, (-y * w2) + z * x2);
    }

    // Computes rotation of y-axis
    Vec3 GetBasisY() const
    {
        Float y2 = y * 2;
        Float w2 = w * 2;

        return Vec3((-z * w2) + x * y2, (w * w2) - 1 + y * y2, (x * w2) + z * y2);
    }

    // Computes rotation of z-axis
    Vec3 GetBasisZ() const
    {
        Float z2 = z * 2;
        Float w2 = w * 2;

        return Vec3((y * w2) + x * z2, (-x * w2) + y * z2, (w * w2) - 1 + z * z2);
    }

    std::string ToString() const
    {
        return std::format("{:.4f}\t{:.4f}\t{:.4f}\t{:.4f}", x, y, z, w);
    }

    static inline Quat FromEuler(Float x, Float y, Float z)
    {
        Float cr = std::cos(x * 0.5f);
        Float sr = std::sin(x * 0.5f);
        Float cp = std::cos(y * 0.5f);
        Float sp = std::sin(y * 0.5f);
        Float cy = std::cos(z * 0.5f);
        Float sy = std::sin(z * 0.5f);

        Quat q;
        q.w = cr * cp * cy + sr * sp * sy;
        q.x = sr * cp * cy - cr * sp * sy;
        q.y = cr * sp * cy + sr * cp * sy;
        q.z = cr * cp * sy - sr * sp * cy;

        return q;
    }

    Float x, y, z, w;
};

// Quat inline functions begin

inline bool operator==(const Quat& a, const Quat& b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

inline Float Dot(const Quat& a, const Quat& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

// Quaternion multiplication
inline Quat operator*(const Quat& a, const Quat& b)
{
    // clang-format off
    return Quat(a.w * b.x + b.w * a.x + a.y * b.z - b.y * a.z,
                a.w * b.y + b.w * a.y + a.z * b.x - b.z * a.x,
                a.w * b.z + b.w * a.z + a.x * b.y - b.x * a.y,
                a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z);
    // clang-format on
}

inline Quat operator+(const Quat& a, const Quat& b)
{
    return Quat(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

inline Quat operator-(const Quat& a, const Quat& b)
{
    return Quat(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

// Compute angle between two quaternions
inline Float Angle(const Quat& a, const Quat& b)
{
    return std::acos(Dot(a, b)) * 2;
}

// Quat inline functions end

} // namespace bulbit
