#ifndef HITABLE_H
#define HITABLE_H

#include "aabb.h"
#include "material.h"
#include "vec3.h"
#include <chrono>

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
    virtual aabb boundingBox() const = 0;
    virtual vec3 centeroid() const = 0;
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
    virtual aabb boundingBox() const
    {
        return aabb(center - vec3(radius, radius, radius), center + vec3(radius, radius, radius));
    };
    virtual vec3 centeroid() const { return center; }
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
    virtual aabb boundingBox() const
    {
        vec3 min(mathx::min(mathx::min(p1.x(), p2.x()), p3.x()),
                 mathx::min(mathx::min(p1.y(), p2.y()), p3.y()),
                 mathx::min(mathx::min(p1.z(), p2.z()), p3.z()));
        vec3 max(mathx::max(mathx::max(p1.x(), p2.x()), p3.x()),
                 mathx::max(mathx::max(p1.y(), p2.y()), p3.y()),
                 mathx::max(mathx::max(p1.z(), p2.z()), p3.z()));
        return aabb(min, max);
    };
    // THIS IS NOT CORRECT
    virtual vec3 centeroid() const { return (p1 + p2 + p3 / 3); }
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
        auto t1 = std::chrono::high_resolution_clock::now();
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
        auto t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
        std::printf("Inside: Hit duration: %u. IsHit: %u.\n", duration, hitAnything);
        return hitAnything;
    }
    virtual aabb boundingBox() const
    {
        if (count == 0) {
            // I might actually need to return false
            return aabb();
        }
        aabb box = list[0]->boundingBox();
        for (int i = 1; i < count; ++i) {
            box = aabb::surroundingBox(box, list[i]->boundingBox());
        }
        return box;
    };
    virtual vec3 centeroid() const
    {
        if (count == 0) {
            // I might actually need to return false
            return vec3();
        }
        vec3 sum = list[0]->centeroid();
        for (int i = 1; i < count; ++i) {
            sum += list[i]->centeroid();
        }
        return sum / count;
    }
    hitable** list;
    unsigned int count;
};

class bvhNode : public hitable
{
  public:
    bvhNode(bool isRoot = false) : left(nullptr), right(nullptr), box(), isRoot(isRoot){};
    bvhNode(hitable** list, int count, bool isRoot = false) : isRoot(isRoot)
    {
        int axis(myRandom::next() * 3);
        switch (axis) {
            case 0: // x
                qsort(list, count, sizeof(hitable*), bvhNode::aabbXCompare);
                break;
            case 1: // y
                qsort(list, count, sizeof(hitable*), bvhNode::aabbYCompare);
                break;
            case 2: // z
                qsort(list, count, sizeof(hitable*), bvhNode::aabbZCompare);
                break;
            default: // shouldn't
                qsort(list, count, sizeof(hitable*), bvhNode::aabbXCompare);
                break;
        }
        if (count == 1) {
            left = list[0];
            right = nullptr;
            box = left->boundingBox();
        } else if (count == 2) {
            left = list[0];
            right = list[1];
            box = aabb::surroundingBox(left->boundingBox(), right->boundingBox());
        } else {
            left = new bvhNode(list, (count / 2));
            right = new bvhNode(list + (count / 2), count - (count / 2));
            box = aabb::surroundingBox(left->boundingBox(), right->boundingBox());
        }
    }
    ~bvhNode()
    {
        if (left != nullptr) {
            delete left;
        }
        if (right != nullptr) {
            delete right;
        }
    }
    virtual bool hit(const ray& r, float tMin, float tMax, hitRecord& rec) const
    {
        bool isHit = false;
        std::chrono::time_point<std::chrono::high_resolution_clock> t1, t2;
        if (isRoot) {
            t1 = std::chrono::high_resolution_clock::now();
        }
        if (box.hit(r, tMin, tMax)) {
            hitRecord lRecord, rRecord;
            bool lHit = left != nullptr && left->hit(r, tMin, tMax, lRecord);
            bool rHit = right != nullptr && right->hit(r, tMin, tMax, rRecord);
            if (lHit && rHit) {
                rec = lRecord.distance < rRecord.distance ? lRecord : rRecord;
                isHit = true;
            } else if (lHit) {
                rec = lRecord;
                isHit = true;
            } else if (rHit) {
                rec = rRecord;
                isHit = true;
            }
        }
        if (isRoot) {
            t2 = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
            std::printf("Inside: Hit duration: %u. IsHit: %u.\n", duration, isHit);
        }
        return isHit;
    }
    virtual aabb boundingBox() const { return box; }
    virtual vec3 centeroid() const { return (box.max() + box.min()) / 2.f; }

    static int aabbXCompare(const void* a, const void* b)
    {
        hitable* aHitable = *(hitable**)a;
        hitable* bHitable = *(hitable**)b;
        return aHitable->boundingBox().min().x() < bHitable->boundingBox().min().x() ? 1 : -1;
    }
    static int aabbYCompare(const void* a, const void* b)
    {
        hitable* aHitable = *(hitable**)a;
        hitable* bHitable = *(hitable**)b;
        return aHitable->boundingBox().min().y() < bHitable->boundingBox().min().y() ? 1 : -1;
    }
    static int aabbZCompare(const void* a, const void* b)
    {
        hitable* aHitable = *(hitable**)a;
        hitable* bHitable = *(hitable**)b;
        return aHitable->boundingBox().min().z() < bHitable->boundingBox().min().z() ? 1 : -1;
    }

    hitable* left;
    hitable* right;
    aabb box;
    bool isRoot;
};

#endif