#ifndef MYRANDOM_H
#define MYRANDOM_H

#include "mathx.h"
#include "vec3.h"
#include <random>

class myRandom
{
  public:
    static float next() { return dist1(e2); };
    static float nextCostheta() { return distCostheta(e2); }
    static float nextPhi() { return distPhi(e2); }
    static vec3 nextInUnitSphere()
    {
        float phi = nextPhi();
        float costheta = nextCostheta();
        float volume = next();

        float theta = acos(costheta);
        float r = cbrtf(volume);
        return r * vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
    };
    static vec3 nextInUnitDiskXY()
    {
        float phi = nextPhi();
        float area = next();
        float r = sqrtf(area);
        return r * vec3(cos(phi), sin(phi), 0.f);
    };

  private:
    static std::random_device rd;
    static std::mt19937 e2;
    static std::uniform_real_distribution<float> dist1;
    static std::uniform_real_distribution<float> distCostheta;
    static std::uniform_real_distribution<float> distPhi;
};

std::random_device myRandom::rd;
std::mt19937 myRandom::e2 = std::mt19937(myRandom::rd());
std::uniform_real_distribution<float> myRandom::dist1 = std::uniform_real_distribution<float>(0, 1);
std::uniform_real_distribution<float> myRandom::distCostheta =
    std::uniform_real_distribution<float>(-1, 1);
std::uniform_real_distribution<float> myRandom::distPhi =
    std::uniform_real_distribution<float>(0, 2 * mathx::pi);

#endif