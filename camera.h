#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"

class camera
{
  public:
    camera(vec3 lookFrom, vec3 lookAt, vec3 up, float fov, float aspectRatio)
        : origin(lookFrom), direction((lookAt - lookFrom).normalized()),
          right(vec3::cross(direction, up).normalized()),
          up(vec3::cross(right, direction).normalized())
    {
        float theta = fov * mathx::pi / 180;
        float halfH = std::tan(theta / 2);
        float halfW = aspectRatio * halfH;
        upperLeftCorner = origin - halfW * right + halfH * up + direction;
        horizontal = 2 * halfW * right;
        vertical = -2 * halfH * up;
    }

    ray getRay(float u, float v)
    {
        return ray(origin, upperLeftCorner + u * horizontal + v * vertical - origin);
    }

    vec3 origin;
    vec3 direction;
    vec3 up;
    vec3 right;
    vec3 upperLeftCorner;
    vec3 horizontal;
    vec3 vertical;
};

#endif