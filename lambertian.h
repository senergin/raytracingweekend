#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include "material.h"
#include "myRandom.h"

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

#endif