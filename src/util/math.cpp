#include "bulbit/matrix.h"
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

Mat4::Mat4(const Transform& t)
    : Mat4(Mat3(t.q), t.p)
{
    ex *= t.s.x;
    ey *= t.s.y;
    ez *= t.s.z;
}

} // namespace bulbit