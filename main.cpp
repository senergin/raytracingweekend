#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "hitableList.h"
#include "ray.h"
#include "sphere.h"
#include "stb_image_write.h"
#include <cstdint>
#include <iostream>

vec3 backgroundColor(const ray& r)
{
    vec3 unit = r.direction;
    float t1 = 0.5f - (0.5f * unit.y);
    float t2 = 0.5f + (0.5f * unit.y);
    return t1 * vec3(1.f, 1.f, 1.f) + t2 * vec3(0.5f, 0.7f, 1.f);
}
vec3 color(const ray& r, const hitable* world, const float minDistance, const float maxDistance)
{
    hitRecord rec;
    bool isHit = world->hit(r, minDistance, maxDistance, rec);
    if (isHit) {
        return 0.5f * (rec.normal + 1);
    }
    return backgroundColor(r);
}
int main()
{
    const int w = 400;
    const int h = 200;
    const float minDistance = 0.f;
    const float maxDistance = 10000.f;
    // Output image data
    const int channels = 4; // STBI_rgb_alpha
    unsigned char* data = new unsigned char[w * h * channels];

    vec3 upperLeftCorner(-2.f, 1.f, -1.f);
    vec3 horizontal(4.f, 0.f, 0.f);
    vec3 vertical(0.f, -2.f, 0.f);
    vec3 origin(0.f, 0.f, 0.f);

    hitable* list[2];
    list[0] = new sphere(vec3(0, 0, -1), 0.5f);
    list[1] = new sphere(vec3(0, -100.5f, -1), 100);
    hitable* world = new hitableList(list, 2);

    for (int j = 0; j < h; ++j) {
        for (int i = 0; i < w; ++i) {
            float u = float(i) / float(w);
            float v = float(j) / float(h);
            ray r(origin, upperLeftCorner + u * horizontal + v * vertical);
            vec3 col = color(r, world, minDistance, maxDistance);

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

    delete world;
    delete list[0];
    delete list[1];

    return 0;
}