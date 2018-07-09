#ifndef MYRANDOM_H
#define MYRANDOM_H

#include <random>

class myRandom
{
  public:
    myRandom()
        : rd(), e2(rd()), dist(0, 1) // Range: [0, 1)
          {};
    float next() { return dist(e2); };

    std::random_device rd;
    std::mt19937 e2;
    std::uniform_real_distribution<float> dist;
};

#endif