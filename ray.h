#ifndef RAY_H
#define RAY_H

#include "vec3.h"

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