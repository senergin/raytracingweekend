#ifndef MATHX_H
#define MATHX_H

namespace mathx
{
const static float pi = 3.1415926535897932f;
const static float deg2rad = pi / 180;
const static float rad2deg = 180 / pi;
const static float golden = 1.61803398875f;
const static float epsilon = 0.00001f;
inline float min(float a, float b) { return a < b ? a : b; }
inline float max(float a, float b) { return a > b ? a : b; }
} // namespace mathx

#endif