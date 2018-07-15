#ifndef MATERIAL_H
#define MATERIAL_H

#include "myRandom.h"

class hitRecord;

class material
{
  public:
    virtual bool scatter(const ray& incoming, const hitRecord& rec, vec3& attuenation,
                         ray& scattered) const = 0;

  protected:
    static vec3 reflect(const vec3& incoming, const vec3& normal)
    {
        return incoming - 2 * vec3::dot(incoming, normal) * normal;
    };
    static bool refract(const vec3& incoming, const vec3& normal, float niOverPoint,
                        vec3& refracted)
    {
        vec3 uv = incoming.normalized();
        float dt = vec3::dot(uv, normal);
        float discriminant = 1.0f - ((niOverPoint * niOverPoint) * (1 - dt * dt));
        if (discriminant > 0) {
            refracted = niOverPoint * (uv - normal * dt) - normal * std::sqrt(discriminant);
            return true;
        }
        return false;
    };
    static float schlick(float cosine, float refIdx)
    {
        float r0 = (1 - refIdx) / (1 + refIdx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * std::pow((1 - cosine), 5);
    }
};

#endif