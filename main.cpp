#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "ray.h"
#include "stb_image_write.h"
#include "vec3.h"
#include <cstdint>
#include <iostream>

bool hitSphere(const vec3& center, float radius, const ray& r)
{
    vec3 oc = r.origin - center;
    float a = vec3::dot(r.direction, r.direction);
    float b = 2.f * vec3::dot(oc, r.direction);
    float c = vec3::dot(oc, oc) - (radius * radius);
    float discriminant = (b * b) - (4 * a * c);
    return discriminant > 0;
}
vec3 backgroundColor(const ray& r)
{
    vec3 unit = r.direction;
    float t1 = 0.5f - (0.5f * unit.y);
    float t2 = 0.5f + (0.5f * unit.y);
    return t1 * vec3(1.f, 1.f, 1.f) + t2 * vec3(0.5f, 0.7f, 1.f);
}
vec3 color(const ray& r)
{
    if (hitSphere(vec3(0, 0, -1), 0.5f, r)) {
        return vec3(1.f, 0.f, 0.f);
    }
    return backgroundColor(r);
}
int main()
{
    const int channels = 4; // STBI_rgb_alpha
    const int w = 400;
    const int h = 200;
    // Create image data
    unsigned char* data = new unsigned char[w * h * channels];

    vec3 upperLeftCorner(-2.f, 1.f, -1.f);
    vec3 horizontal(4.f, 0.f, 0.f);
    vec3 vertical(0.f, -2.f, 0.f);
    vec3 origin(0.f, 0.f, 0.f);

    for (int j = 0; j < h; ++j) {
        for (int i = 0; i < w; ++i) {
            float u = float(i) / float(w);
            float v = float(j) / float(h);
            ray r(origin, upperLeftCorner + u * horizontal + v * vertical);
            vec3 col = color(r);

            int index = ((j * w) + i) * channels;
            data[index + 0] = (unsigned char)(col.x * 255.99f);
            data[index + 1] = (unsigned char)(col.y * 255.99f);
            data[index + 2] = (unsigned char)(col.z * 255.99f);
            data[index + 3] = (unsigned char)255;
        }
    }

    int ret = stbi_write_png("out.png", w, h, channels, data, channels * w);
    if (ret == 0) {
        std::cout << "problem at stbi_write_png" << std::endl;
    }
    return 0;
}