#ifndef HITABLE_H
#define HITABLE_H

#include "ray.h"

class material;

struct hitRecord {
    float distance;
    vec3 point;
    vec3 normal;
    material* mat;
};

class hitable
{
  public:
    virtual bool hit(const ray& r, float tMin, float tMax, hitRecord& rec) const = 0;
};

#endif