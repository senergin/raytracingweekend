#ifndef MYRANDOM_H
#define MYRANDOM_H

#include "mathx.h"
#include <math.h>
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

        float theta = std::acos(costheta);
        float r = std::cbrtf(volume);
        return r * vec3(std::sin(theta) * std::cos(phi), std::sin(theta) * std::sin(phi),
                        std::cos(theta));
    };
    static vec3 nextInUnitSphere_rand()
    {
        vec3 p;
        do {
            p = 2 * vec3(next(), next(), next()) - vec3(1.f, 1.f, 1.f);
        } while (p.squaredLength() >= 1.0f);
        return p;
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