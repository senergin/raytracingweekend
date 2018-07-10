#ifndef MATERIAL_H
#define MATERIAL_H

#include "hitable.h"

class material
{
  public:
    virtual bool scatter(const ray& incoming, const hitRecord& rec, vec3& attuenation,
                         ray& scattered) const = 0;
};

#endif