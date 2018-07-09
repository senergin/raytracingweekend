#ifndef SPHERE_H
#define SPHERE_H

#include "hitable.h"

class sphere : public hitable
{
  public:
    sphere(){};
    sphere(vec3 center, float radius) : center(center), radius(radius){};
    virtual bool hit(const ray& r, float tMin, float tMax, hitRecord& rec) const
    {
        vec3 oc = r.origin - center;
        float a = vec3::dot(r.direction, r.direction);
        float b = vec3::dot(oc, r.direction);
        float c = vec3::dot(oc, oc) - radius * radius;
        float discriminant = b * b - a * c;
        if (discriminant > 0) {
            float temp = (-b - sqrt(b * b - a * c)) / a;
            if (temp < tMax && temp > tMin) {
                rec.distance = temp;
                rec.point = r.getPoint(rec.distance);
                rec.normal = (rec.point - center) / radius;
                return true;
            }
            temp = (-b + sqrt(b * b - a * c) / a);
            if (temp < tMax && temp > tMin) {
                rec.distance = temp;
                rec.point = r.getPoint(rec.distance);
                rec.normal = (rec.point - center) / radius;
                return true;
            }
        }
        return false;
    };

    vec3 center;
    float radius;
};

#endif