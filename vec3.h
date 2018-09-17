#ifndef VEC3_H
#define VEC3_H

#include "mathx.h"
#include <cmath>
#include <iostream>

class vec3
{
  public:
    vec3() : e{0.f, 0.f, 0.f} {}
    vec3(float x, float y, float z) : e{x, y, z} {}
    vec3(const vec3& v) : e{v.x(), v.y(), v.z()} {}

    inline float x() const { return e[0]; }
    inline float y() const { return e[1]; }
    inline float z() const { return e[2]; }

    inline const vec3& operator+() const { return *this; }
    inline const vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }

    inline float operator[](int i) const { return e[i]; }
    inline float& operator[](int i) { return e[i]; };

    inline vec3 operator+(const vec3& v) const
    {
        return vec3(e[0] + v.x(), e[1] + v.y(), e[2] + v.z());
    }
    inline vec3 operator-(const vec3& v) const
    {
        return vec3(e[0] - v.x(), e[1] - v.y(), e[2] - v.z());
    }
    inline vec3 operator*(const vec3& v) const
    {
        return vec3(e[0] * v.x(), e[1] * v.y(), e[2] * v.z());
    }
    inline vec3 operator/(const vec3& v) const
    {
        return vec3(e[0] / v.x(), e[1] / v.y(), e[2] / v.z());
    }
    inline vec3 operator+(const float s) const { return vec3(e[0] + s, e[1] + s, e[2] + s); }
    inline vec3 operator-(const float s) const { return vec3(e[0] - s, e[1] - s, e[2] - s); }
    inline vec3 operator*(const float s) const { return vec3(e[0] * s, e[1] * s, e[2] * s); }
    inline vec3 operator/(const float s) const { return vec3(e[0] / s, e[1] / s, e[2] / s); }

    inline vec3& operator+=(const vec3& v)
    {
        e[0] += v.x();
        e[1] += v.y();
        e[2] += v.z();
        return *this;
    }
    inline vec3& operator-=(const vec3& v)
    {
        e[0] -= v.x();
        e[1] -= v.y();
        e[2] -= v.z();
        return *this;
    }
    inline vec3& operator*=(const vec3& v)
    {
        e[0] *= v.x();
        e[1] *= v.y();
        e[2] *= v.z();
        return *this;
    }
    inline vec3& operator/=(const vec3& v)
    {
        e[0] /= v.x();
        e[1] /= v.y();
        e[2] /= v.z();
        return *this;
    }
    inline vec3& operator*=(const float& s)
    {
        e[0] *= s;
        e[1] *= s;
        e[2] *= s;
        return *this;
    }
    inline vec3& operator/=(const float& s)
    {
        float t = 1.0f / s;
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    friend inline vec3 operator+(const float s, const vec3& v) { return v + s; }
    friend inline vec3 operator-(const float s, const vec3& v) { return v - s; }
    friend inline vec3 operator*(const float s, const vec3& v) { return v * s; }

    friend inline std::istream& operator>>(std::istream& is, vec3& v)
    {
        return is >> v[0] >> v[1] >> v[2];
    }
    friend inline std::ostream& operator>>(std::ostream& os, vec3& v)
    {
        return os << "(" << v.x() << ", " << v.y() << ", " << v.z() << ")";
    }

    inline float length() const { return sqrtf(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]); }
    inline float squaredLength() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }
    inline vec3 normalized() const
    {
        float k = 1.0f / length();
        return vec3(e[0] * k, e[1] * k, e[2] * k);
    }
    inline void normalize()
    {
        float k = 1.0f / length();
        e[0] *= k;
        e[1] *= k;
        e[2] *= k;
    }

    static float angle(const vec3& from, const vec3& to)
    {
        return acoshf(dot(from, to) / (from.length() * to.length())) * mathx::rad2deg;
    }
    static vec3 cross(const vec3& v1, const vec3& v2)
    {
        return vec3(((v1.y() * v2.z()) - (v1.z() * v2.y())),
                    ((v1.z() * v2.x()) - (v1.x() * v2.z())),
                    ((v1.x() * v2.y()) - (v1.y() * v2.x())));
    }
    static float distance(const vec3& v1, const vec3& v2) { return (v1 - v2).length(); }
    static float dot(const vec3& v1, const vec3& v2)
    {
        return v1.x() * v2.x() + v1.y() * v2.y() + v1.z() * v2.z();
    }

    float e[3];
};

class ray
{
  public:
    ray() : origin(), direction() {}
    ray(const vec3& origin, const vec3& direction)
        : origin(origin), direction(direction.normalized())
    {
    }
    inline vec3 getPoint(float distance) const { return origin + direction * distance; }
    vec3 origin, direction;
};

#endif