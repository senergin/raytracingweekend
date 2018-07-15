#ifndef DIELECTRIC_H
#define DIELECTRIC_H

#include "algorithm"
#include "material.h"
#include "myRandom.h"

class dielectric : public material
{
  public:
    dielectric(const vec3& albedo, float refIdx) : albedo(albedo), refIdx(refIdx){};
    virtual bool scatter(const ray& incoming, const hitRecord& rec, vec3& attenuation,
                         ray& scattered) const
    {
        vec3 outwardNormal;
        vec3 reflected = reflect(incoming.direction, rec.normal);
        float niOverPoint;
        vec3 refracted;
        float cosine;
        float reflectProbability;
        attenuation = albedo;
        if (vec3::dot(incoming.direction, rec.normal) > 0) {
            outwardNormal = -rec.normal;
            niOverPoint = refIdx;
            cosine = vec3::dot(incoming.direction, rec.normal) / incoming.direction.length();
            cosine = std::sqrt(1 - refIdx * refIdx * (1 - cosine * cosine));
        } else {
            outwardNormal = rec.normal;
            niOverPoint = 1.0f / refIdx;
            cosine = -vec3::dot(incoming.direction, rec.normal) / incoming.direction.length();
        }
        if (refract(incoming.direction, outwardNormal, niOverPoint, refracted)) {
            reflectProbability = schlick(cosine, refIdx);
        } else {
            // scattered = ray(rec.point, reflected);
            reflectProbability = 1.f;
        }
        if (myRandom::next() < reflectProbability) {
            scattered = ray(rec.point, reflected);
        } else {
            scattered = ray(rec.point, refracted);
        }
        return true;
    };

  private:
    vec3 reflect(const vec3& incoming, const vec3& normal) const
    {
        return incoming - 2 * vec3::dot(incoming, normal) * normal;
    };
    bool refract(const vec3& incoming, const vec3& normal, float niOverPoint, vec3& refracted) const
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
    float schlick(float cosine, float refIdx) const
    {
        float r0 = (1 - refIdx) / (1 + refIdx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * std::pow((1 - cosine), 5);
    }

    vec3 albedo;
    float refIdx;
};

#endif