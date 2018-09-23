#ifndef AABB_H
#define AABB_H

#include "mathx.h"
#include "vec3.h"

class aabb
{
  public:
    aabb() : _min(), _max() {}
    aabb(const vec3& min, const vec3& max) : _min(min), _max(max) {}
    aabb(const vec3& p) : _min(p), _max(p) {}

    inline vec3 min() const { return _min; }
    inline vec3 max() const { return _max; }
    inline vec3 extent() const { return _max - _min; }

    bool hit(const ray& r, float tMin, float tMax) const
    {
        for (int a = 0; a < 3; ++a) {
            float invDirection = 1 / r.direction[a];
            float t0 = (min()[a] - r.origin[a]) * invDirection;
            float t1 = (max()[a] - r.origin[a]) * invDirection;
            if (invDirection < 0.f) {
                std::swap(t0, t1);
            }
            tMin = mathx::max(tMin, t0);
            tMax = mathx::min(tMax, t1);
            if (tMax <= tMin) {
                return false;
            }
            return true;
        }
        return false;
    }
    void expandToInclude(const vec3& p)
    {
        _min[0] = mathx::min(_min.x(), p.x());
        _min[1] = mathx::min(_min.y(), p.y());
        _min[2] = mathx::min(_min.z(), p.z());
        _max[0] = mathx::max(_max.x(), p.x());
        _max[1] = mathx::max(_max.y(), p.y());
        _max[2] = mathx::max(_max.z(), p.z());
    };
    void expandToInclude(const aabb& b)
    {
        _min[0] = mathx::min(_min.x(), b.min().x());
        _min[1] = mathx::min(_min.y(), b.min().y());
        _min[2] = mathx::min(_min.z(), b.min().z());
        _max[0] = mathx::max(_max.x(), b.max().x());
        _max[1] = mathx::max(_max.y(), b.max().y());
        _max[2] = mathx::max(_max.z(), b.max().z());
    };
    unsigned int maxDimension() const
    {
        vec3 e = extent();
        unsigned int result = 0;
        if (e.x() > e.y()) {
            if (e.z() > e.x()) {
                result = 2;
            }
        } else { // e.y > e.x
            if (e.y() > e.z()) {
                result = 1;
            } else {
                result = 2;
            }
        }
        return result;
    }
    static aabb surroundingBox(const aabb& b1, const aabb& b2)
    {
        vec3 min(mathx::min(b1.min().x(), b2.min().x()), mathx::min(b1.min().y(), b2.min().y()),
                 mathx::min(b1.min().z(), b2.min().z()));
        vec3 max(mathx::max(b1.max().x(), b2.max().x()), mathx::max(b1.min().y(), b2.max().y()),
                 mathx::max(b1.max().z(), b2.max().z()));
        return aabb(min, max);
    }

    vec3 _min;
    vec3 _max;
};

#endif