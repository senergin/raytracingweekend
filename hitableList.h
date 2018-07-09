#ifndef HITABLELIST_H
#define HITABLELIST_H

#include "hitable.h"

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