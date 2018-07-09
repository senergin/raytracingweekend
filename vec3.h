#ifndef VEC3_H
#define VEC3_H

#include "mathx.h"
#include <iostream>
#include <math.h>

class vec3
{
  public:
    vec3() : x(0.f), y(0.f), z(0.f) {}
    vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    vec3(const vec3& v) : x(v.x), y(v.y), z(v.z) {}

    inline const vec3& operator+() const { return *this; }
    inline const vec3 operator-() const { return vec3(-x, -y, -z); }

    inline vec3 operator+(const vec3& v) const { return vec3(x + v.x, y + v.y, z + v.z); }
    inline vec3 operator-(const vec3& v) const { return vec3(x - v.x, y - v.y, z - v.z); }
    inline vec3 operator*(const vec3& v) const { return vec3(x * v.x, y * v.y, z * v.z); }
    inline vec3 operator/(const vec3& v) const { return vec3(x / v.x, y / v.y, z / v.z); }
    inline vec3 operator+(const float s) const { return vec3(x + s, y + s, z + s); }
    inline vec3 operator-(const float s) const { return vec3(x - s, y - s, z - s); }
    inline vec3 operator*(const float s) const { return vec3(x * s, y * s, z * s); }
    inline vec3 operator/(const float s) const { return vec3(x / s, y / s, z / s); }

    inline vec3& operator+=(const vec3& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    inline vec3& operator-=(const vec3& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }
    inline vec3& operator*=(const vec3& v)
    {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }
    inline vec3& operator/=(const vec3& v)
    {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        return *this;
    }
    inline vec3& operator*=(const float& s)
    {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }
    inline vec3& operator/=(const float& s)
    {
        float t = 1.0f / s;
        x *= t;
        y *= t;
        z *= t;
        return *this;
    }

    friend inline vec3 operator+(const float s, const vec3& v) { return v + s; }
    friend inline vec3 operator-(const float s, const vec3& v) { return v - s; }
    friend inline vec3 operator*(const float s, const vec3& v) { return v * s; }

    friend inline std::istream& operator>>(std::istream& is, vec3& v)
    {
        return is >> v.x >> v.y >> v.z;
    }
    friend inline std::ostream& operator>>(std::ostream& os, vec3& v)
    {
        return os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    }

    inline float length() const { return std::sqrt(x * x + y * y + z * z); }
    inline float squaredLength() const { return x * x + y * y + z * z; }
    inline vec3 normalized() const
    {
        float k = 1.0f / length();
        return vec3(x * k, y * k, z * k);
    }
    inline void normalize()
    {
        float k = 1.0f / length();
        x *= k;
        y *= k;
        z *= k;
    }

    static float angle(const vec3& from, const vec3& to)
    {
        return std::acoshf(dot(from, to) / (from.length() * to.length())) * mathx::rad2deg;
    }
    static vec3 cross(const vec3& v1, const vec3& v2)
    {
        return vec3(((v1.y * v2.z) - (v1.z * v2.y)), ((v1.z * v2.x) - (v1.x * v2.z)),
                    ((v1.x * v2.y) - (v1.y * v2.x)));
    }
    static float distance(const vec3& v1, const vec3& v2) { return (v1 - v2).length(); }
    static float dot(const vec3& v1, const vec3& v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    float x, y, z;
};

#endif