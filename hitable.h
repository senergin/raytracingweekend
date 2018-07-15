#ifndef HITABLE_H
#define HITABLE_H

#include "material.h"
#include "vec3.h"

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

class sphere : public hitable
{
  public:
    sphere(){};
    ~sphere() { delete mat; };
    sphere(vec3 center, float radius, material* mat) : center(center), radius(radius), mat(mat){};
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
                rec.mat = mat;
                return true;
            }
            temp = (-b + sqrt(b * b - a * c) / a);
            if (temp < tMax && temp > tMin) {
                rec.distance = temp;
                rec.point = r.getPoint(rec.distance);
                rec.normal = (rec.point - center) / radius;
                rec.mat = mat;
                return true;
            }
        }
        return false;
    };

    vec3 center;
    float radius;
    material* mat;
};

class hitableList : public hitable
{
  public:
    hitableList(){};
    hitableList(hitable** list, unsigned int count) : list(list), count(count){};
    virtual bool hit(const ray& r, float tMin, float tMax, hitRecord& rec) const
    {
        hitRecord temp;
        bool hitAnything = false;
        float closest = tMax;
        for (int i = 0; i < count; ++i) {
            hitable* h = list[i];
            bool isHit = h->hit(r, tMin, closest, temp);
            if (isHit) {
                closest = temp.distance;
                rec = temp;
                hitAnything = true;
            }
        }
        return hitAnything;
    }

    hitable** list;
    unsigned int count;
};

#endif