#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"

class camera
{
  public:
    camera()
        : upperLeftCorner(-2.f, 1.f, -1.f), horizontal(4.f, 0.f, 0.f), vertical(0.f, -2.f, 0.f),
          origin(0.f, 0.f, 0.f)
    {
    }

    ray getRay(float u, float v)
    {
        return ray(origin, upperLeftCorner + u * horizontal + v * vertical - origin);
    }

    vec3 upperLeftCorner;
    vec3 horizontal;
    vec3 vertical;
    vec3 origin;
};

#endif