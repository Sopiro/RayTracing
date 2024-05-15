#include "bulbit/math.h"
#include "bulbit/transform.h"

namespace bulbit
{

Mat3::Mat3(const Quat& q)
{
    Float xx = q.x * q.x;
    Float yy = q.y * q.y;
    Float zz = q.z * q.z;
    Float xz = q.x * q.z;
    Float xy = q.x * q.y;
    Float yz = q.y * q.z;
    Float wx = q.w * q.x;
    Float wy = q.w * q.y;
    Float wz = q.w * q.z;

    ex.x = 1 - 2 * (yy + zz);
    ex.y = 2 * (xy + wz);
    ex.z = 2 * (xz - wy);

    ey.x = 2 * (xy - wz);
    ey.y = 1 - 2 * (xx + zz);
    ey.z = 2 * (yz + wx);

    ez.x = 2 * (xz + wy);
    ez.y = 2 * (yz - wx);
    ez.z = 1 - 2 * (xx + yy);
}

Mat3 Mat3::GetInverse() const
{
    Mat3 t;

    Float det = ex.x * (ey.y * ez.z - ey.z * ez.y) - ey.x * (ex.y * ez.z - ez.y * ex.z) + ez.x * (ex.y * ey.z - ey.y * ex.z);

    if (det != 0)
    {
        det = 1 / det;
    }

    t.ex.x = (ey.y * ez.z - ey.z * ez.y) * det;
    t.ey.x = (ez.x * ey.z - ey.x * ez.z) * det;
    t.ez.x = (ey.x * ez.y - ez.x * ey.y) * det;
    t.ex.y = (ez.y * ex.z - ex.y * ez.z) * det;
    t.ey.y = (ex.x * ez.z - ez.x * ex.z) * det;
    t.ez.y = (ex.y * ez.x - ex.x * ez.y) * det;
    t.ex.z = (ex.y * ey.z - ex.z * ey.y) * det;
    t.ey.z = (ex.z * ey.x - ex.x * ey.z) * det;
    t.ez.z = (ex.x * ey.y - ex.y * ey.x) * det;

    return t;
}

Mat3 Mat3::Scale(Float x, Float y)
{
    Mat3 t{ identity };

    t.ex.x = x;
    t.ey.y = y;

    return Mul(*this, t);
}

Mat3 Mat3::Rotate(Float z)
{
    Float s = std::sin(z);
    Float c = std::cos(z);

    Mat3 t;

    // clang-format off
    t.ex.x = c; t.ey.x = -s; t.ez.x = 0;
    t.ex.y = s; t.ey.y = c;  t.ez.y = 0;
    t.ex.z = 0; t.ey.z = 0;  t.ez.z = 1;
    // clang-format on

    return Mul(*this, t);
}

Mat3 Mat3::Translate(Float x, Float y)
{
    Mat3 t{ identity };

    t.ez.x = x;
    t.ez.y = y;

    return Mul(*this, t);
}

Mat3 Mat3::Translate(const Vec2& v)
{
    Mat3 t{ identity };

    t.ez.x = v.x;
    t.ez.y = v.y;

    return Mul(*this, t);
}

Mat4::Mat4(const Transform& t)
    : Mat4(Mat3(t.q), t.p)
{
    ex *= t.r.x;
    ey *= t.r.y;
    ez *= t.r.z;
}

Mat4 Mat4::Scale(Float x, Float y, Float z)
{
    Mat4 t{ identity };

    t.ex.x = x;
    t.ey.y = y;
    t.ez.z = z;

    return Mul(*this, t);
}

Mat4 Mat4::Rotate(Float x, Float y, Float z)
{
    Float sinX = std::sin(x);
    Float cosX = std::cos(x);
    Float sinY = std::sin(y);
    Float cosY = std::cos(y);
    Float sinZ = std::sin(z);
    Float cosZ = std::cos(z);

    Mat4 t;

    t.ex.x = cosY * cosZ;
    t.ex.y = sinX * sinY * cosZ + cosX * sinZ;
    t.ex.z = -cosX * sinY * cosZ + sinX * sinZ;
    t.ex.w = 0;

    t.ey.x = -cosY * sinZ;
    t.ey.y = -sinX * sinY * sinZ + cosX * cosZ;
    t.ey.z = cosX * sinY * sinZ + sinX * cosZ;
    t.ey.w = 0;

    t.ez.x = sinY;
    t.ez.y = -sinX * cosY;
    t.ez.z = cosX * cosY;
    t.ez.w = 0;

    t.ew.x = 0;
    t.ew.y = 0;
    t.ew.z = 0;
    t.ew.w = 1;

    return Mul(*this, t);
}

Mat4 Mat4::Translate(Float x, Float y, Float z)
{
    Mat4 t{ identity };

    t.ew.x = x;
    t.ew.y = y;
    t.ew.z = z;

    return Mul(*this, t);
}

Mat4 Mat4::Translate(const Vec3& v)
{
    Mat4 t{ identity };

    t.ew.x = v.x;
    t.ew.y = v.y;
    t.ew.z = v.z;

    return Mul(*this, t);
}

Mat4 Mat4::GetInverse()
{
    Float a2323 = ez.z * ew.w - ez.w * ew.z;
    Float a1323 = ez.y * ew.w - ez.w * ew.y;
    Float a1223 = ez.y * ew.z - ez.z * ew.y;
    Float a0323 = ez.x * ew.w - ez.w * ew.x;
    Float a0223 = ez.x * ew.z - ez.z * ew.x;
    Float a0123 = ez.x * ew.y - ez.y * ew.x;
    Float a2313 = ey.z * ew.w - ey.w * ew.z;
    Float a1313 = ey.y * ew.w - ey.w * ew.y;
    Float a1213 = ey.y * ew.z - ey.z * ew.y;
    Float a2312 = ey.z * ez.w - ey.w * ez.z;
    Float a1312 = ey.y * ez.w - ey.w * ez.y;
    Float a1212 = ey.y * ez.z - ey.z * ez.y;
    Float a0313 = ey.x * ew.w - ey.w * ew.x;
    Float a0213 = ey.x * ew.z - ey.z * ew.x;
    Float a0312 = ey.x * ez.w - ey.w * ez.x;
    Float a0212 = ey.x * ez.z - ey.z * ez.x;
    Float a0113 = ey.x * ew.y - ey.y * ew.x;
    Float a0112 = ey.x * ez.y - ey.y * ez.x;

    Float det = ex.x * (ey.y * a2323 - ey.z * a1323 + ey.w * a1223) - ex.y * (ey.x * a2323 - ey.z * a0323 + ey.w * a0223) +
                ex.z * (ey.x * a1323 - ey.y * a0323 + ey.w * a0123) - ex.w * (ey.x * a1223 - ey.y * a0223 + ey.z * a0123);

    if (det != 0)
    {
        det = 1 / det;
    }

    Mat4 t;

    t.ex.x = det * (ey.y * a2323 - ey.z * a1323 + ey.w * a1223);
    t.ex.y = det * -(ex.y * a2323 - ex.z * a1323 + ex.w * a1223);
    t.ex.z = det * (ex.y * a2313 - ex.z * a1313 + ex.w * a1213);
    t.ex.w = det * -(ex.y * a2312 - ex.z * a1312 + ex.w * a1212);
    t.ey.x = det * -(ey.x * a2323 - ey.z * a0323 + ey.w * a0223);
    t.ey.y = det * (ex.x * a2323 - ex.z * a0323 + ex.w * a0223);
    t.ey.z = det * -(ex.x * a2313 - ex.z * a0313 + ex.w * a0213);
    t.ey.w = det * (ex.x * a2312 - ex.z * a0312 + ex.w * a0212);
    t.ez.x = det * (ey.x * a1323 - ey.y * a0323 + ey.w * a0123);
    t.ez.y = det * -(ex.x * a1323 - ex.y * a0323 + ex.w * a0123);
    t.ez.z = det * (ex.x * a1313 - ex.y * a0313 + ex.w * a0113);
    t.ez.w = det * -(ex.x * a1312 - ex.y * a0312 + ex.w * a0112);
    t.ew.x = det * -(ey.x * a1223 - ey.y * a0223 + ey.z * a0123);
    t.ew.y = det * (ex.x * a1223 - ex.y * a0223 + ex.z * a0123);
    t.ew.z = det * -(ex.x * a1213 - ex.y * a0213 + ex.z * a0113);
    t.ew.w = det * (ex.x * a1212 - ex.y * a0212 + ex.z * a0112);

    return t;
}

// https://math.stackexchange.com/questions/893984/conversion-of-rotation-matrix-to-quaternion
Quat::Quat(const Mat3& m)
{
    if (m.ez.z < 0)
    {
        if (m.ex.x > m.ey.y)
        {
            Float t = 1 + m.ex.x - m.ey.y - m.ez.z;
            *this = Quat(t, m.ex.y + m.ey.x, m.ez.x + m.ex.z, m.ey.z - m.ez.y) * (0.5f / std::sqrt(t));
        }
        else
        {
            Float t = 1 - m.ex.x + m.ey.y - m.ez.z;
            *this = Quat(m.ex.y + m.ey.x, t, m.ey.z + m.ez.y, m.ez.x - m.ex.z) * (0.5f / std::sqrt(t));
        }
    }
    else
    {
        if (m.ex.x < -m.ey.y)
        {
            Float t = 1 - m.ex.x - m.ey.y + m.ez.z;
            *this = Quat(m.ez.x + m.ex.z, m.ey.z + m.ez.y, t, m.ex.y - m.ey.x) * (0.5f / std::sqrt(t));
        }
        else
        {
            Float t = 1 + m.ex.x + m.ey.y + m.ez.z;
            *this = Quat(m.ey.z - m.ez.y, m.ez.x - m.ex.z, m.ex.y - m.ey.x, t) * (0.5f / std::sqrt(t));
        }
    }
}

Quat::Quat(const Vec3& front, const Vec3& up)
{
    Mat3 rotation;

    rotation.ez = -front;
    rotation.ex = Cross(up, rotation.ez);
    rotation.ex.Normalize();
    rotation.ey = Cross(rotation.ez, rotation.ex);

    *this = Quat(rotation);
}

} // namespace bulbit