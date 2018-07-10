#ifndef METAL_H
#define METAL_H

#include "algorithm"
#include "material.h"
#include "myRandom.h"

class metal : public material
{
  public:
    metal(const vec3& albedo, float fuzz) : albedo(albedo), fuzz(std::min(1.f, fuzz)){};
    virtual bool scatter(const ray& incoming, const hitRecord& rec, vec3& attenuation,
                         ray& scattered) const
    {
        vec3 reflected = reflect(incoming.direction.normalized(), rec.normal);
        scattered = ray(rec.point, reflected + fuzz * myRandom::nextInUnitSphere());
        attenuation = albedo;
        return vec3::dot(scattered.direction, rec.normal) > 0.f;
    };

  private:
    vec3 reflect(const vec3& incoming, const vec3& normal) const
    {
        return incoming - 2 * vec3::dot(incoming, normal) * normal;
    };

    vec3 albedo;
    float fuzz;
};

#endif