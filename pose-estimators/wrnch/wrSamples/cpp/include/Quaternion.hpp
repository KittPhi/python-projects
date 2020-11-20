// Simple Quaternion class used for visualization
// Adapted from : http://www.ncsa.illinois.edu/People/kindr/emtc/quaternions/quaternion.c++
#pragma once

#include <stdexcept>
namespace wrSamples
{
class Quaternion
{
    float const m_x = 0;
    float const m_y = 0;
    float const m_z = 0;
    float const m_w = 0;

  public:
    Quaternion() = default;
    Quaternion(float x, float y, float z, float w);
    Quaternion operator*(const Quaternion&) const;
    Quaternion inverse() const;
    void rotate(float (&v)[3]) const;
};

inline Quaternion::Quaternion(float x, float y, float z, float w)
: m_x(x)
, m_y(y)
, m_z(z)
, m_w(w)
{
}

inline Quaternion Quaternion::operator*(const Quaternion& q) const
{
    return Quaternion(m_w * q.m_x + m_x * q.m_w + m_y * q.m_z - m_z * q.m_y,
                      m_w * q.m_y + m_y * q.m_w + m_z * q.m_x - m_x * q.m_z,
                      m_w * q.m_z + m_z * q.m_w + m_x * q.m_y - m_y * q.m_x,
                      m_w * q.m_w - m_x * q.m_x - m_y * q.m_y - m_z * q.m_z);
}

inline Quaternion Quaternion::inverse() const
{
    float norm = m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w;
    if (norm == 0)
    {
        throw std::runtime_error("Divide by zero exception");
    }
    float xInv = -m_x / norm;
    float yInv = -m_y / norm;
    float zInv = -m_z / norm;

    return Quaternion(xInv, yInv, zInv, m_w);
}

inline void Quaternion::rotate(float (&v)[3]) const
{
    Quaternion qv(v[0], v[1], v[2], 0);
    Quaternion r = (*this) * qv * (*this).inverse();
    v[0] = r.m_x;
    v[1] = r.m_y;
    v[2] = r.m_z;
}
}  // namespace wrSamples
