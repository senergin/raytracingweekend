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
            float t = (-b - sqrtf(b * b - a * c)) / a;
            if (t < tMax && t > tMin) {
                rec.distance = t;
                rec.point = r.getPoint(rec.distance);
                rec.normal = (rec.point - center) / radius;
                rec.mat = mat;
                return true;
            }
            t = (-b + sqrtf(b * b - a * c) / a);
            if (t < tMax && t > tMin) {
                rec.distance = t;
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

class triangle : public hitable
{
  public:
    triangle(){};
    ~triangle() { delete mat; };
    triangle(vec3 p1, vec3 p2, vec3 p3, material* mat) : p1(p1), p2(p2), p3(p3), mat(mat){};
    virtual bool hit(const ray& r, float tMin, float tMax, hitRecord& rec) const
    {
        vec3 p1p2 = p2 - p1;
        vec3 p1p3 = p3 - p1;
        vec3 pvec = vec3::cross(r.direction, p1p3);
        float det = vec3::dot(p1p2, pvec);
        // Backface culling
        // if the determinant is negative the triangle is backfacing
        // if the determinant is close to 0, the ray misses the triangle
        if (det < mathx::epsilon) {
            return false;
        }
        // ray and triangle are parallel if det is close to 0
        if (fabs(det) < mathx::epsilon) {
            return false;
        }
        float invDet = 1 / det;
        vec3 tvec = r.origin - p1;
        float u = vec3::dot(tvec, pvec) * invDet;
        if (u < 0 || u > 1) {
            return false;
        }
        vec3 qvec = vec3::cross(tvec, p1p2);
        float v = vec3::dot(r.direction, qvec) * invDet;
        if (v < 0 || u + v > 1) {
            return false;
        }
        float t = vec3::dot(p1p3, qvec) * invDet;
        if (t > tMax || t < tMin) {
            return false;
        }

        rec.distance = t;
        rec.point = r.getPoint(rec.distance);
        rec.normal = vec3::cross(p1p2, p1p3).normalized();
        if (vec3::dot(r.direction, rec.normal) > 0) {
            rec.normal = -rec.normal;
        }
        rec.mat = mat;
        return true;
    };

    vec3 p1;
    vec3 p2;
    vec3 p3;
    material* mat;
};

class hitableList : public hitable
{
  public:
    hitableList(){};
    ~hitableList()
    {
        for (unsigned int i = 0; i < count; ++i) {
            delete list[i];
        }
        delete[] list;
    };
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