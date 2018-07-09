#ifndef VEC2_H
#define VEC2_H

#include "mathx.h"
#include <iostream>
#include <math.h>

class vec2
{
  public:
    vec2() : x(0.f), y(0.f) {}
    vec2(float x, float y) : x(x), y(y) {}
    vec2(const vec2& v) : x(v.x), y(v.y) {}

    inline const vec2& operator+() const { return *this; }
    inline const vec2 operator-() const { return vec2(-x, -y); }

    inline vec2 operator+(const vec2& v) const { return vec2(x + v.x, y + v.y); }
    inline vec2 operator-(const vec2& v) const { return vec2(x - v.x, y - v.y); }
    inline vec2 operator*(const vec2& v) const { return vec2(x * v.x, y * v.y); }
    inline vec2 operator/(const vec2& v) const { return vec2(x / v.x, y / v.y); }
    inline vec2 operator+(const float s) const { return vec2(x + s, y + s); }
    inline vec2 operator-(const float s) const { return vec2(x - s, y - s); }
    inline vec2 operator*(const float s) const { return vec2(x * s, y * s); }
    inline vec2 operator/(const float s) const { return vec2(x / s, y / s); }

    inline vec2& operator+=(const vec2& v)
    {
        x += v.x;
        y += v.y;
        return *this;
    }
    inline vec2& operator-=(const vec2& v)
    {
        x -= v.x;
        y -= v.y;
        return *this;
    }
    inline vec2& operator*=(const vec2& v)
    {
        x *= v.x;
        y *= v.y;
        return *this;
    }
    inline vec2& operator/=(const vec2& v)
    {
        x /= v.x;
        y /= v.y;
        return *this;
    }
    inline vec2& operator*=(const float& s)
    {
        x *= s;
        y *= s;
        return *this;
    }
    inline vec2& operator/=(const float& s)
    {
        float t = 1.0f / s;
        x *= t;
        y *= t;
        return *this;
    }

    friend inline vec2 operator+(const float s, const vec2& v) { return v + s; }
    friend inline vec2 operator-(const float s, const vec2& v) { return v - s; }
    friend inline vec2 operator*(const float s, const vec2& v) { return v * s; }

    friend inline std::istream& operator>>(std::istream& is, vec2& v) { return is >> v.x >> v.y; }
    friend inline std::ostream& operator>>(std::ostream& os, vec2& v)
    {
        return os << "(" << v.x << ", " << v.y << ")";
    }

    inline float length() const { return std::sqrt(x * x + y * y); }
    inline float squaredLength() const { return x * x + y * y; }
    inline vec2 normalized() const
    {
        float k = 1.0f / length();
        return vec2(x * k, y * k);
    }
    inline void normalize()
    {
        float k = 1.0f / length();
        x *= k;
        y *= k;
    }

    static float angle(const vec2& from, const vec2& to)
    {
        return std::acoshf(dot(from, to) / (from.length() * to.length())) * mathx::rad2deg;
    }
    static float cross(const vec2& v1, const vec2& v2) { return v1.x * v2.y - v1.y * v2.x; }
    static float distance(const vec2& v1, const vec2& v2) { return (v1 - v2).length(); }
    static float dot(const vec2& v1, const vec2& v2) { return v1.x * v2.x + v1.y * v2.y; }

    float x, y;
};

#endif