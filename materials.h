#ifndef MATERIALS_H
#define MATERIALS_H

#include "hitable.h"
#include "material.h"

class lambertian : public material
{
  public:
    lambertian(const vec3& albedo) : albedo(albedo){};
    virtual bool scatter(const ray& incoming, const hitRecord& rec, vec3& attenuation,
                         ray& scattered) const
    {
        vec3 target = rec.point + rec.normal + myRandom::nextInUnitSphere();
        scattered = ray(rec.point, target - rec.point);
        attenuation = albedo;
        return true;
    };

    vec3 albedo;
};

class metal : public material
{
  public:
    metal(const vec3& albedo, float fuzz) : albedo(albedo), fuzz(std::min(1.f, fuzz)){};
    virtual bool scatter(const ray& incoming, const hitRecord& rec, vec3& attenuation,
                         ray& scattered) const
    {
        vec3 reflected = material::reflect(incoming.direction.normalized(), rec.normal);
        scattered = ray(rec.point, reflected + fuzz * myRandom::nextInUnitSphere());
        attenuation = albedo;
        return vec3::dot(scattered.direction, rec.normal) > 0.f;
    };

    vec3 albedo;
    float fuzz;
};

class dielectric : public material
{
  public:
    dielectric(const vec3& mask, float refIdx) : mask(mask), refIdx(refIdx){};
    virtual bool scatter(const ray& incoming, const hitRecord& rec, vec3& attenuation,
                         ray& scattered) const
    {
        vec3 outwardNormal;
        vec3 reflected = material::reflect(incoming.direction, rec.normal);
        float niOverPoint;
        vec3 refracted;
        float cosine;
        float reflectProbability;
        attenuation = mask;
        if (vec3::dot(incoming.direction, rec.normal) > 0) {
            outwardNormal = -rec.normal;
            niOverPoint = refIdx;
            cosine = vec3::dot(incoming.direction, rec.normal) / incoming.direction.length();
            cosine = sqrtf(1 - refIdx * refIdx * (1 - cosine * cosine));
        } else {
            outwardNormal = rec.normal;
            niOverPoint = 1.0f / refIdx;
            cosine = -vec3::dot(incoming.direction, rec.normal) / incoming.direction.length();
        }
        if (material::refract(incoming.direction, outwardNormal, niOverPoint, refracted)) {
            reflectProbability = material::schlick(cosine, refIdx);
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

    vec3 mask;
    float refIdx;
};

#endif