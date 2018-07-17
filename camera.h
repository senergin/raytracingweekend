#ifndef CAMERA_H
#define CAMERA_H

#include "myRandom.h"
#include "vec3.h"

class camera
{
  public:
    camera(vec3 lookFrom, vec3 lookAt, vec3 up, float fov, float aspectRatio, float aperture,
           float focusDistance)
    {
        this->fov = fov;
        this->aspectRatio = aspectRatio;
        this->lensRadius = aperture / 2;
        this->focusDistance = focusDistance;

        float theta = this->fov * mathx::pi / 180;
        float halfH = tan(theta / 2);
        float halfW = this->aspectRatio * halfH;

        this->origin = lookFrom;
        this->direction = (lookAt - lookFrom).normalized();
        this->right = vec3::cross(this->direction, up).normalized();
        this->up = vec3::cross(this->right, this->direction).normalized();

        this->upperLeftCorner = this->origin + (-halfW * this->focusDistance * this->right) +
                                (halfH * this->focusDistance * this->up) + this->direction;
        this->horizontal = 2 * halfW * this->focusDistance * right;
        this->vertical = -2 * halfH * this->focusDistance * up;
    }

    ray getRay(float u, float v)
    {
        vec3 rd = lensRadius * myRandom::nextInUnitDiskXY();
        vec3 offset = right * rd.x + up * rd.y;
        vec3 from = origin + offset;
        vec3 to = upperLeftCorner + u * horizontal + v * vertical;
        return ray(from, to - from);
    }

    vec3 origin;
    vec3 direction;
    vec3 up;
    vec3 right;
    float fov;
    float aspectRatio;
    float lensRadius;
    float focusDistance;
    vec3 upperLeftCorner;
    vec3 horizontal;
    vec3 vertical;
};

#endif