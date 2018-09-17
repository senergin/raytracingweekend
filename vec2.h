#ifndef VEC2_H
#define VEC2_H

#include "mathx.h"
#include <cmath>
#include <iostream>

class vec2
{
  public:
    vec2() : e{0.f, 0.f} {}
    vec2(float x, float y) : e{x, y} {}
    vec2(const vec2& v) : e{v.x(), v.y()} {}

    inline float x() const { return e[0]; }
    inline float y() const { return e[1]; }

    inline float operator[](int i) const { return e[i]; }
    inline float& operator[](int i) { return e[i]; };

    inline const vec2& operator+() const { return *this; }
    inline const vec2 operator-() const { return vec2(-e[0], -e[1]); }

    inline vec2 operator+(const vec2& v) const { return vec2(e[0] + v.x(), e[1] + v.y()); }
    inline vec2 operator-(const vec2& v) const { return vec2(e[0] - v.x(), e[1] - v.y()); }
    inline vec2 operator*(const vec2& v) const { return vec2(e[0] * v.x(), e[1] * v.y()); }
    inline vec2 operator/(const vec2& v) const { return vec2(e[0] / v.x(), e[1] / v.y()); }
    inline vec2 operator+(const float s) const { return vec2(e[0] + s, e[1] + s); }
    inline vec2 operator-(const float s) const { return vec2(e[0] - s, e[1] - s); }
    inline vec2 operator*(const float s) const { return vec2(e[0] * s, e[1] * s); }
    inline vec2 operator/(const float s) const { return vec2(e[0] / s, e[1] / s); }

    inline vec2& operator+=(const vec2& v)
    {
        e[0] += v.x;
        e[1] += v.y;
        return *this;
    }
    inline vec2& operator-=(const vec2& v)
    {
        e[0] -= v.x;
        e[1] -= v.y;
        return *this;
    }
    inline vec2& operator*=(const vec2& v)
    {
        e[0] *= v.x;
        e[1] *= v.y;
        return *this;
    }
    inline vec2& operator/=(const vec2& v)
    {
        e[0] /= v.x;
        e[1] /= v.y;
        return *this;
    }
    inline vec2& operator*=(const float& s)
    {
        e[0] *= s;
        e[1] *= s;
        return *this;
    }
    inline vec2& operator/=(const float& s)
    {
        float t = 1.0f / s;
        e[0] *= t;
        e[1] *= t;
        return *this;
    }

    friend inline vec2 operator+(const float s, const vec2& v) { return v + s; }
    friend inline vec2 operator-(const float s, const vec2& v) { return v - s; }
    friend inline vec2 operator*(const float s, const vec2& v) { return v * s; }

    friend inline std::istream& operator>>(std::istream& is, vec2& v) { return is >> v[0] >> v[1]; }
    friend inline std::ostream& operator>>(std::ostream& os, vec2& v)
    {
        return os << "(" << v.x() << ", " << v.y() << ")";
    }

    inline float length() const { return sqrtf(e[0] * e[0] + e[1] * e[1]); }
    inline float squaredLength() const { return e[0] * e[0] + e[1] * e[1]; }
    inline vec2 normalized() const
    {
        float k = 1.0f / length();
        return vec2(e[0] * k, e[1] * k);
    }
    inline void normalize()
    {
        float k = 1.0f / length();
        e[0] *= k;
        e[1] *= k;
    }

    static float angle(const vec2& from, const vec2& to)
    {
        return acoshf(dot(from, to) / (from.length() * to.length())) * mathx::rad2deg;
    }
    static float cross(const vec2& v1, const vec2& v2) { return v1.x() * v2.y - v1.y() * v2.x(); }
    static float distance(const vec2& v1, const vec2& v2) { return (v1 - v2).length(); }
    static float dot(const vec2& v1, const vec2& v2) { return v1.x() * v2.x() + v1.y() * v2.y(); }

    float e[2];
};

#endif